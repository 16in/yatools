/*--------------------------------------------------------------------------------------------
*
* MLT2HTML  AA�֌W�̃t�@�C����HTML�t�@�C���`���֕ϊ�����R���o�[�^
*
*---------------------------------------------------------------------------------------------*/
/*-- �X�V���� --------------------------------------------------------------------------------
*
*	2013/07/05 ver2.02
*		�����̗\��ł�����MIT/X���C�Z���X�ɂČ��J�B
*		Readme�C���ALicense�t�@�C���ǉ�
*
*	2012/12/23 ver2.01
*		MLT�I�[�ɓ���̏�����[SPLIT]���u���Ă���ƒ�~����o�O���C������
*		AAFileAccesser�̓K�p
*
*	2012/12/05 ver2.0
*		ver1.0�n���p���Aver2.0�n���V�K�ɋN�����B
*
*---------------------------------------------------------------------------------------------*/
#include <Windows.h>
#include <locale.h>
#include <shlwapi.h>
#include <vector>
#include <string>
#include "convert.h"


/** �I�v�V������͍\���� */
struct Option
{
	union
	{
		unsigned long			flags;
		struct
		{
			bool				enableOutputDirectory;			//!< �o�͐�f�B���N�g�����L����
		};
	};

	std::vector<std::string>	inputFiles;						//!< ���̓t�@�C���ꗗ
	std::string					outputDirectory;				//!< �o�͐�f�B���N�g��

	Option( void )
		: flags( 0 )
	{
		inputFiles.clear();
		outputDirectory.clear();
	}
};

/**
* �I�v�V�������
*	@param[out]	option				�I�v�V������̓f�[�^�i�[��
*	@param[in]	argc				�R�}���h���C�������̌�
*	@param[in]	argv				�R�}���h���C�������z��
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
* �G���g���[�|�C���g
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
		// template�̃Z�b�g�A�b�v
#ifdef _DEBUG
		char appDirectory[ MAX_PATH ];
		::GetCurrentDirectoryA( MAX_PATH, appDirectory );
#else
		char* appDirectory = new char[ strlen( argv[ 0 ] ) + 1 ];
		strcpy( appDirectory, argv[ 0 ] );
		PathRemoveFileSpecA( appDirectory );
#endif
		convert::SetupTemplate( appDirectory );

		// �o�͐�f�B���N�g�������݂��Ȃ���΍���Ă���
		if( opt.enableOutputDirectory && !PathFileExistsA( opt.outputDirectory.c_str() ) )
		{
			if( !CreateDirectoryA( opt.outputDirectory.c_str(), NULL ) )
			{
				fprintf( stderr, "�o�͐�f�B���N�g��:%s\n�̐����Ɏ��s���܂����B", opt.outputDirectory.c_str() );
				opt.enableOutputDirectory = false;
			}
		}

		// �t�@�C����S�ĕϊ�
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

		// template�̃N�����i�b�v
		convert::CleanupTemplate();
	}


	return 0;
}


/*----------------------------------------------------------------------
*	MLT2HTML
*
*	Copyright (c) 2012 _16in/��7N5y1wtOn2
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
* MLT2HTML�̃��C�Z���X
*
*	��L���C�Z���X��Open Source Group Japan�ɂ����{���ƂȂ�܂��B
*	���p���͉��LURL�ł��B
*	http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
*
*	Copyright (c) 2012 _16in/��7N5y1wtOn2
*
*	�ȉ��ɒ�߂�����ɏ]���A
*	�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F�A�v�j
*	�̕������擾���邷�ׂĂ̐l�ɑ΂��A
*	�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ����܂��B
*	����ɂ́A�\�t�g�E�F�A�̕������g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A
*	�T�u���C�Z���X�A�����/�܂��͔̔����錠���A
*	����у\�t�g�E�F�A��񋟂��鑊��ɓ������Ƃ������錠�����������Ɋ܂܂�܂��B
*
*	��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕���
*	�܂��͏d�v�ȕ����ɋL�ڂ�����̂Ƃ��܂��B
*
*	�\�t�g�E�F�A�́u����̂܂܁v�ŁA
*	�����ł��邩�Öقł��邩���킸�A����̕ۏ؂��Ȃ��񋟂���܂��B
*	�����ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A
*	����ь�����N�Q�ɂ��Ă̕ۏ؂��܂݂܂����A
*	����Ɍ��肳�����̂ł͂���܂���B
*	��҂܂��͒��쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA
*	�\�t�g�E�F�A�ɋN���܂��͊֘A���A���邢�̓\�t�g�E�F�A�̎g�p
*	�܂��͂��̑��̈����ɂ���Đ������؂̐����A���Q�A
*	���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B
*
*----------------------------------------------------------------------*/
