/*----------------------------------------------------------------------
*
* AA関連ファイルリーダ
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#include <aafile/AAFileAccessor.h>
#include <Windows.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "localReader.h"


namespace aafile {


/**** AAファイルリーダ ****/
/**
* AAFileをファイルから生成する
*	@param[in]	filePath			ファイルパス
*	@return		AAFile*				読み込まれたファイルデータ
*									読み込みに失敗した場合はNULL
*/
AAFile* AAFileReader::CreateAAFileFromFile( const wchar_t* filePath )
{
	AAFile*			ret = NULL;
	unsigned char*	fileBuffer = NULL;
	DWORD			fileSize = 0;


	/**** ファイルを読み込む ****/
	HANDLE hFile = CreateFileW( filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		fileSize = GetFileSize( hFile, NULL );
		fileBuffer = new unsigned char[ fileSize ];
		ReadFile( hFile, fileBuffer, fileSize, &fileSize, NULL );
		CloseHandle( hFile );
	}


	/**** wchar_t型に変換 ****/
	if( fileBuffer == NULL ) return NULL;


	/** BOMチェック **/
	unsigned short BOM = ((unsigned short*)fileBuffer)[ 0 ];
	if( BOM == 0xFFFE )
	{// BEであったら処理しない
		delete[] fileBuffer;
		return NULL;
	}
	if( BOM == 0xFEFF )
	{// UTF-16 LEであればそのまま流し込む
		ret = CreateAAFile( &((wchar_t*)fileBuffer)[ 1 ], (fileSize-2) / sizeof( wchar_t ) );
		if( ret )	ret->fileData = (wchar_t*)fileBuffer;
		else		delete[] fileBuffer;
	}
	else
	{// UTF-16でなければS-JISと仮定して読み込む
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
* AAFileを内容文字列から生成する
*	@param[in]	str					内容文字列
*	@param[in]	length				文字列長
*	@return		AAFile*				読み込まれたファイルデータ
*									読み込みに失敗した場合はNULL
*/
AAFile* AAFileReader::CreateAAFile( wchar_t* str, uint_t length )
{
#define CALL_SPLIT( type ) case AFT_##type: Split##type( pageArray, str, length ); break
	AAFile* ret = NULL;
	AAFilePageArray pageArray;

	if( str && length > 0 )
	{
		/**** ファイルタイプ識別 ****/
		AAFileType type = CheckFileType( str, length );


		/**** ファイルタイプ別にページ分割する ****/
		pageArray.clear();
		switch( type )
		{
		CALL_SPLIT( MLT );
		CALL_SPLIT( AST );
		CALL_SPLIT( ASD );
		CALL_SPLIT( AADATA );
		CALL_SPLIT( AALIST );
		case AFT_TXT:
			{// txtファイル
				AAFilePage page;
				page.name = NULL;
				page.nameLength = 0;
				page.value = str;
				page.valueLength = length;
				pageArray.push_back( page );
			}
			break;
		}


		/**** ページリストの情報を書き込む ****/
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
* 生成されたAAFileを廃棄する
*	@param[in]	pAaFile				廃棄するAAFileのインスタンス
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
* MLTファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
void SplitMLT( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	AAFilePage page;
	const wchar_t* split = L"[SPLIT]";
	size_t len = wcslen( split );

	
	/**** 最初の要素を作る ****/
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
				/**** ページ長を算出して追加 ****/
				page.valueLength = &str[ i ] - page.value;
				if( page.valueLength > 0 )
				{
					pageArray.push_back( page );
				}

				// 改行までぶん回す
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


				/**** 次のページへ移動 ****/
				page.value = &str[ i + 1 ];
			}
		}
	}


	/**** 最終ページ長を算出して追加 ****/
	if( !(str[ length - 1 ] == L']' && _wcsnicmp( page.value, split, len ) == 0) )
	{
		if( str[ length - 1 ] == L'\n' || str[ length - 1 ] == L'\r' )
		{// 最後の改行は無視する
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
* ASTファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
void SplitAST( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	const wchar_t* split = L"[AA]";
	size_t len = wcslen( split );
	AAFilePage page;

	
	/**** 最初の要素を構築 ****/
	uint_t i = 0;

	if( _wcsnicmp( str, split, len ) == 0 )
	{
		i += len;

		// ページ名情報を得る
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

		// ページ情報を得る
		page.value = &str[ i ];
	}


	/**** 以降の要素を追加していく ****/
	for( ; i < length; i++ )
	{
		uint_t step = CheckLineField( &str[ i ], length - i );
		if( step > 0 )
		{
			if( _wcsnicmp( &str[ i + step ], split, len ) == 0 )
			{
				/** ページ追加処理 **/
				page.valueLength = &str[ i ] - page.value;
				pageArray.push_back( page );
				i += step + len;
				
				/** ページ名情報を得る **/
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

				/** ページ情報を得る **/
				page.value = &str[ i ];
			}
		}
	}


	/**** 最終ページ長を算出して追加 ****/
	if( str[ length - 1 ] == L'\n' || str[ length - 1 ] == L'\r' )
	{// 最後の改行は無視する
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
* ASDファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
void SplitASD( AAFilePageArray& pageArray, wchar_t* str, uint_t length )
{
	const wchar_t* split = L"\x2\x2";
	const wchar_t* lf = L"\x1\x1";
	size_t len = 2;
	AAFilePage page;


	/**** 最初の要素を指定 ****/
	page.name = NULL;
	page.nameLength = page.valueLength = 0;
	page.value = str;


	/**** 文字終端まで推し進める ****/
	for( size_t i = 0; i < length; i++ )
	{
		if( _wcsnicmp( lf, &str[ i ], len ) == 0 )
		{// 改行と一致
			str[ i ] = L'\r'; str[ i + 1 ] = L'\n';
		}
		else if( _wcsnicmp( &str[ i ], split, len ) == 0 )
		{// 分離文字列と一致
			page.valueLength = &str[ i ] - page.value;
			page.name = &str[ (i += len) ];

			/**** 改行までループ ****/
			uint_t step = CheckLineField( &str[ i ], length - i );
			for( ; i < length; i++ )
			{
				step = CheckLineField( &str[ i ], length - i );
				if( step > 0 ) break;
			}


			/**** ページ名文字列長を確定し、追加する ****/
			page.nameLength = &str[ i ] - page.name;
			pageArray.push_back( page );
			i += step;


			/**** 次のページへ移動 ****/
			page.value = &str[ i + 1 ];
		}
	}
}

/**
* AADataファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
void SplitAADATA( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	SplitAAListData( pageArray, str, length, L"[MojieName=", L"[END]" );
}

/**
* AAListファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
void SplitAALIST( AAFilePageArray& pageArray, const wchar_t* str, uint_t length )
{
	SplitAAListData( pageArray, str, length, L"[ListName=", L"[END]" );
}

/**
* AAList/AADataファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*	@param[in]	start				先頭分割文字列
*	@param[in]	end					終端分割文字列
*/
void SplitAAListData( AAFilePageArray& pageArray, const wchar_t* str, uint_t length, const wchar_t* start, const wchar_t* end )
{
	size_t startLen = wcslen( start );
	size_t endLen = wcslen( end );
	AAFilePage page;
	page.name = page.value = NULL;
	page.nameLength = page.valueLength = 0;

	
	/**** 最初の要素を判定 ****/
	uint_t i = 0;
	if( _wcsnicmp( start, str, startLen ) == 0 )
	{
		i += startLen;
		page.name = &str[ i ];
		for( ; str[ i ] != L']'; i++ );
		page.nameLength = &str[ i ] - page.name;

		// 改行までぶん回す
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


	/**** 以降の要素を判定 ****/
	for( ; i < length; i++ )
	{
		uint_t step = CheckLineField( &str[ i ], length - i );
		if( step > 0 )
		{
			if( _wcsnicmp( &str[ i + step ], end, endLen ) == 0 )
			{// ページ要素を確定して追加
				page.valueLength = &str[ i ] - page.value;
				pageArray.push_back( page );
				
				i += step;
			}
			else if( _wcsnicmp( start, &str[ i + step ], startLen ) == 0 )
			{// ページ情報を格納する
				i += startLen + step;
				page.name = &str[ i ];
				for( ; str[ i ] != L']'; i++ );
				page.nameLength = &str[ i ] - page.name;

				// 改行までぶん回す
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
