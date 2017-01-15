/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: CSoperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3d2b 14-April-2014
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








bool getIncludeFileNamesFromCorHfile(CSfileReference * firstReferenceInIncludeFileList, CSfileReference * topLevelReference, string topLevelReferenceName, int level)
{
	bool fileFound = false;

	string parseFileName = topLevelReferenceName;
	CSfileReference * previousReference = NULL;

	char * parseFileNamecharstar = const_cast<char*>(parseFileName.c_str());

	ifstream * parseFileObject = new ifstream(parseFileNamecharstar);
	CSfileReference * currentReferenceInIncludeFileList = firstReferenceInIncludeFileList;

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
						#ifdef CS_DEBUG
						//cout << "hashInclude = " << hashInclude << endl;
						#endif
						
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
						hashIncludeFileNameCFile[stringLength-1] = CS_SOURCE_FILE_EXTENSION_CHARACTER_1;
						hashIncludeFileNameCFile[stringLength+0] = CS_SOURCE_FILE_EXTENSION_CHARACTER_2;
						hashIncludeFileNameCFile[stringLength+1] = CS_SOURCE_FILE_EXTENSION_CHARACTER_3;
						hashIncludeFileNameCFile[stringLength+2] = '\0';

						currentReferenceInIncludeFileList->name = hashIncludeFileName;
						currentReferenceInIncludeFileList->id = id;
						currentReferenceInIncludeFileList->level = level;
						currentReferenceInIncludeFileList->firstReferenceInAboveList = topLevelReference;
						currentReferenceInIncludeFileList->previous = previousReference;
						id++;

						CSfileReference * newCSReference = new CSfileReference();
						currentReferenceInIncludeFileList->next = newCSReference;

						previousReference = currentReferenceInIncludeFileList;

						CSfileReference * newfirstReferenceInBelowList = new CSfileReference();
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
						bool hFileFound = getIncludeFileNamesFromCorHfile(currentReferenceInIncludeFileList->firstReferenceInBelowList, currentReferenceInIncludeFileList, referenceName, level+1);

						if(hFileFound)
						{
							//parse into .cpp file;
							CSfileReference * lastReferenceInBelowList = currentReferenceInIncludeFileList->firstReferenceInBelowList;
							while(lastReferenceInBelowList->next != NULL)
							{
								lastReferenceInBelowList = lastReferenceInBelowList->next;
							}
							string referenceNameCFile = hashIncludeFileNameCFile;
							bool cFileFound = getIncludeFileNamesFromCorHfile(lastReferenceInBelowList, currentReferenceInIncludeFileList, referenceNameCFile, level+1);

							if(cFileFound)
							{
								//fill function and function reference list using .h file;
								CSfunctionReference * newfirstReferenceInFunctionList = new CSfunctionReference();
								currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;

								bool hFileFound2 = getFunctionNamesFromFunctionDeclarationsInHfile(currentReferenceInIncludeFileList->firstReferenceInFunctionList, referenceName, level);
								if(hFileFound2)
								{
									#ifdef CS_DEBUG
									//cout << "currentReferenceInIncludeFileList->firstReferenceInFunctionList->name = " << currentReferenceInIncludeFileList->firstReferenceInFunctionList->name << endl;
									//cout << "topLevelReferenceName = " << topLevelReferenceName << endl;
									//cout << "getFunctionReferenceNamesFromFunctionsInCfile()" << endl;
									#endif
									getFunctionReferenceNamesFromFunctionsInCfile(firstReferenceInIncludeFileList, currentReferenceInIncludeFileList->firstReferenceInFunctionList, currentReferenceInIncludeFileList, referenceNameCFile, level);

								}
								else
								{
									#ifdef CS_DEBUG
									//cout << "!hFileFound2" << endl;
									#endif
								}
							}
							else
							{
								#ifdef CS_DEBUG
								//cout << "!cFileFound" << endl;
								#endif
								CSfunctionReference * newfirstReferenceInFunctionList = new CSfunctionReference();
								currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;
							}
						}
						else
						{
							#ifdef CS_DEBUG
							//cout << "!hFileFound" << endl;
							#endif
							CSfunctionReference * newfirstReferenceInFunctionList = new CSfunctionReference();
							currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;
						}

						currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;


					}
					else
					{
						#ifdef CS_DEBUG
						//cout << "hashIncludeFileNameString = " << hashIncludeFileNameString << endl;
						//cout << "topLevelReference->name = " << topLevelReference->name << endl;
						#endif
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
				cout << "getIncludeFileNamesFromCorHfile: token error" << endl;
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


bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunctionReference * firstReferenceInFunctionList, string topLevelFileName, int level)
{
	bool fileFound = false;

	string parseFileName = topLevelFileName;

	char * parseFileNamecharstar = const_cast<char*>(parseFileName.c_str());

	ifstream * parseFileObject = new ifstream(parseFileNamecharstar);
	CSfunctionReference * currentReferenceInFunctionList = firstReferenceInFunctionList;
	CSfunctionReference * currentReferenceInFunctionReferenceList = NULL;

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
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
		int functionReferenceIndentationInHfileTemp = 0;
		#endif		
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
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif						
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
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif						
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
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					if(c == CS_SOURCE_FILE_INDENTATION_CHARACTER)
					{
						functionReferenceIndentationInHfileTemp++;
					}
					#endif
					//ignore preceeding white space
				}
				else if(c == '\n')
				{			
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif					
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
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif						
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
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif						
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

						string functionNameCpp = functionName;
						#ifdef CS_SUPPORT_FUNCTION_RETURN_POINTERS
						if(functionNameCpp[0] == '*')
						{
							int functionNameLength = functionNameCpp.length();
							functionNameCpp = functionNameCpp.substr(1, functionNameLength-1);
						}
						#endif
						
						#ifdef CS_DEBUG
						/*
						//add function reference reference;
						for(int i= 0; i<level+1; i++)
						{
							cout << "\t";
						}
						cout << functionNameCpp << endl;
						cout << functionNameFull << endl;
						*/
						/*
						//add function reference reference;
						for(int i= 0; i<level+1; i++)
						{
							cout << "\t";
						}
						cout << topLevelFileName << ": " << functionNameCpp << endl;
						*/
						#endif

						currentReferenceInFunctionList->isFunctionReference = true;
						currentReferenceInFunctionList->name = functionNameCpp;
						currentReferenceInFunctionList->nameFull = functionNameFull;
						currentReferenceInFunctionList->functionReferenceIndentation = functionReferenceIndentationInHfileTemp;
						CSfunctionReference * newCSReference = new CSfunctionReference();
						currentReferenceInFunctionList->next = newCSReference;

						currentReferenceInFunctionList = currentReferenceInFunctionList->next;

					}
					else
					{
						#ifdef CS_DEBUG
						//cout << "invalid function reference" << endl;
						#endif
					}
	
					readingBeforeClosingBracket = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif					
					functionNameFull[0] = '\0';
				}
				else if(c == '\n')
				{					
					lineCount++;
					waitingForNewLine = false;
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = true;
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION	
					functionReferenceIndentationInHfileTemp = 0;
					#endif					
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
				cout << "getFunctionNamesFromFunctionDeclarationsInHfile: token error" << endl;
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

void getFunctionReferenceNamesFromFunctionsInCfile(CSfileReference * firstReferenceInIncludeFileList, CSfunctionReference * firstReferenceInFunctionList, CSfileReference * topLevelReference, string topLevelFileName, int level)
{
	string codeFileName = topLevelFileName;
	char * codeFileNamecharstar = const_cast<char*>(codeFileName.c_str());

	int codeFileByteArraySize;
	int codeFileNumberOfLines;

	string fileContentsString = "";

	if(!readFileIntoString(codeFileName, &fileContentsString, &codeFileNumberOfLines, &codeFileByteArraySize))
	{
		#ifdef CS_DEBUG
		//cout << "!readFileIntoString" << endl;
		#endif
	}
	else
	{
		CSfunctionReference * currentReference = firstReferenceInFunctionList;

		//0. for each fullfilename [identified in header];

		while(currentReference->next != NULL)
		{
			if(!(currentReference->hasHadFunctionReferencesParsed))
			{
				string fullFunctionName = currentReference->nameFull;

				//1. in cpp file, find fullfilename [identified in header];
				#ifdef CS_DEBUG
				//cout << "fileContentsString = " << fileContentsString << endl;
				//cout << "fullFunctionName = " << fullFunctionName << endl;
				#endif
				int positionOfFunctionReference = fileContentsString.find(fullFunctionName);

				if(positionOfFunctionReference != -1)
				{
					#define CS_MAX_NUM_CHARACTERS_PER_FUNCTION (1000000)
					
				#ifdef CS_IGNORE_COMMENTS_IN_FUNCTIONS
					//NEW method - comments are ignored - there is a problem with this and so it has been disabled at present...

					string functionContentsString = "";

					//2. create a string containing all text from this position to the closing bracket "\n}"
					bool stillFindingEndOfFunction = true;
					bool waitingForNewLine = false;
					bool readingLargeComment = false;
					int lineCount = 0;
					bool newLineFound = false;

					int currentIndexInFunction = positionOfFunctionReference + fullFunctionName.length();
					while(stillFindingEndOfFunction)
					{
						char c = fileContentsString[currentIndexInFunction];
						//cout << c;
						if(readingLargeComment)
						{
							if(c == '\n')
							{
							}
							else
							{
								while(c == '*')
								{
									currentIndexInFunction++;
									c = fileContentsString[currentIndexInFunction];
									//cout << c;
									if(c == '/')
									{
										readingLargeComment = false;
									}
									else if(c == '\n')
									{
									}
								}
							}

						}
						else if(c == '/')
						{
							currentIndexInFunction++;
							c = fileContentsString[currentIndexInFunction];
							//cout << c;
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
								waitingForNewLine = false;
							}
							else
							{
								//do nothing, still waiting for new line
							}
						}
						else if(newLineFound)
						{
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
						
						if(c == '\n')
						{
							if(!readingLargeComment)
							{
								newLineFound = true;
							}
							lineCount++;
						}
						else
						{
							newLineFound = false;
						}
							
						if(currentIndexInFunction > CS_MAX_NUM_CHARACTERS_PER_FUNCTION)
						{
							cout << "getFunctionReferenceNamesFromFunctionsInCfile() error: function definitions in .cpp files must end with a } without any leading white space" << endl;
							exit(0);
						}
												
						currentIndexInFunction++;

					}
					
					//cout << "functionContentsString = " << functionContentsString << endl;
				#else
					
					//OLD method - comments are not ignored;
					bool stillFindingEndOfFunction = true;
					bool newLineFound = false;

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
							cout << "getFunctionReferenceNamesFromFunctionsInCfile() error: function definitions in .cpp files must end with a } without any leading white space" << endl;
							exit(0);
						}

						currentIndexInFunction++;
					}
					
					int positionOfFunctionReferenceEnd = currentIndexInFunction;
					string functionContentsString = fileContentsString.substr(positionOfFunctionReference+fullFunctionName.length(), (positionOfFunctionReferenceEnd-positionOfFunctionReference)-fullFunctionName.length());
				#endif
					
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
					currentReference->functionText = functionContentsString;
					#endif


					//3. search this string for any of the function (not full function names) across all include/header files
					CSfunctionReference * newfirstReferenceInFunctionReferenceList = new CSfunctionReference();
					currentReference->firstReferenceInFunctionReferenceList = newfirstReferenceInFunctionReferenceList;

					#ifdef CS_DEBUG
					/*
					cout << "file topLevelFileName = " << topLevelFileName << endl;
					cout << "topLevelReference->name = " << topLevelReference->name << endl;
					cout << "function currentReference->name = " << currentReference->name << endl;
					cout << "positionOfFunctionReference = " << positionOfFunctionReference << endl;
					//cout << "functionContentsString = " << functionContentsString << endl;
					*/
					/*
					for(int i=0; i<level; i++)
					{
						cout << "\t";
					}
					cout << "\tcurrentReference->name = " << currentReference->name << endl;
					*/
					#endif

					CSfunctionReference * currentReferenceInFunctionReferenceList = currentReference->firstReferenceInFunctionReferenceList;
					//cout << "search current file for function references;" << endl;
					#ifdef CS_DEBUG
					//cout << "firstReferenceInIncludeFileList->name = " << firstReferenceInIncludeFileList->name << endl;
					#endif
					//search current file for function references;
					currentReferenceInFunctionReferenceList = searchFunctionStringForFunctionReferences(firstReferenceInIncludeFileList, topLevelReference, currentReferenceInFunctionReferenceList, functionContentsString);

					#ifdef CS_DEBUG
					/*
					CSfunctionReference * temp1acurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
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
					#endif


					//cout << "search include files for function references;;" << endl;
					#ifdef CS_DEBUG
					//cout << "firstReferenceInIncludeFileList->name = " << firstReferenceInIncludeFileList->name << endl;
					#endif
					//search include files for function references;
					currentReferenceInFunctionReferenceList = searchFunctionStringForFunctionReferencesRecursive(firstReferenceInIncludeFileList, topLevelReference->firstReferenceInBelowList, currentReferenceInFunctionReferenceList, functionContentsString);

					#ifdef CS_DEBUG
					/*
					CSfunctionReference * temp1bcurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
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
					#endif
				}
				else
				{
					cout << "error: function not found in file:" << endl;
					cout << "fullFunctionName = " << fullFunctionName << endl;
					cout << "fileName = " << codeFileName << endl;
					exit(0);
				}
				currentReference->hasHadFunctionReferencesParsed = true;	//not used
			}
			currentReference = currentReference->next;
		}
	}

}

CSfunctionReference * searchFunctionStringForFunctionReferencesRecursive(CSfileReference * firstReferenceInIncludeFileList, CSfileReference * firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunctionReference * firstReferenceInFunctionReferenceList, string functionContentsString)
{
	CSfunctionReference * currentReferenceInFunctionReferenceList = firstReferenceInFunctionReferenceList;
	CSfileReference * currentReference = firstFileNameInLayerContainingFunctionReferencesToSearchFor;

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

CSfunctionReference * searchFunctionStringForFunctionReferences(CSfileReference * firstReferenceInIncludeFileList, CSfileReference * fileNameContainingFunctionReferencesToSearchFor, CSfunctionReference * firstReferenceInFunctionReferenceList, string functionContentsString)
{
	//only search those file names which include topLevelReference.h

	CSfunctionReference * currentReferenceInFunctionReferenceList = firstReferenceInFunctionReferenceList;

	//find reference to this file

	#ifdef CS_DEBUG
	//cout << "\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;
	#endif
	
	//now search function string for all functions within referenceFound;
	//CSfunctionReference * currentFunction = foundReference->firstReferenceInFunctionList;
	CSfunctionReference * currentFunction = fileNameContainingFunctionReferencesToSearchFor->firstReferenceInFunctionList;

	while(currentFunction->next != NULL)
	{
		#ifdef CS_DEBUG
		//cout << "\t\tcurrentFunction->name = " << currentFunction->name << endl;
		#endif
		
		//now search the function string for this function reference;
		int indexToFunctionReference  = functionContentsString.find((currentFunction->name + '('));
		if(indexToFunctionReference != -1)
		{
			#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
			int indexOfFunctionReferenceStartOfLine = functionContentsString.rfind("\n", indexToFunctionReference);
			string functionReferenceWholeLine = functionContentsString.substr(indexOfFunctionReferenceStartOfLine, indexToFunctionReference-indexOfFunctionReferenceStartOfLine);
			int functionReferenceIndentationInCfileTemp = 0;
			
			for(int i=0; i<functionReferenceWholeLine.length(); i++)
			{
				if(functionReferenceWholeLine[i] == CS_SOURCE_FILE_INDENTATION_CHARACTER)
				{
					functionReferenceIndentationInCfileTemp++;
				}
			}
			currentReferenceInFunctionReferenceList->functionReferenceIndentation = functionReferenceIndentationInCfileTemp;
			#endif

			//function reference found, add it to the function reference list of the funciton;

			currentReferenceInFunctionReferenceList->name = currentFunction->name;
			currentReferenceInFunctionReferenceList->isFunctionReferenceReference = true;
			#ifdef CS_DEBUG
			/*
			cout << "\t\tadding function reference;" << endl;
			cout << "\t\t\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;
			cout << "\t\t\t\tcurrentFunction->name = " << currentFunction->name << endl;
			cout << "\t\t\t\t\tcurrentReferenceInFunctionReferenceList->name = " << currentReferenceInFunctionReferenceList->name << endl;
			*/
			#endif

			CSfunctionReference * newfirstReferenceInFunctionReferenceList = new CSfunctionReference();
			currentReferenceInFunctionReferenceList->next = newfirstReferenceInFunctionReferenceList;
			currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
		}

		currentFunction = currentFunction->next;
	}

	return currentReferenceInFunctionReferenceList;
}






























































CSfileReference * findReferenceInIncludeFileList(CSfileReference * firstReferenceInAboveLevelBelowList, string referenceName, CSfileReference * foundReference, bool * referenceFound)
{
	CSfileReference * foundReferenceNew = foundReference;
	CSfileReference * currentReference = firstReferenceInAboveLevelBelowList;
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









