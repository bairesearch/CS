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
 * Project Version: 3h8a 08-December-2015
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
				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_PRECODED_CONST_ARGUMENTS
				if(currentFunctionArgumentInFunction->argument.find("const ") == CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{//added CS3h7a - ignore precoded const function arguments
				#endif
	
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

					//add const to function argument secondary assignements - CS3h5a
					for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunction->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunction->argumentNameAliasList.end(); argumentNameAliasListIter++)
					{
						//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
						string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
						int posOfFunctionText = currentFileObject->sourceFileText.find(currentFunctionObject->functionTextRaw);
						int functionTextOrigLength = currentFunctionObject->functionTextRaw.length();
						if(posOfFunctionText != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							bool foundAtLeastOneInstance = false;
							currentFunctionObject->functionTextRaw = replaceAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction(&(currentFunctionObject->functionTextRaw), functionArgumentSecondaryAssignmentName, &foundAtLeastOneInstance);
							if(foundAtLeastOneInstance)
							{
								//cout << "foundAtLeastOneInstance" << endl;
								currentFileObject->sourceFileText = currentFileObject->sourceFileText.substr(0, posOfFunctionText) + currentFunctionObject->functionTextRaw + currentFileObject->sourceFileText.substr((posOfFunctionText+functionTextOrigLength), currentFileObject->sourceFileText.length()-(posOfFunctionText+functionTextOrigLength));
							}
							/*
							else
							{
								cout << "generateConstFunctionArgumentsFile{} error: !foundAtLeastOneInstance of " << functionArgumentSecondaryAssignmentName << " in currentFunctionObject->functionTextRaw" << endl;
								cout << "currentFileObject->name = " << currentFileObject->name << endl;
								cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
								cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
								exit(0);						
							}
							*/
						}
						else
						{
							cout << "generateConstFunctionArgumentsFile{} error: currentFunctionObject->functionTextRaw not found in currentFileObject->sourceFileText" << endl;
							cout << "currentFunctionObject->functionTextRaw = " <<  currentFunctionObject->functionTextRaw << endl;
							exit(0); 
						}
					}
				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_PRECODED_CONST_ARGUMENTS
				}
				#endif
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

string replaceAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction(string* functionTextOrig, string functionArgumentSecondaryAssignmentName, bool* foundAtLeastOneInstance)
{
	string functionTextUpdated = *functionTextOrig;
	
	string secondaryAssignmentDecarationHypotheticalExtract = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE) + functionArgumentSecondaryAssignmentName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET;	//* secondaryAssignmentName = 
	//cout << "secondaryAssignmentDecarationHypotheticalExtract = " << secondaryAssignmentDecarationHypotheticalExtract << endl;
	
	bool stillSearching = true;
	int pos = 0;
	while(stillSearching)
	{
		pos = functionTextUpdated.find(secondaryAssignmentDecarationHypotheticalExtract, pos);
		if(pos != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{//only update function reference within functionText once
			
			//int indexOfStartOfLine = functionText->rfind(STRING_NEW_LINE, pos);
			int indexOfFunctionArgumentSecondaryAssignmentType = CPP_STRING_FIND_RESULT_FAIL_VALUE;
			string functionArgumentSecondaryAssignmentType = extractFullVariableNameReverse(&functionTextUpdated, pos-1, &indexOfFunctionArgumentSecondaryAssignmentType);
			if(indexOfFunctionArgumentSecondaryAssignmentType != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				string secondaryAssignmentDecaration = functionArgumentSecondaryAssignmentType + secondaryAssignmentDecarationHypotheticalExtract;
				string secondaryAssignmentDecarationWithConst = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST) + secondaryAssignmentDecaration;
				//cout << "secondaryAssignmentDecaration = " << secondaryAssignmentDecaration << endl;
				//cout << "secondaryAssignmentDecarationWithConst = " << secondaryAssignmentDecarationWithConst << endl;
				
				functionTextUpdated.replace(indexOfFunctionArgumentSecondaryAssignmentType, secondaryAssignmentDecaration.length(), secondaryAssignmentDecarationWithConst);
				pos = pos + secondaryAssignmentDecarationWithConst.length();
				*foundAtLeastOneInstance = true;
			}
			else
			{
				cout << "replaceAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction{} error: cannot find functionArgumentSecondaryAssignmentType" << endl;
			}
		}
		else
		{
			stillSearching = false;
		}
	}
	
	return functionTextUpdated;
}

//generateConstFunctionArgumentsFunction limitation: currently doesn't support embedded function reference arguments, eg; doThis(arg1, doThis2(arg2, arg3));
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
		string functionNameFullUpdated = "";

		CSfunctionArgument* currentFunctionArgumentInFunction;
		
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
		currentFunctionObject->parseSecondaryReferencesOnly = true;
		currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{	
			string functionDeclarationArgument = currentFunctionArgumentInFunction->argumentName;
				
			if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument, false))
			{

			}
			
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
		currentFunctionObject->parseSecondaryReferencesOnly = false;
		#endif
		
		currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{	
			string functionDeclarationArgument = currentFunctionArgumentInFunction->argumentName;
		
			if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument, false))
			{

			}
			
			currentFunctionArgumentInFunction->constIdentified = true;
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
	}
				
	return result;
}

//CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS: limitation: trace secondary assignments of iterator (limitation: can only trace immediate secondary assignments of iterator variables)
bool generateConstFunctionArgumentAndSearchForSecondaryReferences(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument, bool ignoreListIterationNextAssignments)
{
	string* functionText = &(currentFunctionObject->functionText);
	
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	if(currentFunctionObject->parseSecondaryReferencesOnly)
	{
	#endif	
		currentFunctionArgumentInFunction->argumentNameAliasList.push_back(functionDeclarationArgument);
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	}
	#endif	
												
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;
	cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: currentFunctionArgumentInFunction->argumentName = " << currentFunctionArgumentInFunction->argumentName << endl;
	cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif

	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	if(!(currentFunctionObject->parseSecondaryReferencesOnly))
	{
	#endif		
		if(generateConstFunctionArgument(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument, ignoreListIterationNextAssignments))
		{

		}
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	}
	#endif
	
	int indexOfFunctionArgument = -1;
	while((indexOfFunctionArgument = functionText->find(functionDeclarationArgument, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		if(functionArgumentReferenceWholeWordCheck(functionText, functionDeclarationArgument, indexOfFunctionArgument))
		{		
			//find next occurance of ';' on same line
			int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfFunctionArgument);
			int indexOfEndOfLine = functionText->find(STRING_NEW_LINE, indexOfFunctionArgument);
			int indexOfStartOfLine = functionText->rfind(STRING_NEW_LINE, indexOfFunctionArgument);
			if(indexOfEndOfCommand < indexOfEndOfLine)
			{
				int indexOfEqualsSetPrevious = CPP_STRING_FIND_RESULT_FAIL_VALUE;
				if((indexOfEqualsSetPrevious = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					if(indexOfEqualsSetPrevious > indexOfStartOfLine)
					{
						if(indexOfEqualsSetPrevious == (indexOfFunctionArgument-string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET).length()))
						{//added condition 3h3a; e.g. " = functionArgument"
							
							//added condition CS3h3a - detect generic secondary assignment; e.g. "classname* x = Q;" / "classname* x = Q->subobject;"	
							bool foundGenericSecondaryAssignment = false;
							int indexOfStartOfSecondaryAssignment = 0;
							string secondaryAssignmentName = extractFullVariableNameReverse(functionText, indexOfEqualsSetPrevious-1, &indexOfStartOfSecondaryAssignment);
							if(indexOfStartOfSecondaryAssignment > indexOfStartOfLine+string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE).length())
							{
								if(functionText->substr(indexOfStartOfSecondaryAssignment-string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE).length(), string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE).length()) == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE)
								{
									foundGenericSecondaryAssignment = true;	
									#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
									cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: foundGenericSecondaryAssignment" << endl;
									cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: currentFunctionArgumentInFunction->argumentName = " << currentFunctionArgumentInFunction->argumentName << endl;
									cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
									cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: secondaryAssignmentName = " << secondaryAssignmentName << endl;
									#endif
									
									/*
									if(currentFunctionObject->name == "findFunctionReferenceTargetRecurse")
									{
										cout << "\tfunctionDeclarationArgument = " << functionDeclarationArgument << endl;
										cout << "\tsecondaryAssignmentName = " << secondaryAssignmentName << endl;
									}
									*/
								
									if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, secondaryAssignmentName, true))
									{

									}
								} 
							}
						
							#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS

							//trace for loop vector list iterators  (and secondary assignments)
							int indexOfIteratorType = CPP_STRING_FIND_RESULT_FAIL_VALUE;
							if((indexOfIteratorType = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
							{
								if(indexOfIteratorType > indexOfStartOfLine)
								{
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
											#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS	
											cout << "functionDeclarationArgumentHasBeginPointerBeingReferenced" << endl;
											#endif
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
											#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS	
											cout << "functionDeclarationArgumentHasFindPointerBeingReferenced" << endl;
											#endif
										}
									}

									if(indexOfIteratorType < indexOfEqualsSetPrevious)
									{
										string iteratorName = extractFullVariableName(functionText, indexOfIteratorType+string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR).length());

										#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
										cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: found for loop vector/map iterator" << endl;
										cout << "iteratorName = " << iteratorName << endl;
										#endif

										if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, iteratorName, false))
										{

										}

										//trace secondary assignments of iterator (limitation: can only trace immediate secondary assignments of iterator variables)
										//e.g. GIAentityNode* currentQueryEntityNode = connectionIterQuery->second;
										int indexOfEqualsSetFollowingLine = CPP_STRING_FIND_RESULT_FAIL_VALUE;
										if((indexOfEqualsSetFollowingLine = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfEndOfLine)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
										{
											int indexOfIteratorFollowingLine = CPP_STRING_FIND_RESULT_FAIL_VALUE;
											if((indexOfIteratorFollowingLine = functionText->find(iteratorName, indexOfEndOfLine)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
											{
												int indexOfStartOfSecondaryAssignmentOfIterator;	//not used
												string secondaryAssignmentOfIterator = extractFullVariableNameReverse(functionText, indexOfEqualsSetFollowingLine-1, &indexOfStartOfSecondaryAssignmentOfIterator);
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
													}
												}

												if(foundSecondaryIteratorAssignmentExact)
												{
													#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
													cout << "found iterator secondary assignment" << endl;
													#endif
													if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, iteratorName, false))
													{

													}
												}
											}
										}
									}
								}
							}

							#endif
						}			
					}
				}	
			}
		}
	}
					
	return result;
}


bool generateConstFunctionArgument(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument, bool ignoreListIterationNextAssignments)
{
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgument{}:" << endl;
	cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
	cout << "functionDeclarationArgument = " << functionDeclarationArgument << endl;
	cout << "currentFunctionArgumentInFunction->argumentType/Name = " << currentFunctionArgumentInFunction->argumentType << " " << currentFunctionArgumentInFunction->argumentName << endl;
	#endif

	//step 1: check function text to see if it contains a modification of the function argument
	if(checkIfVariableIsBeingModifiedInFunction(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument, ignoreListIterationNextAssignments))
	{	
		currentFunctionArgumentInFunction->isNotConst = true;
	}

	if(!(currentFunctionArgumentInFunction->isNotConst))
	{	
		//step 2: check function references to see if function argument has been passed to a child function (and if so whether it has been modified by the child function)
		//condition 2: the same requirement must be met by all functions called by functionX with the argument varnameX
		CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceListRepeats;
		int currentFunctionReferenceIndex = 0;
		while(currentFunctionReference->next != NULL)
		{
			CSfile* fileObjectHoldingFunction = currentFunctionReference->functionReferenceTargetFileOwner;
			CSfunction* functionReferenceTarget = currentFunctionReference->functionReferenceTarget;
			if(functionReferenceTarget != NULL)
			{
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
					if((currentFunctionArgumentInFunctionReference->argument).find(functionDeclarationArgument) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{
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
							exit(0);
						}

						currentFunctionArgumentInFunctionReferenceTarget = functionReferenceTarget->firstFunctionArgumentInFunction;
						for(int i=0; i<functionReferenceArgumentIndex; i++)
						{
							currentFunctionArgumentInFunctionReferenceTarget = currentFunctionArgumentInFunctionReferenceTarget->next;
						}
						if(currentFunctionArgumentInFunctionReferenceTarget->isNotConst)
						{
							currentFunctionArgumentInFunction->isNotConst = true;
						}
					}
					currentFunctionArgumentInFunctionReference = currentFunctionArgumentInFunctionReference->next;
					functionReferenceArgumentIndex++;
				}
			}
			currentFunctionReferenceIndex++;
			currentFunctionReference = currentFunctionReference->next;
		}
	}
}
			
			
bool checkIfVariableIsBeingModifiedInFunction(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument, bool ignoreListIterationNextAssignments)
{ 
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "checkIfVariableIsBeingModifiedInFunction: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif
	
	bool isNotConst = false;
	string* functionText = &(currentFunctionObject->functionText);
				
	//step 1: check function text to see if it contains a modification of the function argument
	//condition 1: text after varnameX, and before ';' on same line must not contain an equals signs [meaning; the variable is not being set by anothing]
	int indexOfFunctionArgument = -1;
														
	while((indexOfFunctionArgument = functionText->find(functionDeclarationArgument, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		if(functionArgumentReferenceWholeWordCheck(functionText, functionDeclarationArgument, indexOfFunctionArgument))
		{
			//ensure variable is not preceeded by a -> or . (ie is not actually the function argument but a subset of another object), e.g. pm->wide = wide;
			if((indexOfFunctionArgument == 0) || !charInCharArray((*functionText)[indexOfFunctionArgument-1], codeReferenceLastCharacters, CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE))
			{//added 3h2d

				//find next occurance of ';' on same line
				//cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;
				int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfFunctionArgument);
				int indexOfEndOfLine = functionText->find(STRING_NEW_LINE, indexOfFunctionArgument);
				int indexOfStartOfLine = functionText->rfind(STRING_NEW_LINE, indexOfFunctionArgument);
				int indexOfEqualsSet = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument);
				int indexOfEqualsSetPrevious = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument);
				int indexOfSquareBracketOpen = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_INDEX_OPEN, indexOfFunctionArgument);
				int indexOfSquareBracketClose = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_INDEX_CLOSE, indexOfFunctionArgument);
				int indexOfCout = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START, indexOfFunctionArgument);				
				//added condition CS3h1d, updated CS3h3b - ensure not a cout start, e.g. 'cout << "indexOfFunctionArgument' in; 'cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;'
				if((indexOfCout == CPP_STRING_FIND_RESULT_FAIL_VALUE) || (indexOfCout < indexOfStartOfLine))
				{
					#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS
					if((indexOfEqualsSetPrevious == CPP_STRING_FIND_RESULT_FAIL_VALUE) || (indexOfEqualsSetPrevious < indexOfStartOfLine))
					{//ignore all lines with assignments [this indicates that the functionArgument objectFunction is not modifying the functionArgument] e.g. "int indexOfFunctionReferenceStartOfLine = functionContentsString->rfind(stringToFind, indexToFunctionReference)"
						
						int indexOfBracketOpenPrevious = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE, indexOfFunctionArgument);
						if((indexOfBracketOpenPrevious == CPP_STRING_FIND_RESULT_FAIL_VALUE) || (indexOfBracketOpenPrevious < indexOfStartOfLine))
						{//e.g. ignore all lines with preceeding brackets, e.g. "format.c_str()" in "sprintf(stringCharStar, format.c_str(), number);"
							
							for(int i=0; i<CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE; i++)
							{
								int indexOfObjectFunctionName = indexOfFunctionArgument+functionDeclarationArgument.length()+codeReference[i].length();
								if(indexOfObjectFunctionName+1 < functionText->length())	//+1 to compensate for function name
								{
									if(functionText->substr(indexOfFunctionArgument+functionDeclarationArgument.length(), codeReference[i].length()) == codeReference[i])
									{
										//detected "->" or "."
										//cout << "detected -> or ." << endl;
										string objectFunctionName = extractFullVariableName(functionText, indexOfObjectFunctionName);
										int indexOfObjectFunctionOpeningBracket = indexOfObjectFunctionName+objectFunctionName.length();
										if(indexOfObjectFunctionOpeningBracket+1 < functionText->length())	//+1 to compensate for (
										{
											if((*functionText)[indexOfObjectFunctionOpeningBracket] == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE)
											{
												//detect execution of object functions: e.g. "functionArgument->WHOLEWORDFUNCTIONNAME("
												isNotConst = true;
												/*
												cout << "detect execution of object functions" << endl;
												cout << "functionDeclarationArgument = " << functionDeclarationArgument << endl;
												cout << "objectFunctionName = " << objectFunctionName << endl;
												exit(0);
												*/
											}
										}

									}
								}
							}
						}
					}
					#endif
					
					#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_INCREMENT_DECREMENTS
					//added CS3h7a - detect functionArgument++/++functionArgument/functionArgument--/--functionArgument
					string twoCharactersBeforeFunctionArgument = ""; 
					string twoCharactersAfterFunctionArgument = "";
					if(indexOfFunctionArgument >= 2)
					{
						twoCharactersBeforeFunctionArgument = functionText->substr(indexOfFunctionArgument-2, 2);
					}
					if(indexOfFunctionArgument+functionDeclarationArgument.length()+2 < functionText->length())
					{
						twoCharactersAfterFunctionArgument = functionText->substr(indexOfFunctionArgument+functionDeclarationArgument.length(), 2);
					}
					if((twoCharactersBeforeFunctionArgument == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_INCREMENT) || (twoCharactersBeforeFunctionArgument == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_DECREMENT)
					|| (twoCharactersAfterFunctionArgument == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_INCREMENT) || (twoCharactersAfterFunctionArgument == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_DECREMENT))
					{
						isNotConst = true;
					}
					#endif
										
					if(indexOfEndOfCommand < indexOfEndOfLine)
					{			
						if((indexOfFunctionArgument < indexOfEqualsSet) && (indexOfEqualsSet < indexOfEndOfCommand))
						{//restored CS3h2c

							string textBeforeFunctionArgument = functionText->substr(indexOfStartOfLine, indexOfFunctionArgument-indexOfStartOfLine);
							if(!charInString(textBeforeFunctionArgument, functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
							{//added condition CS3h3b - ignore new type initialisations of secondary assignments e.g. "currentReferenceInFunctionReferenceListRepeats" in "CSfunction* currentReferenceInFunctionReferenceListRepeats = function->firstReferenceInFunctionReferenceListRepeats;"

								bool passMiscellaneousConditions = true;
								if(ignoreListIterationNextAssignments)	//no longer used
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

												string currentLine = functionText->substr(indexOfStartOfLine, indexOfEndOfLine-indexOfStartOfLine);	
											}
										}
									}
								}
								if((indexOfSquareBracketOpen != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfSquareBracketClose != CPP_STRING_FIND_RESULT_FAIL_VALUE))
								{
									if((indexOfSquareBracketOpen > indexOfStartOfLine) && (indexOfSquareBracketClose < indexOfEqualsSet))
									{
										//e.g. imageWidth in "contrastMap[y*imageWidth + x] = contrastVal;"
										passMiscellaneousConditions = false;
									}
								}

								if(passMiscellaneousConditions)
								{
									isNotConst = true;
									if(functionDeclarationArgument == "contrastVal")
									{								
										string lineText = functionText->substr(indexOfStartOfLine, indexOfEndOfCommand-indexOfStartOfLine);
									}
								}

								/*
								e.g.
								functionArgument = 5;
								*functionArgument = 5;
								functionArgument->parameter = 5;
								*/
							}
						}
						
						if((indexOfEqualsSetPrevious > indexOfStartOfLine) && (indexOfEqualsSetPrevious < indexOfEndOfCommand))
						{
							bool inverseNonConstAssignmentDetected = false;
							string textBeforeFunctionArgument = functionText->substr(indexOfStartOfLine, indexOfFunctionArgument-indexOfStartOfLine);
							CSfunctionArgument* currentFunctionArgumentInFunctionTemp = currentFunctionObject->firstFunctionArgumentInFunction;
							while(currentFunctionArgumentInFunctionTemp->next != NULL)
							{
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS
								//if(!detectDoublePointer(currentFunctionArgumentInFunction->argument))
								//{								
								if(detectDoublePointer(currentFunctionArgumentInFunctionTemp->argument))
								{
									//if(currentFunctionArgumentInFunctionTemp != currentFunctionArgumentInFunction)
									//{
									string doublePointerAssignmentTextHypothetical = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER) + currentFunctionArgumentInFunctionTemp->argumentName;	//*doublePointer

									int textBeforeFunctionArgumentIndex = textBeforeFunctionArgument.find(doublePointerAssignmentTextHypothetical);
									int equalsSetPreviousIndex = textBeforeFunctionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET);
									if(textBeforeFunctionArgumentIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE)
									{
										if(textBeforeFunctionArgumentIndex < equalsSetPreviousIndex)
										{
											//e.g. "*doublepointerargument = functionArgument" / "*doublepointerargument = functionArgument->q"
											inverseNonConstAssignmentDetected = true;		
										}								
									}
									//}
								}
								//}
								#endif
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
								//preconditions: argumentNameAliasList has already been filled for all function arguments
								for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunctionTemp->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunctionTemp->argumentNameAliasList.end(); argumentNameAliasListIter++)
								{
									//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
									string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
									int aliasIndex = textBeforeFunctionArgument.find(functionArgumentSecondaryAssignmentName);
									int equalsSetPreviousIndex = textBeforeFunctionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET);
									if(aliasIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE)
									{
										if(aliasIndex < equalsSetPreviousIndex)
										{
											if(functionArgumentReferenceWholeWordCheck(&textBeforeFunctionArgument, functionArgumentSecondaryAssignmentName, aliasIndex))
											{
												//e.g. "nonconstargumentORsecondaryReferenceToNonConstArgument[->optionalSubObject] = functionArgument"
												inverseNonConstAssignmentDetected = true;
											}
										}
									}
								}
								#endif

								currentFunctionArgumentInFunctionTemp = currentFunctionArgumentInFunctionTemp->next;
							}
							if(inverseNonConstAssignmentDetected)
							{
								isNotConst = true;
								#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
								cout << "checkIfVariableIsBeingModifiedInFunction{}: inverseNonConstAssignmentDetected: isNotConst = true" << endl;
								#endif
							}
						}
					}
				}
			}
		}
	}
	
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_RETURN_OBJECTS
	int indexOfReturn = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_RETURN);
	if(indexOfReturn != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		int indexOfStartOfLine = functionText->rfind(STRING_NEW_LINE, indexOfReturn);
		int indexOfEndOfLine = functionText->find(STRING_NEW_LINE, indexOfReturn);
		int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfReturn);
		if(indexOfEndOfCommand < indexOfEndOfLine)
		{
			int indexOfReturnVar = indexOfReturn + string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_RETURN).length(); 
			string returnVar = functionText->substr(indexOfReturnVar, indexOfEndOfCommand-indexOfReturnVar);
			//cout << "returnVar = " << returnVar << endl;
			
			//now see if the returnVar is functionDeclarationArgument
			if(returnVar == functionDeclarationArgument)
			{
				isNotConst = true;
			}
			
			//now see if can locate the string "returnVar = functionDeclarationArgument;" in the function
			string returnVarSetTextHypothetical = returnVar + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + functionDeclarationArgument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;
			if(functionText->find(returnVarSetTextHypothetical) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				isNotConst = true;
				//cout << "returnVarSetTextHypothetical = " << returnVarSetTextHypothetical << endl;
			} 
		}
	}
	#endif
								
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

string extractFullVariableNameReverse(string* functionText, int indexOfEndOfVariableName, int* indexOfStartOfVariableName)
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
			*indexOfStartOfVariableName = i;
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

bool charInString(string text, char* charArray, int arraySize)
{
	bool result = false;
	for(int i=0; i<text.length(); i++)
	{
		bool foundMatchedChar = false;
		for(int j=0; j<arraySize; j++)
		{
			if(text[i] == charArray[j])
			{
				foundMatchedChar = true;
			}
		}		
		if(foundMatchedChar)
		{
			result = true;
		}
	}
	return result;
}

#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS
bool detectDoublePointer(string functionArgument)
{
	bool result = false;
	if(functionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_DOUBLE_POINTER_TYPE) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		result = true;
	}
	return result;
}
#endif
				
#endif
