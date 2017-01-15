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
 * File Name: CSgenerateObjectOrientedCode.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h6a 06-December-2015
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

bool generateCPPclasses(CSfileContainer* firstObjectInTopLevelBelowListContainer)
{
	bool result = true;
	
	if(!generateCPPclassesRecurse(firstObjectInTopLevelBelowListContainer, firstObjectInTopLevelBelowListContainer))
	{
		result = false;
	}
	
	return result;	
}

bool generateCPPclassesRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer)	
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
				{//only create object oriented code for printed boxes
					currentFileObject->headerFileTextOrig = getFileContents(currentFileObject->name);	//headerFileNameOrig
					currentFileObject->sourceFileTextOrig = getFileContents(currentFileObject->sourceFileNameOrig);
					currentFileObject->headerFileText = currentFileObject->headerFileTextOrig;	//initialise with same source code as original file
					currentFileObject->sourceFileText = currentFileObject->sourceFileTextOrig;	//initialise with same source code as original file

					#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
					//cout << "generateCPPclassesRecurse{}: currentFileObject->sourceFileName = " << currentFileObject->sourceFileName << endl;
					//cout << "generateCPPclassesRecurse{}: currentFileObject->headerFileName = " << currentFileObject->headerFileName << endl;
					#endif
					if(!generateCPPclassesFile(currentFileObject, firstObjectInTopLevelBelowListContainer))
					{
						result = false;
					}
					currentFileObject->printedCodeGeneric = true;
				}

				if(currentFileObject->firstFileInBelowListContainer != NULL)
				{
					generateCPPclassesRecurse(currentFileObject->firstFileInBelowListContainer, firstObjectInTopLevelBelowListContainer);
				}
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}
	return result;
}
	
bool generateCPPclassesFile(CSfile* currentFileObject, CSfileContainer* firstObjectInTopLevelBelowListContainer)
{
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	cout << "generateCPPclassesFile{}: currentFileObject->name = " << currentFileObject->name << endl;	
	#endif
	
	string className = generateClassName(currentFileObject->name);
	ReferencedClass* firstReferencedClassInList = new ReferencedClass();
	ReferencedClass* currentReferencedClassInList = firstReferencedClassInList;
	bool fileHasFunctions = false;
		
	CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
	while(currentFunctionObject->next != NULL)
	{
		//if(currentFunctionObject->printed)
		//{//only create connections to printed boxes (is this required?)
				
			fileHasFunctions = true;

			bool foundStaticReference = false;
			bool foundPublicReference = false;
			isFunctionBeingReferencedPublicallyRecurse(currentFunctionObject->name, currentFileObject->name, firstObjectInTopLevelBelowListContainer, &foundPublicReference);

			//1. convert function names
			int positionOfFunctionObjectHeaderOrig = currentFileObject->headerFileTextOrig.find(currentFunctionObject->nameFull);
			int positionOfFunctionObjectSourceOrig = currentFileObject->sourceFileTextOrig.find(currentFunctionObject->nameFull);
			string classFullFunctionNameSource = convertFunctionNameToClassFunctionNameSource(currentFunctionObject->nameFull, currentFunctionObject->name, className, foundPublicReference, &foundStaticReference);
			string classFullFunctionNameHeader = convertFunctionNameToClassFunctionNameHeader(currentFunctionObject->nameFull, currentFunctionObject->name, className, foundPublicReference, foundStaticReference);	
			currentFunctionObject->headerFunctionNameFullUpdated = classFullFunctionNameHeader;
			currentFunctionObject->sourceFunctionNameFullUpdated = classFullFunctionNameSource;

			bool foundAtLeastOneInstance = false;
			currentFileObject->headerFileText = replaceAllOccurancesOfString(&(currentFileObject->headerFileText), currentFunctionObject->nameFull, classFullFunctionNameHeader, &foundAtLeastOneInstance);
			if(!foundAtLeastOneInstance)
			{
				cout << "generateCPPclassesFile{} error: !foundAtLeastOneInstance of " << currentFunctionObject->nameFull << " in currentFileObject->headerFileText" << endl;
				cout << "currentFileObject->name = " << currentFileObject->name << endl;
				cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
				cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
				exit(0);
			}
			foundAtLeastOneInstance = false;
			currentFileObject->sourceFileText = replaceAllOccurancesOfString(&(currentFileObject->sourceFileText), currentFunctionObject->nameFull, classFullFunctionNameSource, &foundAtLeastOneInstance);
			if(!foundAtLeastOneInstance)
			{
				cout << "generateCPPclassesFile{} error: !foundAtLeastOneInstance of " << currentFunctionObject->nameFull << " in currentFileObject->sourceFileText" << endl;
				cout << "currentFileObject->name = " << currentFileObject->name << endl;
				cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
				cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
				exit(0);
			}		

			//2. convert function references
			CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceList;
			while(currentFunctionReference->next != NULL)
			{
				string functionReferenceName = currentFunctionReference->name;
				string functionReferenceNameUpdated = "";

				string functionReferenceContext = "";
				CSfile* fileObjectHoldingFunction = currentFunctionReference->functionReferenceTargetFileOwner;
				CSfunction* referencedFunction = currentFunctionReference->functionReferenceTarget;
				if(referencedFunction != NULL)
				{
					string fileNameHoldingFunction = fileObjectHoldingFunction->name;
					string classHoldingFunction = generateClassName(fileNameHoldingFunction);

					//add context
					if(foundStaticReference)
					{
						//create a new temporary object pertaining to the functionReference's class
						functionReferenceNameUpdated = functionReferenceNameUpdated + CHAR_OPEN_BRACKET + CS_GENERATE_CPP_CLASSES_TEXT_NEW + CHAR_SPACE + generateClassDeclarationName(classHoldingFunction) + CHAR_CLOSE_BRACKET + CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_DELIMITER_POINTER + functionReferenceName;	//eg (new chickenClass)->function
					}
					else
					{
						if(fileNameHoldingFunction == currentFileObject->name)
						{
							functionReferenceNameUpdated = string(CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_LOCAL) + string(CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_DELIMITER_POINTER) + functionReferenceName;
						}
						else
						{
							functionReferenceNameUpdated = generateClassObjectName(classHoldingFunction) + CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_REFERENCE_CONTEXT_DELIMITER + functionReferenceName;

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
					//cout << "functionReferenceName = " << functionReferenceName << endl;
					//cout << "functionReferenceNameUpdated = " << functionReferenceNameUpdated << endl;
					#endif

					//must search and replace a more specific "functionName(" instead of "functionName" incase there exists a variable called functionNameSomeArbitraryVariable
					functionReferenceName = functionReferenceName + CHAR_OPEN_BRACKET;
					functionReferenceNameUpdated = functionReferenceNameUpdated + CHAR_OPEN_BRACKET;					 

					int posOfFunctionRefUpdated = currentFunctionObject->functionTextRaw.find(functionReferenceNameUpdated);
					if(posOfFunctionRefUpdated == CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{//only update function reference within functionText once

						int posOfFunctionText = currentFileObject->sourceFileText.find(currentFunctionObject->functionTextRaw);
						int functionTextOrigLength = currentFunctionObject->functionTextRaw.length();
						if(posOfFunctionText != CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{
							foundAtLeastOneInstance = false;
							currentFunctionObject->functionTextRaw = replaceAllOccurancesOfFunctionObjectReferenceNameInFunction(&(currentFunctionObject->functionTextRaw), functionReferenceName, functionReferenceNameUpdated, &foundAtLeastOneInstance);
							if(!foundAtLeastOneInstance)
							{
								cout << "generateCPPclassesFile{} error: !foundAtLeastOneInstance of " << functionReferenceName << " in currentFunctionObject->functionTextRaw" << endl;
								cout << "currentFileObject->name = " << currentFileObject->name << endl;
								cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
								cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
								exit(0);
							}
							currentFileObject->sourceFileText = currentFileObject->sourceFileText.substr(0, posOfFunctionText) + currentFunctionObject->functionTextRaw + currentFileObject->sourceFileText.substr((posOfFunctionText+functionTextOrigLength), currentFileObject->sourceFileText.length()-(posOfFunctionText+functionTextOrigLength));

							//cout << "currentFunctionObject->functionTextRaw = " << currentFunctionObject->functionTextRaw << endl;							
							//exit(0);
						}
						else
						{
							cout << "generateCPPclassesFile{} error: currentFunctionObject->functionTextRaw not found in currentFileObject->sourceFileText" << endl;
							cout << "currentFunctionObject->functionTextRaw = " <<  currentFunctionObject->functionTextRaw << endl;
							exit(0); 
						}
					}
				}
				else
				{
					cout << "generateCPPclassesFile{} error: !referencedFunctionFound, functionReferenceName = " << functionReferenceName << endl;
					cout << "currentFileObject->name = " << currentFileObject->name << endl;
					cout << "currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
					cout << "currentFunctionObject->name = " << currentFunctionObject->name << endl;
					exit(0);
				}

				#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
				//cout << "generateCPPclassesFile{}: currentFunctionObject->name = " << currentFunctionObject->name << endl;	
				//cout << "generateCPPclassesFile{}: currentFunctionObject->nameFull = " << currentFunctionObject->nameFull << endl;
				#endif
				currentFunctionReference = currentFunctionReference->next;
			//}
		}
		currentFunctionObject = currentFunctionObject->next;	
	}
	
	if(fileHasFunctions)
	{
	
		//3. add class wrapper header to class function declarations in class header file
		//4. add referenced classes to source file
		//find location of first function in source
		string firstFunctionNameFull = currentFileObject->firstFunctionInFunctionList->headerFunctionNameFullUpdated;
		int positionOfFirstFunctionObjectInHeader = currentFileObject->headerFileText.find(firstFunctionNameFull);
		if(positionOfFirstFunctionObjectInHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string classDeclarationHeader = string(CS_GENERATE_CPP_CLASSES_TEXT_CLASS_HEADER_PART1) + generateClassDeclarationName(className) + string(CS_GENERATE_CPP_CLASSES_TEXT_CLASS_HEADER_PART2);	//class xClass{
			string referencedClassesDeclarations = generateReferencedClassesDeclarations(firstReferencedClassInList);	//ie private: xClass x; private: yClass y;

			currentFileObject->headerFileText = currentFileObject->headerFileText.substr(0, positionOfFirstFunctionObjectInHeader) + classDeclarationHeader + referencedClassesDeclarations + currentFileObject->headerFileText.substr(positionOfFirstFunctionObjectInHeader, currentFileObject->headerFileText.length()-positionOfFirstFunctionObjectInHeader);
		}
		else
		{
			cout << "error: (positionOfFirstFunctionObjectInHeader == CPP_STRING_FIND_RESULT_FAIL_VALUE" << endl;
			exit(0);
		}


		//5. add class wrapper footer to class function declarations in class header file
		//find location of last function reference in header	
		currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
		CSfunction* lastFunctionObject = currentFunctionObject;
		while(currentFunctionObject->next != NULL)
		{
			lastFunctionObject = currentFunctionObject;
			currentFunctionObject = currentFunctionObject->next;
		}
		string lastFunctionNameFull = lastFunctionObject->headerFunctionNameFullUpdated;
		int positionOfLastFunctionObjectInHeader = currentFileObject->headerFileText.find(lastFunctionNameFull);
		if(positionOfFirstFunctionObjectInHeader != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string classDeclarationFooter = CS_GENERATE_CPP_CLASSES_TEXT_CLASS_FOOTER;	//};
			int lastFunctionNameFullLength = (lastFunctionObject->headerFunctionNameFullUpdated).length();
			int positionOfLastFunctionObjectInHeaderEnd = positionOfLastFunctionObjectInHeader + lastFunctionNameFullLength;
			currentFileObject->headerFileText = currentFileObject->headerFileText.substr(0, positionOfLastFunctionObjectInHeaderEnd+1) + classDeclarationFooter + currentFileObject->headerFileText.substr(positionOfLastFunctionObjectInHeaderEnd+1, currentFileObject->headerFileText.length()-positionOfLastFunctionObjectInHeaderEnd);	//+1 for trailing CHAR_SEMICOLON
		}
		else
		{
			cout << "error: (positionOfFirstFunctionObjectInHeader == CPP_STRING_FIND_RESULT_FAIL_VALUE" << endl;
			exit(0);
		}

		//6. add move source include file statements to header (required for referenced class declarations)
		if(!moveIncludeFileStatementsToHeader(currentFileObject))
		{
			result = false;
		}	
	}

	#ifndef CS_GENERATE_CPP_CLASSES_DISABLE_OUTPUT
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
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

string replaceAllOccurancesOfFunctionObjectReferenceNameInFunction(string* functionTextOrig, string functionReferenceName, string functionReferenceNameUpdated, bool* foundAtLeastOneInstance)
{
	string functionTextUpdated = *functionTextOrig;
	
	bool stillSearching = true;
	int pos = 0;
	while(stillSearching)
	{
		pos = functionTextUpdated.find(functionReferenceName, pos);
		if(pos != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{//only update function reference within functionText once

			//added condition CS 3f1b - ensure previous character is not a letter (this ensures that ABCfunctionName is not found when searching for functionName)
			if((pos == 0) || !charInCharArray(functionTextUpdated[pos-1], functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
			{	
				functionTextUpdated.replace(pos, functionReferenceName.length(), functionReferenceNameUpdated);
				pos = pos + functionReferenceNameUpdated.length();
				*foundAtLeastOneInstance = true;
			}
			else
			{
				pos = pos + functionReferenceName.length();
			}
		}
		else
		{
			stillSearching = false;
		}
	}
	
	return functionTextUpdated;
}


void isFunctionBeingReferencedPublicallyRecurse(string functionName, string fileName, CSfileContainer* firstObjectInAboveLevelBelowListContainer, bool* foundPublicReference)
{
	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;

		if(currentFileObject->name != fileName)
		{//search for external references to function only

			CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
			while(currentFunctionObject->next != NULL)
			{
				CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceList;
				while(currentFunctionReference->next != NULL)
				{
					if(currentFunctionReference->name == functionName)
					{
						*foundPublicReference = true;
						#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
						//cout << "foundPublicReference" << endl;
						#endif
					}
					currentFunctionReference = currentFunctionReference->next;
				}
				
				currentFunctionObject = currentFunctionObject->next;
			}

			if(currentFileObject->firstFileInBelowListContainer != NULL)
			{
				isFunctionBeingReferencedPublicallyRecurse(functionName, fileName, currentFileObject->firstFileInBelowListContainer, foundPublicReference);
			}
		}
			
		currentFileObjectContainer = currentFileObjectContainer->next;
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
	return className + CS_GENERATE_CPP_CLASSES_TEXT_CLASS_DECLARATION_APPENDITION;
}

string generateClassObjectName(string className)	
{
	return className + CS_GENERATE_CPP_CLASSES_TEXT_CLASS_OBJECT_APPENDITION;
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
			permissionsString = string(CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_PUBLIC);
		}
		else
		{
			permissionsString = string(CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_PRIVATE);
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
		int positionOfStaticIdentifier = fullFunctionNamePart1.find(CS_GENERATE_CPP_CLASSES_TEXT_STATIC);
		if((positionOfStaticIdentifier != CPP_STRING_FIND_RESULT_FAIL_VALUE) || (functionName == CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_MAIN_NAME))
		{
			classFullFunctionName = fullFunctionName;	//do not add class context to static functions	//do not add class context to main functions
			*foundStaticReference = true;
		}
		else
		{
			classFullFunctionName = fullFunctionNamePart1 + generateClassDeclarationName(className) + string(CS_GENERATE_CPP_CLASSES_TEXT_CLASS_PERMISSIONS_IDENTIFIER) + fullFunctionNamePart2;
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
		referencedClassesDeclarations = referencedClassesDeclarations + CHAR_TAB + CS_GENERATE_CPP_CLASSES_TEXT_FUNCTION_PRIVATE + generateClassDeclarationName(currentReferencedClassInList->className) + CHAR_SPACE + generateClassObjectName(currentReferencedClassInList->className) + CHAR_SEMICOLON + CHAR_NEWLINE; 	//ie private: xClass x;
		//cout << "referencedClassesDeclarations = " << referencedClassesDeclarations << endl;
		currentReferencedClassInList = currentReferencedClassInList->next;
	}
	return referencedClassesDeclarations;
}

bool moveIncludeFileStatementsToHeader(CSfile* firstReferenceInAboveLevelBelowList)	
{
	bool result = true;
	string includeStatementsHeaderNew = "";
	
	CSfileContainer* currentFileObjectContainer = firstReferenceInAboveLevelBelowList->firstFileInBelowListContainer;
	int positionOfLastIncludeStatementEndInHeader = CPP_STRING_FIND_RESULT_FAIL_VALUE;
	int lineOfFirstIncludeStatementEndInSource = REALLY_LARGE_INT;
	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;
		
		/*
		//if(currentFileObject->name != firstReferenceInAboveLevelBelowList->name){	
		//do not modify include .h of .c file - not required because of "if(hashIncludeFileName != topLevelReference->name)	//this is added so that do not parse method.h from within method.cpp!"
		*/
		string includeStatement = string(CS_GENERATE_CPP_CLASSES_TEXT_INCLUDE_START) + currentFileObject->name + CS_GENERATE_CPP_CLASSES_TEXT_INCLUDE_END;
		
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
				
		currentFileObjectContainer = currentFileObjectContainer->next;
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


#endif
