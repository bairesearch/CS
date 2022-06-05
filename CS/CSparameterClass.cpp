 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSparameterClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#include "CSparameterClass.hpp"

CSparameter::CSparameter(void)
{
	name = "";
	type = "";
	isFunction = false;
	functionAccessSpecifierType = CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_UNKNOWN;
	//functionArgumentsParameterList = NULL;
}
CSparameter::~CSparameter(void)
{
}
