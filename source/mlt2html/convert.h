/*--------------------------------------------------------------------------------------------
*
* MLT2HTML  �R���o�[�^�w�b�_
*
*---------------------------------------------------------------------------------------------*/
/*-- �X�V���� --------------------------------------------------------------------------------
*
*	2012/12/05
*		�R���o�[�^���� ver1.0
*
*---------------------------------------------------------------------------------------------*/
#pragma once


namespace convert
{


/**
* @brief html����template�̃Z�b�g�A�b�v
*	$(appdir)/template�ɍ݂�template�t�@�C�����Z�b�g�A�b�v���܂��B
*
*	@param[in]	appdir					�A�v���P�[�V�����݂̍�f�B���N�g��
*/
void SetupTemplate( const char* appdir );

/**
* @brief html����template�̃N�����i�b�v
*	�Z�b�g�A�b�v�ς݂�template���N�����i�b�v���܂��B
*/
void CleanupTemplate( void );

/**
* @brief html�������[�`���̎��s
*	�Z�b�g�A�b�v�ς݂�template�Ɠ��̓t�@�C������html�t�@�C���𐶐����܂��B
*
*	@param[in]	inputFile				���̓t�@�C���p�X
*	@param[in]	outputFile				�o�̓t�@�C���p�X
*/
void Run( const char* inputFile, const char* outputFile );

/**
* @brief html�������[�`���̎��s
*	�Z�b�g�A�b�v�ς݂�template�Ɠ��͕����񂩂�html�t�@�C���𐶐����܂��B
*
*	@param[in]	outputFile				�o�̓t�@�C���p�X
*	@param[in]	srcString				���͓��e������
*	@param[in]	length					���͓��e�̕�����
*/
void RunString( const char* outputFile, const char* srcString, unsigned long length );


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
