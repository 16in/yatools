/*----------------------------------------------------------------------
*
* AA関連ファイルリーダから参照するローカル関数
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include <aafile/AAFileAccessor.h>
#include "types.h"


namespace aafile {


/**
* MLTファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
static void SplitMLT( AAFilePageArray& pageArray, const wchar_t* str, uint_t length );

/**
* ASTファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
static void SplitAST( AAFilePageArray& pageArray, const wchar_t* str, uint_t length );

/**
* ASDファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
static void SplitASD( AAFilePageArray& pageArray, wchar_t* str, uint_t length );

/**
* AADataファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
static void SplitAADATA( AAFilePageArray& pageArray, const wchar_t* str, uint_t length );

/**
* AAListファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*/
static void SplitAALIST( AAFilePageArray& pageArray, const wchar_t* str, uint_t length );

/**
* AAList/AADataファイルを分割
*	@param[in]	pageArray			ページデータ格納先
*	@param[in]	str					分割対象文字列
*	@param[in]	length				文字列長
*	@param[in]	start				先頭分割文字列
*	@param[in]	end					終端分割文字列
*/
static void SplitAAListData( AAFilePageArray& pageArray, const wchar_t* str, uint_t length, const wchar_t* start, const wchar_t* end );


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
