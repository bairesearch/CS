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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3e1b 27-August-2014
 *
 *******************************************************************************/


#include "CSgenerateObjectOrientedCode.h"
#include "CSdraw.h"
#include "SHAREDvars.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
using namespace std;



#ifdef CS_GENERATE_CPP_CLASSES

ReferencedClass::ReferencedClass(void)
{
	className = "";
	next = NULL;
}

bool generateCPPclasses(CSfileReference * firstReferenceInTopLevelBelowList)
{
	bool result = true;
	
	if(!generateCPPclassesRecurse(firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList))
	{
		result = false;
	}
	
	return result;	
}

bool generateCPPclassesRecurse(CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList)	
{
	bool result = true;
	
	CSfileReference * currentFileReference = firstReferenceInAboveLevelBelowList;
	while(currentFileReference->next != NULL)
	{
		if(currentFileReference->firstReferenceInBelowList != NULL)
		{
			if(currentFileReference->printed)
			{//only create object oriented code for printed boxes
			
				currentFileReference->headerFileTextOrig = getFileContents(currentFileReference->name);	//headerFileNameOrig
				currentFileReference->sourceFileTextOrig = getFileContents(currentFileReference->sourceFileNameOrig);
				currentFileReference->headerFileText = currentFileReference->headerFileTextOrig;	//initialise with same source code as original file
				currentFileReference->sourceFileText = currentFileReference->sourceFileTextOrig;	//initialise with same source code as original file
				
				#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
				//cout << "generateCPPclassesRecurse(): currentFileReference->sourceFileName = " << currentFileReference->sourceFileName << endl;
				//cout << "generateCPPclassesRecurse(): currentFileReference->headerFileName = " << currentFileReference->headerFileName << endl;
				#endif
				if(!generateCPPclassesFile(currentFileReference, firstReferenceInTopLevelBelowList))
				{
					result = false;
				}	

			}
			if(currentFileReference->firstReferenceInBelowList != NULL)
			{
				generateCPPclassesRecurse(currentFileReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList);
			}
		}

		currentFileReference = currentFileReference->next;
	}
	return result;
}
	
bool generateCPPclassesFile(CSfileReference * currentFileReference, CSfileReference * firstReferenceInTopLevelBelowList)
{
	bool result = true;
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	cout << "generateCPPclassesFile(): currentFileReference->name = " << currentFileReference->name << endl;	
	#endif
	
	string className = generateClassName(currentFileReference->name);
	ReferencedClass * firstReferencedClassInList = new ReferencedClass();
	ReferencedClass * currentReferencedClassInList = firstReferencedClassInList;
				
	CSfunctionReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
	while(currentFunctionReference->next != NULL)
	{
		//if(currentFunctionReference->printed)
		//{//only create connections to printed boxes (is this required?)
			if(currentFileReference->name != CS_GENERATE_CPP_CLASSES_FUNCTION_MAIN_NAME)
			{//do not modifier main function

				bool foundPublicReference = false;
				isFunctionBeingReferencedPublicallyRecurse(currentFunctionReference->name, currentFileReference->name, firstReferenceInTopLevelBelowList, &foundPublicReference);

				//1. convert function names
				int positionOfFunctionReferenceHeaderOrig = currentFileReference->headerFileTextOrig.find(currentFunctionReference->nameFull);
				int positionOfFunctionReferenceSourceOrig = currentFileReference->sourceFileTextOrig.find(currentFunctionReference->nameFull);
				string classFullFunctionNameHeader = convertFunctionNameToClassFunctionNameHeader(currentFunctionReference->nameFull, currentFunctionReference->name, className, foundPublicReference);
				string classFullFunctionNameSource = convertFunctionNameToClassFunctionNameSource(currentFunctionReference->nameFull, currentFunctionReference->name, className, foundPublicReference);
				currentFunctionReference->headerFunctionNameFullUpdated = classFullFunctionNameHeader;
				currentFunctionReference->sourceFunctionNameFullUpdated = classFullFunctionNameSource;

				bool foundAtLeastOneInstance = false;
				currentFileReference->headerFileText = replaceAllOccurancesOfString(&(currentFileReference->headerFileText), currentFunctionReference->nameFull, classFullFunctionNameHeader, &foundAtLeastOneInstance);
				if(!foundAtLeastOneInstance)
				{
					cout << "generateCPPclassesFile() error: !foundAtLeastOneInstance of " << currentFunctionReference->nameFull << " in currentFileReference->headerFileText" << endl;
					exit(0);
				}
				foundAtLeastOneInstance = false;
				currentFileReference->sourceFileText = replaceAllOccurancesOfString(&(currentFileReference->sourceFileText), currentFunctionReference->nameFull, classFullFunctionNameSource, &foundAtLeastOneInstance);
				if(!foundAtLeastOneInstance)
				{
					cout << "generateCPPclassesFile() error: !foundAtLeastOneInstance of " << currentFunctionReference->nameFull << " in currentFileReference->sourceFileText" << endl;
					exit(0);
				}		

				//2. convert function references
				CSfunctionReference * currentfunctionReferenceReference = currentFunctionReference->firstReferenceInFunctionReferenceList;
				while(currentfunctionReferenceReference->next != NULL)
				{
					string functionReferenceReferenceName = currentfunctionReferenceReference->name;
					string functionReferenceReferenceNameUpdated = "";

					string functionReferenceContext = "";
					CSfileReference * fileReferenceHoldingFunction = NULL;
					bool referencedFunctionFound = false;
					CSfunctionReference * referencedFunction = findPrintedFunctionReferenceWithName(functionReferenceReferenceName, NULL, firstReferenceInTopLevelBelowList, &referencedFunctionFound, &fileReferenceHoldingFunction);
					if(referencedFunctionFound)
					{
						string fileNameHoldingFunction = fileReferenceHoldingFunction->name;
						string classHoldingFunction = generateClassName(fileNameHoldingFunction);

						//add context
						if(fileNameHoldingFunction == currentFileReference->name)
						{
							functionReferenceReferenceNameUpdated = string(CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_LOCAL) + functionReferenceReferenceName;
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
						#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
						//cout << "functionReferenceReferenceName = " << functionReferenceReferenceName << endl;
						//cout << "functionReferenceReferenceNameUpdated = " << functionReferenceReferenceNameUpdated << endl;
						#endif
						
					
						int posOfFunctionRefUpdated = currentFunctionReference->functionTextRaw.find(functionReferenceReferenceNameUpdated);
						if(posOfFunctionRefUpdated == CPP_STRING_FIND_RESULT_FAIL_VALUE)
						{//only update function reference within functionText once
							
							int posOfFunctionText = currentFileReference->sourceFileText.find(currentFunctionReference->functionTextRaw);
							int functionTextOrigLength = currentFunctionReference->functionTextRaw.length();
							if(posOfFunctionText != CPP_STRING_FIND_RESULT_FAIL_VALUE)
							{
								foundAtLeastOneInstance = false;
								currentFunctionReference->functionTextRaw = replaceAllOccurancesOfString(&(currentFunctionReference->functionTextRaw), functionReferenceReferenceName, functionReferenceReferenceNameUpdated, &foundAtLeastOneInstance);
								if(!foundAtLeastOneInstance)
								{
									cout << "generateCPPclassesFile() error: !foundAtLeastOneInstance of " << functionReferenceReferenceName << " in currentFunctionReference->functionTextRaw" << endl;
									exit(0);
								}
								currentFileReference->sourceFileText = currentFileReference->sourceFileText.substr(0, posOfFunctionText) + currentFunctionReference->functionTextRaw + currentFileReference->sourceFileText.substr(posOfFunctionText+functionTextOrigLength, currentFileReference->sourceFileText.length()-posOfFunctionText+functionTextOrigLength);

								//cout << "currentFunctionReference->functionTextRaw = " << currentFunctionReference->functionTextRaw << endl;							
								//exit(0);
							}
							else
							{
								cout << "generateCPPclassesFile() error: currentFunctionReference->functionTextRaw not found in currentFileReference->sourceFileText" << endl;
								cout << "currentFunctionReference->functionTextRaw = " <<  currentFunctionReference->functionTextRaw << endl;
								exit(0); 
							}
						}
					}

					#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
					//cout << "generateCPPclassesFile(): currentFunctionReference->name = " << currentFunctionReference->name << endl;	
					//cout << "generateCPPclassesFile(): currentFunctionReference->nameFull = " << currentFunctionReference->nameFull << endl;
					#endif
					currentfunctionReferenceReference = currentfunctionReferenceReference->next;
				}
			//}
		}
		currentFunctionReference = currentFunctionReference->next;	
	}
	
	//CHECK THIS QUICK CODING;
	
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
	
		
	//5. add class wrapper erfoot to class function declarations in class header file
	//find location of last function reference in header	
	currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
	CSfunctionReference * lastFunctionReference = currentFunctionReference;
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
	
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	cout << "currentFileReference->headerFileText = \n" << currentFileReference->headerFileText << endl;
	cout << "currentFileReference->sourceFileText = \n" << currentFileReference->sourceFileText << endl;
	#endif
	
	writeStringToFile(&(currentFileReference->headerFileName), &(currentFileReference->headerFileText));
	writeStringToFile(&(currentFileReference->sourceFileName), &(currentFileReference->sourceFileText));
		
	return result;
}
		
string getFileContents(string inputFileName)
{
	string fileContents = "";
	
	bool result = true;
	ifstream parseFileObject(inputFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//.c/.h file does not exist in current directory.
		cout << "Error: CS input file does not exist in current directory: " << inputFileName << endl;
		//exit(0);
		result = false;
	}
	else
	{
		string currentLine = "";
		int currentLineNumber = 0;
		while(getline(parseFileObject, currentLine))
		{
			fileContents = fileContents + currentLine + CHAR_NEWLINE;
			currentLineNumber++;
		}
	}
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	//cout << "fileContents = " << fileContents << endl;
	#endif
	
	return fileContents;
}

void isFunctionBeingReferencedPublicallyRecurse(string functionName, string fileName, CSfileReference * firstReferenceInAboveLevelBelowList, bool * foundPublicReference)
{
	CSfileReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{
		if(currentFileReference->printed)
		{
			if(currentFileReference->name != fileName)
			{//search for external references to function only
			
				CSfunctionReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
				while(currentFunctionReference->next != NULL)
				{
					if(currentFunctionReference->printed)
					{
						CSfunctionReference * currentfunctionReferenceReference = currentFunctionReference->firstReferenceInFunctionReferenceList;
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
					}
					currentFunctionReference = currentFunctionReference->next;
				}

				if(currentFileReference->firstReferenceInBelowList != NULL)
				{
					isFunctionBeingReferencedPublicallyRecurse(functionName, fileName, currentFileReference->firstReferenceInBelowList, foundPublicReference);
				}
			}
			
		}
		currentFileReference = currentFileReference->next;
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


string convertFunctionNameToClassFunctionNameHeader(string fullFunctionName, string functionName, string className, bool foundPublicReference)
{
	string classFullFunctionName = "";
	
	string permissionsString = "";
	if(foundPublicReference)
	{
		permissionsString = string(CS_GENERATE_CPP_CLASSES_FUNCTION_PUBLIC);
	}
	else
	{
		permissionsString = string(CS_GENERATE_CPP_CLASSES_FUNCTION_PRIVATE);
	}
	
	classFullFunctionName = STRING_TAB + permissionsString + fullFunctionName;
	
	return classFullFunctionName;
	
	
}

string convertFunctionNameToClassFunctionNameSource(string fullFunctionName, string functionName, string className, bool foundPublicReference)
{
	string classFullFunctionName = "";
	
	string fullFunctionNamePart1 = "";
	string fullFunctionNamePart2 = ""; 
	int positionOfFunctionName = fullFunctionName.find(functionName);
	if(positionOfFunctionName != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		fullFunctionNamePart1 = fullFunctionName.substr(0, positionOfFunctionName);
		fullFunctionNamePart2 = fullFunctionName.substr(positionOfFunctionName, fullFunctionName.length());

		#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
		//cout << "fullFunctionNamePart1 = " << fullFunctionNamePart1 << endl;
		//cout << "fullFunctionNamePart2 = " << fullFunctionNamePart2 << endl;
		#endif
	}
		
	classFullFunctionName = fullFunctionNamePart1 + className + string(CS_GENERATE_CPP_CLASSES_CLASS_PERMISSIONS_IDENTIFIER) + fullFunctionNamePart2;
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	//cout << "classFullFunctionName = " << classFullFunctionName << endl;
	#endif
	
	return classFullFunctionName;
		
}

bool findReferencedClassInList(ReferencedClass * firstReferencedClassInList, string classNameToFind)
{
	bool foundReferencedClass = false;
	
	ReferencedClass * currentReferencedClassInList = firstReferencedClassInList;
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

string generateReferencedClassesDeclarations(ReferencedClass * firstReferencedClassInList)
{
	string referencedClassesDeclarations = "";
	ReferencedClass * currentReferencedClassInList = firstReferencedClassInList;
	while(currentReferencedClassInList->next != NULL)
	{
		referencedClassesDeclarations = referencedClassesDeclarations + CHAR_TAB + CS_GENERATE_CPP_CLASSES_FUNCTION_PRIVATE + generateClassDeclarationName(currentReferencedClassInList->className) + CHAR_SPACE + generateClassObjectName(currentReferencedClassInList->className) + CHAR_SEMICOLON + CHAR_NEWLINE; 	//ie private: xClass x;
		//cout << "referencedClassesDeclarations = " << referencedClassesDeclarations << endl;
		currentReferencedClassInList = currentReferencedClassInList->next;
	}
	return referencedClassesDeclarations;
}

void writeStringToFile(string * fileName, string * s)
{
	ofstream writeFileObject(fileName->c_str());

	for(int i=0; i < s->size(); i++)
	{
		writeFileObject.put((*s)[i]);
	}

	writeFileObject.close();
}


#endif
