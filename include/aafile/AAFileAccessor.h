/*----------------------------------------------------------------------
*
* AA関連ファイルアクセサ
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include "aatype.h"
#include "AAFileTypes.h"


namespace aafile {


/**
* AAファイルリーダ
*/
class AAFileReader
{
public:
	/**
	* AAFileをファイルから生成する
	*	(ASDの場合は改行コードを\r\nに置き換えます)
	*	@param[in]	filePath			ファイルパス
	*	@return		AAFile*				読み込まれたファイルデータ
	*									読み込みに失敗した場合はNULL
	*/
	static AAFile* CreateAAFileFromFile( const wchar_t* filePath );

	/**
	* AAFileを内容文字列から生成する
	*	(ASDの場合は改行コードを\r\nに置き換えます)
	*	@param[in]	str					内容文字列
	*	@param[in]	length				文字列長
	*	@return		AAFile*				読み込まれたファイルデータ
	*									読み込みに失敗した場合はNULL
	*/
	static AAFile* CreateAAFile( wchar_t* str, uint_t length );

	/**
	* 生成されたAAFileを廃棄する
	*	@param[in]	pAaFile				廃棄するAAFileのインスタンス
	*/
	static void ReleaseAAFile( AAFile* pAaFile );
};


/**
* AAファイルライタ
*/
class AAFileWriter
{
public:
	/**
	* @biref 指定のファイルにファイル全体の内容を書き出す
	*	AAFileの情報に従って、データをファイルに書き出します。
	*
	*	@param[in]	pAaFile				書き出す情報を纏めたAAFile
	*	@param[in]	filePath			書き出すファイルへのパス
	*	@param[in]	utf16				UTF16-LEとして書き出すなら真、それ以外は偽
	*	@return		uint_t				実際に書き出した文字数
	*/
	static uint_t WriteAAFileToFile( const AAFile* pAaFile, const wchar_t* filePath, bool utf16 = false );

	/**
	* @biref メモリ上にファイル全体の内容を書き出す
	*	AAFileの情報に従って、データをメモリ上に書き出します。
	*	strがNULLの場合、格納処理は行わず、必要な要素数だけ返します。
	*
	*	@param[in]	pAaFile				書き出す情報を纏めたAAFile
	*	@param[out]	str					文字列の格納先
	*	@param[in]	length				strの容量(wchar_t単位の要素数)
	*	@return		uint_t				実際に書き出した文字数
	*/
	static uint_t WriteAAFile( const AAFile* pAaFile, wchar_t* str, uint_t length );

	/**
	* @brief 改行コードを指定
	*	AAFileWriterに改行コードを設定します。
	*
	*	@param[in]	lfString			改行コード(L'\0'終端である必要があります)
	*/
	static void SetLineField( const wchar_t* lfString );

	/**
	* @brief 改行コードを取得
	*	AAFileWriterに設定されている改行コードを取得します。
	*
	*	@return		const wchar_t*		改行コード
	*/
	static const wchar_t* GetLineField( void );
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
/*----------------------------------------------------------------------
* ライセンス
*
*	上記ライセンスのOpen Source Group Japanによる日本語訳となります。
*	引用元は下記URLです。
*	http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
*
*	Copyright (c) 2012-2019 _16in/◆7N5y1wtOn2
*
*	以下に定める条件に従い、
*	本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）
*	の複製を取得するすべての人に対し、
*	ソフトウェアを無制限に扱うことを無償で許可します。
*	これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、
*	サブライセンス、および/または販売する権利、
*	およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。
*
*	上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製
*	または重要な部分に記載するものとします。
*
*	ソフトウェアは「現状のまま」で、
*	明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。
*	ここでいう保証とは、商品性、特定の目的への適合性、
*	および権利非侵害についての保証も含みますが、
*	それに限定されるものではありません。
*	作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、
*	ソフトウェアに起因または関連し、あるいはソフトウェアの使用
*	またはその他の扱いによって生じる一切の請求、損害、
*	その他の義務について何らの責任も負わないものとします。
*
*----------------------------------------------------------------------*/
