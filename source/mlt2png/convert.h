/*--------------------------------------------------------------------------------------------
*
* MLT2PNG  �R���o�[�^�w�b�_
*
*---------------------------------------------------------------------------------------------*/
/*-- �X�V���� --------------------------------------------------------------------------------
*
*	2019/03/02
*		�R���o�[�^���� ver1.0
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
	* @brief �R���o�[�^
	* @details
	*	AA�֌W�t�@�C������摜�փR���o�[�g���܂��B
	*/
	class Converter
	{
	public:
		/**
		* @name �Z�b�g�A�b�v/�N�����i�b�v
		* @{
		*/
		/**
		* @brief �Z�b�g�A�b�v
		* @param[in] fontName				�t�H���g��		\n
		*									NULL�A�󕶎���̏ꍇ�͎g�p�\�ȃt�H���g���������܂��B
		* @param[in] fontSize				�t�H���g�T�C�Y	\n
		*									0�̏ꍇ��-16���w�肳��܂��B
		* @retval true						�Z�b�g�A�b�v�ɐ���
		* @retval false						�Z�b�g�A�b�v�Ɏ��s
		* @details
		*	�R���o�[�^���Z�b�g�A�b�v���܂��B
		*/
		static bool Setup( const char* fontName, int fontSize );

		/**
		* @brief �N�����i�b�v
		* @details
		*	�R���o�[�^���N���[���i�b�v���܂��B
		*/
		static void Cleanup( void );

		/**
		* @}
		*/
		/**
		* @name ���s
		* @{
		*/
		/**
		* @brief PNG�������[�`���̎��s
		* @param[in] inputFile				���̓t�@�C���p�X
		* @param[in] outputDirectory		�o�̓f�B���N�g��
		*/
		static void Run( const char* inputFile, const char* outputDirectory );

		/**
		* @brief PNG�������[�`���̎��s
		* @param[in] outputDirectory		�o�̓f�B���N�g��
		* @param[in]	srcString				���͓��e������
		* @param[in]	length					���͓��e�̕�����
		*/
		static void RunString( const char* outputDirectory, const char* srcString, unsigned long length );

		/**
		* @}
		*/

	private:
		// �S�y�[�W�����o��
		static void paintAllPage( aafile::AAFile* file, const char* outputDirectory );

		// �y�[�W��������
		static void paintPage( HDC hDC, aafile::AAFilePage* page, int width, int height, int lfCount );

		// �ŃT�C�Y���v�Z
		static void getPageSize( aafile::AAFilePage* page, int& width, int& height, int& lfCount );

		// �擪�����̕������ƕ����R�[�h���擾
		static int getCharWidth( const wchar_t* str, size_t length, unsigned long& step, wchar_t* ch = NULL );

		// �Q�ƕ����񂩃`�F�b�N���A�Q�ƕ�����Ȃ�ΎQ�Ƃ���������
		static bool CheckRefChar( const wchar_t* str, unsigned long length, wchar_t refChar[ 2 ], unsigned long& step, unsigned long& refCount );


	private:
		static HFONT		m_hFont;						//!< �t�H���g�n���h��
		static int32_t		m_CharacterWidth[ 0x10000 ];	//!< �������Ƃ̉���
		static int32_t		m_CharacterHeight;				//!< �����c��

	};
	


}



/*----------------------------------------------------------------------
*	MLT2PNG
*
*	Copyright (c) 2019 _16in/��7N5y1wtOn2
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
