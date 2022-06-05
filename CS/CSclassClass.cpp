 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSclassClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#include "CSclassClass.hpp"


CSclass::CSclass(void)
{
	name = "";
	//classList = null
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS_FUNCTIONS_IGNORE_PRIMARY_FILE_CLASS
	primaryFileClass = false;
	#endif
}
CSclass::~CSclass(void)
{
}

#ifdef CS_SUPPORT_GENERATED_CPP_CODE
bool CSclassClass::getClassNameBase(const string className, string* classNameBase)
{
	bool result = false;
	*classNameBase = "";
	
	int classNameLength = className.length();
	if(classNameLength > CS_CLASS_NAME_APPEND.length())
	{
		string classNameAppendDetected = className.substr(classNameLength-CS_CLASS_NAME_APPEND.length(),CS_CLASS_NAME_APPEND.length());
		if(classNameAppendDetected == CS_CLASS_NAME_APPEND)
		{
			//class name ends with "Class"

			*classNameBase = className.substr(0, classNameLength-CS_CLASS_NAME_APPEND.length());
			result = true;
		}		
	}
	
	return result;				
}
#endif	
