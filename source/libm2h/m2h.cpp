#include "m2h.h"
#include "convert.h"


extern "C" {


/**
* @fn M2H_IMPORT void M2H_CALL M2H_SetupTemplate( const char* appdir )
* @brief html生成templateのセットアップ
*	$(appdir)/templateに在るtemplateファイルをセットアップします。
*
*	@param[in]	appdir					アプリケーションの在るディレクトリ
*/
M2H_IMPORT void M2H_CALL M2H_SetupTemplate( const char* appdir )
{
	convert::SetupTemplate( appdir );
}

/**
* @fn M2H_IMPORT void M2H_CALL M2H_CleanupTemplate( void )
* @brief html生成templateのクリンナップ
*	セットアップ済みのtemplateをクリンナップします。
*/
M2H_IMPORT void M2H_CALL M2H_CleanupTemplate( void )
{
	convert::CleanupTemplate();
}

/**
* @fn M2H_IMPORT void M2H_CALL M2H_Run( const char* inputFile, const char* outputFile )
* @brief html生成ルーチンの実行
*	セットアップ済みのtemplateと入力ファイルからhtmlファイルを生成します。
*
*	@param[in]	inputFile				入力ファイルパス
*	@param[in]	outputFile				出力ファイルパス
*/
M2H_IMPORT void M2H_CALL M2H_Run( const char* inputFile, const char* outputFile )
{
	convert::Run( inputFile, outputFile );
}

/**
* @fn M2H_IMPORT void M2H_CALL M2H_RunString( const char* outputFile, const char* srcString, unsigned long length )
* @brief html生成ルーチンの実行
*	セットアップ済みのtemplateと入力文字列からhtmlファイルを生成します。
*
*	@param[in]	outputFile				出力ファイルパス
*	@param[in]	srcString				入力内容文字列
*	@param[in]	length					入力内容の文字列長
*/
M2H_IMPORT void M2H_CALL M2H_RunString( const char* outputFile, const char* srcString, unsigned long length )
{
	convert::RunString( outputFile, srcString, length );
}



}




/*----------------------------------------------------------------------
*	libM2H
*
*	Copyright (c) 2013 _16in/◆7N5y1wtOn2
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
