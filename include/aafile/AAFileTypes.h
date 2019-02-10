/*----------------------------------------------------------------------
*
* AA関連ファイルアクセス用定義類
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include <stdint.h>


namespace aafile {

/** ページ数型 */
typedef uint_t AAPageCount;

/** 文字列長型 */
typedef uint_t StringLength;


/**
* @brief ファイル形式列挙体
*/
enum AAFileType
{
	AFT_TXT		= 0x00,			//!< 純粋なテキストファイル(.txt)
	AFT_MLT,					//!< MLTファイル(.mlt)
	AFT_AST,					//!< ASTファイル(.ast)
	AFT_ASD,					//!< ASDファイル(.asd)
	AFT_AADATA,					//!< AADataファイル(.txt)
	AFT_AALIST,					//!< AAListファイル(.txt)

	AAFILETYPE_MAX
};

/**
* @brief ページデータ構造体
*/
struct AAFilePage
{
	const wchar_t*		name;			//!< ページ名
	const wchar_t*		value;			//!< ページ内容
	StringLength		nameLength;		//!< ページ名文字列長
	StringLength		valueLength;	//!< ページ内容文字列長
};

/**
* @brief ファイル全体の構造体
*/
struct AAFile
{
	AAFileType			type;			//!< ファイルタイプ
	AAFilePage*			pageList;		//!< ページデータの一覧
	AAPageCount			pageCount;		//!< ページ数

	const wchar_t*		fileData;		//!< ファイル読み込みから生成した場合のファイル内容
										///  文字列から生成した場合はNULL
};


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
