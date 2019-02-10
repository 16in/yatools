/*----------------------------------------------------------------------
*
* AA�֘A�t�@�C���A�N�Z�T�̓������ʊ֐�
*
*	Copyright (c) 2012-2019 _16in/��7N5y1wtOn2
*
*----------------------------------------------------------------------*/
#include <aafile/AAFileAccessor.h>
#include "common.h"


namespace aafile {


/**** �t�@�C���^�C�v�`�F�b�N�p�}�N�� ****/
#define CHECK_TYPE( str, tag, len ) (i + len < length && _wcsnicmp( &str[ i ], tag, len ) == 0)


/**
* �t�@�C���^�C�v�擾
*	@param[in]	str					���ʗp������
*	@param[in]	length				������
*	@return		AAFileType			�t�@�C���^�C�v
*/
AAFileType CheckFileType( const wchar_t* str, uint_t length )
{
	AAFileType ret = AFT_TXT;

	for( uint_t i = 0; i < length; i++ )
	{
		if( CHECK_TYPE( str, L"[SPLIT]", 7 ) )
		{
			return AFT_MLT;
		}
		else if( CHECK_TYPE( str, L"[AA]", 4 ) )
		{
			return AFT_AST;
		}
		else if( CHECK_TYPE( str, L"[MojieName=",11 ) )
		{
			return AFT_AADATA;
		}
		else if( CHECK_TYPE( str, L"[ListName=",10 ) )
		{
			return AFT_AALIST;
		}
		else
		{// ���s�܂ł�����΂�
			while( i < length && str[ i ] != L'\n' )
			{
				if( i + 1 < length && ((str[ i ] == 0x01 && str[ i + 1 ] == 0x01)
					|| (str[ i ] == 0x02 && str[ i + 1 ] == 0x02)) )
				{
					return AFT_ASD;
				}
				i++;
			}
		}
	}

	return ret;
}

/**
* �n���ꂽ������̐擪�����s�R�[�h��
*	@param[in]	str					������
*	@param[in]	length				������
*	@return		uint_t				���s������("\r\n":2 "\r","\n":1 ����ȊO��0)
*/
uint_t CheckLineField( const wchar_t* str, uint_t length )
{
	if( length >= 2 && str[ 0 ] == L'\r' && str[ 1 ] == L'\n' )
		return 2;
	if( length >= 1 && (str[ 0 ] == L'\r' || str[ 0 ] == L'\n') )
		return 1;

	return 0;
}


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
