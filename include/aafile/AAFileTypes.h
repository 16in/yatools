/*----------------------------------------------------------------------
*
* AA�֘A�t�@�C���A�N�Z�X�p��`��
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#pragma once

#include <stdint.h>


namespace aafile {

/** �y�[�W���^ */
typedef uint_t AAPageCount;

/** �����񒷌^ */
typedef uint_t StringLength;


/**
* @brief �t�@�C���`���񋓑�
*/
enum AAFileType
{
	AFT_TXT		= 0x00,			//!< �����ȃe�L�X�g�t�@�C��(.txt)
	AFT_MLT,					//!< MLT�t�@�C��(.mlt)
	AFT_AST,					//!< AST�t�@�C��(.ast)
	AFT_ASD,					//!< ASD�t�@�C��(.asd)
	AFT_AADATA,					//!< AAData�t�@�C��(.txt)
	AFT_AALIST,					//!< AAList�t�@�C��(.txt)

	AAFILETYPE_MAX
};

/**
* @brief �y�[�W�f�[�^�\����
*/
struct AAFilePage
{
	const wchar_t*		name;			//!< �y�[�W��
	const wchar_t*		value;			//!< �y�[�W���e
	StringLength		nameLength;		//!< �y�[�W��������
	StringLength		valueLength;	//!< �y�[�W���e������
};

/**
* @brief �t�@�C���S�̂̍\����
*/
struct AAFile
{
	AAFileType			type;			//!< �t�@�C���^�C�v
	AAFilePage*			pageList;		//!< �y�[�W�f�[�^�̈ꗗ
	AAPageCount			pageCount;		//!< �y�[�W��

	const wchar_t*		fileData;		//!< �t�@�C���ǂݍ��݂��琶�������ꍇ�̃t�@�C�����e
										///  �����񂩂琶�������ꍇ��NULL
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
