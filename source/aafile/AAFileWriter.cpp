/*----------------------------------------------------------------------
*
* AA�֘A�t�@�C�����C�^
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#include <aafile/AAFileAccessor.h>
#include <Windows.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "localWriter.h"


namespace aafile {


/**** �O���[�o���ϐ� ****/
static const wchar_t*		cs_LineFieldString = L"\r\n";
static uint_t				s_LineFieldLength = 2;


/**** AA�t�@�C�����C�^ ****/
/**
* AAFile�̏��ɏ]���āA�f�[�^���t�@�C���ɏ����o��
*	@param[in]	pAaFile				�����o������Z�߂�AAFile
*	@param[in]	filePath			�����o���t�@�C���ւ̃p�X
*	@param[in]	utf16				UTF16-LE�Ƃ��ď����o���Ȃ�^�A����ȊO�͋U
*	@return		uint_t				���ۂɏ����o����������
*/
uint_t AAFileWriter::WriteAAFileToFile( const AAFile* pAaFile, const wchar_t* filePath, bool utf16 )
{
	/**** �o�b�t�@���쐬���� ****/
	uint_t wSize = WriteAAFile( pAaFile, NULL, 0 );
	wchar_t* writeBuffer = new wchar_t[ wSize ];
	wSize = WriteAAFile( pAaFile, writeBuffer, wSize );


	/**** �t�@�C���𐶐����� ****/
	::HANDLE hFile = ::CreateFileW( filePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile )
	{
		::DWORD writeSize;
		if( utf16 )
		{
			::WORD BOM = 0xfeff;
			::WriteFile( hFile, &BOM, sizeof( BOM ), &writeSize, NULL );
			
			if( writeBuffer )
				::WriteFile( hFile, writeBuffer, (DWORD)(sizeof( wchar_t ) * wSize), &writeSize, NULL );
		}
		else
		{
			if( writeBuffer )
			{
				int length = ::WideCharToMultiByte( 932, 0, writeBuffer, (int)wSize, NULL, 0, NULL, NULL );
				char* str = new char[ length ];
				::WideCharToMultiByte( 932, 0, writeBuffer, (int)wSize, str, length, NULL, NULL );

				::WriteFile( hFile, str, length, &writeSize, NULL );

				delete[] str;
			}
		}

		::CloseHandle( hFile );
	}


	/**** �o�b�t�@�̔p�� ****/
	if( writeBuffer )
	{
		delete[] writeBuffer;
	}


	return wSize;
}

/**
* AAFile�̏��ɏ]���āA�f�[�^����������ɏ����o��
*	str��NULL�̏ꍇ�A�i�[�����͍s�킸�A�K�v�ȗv�f�������Ԃ��B
*	@param[in]	pAaFile				�����o������Z�߂�AAFile
*	@param[out]	str					������̊i�[��
*	@param[in]	length				str�̗e��(wchar_t�P�ʂ̗v�f��)
*	@return		uint_t				���ۂɏ����o����������
*/
uint_t AAFileWriter::WriteAAFile( const AAFile* pAaFile, wchar_t* str, uint_t length )
{
#define CALL_WRITEFUNC( type )	case AFT_##type:												\
if( str )ret += WritePage##type( &pAaFile->pageList[ i ], &str[ ret ], length - ret, last );	\
else	 ret += WritePage##type( &pAaFile->pageList[ i ], NULL, 0, last );	break


	uint_t ret = 0;


	/**** �y�[�W���Ƃɏ��������� ****/
	for( uint_t i = 0; i < pAaFile->pageCount; i++ )
	{
		bool last = (i + 1 == pAaFile->pageCount);

		switch( pAaFile->type )
		{
		CALL_WRITEFUNC( MLT );
		CALL_WRITEFUNC( AST );
		CALL_WRITEFUNC( ASD );
		CALL_WRITEFUNC( AADATA );
		CALL_WRITEFUNC( AALIST );
		default:
			if( str )ret += WritePageTXT( &pAaFile->pageList[ i ], &str[ ret ], length - ret, last );
			else	 ret += WritePageTXT( &pAaFile->pageList[ i ], NULL, 0, last );
			break;
		}
	}


	return ret;
}

/**
* ���s�R�[�h���w��
*	@param[in]	lfString			���s�R�[�h(L'\0'�I�[�ł���K�v������܂�)
*/
void AAFileWriter::SetLineField( const wchar_t* lfString )
{
	cs_LineFieldString = lfString;
	s_LineFieldLength = wcslen( lfString );
}

/**
* ���s�R�[�h���擾
*	@return		const wchar_t*		���s�R�[�h
*/
const wchar_t* AAFileWriter::GetLineField( void )
{
	return cs_LineFieldString;
}

/**
* �y�[�W���e�̂ݏ����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@return		uint_t				�����o����������
*/
uint_t WritePageValue( AAFilePage* page, wchar_t* writeString, uint_t length )
{
	uint_t ret = 0;

	/**** �ꕶ�������肵�Ă��� ****/
	for( uint_t i = 0; i < page->valueLength; i++ )
	{
		uint_t step = CheckLineField( &page->value[ i ], page->valueLength - i );
		if( step == 0 )
		{
			if( writeString )
			{// ���������o��
				if( ret >= length ) break;
				writeString[ ret ] = page->value[ i ];
			}
			ret++;
		}
		else
		{
			if( writeString )
			{// ���������o��
				if( ret + step - 1 >= length ) break;
				for(uint_t l = 0; l < s_LineFieldLength; l++ )
					writeString[ ret + l ] = cs_LineFieldString[ l ];
			}
			i += step - 1;
			ret += s_LineFieldLength;
		}
	}

	return ret;
}

/**
* TXT�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageTXT( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;

	// �y�[�W���e�������o��
	ret = WritePageValue( page, writeString, length );

	// �y�[�W�̏I�[�ɉ��s������
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	return ret;
}

/**
* MLT�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageMLT( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** �y�[�W���e�������o�� ****/
	ret = WritePageValue( page, writeString, length );


	/**** ���s�ƕ������������o�� ****/
	const wchar_t* split = L"[SPLIT]";
	uint_t splitLen = wcslen( split );

	// ���������O�̉��s
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	// �ŏI�y�[�W�Ȃ番��������͏o�͂��Ȃ�
	if( !lastPage )
	{
		// ��������
		if( writeString )
		{
			if( length - ret < splitLen ) return ret;
			for( uint_t s = 0; s < splitLen; s++ )
				writeString[ ret + s ] = split[ s ];
		}
		ret += splitLen;

		// ����������̉��s
		if( writeString )
		{
			if( length - ret < s_LineFieldLength ) return ret;
			for( uint_t l = 0; l < s_LineFieldLength; l++ )
				writeString[ ret + l ] = cs_LineFieldString[ l ];
		}
		ret += s_LineFieldLength;
	}

	return ret;
}

/**
* AST�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageAST( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** ���s�ƕ������������o�� ****/
	const wchar_t* split = L"[AA][";
	uint_t splitLen = wcslen( split );

	// ���������ƃy�[�W��
	uint_t namelength = splitLen + page->nameLength + 1;
	if( writeString )
	{
		if( length - ret < namelength ) return ret;
		for( uint_t i = 0; i < splitLen; i++ )
			writeString[ ret + i ] = split[ i ];
		for( uint_t i = 0; i < page->nameLength; i++ )
			writeString[ ret + splitLen + i ] = page->name[ i ];
		writeString[ ret + splitLen + page->nameLength ] = L']';
	}
	ret += splitLen + page->nameLength + 1;

	// ����������̉��s
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	/**** �y�[�W���e�������o�� ****/
	if( writeString )	ret += WritePageValue( page, &writeString[ ret ], length - ret );
	else				ret += WritePageValue( page, NULL, 0 );

	// �y�[�W�̏I�[�ɉ��s������
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	return ret;
}

/**
* ASD�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageASD( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** �y�[�W���e�������o�� ****/
	// ���s�R�[�h�U��
	const wchar_t* oldLF = cs_LineFieldString;
	uint_t oldLen = s_LineFieldLength;
	cs_LineFieldString = L"\x1\x1";
	s_LineFieldLength = 2;

	// ���f�[�^�����o��
	ret += WritePageValue( page, writeString, length );

	// ���s�R�[�h�̋U���I��
	cs_LineFieldString = oldLF;
	s_LineFieldLength = oldLen;


	/**** �y�[�W�������o�� ****/
	if( writeString )
	{
		if( length - ret < 2 + page->nameLength + s_LineFieldLength ) return ret;
		writeString[ ret ] = 0x02; writeString[ ret + 1 ] = 0x02;
		for( uint_t i = 0; i < page->nameLength; i++ )
			writeString[ ret + 2 + i ] = page->name[ i ];
		for( uint_t i = 0; i < s_LineFieldLength; i++ )
			writeString[ ret + 2 + page->nameLength + i ] = cs_LineFieldString[ i ];
	}
	ret += 2 + page->nameLength + s_LineFieldLength;


	return ret;
}

/**
* AAData�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageAADATA( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	return WritePageAADataList( L"[MojieName=", L"[END]", page, writeString, length, lastPage );
}

/**
* AAList�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageAALIST( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	return WritePageAADataList( L"[ListName=", L"[end]", page, writeString, length, lastPage );
}

/**
* AAData�t�@�C���̏����o��
*	@param[in]	split				����������
*	@param[in]	end					�I�[����������
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
uint_t WritePageAADataList( const wchar_t* split, const wchar_t* end, AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** ���s�ƕ������������o�� ****/
	uint_t splitLen = wcslen( split );

	// ���������ƃy�[�W��
	uint_t namelength = splitLen + page->nameLength + 1;
	if( writeString )
	{
		if( length - ret < namelength ) return ret;
		for( uint_t i = 0; i < splitLen; i++ )
			writeString[ ret + i ] = split[ i ];
		for( uint_t i = 0; i < page->nameLength; i++ )
			writeString[ ret + splitLen + i ] = page->name[ i ];
		writeString[ ret + splitLen + page->nameLength ] = L']';
	}
	ret += splitLen + page->nameLength + 1;

	// ����������̉��s
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	/**** �y�[�W���e�������o�� ****/
	if( writeString )	ret += WritePageValue( page, &writeString[ ret ], length - ret );
	else				ret += WritePageValue( page, NULL, 0 );

	// �y�[�W�̏I�[�ɉ��s������
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	/**** �I���^�O���������� ****/
	uint_t endLen = wcslen( end );
	if( writeString )
	{
		if( length - ret < endLen ) return ret;
		for( uint_t l = 0; l < endLen; l++ )
			writeString[ ret + l ] = end[ l ];
	}
	ret += endLen;


	// �I�[�ɉ��s������
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	return ret;
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
