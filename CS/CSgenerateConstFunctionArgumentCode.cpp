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
 * Project Version: 3h2a 23-November-2015
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
			if(!(currentFunctionArgumentInFunction->isNotConst))
			{
				string argument = currentFunctionArgumentInFunction->argument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_ARGUMENT_DELIMITER;
				string argumentWithConsts = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST) + argument;
				#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
				cout << "argument = " << currentFunctionArgumentInFunction->argument << endl;
				cout << "argumentWithConsts = " << argumentWithConsts << endl;
				#endif
				nameFullWithConsts = replaceAllOccurancesOfString(&(nameFullWithConsts), argument, argumentWithConsts);
				
				argument = currentFunctionArgumentInFunction->argument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_CLOSE;
				argumentWithConsts = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST) + argument;
				#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
				cout << "argument = " << currentFunctionArgumentInFunction->argument << endl;
				cout << "argumentWithConsts = " << argumentWithConsts << endl;
				#endif
				nameFullWithConsts = replaceAllOccurancesOfString(&(nameFullWithConsts), argument, argumentWithConsts);
			}
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
		currentFileObject->sourceFileText = replaceAllOccurancesOfString(&(currentFileObject->sourceFileText), currentFunctionObject->nameFull, nameFullWithConsts);
		currentFileObject->headerFileText = replaceAllOccurancesOfString(&(currentFileObject->headerFileText), currentFunctionObject->nameFull, nameFullWithConsts);

		currentFunctionObject = currentFunctionObject->next;		
	}
	
	#ifndef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DISABLE_OUTPUT
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
//generateConstFunctionArgumentsFunction limitation: currently doesn't support comments; eg cout << "generateHTMLdocumentationMode = " << generateHTMLdocumentationMode << endl;
bool generateConstFunctionArgumentsFunction(CSfunction* currentFunctionObject)
{
	string* functionText = &(currentFunctionObject->functionText);
	
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentsFunction{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;	
	#endif
			
	if(!(currentFunctionObject->functionArgumentConstsIdentified))
	{
		currentFunctionObject->functionArgumentConstsIdentified = true;		//moved condition CS3h1d; ignore recursive function references

		string functionNameFull = currentFunctionObject->nameFull;
		#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
		cout << "functionNameFull = " << functionNameFull << endl;
		#endif
		//cout << "functionNameFull = " << functionNameFull << endl;
		string functionNameFullUpdated = "";

		CSfunctionArgument* currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{	
			string functionDeclarationArgument = currentFunctionArgumentInFunction->argumentName;

			if(generateConstFunctionArgumentFunction(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument, false))
			{
			
			}

			#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS
			
			int indexOfFunctionArgument = -1;
			while((indexOfFunctionArgument = functionText->find(functionDeclarationArgument, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				if(functionArgumentReferenceWholeWordCheck(functionText, functionDeclarationArgument, indexOfFunctionArgument))
				{		
					//find next occurance of ';' on same line
					//cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;
					int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfFunctionArgument);
					int indexOfEndOfNewline = functionText->find(STRING_NEW_LINE, indexOfFunctionArgument);
					int indexOfStartOfNewline = functionText->rfind(STRING_NEW_LINE, indexOfFunctionArgument);
					if(indexOfEndOfCommand < indexOfEndOfNewline)
					{
						int indexOfEndOfEqualsSetPrevious = CPP_STRING_FIND_RESULT_FAIL_VALUE;
						if((indexOfEndOfEqualsSetPrevious = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							if(indexOfEndOfEqualsSetPrevious > indexOfStartOfNewline)
							{
								string listCurrentPointerName = "";

								//added condition CS3h2a - detect list; eg "CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;"
								bool functionDeclarationArgumentIsListStartPointer = false;
								if(functionDeclarationArgument.substr(0, string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START).length()) == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START)
								{
									functionDeclarationArgumentIsListStartPointer = true;
									//cout << "functionDeclarationArgumentIsListStartPointer" << endl;
									listCurrentPointerName = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START) + functionDeclarationArgument.substr(string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START).length(), functionDeclarationArgument.length()-string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START).length());

								}

								//added condition CS3h2a - detect list; eg "CSfunctionArgument* currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;"
								bool functionDeclarationArgumentHasStartPointerBeingReferenced = false;
								int indexOfStartPointerBeingReferenced = CPP_STRING_FIND_RESULT_FAIL_VALUE;
								if((indexOfStartPointerBeingReferenced = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_START, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
								{
									if((indexOfStartPointerBeingReferenced > functionDeclarationArgument.length()) && (indexOfStartPointerBeingReferenced < indexOfEndOfCommand))
									{
										functionDeclarationArgumentHasStartPointerBeingReferenced = true;	
										//cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
										//cout << "functionDeclarationArgumentHasStartPointerBeingReferenced" << endl;
										listCurrentPointerName = CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_PREFIX_CURRENT;
									}
								}

								if(functionDeclarationArgumentIsListStartPointer || functionDeclarationArgumentHasStartPointerBeingReferenced)
								{
									int indexOfCurrentPointer = CPP_STRING_FIND_RESULT_FAIL_VALUE;
									if((indexOfCurrentPointer = functionText->find(listCurrentPointerName, indexOfStartOfNewline)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
									{
										if(!functionDeclarationArgumentIsListStartPointer)
										{//ie functionDeclarationArgumentHasStartPointerBeingReferenced
											listCurrentPointerName = extractFullVariableName(functionText, indexOfCurrentPointer);
										}

										if((indexOfCurrentPointer < indexOfEndOfEqualsSetPrevious))
										{
											//cout << "listCurrentPointerName = " << listCurrentPointerName << endl;
											
											//eg "CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;"
											#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
											cout << "generateConstFunctionArgumentsFunction{}: found list current pointer initialisation" << endl;
											#endif
											if(generateConstFunctionArgumentFunction(currentFunctionObject, currentFunctionArgumentInFunction, listCurrentPointerName, true))
											{

											}
										}
									}
								}

								//trace for loop vector list iterators  (and secondary assignments)
								int indexOfIteratorType = CPP_STRING_FIND_RESULT_FAIL_VALUE;
								if((indexOfIteratorType = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
								{
									if(indexOfIteratorType > indexOfStartOfNewline)
									{
										//cout << "(indexOfIteratorType > indexOfStartOfNewline)" << endl;
										
										//added condition CS3h2a - detect list; eg "for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); ..."
											//can't handle cases where iterator is defined on separate line to for loop (or where multiple iterators are used to refer to the same list), eg; NLCtranslatorCodeBlocksLogicalConditions.cpp: generateCodeBlocksFromMathText: map<int, vector<GIAentityNode*>*>::iterator sentenceIter = sentenceIterFirstInFullSentence;
											//can't handle cases where vector/map functions are executed to modify the vector/map (or where multiple iterators are used to refer to the same list), eg; GIAtranslatorOperations.cpp: eraseSubstanceFromSubstanceList: entityNodesActiveListSubstances->erase(substanceEntityNodesListIteratorFound);
										bool functionDeclarationArgumentHasBeginPointerBeingReferenced = false;
										int indexOfBeginPointerBeingReferenced = CPP_STRING_FIND_RESULT_FAIL_VALUE;
										if((indexOfBeginPointerBeingReferenced = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_START, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
										{
											if((indexOfBeginPointerBeingReferenced > functionDeclarationArgument.length()) && (indexOfBeginPointerBeingReferenced < indexOfEndOfCommand))
											{
												functionDeclarationArgumentHasBeginPointerBeingReferenced = true;	
												//cout << "functionDeclarationArgumentHasBeginPointerBeingReferenced" << endl;
											}
										}

										//added condition CS3h2a - detect list; eg "unordered_map<string, GIAentityNode*>::iterator conceptEntityNodesListIterator = entityNodesActiveListConcepts->find(*entityName); ..."
										bool functionDeclarationArgumentHasFindPointerBeingReferenced = false;
										int indexOfFindPointerBeingReferenced = CPP_STRING_FIND_RESULT_FAIL_VALUE;
										if((indexOfFindPointerBeingReferenced = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_FIND, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
										{
											if((indexOfFindPointerBeingReferenced > functionDeclarationArgument.length()) && (indexOfFindPointerBeingReferenced < indexOfEndOfCommand))
											{
												functionDeclarationArgumentHasFindPointerBeingReferenced = true;	
												//cout << "functionDeclarationArgumentHasFindPointerBeingReferenced" << endl;
											}
										}
										
										if(indexOfIteratorType < indexOfEndOfEqualsSetPrevious)
										{
											//cout << "(indexOfIteratorType < indexOfEndOfEqualsSetPrevious)" << endl;
											#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
											cout << "generateConstFunctionArgumentsFunction{}: found for loop vector/map iterator" << endl;
											#endif
														
											string iteratorName = extractFullVariableName(functionText, indexOfIteratorType+string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR).length());
											if(generateConstFunctionArgumentFunction(currentFunctionObject, currentFunctionArgumentInFunction, iteratorName, false))
											{

											}
											
											//trace secondary assignments of iterator (FUTURE CHECKTHIS: NB really need to trace secondary/tertiary/etc assignments of all variables including all lists; not just vector/map lists):
											//e.g. GIAentityNode* currentQueryEntityNode = connectionIterQuery->second;
											int indexOfEqualsSetFollowingLine = CPP_STRING_FIND_RESULT_FAIL_VALUE;
											if((indexOfEqualsSetFollowingLine = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfEndOfNewline)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
											{
												//cout << "(indexOfEqualsSetFollowingLine > indexOfEndOfNewline)" << endl;

												int indexOfIteratorFollowingLine = CPP_STRING_FIND_RESULT_FAIL_VALUE;
												if((indexOfIteratorFollowingLine = functionText->find(iteratorName, indexOfEndOfNewline)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
												{
													//cout << "((indexOfIteratorFollowingLine = functionText->find(iteratorName, indexOfEndOfNewline)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)" << endl;

													string secondaryAssignmentOfIterator = extractFullVariableNameReverse(functionText, indexOfEqualsSetFollowingLine-1);
													//cout << "secondaryAssignmentOfIterator = " << secondaryAssignmentOfIterator << endl;
													string followingLineHypothetical[4];
													followingLineHypothetical[0] = secondaryAssignmentOfIterator + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CLOSE_PARAMETER_SPACE + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//currentQueryEntityNode = (*connectionIterQuery); 
													followingLineHypothetical[1] = secondaryAssignmentOfIterator + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//currentQueryEntityNode = *connectionIterQuery; 
													followingLineHypothetical[2] = secondaryAssignmentOfIterator + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_MAP_ITERATOR_FIRST + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//currentQueryEntityNode = connectionIterQuery->first;
													followingLineHypothetical[3] = secondaryAssignmentOfIterator + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_MAP_ITERATOR_SECOND + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//currentQueryEntityNode = connectionIterQuery->second;
													bool foundSecondaryIteratorAssignmentExact = false;
													int indexOfStartOfFollowingLine = indexOfEqualsSetFollowingLine - secondaryAssignmentOfIterator.length();
													for(int i=0; i<3; i++)
													{
														int indexOfFollowingLineHypotheticalMatch = functionText->find(followingLineHypothetical[i], indexOfStartOfFollowingLine);
														if(indexOfFollowingLineHypotheticalMatch != CPP_STRING_FIND_RESULT_FAIL_VALUE)
														{
															foundSecondaryIteratorAssignmentExact = true;
															//cout << "followingLineHypothetical[i] = " << followingLineHypothetical[i] << endl;
															//exit(0);
														}
													}
													
													if(foundSecondaryIteratorAssignmentExact)
													{
														#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
														cout << "found iterator secondary assignment" << endl;
														#endif
														//cout << "foundSecondaryIteratorAssignmentExact = " << foundSecondaryIteratorAssignmentExact << endl;
														//cout << "secondaryAssignmentOfIterator = " << secondaryAssignmentOfIterator << endl;
														if(generateConstFunctionArgumentFunction(currentFunctionObject, currentFunctionArgumentInFunction, iteratorName, false))
														{

														}
													}
												}
											}
										}
									}
								}				
							}
						}	
					}
				}
			}


			#endif
			
			currentFunctionArgumentInFunction->constIdentified = true;
			
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
	}
	
	/*
	if(currentFunctionObject->name == "addToHTMLdocumentationIndentedList")
	{
		exit(0);
	}	
	*/					
	return result;
}

bool generateConstFunctionArgumentFunction(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument, bool ignoreListIterationNextAssignments)
{
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentFunction{}:" << endl;
	cout << "functionDeclarationArgument = " << functionDeclarationArgument << endl;
	cout << "currentFunctionArgumentInFunction->argumentType/Name = " << currentFunctionArgumentInFunction->argumentType << " " << currentFunctionArgumentInFunction->argumentName << endl;
	#endif
	//cout << "\t currentFunctionArgumentInFunction->argumentName = " << currentFunctionArgumentInFunction->argumentName << endl;

	//step 2: check function references to see if function argument has been passed to a child function (and if so whether it has been modified by the child function)
	//condition 2: the same requirement must be met by all functions called by functionX with the argument varnameX
	CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceListRepeats;
	int currentFunctionReferenceIndex = 0;
	while(currentFunctionReference->next != NULL)
	{
		//cout << "\t\t currentFunctionReference->name = " << currentFunctionReference->name << endl;
		//cout << "\t\t currentFunctionReferenceIndex = " << currentFunctionReferenceIndex << endl;

		CSfile* fileObjectHoldingFunction = currentFunctionReference->functionReferenceTargetFileOwner;
		CSfunction* functionReferenceTarget = currentFunctionReference->functionReferenceTarget;
		if(functionReferenceTarget != NULL)
		{
			//cout << "\t\t\t functionReferenceTarget->name = " << functionReferenceTarget->name << endl;

			CSfunctionArgument* currentFunctionArgumentInFunctionReference = currentFunctionReference->firstFunctionArgumentInFunction;

			//debug only;
			int functionReferenceArgumentCount = 0;
			while(currentFunctionArgumentInFunctionReference->next != NULL)
			{
				functionReferenceArgumentCount++;
				currentFunctionArgumentInFunctionReference = currentFunctionArgumentInFunctionReference->next;
			}

			currentFunctionArgumentInFunctionReference = currentFunctionReference->firstFunctionArgumentInFunction;
			int functionReferenceArgumentIndex = 0;
			while(currentFunctionArgumentInFunctionReference->next != NULL)
			{	
				//cout << "\t\t\t\t currentFunctionArgumentInFunctionReference->argument = " << currentFunctionArgumentInFunctionReference->argument << endl;

				if((currentFunctionArgumentInFunctionReference->argument).find(functionDeclarationArgument) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					//cout << "\t\t\t\t currentFunctionArgumentInFunctionReference->argument = " << currentFunctionArgumentInFunctionReference->argument << endl;
					//cout << "\t\t\t\t\t match found" << endl;

					//now recurse and see if this function argument is considered const 
					generateConstFunctionArgumentsFunction(functionReferenceTarget);

					CSfunctionArgument* currentFunctionArgumentInFunctionReferenceTarget = functionReferenceTarget->firstFunctionArgumentInFunction;

					//debug only;
					int i = 0;
					while(currentFunctionArgumentInFunctionReferenceTarget->next != NULL)
					{
						currentFunctionArgumentInFunctionReferenceTarget = currentFunctionArgumentInFunctionReferenceTarget->next;
						i++;
					}
					if(i != functionReferenceArgumentCount)
					{
						cout << "generateConstFunctionArgumentsFunction{} error: (i != functionReferenceArgumentCount)" << endl;
						cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;	//CHECKTHIS
						cout << "functionReferenceTarget->name = " << functionReferenceTarget->name << endl;
						cout << "functionReferenceArgumentCount = " << functionReferenceArgumentCount << endl;
						cout << "i = " << i << endl;
						exit(0);
					}

					currentFunctionArgumentInFunctionReferenceTarget = functionReferenceTarget->firstFunctionArgumentInFunction;
					for(int i=0; i<functionReferenceArgumentIndex; i++)
					{
						currentFunctionArgumentInFunctionReferenceTarget = currentFunctionArgumentInFunctionReferenceTarget->next;
					}
					//cout << "\t\t\t\t\t functionReferenceArgumentIndex = " << functionReferenceArgumentIndex << endl;

					if(currentFunctionArgumentInFunctionReferenceTarget->isNotConst)
					{
						currentFunctionArgumentInFunction->isNotConst = true;
					}

					/*
					if(currentFunctionObject->name == "configureFileOrFunctionObjectConnection" && functionReferenceTarget->name == "writeSVGconnector" && currentFunctionArgumentInFunctionReferenceTarget->argumentName == "currentTag")
					{
						cout << "at" << endl;
						exit(0);
					}
					*/

					//cout << "\t\t\t\t\t fin" << endl;
				}
				currentFunctionArgumentInFunctionReference = currentFunctionArgumentInFunctionReference->next;
				functionReferenceArgumentIndex++;
			}
		}
		currentFunctionReferenceIndex++;
		currentFunctionReference = currentFunctionReference->next;
	}

	if(currentFunctionArgumentInFunction->isNotConst)
	{

	}
	else
	{
		if(checkIfVariableIsBeingModifiedInFunction(&(currentFunctionObject->functionText), functionDeclarationArgument, ignoreListIterationNextAssignments))
		{	
			currentFunctionArgumentInFunction->isNotConst = true;
		}
	}
}
			
			
bool checkIfVariableIsBeingModifiedInFunction(string* functionText, string functionDeclarationArgument, bool ignoreListIterationNextAssignments)
{ 
	bool isNotConst = false;
	//step 1: check function text to see if it contains a modification of the function argument
	//condition 1: text after varnameX, and before ';' on same line must not contain an equals signs [meaning; the variable is not being set by anothing]
	int indexOfFunctionArgument = -1;
	while((indexOfFunctionArgument = functionText->find(functionDeclarationArgument, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		if(functionArgumentReferenceWholeWordCheck(functionText, functionDeclarationArgument, indexOfFunctionArgument))
		{		
			//find next occurance of ';' on same line
			//cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;
			int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfFunctionArgument);
			int indexOfEndOfNewline = functionText->find(STRING_NEW_LINE, indexOfFunctionArgument);
			int indexOfStartOfNewline = functionText->rfind(STRING_NEW_LINE, indexOfFunctionArgument);
			if(indexOfEndOfCommand < indexOfEndOfNewline)
			{
				//now see if the function argument has been reassigned in this command
				int indexOfEndOfEqualsSet = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument);
				/*
				int indexOfEndOfEqualsTest = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_TEST, indexOfFunctionArgument);
				int indexOfEndOfNotEqualsTest = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_NOT_EQUALS_TEST, indexOfFunctionArgument);					
				*/
				if((indexOfEndOfEqualsSet < indexOfEndOfCommand) && (indexOfEndOfEqualsSet < indexOfEndOfNewline))
				{
					/*
					if((indexOfEndOfEqualsSet != indexOfEndOfEqualsTest) && (indexOfEndOfEqualsSet != indexOfEndOfNotEqualsTest))	//not required for multiline logical conditions because checking for (indexOfEndOfEqualsSet < indexOfEndOfCommand), e.g. if(chicken == 5). Only required for single line logical conditions, eg. if(chicken == 5) bat = 2;
					{
					*/
						//added condition CS3h1d - ensure not a cout start, e.g. 'cout << "indexOfFunctionArgument' in; 'cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;'
						if((indexOfFunctionArgument < string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START).length()) || (functionText->substr(indexOfFunctionArgument-string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START).length(), string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START).length()) != CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START))
						{
							bool passMiscellaneousConditions = true;
							if(ignoreListIterationNextAssignments)
							{
								int indexOfNextFunctionArgument = -1;
								int indexOfNextFunctionArgumentNextPointer = -1;
								if((indexOfNextFunctionArgument = functionText->find(functionDeclarationArgument, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
								{
									if((indexOfNextFunctionArgumentNextPointer = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_LIST_VARIABLE_POINTER_NEXT, indexOfNextFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
									{
										if((indexOfNextFunctionArgumentNextPointer > indexOfNextFunctionArgument) && (indexOfNextFunctionArgumentNextPointer < indexOfEndOfCommand))
										{
											//e.g. "currentFileObjectContainer = currentFileObjectContainer->next;"
											passMiscellaneousConditions = false;
											
											string currentLine = functionText->substr(indexOfStartOfNewline, indexOfEndOfNewline-indexOfStartOfNewline);	
											//cout << "!passMiscellaneousConditions, currentLine = " << currentLine << endl;
										}
									}
								}
							}
							if(passMiscellaneousConditions)
							{
								isNotConst = true;
							}
							
							/*
							e.g.
							functionArgument = 5;
							*functionArgument = 5;
							functionArgument->parameter = 5;
							*/
						}
					/*
					}
					*/
				}
			}
			else
			{
				//function reference argument detected; ignore (as this is dealt with in step 2)
			}
		}
	}
	return isNotConst;
}

bool functionArgumentReferenceWholeWordCheck(string* functionText, string functionDeclarationArgument, int indexOfFunctionArgument)
{
	bool functionArgumentReferenceWholeWord = false;
	//added condition CS3h1c - ensure previous character is not a letter (this ensures that ABCargumentName is not found when searching for argumentName)
	if((indexOfFunctionArgument == 0) || !charInCharArray((*functionText)[indexOfFunctionArgument-1], functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
	{
		//added condition CS3h1c - ensure next character is not a letter (this ensures that argumentNameABC is not found when searching for argumentName)
		if((functionText->length() == indexOfFunctionArgument) || !charInCharArray((*functionText)[indexOfFunctionArgument+functionDeclarationArgument.length()], functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
		{
			functionArgumentReferenceWholeWord = true;	
		}
	}
	return functionArgumentReferenceWholeWord;
}


string extractFullVariableName(string* functionText, int indexOfStartOfVariableName)
{
	string fullVariableName = "";
	int i = indexOfStartOfVariableName;
	bool stillFindingVariableNameCharacters = true;
	while(stillFindingVariableNameCharacters)
	{
		bool foundValidChar = false;
		for(int j=0; j<CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES; j++)
		{
			if((*functionText)[i] == functionOrVariableNameCharacters[j])
			{
				foundValidChar = true;
			}
		}
		if(foundValidChar)
		{
			fullVariableName = fullVariableName + (*functionText)[i];
		}
		else
		{
			stillFindingVariableNameCharacters = false;
		}
		if(i >= functionText->length())
		{
			stillFindingVariableNameCharacters = false;
		}
		i++;
	}
	return fullVariableName;
}

string extractFullVariableNameReverse(string* functionText, int indexOfEndOfVariableName)
{
	string fullVariableName = "";
	int i = indexOfEndOfVariableName;
	bool stillFindingVariableNameCharacters = true;
	while(stillFindingVariableNameCharacters)
	{
		bool foundValidChar = false;
		for(int j=0; j<CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES; j++)
		{
			if((*functionText)[i] == functionOrVariableNameCharacters[j])
			{
				foundValidChar = true;
			}
		}
		if(foundValidChar)
		{
			string blank = "";
			fullVariableName = blank + (*functionText)[i] + fullVariableName;
		}
		else
		{
			stillFindingVariableNameCharacters = false;
		}
		if(i == 0)
		{
			stillFindingVariableNameCharacters = false;
		}
		i--;
	}
	return fullVariableName;
}

				
#endif
