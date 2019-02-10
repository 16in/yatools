/*----------------------------------------------------------------------
*
* AA�֘A�t�@�C�����C�^����Q�Ƃ��郍�[�J���֐�
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include <aafile/AAFileAccessor.h>
#include "types.h"


namespace aafile {


/**
* �y�[�W���e�̂ݏ����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@return		uint_t				�����o����������
*/
static uint_t WritePageValue( AAFilePage* page, wchar_t* writeString, uint_t length );

/**
* TXT�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageTXT( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );

/**
* MLT�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageMLT( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );

/**
* AST�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageAST( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );

/**
* ASD�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageASD( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );

/**
* AAData�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageAADATA( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );

/**
* AAList�t�@�C���̏����o��
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageAALIST( AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );

/**
* AAData�t�@�C���̏����o��
*	@param[in]	split				����������
*	@param[in]	end					�I�[����������
*	@param[in]	page				�y�[�W�f�[�^
*	@param[out]	writeString			�����o����
*	@param[in]	length				�����o����̕�����
*	@param[in]	lastPage			�ŏI�y�[�W�Ȃ�^
*	@return		uint_t				�����o����������
*/
static uint_t WritePageAADataList( const wchar_t* split, const wchar_t* end, AAFilePage* page, wchar_t* writeString, uint_t length, bool lastPage );


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
