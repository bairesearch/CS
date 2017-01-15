/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSreferenceContainerClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h13a 11-December-2015
 *
 *******************************************************************************/

#include "CSreferenceContainerClass.h"




CSfunctionContainer::CSfunctionContainer(void)
{
	next = NULL;
	functionObject = NULL;
	fileObjectHoldingFunction = NULL;
}

CSfunctionContainer::~CSfunctionContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
	if(functionObject != NULL)
	{
		delete functionObject;
	}
}

CSfileContainer::CSfileContainer(void)
{
	next = NULL;
	fileObject = NULL;
	aboveLevelObject = NULL;
}

CSfileContainer::~CSfileContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
	if(fileObject != NULL)
	{
		delete fileObject;
	}
}





//CHECKTHIS;
int findEndPositionOfArgument(string* functionArgumentsRaw, int startPositionOfArgument, bool* lastArgument)
{
	//cout << "functionArgumentsRaw = " << *functionArgumentsRaw << endl;
	int endPositionOfArgument = -1;
	int bracketLevel = 1;
	int pos = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	char cPrevious = 'a';
	while(stillFindingEndPositionOfArgument)
	{
		char c = (*functionArgumentsRaw)[pos];
		if(bracketLevel == 1)
		{//only detect base level function reference commas (not commas for embedded function reference arguments)
			if((c == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_ARGUMENT_DELIMITER) || (c == CHAR_CLOSE_BRACKET))
			{
				endPositionOfArgument = pos;
				stillFindingEndPositionOfArgument = false;
				if(c == CHAR_CLOSE_BRACKET)
				{
					*lastArgument = true;
				}
			}
		}
		if(c == CLASS_TYPE_OPEN_TAG)
		{
			bracketLevel++;
		}
		else if(c == CLASS_TYPE_CLOSE_TAG)
		{
			if(cPrevious != string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OBJECT_REFERENCE_DELIMITER)[0])
			{
				bracketLevel--;
			}
		}	
		pos++;
		cPrevious = c;
	}
	return endPositionOfArgument;

}

/*
//limitation; only supports a single level of class type assignment (ie, multiple <> tags not supported in a given variable type, eg <><>)
int findEndPositionOfArgument(string* functionArgumentsRaw, int startPositionOfArgument)
{
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	//cout << "functionArgumentsRaw = " <<* functionArgumentsRaw << endl;
	//cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
	#endif

	int startPositionOfArgumentTemp = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	int endPositionOfArgument = -1;
	while(stillFindingEndPositionOfArgument)
	{
		endPositionOfArgument = functionArgumentsRaw->find(CHAR_COMMA, startPositionOfArgumentTemp);	//find next comma
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		//cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
		#endif

		if(endPositionOfArgument == CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			stillFindingEndPositionOfArgument = false;
		}
		else
		{
			int nextPositionOfClassTypeOpenTag = functionArgumentsRaw->find(CLASS_TYPE_OPEN_TAG, startPositionOfArgumentTemp);	//find next comma
			int nextPositionOfClassTypeCloseTag = functionArgumentsRaw->find(CLASS_TYPE_CLOSE_TAG, startPositionOfArgumentTemp);	//find next comma
			if(nextPositionOfClassTypeOpenTag == CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				stillFindingEndPositionOfArgument = false;
			}
			else
			{
				if(nextPositionOfClassTypeOpenTag < endPositionOfArgument)
				{
					bool foundCloseTag = false;

					if(nextPositionOfClassTypeCloseTag != CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{
						if(nextPositionOfClassTypeCloseTag < endPositionOfArgument)
						{
							foundCloseTag = true;
						}
					}
					if(!foundCloseTag)
					{
						startPositionOfArgumentTemp = endPositionOfArgument+1;
						#ifdef CS_DEBUG_HTML_DOCUMENTATION
						//cout << "startPositionOfArgumentTemp = " << startPositionOfArgumentTemp << endl;
						#endif
					}
					else
					{
						stillFindingEndPositionOfArgument = false;
					}
				}
				else
				{
					stillFindingEndPositionOfArgument = false;
				}
			}
		}
	}
	return endPositionOfArgument;
}
*/

//limitation; can't match argument types (only number of arguments)
bool findFunctionReferenceTarget(CSfunction* functionReference, CSfile* currentFileObject, CSfile** fileObjectHoldingFunction, CSfunction** functionReferenceTarget, bool countArguments)
{
	string name = functionReference->name;
	int numArguments = countArgumentList(functionReference->firstFunctionArgumentInFunction);
	
	bool foundPrintedReferenceWithName = false;
	
	CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
	while(currentFunctionObject->next != NULL)
	{
		bool conditions = false;
		if(currentFunctionObject->name == name)
		{
			if(countArguments)
			{
				if(countArgumentList(currentFunctionObject->firstFunctionArgumentInFunction) == numArguments)
				{
					conditions = true;
				} 
			}
			else
			{
				conditions = true;
			}
		}
		if(conditions)
		{
			*functionReferenceTarget = currentFunctionObject;
			foundPrintedReferenceWithName = true;
			*fileObjectHoldingFunction = currentFileObject;
		}
		currentFunctionObject = currentFunctionObject->next;
	}

	if(currentFileObject->firstFileInBelowListContainer != NULL)
	{
		if(findFunctionReferenceTargetRecurse(functionReference, currentFileObject->firstFileInBelowListContainer, fileObjectHoldingFunction, functionReferenceTarget, countArguments))
		{
			foundPrintedReferenceWithName = true;
		}
	}

	return foundPrintedReferenceWithName;
}

bool findFunctionReferenceTargetRecurse(CSfunction* functionReference, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfile** fileObjectHoldingFunction, CSfunction** functionReferenceTarget, bool countArguments)
{
	bool foundPrintedReferenceWithName = false;

	string name = functionReference->name;
	int numArguments = countArgumentList(functionReference->firstFunctionArgumentInFunction);
		
	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;
		
		CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
		while(currentFunctionObject->next != NULL)
		{
			bool conditions = false;
			if(currentFunctionObject->name == name)
			{
				if(countArguments)
				{
					if(countArgumentList(currentFunctionObject->firstFunctionArgumentInFunction) == numArguments)
					{
						conditions = true;
					} 
				}
				else
				{
					conditions = true;
				}
			}
			if(conditions)
			{
				*functionReferenceTarget = currentFunctionObject;
				foundPrintedReferenceWithName = true;
				*fileObjectHoldingFunction = currentFileObject;
			}
			currentFunctionObject = currentFunctionObject->next;
		}

		if(currentFileObject->firstFileInBelowListContainer != NULL)
		{
			if(findFunctionReferenceTargetRecurse(functionReference, currentFileObject->firstFileInBelowListContainer, fileObjectHoldingFunction, functionReferenceTarget, countArguments))
			{
				foundPrintedReferenceWithName = true;
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}

	return foundPrintedReferenceWithName;
}

int countArgumentList(CSfunctionArgument* firstFunctionArgumentInFunction)
{
	int count = 0;
	CSfunctionArgument* currentFunctionArgumentInFunction = firstFunctionArgumentInFunction;
	while(currentFunctionArgumentInFunction->next != NULL)
	{
		count++;
		currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
	}
	return count;
}


