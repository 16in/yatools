/*--------------------------------------------------------------------------------------------
*
* MLT2PNG  AA関係のファイルをPNGファイル形式へ変換するコンバータ
*
*---------------------------------------------------------------------------------------------*/
/*-- 更新履歴 --------------------------------------------------------------------------------
*
*---------------------------------------------------------------------------------------------*/
#include <Windows.h>
#include <locale.h>
#include <shlwapi.h>
#include <vector>
#include <string>
#include "convert.h"


/** オプション解析構造体 */
struct Option
{
	union
	{
		unsigned long			flags;
		struct
		{
			bool				enableOutputDirectory;			//!< 出力先ディレクトリが有効か
		};
	};

	std::vector<std::string>	inputFiles;						//!< 入力ファイル一覧
	std::string					outputDirectory;				//!< 出力先ディレクトリ
	int							width;							//!< 出力画像横幅(0以下の場合、AAの横幅)
	int							height;							//!< 出力画像縦幅(0以下の場合、AAの縦幅)
	std::string					fontName;						//!< フォント名
	int							fontSize;						//!< フォントサイズ

	Option( void )
		: flags( 0 )
	{
		inputFiles.clear( );
		outputDirectory.clear( );
		width = 0;
		height = 0;
		fontName = "";
		fontSize = 0;
	}
};

/**
* @brief オプション解析
* @param[out]	option				オプション解析データ格納先
* @param[in]	argc				コマンドライン引数の個数
* @param[in]	argv				コマンドライン引数配列
* @details
*	コマンドライン引数からオプションを解析して格納します。
*/
void convCommandLine( Option& option, int argc, char* argv[] )
{
	for( int i = 1; i < argc; i++ )
	{
		// 出力ファイル
		if( i + 1 < argc && strcmp( argv[ i ], "-o" ) == 0 )
		{
			char* outputDirectory = argv[ i + 1 ];
			if( (PathFileExistsA( outputDirectory ) && PathIsDirectoryA( outputDirectory )) || !PathFileExistsA( outputDirectory ) )
			{
				option.enableOutputDirectory = true;
				option.outputDirectory = argv[ ++i ];
				continue;
			}
		}

		// 横幅
		if( i + 1 < argc && strcmp( argv[ i ], "-w" ) == 0 )
		{
			option.width = atoi( argv[ i + 1 ] );
			if( option.width <= 0 )
			{
				option.width = 0;
			}
		}

		// 縦幅
		if( i + 1 < argc && strcmp( argv[ i ], "-h" ) == 0 )
		{
			option.height = atoi( argv[ i + 1 ] );
			if( option.height <= 0 )
			{
				option.height = 0;
			}
		}

		// フォント名
		if( i + 1 < argc && strcmp( argv[ i ], "-f" ) == 0 )
		{
			option.fontName = argv[ i + 1 ];
		}

		// フォントサイズ
		if( i + 1 < argc && strcmp( argv[ i ], "-fs" ) == 0 )
		{
			option.fontSize = atoi( argv[ i + 1 ] );
		}

		// 入力ファイル
		if( PathFileExistsA( argv[ i ] ) && !PathIsDirectoryA( argv[ i ] ) )
		{
			option.inputFiles.push_back( argv[ i ] );
		}
	}
}


/**
* @brief エントリポイント
* @param[in] argc					コマンドライン引数の個数
* @param[in] argv					コマンドライン引数
*/
int main( int argc, char* argv[] )
{
	setlocale( LC_ALL, "jpn" );
	

	/*---- オプション解析 ----*/
	Option opt;
	convCommandLine( opt, argc, argv );


	/*---- 変換実行 ----*/
	// コンバータセットアップ
	mlt2png::Converter::Setup( opt.fontName.c_str( ), opt.fontSize );

	// 入力ファイルごとに実行
	for( int i = 0; i < opt.inputFiles.size( ); i++ )
	{
		char outputPath[ MAX_PATH ] = "./";
		if( opt.enableOutputDirectory )
		{
			sprintf_s( outputPath, "%s", opt.outputDirectory.c_str( ) );
		}
		std::string fileName = PathFindFileNameA( opt.inputFiles[ i ].c_str( ) );
		size_t pos = 0;
		while( (pos = fileName.find( '.' )) != std::string::npos )
		{
			fileName[ pos ] = '_';
		}
		sprintf_s( outputPath, "%s/%s/", outputPath, fileName.c_str() );

		mlt2png::Converter::Run( opt.inputFiles[ i ].c_str( ), outputPath );
	}

	// コンバータクリンナップ
	mlt2png::Converter::Cleanup( );


    return 0;
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
/*----------------------------------------------------------------------
* MLT2PNGのライセンス
*
*	上記ライセンスのOpen Source Group Japanによる日本語訳となります。
*	引用元は下記URLです。
*	http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
*
*	Copyright (c) 2019 _16in/◆7N5y1wtOn2
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