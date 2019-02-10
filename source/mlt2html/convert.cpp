/*--------------------------------------------------------------------------------------------
*
* MLT2HTML  コンバータ
*
*---------------------------------------------------------------------------------------------*/
/*-- 更新履歴 --------------------------------------------------------------------------------
*
*	2012/12/05
*		コンバータ実装 ver1.0
*
*---------------------------------------------------------------------------------------------*/
#include "convert.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <aafile/AAFileAccessor.h>
#include <shlwapi.h>


namespace convert
{


/*--------------------------------------------------------------------------
* 定数定義
*/
static const int MAX_VARIABLE_NAME = 128;						//!< template変数名の最大文字数
	

/*--------------------------------------------------------------------------
* 列挙体定義
*/
/** template変数 */
enum TemplateVariableType
{
	TV_FILENAME = 0x00,				//!< ファイル名
	TV_FILESIZE,					//!< 全ページ総容量
	TV_PAGECOUNT,					//!< ファイル総ページ数
	TV_PAGENUM,						//!< 現在のページ数
	TV_TIME,						//!< 現在時刻

	TV_MAX							//!< 最大数
};


/*--------------------------------------------------------------------------
* 構造体定義
*/
//! templateの要素
struct TemplateElement
{
	int				type;			//!< 0:文字列 1:置き換え変数
	std::string		str;			//!< 文字列内容
	int				varType;		//!< 変数のタイプ
};
typedef std::vector<TemplateElement>	TemplateList;

//! template変数
struct TemplateVariable
{
	const char*		filePath;		//!< ファイルパス
	size_t			fileSize;		//!< 処理済のページデータ総量
	size_t			pageCount;		//!< ファイル総ページ数
	size_t			pageNum;		//!< 処理しているページ番号
};


/*--------------------------------------------------------------------------
* ファイルスコープグローバル変数定義
*/
//-- template
static TemplateList			sg_HeaderTemplate;
static TemplateList			sg_FooterTemplate;
static TemplateList			sg_PageHeaderTemplate;
static TemplateList			sg_PageFooterTemplate;

//-- template変数
static const char*			sg_TemplateVariableName[] =
{
	"FILE_NAME",			// TV_FILENAME
	"FILE_SIZE",			// TV_FILESIZE
	"PAGE_COUNT",			// TV_PAGECOUNT
	"PAGE_NUM",				// TV_PAGENUM
	"TIME",					// TV_TIME

	NULL
};
static TemplateVariable		sg_TemplateVariable;


/*--------------------------------------------------------------------------
* ファイルスコープ関数宣言
*/
//-- 実行関係
static void _runFunc( FILE* fp, aafile::AAFile* file );

//-- template関係
// ファイルヘッダのセットアップ
static void _setupheader( const char* path );

// ファイルフッタのセットアップ
static void _setupfooter( const char* path );

// ページヘッダのセットアップ
static void _setuppageheader( const char* path );

// ページフッタのセットアップ
static void _setuppagefooter( const char* path );

// templateファイルをセットアップ
static void _setuptemplate( const char* path, TemplateList& temp );

//-- ファイル出力関係
// 指定ファイルにwchar_t文字を変換して書き出す
static void _writeChar( FILE* fp, wchar_t wc );

// 指定ファイルにtemplateを書き出します
static void _writeTemplate( FILE* fp, TemplateList& temp );

// 指定ファイルにtemplate変数を書き出します
static void _writeTemplateVariable( FILE* fp, int vartype );

//-- 文字列判定
static bool _isHttpString( const wchar_t* str, size_t& count );


/*--------------------------------------------------------------------------
* 外部公開関数
*/
/**
* @brief html生成templateのセットアップ
*	$(appdir)/templateに在るtemplateファイルをセットアップします。
*
*	@param[in]	appdir					アプリケーションの在るディレクトリ
*/
void SetupTemplate( const char* appdir )
{
	char templatePath[ MAX_PATH ];
	sprintf( templatePath, "%s\\template", appdir );

	_setupheader( templatePath );
	_setupfooter( templatePath );
	_setuppageheader( templatePath );
	_setuppagefooter( templatePath );
}

/**
* @brief html生成templateのクリンナップ
*	セットアップ済みのtemplateをクリンナップします。
*/
void CleanupTemplate( void )
{
}

/**
* @brief html生成ルーチンの実行
*	セットアップ済みのtemplateと入力ファイルからhtmlファイルを生成します。
*
*	@param[in]	inputFile				入力ファイルパス
*	@param[in]	outputFile				出力ファイルパス
*/
void Run( const char* inputFile, const char* outputFile )
{
	/**** template変数をクリアする ****/
	memset( &sg_TemplateVariable, 0x00, sizeof( sg_TemplateVariable ) );


	/**** AAファイル読み込み ****/
	wchar_t inPath[ MAX_PATH ];
	mbstowcs( inPath, inputFile, MAX_PATH );
	aafile::AAFile* file = aafile::AAFileReader::CreateAAFileFromFile( inPath );
	if( file )
	{
		// template変数構築
		char fileName[ MAX_PATH ] = { 0 };
		if( strcpy( fileName, PathFindFileNameA( inputFile ) ) != NULL )
		{
			PathRemoveExtensionA( fileName );
		}
		sg_TemplateVariable.filePath = fileName;
		sg_TemplateVariable.pageCount = file->pageCount;


		FILE* fp;
		if( (fp = fopen( outputFile, "w" )) )
		{
			_runFunc( fp, file );
			fclose( fp );
		}

		// AAファイル廃棄
		aafile::AAFileReader::ReleaseAAFile( file );
	}
}

/**
* @brief html生成ルーチンの実行
*	セットアップ済みのtemplateと入力文字列からhtmlファイルを生成します。
*
*	@param[in]	outputFile				出力ファイルパス
*	@param[in]	srcString				入力内容文字列
*	@param[in]	length					入力内容の文字列長
*/
void RunString( const char* outputFile, const char* srcString, unsigned long length )
{
	/**** template変数をクリアする ****/
	memset( &sg_TemplateVariable, 0x00, sizeof( sg_TemplateVariable ) );


	/**** 文字列をwcharへ変換 ****/
	size_t wlen = mbstowcs( NULL, srcString, length );
	wchar_t* srcWString = new wchar_t[ wlen + 1 ];
	srcWString[ mbstowcs( srcWString, srcString, length ) ] = L'\0';


	/**** ファイル生成 ****/
	aafile::AAFile* file = aafile::AAFileReader::CreateAAFile( srcWString, wlen );
	if( file )
	{
		// template変数構築
		sg_TemplateVariable.filePath = "from String";
		sg_TemplateVariable.pageCount = file->pageCount;


		FILE* fp;
		if( (fp = fopen( outputFile, "w" )) )
		{
			_runFunc( fp, file );
			fclose( fp );
		}

		aafile::AAFileReader::ReleaseAAFile( file );
	}

	if( srcWString )
	{
		delete[] srcWString;
	}
}


/*--------------------------------------------------------------------------
* ファイルスコープ関数定義
*/
/*------------
* 実行
*/
void _runFunc( FILE* fp, aafile::AAFile* file )
{
	// ファイルヘッダを書き出す
	_writeTemplate( fp, sg_HeaderTemplate );

	for( size_t i = 0; i < file->pageCount; i++ )
	{
		sg_TemplateVariable.pageNum = i + 1;

		// ページヘッダを書き出す
		_writeTemplate( fp, sg_PageHeaderTemplate );

		// ページ内容を書き出す
		for( size_t j = 0; j < file->pageList[ i ].valueLength; j++ )
		{
			switch( file->pageList[ i ].value[ j ] )
			{
			case L'\r':
				if( file->pageList[ i ].value[ j + 1 ] == L'\n' )
				{
					j++;
				}
			case L'\n':
				fputs( "<br>", fp );

				sg_TemplateVariable.fileSize += 2;// 改行は2byteで処理しておく
				break;
			case L'<':
				fputs( "&lt;", fp );
				sg_TemplateVariable.fileSize += 4;
				break;
			case L'>':
				{
					if( file->pageList[ i ].value[ j + 1 ] == L'>' )
					{
						int jmp = 0;
						int num = 0;
						for( num = 0; file->pageList[ i ].value[ j + 2 + num ] >= L'0' && file->pageList[ i ].value[ j + 2 + num ] <= L'9'; num++ )
						{
							jmp *= 10;
							jmp += (file->pageList[ i ].value[ j + 2 + num ] - L'0');
						}
						if( num > 0 )
						{
							fprintf( fp, "<a href=\"#%d\">", jmp );
							fprintf( fp, "&gt;&gt;%d", jmp );
							fputs( "</a>", fp );

							j += 2 + num - 1;
							sg_TemplateVariable.fileSize += 2 + num;
							break;
						}
					}

					fputs( "&gt;", fp );
					sg_TemplateVariable.fileSize += 4;
				}
				break;
			case L'h': case L'H':
			case L't': case L'T':
				{
					size_t count = 0;
					if( _isHttpString( &file->pageList[ i ].value[ j ], count ) )
					{
						wchar_t* addr = new wchar_t[ count ];
						::wcsncpy( addr, &file->pageList[ i ].value[ j ], count - 1 );
						addr[ count - 1 ] = L'\0';
						
						fprintf( fp, "<a href=\"" );
						if( addr[ 0 ] == L't' || addr[ 0 ] == L'T' )
						{
							fputs( "h", fp );
						}
						fwprintf( fp, L"%s\">%s</a>", addr, addr );

						
						sg_TemplateVariable.fileSize += count - 1;
						j += count - 2;
						break;
					}
				}
			default:
				_writeChar( fp, file->pageList[ i ].value[ j ] );
				break;
			}
		}

		// ページフッタを書き出す
		_writeTemplate( fp, sg_PageFooterTemplate );
	}

	// ファイルフッタを書き出す
	_writeTemplate( fp, sg_FooterTemplate );
}

/*------------
* template
*/
/**
* @brief ファイルヘッダのセットアップ
*	templateディレクトリ内のheader.htmlをセットアップします。
*
*	@param[in]	path					templateディレクトリへのパス
*/
static void _setupheader( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\header.html", path );
	_setuptemplate( tmpFilePath, sg_HeaderTemplate );
}

/**
* @brief ファイルフッタのセットアップ
*	templateディレクトリ内のfooter.htmlをセットアップします。
*
*	@param[in]	path					templateディレクトリへのパス
*/
static void _setupfooter( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\footer.html", path );
	_setuptemplate( tmpFilePath, sg_FooterTemplate );
}

/**
* @brief ページヘッダのセットアップ
*	templateディレクトリ内のpageheader.htmlをセットアップします。
*
*	@param[in]	path					templateディレクトリへのパス
*/
static void _setuppageheader( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\pageheader.html", path );
	_setuptemplate( tmpFilePath, sg_PageHeaderTemplate );
}

/**
* @brief ページフッタのセットアップ
*	templateディレクトリ内のpagefooter.htmlをセットアップします。
*
*	@param[in]	path					templateディレクトリへのパス
*/
static void _setuppagefooter( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\pagefooter.html", path );
	_setuptemplate( tmpFilePath, sg_PageFooterTemplate );
}

/**
* @brief templateのセットアップ
*	pathに指定されたtemplateファイルをtempに構築します。
*
*	@param[in]	path					templateファイルパス
*	@param[out]	temp					templateセットアップ先
*/
static void _setuptemplate( const char* path, TemplateList& temp )
{
	// templateをクリア
	temp.clear();

	// ファイルを読み込みながら構築
	FILE* fp;
	fp = fopen( path, "r" );
	if( fp )
	{
		char ch = 0;
		TemplateElement elem;
		while( (ch = fgetc( fp )) != EOF )
		{
			if( ch == '%' )
			{
				if( (ch = fgetc( fp )) != '%' )
				{
					// 現状を一旦プッシュ
					temp.push_back( elem );
					elem.str.clear();

					// 変数名取得
					char varName[ MAX_VARIABLE_NAME + 1 ] = { 0 };
					int count = 0;
					do {
						if( ch != EOF )
						{
							varName[ count++ ] = ch;
						}
					} while( (ch = fgetc( fp )) != '%' );
					varName[ count ] = '\0';

					// 変数のインデックスを設定
					elem.type = 1;
					elem.varType = TV_MAX;
					for( int i = 0; sg_TemplateVariableName[ i ]; i++ )
					{
						if( strcmp( varName, sg_TemplateVariableName[ i ] ) == 0 )
						{
							elem.varType = i;
							break;
						}
					}

					if( elem.varType != TV_MAX )
					{
						temp.push_back( elem );
					}
					continue;
				}
			}

			elem.str += ch;
			elem.type = 0;
		}
		temp.push_back( elem );
		fclose( fp );
	}
}

/*------------
* 書き出し
*/
/**
* @brief 指定ファイルにwchar_t文字を変換して書き出す
*	fpにwcをmbsに変換した文字列を書き出します。
*	wcが変換できない場合、&#xで始まるhtmlの文字参照を書き出します。
*
*	@param[in]	fp						書き出し先ファイルポインタ
*	@param[in]	wc						書き出す文字
*/
static void _writeChar( FILE* fp, wchar_t wc )
{
	if( fp )
	{
		char* str = new char[ MB_CUR_MAX + 1 ];

		int length = wctomb( str, wc );
		if( length < 0 )
		{
			fprintf( fp, "#x%04x;", wc );
			sg_TemplateVariable.fileSize += 7;
		}
		else
		{
			str[ length ] = '\0';
			fputs( str, fp );
			sg_TemplateVariable.fileSize += length;
		}
		delete[] str;
	}
}

/**
* @brief 指定ファイルにtemplateを書き出します
*	fpにtempに構築されているテンプレートを書き出します。
*	変数の解決などもここで行っています。
*
*	@param[in]	fp						書き出し先ファイルポインタ
*	@param[in]	temp					書き出すテンプレート
*/
static void _writeTemplate( FILE* fp, TemplateList& temp )
{
	if( fp )
	{
		for( size_t i = 0; i < temp.size(); i++ )
		{
			switch( temp[ i ].type )
			{
			case 0:// プレーンな文字列
				fputs( temp[ i ].str.c_str(), fp );
				break;
			case 1:// template変数
				_writeTemplateVariable( fp, temp[ i ].varType );
				break;
			}
		}
	}
}

/**
* @brief 指定ファイルにtemplate変数を書き出します
*	fpにvartypeで渡された変数と一致する内容を書き出します。
*
*	@param[in]	fp						書き出し先ファイルポインタ
*	@param[in]	vartype					変数の種別
*/
static void _writeTemplateVariable( FILE* fp, int vartype )
{
	switch( vartype )
	{	
	case TV_FILENAME:	// ファイル名
		fprintf( fp, "%s", sg_TemplateVariable.filePath );
		break;
	case TV_FILESIZE:	// 全ページ総容量
		fprintf( fp, "%d", sg_TemplateVariable.fileSize );
		break;
	case TV_PAGECOUNT:	// ファイル総ページ数
		fprintf( fp, "%d", sg_TemplateVariable.pageCount );
		break;
	case TV_PAGENUM:	// 現在のページ数
		fprintf( fp, "%d", sg_TemplateVariable.pageNum );
		break;
	case TV_TIME:		// 現在時刻
		{
			const char* week[] = { "日", "月", "火", "水", "木", "金", "土" };
			::SYSTEMTIME time;
			::GetLocalTime( &time );
			fprintf( fp, "%04d/%02d/%02d(%s) %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, week[ time.wDayOfWeek ], time.wHour, time.wMinute, time.wSecond );
		}
		break;
	}
}


//-- 文字列判定
static bool _isHttpString( const wchar_t* str, size_t& count )
{
	bool ret = false;

	wchar_t c = towlower( str[ 0 ] );
	if( c == L'h' || c == L't' )
	{
		count = 0;
		if( c == 'h' )
		{
			count = 1;
		}
		if( towlower( str[ count++ ] ) == 't'
			&& towlower( str[ count++ ] ) == 't'
			&& towlower( str[ count++ ] ) == 'p'
			&& towlower( str[ count++ ] ) == ':'
			&& towlower( str[ count++ ] ) == '/'
			&& towlower( str[ count++ ] ) == '/' )
		{
			while( str[ count ] != L'\0' && !isspace( str[ count++ ] ) );
			ret = true;
		}
	}

	return ret;
}


}



/*----------------------------------------------------------------------
*	MLT2HTML
*
*	Copyright (c) 2012 _16in/◆7N5y1wtOn2
*
*	Permission is hereby granted, free of charge, to any person obtaining
*	a copy of this software and associated documentation files (the "Software"),
*	to deal in the Software without restriction,
*	including without limitation the rights to use, copy, modify, merge,
*	publish, distribute, sublicense, and/or sell copies of the Software,
*	and to permit persons to whom the Software is furnished to do so,
*	subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be
*	included in all copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
*	THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
*	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*	THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*----------------------------------------------------------------------*/
