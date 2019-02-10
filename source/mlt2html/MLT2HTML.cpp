/*--------------------------------------------------------------------------------------------
*
* MLT2HTML  AA関係のファイルをHTMLファイル形式へ変換するコンバータ
*
*---------------------------------------------------------------------------------------------*/
/*-- 更新履歴 --------------------------------------------------------------------------------
*
*	2013/07/05 ver2.02
*		当初の予定であったMIT/Xライセンスにて公開。
*		Readme修正、Licenseファイル追加
*
*	2012/12/23 ver2.01
*		MLT終端に特定の書式で[SPLIT]が置いてあると停止するバグを修正した
*		AAFileAccesserの適用
*
*	2012/12/05 ver2.0
*		ver1.0系列を廃棄、ver2.0系列を新規に起こす。
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

	Option( void )
		: flags( 0 )
	{
		inputFiles.clear();
		outputDirectory.clear();
	}
};

/**
* オプション解析
*	@param[out]	option				オプション解析データ格納先
*	@param[in]	argc				コマンドライン引数の個数
*	@param[in]	argv				コマンドライン引数配列
*/
void convCommandLine( Option& option, int argc, char* argv[] )
{
	for( int i = 1; i < argc; i++ )
	{
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

		if( PathFileExistsA( argv[ i ] ) && !PathIsDirectoryA( argv[ i ] ) )
		{
			option.inputFiles.push_back( argv[ i ] );
		}
	}
}

/**
* エントリーポイント
*/
int main( int argc, char* argv[] )
{
	setlocale( LC_ALL, "jpn" );

	Option opt;
	convCommandLine( opt, argc, argv );

#ifdef _DEBUG
	puts( "option:" );
	if( opt.enableOutputDirectory )
	{
		printf( "  output:%s\n", opt.outputDirectory.c_str() );
	}
	for( size_t i = 0; i < opt.inputFiles.size(); i++ )
	{
		printf( "  file[%d]:%s\n", i + 1, opt.inputFiles[ i ].c_str() );
	}
#endif

	if( opt.inputFiles.size() > 0 )
	{
		// templateのセットアップ
#ifdef _DEBUG
		char appDirectory[ MAX_PATH ];
		::GetCurrentDirectoryA( MAX_PATH, appDirectory );
#else
		char* appDirectory = new char[ strlen( argv[ 0 ] ) + 1 ];
		strcpy( appDirectory, argv[ 0 ] );
		PathRemoveFileSpecA( appDirectory );
#endif
		convert::SetupTemplate( appDirectory );

		// 出力先ディレクトリが存在しなければ作っておく
		if( opt.enableOutputDirectory && !PathFileExistsA( opt.outputDirectory.c_str() ) )
		{
			if( !CreateDirectoryA( opt.outputDirectory.c_str(), NULL ) )
			{
				fprintf( stderr, "出力先ディレクトリ:%s\nの生成に失敗しました。", opt.outputDirectory.c_str() );
				opt.enableOutputDirectory = false;
			}
		}

		// ファイルを全て変換
		for( size_t i = 0; i < opt.inputFiles.size(); i++ )
		{
			char input[ MAX_PATH ];
			char output[ MAX_PATH ];

			strcpy( input, opt.inputFiles[ i ].c_str() );
			if( !opt.enableOutputDirectory )
			{
				strcpy( output, opt.inputFiles[ i ].c_str() );
				PathRenameExtensionA( output, ".html" );
			}
			else
			{
				sprintf( output, "%s\\%s", opt.outputDirectory.c_str(), PathFindFileNameA( opt.inputFiles[ i ].c_str() ) );
				PathRenameExtensionA( output, ".html" );
			}

			convert::Run( input, output );
		}

		// templateのクリンナップ
		convert::CleanupTemplate();
	}


	return 0;
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
/*----------------------------------------------------------------------
* MLT2HTMLのライセンス
*
*	上記ライセンスのOpen Source Group Japanによる日本語訳となります。
*	引用元は下記URLです。
*	http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
*
*	Copyright (c) 2012 _16in/◆7N5y1wtOn2
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
