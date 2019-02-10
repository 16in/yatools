/*--------------------------------------------------------------------------------------------
*
* MLT2HTML  �R���o�[�^
*
*---------------------------------------------------------------------------------------------*/
/*-- �X�V���� --------------------------------------------------------------------------------
*
*	2012/12/05
*		�R���o�[�^���� ver1.0
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
* �萔��`
*/
static const int MAX_VARIABLE_NAME = 128;						//!< template�ϐ����̍ő啶����
	

/*--------------------------------------------------------------------------
* �񋓑̒�`
*/
/** template�ϐ� */
enum TemplateVariableType
{
	TV_FILENAME = 0x00,				//!< �t�@�C����
	TV_FILESIZE,					//!< �S�y�[�W���e��
	TV_PAGECOUNT,					//!< �t�@�C�����y�[�W��
	TV_PAGENUM,						//!< ���݂̃y�[�W��
	TV_TIME,						//!< ���ݎ���

	TV_MAX							//!< �ő吔
};


/*--------------------------------------------------------------------------
* �\���̒�`
*/
//! template�̗v�f
struct TemplateElement
{
	int				type;			//!< 0:������ 1:�u�������ϐ�
	std::string		str;			//!< ��������e
	int				varType;		//!< �ϐ��̃^�C�v
};
typedef std::vector<TemplateElement>	TemplateList;

//! template�ϐ�
struct TemplateVariable
{
	const char*		filePath;		//!< �t�@�C���p�X
	size_t			fileSize;		//!< �����ς̃y�[�W�f�[�^����
	size_t			pageCount;		//!< �t�@�C�����y�[�W��
	size_t			pageNum;		//!< �������Ă���y�[�W�ԍ�
};


/*--------------------------------------------------------------------------
* �t�@�C���X�R�[�v�O���[�o���ϐ���`
*/
//-- template
static TemplateList			sg_HeaderTemplate;
static TemplateList			sg_FooterTemplate;
static TemplateList			sg_PageHeaderTemplate;
static TemplateList			sg_PageFooterTemplate;

//-- template�ϐ�
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
* �t�@�C���X�R�[�v�֐��錾
*/
//-- ���s�֌W
static void _runFunc( FILE* fp, aafile::AAFile* file );

//-- template�֌W
// �t�@�C���w�b�_�̃Z�b�g�A�b�v
static void _setupheader( const char* path );

// �t�@�C���t�b�^�̃Z�b�g�A�b�v
static void _setupfooter( const char* path );

// �y�[�W�w�b�_�̃Z�b�g�A�b�v
static void _setuppageheader( const char* path );

// �y�[�W�t�b�^�̃Z�b�g�A�b�v
static void _setuppagefooter( const char* path );

// template�t�@�C�����Z�b�g�A�b�v
static void _setuptemplate( const char* path, TemplateList& temp );

//-- �t�@�C���o�͊֌W
// �w��t�@�C����wchar_t������ϊ����ď����o��
static void _writeChar( FILE* fp, wchar_t wc );

// �w��t�@�C����template�������o���܂�
static void _writeTemplate( FILE* fp, TemplateList& temp );

// �w��t�@�C����template�ϐ��������o���܂�
static void _writeTemplateVariable( FILE* fp, int vartype );

//-- �����񔻒�
static bool _isHttpString( const wchar_t* str, size_t& count );


/*--------------------------------------------------------------------------
* �O�����J�֐�
*/
/**
* @brief html����template�̃Z�b�g�A�b�v
*	$(appdir)/template�ɍ݂�template�t�@�C�����Z�b�g�A�b�v���܂��B
*
*	@param[in]	appdir					�A�v���P�[�V�����݂̍�f�B���N�g��
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
* @brief html����template�̃N�����i�b�v
*	�Z�b�g�A�b�v�ς݂�template���N�����i�b�v���܂��B
*/
void CleanupTemplate( void )
{
}

/**
* @brief html�������[�`���̎��s
*	�Z�b�g�A�b�v�ς݂�template�Ɠ��̓t�@�C������html�t�@�C���𐶐����܂��B
*
*	@param[in]	inputFile				���̓t�@�C���p�X
*	@param[in]	outputFile				�o�̓t�@�C���p�X
*/
void Run( const char* inputFile, const char* outputFile )
{
	/**** template�ϐ����N���A���� ****/
	memset( &sg_TemplateVariable, 0x00, sizeof( sg_TemplateVariable ) );


	/**** AA�t�@�C���ǂݍ��� ****/
	wchar_t inPath[ MAX_PATH ];
	mbstowcs( inPath, inputFile, MAX_PATH );
	aafile::AAFile* file = aafile::AAFileReader::CreateAAFileFromFile( inPath );
	if( file )
	{
		// template�ϐ��\�z
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

		// AA�t�@�C���p��
		aafile::AAFileReader::ReleaseAAFile( file );
	}
}

/**
* @brief html�������[�`���̎��s
*	�Z�b�g�A�b�v�ς݂�template�Ɠ��͕����񂩂�html�t�@�C���𐶐����܂��B
*
*	@param[in]	outputFile				�o�̓t�@�C���p�X
*	@param[in]	srcString				���͓��e������
*	@param[in]	length					���͓��e�̕�����
*/
void RunString( const char* outputFile, const char* srcString, unsigned long length )
{
	/**** template�ϐ����N���A���� ****/
	memset( &sg_TemplateVariable, 0x00, sizeof( sg_TemplateVariable ) );


	/**** �������wchar�֕ϊ� ****/
	size_t wlen = mbstowcs( NULL, srcString, length );
	wchar_t* srcWString = new wchar_t[ wlen + 1 ];
	srcWString[ mbstowcs( srcWString, srcString, length ) ] = L'\0';


	/**** �t�@�C������ ****/
	aafile::AAFile* file = aafile::AAFileReader::CreateAAFile( srcWString, wlen );
	if( file )
	{
		// template�ϐ��\�z
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
* �t�@�C���X�R�[�v�֐���`
*/
/*------------
* ���s
*/
void _runFunc( FILE* fp, aafile::AAFile* file )
{
	// �t�@�C���w�b�_�������o��
	_writeTemplate( fp, sg_HeaderTemplate );

	for( size_t i = 0; i < file->pageCount; i++ )
	{
		sg_TemplateVariable.pageNum = i + 1;

		// �y�[�W�w�b�_�������o��
		_writeTemplate( fp, sg_PageHeaderTemplate );

		// �y�[�W���e�������o��
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

				sg_TemplateVariable.fileSize += 2;// ���s��2byte�ŏ������Ă���
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

		// �y�[�W�t�b�^�������o��
		_writeTemplate( fp, sg_PageFooterTemplate );
	}

	// �t�@�C���t�b�^�������o��
	_writeTemplate( fp, sg_FooterTemplate );
}

/*------------
* template
*/
/**
* @brief �t�@�C���w�b�_�̃Z�b�g�A�b�v
*	template�f�B���N�g������header.html���Z�b�g�A�b�v���܂��B
*
*	@param[in]	path					template�f�B���N�g���ւ̃p�X
*/
static void _setupheader( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\header.html", path );
	_setuptemplate( tmpFilePath, sg_HeaderTemplate );
}

/**
* @brief �t�@�C���t�b�^�̃Z�b�g�A�b�v
*	template�f�B���N�g������footer.html���Z�b�g�A�b�v���܂��B
*
*	@param[in]	path					template�f�B���N�g���ւ̃p�X
*/
static void _setupfooter( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\footer.html", path );
	_setuptemplate( tmpFilePath, sg_FooterTemplate );
}

/**
* @brief �y�[�W�w�b�_�̃Z�b�g�A�b�v
*	template�f�B���N�g������pageheader.html���Z�b�g�A�b�v���܂��B
*
*	@param[in]	path					template�f�B���N�g���ւ̃p�X
*/
static void _setuppageheader( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\pageheader.html", path );
	_setuptemplate( tmpFilePath, sg_PageHeaderTemplate );
}

/**
* @brief �y�[�W�t�b�^�̃Z�b�g�A�b�v
*	template�f�B���N�g������pagefooter.html���Z�b�g�A�b�v���܂��B
*
*	@param[in]	path					template�f�B���N�g���ւ̃p�X
*/
static void _setuppagefooter( const char* path )
{
	char tmpFilePath[ MAX_PATH ];
	sprintf( tmpFilePath, "%s\\pagefooter.html", path );
	_setuptemplate( tmpFilePath, sg_PageFooterTemplate );
}

/**
* @brief template�̃Z�b�g�A�b�v
*	path�Ɏw�肳�ꂽtemplate�t�@�C����temp�ɍ\�z���܂��B
*
*	@param[in]	path					template�t�@�C���p�X
*	@param[out]	temp					template�Z�b�g�A�b�v��
*/
static void _setuptemplate( const char* path, TemplateList& temp )
{
	// template���N���A
	temp.clear();

	// �t�@�C����ǂݍ��݂Ȃ���\�z
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
					// �������U�v�b�V��
					temp.push_back( elem );
					elem.str.clear();

					// �ϐ����擾
					char varName[ MAX_VARIABLE_NAME + 1 ] = { 0 };
					int count = 0;
					do {
						if( ch != EOF )
						{
							varName[ count++ ] = ch;
						}
					} while( (ch = fgetc( fp )) != '%' );
					varName[ count ] = '\0';

					// �ϐ��̃C���f�b�N�X��ݒ�
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
* �����o��
*/
/**
* @brief �w��t�@�C����wchar_t������ϊ����ď����o��
*	fp��wc��mbs�ɕϊ�����������������o���܂��B
*	wc���ϊ��ł��Ȃ��ꍇ�A&#x�Ŏn�܂�html�̕����Q�Ƃ������o���܂��B
*
*	@param[in]	fp						�����o����t�@�C���|�C���^
*	@param[in]	wc						�����o������
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
* @brief �w��t�@�C����template�������o���܂�
*	fp��temp�ɍ\�z����Ă���e���v���[�g�������o���܂��B
*	�ϐ��̉����Ȃǂ������ōs���Ă��܂��B
*
*	@param[in]	fp						�����o����t�@�C���|�C���^
*	@param[in]	temp					�����o���e���v���[�g
*/
static void _writeTemplate( FILE* fp, TemplateList& temp )
{
	if( fp )
	{
		for( size_t i = 0; i < temp.size(); i++ )
		{
			switch( temp[ i ].type )
			{
			case 0:// �v���[���ȕ�����
				fputs( temp[ i ].str.c_str(), fp );
				break;
			case 1:// template�ϐ�
				_writeTemplateVariable( fp, temp[ i ].varType );
				break;
			}
		}
	}
}

/**
* @brief �w��t�@�C����template�ϐ��������o���܂�
*	fp��vartype�œn���ꂽ�ϐ��ƈ�v������e�������o���܂��B
*
*	@param[in]	fp						�����o����t�@�C���|�C���^
*	@param[in]	vartype					�ϐ��̎��
*/
static void _writeTemplateVariable( FILE* fp, int vartype )
{
	switch( vartype )
	{	
	case TV_FILENAME:	// �t�@�C����
		fprintf( fp, "%s", sg_TemplateVariable.filePath );
		break;
	case TV_FILESIZE:	// �S�y�[�W���e��
		fprintf( fp, "%d", sg_TemplateVariable.fileSize );
		break;
	case TV_PAGECOUNT:	// �t�@�C�����y�[�W��
		fprintf( fp, "%d", sg_TemplateVariable.pageCount );
		break;
	case TV_PAGENUM:	// ���݂̃y�[�W��
		fprintf( fp, "%d", sg_TemplateVariable.pageNum );
		break;
	case TV_TIME:		// ���ݎ���
		{
			const char* week[] = { "��", "��", "��", "��", "��", "��", "�y" };
			::SYSTEMTIME time;
			::GetLocalTime( &time );
			fprintf( fp, "%04d/%02d/%02d(%s) %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, week[ time.wDayOfWeek ], time.wHour, time.wMinute, time.wSecond );
		}
		break;
	}
}


//-- �����񔻒�
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
*	Copyright (c) 2012 _16in/��7N5y1wtOn2
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
