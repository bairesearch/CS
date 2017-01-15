/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateConstFunctionArgumentCode.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h1a 14-November-2015
 *
 *******************************************************************************/


#include "CSgenerateConstFunctionArgumentCode.h"
//#include "CSdraw.h"
#include "SHAREDvars.h"


#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS


bool generateConstFunctionArguments(CSfileContainer* firstObjectInTopLevelBelowListContainer)
{
	bool result = true;
	
	if(!generateConstFunctionArgumentsRecurse(firstObjectInTopLevelBelowListContainer, firstObjectInTopLevelBelowListContainer))
	{
		result = false;
	}
	
	return result;	
}

bool generateConstFunctionArgumentsRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer)	
{
	bool result = true;
	
	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;
	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;
		
		if(currentFileObject->firstFileInBelowListContainer != NULL)
		{
			if(!(currentFileObject->printedCodeGeneric))
			{
				if(currentFileObject->printed)	//now redundant
				{//only create new code for printed boxes
					currentFileObject->headerFileTextOrig = getFileContents(currentFileObject->name);	//headerFileNameOrig
					currentFileObject->sourceFileTextOrig = getFileContents(currentFileObject->sourceFileNameOrig);
					currentFileObject->headerFileText = currentFileObject->headerFileTextOrig;	//initialise with same source code as original file
					currentFileObject->sourceFileText = currentFileObject->sourceFileTextOrig;	//initialise with same source code as original file

					#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
					//cout << "generateConstFunctionArgumentsRecurse{}: currentFileObject->sourceFileName = " << currentFileObject->sourceFileName << endl;
					//cout << "generateConstFunctionArgumentsRecurse{}: currentFileObject->headerFileName = " << currentFileObject->headerFileName << endl;
					#endif
					if(!generateConstFunctionArgumentsFile(currentFileObject))
					{
						result = false;
					}
					currentFileObject->printedCodeGeneric = true;
				}

				if(currentFileObject->firstFileInBelowListContainer != NULL)
				{
					generateConstFunctionArgumentsRecurse(currentFileObject->firstFileInBelowListContainer, firstObjectInTopLevelBelowListContainer);
				}
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}
	return result;
}
	
bool generateConstFunctionArgumentsFile(CSfile* currentFileObject)
{
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentsFile{}: currentFileObject->name = " << currentFileObject->name << endl;	
	#endif
	
	bool fileHasFunctions = false;
		
	CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
	while(currentFunctionObject->next != NULL)
	{
		generateConstFunctionArgumentsFunction(currentFunctionObject);

		//replace function source/header fullName with modified version (with Consts)
		string nameFullWithConsts = currentFunctionObject->nameFull;
		CSfunctionArgument* currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{
			if(currentFunctionArgumentInFunction->isConst)
			{
				string argumentWithConsts = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_CONST) + currentFunctionArgumentInFunction->argument;
				#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
				cout << "argument = " << currentFunctionArgumentInFunction->argument << endl;
				cout << "argumentWithConsts = " << argumentWithConsts << endl;
				#endif
				nameFullWithConsts = replaceAllOccurancesOfString(&(nameFullWithConsts), currentFunctionArgumentInFunction->argument, argumentWithConsts);
			}
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
		currentFileObject->sourceFileText = replaceAllOccurancesOfString(&(currentFileObject->sourceFileText), currentFunctionObject->nameFull, nameFullWithConsts);
		currentFileObject->headerFileText = replaceAllOccurancesOfString(&(currentFileObject->headerFileText), currentFunctionObject->nameFull, nameFullWithConsts);

		currentFunctionObject = currentFunctionObject->next;		
	}
	
	#ifndef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS_DISABLE_OUTPUT
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "currentFileObject->headerFileText = \n" << currentFileObject->headerFileText << endl;
	cout << "currentFileObject->sourceFileText = \n" << currentFileObject->sourceFileText << endl;	
	#endif
	writeStringToFile(currentFileObject->headerFileName, &(currentFileObject->headerFileText));
	writeStringToFile(currentFileObject->sourceFileName, &(currentFileObject->sourceFileText));
	#else
	cout << "currentFileObject->headerFileText = \n" << currentFileObject->headerFileText << endl;
	cout << "currentFileObject->sourceFileText = \n" << currentFileObject->sourceFileText << endl;	
	#endif
	
	return result;
}

//generateConstFunctionArgumentsFunction limitation: currently doesn't support embedded function reference arguments, eg; doThis(arg1, doThis2(arg2, arg3));
bool generateConstFunctionArgumentsFunction(CSfunction* currentFunctionObject)
{
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentsFunction{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;	
	#endif
			
	if(!(currentFunctionObject->functionArgumentConstsIdentified))
	{
		string functionNameFull = currentFunctionObject->nameFull;
		#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
		cout << "functionNameFull = " << functionNameFull << endl;
		#endif
		string functionNameFullUpdated = "";

		CSfunctionArgument* currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{	
			#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
			cout << "currentFunctionArgumentInFunction->argumentType/Name = " << currentFunctionArgumentInFunction->argumentType << " " << currentFunctionArgumentInFunction->argumentName << endl;
			#endif
			string functionDeclarationArgument = currentFunctionArgumentInFunction->argumentName;

			//step 2: check function references to see if function argument has been passed to a child function (and if so whether it has been modified by the child function)
			//condition 2: the same requirement must be met by all functions called by functionX with the argument varnameX
			CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceList;
			while(currentFunctionReference->next != NULL)
			{
				CSfile* fileObjectHoldingFunction = currentFunctionReference->functionReferenceTargetFileOwner;
				CSfunction* functionReferenceTarget = currentFunctionReference->functionReferenceTarget;
				if(functionReferenceTarget != NULL)
				{
					CSfunctionArgument* currentFunctionArgumentInFunctionReference = currentFunctionReference->firstFunctionArgumentInFunction;
					int functionReferenceArgumentIndex = 0;
					while(currentFunctionArgumentInFunctionReference->next != NULL)
					{	
						if((currentFunctionArgumentInFunctionReference->argument).find(functionDeclarationArgument) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							//now recurse and see if this function argument is considered const 
							generateConstFunctionArgumentsFunction(functionReferenceTarget);
							
							CSfunctionArgument* currentFunctionArgumentInFunctionReferenceTarget = functionReferenceTarget->firstFunctionArgumentInFunction;
							for(int i=0; i<functionReferenceArgumentIndex; i++)
							{
								currentFunctionArgumentInFunctionReferenceTarget = currentFunctionArgumentInFunctionReferenceTarget->next;
								i++;
							}
							if(currentFunctionArgumentInFunctionReferenceTarget->isConst)
							{
								//currentFunctionArgumentInFunctionReference->isConst = true;	//redundant
								currentFunctionArgumentInFunction->isConst = true;
							}
						}
						currentFunctionArgumentInFunctionReference = currentFunctionArgumentInFunctionReference->next;
						functionReferenceArgumentIndex++;
					}
				}
				currentFunctionReference = currentFunctionReference->next;
			}
			
			if(!(currentFunctionArgumentInFunction->isConst))
			{
				bool isConst = true;
				//step 1: check function text to see if it contains a modification of the function argument
				//condition 1: text after varnameX, and before ';' on same line must not contain an equals signs [meaning; the variable is not being set by anothing]
				int indexOfFunctionArgument = -1;
				while((indexOfFunctionArgument = (currentFunctionObject->functionText).find(currentFunctionArgumentInFunction->argumentName, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					//find next occurance of ';' on same line
					cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;
					int indexOfEndOfCommand = (currentFunctionObject->functionText).find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_END_OF_COMMAND, indexOfFunctionArgument);
					int indexOfEndOfNewline = (currentFunctionObject->functionText).find(STRING_NEW_LINE, indexOfFunctionArgument);
					if(indexOfEndOfCommand < indexOfEndOfNewline)
					{
						cout << "(indexOfEndOfCommand < indexOfEndOfNewline)" << endl;
						//now see if the function argument has been reassigned in this command
						int indexOfEndOfEqualsSet = (currentFunctionObject->functionText).find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_EQUALS_SET, indexOfFunctionArgument);
						int indexOfEndOfEqualsTest = (currentFunctionObject->functionText).find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_EQUALS_TEST, indexOfFunctionArgument);						
						if(indexOfEndOfEqualsSet < indexOfEndOfCommand)
						{
							if(indexOfEndOfEqualsSet != indexOfEndOfEqualsTest)
							{
								isConst = false;
								/*
								e.g.
								functionArgument = 5;
								*functionArgument = 5;
								functionArgument->parameter = 5;
								*/
							}
						}
					}
					else
					{
						//function reference argument detected; ignore (as this is dealt with in step 2)
					}

				}
				if(isConst)
				{
					currentFunctionArgumentInFunction->isConst = true;
				}
			}
			
			currentFunctionArgumentInFunction->constIdentified = true;
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
		
		currentFunctionObject->functionArgumentConstsIdentified = true;
	}
	return result;
}


#endif
