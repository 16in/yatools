#include "convert.h"
#include "refchar.h"
#include <vector>
#include <shlwapi.h>
#include <aafile/AAFileAccessor.h>
#include <yatools.image.file.h>

namespace mlt2png
{

typedef std::vector<LOGFONTA> LogFontList;


//-------------------------------------
// 列挙用コールバック
//-------------------------------------
// フォント列挙
int CALLBACK callbackEnumFontFamilies( const LOGFONTA* logFont, const TEXTMETRICA* textMetric, DWORD FontType, LPARAM lParam )
{
	LogFontList* list = (LogFontList*)lParam;
	if( list != NULL )
	{
		list->push_back( *logFont );
	}
	return TRUE;
}


//-------------------------------------
// 静的メンバ変数
//-------------------------------------
HFONT	Converter::m_hFont = NULL;							//!< フォントハンドル
int32_t	Converter::m_CharacterWidth[ 0x10000 ] = { 0 };		//!< 文字ごとの横幅
int32_t	Converter::m_CharacterHeight = 0;					//!< 文字縦幅


//-------------------------------------
// セットアップ/クリンナップ
//-------------------------------------
//-------------------------------------
// セットアップ
//
bool Converter::Setup( const char* fontName, int fontSize )
{
	/*---- フォント生成 ----*/
	LOGFONTA logFont;
	ZeroMemory( &logFont, sizeof( logFont ) );
	if( fontName != NULL && fontName[ 0 ] != '\0' )
	{
		strncpy( logFont.lfFaceName, fontName, LF_FACESIZE );
	}
	else
	{
		HWND hDesktop = GetDesktopWindow( );
		HDC hDC = GetDC( hDesktop );

		LogFontList enumFontList;
		EnumFontFamiliesExA( hDC, &logFont, callbackEnumFontFamilies, (LPARAM)&enumFontList, 0 );

		ReleaseDC( hDesktop, hDC );

		for( int i = 0; i < enumFontList.size( ); i++ )
		{
			if( strcmp( enumFontList[ i ].lfFaceName, "MS Pｺﾞｼｯｸ" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "MS PGothic" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "MS Pゴシック" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "ＭＳ Ｐゴシック" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "ＭＳ　Ｐゴシック" ) == 0
			)
			{
				strcpy( logFont.lfFaceName, enumFontList[ i ].lfFaceName );
				break;
			}
		}
	}


	/*---- 論理フォント初期化 ----*/
	// フォント名があるか判定
	if( logFont.lfFaceName[ 0 ] == '\0' )
	{
		return false;
	}

	// フォント詳細設定
	logFont.lfHeight = fontSize == 0 ? -16 : fontSize;


	/*---- セットアップ済みなら一旦廃棄 ----*/
	if( m_hFont != NULL )
	{
		Cleanup( );
	}


	// 共通フォント生成
	m_hFont = CreateFontIndirectA( &logFont );
	if( m_hFont != NULL )
	{
		// 文字幅テーブル更新
		HWND hWnd = GetDesktopWindow( );
		HDC hdc = GetDC( hWnd );
		HFONT hOldFont = (HFONT)SelectObject( hdc, m_hFont );

		SIZE size;
		::GetTextExtentPoint32W( hdc, L" ", 1, &size );
		m_CharacterHeight = size.cy;
		GetCharWidth32W( hdc, 0x0000, 0xffff, m_CharacterWidth );

		// DC設定戻す
		SelectObject( hdc, hOldFont );
		ReleaseDC( hWnd, hdc );
	}
	return m_hFont != NULL;
}

//-------------------------------------
// クリンナップ
//
void Converter::Cleanup( void )
{
	if( m_hFont != NULL )
	{
		DeleteObject( m_hFont );
		m_hFont = NULL;
	}
}


//-------------------------------------
// 実行
//-------------------------------------
//-------------------------------------
// PNG生成ルーチンの実行
//
void Converter::Run( const char* inputFile, const char* outputDirectory )
{
	wchar_t inPath[ MAX_PATH ];
	mbstowcs( inPath, inputFile, MAX_PATH );
	aafile::AAFile* file = aafile::AAFileReader::CreateAAFileFromFile( inPath );
	if( file != NULL )
	{
		paintAllPage( file, outputDirectory );
		aafile::AAFileReader::ReleaseAAFile( file );
	}
}

//-------------------------------------
// PNG生成ルーチンの実行
//
void Converter::RunString( const char* outputDirectory, const char* srcString, unsigned long length )
{
	size_t wlength = mbstowcs( NULL, srcString, length );
	wchar_t* wstring = new wchar_t[ wlength ];
}

//-------------------------------------
// 全ページ書き出し
//
void Converter::paintAllPage( aafile::AAFile* file, const char* outputDirectory )
{
	// 出力先生成
	if( !PathFileExistsA( outputDirectory ) || (PathFileExistsA( outputDirectory ) && !PathIsDirectoryA( outputDirectory )) )
	{
		char path[ MAX_PATH ];
		sprintf( path, "%s", outputDirectory );
		if( !CreateDirectoryA( path, NULL ) )
		{
			puts( "出力先ディレクトリの作成に失敗しました。" );
			return;
		}
	}


	HDC hMemDC = CreateCompatibleDC( NULL );
	for( uint_t i = 0; i < file->pageCount; i++ )
	{
		aafile::AAFilePage* page = &file->pageList[ i ];

		// ページサイズ取得
		int width, height, lineCount;
		getPageSize( page, width, height, lineCount );

		// ビットマップ生成
		HBITMAP hBitmap = CreateCompatibleBitmap( hMemDC, width, height );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

		paintPage( hMemDC, page, width, height, lineCount );
		yatools::image::ColorBuffer* buffer = yatools::image::Windows::GetColorBuffer( hMemDC, (uint32_t)width, (uint32_t)height );

		// ビットマップ開放
		SelectObject( hMemDC, hOldBitmap );
		DeleteObject( hBitmap );


		uint64_t pngSize = yatools::image::file::PngFile::WriteFileBuffer( buffer, NULL, 0 );
		uint8_t* pngBuffer = new uint8_t[ pngSize ];
		yatools::image::file::PngFile::WriteFileBuffer( buffer, (uintptr_t)pngBuffer, pngSize );
		buffer->Release( );


		char path[ MAX_PATH ];
		sprintf( path, "%s/%05d.png", outputDirectory, i );
		FILE* fp = fopen( path, "wb" );
		fwrite( pngBuffer, 1, pngSize, fp );
		fclose( fp );
	}
	DeleteDC( hMemDC );
}

//----------------------------------------------
// ページ書き込み
//
void Converter::paintPage( HDC hDC, aafile::AAFilePage* page, int width, int height, int lfCount )
{
	/*---- デバイスコンテキストの設定 ----*/
	HFONT hOldFont = (HFONT)SelectObject( hDC, m_hFont );
	int oldMode = SetBkMode( hDC, TRANSPARENT );


	/*---- 背景クリア ----*/
	RECT rect = { 0, 0, width, height };
	FillRect( hDC, &rect, WHITE_BRUSH );


	/*---- 一文字ずつ出力 ----*/
	std::wstring lfString = L"";
	int y = 0;
	for( unsigned long pos = 0, step = 1; pos < page->valueLength; pos += step )
	{
		bool lf = false;
		step = 1;

		// 改行チェック
		if( page->value[ pos ] == L'\r' )
		{
			lf = true;
			if( page->value[ pos + 1 ] == L'\n' ) step = 2;
		}
		else if( page->value[ pos ] == L'\n' )
		{
			lf = true;
		}

		if( !lf )
		{
			/*---- 表示文字列取得 ----*/
			wchar_t ch[ 4 ] = { 0 };
			getCharWidth( &page->value[ pos ], page->valueLength, step, ch );
			lfString += ch;
		}
		else
		{
			/*-- ライン出力 --*/
			TextOutW( hDC, 0, y, lfString.c_str( ), (int)lfString.size( ) );
			lfString.clear( );
			y += m_CharacterHeight;
		}
	}
	TextOutW( hDC, 0, y, lfString.c_str( ), (int)lfString.size( ) );


	/*---- デバイスコンテキストの設定 ----*/
	SelectObject( hDC, hOldFont );
}

//----------------------------------------------
// 頁サイズを計算
//
void Converter::getPageSize( aafile::AAFilePage* page, int& width, int& height, int& lfCount )
{
	/*---- 状態を初期化 ----*/
	lfCount = 1;
	height = width = 0;


	/*---- 文字幅を計算 ----*/
	int lineWidth = 0;
	for( unsigned long pos = 0, step = 1; pos < page->valueLength; pos += step )
	{
		bool lf = false;
		step = 1;

		// 改行チェック
		if( page->value[ pos ] == L'\r' )
		{
			lf = true;
			if( page->value[ pos + 1 ] == L'\n' ) step = 2;;
		}
		else if( page->value[ pos ] == L'\n' )
		{
			lf = true;
		}

		if( !lf )
		{
			lineWidth += getCharWidth( &page->value[ pos ], page->valueLength, step );
		}
		else
		{
			if( width < lineWidth ) width = lineWidth;
			lineWidth = 0;
			lfCount++;
		}
	}
	if( width < lineWidth ) width = lineWidth;


	/*---- 高さを計算 ----*/
	height = lfCount * m_CharacterHeight;
}


//----------------------------------------------
// 先頭文字の文字幅と文字コードを取得
//
int Converter::getCharWidth( const wchar_t* str, size_t length, unsigned long& step, wchar_t* ch )
{
	int ret = 0;
	wchar_t refChar[ 2 ];
	unsigned long refCount;

	if( !CheckRefChar( str, (unsigned long)length, refChar, step, refCount ) )
	{// 参照文字解決しないもの
		step = 1;
		ret = m_CharacterWidth[ str[ 0 ] ];
		if( ch ) *ch = str[ 0 ];
	}
	else
	{// 参照文字解決済みのもの
		if( refCount == 1 )
		{// 非サロゲート文字
			ret = m_CharacterWidth[ refChar[ 0 ] ];
			if( ch ) *ch = refChar[ 0 ];
		}
		else
		{// サロゲート文字…だけど、どうしよう
			if( ch )
			{
				ch[ 0 ] = refChar[ 0 ];
				ch[ 1 ] = refChar[ 1 ];
			}
		}
	}

	return ret;
}


//----------------------------------------------------------
// 参照文字列かチェックし、参照文字列ならば参照を解決する
//
bool Converter::CheckRefChar( const wchar_t* str, unsigned long length, wchar_t refChar[ 2 ], unsigned long& step, unsigned long& refCount )
{
	bool ret = false;

	if( str[ 0 ] == L'&' && length >= 2 )
	{// 参照文字列
		if( str[ 1 ] != L'#' )
		{// 実体参照
			for( size_t i = 0; sg_RefCharList[ i ].refString != NULL; i++ )
			{
				size_t n = wcslen( sg_RefCharList[ i ].refString );
				if( length >= n )
				{
					if( _wcsnicmp( str, sg_RefCharList[ i ].refString, n ) == 0 )
					{
						refChar[ 0 ] = sg_RefCharList[ i ].refChar;
						step = (unsigned long)n;
						refCount = 1;
						ret = true;
						break;
					}
				}
			}
		}
		else
		{// 数値参照
			wchar_t*		endPtr;
			unsigned long	code;

			if( towlower( str[ 2 ] ) != L'x' )
			{// 10進数参照
				code = wcstol( &str[ 2 ], &endPtr, 10 );
			}
			else
			{// 16進数参照
				code = wcstol( &str[ 3 ], &endPtr, 16 );
			}

			if( endPtr[ 0 ] == L';' )
			{// 参照文字列であった
				if( code <= 0xffff )
				{
					refChar[ 0 ] = (wchar_t)code;
					refCount = 1;
				}
				else
				{
					unsigned long X = code - 0x10000;
					refChar[ 0 ] = (wchar_t)((X / 0x400) + 0xD800);
					refChar[ 1 ] = (wchar_t)((X % 0x400) + 0xDC00);
					refCount = 2;
				}
				step = (unsigned long)(endPtr - str + 1);
				ret = true;
			}
		}
	}

	return ret;
}



}