#ifndef _JW_JWUTIL_H__
#define _JW_JWUTIL_H__
#include <string>

namespace JWUtil
{
	/**
	 * ��ȡ����Ŀ¼
	 */
	std::string getWorkingDir();
	/**
	 * ��ȡ��ǰ�����ִ�г�������Ŀ¼
	 */
	std::string getCurrentExeFileDir();
	/**
	 * ��ȡ��ǰ�����ִ�г����ȫ·�������ļ���
	 */
	std::string getCurrentExeFilePath();
};

#endif // end of _JW_JWUTIL_H__
