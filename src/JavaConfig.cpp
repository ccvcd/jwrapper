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
#include "JavaConfig.h"
#include "JWUtil.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/std/utility.hpp>

using namespace boost::property_tree;
using std::string;

#include <iostream>
using std::cout;
using std::endl;

JavaConfig::JavaConfig() : debug(false)
{
}

JavaConfig::~JavaConfig()
{
}

JavaConfig* JavaConfig::getInstance()
{
	static JavaConfig *singleton = new JavaConfig();
	return singleton;
}

bool JavaConfig::load()
{
	ptree pt;
	read_xml(getConfigFile().c_str(), pt);
	try {
		// Debug ?
		this->debug = pt.get<string>("java.debug") == "true";

		// Java Home
		const char* jh = getenv("JAVA_HOME");
		if(!jh) jh = "";
		this->javaHome = pt.get("java.javahome", jh);
		if(this->javaHome.empty()) {
			fprintf(stderr, "You must set JAVA_HOME environment variable or set it in config file.\n");
			return false;
		}
		JWUtil::setEnv("JAVA_HOME", this->javaHome.c_str());

		// JVM Type
		this->jvmType = pt.get<string>("java.jvmtype");

		// Main Class
		this->mainClass = pt.get<string>("java.mainclass");

		// Java System Property
		{
			BOOST_AUTO(child, pt.get_child("java.properties"));
			for(BOOST_AUTO(pos, child.begin());
			    pos != child.end();
			    ++pos) {
				std::string value = pos->second.data();
				if(this->expandMacro(value)) {
					this->propertyTable[pos->first.data()] = value;
				} else {
					return false;
				}
			}
		}

		// Class Path
		{
			BOOST_AUTO(child, pt.get_child("java.classpaths"));
			for(BOOST_AUTO(pos, child.begin());
			    pos != child.end();
			    ++pos) {
				std::string value = pos->second.data();
				if(this->expandMacro(value)) {
					this->classPathList.push_back(value);
				} else {
					return false;
				}
			}
		}

		// Java Options
		{
			BOOST_AUTO(child, pt.get_child("java.options"));
			for(BOOST_AUTO(pos, child.begin());
			    pos != child.end();
			    ++pos) {
				this->optionTable[pos->first.data()] = pos->second.data();
			}
		}

	} catch(const std::exception& e) {
		fprintf(stderr, "%s\n", e.what());
		return false;
	} catch(...) {
		fprintf(stderr, "Unexpected exception.\n");
		return false;
	}

	return true;
}

void JavaConfig::printAll()
{
	if(this->debug) {
		printf("javahome = [%s]\n", this->javaHome.c_str());
		printf("jvmtype = [%s]\n", this->jvmType.c_str());
		printf("mainclass = [%s]\n", this->mainClass.c_str());
		{
			printf("properties:\n");
			PropertyTable::iterator it = this->propertyTable.begin();
			for(; it != this->propertyTable.end(); it++) {
				printf("\t %s = [%s]\n", it->first.c_str(), it->second.c_str());
			}
		}
		{
			printf("classpaths:\n");
			for(size_t i = 0; i < this->classPathList.size(); i++) {
				printf("\t %2d = [%s]\n", i, this->classPathList[i].c_str());
			}
		}
		{
			printf("options:\n");
			OptionTable::iterator it = this->optionTable.begin();
			for(; it != this->optionTable.end(); it++) {
				printf("\t %s = [%s]\n", it->first.c_str(), it->second.c_str());
			}
		}
	}
}

std::string JavaConfig::getConfigFile()
{
	return JWUtil::getCurrentExeFilePath() + ".xml";
}

bool JavaConfig::expandMacro(std::string& value)
{
	bool result = true;
	for(std::size_t start = 0; result
	    && std::string::npos != (start = value.find("${", start));) {
		start += 2;
		std::size_t end = value.find("}", start);
		result = end != std::string::npos;
		if(result) {
			std::string var = value.substr(start, end - start);

			// ��ִ�г�������Ŀ¼
			if(var == "cpd") {
				value.replace(start - 2, end - start + 3, JWUtil::getCurrentExeFileDir());
			}
			// ��ǰ����Ŀ¼
			else if(var == "cwd") {
				value.replace(start - 2, end - start + 3, JWUtil::getWorkingDir());
			}
			// ��������
			else {
				const char* env = getenv(var.c_str());
				if(env != NULL) {
					value.replace(start - 2, end - start + 3, env);
				} else {
					fprintf(stderr, "The environment variable <%s> is not exist\n", var.c_str());
					result = false;
				}
			}
		}
	}

	return result;
}
