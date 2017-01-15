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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3i17a 20-September-2016
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

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentsRecurse{}" << endl;
	#endif

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

					bool headerExists = false;
					bool sourceExists = false;
					if(fileExists(currentFileObject->name))
					{
						currentFileObject->headerFileTextOrig = getFileContents(currentFileObject->name);	//headerFileNameOrig
						currentFileObject->headerFileText = currentFileObject->headerFileTextOrig;	//initialise with same source code as original file
						headerExists = true;
					}
					if(fileExists(currentFileObject->sourceFileNameOrig))
					{
						currentFileObject->sourceFileTextOrig = getFileContents(currentFileObject->sourceFileNameOrig);
						currentFileObject->sourceFileText = currentFileObject->sourceFileTextOrig;	//initialise with same source code as original file
						sourceExists = true;
					}
					if(headerExists || sourceExists)
					{
						#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
						//cout << "generateConstFunctionArgumentsRecurse{}: currentFileObject->sourceFileName = " << currentFileObject->sourceFileName << endl;
						//cout << "generateConstFunctionArgumentsRecurse{}: currentFileObject->headerFileName = " << currentFileObject->headerFileName << endl;
						#endif
						if(!generateConstFunctionArgumentsFile(currentFileObject))
						{
							result = false;
						}
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

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "end generateConstFunctionArgumentsRecurse{}" << endl;
	#endif

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
				if((currentFunctionArgumentInFunction->argument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST) == CPP_STRING_FIND_RESULT_FAIL_VALUE) && (currentFunctionArgumentInFunction->argument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST_EFFECTIVE) == CPP_STRING_FIND_RESULT_FAIL_VALUE))
				{//added CS3h7a - ignore precoded const function arguments
				#endif

					bool isConstEffective = false;
					string constString = CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST;
					#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
					if(!(currentFunctionArgumentInFunction->isNotConst) && (currentFunctionArgumentInFunction->isConstEffective))
					{//NB !isNotConst overrides isConstEffective value
						isConstEffective = true;
						constString = CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CONST_EFFECTIVE;
					}
					#endif

					string argument = currentFunctionArgumentInFunction->argument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_ARGUMENT_DELIMITER;
					string argumentWithConsts = constString + argument;
					#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
					cout << "argument = " << currentFunctionArgumentInFunction->argument << endl;
					cout << "argumentWithConsts = " << argumentWithConsts << endl;
					#endif
					nameFullWithConsts = replaceAllOccurancesOfString(&(nameFullWithConsts), argument, argumentWithConsts);

					argument = currentFunctionArgumentInFunction->argument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_CLOSE;
					argumentWithConsts = constString + argument;
					#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
					cout << "argument = " << currentFunctionArgumentInFunction->argument << endl;
					cout << "argumentWithConsts = " << argumentWithConsts << endl;
					#endif
					nameFullWithConsts = replaceAllOccurancesOfString(&(nameFullWithConsts), argument, argumentWithConsts);

					//add const to function argument secondary assignements - CS3h5a
					for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunction->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunction->argumentNameAliasList.end(); argumentNameAliasListIter++)
					{
						#ifdef CS_DEBUG
						//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
						#endif
						string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
						int posOfFunctionText = currentFileObject->sourceFileText.find(currentFunctionObject->functionTextRaw);
						int functionTextOrigLength = currentFunctionObject->functionTextRaw.length();
						if(posOfFunctionText != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							bool foundAtLeastOneInstance = false;
							currentFunctionObject->functionTextRaw = addConstToAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction(&(currentFunctionObject->functionTextRaw), functionArgumentSecondaryAssignmentName, &foundAtLeastOneInstance, constString);
							if(foundAtLeastOneInstance)
							{
								#ifdef CS_DEBUG
								//cout << "foundAtLeastOneInstance" << endl;
								#endif
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
	if(currentFileObject->headerFileText != "")
	{
		writeStringToFile(currentFileObject->headerFileName, &(currentFileObject->headerFileText));
	}
	if(currentFileObject->sourceFileText != "")
	{
		writeStringToFile(currentFileObject->sourceFileName, &(currentFileObject->sourceFileText));
	}
	#else
	#ifndef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DISABLE_OUTPUT_DISABLE_PRINT_DEBUG
	cout << "currentFileObject->headerFileText = \n" << currentFileObject->headerFileText << endl;
	cout << "currentFileObject->sourceFileText = \n" << currentFileObject->sourceFileText << endl;
	#endif
	#endif

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "end generateConstFunctionArgumentsFile{}: currentFileObject->name = " << currentFileObject->name << endl;
	#endif

	return result;
}

string addConstToAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction(string* functionTextOrig, string functionArgumentSecondaryAssignmentName, bool* foundAtLeastOneInstance, string constString)
{
	string functionTextUpdated = *functionTextOrig;

	string secondaryAssignmentDecarationHypotheticalExtract[3];
	secondaryAssignmentDecarationHypotheticalExtract[0] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE) + functionArgumentSecondaryAssignmentName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET;	//* secondaryAssignmentName =
	#ifdef CS_DEBUG
	//cout << "secondaryAssignmentDecarationHypotheticalExtract = " << secondaryAssignmentDecarationHypotheticalExtract << endl;
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS
	secondaryAssignmentDecarationHypotheticalExtract[1] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR) + functionArgumentSecondaryAssignmentName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET;	//::iterator secondaryAssignmentName =
	secondaryAssignmentDecarationHypotheticalExtract[2] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR_REVERSE) + functionArgumentSecondaryAssignmentName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET;	//::reverse_iterator secondaryAssignmentName =
	int secondaryAssignmentDecarationHypotheticalExtractSize = 3;
	#else
	int secondaryAssignmentDecarationHypotheticalExtractSize = 1;
	#endif

	bool stillSearching = true;
	int pos = 0;
	while(stillSearching)
	{
		for(int i=0; i<secondaryAssignmentDecarationHypotheticalExtractSize; i++)
		{
			pos = functionTextUpdated.find(secondaryAssignmentDecarationHypotheticalExtract[i], pos);
			if(pos != CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{//only update function reference within functionText once

				int indexOfFunctionArgumentSecondaryAssignmentType = CPP_STRING_FIND_RESULT_FAIL_VALUE;
				string functionArgumentSecondaryAssignmentType = extractFullVariableTypeReverse(&functionTextUpdated, pos-1, &indexOfFunctionArgumentSecondaryAssignmentType);
				if(indexOfFunctionArgumentSecondaryAssignmentType != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					string secondaryAssignmentDecaration = functionArgumentSecondaryAssignmentType + secondaryAssignmentDecarationHypotheticalExtract[i];
					string secondaryAssignmentDecarationWithConst = constString + secondaryAssignmentDecaration;
					if(i > 0)
					{
						cout << "secondaryAssignmentDecaration = " << secondaryAssignmentDecaration << endl;
						cout << "secondaryAssignmentDecarationWithConst = " << secondaryAssignmentDecarationWithConst << endl;
						exit(0);
					}

					functionTextUpdated.replace(indexOfFunctionArgumentSecondaryAssignmentType, secondaryAssignmentDecaration.length(), secondaryAssignmentDecarationWithConst);
					pos = pos + secondaryAssignmentDecarationWithConst.length();
					*foundAtLeastOneInstance = true;
				}
				else
				{
					cout << "addConstToAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction{} error: cannot find functionArgumentSecondaryAssignmentType" << endl;
				}
			}
			else
			{
				stillSearching = false;
			}
		}
	}

	return functionTextUpdated;
}

//generateConstFunctionArgumentsFunction limitation: currently doesn't support embedded function reference arguments, eg; doThis(arg1, doThis2(arg2, arg3));
bool generateConstFunctionArgumentsFunction(CSfunction* currentFunctionObject)
{
	string* functionText = &(currentFunctionObject->functionText);

	bool result = true;

	if(!(currentFunctionObject->functionArgumentConstsIdentified))
	{
		#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
		cout << "generateConstFunctionArgumentsFunction{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;
		#endif

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

			if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument))
			{

			}

			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
		currentFunctionObject->parseSecondaryReferencesOnly = false;
		#endif

		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION
		currentFunctionObject->parseSecondaryReferencesAndModificationsOnly = true;
		currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{
			string functionDeclarationArgument = currentFunctionArgumentInFunction->argumentName;

			if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument))
			{

			}

			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
		currentFunctionObject->parseSecondaryReferencesAndModificationsOnly = false;
		#endif

		currentFunctionArgumentInFunction = currentFunctionObject->firstFunctionArgumentInFunction;
		while(currentFunctionArgumentInFunction->next != NULL)
		{
			string functionDeclarationArgument = currentFunctionArgumentInFunction->argumentName;

			if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument))
			{

			}

			currentFunctionArgumentInFunction->constIdentified = true;
			currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
		}
	}

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "end generateConstFunctionArgumentsFunction{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;
	#endif

	return result;
}

//CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS: limitation: trace secondary assignments of iterator (limitation: can only trace immediate secondary assignments of iterator variables)
bool generateConstFunctionArgumentAndSearchForSecondaryReferences(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument)
{
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;
	cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: currentFunctionArgumentInFunction->argumentName = " << currentFunctionArgumentInFunction->argumentName << endl;
	cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif

	bool result = true;

	string* functionText = &(currentFunctionObject->functionText);

	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	if(currentFunctionObject->parseSecondaryReferencesOnly)
	{
	#endif
		currentFunctionArgumentInFunction->argumentNameAliasList.push_back(functionDeclarationArgument);
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	}
	else
	{
	#endif
		if(generateConstFunctionArgument(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument))
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

									if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, secondaryAssignmentName))
									{

									}
								}
							}

							#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PARSE_LISTS

							//trace for loop vector list iterators  (and secondary assignments)
							int indexOfIteratorType = CPP_STRING_FIND_RESULT_FAIL_VALUE;
							bool foundIterator = false;
							string iterType;
							if((indexOfIteratorType = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
							{
								foundIterator = true;
								iterType = CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR;
							}
							else
							{
								if((indexOfIteratorType = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR_REVERSE, indexOfFunctionArgument)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
								{
									foundIterator = true;
									iterType = CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_VECTOR_OR_MAP_ITERATOR_REVERSE;
								}
							}
							if(foundIterator)
							{
								if(indexOfIteratorType > indexOfStartOfLine)
								{
									//added condition CS3h2a - detect list; eg "for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); ..."
										//can't handle cases where iterator is defined on separate line to for loop (or where multiple iterators are used to refer to the same list), eg; NLCtranslatorCodeBlocksLogicalConditions.cpp: generateCodeBlocksFromMathText: map<int, vector<GIAentityNode*>*>::iterator sentenceIter = sentenceIterFirstInFullSentence;
										//can't handle cases where vector/map functions are executed to modify the vector/map (or where multiple iterators are used to refer to the same list), eg; GIAtranslatorOperations.cpp: eraseSubstanceFromSubstanceList: entityNodesActiveListSubstances->erase(substanceEntityNodesListIteratorFound);

									/*
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
									*/

									if(indexOfIteratorType < indexOfEqualsSetPrevious)
									{
										string iteratorName = extractFullVariableName(functionText, indexOfIteratorType+iterType.length());

										#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
										cout << "generateConstFunctionArgumentAndSearchForSecondaryReferences{}: found for loop vector/map iterator" << endl;
										cout << "iteratorName = " << iteratorName << endl;
										#endif

										if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, iteratorName))
										{

										}

										//trace secondary assignments of iterator
										//e.g. GIAentityNode* currentQueryEntityNode = connectionIterQuery->second;
										string followingLineHypothetical[4];
										followingLineHypothetical[0] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET) + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CLOSE_PARAMETER_SPACE + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//[currentQueryEntityNode] = (*connectionIterQuery);
										followingLineHypothetical[1] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET) + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//[currentQueryEntityNode] = *connectionIterQuery;
										followingLineHypothetical[2] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET) + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_MAP_ITERATOR_FIRST + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//[currentQueryEntityNode] = connectionIterQuery->first;
										followingLineHypothetical[3] = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET) + iteratorName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_MAP_ITERATOR_SECOND + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//[currentQueryEntityNode] = connectionIterQuery->second;
										int followingLineHypotheticalSize = 4;

										for(int i=0; i<followingLineHypotheticalSize; i++)
										{
											int indexOfFollowingLineHypotheticalMatch = functionText->find(followingLineHypothetical[i], indexOfEndOfLine);
											if(indexOfFollowingLineHypotheticalMatch != CPP_STRING_FIND_RESULT_FAIL_VALUE)
											{
												int indexOfStartOfSecondaryAssignmentOfIterator;
												string secondaryAssignmentOfIterator = extractFullVariableNameReverse(functionText, indexOfFollowingLineHypotheticalMatch-1, &indexOfStartOfSecondaryAssignmentOfIterator);

												#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
												cout << "found iterator secondary assignment" << endl;
												#endif
												if(generateConstFunctionArgumentAndSearchForSecondaryReferences(currentFunctionObject, currentFunctionArgumentInFunction, secondaryAssignmentOfIterator))
												{

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

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "end generateConstFunctionArgumentAndSearchForSecondaryReferences{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif

	return result;
}


bool generateConstFunctionArgument(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument)
{
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "generateConstFunctionArgument{}:" << endl;
	cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
	cout << "functionDeclarationArgument = " << functionDeclarationArgument << endl;
	cout << "currentFunctionArgumentInFunction->argumentType/Name = " << currentFunctionArgumentInFunction->argumentType << " " << currentFunctionArgumentInFunction->argumentName << endl;
	#endif

	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION
	if(currentFunctionObject->parseSecondaryReferencesAndModificationsOnly)
	{
	#endif
		//step 1: check function text to see if it contains a modification of the function argument
		bool isNotConst = false;
		bool isConstEffective = false;
		checkIfVariableIsBeingModifiedInFunction(currentFunctionObject, currentFunctionArgumentInFunction, functionDeclarationArgument, &isNotConst, &isConstEffective);
		if(isNotConst)
		{
			currentFunctionArgumentInFunction->isNotConst = true;
		}
		if(isConstEffective)
		{
			currentFunctionArgumentInFunction->isConstEffective = true;
		}
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION
	}
	else
	{
	#endif
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
						int indexOfFunctionDeclarationArgumentInFunctionReference = (currentFunctionArgumentInFunctionReference->argument).find(functionDeclarationArgument);
						if(indexOfFunctionDeclarationArgumentInFunctionReference != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							if(functionArgumentReferenceWholeWordCheck(&(currentFunctionArgumentInFunctionReference->argument), functionDeclarationArgument, indexOfFunctionDeclarationArgumentInFunctionReference))
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
								if(currentFunctionArgumentInFunctionReferenceTarget->isConstEffective)
								{
									currentFunctionArgumentInFunction->isConstEffective = true;
								}
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
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION
	}
	#endif

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "end generateConstFunctionArgument{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif
}


void checkIfVariableIsBeingModifiedInFunction(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument, bool* isNotConst, bool* isConstEffective)
{
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "checkIfVariableIsBeingModifiedInFunction{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif

	string* functionText = &(currentFunctionObject->functionText);


	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_RETURN_OBJECTS
	//NB this implementation is non-standard because it has to support return of both typeX* and standard types (eg string/double); and string/double secondary assignments are not currently recorded by
	int indexOfReturn = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_RETURN);
	string returnVar = "";
	if(indexOfReturn != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		int indexOfStartOfLine = functionText->rfind(STRING_NEW_LINE, indexOfReturn);
		int indexOfEndOfLine = functionText->find(STRING_NEW_LINE, indexOfReturn);
		int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfReturn);
		if(indexOfEndOfCommand < indexOfEndOfLine)
		{
			int indexOfReturnVar = indexOfReturn + string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_RETURN).length();
			returnVar = functionText->substr(indexOfReturnVar, indexOfEndOfCommand-indexOfReturnVar);
			#ifdef CS_DEBUG
			//cout << "returnVar = " << returnVar << endl;
			#endif

			//now see if the returnVar is functionDeclarationArgument
			if(returnVar == functionDeclarationArgument)
			{
				//e.g. "return functionDeclarationArgument;"
				*isNotConst = true;
			}
		}
	}
	#endif

	//step 1: check function text to see if it contains a modification of the function argument
	//condition 1: text after varnameX, and before ';' on same line must not contain an equals signs [meaning; the variable is not being set by anothing]
	int indexOfFunctionArgument = -1;

	while((indexOfFunctionArgument = functionText->find(functionDeclarationArgument, indexOfFunctionArgument+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		if(functionArgumentReferenceWholeWordCheck(functionText, functionDeclarationArgument, indexOfFunctionArgument))
		{
			//ensure variable is not preceeded by a -> or . (ie is not actually the function argument but a subset of another object), e.g. pm->wide = wide;
			if((indexOfFunctionArgument == 0) || !charInCharArray((*functionText)[indexOfFunctionArgument-1], codeReferenceLastCharacters, CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE_NUMBER_OF_TYPES))
			{//added 3h2d

				//find next occurance of ';' on same line
				#ifdef CS_DEBUG
				//cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;
				#endif
				int indexOfEndOfCommand = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND, indexOfFunctionArgument);
				int indexOfEndOfLine = functionText->find(STRING_NEW_LINE, indexOfFunctionArgument);
				int indexOfStartOfLine = functionText->rfind(STRING_NEW_LINE, indexOfFunctionArgument);
				int indexOfEqualsSet = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument);
				int indexOfEqualsSetPrevious = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET, indexOfFunctionArgument);
				int indexOfSquareBracketOpen = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_INDEX_OPEN, indexOfFunctionArgument);
				int indexOfSquareBracketClose = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_INDEX_CLOSE, indexOfFunctionArgument);
				int indexOfCout = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_COUT_START, indexOfFunctionArgument);
				string currentLine = functionText->substr(indexOfStartOfLine, indexOfEndOfLine-indexOfStartOfLine);

				//added condition CS3h1d, updated CS3h3b - ensure not a cout start, e.g. 'cout << "indexOfFunctionArgument' in; 'cout << "indexOfFunctionArgument = " << indexOfFunctionArgument << endl;'
				if((indexOfCout == CPP_STRING_FIND_RESULT_FAIL_VALUE) || (indexOfCout < indexOfStartOfLine))
				{
					#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS
					if((indexOfEqualsSetPrevious == CPP_STRING_FIND_RESULT_FAIL_VALUE) || (indexOfEqualsSetPrevious < indexOfStartOfLine))
					{//ignore all lines with assignments [this indicates that the functionArgument objectFunction is not modifying the functionArgument] e.g. "int indexOfFunctionReferenceStartOfLine = functionContentsString->rfind(stringToFind, indexToFunctionReference)"

						//modified CS3h11e
						bool lineIncludesNonObjectFunctionReference = false;
						int indexOfBracketOpenPrevious = functionText->rfind(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE, indexOfFunctionArgument);
						if((indexOfBracketOpenPrevious != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfBracketOpenPrevious > indexOfStartOfLine))
						{
							if((indexOfEndOfCommand != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfEndOfCommand < indexOfEndOfLine))
							{
								//ignore all lines with function reference on same line, e.g. "sprintf(stringCharStar, format.c_str(), number); / for(int i=0; i<x.length()..." / etc
								lineIncludesNonObjectFunctionReference = true;
							}
							else
							{
								//accept this case; "if(!(parseFileObject->get(currentToken)))"...
							}

						}
						if(!lineIncludesNonObjectFunctionReference)
						{
							int indexOfBracketOpen = functionText->find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE, indexOfFunctionArgument);
							if((indexOfBracketOpen != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfBracketOpen < indexOfEndOfLine))
							{
								int indexOfObjectFunctionName = CPP_STRING_FIND_RESULT_FAIL_VALUE;
								string objectFunctionName = extractFullVariableNameReverse(functionText, indexOfBracketOpen-1, &indexOfObjectFunctionName);
								if(objectFunctionName != "")
								{
									for(int i=0; i<CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE_NUMBER_OF_TYPES; i++)
									{
										if(functionText->substr(indexOfObjectFunctionName-codeReference[i].length(), codeReference[i].length()) == codeReference[i])
										{
											//detect execution of object functions: e.g. "functionArgument->WHOLEWORDFUNCTIONNAME(" / "functionArgument->someIntermediaryObject[].WHOLEWORDFUNCTIONNAME("
											*isNotConst = true;

											#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS_DETECT_PARAMETERS_FUNCTION_ARGUMENTS_OR_SECONDARY_ASSIGNMENTS
											CSfunctionArgument* currentFunctionArgumentInFunctionTemp = currentFunctionObject->firstFunctionArgumentInFunction;
											while(currentFunctionArgumentInFunctionTemp->next != NULL)
											{
												//NB this code is redundant if CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES is enabled, since argumentNameAliasList includes the original (primary) functionArgument;
												int indexOfFunctionArgumentTemp = indexOfObjectFunctionName+objectFunctionName.length();	//-1 not required because of '('
												while((indexOfFunctionArgumentTemp = functionText->find(currentFunctionArgumentInFunctionTemp->argumentName, indexOfFunctionArgumentTemp+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
												{
													if((indexOfFunctionArgumentTemp != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfFunctionArgumentTemp < indexOfEndOfLine))
													{
														if(functionArgumentReferenceWholeWordCheck(functionText, currentFunctionArgumentInFunctionTemp->argumentName, indexOfFunctionArgumentTemp))
														{
															//e.g. "entityNodesActiveListCompleteFastIndex->insert(pair<string, GIAentityNode*>(entityNodesTempActiveListCompleteIndex, entityNode));"
															currentFunctionArgumentInFunctionTemp->isNotConst = true;
														}
													}
												}

												#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
												//preconditions: argumentNameAliasList has already been filled for all function arguments
												for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunctionTemp->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunctionTemp->argumentNameAliasList.end(); argumentNameAliasListIter++)
												{
													#ifdef CS_DEBUG
													//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
													#endif
													string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
													int indexOfFunctionArgumentSecondaryAssignment = indexOfObjectFunctionName+objectFunctionName.length();	//-1 not required because of '('
													while((indexOfFunctionArgumentSecondaryAssignment = functionText->find(functionArgumentSecondaryAssignmentName, indexOfFunctionArgumentSecondaryAssignment+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
													{
														if((indexOfFunctionArgumentSecondaryAssignment != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfFunctionArgumentSecondaryAssignment < indexOfEndOfLine))
														{
															if(functionArgumentReferenceWholeWordCheck(functionText, functionArgumentSecondaryAssignmentName, indexOfFunctionArgumentSecondaryAssignment))
															{
																currentFunctionArgumentInFunctionTemp->isNotConst = true;
															}
														}
													}
												}
												#endif

												currentFunctionArgumentInFunctionTemp = currentFunctionArgumentInFunctionTemp->next;
											}
											#endif
										}
									}
								}
							}
						}
					}
					#endif
					#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_CSTDLIB_NON_OBJECT_FUNCTION_EXECUTIONS
					bool lineIncludesStdLibNonObjectFunctionReference = false;
					for(int i=0; i<CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CSTDLIB_NON_OBJECT_FUNCTIONS_NUMBER_OF_TYPES; i++)
					{
						string stdLibNonObjectFunctionExecutionHypothetical = cstdlibNonObjectFunctions[i] + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE + functionDeclarationArgument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_CLOSE_PARAMETER_SPACE + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;
						if(currentLine.find(stdLibNonObjectFunctionExecutionHypothetical) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							*isNotConst = true;
							//e.g. free(functionDeclarationArgument);
							#ifdef CS_DEBUG
							//cout << "stdLibNonObjectFunctionExecutionHypothetical = " << stdLibNonObjectFunctionExecutionHypothetical << endl;
							#endif
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
						*isNotConst = true;
					}
					#endif

					if(indexOfEndOfCommand < indexOfEndOfLine)
					{
						if((indexOfFunctionArgument < indexOfEqualsSet) && (indexOfEqualsSet < indexOfEndOfCommand))
						{//restored CS3h2c

							string textBeforeFunctionArgument = functionText->substr(indexOfStartOfLine, indexOfFunctionArgument-indexOfStartOfLine);
							if(verifyThatVariableIsNotDeclarationOrNextIteratorAssignmentOrArrayIndex(currentFunctionObject, functionText, textBeforeFunctionArgument, functionDeclarationArgument, indexOfFunctionArgument, indexOfSquareBracketOpen, indexOfSquareBracketClose, indexOfEndOfCommand, indexOfStartOfLine, indexOfEqualsSet))
							{
								bool doublePointerAssignmentDetected = false;
								if(detectDoublePointer(currentFunctionArgumentInFunction->argument))
								{
									CSfunctionArgument* currentFunctionArgumentInFunctionTemp = currentFunctionObject->firstFunctionArgumentInFunction;
									while(currentFunctionArgumentInFunctionTemp->next != NULL)
									{
										//modified CS3h13a

										//NB this code is redundant if CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES is enabled, since argumentNameAliasList includes the original (primary) functionArgument;
										string doublePointerAssignmentTextHypothetical = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER) + functionDeclarationArgument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + currentFunctionArgumentInFunctionTemp->argumentName;	//*doublePointerfunctionArgument = functionArgument
										int doublePointerAssignmentTextHypotheticalIndex = functionText->find(doublePointerAssignmentTextHypothetical, indexOfStartOfLine);
										if((doublePointerAssignmentTextHypotheticalIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (doublePointerAssignmentTextHypotheticalIndex < indexOfEndOfLine))
										{
											//e.g. "*doublepointerfunctionArgument = functionArgument2" / "*doublepointerfunctionArgument = functionArgument2->q"
											doublePointerAssignmentDetected = true;
											#ifdef CS_DEBUG
											//cout << "doublePointerAssignmentDetected" << endl;
											#endif
										}

										#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
										//preconditions: argumentNameAliasList has already been filled for all function arguments
										for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunctionTemp->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunctionTemp->argumentNameAliasList.end(); argumentNameAliasListIter++)
										{
											//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
											string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
											string doublePointerAssignmentTextHypothetical = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER) + functionDeclarationArgument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + functionArgumentSecondaryAssignmentName;	//*doublePointerfunctionArgument = secondaryAssignment
											int doublePointerAssignmentTextHypotheticalIndex = functionText->find(doublePointerAssignmentTextHypothetical, indexOfStartOfLine);
											if((doublePointerAssignmentTextHypotheticalIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (doublePointerAssignmentTextHypotheticalIndex < indexOfEndOfLine))
											{
												//e.g. "*doublepointerfunctionArgument = functionArgument2secondaryAssignment" / "*doublepointerfunctionArgument = functionArgument2secondaryAssignment->q"
												doublePointerAssignmentDetected = true;
												#ifdef CS_DEBUG
												//cout << "doublePointerAssignmentDetected" << endl;
												#endif
											}
										}
										#endif

										currentFunctionArgumentInFunctionTemp = currentFunctionArgumentInFunctionTemp->next;
									}
								}

								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
								if(doublePointerAssignmentDetected)
								{
									*isConstEffective = true;
								}
								else
								{
									*isNotConst = true;
								}
								#else
								*isNotConst = true;
								#endif

								/*
								e.g.
								functionArgument = 5;
								*functionArgument = 5;
								functionArgument->parameter = 5;
								*/
							}
						}

						if(indexOfEqualsSetPrevious > indexOfStartOfLine)
						{
							bool inverseNonConstAssignmentDetectedDoublePointer = false;
							bool inverseNonConstAssignmentDetectedAlias = false;
							//int indexOfTextBeforeFunctionArgument = indexOfStartOfLine;
							string textBeforeFunctionArgument = functionText->substr(indexOfStartOfLine, indexOfFunctionArgument-indexOfStartOfLine);
							CSfunctionArgument* currentFunctionArgumentInFunctionTemp = currentFunctionObject->firstFunctionArgumentInFunction;
							while(currentFunctionArgumentInFunctionTemp->next != NULL)
							{
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS
								if(detectDoublePointer(currentFunctionArgumentInFunctionTemp->argument))
								{
									#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS_STRICT
									//modified CS3h13a
									string doublePointerAssignmentTextHypothetical = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER) + currentFunctionArgumentInFunctionTemp->argumentName + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + functionDeclarationArgument;	//*doublePointer = functionArgument
									int doublePointerAssignmentTextHypotheticalIndex = functionText->find(doublePointerAssignmentTextHypothetical, indexOfStartOfLine);
									if((doublePointerAssignmentTextHypotheticalIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (doublePointerAssignmentTextHypotheticalIndex < indexOfEndOfLine))
									{
										//e.g. "*doublepointerargument = functionArgument" / "*doublepointerargument = functionArgument->q"
										inverseNonConstAssignmentDetectedDoublePointer = true;
									}
									#else
									//original version; before CS3h13a
									string doublePointerAssignmentTextHypothetical = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER) + currentFunctionArgumentInFunctionTemp->argumentName;	//*doublePointer
									int textBeforeFunctionArgumentIndex = textBeforeFunctionArgument.find(doublePointerAssignmentTextHypothetical);
									int equalsSetPreviousIndex = textBeforeFunctionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET);
									if(textBeforeFunctionArgumentIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE)
									{
										if(textBeforeFunctionArgumentIndex < equalsSetPreviousIndex)
										{
											//e.g. "*doublepointerargument = functionArgument" / "*doublepointerargument = functionArgument->q"
											inverseNonConstAssignmentDetectedDoublePointer = true;
										}
									}
									#endif
								}
								#endif
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
								//preconditions: argumentNameAliasList has already been filled for all function arguments
								for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunctionTemp->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunctionTemp->argumentNameAliasList.end(); argumentNameAliasListIter++)
								{
									#ifdef CS_DEBUG
									//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
									#endif
									string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
									if(!detectDoublePointer(currentFunctionArgumentInFunctionTemp->argument))	//added 3h13a
									{
										int aliasIndex = textBeforeFunctionArgument.find(functionArgumentSecondaryAssignmentName);
										int equalsSetPreviousIndex = textBeforeFunctionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET);
										if(aliasIndex != CPP_STRING_FIND_RESULT_FAIL_VALUE)
										{
											if(aliasIndex < equalsSetPreviousIndex)
											{
												if(functionArgumentReferenceWholeWordCheck(&textBeforeFunctionArgument, functionArgumentSecondaryAssignmentName, aliasIndex))
												{
													string textBeforeSecondaryReference = textBeforeFunctionArgument.substr(0, aliasIndex);

													#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES_STRICT
													if(verifyThatVariableIsNotDeclarationOrNextIteratorAssignmentOrArrayIndex(currentFunctionObject, functionText, textBeforeSecondaryReference, functionArgumentSecondaryAssignmentName, indexOfStartOfLine+aliasIndex, indexOfSquareBracketOpen, indexOfSquareBracketClose, indexOfEndOfCommand, indexOfStartOfLine, indexOfEqualsSetPrevious))
													{//prevent detection of typeX* nonconstargumentORsecondaryReferenceToNonConstArgument = functionArgument
													#endif
														//e.g. "nonconstargumentORsecondaryReferenceToNonConstArgument[->optionalSubObject] = functionArgument"
														inverseNonConstAssignmentDetectedAlias = true;
													#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES_STRICT
													}
													#endif
												}
											}
										}
									}
								}
								#endif

								currentFunctionArgumentInFunctionTemp = currentFunctionArgumentInFunctionTemp->next;
							}
							if(inverseNonConstAssignmentDetectedDoublePointer)
							{
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
								*isConstEffective = true;
								#else
								*isNotConst = true;
								#endif
								#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
								cout << "checkIfVariableIsBeingModifiedInFunction{}: inverseNonConstAssignmentDetected: isNotConst = true" << endl;
								#endif
							}
							if(inverseNonConstAssignmentDetectedAlias)
							{
								*isNotConst = true;
								#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
								cout << "checkIfVariableIsBeingModifiedInFunction{}: inverseNonConstAssignmentDetected: isNotConst = true" << endl;
								#endif
							}
						}
					}
				}

				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NONCONST_BASED_ON_EXISTENCE_OF_ARBITRARY_SPECIAL_CASE_TEXT
				for(int i=0; i<CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_SPECIAL_CASES_NUMBER_OF_TYPES; i++)
				{
					int indexOfSpecialCaseTextForAssignmentOfNonConst = currentLine.find(specialCaseTextForAssignmentOfNonConst[i]);
					if(indexOfSpecialCaseTextForAssignmentOfNonConst != CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{
						*isNotConst = true;
					}
				}
				#endif


				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_RETURN_OBJECTS
				//now see if can locate the string "returnVar = functionDeclarationArgument;" / "newConnection->entity = functionDeclarationArgument;" in the function
				string returnVarSetTextHypothetical = string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET) + functionDeclarationArgument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;
				int indexOfReturnVarSetTextHypothetical = indexOfFunctionArgument-string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET).length();
				if(functionText->substr(indexOfReturnVarSetTextHypothetical, returnVarSetTextHypothetical.length()) == returnVarSetTextHypothetical)
				{
					int indexOfReturnVar = functionText->rfind(returnVar, indexOfReturnVarSetTextHypothetical);
					if((indexOfReturnVar != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfReturnVar > indexOfStartOfLine))
					{
						if(functionArgumentReferenceWholeWordCheck(functionText, returnVar, indexOfReturnVar))
						{
							*isNotConst = true;
							#ifdef CS_DEBUG
							//cout << "returnVarSetTextHypothetical = " << returnVarSetTextHypothetical << endl;
							#endif
						}
					}
				}
				#endif

				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NON_CONST_GLOBAL_ASSIGNMENTS
				for(int i=0; i<CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_NON_CONST_GLOBAL_ASSIGNMENTS_NUMBER_OF_TYPES; i++)
				{
					string nonConstGlobalAssignmentHypothetical = specialCaseTextForAssignmentOfNonConstGlobal[i] + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_EQUALS_SET + functionDeclarationArgument + CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_END_OF_COMMAND;	//e.g. entityNodesActiveListComplete = newEntityNodesCompleteList;
					if(functionText->find(nonConstGlobalAssignmentHypothetical) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{
						*isNotConst = true;
						#ifdef CS_DEBUG
						//cout << "nonConstGlobalAssignmentHypothetical = " << nonConstGlobalAssignmentHypothetical << endl;
						#endif
					}
				}
				#endif
			}
		}
	}

	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_POINTER_ARRAY_TYPES_NON_CONST
	if(detectPointerArray(currentFunctionArgumentInFunction->argument))
	{
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
		*isConstEffective = true;
		#else
		*isNotConst = true;
		#endif
	}
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_DOUBLE_POINTER_TYPES_NON_CONST
	if(detectDoublePointer(currentFunctionArgumentInFunction->argument))
	{
		#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
		*isConstEffective = true;
		#else
		*isNotConst = true;
		#endif
	}
	#endif

	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_OBJECT_FUNCTION_EXECUTIONS_DETECT_PARAMETERS_FUNCTION_ARGUMENTS_OR_SECONDARY_ASSIGNMENTS_ALTERNATE
	//limitation; doesnt support cases with enclosing brackets, e.g. "if(!(newParseFileObject->get(functionArgumentName))"; but such cases should never be used anyway
	int indexOfNewLine = -1;
	int indexOfEndOfLine = -1;
	while((indexOfNewLine = functionText->find(STRING_NEW_LINE, indexOfNewLine+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		int indexOfStartOfLine = indexOfEndOfLine+1;
		indexOfEndOfLine = indexOfNewLine;

		int i = indexOfStartOfLine;
		bool stillFindingWhiteSpace = true;
		while((i < indexOfEndOfLine) && stillFindingWhiteSpace)
		{
			char c = (*functionText)[i];
			if((c == CHAR_TAB) || (c == CHAR_SPACE))
			{
				i++;
			}
			else
			{
				stillFindingWhiteSpace = false;
			}

		}

		int indexOfVariableName = i;
		string variableName = extractFullVariableName(functionText, indexOfVariableName);
		if(variableName != "")
		{
			int indexOfReferenceToken = indexOfVariableName + variableName.length();
			for(int i=0; i<CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_REFERENCE_NUMBER_OF_TYPES; i++)
			{
				if(functionText->substr(indexOfReferenceToken, codeReference[i].length()) == codeReference[i])
				{
					int indexOfObjectFunctionName = indexOfReferenceToken + codeReference[i].length();
					string objectFunctionName = extractFullVariableName(functionText, indexOfObjectFunctionName);
					if(objectFunctionName != "")
					{
						int indexOfBracketOpen = indexOfObjectFunctionName + objectFunctionName.length();
						if(indexOfBracketOpen < indexOfEndOfLine)
						{
							if((*functionText)[indexOfBracketOpen] == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OPEN_PARAMETER_SPACE)
							{
								CSfunctionArgument* currentFunctionArgumentInFunctionTemp = currentFunctionObject->firstFunctionArgumentInFunction;
								while(currentFunctionArgumentInFunctionTemp->next != NULL)
								{
									//NB this code is redundant if CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES is enabled, since argumentNameAliasList includes the original (primary) functionArgument;
									int indexOfFunctionArgumentTemp = indexOfObjectFunctionName+objectFunctionName.length();	//-1 not required because of '('
									while((indexOfFunctionArgumentTemp = functionText->find(currentFunctionArgumentInFunctionTemp->argumentName, indexOfFunctionArgumentTemp+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
									{
										if((indexOfFunctionArgumentTemp != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfFunctionArgumentTemp < indexOfEndOfLine))
										{
											if(functionArgumentReferenceWholeWordCheck(functionText, currentFunctionArgumentInFunctionTemp->argumentName, indexOfFunctionArgumentTemp))
											{
												//e.g. "entityNodesActiveListCompleteFastIndex->insert(pair<string, GIAentityNode*>(entityNodesTempActiveListCompleteIndex, entityNode));"
												currentFunctionArgumentInFunctionTemp->isNotConst = true;
											}
										}
									}

									#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
									//preconditions: argumentNameAliasList has already been filled for all function arguments
									for(vector<string>::iterator argumentNameAliasListIter = currentFunctionArgumentInFunctionTemp->argumentNameAliasList.begin(); argumentNameAliasListIter < currentFunctionArgumentInFunctionTemp->argumentNameAliasList.end(); argumentNameAliasListIter++)
									{
										#ifdef CS_DEBUG
										//cout << "argumentNameAliasListIter = " << *argumentNameAliasListIter << endl;
										#endif
										string functionArgumentSecondaryAssignmentName = *argumentNameAliasListIter;
										int indexOfFunctionArgumentSecondaryAssignment = indexOfObjectFunctionName+objectFunctionName.length();	//-1 not required because of '('
										while((indexOfFunctionArgumentSecondaryAssignment = functionText->find(functionArgumentSecondaryAssignmentName, indexOfFunctionArgumentSecondaryAssignment+1)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
										{
											if((indexOfFunctionArgumentSecondaryAssignment != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfFunctionArgumentSecondaryAssignment < indexOfEndOfLine))
											{
												if(functionArgumentReferenceWholeWordCheck(functionText, functionArgumentSecondaryAssignmentName, indexOfFunctionArgumentSecondaryAssignment))
												{
													currentFunctionArgumentInFunctionTemp->isNotConst = true;
												}
											}
										}
									}
									#endif

									currentFunctionArgumentInFunctionTemp = currentFunctionArgumentInFunctionTemp->next;
								}
							}
						}
					}
				}
			}
		}
	}
	#endif

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "end checkIfVariableIsBeingModifiedInFunction{}: functionDeclarationArgument = " << functionDeclarationArgument << endl;
	#endif
}

bool verifyThatVariableIsNotDeclarationOrNextIteratorAssignmentOrArrayIndex(CSfunction* currentFunctionObject, string* functionText, string textBeforeFunctionArgument, string functionDeclarationArgument, int indexOfFunctionArgument, int indexOfSquareBracketOpen, int indexOfSquareBracketClose, int indexOfEndOfCommand, int indexOfStartOfLine, int indexOfEqualsSet)
{
	bool  passMiscellaneousConditions = true;

	if(charInString(textBeforeFunctionArgument, functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
	{//added condition CS3h3b - ignore new type initialisations of secondary assignments e.g. "currentReferenceInFunctionReferenceListRepeats" in "CSfunction* currentReferenceInFunctionReferenceListRepeats = function->firstReferenceInFunctionReferenceListRepeats;"
		passMiscellaneousConditions = false;
	}
	else
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
				}
			}
		}

		/*
		//NB this check appears to be redundant based on the method in which new type initialisations of secondary assignments were detected; ie "if(!charInString(textBeforeFunctionArgument, functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))"
		if((indexOfSquareBracketOpen != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (indexOfSquareBracketClose != CPP_STRING_FIND_RESULT_FAIL_VALUE))
		{
			if((indexOfSquareBracketOpen > indexOfStartOfLine) && (indexOfSquareBracketClose < indexOfEqualsSet))
			{
				if((indexOfSquareBracketOpen < indexOfFunctionArgument) && (indexOfSquareBracketClose > indexOfFunctionArgument))
				{
					//e.g. "contrastMap[y*imageWidth + x] = contrastVal;", where functionArgument is imageWidth
					//not "GIAentityNodeArrayFilled[relationIndex[i]] = true;", where functionArgument is GIAentityNodeArrayFilled
					passMiscellaneousConditions = false;
				}
			}
		}
		*/
	}
	return passMiscellaneousConditions;
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

string extractFullVariableTypeReverse(string* functionText, int indexOfEndOfVariableType, int* indexOfStartOfVariableType)
{
	string fullVariableType = "";
	int i = indexOfEndOfVariableType;
	bool stillFindingVariableNameCharacters = true;
	int currentNumberTemplateUseLevels = 0;
	while(stillFindingVariableNameCharacters)
	{
		char c = (*functionText)[i];
		bool foundValidChar = false;
		if(currentNumberTemplateUseLevels > 0)
		{
			if(c == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_TEMPLATE_USE_CLOSE)
			{
				currentNumberTemplateUseLevels++;
			}
			else if(c == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_TEMPLATE_USE_OPEN)
			{
				currentNumberTemplateUseLevels--;
			}
			foundValidChar = true;
		}
		else
		{
			for(int j=0; j<CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES; j++)
			{
				if((*functionText)[i] == functionOrVariableNameCharacters[j])
				{
					foundValidChar = true;
				}
			}
			if(c == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_TEMPLATE_USE_CLOSE)
			{
				currentNumberTemplateUseLevels++;
				foundValidChar = true;
			}
		}

		if(foundValidChar)
		{
			string blank = "";
			fullVariableType = blank + c + fullVariableType;
			*indexOfStartOfVariableType = i;
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
	return fullVariableType;
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

//or ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_DOUBLE_POINTER_TYPES_NON_CONST
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

#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_POINTER_ARRAY_TYPES_NON_CONST
bool detectPointerArray(string functionArgument)
{
	bool result = false;
	bool pointerTypeDetected = false;
	if(functionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_POINTER_TYPE) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		pointerTypeDetected = true;
	}
	bool arrayTypeDetected = false;
	if(functionArgument.find(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_ARRAY_TYPE) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		arrayTypeDetected = true;
	}
	if(pointerTypeDetected && arrayTypeDetected)
	{
		result = true;
	}
	return result;
}
#endif

#endif
