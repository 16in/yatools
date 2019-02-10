/*----------------------------------------------------------------------
*
* AA�֘A�t�@�C�����[�_
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#include <aafile/AAFileAccessor.h>
#include <Windows.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "localReader.h"


namespace aafile {


/**** AA�t�@�C�����[�_ ****/
/**
* AAFile���t�@�C�����琶������
*	@param[in]	filePath			�t�@�C���p�X
*	@return		AAFile*				�ǂݍ��܂ꂽ�t�@�C���f�[�^
*									�ǂݍ��݂Ɏ��s�����ꍇ��NULL
*/
AAFile* AAFileReader::CreateAAFileFromFile( const wchar_t* filePath )
{
	AAFile*			ret = NULL;
	unsigned char*	fileBuffer = NULL;
	DWORD			fileSize = 0;


	/**** �t�@�C����ǂݍ��� ****/
	HANDLE hFile = CreateFileW( filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		fileSize = GetFileSize( hFile, NULL );
		fileBuffer = new unsigned char[ fileSize ];
		ReadFile( hFile, fileBuffer, fileSize, &fileSize, NULL );
		CloseHandle( hFile );
	}


	/**** wchar_t�^�ɕϊ� ****/
	if( fileBuffer == NULL ) return NULL;


	/** BOM�`�F�b�N **/
	unsigned short BOM = ((unsigned short*)fileBuffer)[ 0 ];
	if( BOM == 0xFFFE )
	{// BE�ł������珈�����Ȃ�
		delete[] fileBuffer;
		return NULL;
	}
	if( BOM == 0xFEFF )
	{// UTF-16 LE�ł���΂��̂܂ܗ�������
		ret = CreateAAFile( &((wchar_t*)fileBuffer)[ 1 ], (fileSize-2) / sizeof( wchar_t ) );
		if( ret )	ret->fileData = (wchar_t*)fileBuffer;
		else		delete[] fileBuffer;
	}
	else
	{// UTF-16�łȂ����S-JIS�Ɖ��肵�ēǂݍ���
		int length = MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, (LPCSTR)fileBuffer, fileSize, NULL, 0 );
		wchar_t* wstr = new wchar_t[ length ];
		MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, (LPCSTR)fileBuffer, fileSize, wstr, length );
		delete[] fileBuffer;
		
		ret = CreateAAFile( wstr, length );
		if( ret )	ret->fileData = wstr;
		else		delete[] wstr;
	}


	return ret;
}

/**
* AAFile����e�����񂩂琶������
*	@param[in]	str					���e������
*	@param[in]	length				������
*	@return		AAFile*				�ǂݍ��܂ꂽ�t�@�C���f�[�^
*									�ǂݍ��݂Ɏ��s�����ꍇ��NULL
*/
AAFile* AAFileReader::CreateAAFile( wchar_t* str, uint_t length )
{
#define CALL_SPLIT( type ) case AFT_##type: Split##type( pageArray, str, length ); break
	AAFile* ret = NULL;
	AAFilePageArray pageArray;

	if( str && length > 0 )
	{
		/**** �t�@�C���^�C�v���� ****/
		AAFileType type = CheckFileType( str, length );


		/**** �t�@�C���^�C�v�ʂɃy�[�W�������� ****/
		pageArray.clear();
		switch( type )
		{
		CALL_SPLIT( MLT );
		CALL_SPLIT( AST );
		CALL_SPLIT( ASD );
		CALL_SPLIT( AADATA );
		CALL_SPLIT( AALIST );
		case AFT_TXT:
			{// txt�t�@�C��
				AAFilePage page;
				page.name = NULL;
				page.nameLength = 0;
				page.value = str;
				page.valueLength = length;
				pageArray.push_back( page );
			}
			break;
		}


		/**** �y�[�W���X�g�̏����������� ****/
		ret = new AAFile;
		ret->fileData = NULL;
		ret->type = type;
		ret->pageCount = pageArray.size();
		ret->pageList = new AAFilePage[ ret->pageCount ];
		for( uint_t i = 0; i < ret->pageCount; i++ )
		{
			ret->pageList[ i ] = pageArray[ i ];
		}
	}

	return ret;
}

/**
* �������ꂽAAFile��p������
*	@param[in]	pAaFile				�p������AAFile�̃C���X�^���X
*/
void AAFileReader::ReleaseAAFile( AAFile* pAaFile )
{
	if( pAaFile )
	{
		if( pAaFile->fileData ) delete[] pAaFile->fileData;
		if( pAaFile->pageList ) delete[] pAaFile->pageList;
		delete pAaFile;
	}
}

/**
* MLT�t�@�C���𕪊�
*	@param[in]	pageArray			�y�[�W�f�[�^�i�[��
*	@param[in]	str					�����Ώە�����
*	@param[in]	length				������
*/
void SplitMLT( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	AAFilePage page;
	const wchar_t* split = L"[SPLIT]";
	size_t len = wcslen( split );

	
	/**** �ŏ��̗v�f����� ****/
	page.name = NULL;
	page.nameLength = 0;
	page.value = str;

	for( uint_t i = 0; i < length; i++ )
	{
		uint_t step = CheckLineField( &str[ i ], length - i );
		if( step > 0 )
		{
			if( _wcsnicmp( &str[ i + step ], split, len ) == 0 )
			{
				/**** �y�[�W�����Z�o���Ēǉ� ****/
				page.valueLength = &str[ i ] - page.value;
				if( page.valueLength > 0 )
				{
					pageArray.push_back( page );
				}

				// ���s�܂łԂ��
				for( uint_t j = i + len + step; j < length; j++ )
				{
					if( str[ j ] == L'\r' && str[ j + 1 ] == L'\n' )
					{
						i = j + 1;
						break;
					}
					else if( str[ j ] == L'\r' || str[ j ] == L'\n' )
					{
						i = j;
						break;
					}
				}


				/**** ���̃y�[�W�ֈړ� ****/
				page.value = &str[ i + 1 ];
			}
		}
	}


	/**** �ŏI�y�[�W�����Z�o���Ēǉ� ****/
	if( !(str[ length - 1 ] == L']' && _wcsnicmp( page.value, split, len ) == 0) )
	{
		if( str[ length - 1 ] == L'\n' || str[ length - 1 ] == L'\r' )
		{// �Ō�̉��s�͖�������
			length--;
			if( str[ length ] == L'\n' && str[ length - 1 ] == L'\r' )
				length--;
		}
		if( &str[ length ] - page.value > 0 )
		{
			page.valueLength = &str[ length ] - page.value;
		}
		else
		{
			page.valueLength = 0;
		}
		if( page.valueLength > 0 )
		{
			pageArray.push_back( page );
		}
	}
}

/**
* AST�t�@�C���𕪊�
*	@param[in]	pageArray			�y�[�W�f�[�^�i�[��
*	@param[in]	str					�����Ώە�����
*	@param[in]	length				������
*/
void SplitAST( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	const wchar_t* split = L"[AA]";
	size_t len = wcslen( split );
	AAFilePage page;

	
	/**** �ŏ��̗v�f���\�z ****/
	uint_t i = 0;

	if( _wcsnicmp( str, split, len ) == 0 )
	{
		i += len;

		// �y�[�W�����𓾂�
		for( ; i < length && str[ i ] != L'['; i++ );
		page.name = &str[ ++i ];
		for( ; i < length && str[ i ] != L']'; i++ );
		page.nameLength = &str[ i ] - page.name;
		for( ; i < length; i++ )
		{
			uint_t step = CheckLineField( &str[ i ], length - i );
			if( step > 0 )
			{
				i += step;
				break;
			}
		}

		// �y�[�W���𓾂�
		page.value = &str[ i ];
	}


	/**** �ȍ~�̗v�f��ǉ����Ă��� ****/
	for( ; i < length; i++ )
	{
		uint_t step = CheckLineField( &str[ i ], length - i );
		if( step > 0 )
		{
			if( _wcsnicmp( &str[ i + step ], split, len ) == 0 )
			{
				/** �y�[�W�ǉ����� **/
				page.valueLength = &str[ i ] - page.value;
				pageArray.push_back( page );
				i += step + len;
				
				/** �y�[�W�����𓾂� **/
				for( ; i < length && str[ i ] != L'['; i++ );
				page.name = &str[ ++i ];
				for( ; i < length && str[ i ] != L']'; i++ );
				page.nameLength = &str[ i ] - page.name;
				for( ; i < length; i++ )
				{
					uint_t step = CheckLineField( &str[ i ], length - i );
					if( step > 0 )
					{
						i += step;
						break;
					}
				}

				/** �y�[�W���𓾂� **/
				page.value = &str[ i ];
			}
		}
	}


	/**** �ŏI�y�[�W�����Z�o���Ēǉ� ****/
	if( str[ length - 1 ] == L'\n' || str[ length - 1 ] == L'\r' )
	{// �Ō�̉��s�͖�������
		length--;
		if( str[ length ] == L'\n' && str[ length - 1 ] == L'\r' )
			length--;
	}
	if( &str[ length ] - page.value > 0 )
	{
		page.valueLength = &str[ length ] - page.value;
	}
	else
	{
		page.valueLength = 0;
	}
	pageArray.push_back( page );
}

/**
* ASD�t�@�C���𕪊�
*	@param[in]	pageArray			�y�[�W�f�[�^�i�[��
*	@param[in]	str					�����Ώە�����
*	@param[in]	length				������
*/
void SplitASD( AAFilePageArray& pageArray, wchar_t* str, uint_t length )
{
	const wchar_t* split = L"\x2\x2";
	const wchar_t* lf = L"\x1\x1";
	size_t len = 2;
	AAFilePage page;


	/**** �ŏ��̗v�f���w�� ****/
	page.name = NULL;
	page.nameLength = page.valueLength = 0;
	page.value = str;


	/**** �����I�[�܂Ő����i�߂� ****/
	for( size_t i = 0; i < length; i++ )
	{
		if( _wcsnicmp( lf, &str[ i ], len ) == 0 )
		{// ���s�ƈ�v
			str[ i ] = L'\r'; str[ i + 1 ] = L'\n';
		}
		else if( _wcsnicmp( &str[ i ], split, len ) == 0 )
		{// ����������ƈ�v
			page.valueLength = &str[ i ] - page.value;
			page.name = &str[ (i += len) ];

			/**** ���s�܂Ń��[�v ****/
			uint_t step = CheckLineField( &str[ i ], length - i );
			for( ; i < length; i++ )
			{
				step = CheckLineField( &str[ i ], length - i );
				if( step > 0 ) break;
			}


			/**** �y�[�W�������񒷂��m�肵�A�ǉ����� ****/
			page.nameLength = &str[ i ] - page.name;
			pageArray.push_back( page );
			i += step;


			/**** ���̃y�[�W�ֈړ� ****/
			page.value = &str[ i + 1 ];
		}
	}
}

/**
* AAData�t�@�C���𕪊�
*	@param[in]	pageArray			�y�[�W�f�[�^�i�[��
*	@param[in]	str					�����Ώە�����
*	@param[in]	length				������
*/
void SplitAADATA( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	SplitAAListData( pageArray, str, length, L"[MojieName=", L"[END]" );
}

/**
* AAList�t�@�C���𕪊�
*	@param[in]	pageArray			�y�[�W�f�[�^�i�[��
*	@param[in]	str					�����Ώە�����
*	@param[in]	length				������
*/
void SplitAALIST( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	SplitAAListData( pageArray, str, length, L"[ListName=", L"[END]" );
}

/**
* AAList/AAData�t�@�C���𕪊�
*	@param[in]	pageArray			�y�[�W�f�[�^�i�[��
*	@param[in]	str					�����Ώە�����
*	@param[in]	length				������
*	@param[in]	start				�擪����������
*	@param[in]	end					�I�[����������
*/
void SplitAAListData( AAFilePageArray& pageArray, const wchar_t* str, uint_t length, const wchar_t* start, const wchar_t* end )
{
	size_t startLen = wcslen( start );
	size_t endLen = wcslen( end );
	AAFilePage page;
	page.name = page.value = NULL;
	page.nameLength = page.valueLength = 0;

	
	/**** �ŏ��̗v�f�𔻒� ****/
	uint_t i = 0;
	if( _wcsnicmp( start, str, startLen ) == 0 )
	{
		i += startLen;
		page.name = &str[ i ];
		for( ; str[ i ] != L']'; i++ );
		page.nameLength = &str[ i ] - page.name;

		// ���s�܂łԂ��
		for( ; i < length; i++ )
		{
			uint_t step = CheckLineField( &str[ i ], length - i );
			if( step > 0 )
			{
				i += step;
				break;
			}
		}

		page.value = &str[ i ];
	}


	/**** �ȍ~�̗v�f�𔻒� ****/
	for( ; i < length; i++ )
	{
		uint_t step = CheckLineField( &str[ i ], length - i );
		if( step > 0 )
		{
			if( _wcsnicmp( &str[ i + step ], end, endLen ) == 0 )
			{// �y�[�W�v�f���m�肵�Ēǉ�
				page.valueLength = &str[ i ] - page.value;
				pageArray.push_back( page );
				
				i += step;
			}
			else if( _wcsnicmp( start, &str[ i + step ], startLen ) == 0 )
			{// �y�[�W�����i�[����
				i += startLen + step;
				page.name = &str[ i ];
				for( ; str[ i ] != L']'; i++ );
				page.nameLength = &str[ i ] - page.name;

				// ���s�܂łԂ��
				for( ; i < length; i++ )
				{
					uint_t step = CheckLineField( &str[ i ], length - i );
					if( step > 0 )
					{
						i += step;
						break;
					}
				}
				page.value = &str[ i ];
			}
		}
	}
}



}


/*----------------------------------------------------------------------
* License
*
*	AAFileAccessor
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
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
