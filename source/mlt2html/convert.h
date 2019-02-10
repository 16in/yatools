/*--------------------------------------------------------------------------------------------
*
* MLT2HTML  コンバータヘッダ
*
*---------------------------------------------------------------------------------------------*/
/*-- 更新履歴 --------------------------------------------------------------------------------
*
*	2012/12/05
*		コンバータ実装 ver1.0
*
*---------------------------------------------------------------------------------------------*/
#pragma once


namespace convert
{


/**
* @brief html生成templateのセットアップ
*	$(appdir)/templateに在るtemplateファイルをセットアップします。
*
*	@param[in]	appdir					アプリケーションの在るディレクトリ
*/
void SetupTemplate( const char* appdir );

/**
* @brief html生成templateのクリンナップ
*	セットアップ済みのtemplateをクリンナップします。
*/
void CleanupTemplate( void );

/**
* @brief html生成ルーチンの実行
*	セットアップ済みのtemplateと入力ファイルからhtmlファイルを生成します。
*
*	@param[in]	inputFile				入力ファイルパス
*	@param[in]	outputFile				出力ファイルパス
*/
void Run( const char* inputFile, const char* outputFile );

/**
* @brief html生成ルーチンの実行
*	セットアップ済みのtemplateと入力文字列からhtmlファイルを生成します。
*
*	@param[in]	outputFile				出力ファイルパス
*	@param[in]	srcString				入力内容文字列
*	@param[in]	length					入力内容の文字列長
*/
void RunString( const char* outputFile, const char* srcString, unsigned long length );


}



/*----------------------------------------------------------------------
*	MLT2HTML
*
*	Copyright (c) 2012 _16in/◆7N5y1wtOn2
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
