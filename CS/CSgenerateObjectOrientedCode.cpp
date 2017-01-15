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
 * File Name: CSgenerateObjectOrientedCode.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3f4a 11-July-2015
 *
 *******************************************************************************/


#include "CSgenerateObjectOrientedCode.h"
#include "CSdraw.h"
#include "SHAREDvars.h"


#ifdef CS_GENERATE_CPP_CLASSES

ReferencedClass::ReferencedClass(void)
{
	className = "";
	next = NULL;
}

bool generateCPPclasses(CSfileReferenceContainer* firstReferenceInTopLevelBelowListContainer)
{
	bool result = true;
	
	if(!generateCPPclassesRecurse(firstReferenceInTopLevelBelowListContainer, firstReferenceInTopLevelBelowListContainer))
	{
		result = false;
	}
	
	return result;	
}

bool generateCPPclassesRecurse(CSfileReferenceContainer* firstReferenceInAboveLevelBelowListContainer, CSfileReferenceContainer* firstReferenceInTopLevelBelowListContainer)	
{
	bool result = true;
	
	CSfileReferenceContainer* currentFileReferenceContainer = firstReferenceInAboveLevelBelowListContainer;
	while(currentFileReferenceContainer->next != NULL)
	{
		CSfileReference* currentFileReference = currentFileReferenceContainer->fileReference;
		
		if(currentFileReference->firstReferenceInBelowListContainer != NULL)
		{
			if(!(currentFileReference->printedOO))
			{
				if(currentFileReference->printed)	//now redundant
				{//only create object oriented code for printed boxes
					currentFileReference->headerFileTextOrig = getFileContents(currentFileReference->name);	//headerFileNameOrig
					currentFileReference->sourceFileTextOrig = getFileContents(currentFileReference->sourceFileNameOrig);
					currentFileReference->headerFileText = currentFileReference->headerFileTextOrig;	//initialise with same source code as original file
					currentFileReference->sourceFileText = currentFileReference->sourceFileTextOrig;	//initialise with same source code as original file

					#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
					//cout << "generateCPPclassesRecurse{}: currentFileReference->sourceFileName = " << currentFileReference->sourceFileName << endl;
					//cout << "generateCPPclassesRecurse{}: currentFileReference->headerFileName = " << currentFileReference->headerFileName << endl;
					#endif
					if(!generateCPPclassesFile(currentFileReference, firstReferenceInTopLevelBelowListContainer))
					{
						result = false;
					}
					currentFileReference->printedOO = true;
				}

				if(currentFileReference->firstReferenceInBelowListContainer != NULL)
				{
					generateCPPclassesRecurse(currentFileReference->firstReferenceInBelowListContainer, firstReferenceInTopLevelBelowListContainer);
				}
			}
		}

		currentFileReferenceContainer = currentFileReferenceContainer->next;
	}
	return result;
}
	
bool generateCPPclassesFile(CSfileReference* currentFileReference, CSfileReferenceContainer* firstReferenceInTopLevelBelowListContainer)
{
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	cout << "generateCPPclassesFile{}: currentFileReference->name = " << currentFileReference->name << endl;	
	#endif
	
	string className = generateClassName(currentFileReference->name);
	ReferencedClass* firstReferencedClassInList = new ReferencedClass();
	ReferencedClass* currentReferencedClassInList = firstReferencedClassInList;
	bool fileHasFunctions = false;
		
	CSfunctionReference* currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
	while(currentFunctionReference->next != NULL)
	{
		//if(currentFunctionReference->printed)
		//{//only create connections to printed boxes (is this required?)
				
			fileHasFunctions = true;

			bool foundStaticReference = false;
			bool foundPublicReference = false;
			isFunctionBeingReferencedPublicallyRecurse(currentFunctionReference->name, currentFileReference->name, firstReferenceInTopLevelBelowListContainer, &foundPublicReference);

			//1. convert function names
			int positionOfFunctionReferenceHeaderOrig = currentFileReference->headerFileTextOrig.find(currentFunctionReference->nameFull);
			int positionOfFunctionReferenceSourceOrig = currentFileReference->sourceFileTextOrig.find(currentFunctionReference->nameFull);
			string classFullFunctionNameSource = convertFunctionNameToClassFunctionNameSource(currentFunctionReference->nameFull, currentFunctionReference->name, className, foundPublicReference, &foundStaticReference);
			string classFullFunctionNameHeader = convertFunctionNameToClassFunctionNameHeader(currentFunctionReference->nameFull, currentFunctionReference->name, className, foundPublicReference, foundStaticReference);	
			currentFunctionReference->headerFunctionNameFullUpdated = classFullFunctionNameHeader;
			currentFunctionReference->sourceFunctionNameFullUpdated = classFullFunctionNameSource;

			bool foundAtLeastOneInstance = false;
			currentFileReference->headerFileText = replaceAllOccurancesOfString(&(currentFileReference->headerFileText), currentFunctionReference->nameFull, classFullFunctionNameHeader, &foundAtLeastOneInstance);
			if(!foundAtLeastOneInstance)
			{
				cout << "generateCPPclassesFile{} error: !foundAtLeastOneInstance of " << currentFunctionReference->nameFull << " in currentFileReference->headerFileText" << endl;
				cout << "currentFileReference->name = " << currentFileReference->name << endl;
				cout << "currentFunctionReference->nameFull = " << currentFunctionReference->nameFull << endl;
				cout << "currentFunctionReference->name = " << currentFunctionReference->name << endl;
				exit(0);
			}
			foundAtLeastOneInstance = false;
			currentFileReference->sourceFileText = replaceAllOccurancesOfString(&(currentFileReference->sourceFileText), currentFunctionReference->nameFull, classFullFunctionNameSource, &foundAtLeastOneInstance);
			if(!foundAtLeastOneInstance)
			{
				cout << "generateCPPclassesFile{} error: !foundAtLeastOneInstance of " << currentFunctionReference->nameFull << " in currentFileReference->sourceFileText" << endl;
				cout << "currentFileReference->name = " << currentFileReference->name << endl;
				cout << "currentFunctionReference->nameFull = " << currentFunctionReference->nameFull << endl;
				cout << "currentFunctionReference->name = " << currentFunctionReference->name << endl;
				exit(0);
			}		

			//2. convert function references
			CSfunctionReference* currentfunctionReferenceReference = currentFunctionReference->firstReferenceInFunctionReferenceList;
			while(currentfunctionReferenceReference->next != NULL)
			{
				string functionReferenceReferenceName = currentfunctionReferenceReference->name;
				string functionReferenceReferenceNameUpdated = "";

				string functionReferenceContext = "";
				CSfileReference* fileReferenceHoldingFunction = NULL;
				CSfunctionReference* referencedFunction = NULL;		//referencedFunction = function pertaining to currentFunctionReference
				bool referencedFunctionFound = findFunctionReferenceWithName(functionReferenceReferenceName, firstReferenceInTopLevelBelowListContainer, &fileReferenceHoldingFunction, &referencedFunction);
				if(referencedFunctionFound)
				{
					string fileNameHoldingFunction = fileReferenceHoldingFunction->name;
					string classHoldingFunction = generateClassName(fileNameHoldingFunction);

					//add context
					if(foundStaticReference)
					{
						//create a new temporary object pertaining to the functionReferenceReference's class
						functionReferenceReferenceNameUpdated = functionReferenceReferenceNameUpdated + CHAR_OPEN_BRACKET + CS_GENERATE_CPP_CLASSES_NEW + CHAR_SPACE + generateClassDeclarationName(classHoldingFunction) + CHAR_CLOSE_BRACKET + CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_DELIMITER_POINTER + functionReferenceReferenceName;	//eg (new chickenClass)->function
					}
					else
					{
						if(fileNameHoldingFunction == currentFileReference->name)
						{
							functionReferenceReferenceNameUpdated = string(CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_LOCAL) + string(CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_DELIMITER_POINTER) + functionReferenceReferenceName;
						}
						else
						{
							functionReferenceReferenceNameUpdated = generateClassObjectName(classHoldingFunction) + CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_DELIMITER + functionReferenceReferenceName;

							//cout << "classHoldingFunction = " << classHoldingFunction << endl;
							bool foundReferencedClass = findReferencedClassInList(firstReferencedClassInList, classHoldingFunction);
							if(!foundReferencedClass)
							{
								currentReferencedClassInList->className = classHoldingFunction;
								currentReferencedClassInList->next = new ReferencedClass();
								currentReferencedClassInList = currentReferencedClassInList->next;
							}
						}
					}
					#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
					//cout << "functionReferenceReferenceName = " << functionReferenceReferenceName << endl;
					//cout << "functionReferenceReferenceNameUpdated = " << functionReferenceReferenceNameUpdated << endl;
					#endif

					//must search and replace a more specific "functionName(" instead of "functionName" incase there exists a variable called functionNameSomeArbitraryVariable
					functionReferenceReferenceName = functionReferenceReferenceName + CHAR_OPEN_BRACKET;
					functionReferenceReferenceNameUpdated = functionReferenceReferenceNameUpdated + CHAR_OPEN_BRACKET;					 

					int posOfFunctionRefUpdated = currentFunctionReference->functionTextRaw.find(functionReferenceReferenceNameUpdated);
					if(posOfFunctionRefUpdated == CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{//only update function reference within functionText once

						int posOfFunctionText = currentFileReference->sourceFileText.find(currentFunctionReference->functionTextRaw);
						int functionTextOrigLength = currentFunctionReference->functionTextRaw.length();
						if(posOfFunctionText != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							foundAtLeastOneInstance = false;
							currentFunctionReference->functionTextRaw = replaceAllOccurancesOfFunctionReferenceReferenceNameInFunction(&(currentFunctionReference->functionTextRaw), functionReferenceReferenceName, functionReferenceReferenceNameUpdated, &foundAtLeastOneInstance);
							if(!foundAtLeastOneInstance)
							{
								cout << "generateCPPclassesFile{} error: !foundAtLeastOneInstance of " << functionReferenceReferenceName << " in currentFunctionReference->functionTextRaw" << endl;
								cout << "currentFileReference->name = " << currentFileReference->name << endl;
								cout << "currentFunctionReference->nameFull = " << currentFunctionReference->nameFull << endl;
								cout << "currentFunctionReference->name = " << currentFunctionReference->name << endl;
								exit(0);
							}
							currentFileReference->sourceFileText = currentFileReference->sourceFileText.substr(0, posOfFunctionText) + currentFunctionReference->functionTextRaw + currentFileReference->sourceFileText.substr(posOfFunctionText+functionTextOrigLength, currentFileReference->sourceFileText.length()-posOfFunctionText+functionTextOrigLength);

							//cout << "currentFunctionReference->functionTextRaw = " << currentFunctionReference->functionTextRaw << endl;							
							//exit(0);
						}
						else
						{
							cout << "generateCPPclassesFile{} error: currentFunctionReference->functionTextRaw not found in currentFileReference->sourceFileText" << endl;
							cout << "currentFunctionReference->functionTextRaw = " <<  currentFunctionReference->functionTextRaw << endl;
							exit(0); 
						}
					}
				}
				else
				{
					cout << "generateCPPclassesFile{} error: !referencedFunctionFound, functionReferenceReferenceName = " << functionReferenceReferenceName << endl;
					cout << "currentFileReference->name = " << currentFileReference->name << endl;
					cout << "currentFunctionReference->nameFull = " << currentFunctionReference->nameFull << endl;
					cout << "currentFunctionReference->name = " << currentFunctionReference->name << endl;
					exit(0);
				}

				#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
				//cout << "generateCPPclassesFile{}: currentFunctionReference->name = " << currentFunctionReference->name << endl;	
				//cout << "generateCPPclassesFile{}: currentFunctionReference->nameFull = " << currentFunctionReference->nameFull << endl;
				#endif
				currentfunctionReferenceReference = currentfunctionReferenceReference->next;
			//}
		}
		currentFunctionReference = currentFunctionReference->next;	
	}
	
	if(fileHasFunctions)
	{
	
		//3. add class wrapper header to class function declarations in class header file
		//4. add referenced classes to source file
		//find location of first function in source
		string firstFunctionNameFull = currentFileReference->firstReferenceInFunctionList->headerFunctionNameFullUpdated;
		int positionOfFirstFunctionReferenceInHeader = currentFileReference->headerFileText.find(firstFunctionNameFull);
		if(positionOfFirstFunctionReferenceInHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string classDeclarationHeader = string(CS_GENERATE_CPP_CLASSES_CLASS_HEADER_PART1) + generateClassDeclarationName(className) + string(CS_GENERATE_CPP_CLASSES_CLASS_HEADER_PART2);	//class xClass{
			string referencedClassesDeclarations = generateReferencedClassesDeclarations(firstReferencedClassInList);	//ie private: xClass x; private: yClass y;

			currentFileReference->headerFileText = currentFileReference->headerFileText.substr(0, positionOfFirstFunctionReferenceInHeader) + classDeclarationHeader + referencedClassesDeclarations + currentFileReference->headerFileText.substr(positionOfFirstFunctionReferenceInHeader, currentFileReference->headerFileText.length()-positionOfFirstFunctionReferenceInHeader);
		}
		else
		{
			cout << "error: (positionOfFirstFunctionReferenceInHeader == CPP_STRING_FIND_RESULT_FAIL_VALUE" << endl;
			exit(0);
		}


		//5. add class wrapper footer to class function declarations in class header file
		//find location of last function reference in header	
		currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
		CSfunctionReference* lastFunctionReference = currentFunctionReference;
		while(currentFunctionReference->next != NULL)
		{
			lastFunctionReference = currentFunctionReference;
			currentFunctionReference = currentFunctionReference->next;
		}
		string lastFunctionNameFull = lastFunctionReference->headerFunctionNameFullUpdated;
		int positionOfLastFunctionReferenceInHeader = currentFileReference->headerFileText.find(lastFunctionNameFull);
		if(positionOfFirstFunctionReferenceInHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string classDeclarationFooter = CS_GENERATE_CPP_CLASSES_CLASS_FOOTER;	//};
			int lastFunctionNameFullLength = (lastFunctionReference->headerFunctionNameFullUpdated).length();
			int positionOfLastFunctionReferenceInHeaderEnd = positionOfLastFunctionReferenceInHeader + lastFunctionNameFullLength;
			currentFileReference->headerFileText = currentFileReference->headerFileText.substr(0, positionOfLastFunctionReferenceInHeaderEnd+1) + classDeclarationFooter + currentFileReference->headerFileText.substr(positionOfLastFunctionReferenceInHeaderEnd+1, currentFileReference->headerFileText.length()-positionOfLastFunctionReferenceInHeaderEnd);	//+1 for trailing CHAR_SEMICOLON
		}
		else
		{
			cout << "error: (positionOfFirstFunctionReferenceInHeader == CPP_STRING_FIND_RESULT_FAIL_VALUE" << endl;
			exit(0);
		}

		//6. add move source include file statements to header (required for referenced class declarations)
		if(!moveIncludeFileStatementsToHeader(currentFileReference))
		{
			result = false;
		}	
	}

	#ifndef CS_GENERATE_CPP_CLASSES_DISABLE_OUTPUT
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	cout << "currentFileReference->headerFileText = \n" << currentFileReference->headerFileText << endl;
	cout << "currentFileReference->sourceFileText = \n" << currentFileReference->sourceFileText << endl;	
	#endif
	writeStringToFile(currentFileReference->headerFileName, &(currentFileReference->headerFileText));
	writeStringToFile(currentFileReference->sourceFileName, &(currentFileReference->sourceFileText));
	#else
	cout << "currentFileReference->headerFileText = \n" << currentFileReference->headerFileText << endl;
	cout << "currentFileReference->sourceFileText = \n" << currentFileReference->sourceFileText << endl;	
	#endif
	
	return result;
}

string replaceAllOccurancesOfFunctionReferenceReferenceNameInFunction(string* functionTextOrig, string functionReferenceReferenceName, string functionReferenceReferenceNameUpdated, bool* foundAtLeastOneInstance)
{
	string functionTextUpdated = *functionTextOrig;
	
	bool stillSearching = true;
	int pos = 0;
	while(stillSearching)
	{
		pos = functionTextUpdated.find(functionReferenceReferenceName, pos);
		if(pos != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{//only update function reference within functionText once

			//added condition CS 3f1b - ensure previous character is not a letter (this ensures that ABCfunctionName is not found when searching for functionName)
			if((pos == 0) || !charInCharArray(functionTextUpdated[pos-1], functionNameCharacters, CS_FUNCTION_NAME_CHARACTERS_NUMBER_OF_TYPES))
			{	
				functionTextUpdated.replace(pos, functionReferenceReferenceName.length(), functionReferenceReferenceNameUpdated);
				pos = pos + functionReferenceReferenceNameUpdated.length();
				*foundAtLeastOneInstance = true;
			}
			else
			{
				pos = pos + functionReferenceReferenceName.length();
			}
		}
		else
		{
			stillSearching = false;
		}
	}
	
	return functionTextUpdated;
}


void isFunctionBeingReferencedPublicallyRecurse(string functionName, string fileName, CSfileReferenceContainer* firstReferenceInAboveLevelBelowListContainer, bool* foundPublicReference)
{
	CSfileReferenceContainer* currentFileReferenceContainer = firstReferenceInAboveLevelBelowListContainer;

	while(currentFileReferenceContainer->next != NULL)
	{
		CSfileReference* currentFileReference = currentFileReferenceContainer->fileReference;

		if(currentFileReference->name != fileName)
		{//search for external references to function only

			CSfunctionReference* currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
			while(currentFunctionReference->next != NULL)
			{
				CSfunctionReference* currentfunctionReferenceReference = currentFunctionReference->firstReferenceInFunctionReferenceList;
				while(currentfunctionReferenceReference->next != NULL)
				{
					if(currentfunctionReferenceReference->name == functionName)
					{
						*foundPublicReference = true;
						#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
						//cout << "foundPublicReference" << endl;
						#endif
					}
					currentfunctionReferenceReference = currentfunctionReferenceReference->next;
				}
				
				currentFunctionReference = currentFunctionReference->next;
			}

			if(currentFileReference->firstReferenceInBelowListContainer != NULL)
			{
				isFunctionBeingReferencedPublicallyRecurse(functionName, fileName, currentFileReference->firstReferenceInBelowListContainer, foundPublicReference);
			}
		}
			
		currentFileReferenceContainer = currentFileReferenceContainer->next;
	}

}

string generateClassName(string headerFileName)
{
	string className = "";
	int positionOfExtension = headerFileName.rfind(CHAR_FULLSTOP);
	if(positionOfExtension != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		className = headerFileName.substr(0, positionOfExtension);
		//cout << "className = " << className << endl;
	}
	return className;
}

string generateClassDeclarationName(string className)
{
	return className + CS_GENERATE_CPP_CLASSES_CLASS_DECLARATION_APPENDITION;
}

string generateClassObjectName(string className)	
{
	return className + CS_GENERATE_CPP_CLASSES_CLASS_OBJECT_APPENDITION;
}


string convertFunctionNameToClassFunctionNameHeader(string fullFunctionName, string functionName, string className, bool foundPublicReference, bool foundStaticReference)
{
	string classFullFunctionName = "";

	string permissionsString = "";
	if(foundStaticReference)
	{
		permissionsString = "";	//static functions cannot be part of class - they must be manually moved out of header class definition once oo code is generated 
	}
	else
	{
		if(foundPublicReference)
		{
			permissionsString = string(CS_GENERATE_CPP_CLASSES_FUNCTION_PUBLIC);
		}
		else
		{
			permissionsString = string(CS_GENERATE_CPP_CLASSES_FUNCTION_PRIVATE);
		}
	}
	
	classFullFunctionName = STRING_TAB + permissionsString + fullFunctionName;
	
	return classFullFunctionName;
	
	
}

string convertFunctionNameToClassFunctionNameSource(string fullFunctionName, string functionName, string className, bool foundPublicReference, bool* foundStaticReference)
{
	string classFullFunctionName = "";
	
	string fullFunctionNamePart1 = "";
	string fullFunctionNamePart2 = ""; 
	int positionOfFunctionName = fullFunctionName.find(functionName);
	
	if(positionOfFunctionName != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		fullFunctionNamePart1 = fullFunctionName.substr(0, positionOfFunctionName);
		fullFunctionNamePart2 = fullFunctionName.substr(positionOfFunctionName, fullFunctionName.length());

		//detect static functions and do not add context to these - added 3e2b
		int positionOfStaticIdentifier = fullFunctionNamePart1.find(CS_GENERATE_CPP_CLASSES_STATIC);
		if((positionOfStaticIdentifier != CPP_STRING_FIND_RESULT_FAIL_VALUE) || (functionName == CS_GENERATE_CPP_CLASSES_FUNCTION_MAIN_NAME))
		{
			classFullFunctionName = fullFunctionName;	//do not add class context to static functions	//do not add class context to main functions
			*foundStaticReference = true;
		}
		else
		{
			classFullFunctionName = fullFunctionNamePart1 + generateClassDeclarationName(className) + string(CS_GENERATE_CPP_CLASSES_CLASS_PERMISSIONS_IDENTIFIER) + fullFunctionNamePart2;
		}
		
		#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
		//cout << "fullFunctionNamePart1 = " << fullFunctionNamePart1 << endl;
		//cout << "fullFunctionNamePart2 = " << fullFunctionNamePart2 << endl;
		#endif
	}
		
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	//cout << "classFullFunctionName = " << classFullFunctionName << endl;
	#endif
	
	return classFullFunctionName;
		
}

bool findReferencedClassInList(ReferencedClass* firstReferencedClassInList, string classNameToFind)
{
	bool foundReferencedClass = false;
	
	ReferencedClass* currentReferencedClassInList = firstReferencedClassInList;
	while(currentReferencedClassInList->next != NULL)
	{
		if(currentReferencedClassInList->className == classNameToFind)
		{
			foundReferencedClass = true;
		}
		currentReferencedClassInList = currentReferencedClassInList->next;
	}
	return foundReferencedClass;
}

string generateReferencedClassesDeclarations(ReferencedClass* firstReferencedClassInList)
{
	string referencedClassesDeclarations = "";
	ReferencedClass* currentReferencedClassInList = firstReferencedClassInList;
	while(currentReferencedClassInList->next != NULL)
	{
		referencedClassesDeclarations = referencedClassesDeclarations + CHAR_TAB + CS_GENERATE_CPP_CLASSES_FUNCTION_PRIVATE + generateClassDeclarationName(currentReferencedClassInList->className) + CHAR_SPACE + generateClassObjectName(currentReferencedClassInList->className) + CHAR_SEMICOLON + CHAR_NEWLINE; 	//ie private: xClass x;
		//cout << "referencedClassesDeclarations = " << referencedClassesDeclarations << endl;
		currentReferencedClassInList = currentReferencedClassInList->next;
	}
	return referencedClassesDeclarations;
}

bool moveIncludeFileStatementsToHeader(CSfileReference* firstReferenceInAboveLevelBelowList)	
{
	bool result = true;
	string includeStatementsHeaderNew = "";
	
	CSfileReferenceContainer* currentFileReferenceContainer = firstReferenceInAboveLevelBelowList->firstReferenceInBelowListContainer;
	int positionOfLastIncludeStatementEndInHeader = CPP_STRING_FIND_RESULT_FAIL_VALUE;
	int lineOfFirstIncludeStatementEndInSource = REALLY_LARGE_INT;
	while(currentFileReferenceContainer->next != NULL)
	{
		CSfileReference* currentFileReference = currentFileReferenceContainer->fileReference;
		
		//if(currentFileReference->name != firstReferenceInAboveLevelBelowList->name){	//do not modify include .h of .c file - not required because of "if(hashIncludeFileName != topLevelReference->name)	//this is added so that do not parse method.h from within method.cpp!"
		string includeStatement = string(CS_GENERATE_CPP_CLASSES_INCLUDE_START) + currentFileReference->name + CS_GENERATE_CPP_CLASSES_INCLUDE_END;
		
		int positionOfIncludeStatementHeader = firstReferenceInAboveLevelBelowList->headerFileText.find(includeStatement);
		int positionOfIncludeStatementSource = firstReferenceInAboveLevelBelowList->sourceFileText.find(includeStatement);
		bool foundIncludeFileInHeader = false;
		bool foundIncludeFileInSource = false;
		string includeStatementFullLineHeader = "";
		string includeStatementFullLineSource = "";
		
		int posStartOfLineHeader = CPP_STRING_FIND_RESULT_FAIL_VALUE;
		int posEndOfLineHeader = CPP_STRING_FIND_RESULT_FAIL_VALUE;		
		if(positionOfIncludeStatementHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			//cout << "foundIncludeFileInHeader" << endl;
			
			foundIncludeFileInHeader = true;
			posStartOfLineHeader = firstReferenceInAboveLevelBelowList->headerFileText.rfind(CHAR_NEWLINE, positionOfIncludeStatementHeader);
			posEndOfLineHeader = firstReferenceInAboveLevelBelowList->headerFileText.find(CHAR_NEWLINE, positionOfIncludeStatementHeader);
			if((posStartOfLineHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (posEndOfLineHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE))
			{
				includeStatementFullLineHeader = firstReferenceInAboveLevelBelowList->headerFileText.substr(posStartOfLineHeader, posEndOfLineHeader-posStartOfLineHeader);
				//cout << "includeStatementFullLineHeader = " << includeStatementFullLineHeader << endl;
				//exit(0);
				
				if(posEndOfLineHeader > positionOfLastIncludeStatementEndInHeader)
				{
					positionOfLastIncludeStatementEndInHeader = posEndOfLineHeader;
				}
			}	
			else
			{
				cout << "moveIncludeFileStatementsToHeader{} error: ((posStartOfLineHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (posEndOfLineHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE))" << endl;
			}	
		}
		int posStartOfLineSource = CPP_STRING_FIND_RESULT_FAIL_VALUE;
		int posEndOfLineSource = CPP_STRING_FIND_RESULT_FAIL_VALUE;
		if(positionOfIncludeStatementSource != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			//cout << "foundIncludeFileInSource" << endl;
					
			foundIncludeFileInSource = true;
			posStartOfLineSource = firstReferenceInAboveLevelBelowList->sourceFileText.rfind(CHAR_NEWLINE, positionOfIncludeStatementSource);
			posEndOfLineSource = firstReferenceInAboveLevelBelowList->sourceFileText.find(CHAR_NEWLINE, positionOfIncludeStatementSource);
			if((posStartOfLineSource != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (posEndOfLineSource != CPP_STRING_FIND_RESULT_FAIL_VALUE))
			{
				includeStatementFullLineSource = firstReferenceInAboveLevelBelowList->sourceFileText.substr(posStartOfLineSource, posEndOfLineSource-posStartOfLineSource);
				//cout << "includeStatementFullLineSource = " << includeStatementFullLineSource << endl;
				//exit(0);
				
				int lineStartOfLineSource = 0;
				for(int i=0; i<posStartOfLineSource; i++)
				{
					if(firstReferenceInAboveLevelBelowList->sourceFileText[i] == CHAR_NEWLINE)
					{
						lineStartOfLineSource++;
					}
				}
				if(lineStartOfLineSource < lineOfFirstIncludeStatementEndInSource)
				{
					lineOfFirstIncludeStatementEndInSource = lineStartOfLineSource;
				}				
			}	
			else
			{
				cout << "moveIncludeFileStatementsToSource{} error: ((posStartOfLineSource != CPP_STRING_FIND_RESULT_FAIL_VALUE) && (posEndOfLineSource != CPP_STRING_FIND_RESULT_FAIL_VALUE))" << endl;
			}		
		}
		
		if(foundIncludeFileInSource)
		{
			//cout << "foundIncludeFileInSource = " << foundIncludeFileInSource << endl;
			//remove from source;
			firstReferenceInAboveLevelBelowList->sourceFileText = firstReferenceInAboveLevelBelowList->sourceFileText.substr(0, posStartOfLineSource) + firstReferenceInAboveLevelBelowList->sourceFileText.substr(posEndOfLineSource, firstReferenceInAboveLevelBelowList->sourceFileText.length()-posEndOfLineSource);
			if(!foundIncludeFileInHeader)
			{
				includeStatementsHeaderNew = includeStatementsHeaderNew + includeStatementFullLineSource;
			}
		}
				
		currentFileReferenceContainer = currentFileReferenceContainer->next;
	}
	
	if(includeStatementsHeaderNew != "")
	{
		includeStatementsHeaderNew + includeStatementsHeaderNew + CHAR_NEWLINE;
		//cout << "includeStatementsHeaderNew = " << endl;
		
		int positionInHeader = 0;
		if(positionOfLastIncludeStatementEndInHeader == CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			//no include file statements found in header - take the line of the first include file statment in source instead (as this should coincide with the appropriate place for include files in the header also, assuming they are formatted the same)
			int lineHeader = 0;
			bool stillFindingLines = true;
			while((lineHeader < lineOfFirstIncludeStatementEndInSource+1) && stillFindingLines)
			{
				if(positionInHeader < firstReferenceInAboveLevelBelowList->headerFileText.length())
				{
					if(firstReferenceInAboveLevelBelowList->headerFileText[positionInHeader] == CHAR_NEWLINE)
					{
						lineHeader++;
					}
				}
				else
				{
					stillFindingLines = false;
				}
				positionInHeader++;
			}
		}
		else
		{
			positionInHeader = positionOfLastIncludeStatementEndInHeader;
		}
		firstReferenceInAboveLevelBelowList->headerFileText = firstReferenceInAboveLevelBelowList->headerFileText.substr(0, positionInHeader) + includeStatementsHeaderNew + firstReferenceInAboveLevelBelowList->headerFileText.substr(positionInHeader, includeStatementsHeaderNew.length()-positionInHeader);
		
		//cout << "firstReferenceInAboveLevelBelowList->headerFileText = " << firstReferenceInAboveLevelBelowList->headerFileText << endl;
		//exit(0);
	}
	
	return result;
}


bool findFunctionReferenceWithName(string name, CSfileReferenceContainer* firstReferenceInAboveLevelBelowListContainer, CSfileReference** fileReferenceHoldingFunction, CSfunctionReference** updatedFunctionReference)
{
	bool foundPrintedReferenceWithName = false;
	
	CSfileReferenceContainer* currentFileReferenceContainer = firstReferenceInAboveLevelBelowListContainer;

	while(currentFileReferenceContainer->next != NULL)
	{
		CSfileReference* currentFileReference = currentFileReferenceContainer->fileReference;
		
		CSfunctionReference* currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
		while(currentFunctionReference->next != NULL)
		{
			if(currentFunctionReference->name == name)
			{
				*updatedFunctionReference = currentFunctionReference;
				foundPrintedReferenceWithName = true;
				*fileReferenceHoldingFunction = currentFileReference;
			}
			currentFunctionReference = currentFunctionReference->next;
		}

		if(currentFileReference->firstReferenceInBelowListContainer != NULL)
		{
			if(findFunctionReferenceWithName(name, currentFileReference->firstReferenceInBelowListContainer, fileReferenceHoldingFunction, updatedFunctionReference))
			{
				foundPrintedReferenceWithName = true;
			}
		}

		currentFileReferenceContainer = currentFileReferenceContainer->next;
	}

	return foundPrintedReferenceWithName;
}


#endif
