/*----------------------------------------------------------------------
*
* AA関連ファイルアクセサの内部共通関数
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include <aafile/AAFileAccessor.h>
#include "types.h"


namespace aafile {


/**
* ファイルタイプ取得
*	@param[in]	str					判別用文字列
*	@param[in]	length				文字列長
*	@return		AAFileType			ファイルタイプ
*/
AAFileType CheckFileType( const wchar_t* str, uint_t length );

/**
* 渡された文字列の先頭が改行コードか
*	@param[in]	str					文字列
*	@param[in]	length				文字列長
*	@return		uint_t				改行文字数("\r\n":2 "\r","\n":1 それ以外は0)
*/
uint_t CheckLineField( const wchar_t* str, uint_t length );


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
