/*----------------------------------------------------------------------
*
* AA関連ファイルライタ
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#include <aafile/AAFileAccessor.h>
#include <Windows.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "localWriter.h"


namespace aafile {


/**** グローバル変数 ****/
static const wchar_t*		cs_LineFieldString = L"\r\n";
static uint_t				s_LineFieldLength = 2;


/**** AAファイルライタ ****/
/**
* AAFileの情報に従って、データをファイルに書き出す
*	@param[in]	pAaFile				書き出す情報を纏めたAAFile
*	@param[in]	filePath			書き出すファイルへのパス
*	@param[in]	utf16				UTF16-LEとして書き出すなら真、それ以外は偽
*	@return		uint_t				実際に書き出した文字数
*/
uint_t AAFileWriter::WriteAAFileToFile( const AAFile* pAaFile, const wchar_t* filePath, bool utf16 )
{
	/**** バッファを作成する ****/
	uint_t wSize = WriteAAFile( pAaFile, NULL, 0 );
	wchar_t* writeBuffer = new wchar_t[ wSize ];
	wSize = WriteAAFile( pAaFile, writeBuffer, wSize );


	/**** ファイルを生成する ****/
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


	/**** バッファの廃棄 ****/
	if( writeBuffer )
	{
		delete[] writeBuffer;
	}


	return wSize;
}

/**
* AAFileの情報に従って、データをメモリ上に書き出す
*	strがNULLの場合、格納処理は行わず、必要な要素数だけ返す。
*	@param[in]	pAaFile				書き出す情報を纏めたAAFile
*	@param[out]	str					文字列の格納先
*	@param[in]	length				strの容量(wchar_t単位の要素数)
*	@return		uint_t				実際に書き出した文字数
*/
uint_t AAFileWriter::WriteAAFile( const AAFile* pAaFile, wchar_t* str, uint_t length )
{
#define CALL_WRITEFUNC( type )	case AFT_##type:												\
if( str )ret += WritePage##type( &pAaFile->pageList[ i ], &str[ ret ], length - ret, last );	\
else	 ret += WritePage##type( &pAaFile->pageList[ i ], NULL, 0, last );	break


	uint_t ret = 0;


	/**** ページごとに処理をする ****/
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
* 改行コードを指定
*	@param[in]	lfString			改行コード(L'\0'終端である必要があります)
*/
void AAFileWriter::SetLineField( const wchar_t* lfString )
{
	cs_LineFieldString = lfString;
	s_LineFieldLength = wcslen( lfString );
}

/**
* 改行コードを取得
*	@return		const wchar_t*		改行コード
*/
const wchar_t* AAFileWriter::GetLineField( void )
{
	return cs_LineFieldString;
}

/**
* ページ内容のみ書き出す
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageValue( AAFilePage* page, wchar_t* writeString, uint_t length )
{
	uint_t ret = 0;

	/**** 一文字ずつ判定していく ****/
	for( uint_t i = 0; i < page->valueLength; i++ )
	{
		uint_t step = CheckLineField( &page->value[ i ], page->valueLength - i );
		if( step == 0 )
		{
			if( writeString )
			{// 文字書き出し
				if( ret >= length ) break;
				writeString[ ret ] = page->value[ i ];
			}
			ret++;
		}
		else
		{
			if( writeString )
			{// 文字書き出し
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
* TXTファイルの書き出し
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageTXT( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;

	// ページ内容を書き出す
	ret = WritePageValue( page, writeString, length );

	// ページの終端に改行を挟む
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
* MLTファイルの書き出し
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageMLT( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** ページ内容を書き出す ****/
	ret = WritePageValue( page, writeString, length );


	/**** 改行と分離文字書き出し ****/
	const wchar_t* split = L"[SPLIT]";
	uint_t splitLen = wcslen( split );

	// 分離文字前の改行
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	// 最終ページなら分離文字列は出力しない
	if( !lastPage )
	{
		// 分離文字
		if( writeString )
		{
			if( length - ret < splitLen ) return ret;
			for( uint_t s = 0; s < splitLen; s++ )
				writeString[ ret + s ] = split[ s ];
		}
		ret += splitLen;

		// 分離文字後の改行
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
* ASTファイルの書き出し
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageAST( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** 改行と分離文字書き出し ****/
	const wchar_t* split = L"[AA][";
	uint_t splitLen = wcslen( split );

	// 分離文字とページ名
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

	// 分離文字後の改行
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	/**** ページ内容を書き出す ****/
	if( writeString )	ret += WritePageValue( page, &writeString[ ret ], length - ret );
	else				ret += WritePageValue( page, NULL, 0 );

	// ページの終端に改行を挟む
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
* ASDファイルの書き出し
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageASD( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** ページ内容を書き出す ****/
	// 改行コード偽装
	const wchar_t* oldLF = cs_LineFieldString;
	uint_t oldLen = s_LineFieldLength;
	cs_LineFieldString = L"\x1\x1";
	s_LineFieldLength = 2;

	// 実データ書き出し
	ret += WritePageValue( page, writeString, length );

	// 改行コードの偽装終了
	cs_LineFieldString = oldLF;
	s_LineFieldLength = oldLen;


	/**** ページ名書き出し ****/
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
* AADataファイルの書き出し
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageAADATA( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	return WritePageAADataList( L"[MojieName=", L"[END]", page, writeString, length, lastPage );
}

/**
* AAListファイルの書き出し
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageAALIST( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	return WritePageAADataList( L"[ListName=", L"[end]", page, writeString, length, lastPage );
}

/**
* AADataファイルの書き出し
*	@param[in]	split				分割文字列
*	@param[in]	end					終端分割文字列
*	@param[in]	page				ページデータ
*	@param[out]	writeString			書き出し先
*	@param[in]	length				書き出し先の文字数
*	@param[in]	lastPage			最終ページなら真
*	@return		uint_t				書き出した文字数
*/
uint_t WritePageAADataList( const wchar_t* split, const wchar_t* end, AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage )
{
	uint_t ret = 0;


	/**** 改行と分離文字書き出し ****/
	uint_t splitLen = wcslen( split );

	// 分離文字とページ名
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

	// 分離文字後の改行
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	/**** ページ内容を書き出す ****/
	if( writeString )	ret += WritePageValue( page, &writeString[ ret ], length - ret );
	else				ret += WritePageValue( page, NULL, 0 );

	// ページの終端に改行を挟む
	if( writeString )
	{
		if( length - ret < s_LineFieldLength ) return ret;
		for( uint_t l = 0; l < s_LineFieldLength; l++ )
			writeString[ ret + l ] = cs_LineFieldString[ l ];
	}
	ret += s_LineFieldLength;


	/**** 終了タグを書き込む ****/
	uint_t endLen = wcslen( end );
	if( writeString )
	{
		if( length - ret < endLen ) return ret;
		for( uint_t l = 0; l < endLen; l++ )
			writeString[ ret + l ] = end[ l ];
	}
	ret += endLen;


	// 終端に改行を挟む
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
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
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
