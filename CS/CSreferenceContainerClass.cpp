 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSreferenceContainerClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#include "CSreferenceContainerClass.hpp"




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
int CSreferenceContainerClassClass::findEndPositionOfArgument(const string* functionArgumentsRaw, const int startPositionOfArgument, bool* lastArgument)
{
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
bool CSreferenceContainerClassClass::findFunctionReferenceTarget(const CSfunction* functionReference, constEffective CSfile* currentFileObject, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** functionReferenceTarget, const bool countArguments)
{
	string name = functionReference->name;
	int numArguments = countArgumentList(functionReference->firstFunctionArgumentInFunction);
	//cout << "findFunctionReferenceTarget{}:" << endl;
	//cout << "name = " << name << endl;
	//cout << "numArguments = " << numArguments << endl;

	bool foundPrintedReferenceWithName = false;

	constEffective CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
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
		#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
		findFunctionReferenceTargetRecurseReset(currentFileObject->firstFileInBelowListContainer);
		#endif
	}

	//cout << "end findFunctionReferenceTarget{}:" << endl;

	return foundPrintedReferenceWithName;
}

bool CSreferenceContainerClassClass::findFunctionReferenceTargetRecurse(const CSfunction* functionReference, constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** functionReferenceTarget, const bool countArguments)
{
	bool foundPrintedReferenceWithName = false;

	string name = functionReference->name;
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE
	bool detectClass = false;
	string className = functionReference->className;
	if(className != "")
	{
		detectClass = true; 
	}
	#endif
	int numArguments = countArgumentList(functionReference->firstFunctionArgumentInFunction);

	constEffective CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		constEffective CSfile* currentFileObject = currentFileObjectContainer->fileObject;
		
		#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
		if(!(currentFileObject->functionReferenceTargetSearched))
		{
			currentFileObject->functionReferenceTargetSearched = true;
		#endif
			constEffective CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
			while(currentFunctionObject->next != NULL)
			{
				bool conditions = false;
				if(currentFunctionObject->name == name)
				{
					conditions = true;
					#ifdef CS_SUPPORT_GENERATED_CPP_CODE
					if(detectClass)
					{
						conditions = false;
						if(currentFunctionObject->className == className)
						{
							conditions = true;
						}
					}
					#endif
					if(countArguments)
					{
						conditions = false;
						if(countArgumentList(currentFunctionObject->firstFunctionArgumentInFunction) == numArguments)
						{
							conditions = true;
						}
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

		#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
		}
		#endif
		
		currentFileObjectContainer = currentFileObjectContainer->next;
	}

	return foundPrintedReferenceWithName;
}
#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
void CSreferenceContainerClassClass::findFunctionReferenceTargetRecurseReset(constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer)
{
	constEffective CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;
	while(currentFileObjectContainer->next != NULL)
	{
		constEffective CSfile* currentFileObject = currentFileObjectContainer->fileObject;

		if(currentFileObject->functionReferenceTargetSearched)
		{
			currentFileObject->functionReferenceTargetSearched = false;
			
			if(currentFileObject->firstFileInBelowListContainer != NULL)
			{
				findFunctionReferenceTargetRecurseReset(currentFileObject->firstFileInBelowListContainer);
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}
}
#endif



int CSreferenceContainerClassClass::countArgumentList(const CSfunctionArgument* firstFunctionArgumentInFunction)
{
	int count = 0;
	const CSfunctionArgument* currentFunctionArgumentInFunction = firstFunctionArgumentInFunction;
	while(currentFunctionArgumentInFunction->next != NULL)
	{
		count++;
		currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
	}
	return count;
}


