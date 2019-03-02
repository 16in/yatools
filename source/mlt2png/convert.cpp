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
// �񋓗p�R�[���o�b�N
//-------------------------------------
// �t�H���g��
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
// �ÓI�����o�ϐ�
//-------------------------------------
HFONT	Converter::m_hFont = NULL;							//!< �t�H���g�n���h��
int32_t	Converter::m_CharacterWidth[ 0x10000 ] = { 0 };		//!< �������Ƃ̉���
int32_t	Converter::m_CharacterHeight = 0;					//!< �����c��


//-------------------------------------
// �Z�b�g�A�b�v/�N�����i�b�v
//-------------------------------------
//-------------------------------------
// �Z�b�g�A�b�v
//
bool Converter::Setup( const char* fontName, int fontSize )
{
	/*---- �t�H���g���� ----*/
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
			if( strcmp( enumFontList[ i ].lfFaceName, "MS P�޼��" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "MS PGothic" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "MS P�S�V�b�N" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "�l�r �o�S�V�b�N" ) == 0
				|| strcmp( enumFontList[ i ].lfFaceName, "�l�r�@�o�S�V�b�N" ) == 0
			)
			{
				strcpy( logFont.lfFaceName, enumFontList[ i ].lfFaceName );
				break;
			}
		}
	}


	/*---- �_���t�H���g������ ----*/
	// �t�H���g�������邩����
	if( logFont.lfFaceName[ 0 ] == '\0' )
	{
		return false;
	}

	// �t�H���g�ڍאݒ�
	logFont.lfHeight = fontSize == 0 ? -16 : fontSize;


	/*---- �Z�b�g�A�b�v�ς݂Ȃ��U�p�� ----*/
	if( m_hFont != NULL )
	{
		Cleanup( );
	}


	// ���ʃt�H���g����
	m_hFont = CreateFontIndirectA( &logFont );
	if( m_hFont != NULL )
	{
		// �������e�[�u���X�V
		HWND hWnd = GetDesktopWindow( );
		HDC hdc = GetDC( hWnd );
		HFONT hOldFont = (HFONT)SelectObject( hdc, m_hFont );

		SIZE size;
		::GetTextExtentPoint32W( hdc, L" ", 1, &size );
		m_CharacterHeight = size.cy;
		GetCharWidth32W( hdc, 0x0000, 0xffff, m_CharacterWidth );

		// DC�ݒ�߂�
		SelectObject( hdc, hOldFont );
		ReleaseDC( hWnd, hdc );
	}
	return m_hFont != NULL;
}

//-------------------------------------
// �N�����i�b�v
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
// ���s
//-------------------------------------
//-------------------------------------
// PNG�������[�`���̎��s
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
// PNG�������[�`���̎��s
//
void Converter::RunString( const char* outputDirectory, const char* srcString, unsigned long length )
{
	size_t wlength = mbstowcs( NULL, srcString, length );
	wchar_t* wstring = new wchar_t[ wlength ];
}

//-------------------------------------
// �S�y�[�W�����o��
//
void Converter::paintAllPage( aafile::AAFile* file, const char* outputDirectory )
{
	// �o�͐搶��
	if( !PathFileExistsA( outputDirectory ) || (PathFileExistsA( outputDirectory ) && !PathIsDirectoryA( outputDirectory )) )
	{
		char path[ MAX_PATH ];
		sprintf( path, "%s", outputDirectory );
		if( !CreateDirectoryA( path, NULL ) )
		{
			puts( "�o�͐�f�B���N�g���̍쐬�Ɏ��s���܂����B" );
			return;
		}
	}


	HDC hMemDC = CreateCompatibleDC( NULL );
	for( uint_t i = 0; i < file->pageCount; i++ )
	{
		aafile::AAFilePage* page = &file->pageList[ i ];

		// �y�[�W�T�C�Y�擾
		int width, height, lineCount;
		getPageSize( page, width, height, lineCount );

		// �r�b�g�}�b�v����
		HBITMAP hBitmap = CreateCompatibleBitmap( hMemDC, width, height );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

		paintPage( hMemDC, page, width, height, lineCount );
		yatools::image::ColorBuffer* buffer = yatools::image::Windows::GetColorBuffer( hMemDC, (uint32_t)width, (uint32_t)height );

		// �r�b�g�}�b�v�J��
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
// �y�[�W��������
//
void Converter::paintPage( HDC hDC, aafile::AAFilePage* page, int width, int height, int lfCount )
{
	/*---- �f�o�C�X�R���e�L�X�g�̐ݒ� ----*/
	HFONT hOldFont = (HFONT)SelectObject( hDC, m_hFont );
	int oldMode = SetBkMode( hDC, TRANSPARENT );


	/*---- �w�i�N���A ----*/
	RECT rect = { 0, 0, width, height };
	FillRect( hDC, &rect, WHITE_BRUSH );


	/*---- �ꕶ�����o�� ----*/
	std::wstring lfString = L"";
	int y = 0;
	for( unsigned long pos = 0, step = 1; pos < page->valueLength; pos += step )
	{
		bool lf = false;
		step = 1;

		// ���s�`�F�b�N
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
			/*---- �\��������擾 ----*/
			wchar_t ch[ 4 ] = { 0 };
			getCharWidth( &page->value[ pos ], page->valueLength, step, ch );
			lfString += ch;
		}
		else
		{
			/*-- ���C���o�� --*/
			TextOutW( hDC, 0, y, lfString.c_str( ), (int)lfString.size( ) );
			lfString.clear( );
			y += m_CharacterHeight;
		}
	}
	TextOutW( hDC, 0, y, lfString.c_str( ), (int)lfString.size( ) );


	/*---- �f�o�C�X�R���e�L�X�g�̐ݒ� ----*/
	SelectObject( hDC, hOldFont );
}

//----------------------------------------------
// �ŃT�C�Y���v�Z
//
void Converter::getPageSize( aafile::AAFilePage* page, int& width, int& height, int& lfCount )
{
	/*---- ��Ԃ������� ----*/
	lfCount = 1;
	height = width = 0;


	/*---- ���������v�Z ----*/
	int lineWidth = 0;
	for( unsigned long pos = 0, step = 1; pos < page->valueLength; pos += step )
	{
		bool lf = false;
		step = 1;

		// ���s�`�F�b�N
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


	/*---- �������v�Z ----*/
	height = lfCount * m_CharacterHeight;
}


//----------------------------------------------
// �擪�����̕������ƕ����R�[�h���擾
//
int Converter::getCharWidth( const wchar_t* str, size_t length, unsigned long& step, wchar_t* ch )
{
	int ret = 0;
	wchar_t refChar[ 2 ];
	unsigned long refCount;

	if( !CheckRefChar( str, (unsigned long)length, refChar, step, refCount ) )
	{// �Q�ƕ����������Ȃ�����
		step = 1;
		ret = m_CharacterWidth[ str[ 0 ] ];
		if( ch ) *ch = str[ 0 ];
	}
	else
	{// �Q�ƕ��������ς݂̂���
		if( refCount == 1 )
		{// ��T���Q�[�g����
			ret = m_CharacterWidth[ refChar[ 0 ] ];
			if( ch ) *ch = refChar[ 0 ];
		}
		else
		{// �T���Q�[�g�����c�����ǁA�ǂ����悤
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
// �Q�ƕ����񂩃`�F�b�N���A�Q�ƕ�����Ȃ�ΎQ�Ƃ���������
//
bool Converter::CheckRefChar( const wchar_t* str, unsigned long length, wchar_t refChar[ 2 ], unsigned long& step, unsigned long& refCount )
{
	bool ret = false;

	if( str[ 0 ] == L'&' && length >= 2 )
	{// �Q�ƕ�����
		if( str[ 1 ] != L'#' )
		{// ���̎Q��
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
		{// ���l�Q��
			wchar_t*		endPtr;
			unsigned long	code;

			if( towlower( str[ 2 ] ) != L'x' )
			{// 10�i���Q��
				code = wcstol( &str[ 2 ], &endPtr, 10 );
			}
			else
			{// 16�i���Q��
				code = wcstol( &str[ 3 ], &endPtr, 16 );
			}

			if( endPtr[ 0 ] == L';' )
			{// �Q�ƕ�����ł�����
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