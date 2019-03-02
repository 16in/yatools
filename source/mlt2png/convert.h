/*--------------------------------------------------------------------------------------------
*
* MLT2PNG  コンバータヘッダ
*
*---------------------------------------------------------------------------------------------*/
/*-- 更新履歴 --------------------------------------------------------------------------------
*
*	2019/03/02
*		コンバータ実装 ver1.0
*
*---------------------------------------------------------------------------------------------*/
#pragma once

#include <Windows.h>
#include <stdint.h>
#include <aafile/AAFileAccessor.h>


namespace mlt2png
{


	/**
	* @class Converter
	* @brief コンバータ
	* @details
	*	AA関係ファイルから画像へコンバートします。
	*/
	class Converter
	{
	public:
		/**
		* @name セットアップ/クリンナップ
		* @{
		*/
		/**
		* @brief セットアップ
		* @param[in] fontName				フォント名		\n
		*									NULL、空文字列の場合は使用可能なフォントを検索します。
		* @param[in] fontSize				フォントサイズ	\n
		*									0の場合は-16が指定されます。
		* @retval true						セットアップに成功
		* @retval false						セットアップに失敗
		* @details
		*	コンバータをセットアップします。
		*/
		static bool Setup( const char* fontName, int fontSize );

		/**
		* @brief クリンナップ
		* @details
		*	コンバータをクリーンナップします。
		*/
		static void Cleanup( void );

		/**
		* @}
		*/
		/**
		* @name 実行
		* @{
		*/
		/**
		* @brief PNG生成ルーチンの実行
		* @param[in] inputFile				入力ファイルパス
		* @param[in] outputDirectory		出力ディレクトリ
		*/
		static void Run( const char* inputFile, const char* outputDirectory );

		/**
		* @brief PNG生成ルーチンの実行
		* @param[in] outputDirectory		出力ディレクトリ
		* @param[in]	srcString				入力内容文字列
		* @param[in]	length					入力内容の文字列長
		*/
		static void RunString( const char* outputDirectory, const char* srcString, unsigned long length );

		/**
		* @}
		*/

	private:
		// 全ページ書き出し
		static void paintAllPage( aafile::AAFile* file, const char* outputDirectory );

		// ページ書き込み
		static void paintPage( HDC hDC, aafile::AAFilePage* page, int width, int height, int lfCount );

		// 頁サイズを計算
		static void getPageSize( aafile::AAFilePage* page, int& width, int& height, int& lfCount );

		// 先頭文字の文字幅と文字コードを取得
		static int getCharWidth( const wchar_t* str, size_t length, unsigned long& step, wchar_t* ch = NULL );

		// 参照文字列かチェックし、参照文字列ならば参照を解決する
		static bool CheckRefChar( const wchar_t* str, unsigned long length, wchar_t refChar[ 2 ], unsigned long& step, unsigned long& refCount );


	private:
		static HFONT		m_hFont;						//!< フォントハンドル
		static int32_t		m_CharacterWidth[ 0x10000 ];	//!< 文字ごとの横幅
		static int32_t		m_CharacterHeight;				//!< 文字縦幅

	};
	


}



/*----------------------------------------------------------------------
*	MLT2PNG
*
*	Copyright (c) 2019 _16in/◆7N5y1wtOn2
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
