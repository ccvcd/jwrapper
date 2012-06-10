/**
 * Copyright 2012 Wangxr, vintage.wang@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

	/**
	 * ���û�������
	 */
	void setEnv(const char* name, const char* value);

	/**
	 * �ַ����滻
	 */
	void replaceAll(std::string& source, const char* oldString, const char* newString);
};

#endif // end of _JW_JWUTIL_H__
