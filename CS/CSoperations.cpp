/*******************************************************************************
 *
 * File Name: CSoperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a6c 07-May-2012
 *
 *******************************************************************************/



#include "CSoperations.h"
#include "LDsprite.h"
#include "LDreferenceManipulation.h"



#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;








bool getIncludeFileNamesFromCorHFile(CSReference * firstReferenceInIncludeFileList, CSReference * topLevelReference, string topLevelReferenceName, int level)
{
	bool fileFound = false;
	
	string parseFileName = topLevelReferenceName;
	CSReference * previousReference = NULL;

	char * parseFileNamecharstar = const_cast<char*>(parseFileName.c_str());

	ifstream * parseFileObject = new ifstream(parseFileNamecharstar);
	 CSReference * currentReferenceInIncludeFileList = firstReferenceInIncludeFileList;

	if(!parseFileObject->rdbuf( )->is_open( ))
	{
		//cout << "CS error - file not found, " << parseFileName << endl;
	}
	else
	{
		fileFound = true;
		
		char c;
		int id = 0;
		int charCount = 0;
		int lineCount = 0;
		bool readingHashIncludePreceedingWhiteSpace = true;
		bool readingHashInclude = false;
		bool readingHashIncludeFileName = false;
		bool waitingForNewLine = false;
		char hashInclude[1000] = "";
		char hashIncludeFileName[1000] = "";
		//bool readingLineComment = false;	//same as waitingForNewLine
		bool readingLargeComment = false;

		while(parseFileObject->get(c))
		{
			charCount++;

			if(readingLargeComment)
			{

				if(c == '\n')
				{
					lineCount++;
				}
				else
				{
					while(c == '*')
					{
						parseFileObject->get(c);
						if(c == '/')
						{
							readingLargeComment = false;
							waitingForNewLine = true;
						}
						else if(c == '\n')
						{
							lineCount++;
						}
					}
				}

			}
			else if(c == '/')
			{
				char tempChar;
				parseFileObject->get(tempChar);
				if(tempChar == '*')
				{
					readingLargeComment = true;
				}
				else if(c == '/')
				{
					//single line comment found
					waitingForNewLine = true;

				}
				else if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingHashIncludePreceedingWhiteSpace = true;
					readingHashInclude = false;
					readingHashIncludeFileName = false;
					hashInclude[0] = '\0';
				}
				else
				{
					//CS does not support relative paths in #include that may include a / or a \ - header h/hpp files not be in the same folder as source c/cpp files
					waitingForNewLine = true;
				}
			}
			else if(waitingForNewLine)
			{
				if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingHashIncludePreceedingWhiteSpace = true;
					readingHashInclude = false;
					readingHashIncludeFileName = false;
					hashInclude[0] = '\0';
				}
				else
				{
					//do nothing, still waiting for new line
				}
			}
			else if(readingHashIncludePreceedingWhiteSpace)
			{
				if((c == ' ') || (c == '\t'))
				{

				}
				else if((c == '\n'))
				{
					lineCount++;
					hashInclude[0] = '\0';
				}
				else
				{
					readingHashIncludePreceedingWhiteSpace = false;
					readingHashInclude = true;

					//add this character to first character
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(hashInclude, typeString);
				}
			}
			else if(readingHashInclude)
			{
				if(c == ' ')
				{
					readingHashIncludePreceedingWhiteSpace = false;

					if(!strcmp(hashInclude, "#include"))
					{
						//cout << "hashInclude = " << hashInclude << endl;

						char tempChar;
						parseFileObject->get(tempChar);
						if(tempChar == '\"')
						{
							readingHashInclude = false;
							readingHashIncludeFileName = true;
						}
						else
						{
							readingHashInclude = false;
							waitingForNewLine = true;
						}
					}
					else
					{
						readingHashInclude = false;
						waitingForNewLine = true;
					}

				}
				else if(c == '\n')
				{
					readingHashInclude = false;
					readingHashIncludePreceedingWhiteSpace = true;
					hashInclude[0] = '\0';
				}
				else
				{
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(hashInclude, typeString);
				}

			}
			else if(readingHashIncludeFileName)
			{
				if(c == '\"')
				{
					string hashIncludeFileNameString = hashIncludeFileName;
					if(hashIncludeFileNameString != topLevelReference->name)	//this is added so that do not parse method.h from within method.cpp!
					{

						/*
						string hashIncludeFileNameStringCNotH = hashIncludeFileName;
						string cExtension = ".c";
						string hExtension = ".h";
						int positionOfHExtension =
						hashIncludeFileNameStringCNotH.replace(9,5,cExtension);
						currentReferenceInIncludeFileList->name = hashIncludeFileNameCNotH;
						*/

						char hashIncludeFileNameCFile[100];
						strcpy(hashIncludeFileNameCFile, hashIncludeFileName);
						int stringLength = strlen(hashIncludeFileNameCFile);
						hashIncludeFileNameCFile[stringLength-1] = 'c';
						hashIncludeFileNameCFile[stringLength+0] = 'p';
						hashIncludeFileNameCFile[stringLength+1] = 'p';
						hashIncludeFileNameCFile[stringLength+2] = '\0';

						currentReferenceInIncludeFileList->isFileReference = true;
						currentReferenceInIncludeFileList->name = hashIncludeFileName;
						currentReferenceInIncludeFileList->id = id;
						currentReferenceInIncludeFileList->level = level;
						currentReferenceInIncludeFileList->firstReferenceInAboveList = topLevelReference;
						currentReferenceInIncludeFileList->previous = previousReference;
						id++;

						CSReference * newCSReference = new CSReference();
						currentReferenceInIncludeFileList->next = newCSReference;

						previousReference = currentReferenceInIncludeFileList;

						CSReference * newfirstReferenceInBelowList = new CSReference();
						currentReferenceInIncludeFileList->firstReferenceInBelowList = newfirstReferenceInBelowList;

						#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
							//parse into .h file;
							for(int i= 0; i<level; i++)
							{
								cout << "\t";
							}
							cout << hashIncludeFileName << endl;
							//parse into .cpp file;
							for(int i= 0; i<level; i++)
							{
								cout << "\t";
							}
							cout << hashIncludeFileNameCFile << endl;
						#endif

						//parse into .h file;
						string referenceName = hashIncludeFileName;
						bool hFileFound = getIncludeFileNamesFromCorHFile(currentReferenceInIncludeFileList->firstReferenceInBelowList, currentReferenceInIncludeFileList, referenceName, level+1);
						
						if(hFileFound)
						{
							//parse into .cpp file;
							CSReference * lastReferenceInBelowList = currentReferenceInIncludeFileList->firstReferenceInBelowList;
							while(lastReferenceInBelowList->next != NULL)
							{
								lastReferenceInBelowList = lastReferenceInBelowList->next;
							}
							string referenceNameCFile = hashIncludeFileNameCFile;
							bool cFileFound = getIncludeFileNamesFromCorHFile(lastReferenceInBelowList, currentReferenceInIncludeFileList, referenceNameCFile, level+1);

							if(cFileFound)
							{
								//fill function and function reference list using .h file;
								CSReference * newfirstReferenceInFunctionList = new CSReference();
								currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;
								
								bool hFileFound2 = getFunctionNamesFromFunctionDeclarationsInHFile(currentReferenceInIncludeFileList->firstReferenceInFunctionList, referenceName, level);
								if(hFileFound2)
								{								
									//cout << "currentReferenceInIncludeFileList->firstReferenceInFunctionList->name = " << currentReferenceInIncludeFileList->firstReferenceInFunctionList->name << endl;

									//cout << "topLevelReferenceName = " << topLevelReferenceName << endl;
									//cout << "getFunctionReferenceNamesFromFunctionsInCFile()" << endl;
									getFunctionReferenceNamesFromFunctionsInCFile(firstReferenceInIncludeFileList, currentReferenceInIncludeFileList->firstReferenceInFunctionList, currentReferenceInIncludeFileList, referenceNameCFile, level);

								}
								else
								{
									//cout << "!hFileFound2" << endl;
								}
							}
							else
							{
								//cout << "!cFileFound" << endl;
								CSReference * newfirstReferenceInFunctionList = new CSReference();
								currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;							
							}
						}
						else
						{
							//cout << "!hFileFound" << endl;
							CSReference * newfirstReferenceInFunctionList = new CSReference();
							currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;						
						}

						//cout << "h3" << endl;
						
						currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;


					}
					else
					{
						/*
						cout << "hashIncludeFileNameString = " << hashIncludeFileNameString << endl;
						cout << "topLevelReference->name = " << topLevelReference->name << endl;
						*/
					}

					hashIncludeFileName[0] = '\0';

					lineCount++;
					readingHashIncludeFileName = false;
					waitingForNewLine = true;
				}
				else
				{
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(hashIncludeFileName, typeString);
				}
			}
			else
			{
				cout << "getIncludeFileNamesFromCorHFile: token error" << endl;
				cout << "char = " << c << endl;
				cout << "parseFileNamecharstar = " << parseFileNamecharstar << endl;
				cout << "lineCount = " << lineCount << endl;
			}
		}
		parseFileObject->close();
	}
	
	delete parseFileObject;
	return fileFound;
}


bool getFunctionNamesFromFunctionDeclarationsInHFile(CSReference * firstReferenceInFunctionList, string topLevelFileName, int level)
{	
	bool fileFound = false;
	
	string parseFileName = topLevelFileName;

	char * parseFileNamecharstar = const_cast<char*>(parseFileName.c_str());

	ifstream * parseFileObject = new ifstream(parseFileNamecharstar);
	CSReference * currentReferenceInFunctionList = firstReferenceInFunctionList;
	CSReference * currentReferenceInFunctionReferenceList = NULL;

	if(!parseFileObject->rdbuf( )->is_open( ))
	{
		//cout << "CS error - file not found, " << parseFileName << endl;
	}
	else
	{
		fileFound = true;

		char c;
		int id = 0;
		int charCount = 0;
		int lineCount = 0;
		bool readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
		bool readingBeforeSpaceBetweenFunctionTypeAndName = false;
		bool readingBeforeOpeningBracket = false;
		bool readingBeforeClosingBracket = false;
		bool waitingForNewLine = false;
		bool readingLargeComment = false;

		char functionName[1000] = "";
		char functionNameFull[1000] = "";


		while(parseFileObject->get(c))
		{
			charCount++;

			if(readingLargeComment)
			{

				if(c == '\n')
				{
					lineCount++;
				}
				else
				{
					while(c == '*')
					{
						parseFileObject->get(c);
						if(c == '/')
						{
							readingLargeComment = false;
							waitingForNewLine = true;
						}
						else if(c == '\n')
						{
							lineCount++;
						}
					}
				}

			}
			else if(c == '/')
			{
				char tempChar;
				parseFileObject->get(tempChar);
				if(tempChar == '*')
				{
					readingLargeComment = true;
				}
				else if(c == '/')
				{
					//single line comment found
					waitingForNewLine = true;

				}
				else if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					readingBeforeSpaceBetweenFunctionTypeAndName = false;
					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = false;
					functionName[0] = '\0';
					functionNameFull[0] = '\0';
				}
				else
				{
					//CS does not support / values in function names
					waitingForNewLine = true;
				}
			}
			else if(waitingForNewLine)
			{
				if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					readingBeforeSpaceBetweenFunctionTypeAndName = false;
					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = false;
					functionName[0] = '\0';
					functionNameFull[0] = '\0';
				}
				else
				{
					//do nothing, still waiting for new line
				}
			}
			else if(readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace)
			{
				if((c == ' ') || (c == '\t'))
				{
					//ignore preceeding white space
				}
				else if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					readingBeforeSpaceBetweenFunctionTypeAndName = false;
					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = false;
					functionName[0] = '\0';
					functionNameFull[0] = '\0';
				}
				else
				{
					//found start of function declaration
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = false;
					readingBeforeSpaceBetweenFunctionTypeAndName = true;

					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);
				}
			}
			else if(readingBeforeSpaceBetweenFunctionTypeAndName)
			{
				if(c == ' ')
				{
					readingBeforeSpaceBetweenFunctionTypeAndName = false;

					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);

					//restart function name fill;
					functionName[0] = '\0';
					readingBeforeOpeningBracket = true;
				}
				else if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					readingBeforeSpaceBetweenFunctionTypeAndName = false;
					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = false;
					functionName[0] = '\0';
					functionNameFull[0] = '\0';
				}
				else
				{
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);
				}

			}
			else if(readingBeforeOpeningBracket)
			{
				if(c == ' ')
				{

					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);

					//not a function, restart function name fill;
					functionName[0] = '\0';
				}
				else if((readingBeforeOpeningBracket) && (c == '('))
				{

					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);

					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = true;
				}
				else if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					readingBeforeSpaceBetweenFunctionTypeAndName = false;
					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = false;
					functionName[0] = '\0';
					functionNameFull[0] = '\0';
				}
				else
				{
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionName, typeString);

					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);
				}
			}
			else if(readingBeforeClosingBracket)
			{
				if(c == ')')
				{
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);

					char newC;
					parseFileObject->get(newC);
					if(newC == ';')
					{//function reference found


							/*
							//add function reference reference;
							for(int i= 0; i<level+1; i++)
							{
								cout << "\t";
							}
							cout << functionName << endl;
							cout << functionNameFull << endl;
							*/

							/*
							//add function reference reference;
							for(int i= 0; i<level+1; i++)
							{
								cout << "\t";
							}
							cout << topLevelFileName << ": " << functionName << endl;
							*/


						currentReferenceInFunctionList->isFunctionReference = true;
						currentReferenceInFunctionList->name = functionName;
						currentReferenceInFunctionList->nameFull = functionNameFull;
						CSReference * newCSReference = new CSReference();
						currentReferenceInFunctionList->next = newCSReference;

						currentReferenceInFunctionList = currentReferenceInFunctionList->next;

					}
					else
					{
						//cout << "error - invalid function reference" << endl;


					}

					readingBeforeClosingBracket = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					functionNameFull[0] = '\0';
				}
				else if(c == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					readingBeforeSpaceBetweenFunctionTypeAndName = false;
					readingBeforeOpeningBracket = false;
					readingBeforeClosingBracket = false;
					functionName[0] = '\0';
					functionNameFull[0] = '\0';
				}
				else
				{
					char typeString[2];
					typeString[0] = c;
					typeString[1] = '\0';
					strcat(functionNameFull, typeString);
				}
			}
			else
			{
				cout << "getFunctionNamesFromFunctionDeclarationsInHFile: token error" << endl;
				cout << "char = " << c << endl;
				cout << "parseFileNamecharstar = " << parseFileNamecharstar << endl;
				cout << "lineCount = " << lineCount << endl;
			}
		}
		parseFileObject->close();
	}
	
	delete parseFileObject;
	return fileFound;
}



#define MAX_CODE_FILE_SIZE (1000000)

void getFunctionReferenceNamesFromFunctionsInCFile(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInFunctionList, CSReference * topLevelReference, string topLevelFileName, int level)
{
	string codeFileName = topLevelFileName;
	char * codeFileNamecharstar = const_cast<char*>(codeFileName.c_str());

	int codeFileByteArraySize;
	int codeFileNumberOfLines;

						/*
						char * codeFileByteArray = new char[MAX_CODE_FILE_SIZE];
						if(!readFileIntoByteArray(codeFileNamecharstar, codeFileByteArray, &codeFileNumberOfLines, &codeFileByteArraySize))
						{
							cout << "error; cannot find file" << endl;
							exit(0);
						}
						*/

	string fileContentsString = "";

	if(!readFileIntoString(codeFileName, &fileContentsString, &codeFileNumberOfLines, &codeFileByteArraySize))
	{
		//cout << "error; cannot find file" << endl;
		//exit(0);
	}
	else
	{

		CSReference * currentReference = firstReferenceInFunctionList;

		//0. for each fullfilename [identified in header];

		while(currentReference->next != NULL)
		{

			string fullFunctionName = currentReference->nameFull;

			//1. in cpp file, find fullfilename [identified in header];
						//char * positionOfFunctionReference;
						//positionOfFunctionReference = strstr(codeFileByteArray, const_cast<char*>(functionName.c_str()));
			//cout << "fileContentsString = " << fileContentsString << endl;
			//cout << "fullFunctionName = " << fullFunctionName << endl;
			int positionOfFunctionReference = fileContentsString.find(fullFunctionName);

			if(positionOfFunctionReference != -1)
			{
				/*NEW method - comments are ignored - there is a problem with this...*/

				/*
				string functionContentsString = "";

				//2. create a string containing all text from this position to the closing bracket "\n}"
				bool stillFindingEndOfFunction = true;
				bool waitingForNewLine = false;
				bool readingLargeComment = false;
				int lineCount = 0;

				int currentIndexInFunction = positionOfFunctionReference;
				while(stillFindingEndOfFunction)
				{
					char c = fileContentsString[currentIndexInFunction];
					currentIndexInFunction++;
					if(readingLargeComment)
					{
						if(c == '\n')
						{
							lineCount++;
						}
						else
						{
							while(c == '*')
							{

								c = fileContentsString[currentIndexInFunction];
								currentIndexInFunction++;
								if(c == '/')
								{
									readingLargeComment = false;
								}
								else if(c == '\n')
								{
									lineCount++;
								}
							}
						}

					}
					else if(c == '/')
					{
						c = fileContentsString[currentIndexInFunction];
						currentIndexInFunction++;
						if(c == '*')
						{
							readingLargeComment = true;
						}
						else if(c == '/')
						{
							//single line comment found
							waitingForNewLine = true;

						}
						else if(c == '\n')
						{
							lineCount++;
							if(waitingForNewLine)
							{
								waitingForNewLine = false;
							}
							else
							{
								functionContentsString = functionContentsString + c;
							}
						}
						else
						{
							functionContentsString = functionContentsString + c;
						}
					}
					else if(waitingForNewLine)
					{
						if(c == '\n')
						{
							lineCount++;
							waitingForNewLine = false;
						}
						else
						{
							//do nothing, still waiting for new line
						}
					}
					else if(c == '\n')
					{
						functionContentsString = functionContentsString + c;

						c = fileContentsString[currentIndexInFunction];
						currentIndexInFunction++;
						if(c == '}')
						{
							stillFindingEndOfFunction = false;
						}
						functionContentsString = functionContentsString + c;
					}
					else
					{
						functionContentsString = functionContentsString + c;
					}
				}
				*/

				//OLD method - comments are not ignored;
				bool stillFindingEndOfFunction = true;
				bool newLineFound = false;
				
				#define CS_MAX_NUM_CHARACTERS_PER_FUNCTION (1000000)
				int currentIndexInFunction = positionOfFunctionReference;
				while(stillFindingEndOfFunction)
				{
					char c = fileContentsString[currentIndexInFunction];
					
					if(newLineFound)
					{
						if(c == '}')
						{
							stillFindingEndOfFunction = false;
						}
						else
						{
							newLineFound = false;
						}
					}
					else
					{
						newLineFound = false;
					}
					
					if(c == '\n')
					{
						newLineFound = true;
					}
					
					if(currentIndexInFunction > CS_MAX_NUM_CHARACTERS_PER_FUNCTION)
					{
						cout << "getFunctionReferenceNamesFromFunctionsInCFile() error: function definitions in .cpp files must end with a } without any leading white space" << endl;
						exit(0);
					}

					currentIndexInFunction++;
				}
				int positionOfFunctionReferenceEnd = currentIndexInFunction;
				string functionContentsString = fileContentsString.substr(positionOfFunctionReference+fullFunctionName.length(), (positionOfFunctionReferenceEnd-positionOfFunctionReference)-fullFunctionName.length());





				//3. search this string for any of the function (not full function names) across all include/header files
				CSReference * newfirstReferenceInFunctionReferenceList = new CSReference();
				currentReference->firstReferenceInFunctionReferenceList = newfirstReferenceInFunctionReferenceList;

				/*
				cout << "file topLevelFileName = " << topLevelFileName << endl;
				cout << "topLevelReference->name = " << topLevelReference->name << endl;
				cout << "function currentReference->name = " << currentReference->name << endl;
				cout << "positionOfFunctionReference = " << positionOfFunctionReference << endl;
				//cout << "functionContentsString = " << functionContentsString << endl;
				//exit(0);
				*/
				/*
				for(int i=0; i<level; i++)
				{
					cout << "\t";
				}
				cout << "\tcurrentReference->name = " << currentReference->name << endl;
				*/

				CSReference * currentReferenceInFunctionReferenceList = currentReference->firstReferenceInFunctionReferenceList;
				//cout << "search current file for function references;" << endl;
				//cout << "firstReferenceInIncludeFileList->name = " << firstReferenceInIncludeFileList->name << endl;
				//search current file for function references;
				currentReferenceInFunctionReferenceList = searchFunctionStringForFunctionReferences(firstReferenceInIncludeFileList, topLevelReference, currentReferenceInFunctionReferenceList, functionContentsString);

				/*
				CSReference * temp1acurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
				while(temp1acurrentReferenceInFunctionList->next != NULL)
				{
					for(int i=0; i<level; i++)
					{
						cout << "\t";
					}
					cout << "\t\ttemp1acurrentReferenceInFunctionList->name = " << temp1acurrentReferenceInFunctionList->name << endl;
					temp1acurrentReferenceInFunctionList = temp1acurrentReferenceInFunctionList->next;
				}
				*/


				//cout << "search include files for function references;;" << endl;
				//cout << "firstReferenceInIncludeFileList->name = " << firstReferenceInIncludeFileList->name << endl;
				//search include files for function references;
				currentReferenceInFunctionReferenceList = searchFunctionStringForFunctionReferencesRecursive(firstReferenceInIncludeFileList, topLevelReference->firstReferenceInBelowList, currentReferenceInFunctionReferenceList, functionContentsString);
				//cout << "here" << endl;

				/*
				CSReference * temp1bcurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
				while(temp1bcurrentReferenceInFunctionList->next != NULL)
				{
					for(int i=0; i<level; i++)
					{
						cout << "\t";
					}
					cout << "\t\ttemp1bcurrentReferenceInFunctionList->name = " << temp1bcurrentReferenceInFunctionList->name << endl;
					temp1bcurrentReferenceInFunctionList = temp1bcurrentReferenceInFunctionList->next;
				}
				*/



			}
			else
			{
				cout << "error: function not found in file:" << endl;
				cout << "fullFunctionName = " << fullFunctionName << endl;
				cout << "fileName = " << codeFileName << endl;
				exit(0);
			}
			currentReference = currentReference->next;
		}
	}

}

CSReference * searchFunctionStringForFunctionReferencesRecursive(CSReference * firstReferenceInIncludeFileList, CSReference * firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSReference * firstReferenceInFunctionReferenceList, string functionContentsString)
{
	CSReference * currentReferenceInFunctionReferenceList = firstReferenceInFunctionReferenceList;
	CSReference * currentReference = firstFileNameInLayerContainingFunctionReferencesToSearchFor;

	while(currentReference->next != NULL)
	{
		currentReferenceInFunctionReferenceList = searchFunctionStringForFunctionReferences(firstReferenceInIncludeFileList, currentReference, currentReferenceInFunctionReferenceList, functionContentsString);

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			currentReferenceInFunctionReferenceList = searchFunctionStringForFunctionReferencesRecursive(firstReferenceInIncludeFileList, currentReference->firstReferenceInBelowList, currentReferenceInFunctionReferenceList, functionContentsString);
		}

		currentReference = currentReference->next;
	}

	return currentReferenceInFunctionReferenceList;
}

CSReference * searchFunctionStringForFunctionReferences(CSReference * firstReferenceInIncludeFileList, CSReference * fileNameContainingFunctionReferencesToSearchFor, CSReference * firstReferenceInFunctionReferenceList, string functionContentsString)
{
	//only search those file names which include topLevelReference.h

	CSReference * currentReferenceInFunctionReferenceList = firstReferenceInFunctionReferenceList;

	//find reference to this file

	//cout << "\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;

	//now search function string for all functions within referenceFound;
	//CSReference * currentFunction = foundReference->firstReferenceInFunctionList;
	CSReference * currentFunction = fileNameContainingFunctionReferencesToSearchFor->firstReferenceInFunctionList;

	while(currentFunction->next != NULL)
	{
		//cout << "\t\tcurrentFunction->name = " << currentFunction->name << endl;

		//now search the function string for this function reference;
		int indexToFunctionReference  = functionContentsString.find((currentFunction->name + '('));
		if(indexToFunctionReference != -1)
		{
			//function reference found, add it to the function reference list of the funciton;

			currentReferenceInFunctionReferenceList->name = currentFunction->name;
			currentReferenceInFunctionReferenceList->isFunctionReferenceReference = true;
			/*
			cout << "\t\tadding function reference;" << endl;
			cout << "\t\t\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;
			cout << "\t\t\t\tcurrentFunction->name = " << currentFunction->name << endl;
			cout << "\t\t\t\t\tcurrentReferenceInFunctionReferenceList->name = " << currentReferenceInFunctionReferenceList->name << endl;
			*/

			CSReference * newfirstReferenceInFunctionReferenceList = new CSReference();
			currentReferenceInFunctionReferenceList->next = newfirstReferenceInFunctionReferenceList;
			currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
		}

		currentFunction = currentFunction->next;
	}

	return currentReferenceInFunctionReferenceList;
}






























































CSReference * findReferenceInIncludeFileList(CSReference * firstReferenceInAboveLevelBelowList, string referenceName, CSReference * foundReference, bool * referenceFound)
{
	CSReference * foundReferenceNew = foundReference;
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;
	while(currentReference->next != NULL)
	{
		if(currentReference->name == referenceName)
		{
			*referenceFound = true;
			foundReferenceNew = currentReference;
		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			foundReferenceNew = findReferenceInIncludeFileList(currentReference->firstReferenceInBelowList, referenceName, foundReferenceNew, referenceFound);
		}

		currentReference = currentReference->next;
	}

	return foundReferenceNew;

}









