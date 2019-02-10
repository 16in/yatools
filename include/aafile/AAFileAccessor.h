/*----------------------------------------------------------------------
*
* AA�֘A�t�@�C���A�N�Z�T
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include "aatype.h"
#include "AAFileTypes.h"


namespace aafile {


/**
* AA�t�@�C�����[�_
*/
class AAFileReader
{
public:
	/**
	* AAFile���t�@�C�����琶������
	*	(ASD�̏ꍇ�͉��s�R�[�h��\r\n�ɒu�������܂�)
	*	@param[in]	filePath			�t�@�C���p�X
	*	@return		AAFile*				�ǂݍ��܂ꂽ�t�@�C���f�[�^
	*									�ǂݍ��݂Ɏ��s�����ꍇ��NULL
	*/
	static AAFile* CreateAAFileFromFile( const wchar_t* filePath );

	/**
	* AAFile����e�����񂩂琶������
	*	(ASD�̏ꍇ�͉��s�R�[�h��\r\n�ɒu�������܂�)
	*	@param[in]	str					���e������
	*	@param[in]	length				������
	*	@return		AAFile*				�ǂݍ��܂ꂽ�t�@�C���f�[�^
	*									�ǂݍ��݂Ɏ��s�����ꍇ��NULL
	*/
	static AAFile* CreateAAFile( wchar_t* str, uint_t length );

	/**
	* �������ꂽAAFile��p������
	*	@param[in]	pAaFile				�p������AAFile�̃C���X�^���X
	*/
	static void ReleaseAAFile( AAFile* pAaFile );
};


/**
* AA�t�@�C�����C�^
*/
class AAFileWriter
{
public:
	/**
	* @biref �w��̃t�@�C���Ƀt�@�C���S�̂̓��e�������o��
	*	AAFile�̏��ɏ]���āA�f�[�^���t�@�C���ɏ����o���܂��B
	*
	*	@param[in]	pAaFile				�����o������Z�߂�AAFile
	*	@param[in]	filePath			�����o���t�@�C���ւ̃p�X
	*	@param[in]	utf16				UTF16-LE�Ƃ��ď����o���Ȃ�^�A����ȊO�͋U
	*	@return		uint_t				���ۂɏ����o����������
	*/
	static uint_t WriteAAFileToFile( const AAFile* pAaFile, const wchar_t* filePath, bool utf16 = false );

	/**
	* @biref ��������Ƀt�@�C���S�̂̓��e�������o��
	*	AAFile�̏��ɏ]���āA�f�[�^����������ɏ����o���܂��B
	*	str��NULL�̏ꍇ�A�i�[�����͍s�킸�A�K�v�ȗv�f�������Ԃ��܂��B
	*
	*	@param[in]	pAaFile				�����o������Z�߂�AAFile
	*	@param[out]	str					������̊i�[��
	*	@param[in]	length				str�̗e��(wchar_t�P�ʂ̗v�f��)
	*	@return		uint_t				���ۂɏ����o����������
	*/
	static uint_t WriteAAFile( const AAFile* pAaFile, wchar_t* str, uint_t length );

	/**
	* @brief ���s�R�[�h���w��
	*	AAFileWriter�ɉ��s�R�[�h��ݒ肵�܂��B
	*
	*	@param[in]	lfString			���s�R�[�h(L'\0'�I�[�ł���K�v������܂�)
	*/
	static void SetLineField( const wchar_t* lfString );

	/**
	* @brief ���s�R�[�h���擾
	*	AAFileWriter�ɐݒ肳��Ă�����s�R�[�h���擾���܂��B
	*
	*	@return		const wchar_t*		���s�R�[�h
	*/
	static const wchar_t* GetLineField( void );
};



}


/*----------------------------------------------------------------------
* License
*
*	AAFileAccessor
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
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
* ���C�Z���X
*
*	��L���C�Z���X��Open Source Group Japan�ɂ����{���ƂȂ�܂��B
*	���p���͉��LURL�ł��B
*	http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
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
