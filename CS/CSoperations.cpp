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
 * File Name: CSoperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h5b 03-December-2015
 *
 *******************************************************************************/


#include "CSoperations.h"
#include "LDsprite.h"
#include "LDreferenceManipulation.h"




bool getIncludeFileNamesFromCorHfile(CSfileContainer* firstReferenceInIncludeFileListContainer, CSfileContainer* topLevelReferenceContainer, CSfile* aboveLevelObject, string topLevelReferenceName, int level)
{
	bool fileFound = false;

	string parseFileName = topLevelReferenceName;

	ifstream parseFileObject(parseFileName.c_str());
	CSfileContainer* currentReferenceInIncludeFileListContainer = firstReferenceInIncludeFileListContainer;

	if(!parseFileObject.rdbuf()->is_open())
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
		string hashInclude = "";
		string hashIncludeFileName = "";
		//bool readingLineComment = false;	//same as waitingForNewLine
		bool readingLargeComment = false;
		
		/*//this implementation will use up too much memory
		#ifdef CS_GENERATE_CODE_GENERIC
		string fileContentsString = "";
		#endif
		*/
		
		while(parseFileObject.get(c))
		{
			#ifdef CS_DEBUG_VERBOSE
			//cout << c;
			#endif
			
			/*//this implementation will use up too much memory
			#ifdef CS_GENERATE_CODE_GENERIC
			fileContentsString = fileContentsString + c;
			#endif
			*/
			
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
						parseFileObject.get(c);
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
				parseFileObject.get(tempChar);
				if(tempChar == '*')
				{
					readingLargeComment = true;
				}
				else if(tempChar == '/')
				{
					//single line comment found
					waitingForNewLine = true;

				}
				else if(tempChar == '\n')
				{
					lineCount++;
					waitingForNewLine = false;
					readingHashIncludePreceedingWhiteSpace = true;
					readingHashInclude = false;
					readingHashIncludeFileName = false;
					hashInclude = "";
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
					hashInclude = "";
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
					hashInclude = "";
				}
				else
				{
					readingHashIncludePreceedingWhiteSpace = false;
					readingHashInclude = true;

					//add this character to first character
					hashInclude = hashInclude + c;
				}
			}
			else if(readingHashInclude)
			{
				if(c == ' ')
				{
					readingHashIncludePreceedingWhiteSpace = false;

					if(hashInclude == "#include")
					{
						#ifdef CS_DEBUG
						//cout << "hashInclude = " << hashInclude << endl;
						#endif

						char tempChar;
						parseFileObject.get(tempChar);
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
					hashInclude = "";
				}
				else
				{
					hashInclude = hashInclude + c;
				}

			}
			else if(readingHashIncludeFileName)
			{
				if(c == '\"')
				{
					if(hashIncludeFileName != aboveLevelObject->name)	//this is added so that do not parse method.h from within method.cpp!
					{
						bool alreadyParsedFile = false;
						CSfile* currentReferenceInIncludeFileList = NULL;
						if(findFileObjectInFileObjectContainerList(topLevelReferenceContainer->fileObject->firstFileInBelowListContainer, hashIncludeFileName, &currentReferenceInIncludeFileList))
						{
							currentReferenceInIncludeFileListContainer->fileObject = currentReferenceInIncludeFileList;
							
							CSfileContainer* newCSReferenceContainer = new CSfileContainer();
							newCSReferenceContainer->previous = currentReferenceInIncludeFileListContainer;
							currentReferenceInIncludeFileListContainer->next = newCSReferenceContainer;
						}
						else
						{
							currentReferenceInIncludeFileList = new CSfile();
							currentReferenceInIncludeFileListContainer->fileObject = currentReferenceInIncludeFileList;
							currentReferenceInIncludeFileList->name = hashIncludeFileName;
							string hashIncludeFileNameCFile = generateSourceFileNameFromHeaderFileName(hashIncludeFileName, CS_SOURCE_FILE_EXTENSION);
							#ifdef CS_GENERATE_CODE_GENERIC
							currentReferenceInIncludeFileList->sourceFileNameOrig = hashIncludeFileNameCFile;
							currentReferenceInIncludeFileList->headerFileName = generateSourceFileNameFromHeaderFileName(currentReferenceInIncludeFileList->name, CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION);
							currentReferenceInIncludeFileList->sourceFileName = generateSourceFileNameFromHeaderFileName(currentReferenceInIncludeFileList->sourceFileNameOrig, CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION);
							#endif
							currentReferenceInIncludeFileList->id = id;
							currentReferenceInIncludeFileList->level = level;
							currentReferenceInIncludeFileListContainer->aboveLevelObject = aboveLevelObject;
							id++;

							CSfileContainer* newCSReferenceContainer = new CSfileContainer();
							newCSReferenceContainer->previous = currentReferenceInIncludeFileListContainer;
							currentReferenceInIncludeFileListContainer->next = newCSReferenceContainer;

							CSfileContainer* newfirstReferenceInBelowListContainer = new CSfileContainer();
							currentReferenceInIncludeFileList->firstFileInBelowListContainer = newfirstReferenceInBelowListContainer;

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
							string referenceName = hashIncludeFileName;	//hashIncludeFileName is the current filename being parsed by this instance of getIncludeFileNamesFromCorHfile, ie "x.h" in "#include x.h" [and will recurse]
							//getIncludeFileNamesFromCorHfile{}: this will add the include file references (filenames) within the .h file in "#include x.h" to currentReferenceInIncludeFileList->firstFileInBelowListContainer
							bool hFileFound = getIncludeFileNamesFromCorHfile(currentReferenceInIncludeFileList->firstFileInBelowListContainer, topLevelReferenceContainer, currentReferenceInIncludeFileList, referenceName, level+1);


							if(hFileFound)
							{
								//parse into .cpp file;
								CSfileContainer* lastReferenceInBelowListContainer = currentReferenceInIncludeFileList->firstFileInBelowListContainer;
								while(lastReferenceInBelowListContainer->next != NULL)
								{
									lastReferenceInBelowListContainer = lastReferenceInBelowListContainer->next;
								}
								string referenceNameCFile = hashIncludeFileNameCFile;
								//getIncludeFileNamesFromCorHfile{}: this will add (append) the include file references (filenames) within the .c equivalent (ie x.c) of the .h file in "#include x.h" to currentReferenceInIncludeFileList->firstFileInBelowListContainer [and will recurse]
								bool cFileFound = getIncludeFileNamesFromCorHfile(lastReferenceInBelowListContainer, topLevelReferenceContainer, currentReferenceInIncludeFileList, referenceNameCFile, level+1);

								if(cFileFound)
								{
									//parse the new file reference only if both its ".h" and ".c" file are found

									//fill function and function reference list using .h file;
									CSfunction* newfirstReferenceInFunctionList = new CSfunction();
									currentReferenceInIncludeFileList->firstFunctionInFunctionList = newfirstReferenceInFunctionList;

									//getFunctionNamesFromFunctionDeclarationsInHfile{}: this opens the .h file hashIncludeFileName and extracts its function declarations (hashIncludeFileName is the current filename being parsed by this instance of getIncludeFileNamesFromCorHfile, ie "x.h" in "#include x.h")
									bool hFileFound2 = getFunctionNamesFromFunctionDeclarationsInHfile(currentReferenceInIncludeFileList->firstFunctionInFunctionList, referenceName, level);
									if(hFileFound2)
									{
										#ifdef CS_DEBUG
										//cout << "currentReferenceInIncludeFileList->firstFunctionInFunctionList->name = " << currentReferenceInIncludeFileList->firstFunctionInFunctionList->name << endl;
										//cout << "topLevelReferenceName = " << topLevelReferenceName << endl;
										//cout << "getFunctionObjectNamesFromFunctionsInCfile{}" << endl;
										#endif
										//getFunctionObjectNamesFromFunctionsInCfile{}: this opens the .c file (hashIncludeFileName equivalent) and locates all the functions corresponding to those declared in its .h file
										getFunctionObjectNamesFromFunctionsInCfile(firstReferenceInIncludeFileListContainer->fileObject, currentReferenceInIncludeFileList->firstFunctionInFunctionList, currentReferenceInIncludeFileList, referenceNameCFile, level);

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
									CSfunction* newfirstReferenceInFunctionList = new CSfunction();
									currentReferenceInIncludeFileList->firstFunctionInFunctionList = newfirstReferenceInFunctionList;
								}
							}
							else
							{
								#ifdef CS_DEBUG
								//cout << "!hFileFound" << endl;
								#endif
								CSfunction* newfirstReferenceInFunctionList = new CSfunction();
								currentReferenceInIncludeFileList->firstFunctionInFunctionList = newfirstReferenceInFunctionList;
							}
						}

						currentReferenceInIncludeFileListContainer = currentReferenceInIncludeFileListContainer->next;


					}
					else
					{
						#ifdef CS_DEBUG
						//cout << "hashIncludeFileName = " << hashIncludeFileName << endl;
						//cout << "aboveLevelObject->name = " << aboveLevelObject->name << endl;
						#endif
					}

					hashIncludeFileName = "";

					lineCount++;
					readingHashIncludeFileName = false;
					waitingForNewLine = true;
				}
				else
				{
					hashIncludeFileName = hashIncludeFileName + c;
				}
			}
			else
			{
				cout << "getIncludeFileNamesFromCorHfile: token error" << endl;
				cout << "char = " << c << endl;
				cout << "parseFileName = " << parseFileName << endl;
				cout << "lineCount = " << lineCount << endl;
			}
		}
		parseFileObject.close();
		
		/*//this implementation will use up too much memory
		#ifdef CS_GENERATE_CODE_GENERIC
		if(isHeader)
		{
			aboveLevelObject->headerFileText = fileContentsString;
		}
		else
		{
			aboveLevelObject->sourceFileText = fileContentsString;
		}
		#endif
		*/
	}

	return fileFound;
}


bool findFileObjectInFileObjectContainerList(CSfileContainer* firstReferenceContainerInLevel, string fileReferenceName, CSfile** fileReferenceFound)
{
	bool foundFileObject = false;
	CSfileContainer* currentReferenceContainerInLevel = firstReferenceContainerInLevel;
	while(currentReferenceContainerInLevel->next != NULL)
	{
		if(currentReferenceContainerInLevel->fileObject->name == fileReferenceName)
		{
			foundFileObject = true;
			*fileReferenceFound = currentReferenceContainerInLevel->fileObject;
		}
		if(!foundFileObject)
		{
			if(currentReferenceContainerInLevel->fileObject->firstFileInBelowListContainer != NULL)
			{
				if(findFileObjectInFileObjectContainerList(currentReferenceContainerInLevel->fileObject->firstFileInBelowListContainer, fileReferenceName, fileReferenceFound))
				{
					foundFileObject = true;
				}
			}
		}
		currentReferenceContainerInLevel = currentReferenceContainerInLevel->next;
	}
	return foundFileObject;
}


bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunction* firstFunctionInFunctionList, string topLevelFileName, int level)
{
	bool fileFound = false;

	string parseFileName = topLevelFileName;

	ifstream parseFileObject(parseFileName.c_str());
	CSfunction* currentReferenceInFunctionList = firstFunctionInFunctionList;
	CSfunction* currentReferenceInFunctionReferenceList = NULL;

	if(!parseFileObject.rdbuf()->is_open())
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

		string functionName = "";
		string functionNameFull = "";

		while(parseFileObject.get(c))
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
						parseFileObject.get(c);
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
				parseFileObject.get(tempChar);
				if(tempChar == '*')
				{
					readingLargeComment = true;
				}
				else if(tempChar == '/')
				{
					//single line comment found
					waitingForNewLine = true;

				}
				else if(tempChar == '\n')
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
					functionName = "";
					functionNameFull = "";
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
					functionName = "";
					functionNameFull = "";
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
					functionName = "" ;
					functionNameFull = "";
				}
				else
				{
					//found start of function declaration
					readingBeforeSpaceBetweenFunctionTypeAndNamePreceedingWhiteSpace = false;
					readingBeforeSpaceBetweenFunctionTypeAndName = true;

					functionNameFull = functionNameFull + c;
				}
			}
			else if(readingBeforeSpaceBetweenFunctionTypeAndName)
			{
				if(c == ' ')
				{
					readingBeforeSpaceBetweenFunctionTypeAndName = false;

					functionNameFull = functionNameFull + c;

					//restart function name fill;
					functionName = "";
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
					functionName = "";
					functionNameFull = "";
				}
				else
				{
					functionNameFull = functionNameFull + c;
				}

			}
			else if(readingBeforeOpeningBracket)
			{
				if(c == ' ')
				{
					functionNameFull = functionNameFull + c;

					//not a function, restart function name fill;
					functionName = "";
				}
				else if(c == '(')
				{
					functionNameFull = functionNameFull + c;

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
					functionName = "";
					functionNameFull = "";
				}
				else
				{
					functionName = functionName + c;
					functionNameFull = functionNameFull + c;
				}
			}
			else if(readingBeforeClosingBracket)
			{
				if(c == ')')
				{
					functionNameFull = functionNameFull + c;

					char newC;
					parseFileObject.get(newC);
					if(newC == ';')
					{//function reference found

						#ifdef CS_SUPPORT_FUNCTION_RETURN_POINTERS
						if(functionName[0] == '*')
						{
							int functionNameLength = functionName.length();
							functionName = functionName.substr(1, functionNameLength-1);
						}
						#endif

						#ifdef CS_DEBUG
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
						#endif
						
						/*
						if(topLevelFileName == "LDreferenceManipulation.h")
						{
							cout << "functionName = " << functionName << endl;
						}
						*/
						
						currentReferenceInFunctionList->isFunction = true;
						currentReferenceInFunctionList->name = functionName;
						currentReferenceInFunctionList->nameFull = functionNameFull;
						#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
						identifyFunctionDeclarationArguments(currentReferenceInFunctionList, &functionNameFull);
						#endif
						#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
						currentReferenceInFunctionList->functionReferenceIndentation = functionReferenceIndentationInHfileTemp;
						#endif
						CSfunction* newCSReference = new CSfunction();
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
					functionNameFull = "";
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
					functionName = "";
					functionNameFull = "";
				}
				else
				{
					functionNameFull = functionNameFull + c;
				}
			}
			else
			{
				cout << "getFunctionNamesFromFunctionDeclarationsInHfile: token error" << endl;
				cout << "char = " << c << endl;
				cout << "parseFileName = " << parseFileName << endl;
				cout << "lineCount = " << lineCount << endl;
			}
		}
		parseFileObject.close();
	}

	return fileFound;
}



#define MAX_CODE_FILE_SIZE (1000000)

void getFunctionObjectNamesFromFunctionsInCfile(CSfile* firstFileInIncludeFileList, CSfunction* firstFunctionInFunctionList, CSfile* aboveLevelObject, string topLevelFileName, int level)
{
	string codeFileName = topLevelFileName;
	char* codeFileNamecharstar = const_cast<char*>(codeFileName.c_str());

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
		CSfunction* currentReference = firstFunctionInFunctionList;

		//0. for each fullfilename [declared in the header file];

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
				int positionOfFunctionObject = fileContentsString.find(fullFunctionName);

				if(positionOfFunctionObject != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					#define CS_MAX_NUM_CHARACTERS_PER_FUNCTION (1000000)

				#ifdef CS_IGNORE_COMMENTS_IN_FUNCTIONS
					//NEW method - comments are ignored

					string functionContentsString = "";
					
					//2. create a string containing all text from this position to the closing bracket "\n}"
					bool stillFindingEndOfFunction = true;
					bool waitingForNewLine = false;
					bool readingLargeComment = false;
					int lineCount = 0;
					bool newLineFound = false;

					int currentIndexInFunction = positionOfFunctionObject + fullFunctionName.length();
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
									functionContentsString = functionContentsString + c;	//added CS 3e5a - new lines are required to be stored to recalculate indentation
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
								functionContentsString = functionContentsString + c;	//added CS 3e5a - new lines are required to be stored to recalculate indentation
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
							cout << "getFunctionObjectNamesFromFunctionsInCfile{} error: function definitions in .cpp files must end with a } without any leading white space" << endl;
							exit(0);
						}

						currentIndexInFunction++;

					}

					//cout << "functionContentsString = " << functionContentsString << endl;
				#else

					//OLD method - comments are not ignored;
					bool stillFindingEndOfFunction = true;
					bool newLineFound = false;

					int currentIndexInFunction = positionOfFunctionObject;
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
							cout << "getFunctionObjectNamesFromFunctionsInCfile{} error: function definitions in .cpp files must end with a } without any leading white space" << endl;
							exit(0);
						}

						currentIndexInFunction++;
					}

					int positionOfFunctionObjectEnd = currentIndexInFunction;
					string functionContentsString = fileContentsString.substr(positionOfFunctionObject+fullFunctionName.length(), (positionOfFunctionObjectEnd-positionOfFunctionObject)-fullFunctionName.length());
				#endif
					#ifdef CS_GENERATE_CODE_GENERIC
					int positionOfFunctionObjectEnd = currentIndexInFunction;
					string functionContentsStringRaw = fileContentsString.substr(positionOfFunctionObject+fullFunctionName.length(), (positionOfFunctionObjectEnd-positionOfFunctionObject)-fullFunctionName.length());
					currentReference->functionTextRaw = functionContentsStringRaw;
					functionContentsStringRaw = "";
					//cout << "functionContentsStringRaw = " << functionContentsStringRaw << endl;
					#endif
					
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
					currentReference->functionText = functionContentsString;
					#endif


					//3. search the functionContentsString for any of the function (not full function names) across all include/header files
					CSfunction* newfirstReferenceInFunctionObjectList = new CSfunction();
					currentReference->firstReferenceInFunctionReferenceList = newfirstReferenceInFunctionObjectList;
					CSfunction* currentReferenceInFunctionReferenceList = currentReference->firstReferenceInFunctionReferenceList;
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
					CSfunction* newfirstReferenceInFunctionObjectListRepeats = new CSfunction();
					currentReference->firstReferenceInFunctionReferenceListRepeats = newfirstReferenceInFunctionObjectListRepeats;
					CSfunction* currentReferenceInFunctionReferenceListRepeats = currentReference->firstReferenceInFunctionReferenceListRepeats;
					#endif
					
					#ifdef CS_DEBUG
					/*
					cout << "file topLevelFileName = " << topLevelFileName << endl;
					cout << "aboveLevelObject->name = " << aboveLevelObject->name << endl;
					cout << "function currentReference->name = " << currentReference->name << endl;
					cout << "positionOfFunctionObject = " << positionOfFunctionObject << endl;
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

					//cout << "search current file for function references;" << endl;
					#ifdef CS_DEBUG
					//cout << "firstFileInIncludeFileList->name = " << firstFileInIncludeFileList->name << endl;
					#endif
					//search current file for function references;
					if(!searchFunctionStringForFunctionReferences(firstFileInIncludeFileList, aboveLevelObject, &currentReferenceInFunctionReferenceList, &currentReferenceInFunctionReferenceListRepeats, &functionContentsString))
					{
					
					}
					
					#ifdef CS_DEBUG
					/*
					CSfunction* temp1acurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
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
					//cout << "firstFileInIncludeFileList->name = " << firstFileInIncludeFileList->name << endl;
					#endif
					//search include files for function references;
					if(!searchFunctionStringForFunctionReferencesRecursive(firstFileInIncludeFileList, aboveLevelObject->firstFileInBelowListContainer, &currentReferenceInFunctionReferenceList, &currentReferenceInFunctionReferenceListRepeats, &functionContentsString))
					{
					
					}
					
					#ifdef CS_DEBUG
					/*
					CSfunction* temp1bcurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
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
	
	/*
	#ifdef CS_GENERATE_CODE_GENERIC
	aboveLevelObject->sourceFileText = fileContentsString;
	#endif
	*/
}

bool searchFunctionStringForFunctionReferencesRecursive(CSfile* firstFileInIncludeFileList, CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString)
{
	bool result = true;
	
	CSfileContainer* currentReferenceContainer = firstFileNameInLayerContainingFunctionReferencesToSearchFor;

	while(currentReferenceContainer->next != NULL)
	{
		CSfile* currentReference = currentReferenceContainer->fileObject;
		
		searchFunctionStringForFunctionReferences(firstFileInIncludeFileList, currentReference, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString);

		if(currentReference->firstFileInBelowListContainer != NULL)
		{
			if(!searchFunctionStringForFunctionReferencesRecursive(firstFileInIncludeFileList, currentReference->firstFileInBelowListContainer, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
			{
				result = false;
			}		
		}

		currentReferenceContainer = currentReferenceContainer->next;
	}
	
	return result;
	
}

bool searchFunctionStringForFunctionReferences(CSfile* firstFileInIncludeFileList, CSfile* fileNameContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString)
{
	bool result = true;
	
	//only search those file names which include aboveLevelObject.h

	//find reference to this file

	#ifdef CS_DEBUG
	//cout << "\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;
	#endif

	//now search function string for all functions within referenceFound;
	//CSfunction* currentFunction = foundReference->firstFunctionInFunctionList;
	CSfunction* currentFunction = fileNameContainingFunctionReferencesToSearchFor->firstFunctionInFunctionList;

	while(currentFunction->next != NULL)
	{
		#ifdef CS_DEBUG
		//cout << "\t\tcurrentFunction->name = " << currentFunction->name << endl;
		#endif

		//now search the function string for this function reference;
		
		int startPosOfFunctionReferenceInFunction = 0;
		bool firstTimeFound = true;
		//cout << "function->functionText = " << function->functionText << endl;
		while(CPP_STRING_FIND_RESULT_FAIL_VALUE != (startPosOfFunctionReferenceInFunction = functionContentsString->find((currentFunction->name + CHAR_OPEN_BRACKET), startPosOfFunctionReferenceInFunction)))
		{
			//added condition CS 3f1b - ensure previous character is not a letter (this ensures that ABCfunctionName is not found when searching for functionName)
			if((startPosOfFunctionReferenceInFunction == 0) || !charInCharArray((*functionContentsString)[startPosOfFunctionReferenceInFunction-1], functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
			{
				//cout << "startPosOfFunctionReferenceInFunction = " << startPosOfFunctionReferenceInFunction << endl;

				//function reference found, add it to the function reference list of the funciton;
				#ifdef CS_DEBUG
				/*
				cout << "\t\tadding function reference;" << endl;
				cout << "\t\t\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;
				cout << "\t\t\t\tcurrentFunction->name = " << currentFunction->name << endl;
				cout << "\t\t\t\t\tcurrentReferenceInFunctionReferenceList->name = " << currentReferenceInFunctionReferenceList->name << endl;
				*/
				#endif

				if(firstTimeFound)
				{
					(*currentReferenceInFunctionReferenceList)->name = currentFunction->name;
					(*currentReferenceInFunctionReferenceList)->isFunctionReference = true;
					#ifdef CS_MATCH_FUNCTION_REFERENCES_WITH_CORRECT_NUMBER_OF_ARGUMENTS
					identifyFunctionReferenceArguments((*currentReferenceInFunctionReferenceList), functionContentsString, startPosOfFunctionReferenceInFunction);
					#endif
					
					CSfunction* newfirstReferenceInFunctionReferenceList = new CSfunction();
					(*currentReferenceInFunctionReferenceList)->next = newfirstReferenceInFunctionReferenceList;
					(*currentReferenceInFunctionReferenceList) = (*currentReferenceInFunctionReferenceList)->next;

					firstTimeFound = false;
				}

				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
				(*currentReferenceInFunctionReferenceListRepeats)->name = currentFunction->name;
				(*currentReferenceInFunctionReferenceListRepeats)->isFunctionReference = true;

				int indexToFunctionReference = startPosOfFunctionReferenceInFunction;
				int indexOfFunctionReferenceStartOfLine = functionContentsString->rfind("\n", indexToFunctionReference);
				string functionReferenceStartLine = functionContentsString->substr(indexOfFunctionReferenceStartOfLine, indexToFunctionReference-indexOfFunctionReferenceStartOfLine);
				int functionReferenceIndentationInCfileTemp = 0;
				for(int i=0; i<functionReferenceStartLine.length(); i++)
				{
					if(functionReferenceStartLine[i] == CS_SOURCE_FILE_INDENTATION_CHARACTER)
					{
						functionReferenceIndentationInCfileTemp++;
					}
				}			
				(*currentReferenceInFunctionReferenceListRepeats)->functionReferenceIndentation = functionReferenceIndentationInCfileTemp;
				(*currentReferenceInFunctionReferenceListRepeats)->functionReferenceCharacterIndex = indexToFunctionReference;

				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
				identifyFunctionReferenceArguments((*currentReferenceInFunctionReferenceListRepeats), functionContentsString, startPosOfFunctionReferenceInFunction);
				#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
				cout << "(*currentReferenceInFunctionReferenceListRepeats)->nameFull = " << (*currentReferenceInFunctionReferenceListRepeats)->nameFull << endl;
				#endif
				#endif	
				
				CSfunction* newfirstReferenceInFunctionReferenceListRepeats = new CSfunction();
				(*currentReferenceInFunctionReferenceListRepeats)->next = newfirstReferenceInFunctionReferenceListRepeats;
				(*currentReferenceInFunctionReferenceListRepeats) = (*currentReferenceInFunctionReferenceListRepeats)->next;	
				#endif	

				startPosOfFunctionReferenceInFunction++;
			}
			else
			{
				startPosOfFunctionReferenceInFunction++;
			}
		}

		currentFunction = currentFunction->next;
	}

	return result;
}


//limitations; doesn't support couts containing function references, e.g. "cout << "Error: getFloatArgument(" << keystr << ")" << endl;" (NB '<' is interpreted as CLASS_TYPE_OPEN_TAG)
void identifyFunctionReferenceArguments(CSfunction* currentReferenceInFunctionReferenceListRepeats, string* functionContentsString, int indexToFunctionObject)
{
	//designed to support embedded function references (although this is not currently used by CSgenerateConstFunctionArgumentCode)

	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "start identifyFunctionReferenceArguments{}..." << endl;
	cout << "currentReferenceInFunctionReferenceListRepeats->name = " << currentReferenceInFunctionReferenceListRepeats->name << endl;
	//cout << "functionContentsString = " << *functionContentsString << endl;
	#endif
	
	CSfunctionArgument* currentFunctionArgumentInFunctionReference = currentReferenceInFunctionReferenceListRepeats->firstFunctionArgumentInFunction;
	
	string functionName = currentReferenceInFunctionReferenceListRepeats->name;
	
	int startPositionOfFunctionBrackets = functionContentsString->find(CHAR_OPEN_BRACKET, indexToFunctionObject);
	int endPositionOfFunctionBracketsTemp = functionContentsString->find(CHAR_CLOSE_BRACKET, indexToFunctionObject);
	if(startPositionOfFunctionBrackets != indexToFunctionObject+functionName.length())
	{
		cout << "identifyFunctionReferenceArguments{} error: startPositionOfFunctionBrackets != indexToFunctionObject+functionName.length()" << endl;
		exit(0);
	}
	bool functionHasArguments = false;
	if(endPositionOfFunctionBracketsTemp != startPositionOfFunctionBrackets+1)
	{
		functionHasArguments = true;
	}
	if(functionHasArguments)
	{
		int bracketLevel = 1;
		int pos = startPositionOfFunctionBrackets+1;
		int posStartOfFunctionArgument = pos;
		char cPrevious = 'a';
		while(bracketLevel > 0)
		{
			char c = (*functionContentsString)[pos];
			if(bracketLevel == 1)
			{//only detect base level function reference commas (not commas for embedded function reference arguments)
				if((c == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_ARGUMENT_DELIMITER) || (c == CHAR_CLOSE_BRACKET))
				{
					string argument = functionContentsString->substr(posStartOfFunctionArgument, pos-posStartOfFunctionArgument);
					currentFunctionArgumentInFunctionReference->argument = removePrependedWhiteSpace(argument);
					#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
					cout << "identifyFunctionReferenceArguments{}: currentFunctionArgumentInFunctionReference->argument = " << currentFunctionArgumentInFunctionReference->argument << endl;
					#endif
					currentFunctionArgumentInFunctionReference->next = new CSfunctionArgument();
					currentFunctionArgumentInFunctionReference = currentFunctionArgumentInFunctionReference->next;
					posStartOfFunctionArgument = pos+1;	//account for comma
				}
			}
			if(c == CHAR_OPEN_BRACKET)
			{
				bracketLevel++;
			}
			else if(c == CHAR_CLOSE_BRACKET)
			{
				bracketLevel--;
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
		currentReferenceInFunctionReferenceListRepeats->nameFull = functionContentsString->substr(indexToFunctionObject, pos-indexToFunctionObject);
	}
	else
	{
		currentReferenceInFunctionReferenceListRepeats->nameFull = functionContentsString->substr(indexToFunctionObject, endPositionOfFunctionBracketsTemp-indexToFunctionObject+1);
	}
	#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
	cout << "identifyFunctionReferenceArguments{}: currentReferenceInFunctionReferenceListRepeats->nameFull = " << currentReferenceInFunctionReferenceListRepeats->nameFull << endl;
	cout << "end identifyFunctionReferenceArguments{}..." << endl;
	//exit(0);
	#endif
}

#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
//NB this function is not necessary for the current implementation, but is used for neatness 
string removePrependedWhiteSpace(string argument)
{
	string argumentWithoutPrependedWhitespace = "";
	bool findingWhitespace = true;
	for(int i=0; i<argument.length(); i++)
	{
		if(findingWhitespace)
		{
			if(argument[i] == CHAR_SPACE)
			{

			}
			else
			{
				findingWhitespace = false;
				argumentWithoutPrependedWhitespace = argumentWithoutPrependedWhitespace + argument[i];
			}
		}
		else
		{
			argumentWithoutPrependedWhitespace = argumentWithoutPrependedWhitespace + argument[i];
		}
	}
	return argumentWithoutPrependedWhitespace;
}

void identifyFunctionDeclarationArguments(CSfunction* currentReferenceInFunctionReferenceList, string* functionNameFull)
{	
	CSfunctionArgument* currentFunctionArgumentInFunctionReference = currentReferenceInFunctionReferenceList->firstFunctionArgumentInFunction;
						 	
	int startPositionOfFunctionBrackets = functionNameFull->find(CHAR_OPEN_BRACKET);
	int endPositionOfFunctionBrackets = functionNameFull->find(CHAR_CLOSE_BRACKET);
	bool functionHasArguments = false;
	if(startPositionOfFunctionBrackets != endPositionOfFunctionBrackets-1)
	{
		functionHasArguments = true;
	}
	if(functionHasArguments)
	{
		#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
		cout << "functionHasArguments: functionNameFull = " <<* functionNameFull << endl;
		#endif

		int startPositionOfFunctionArguments = startPositionOfFunctionBrackets+1;
		int endPositionOfFunctionArguments = endPositionOfFunctionBrackets;
		string functionArgumentsRaw = functionNameFull->substr(startPositionOfFunctionArguments, endPositionOfFunctionArguments-startPositionOfFunctionArguments+1);

		bool stillParsingArguments = true;
		int startPositionOfArgument = 0;
		while(stillParsingArguments)
		{
			bool lastArgument = false;
			int endPositionOfArgument = findEndPositionOfArgument(&functionArgumentsRaw, startPositionOfArgument, &lastArgument);
			/*
			cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
			cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
			cout << "functionArgumentsRaw = " << functionArgumentsRaw << endl;
			*/
			if(lastArgument)
			{
				stillParsingArguments = false;
			}

			string currentArgument = functionArgumentsRaw.substr(startPositionOfArgument, endPositionOfArgument-startPositionOfArgument);
			currentArgument = removePrependedWhiteSpace(currentArgument);

			int startPositionOfArgumentName = currentArgument.rfind(CHAR_SPACE) + 1;	//last space
			if(startPositionOfArgumentName == CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				cout << "generateHTMLdocumentationFunctionInputArguments{} error: (startPositionOfArgumentName == CPP_STRING_FIND_RESULT_FAIL_VALUE)" << endl;
				exit(0);
			}
			string currentArgumentName = currentArgument.substr(startPositionOfArgumentName, endPositionOfArgument-startPositionOfArgumentName);
			string currentArgumentType = currentArgument.substr(0, startPositionOfArgumentName);

			#ifdef CS_DEBUG_GENERATE_CONST_FUNCTION_ARGUMENTS
			cout << "identifyFunctionDeclarationArguments{}: functionNameFull = " << *functionNameFull << endl;
			cout << "identifyFunctionDeclarationArguments{}: currentArgument = " << currentArgument << endl;
			cout << "identifyFunctionDeclarationArguments{}: currentArgumentName = " << currentArgumentName << endl;
			cout << "identifyFunctionDeclarationArguments{}: currentArgumentType = " << currentArgumentType << endl;
			//cout << "currentArgumentName = " << currentArgumentName << ", currentArgumentType = " << currentArgumentType << endl;			
			//if(*functionName == "...")
			//{
			//	cout << "functionArgumentsRaw = " << functionArgumentsRaw << endl;
			//	cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
			//	cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
			//	cout << "currentArgument = " << currentArgument << endl;
			//	cout << "startPositionOfArgumentName = " << startPositionOfArgumentName << endl;
			//	cout << "currentArgumentName = " << currentArgumentName << endl;
			//	cout << "currentArgumentType = " << currentArgumentType << endl;
			//	cout << "currentArgumentTypeLength = " << startPositionOfArgumentName << endl;
			//}
			#endif
			
			currentFunctionArgumentInFunctionReference->argument = currentArgument;
			currentFunctionArgumentInFunctionReference->argumentName = currentArgumentName;
			currentFunctionArgumentInFunctionReference->argumentType = currentArgumentType;
			currentFunctionArgumentInFunctionReference->next = new CSfunctionArgument();
			currentFunctionArgumentInFunctionReference = currentFunctionArgumentInFunctionReference->next;
						
			startPositionOfArgument = endPositionOfArgument+1;
		}
	}
	else
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		cout << "!functionHasArguments: functionNameFull = " <<* functionNameFull << endl;
		#endif
	}
}
#endif






















































/*
CSfile* findReferenceInIncludeFileList(CSfile* firstReferenceInAboveLevelBelowList, string referenceName, CSfile* foundReference, bool* referenceFound)
{
	CSfile* foundReferenceNew = foundReference;
	CSfile* currentReference = firstReferenceInAboveLevelBelowList;
	while(currentReference->next != NULL)
	{
		if(currentReference->name == referenceName)
		{
			*referenceFound = true;
			foundReferenceNew = currentReference;
		}

		if(currentReference->firstFileInBelowListContainer != NULL)
		{
			foundReferenceNew = findReferenceInIncludeFileList(currentReference->firstFileInBelowListContainer, referenceName, foundReferenceNew, referenceFound);
		}

		currentReference = currentReference->next;
	}

	return foundReferenceNew;

}
*/


string generateSourceFileNameFromHeaderFileName(string headerFileName, string sourceFileNameExtension)
{
	string sourceFileName = "";
	int positionOfExtension = headerFileName.rfind(CHAR_FULLSTOP);
	if(positionOfExtension != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		sourceFileName = headerFileName.substr(0, positionOfExtension+1) + sourceFileNameExtension;
		//cout << "sourceFileName = " << sourceFileName << endl;
	}
	return sourceFileName;
}

			



void attachFunctionReferenceTargets(CSfileContainer* firstObjectInAboveLevelBelowListContainer)
{	
	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;
		
		CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
		while(currentFunctionObject->next != NULL)
		{
			CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceList;
			while(currentFunctionReference->next != NULL)
			{
				CSfile* fileObjectHoldingFunction = NULL;
				CSfunction* functionReferenceTarget = NULL;		//function pertaining to currentFunctionReference
				if(findFunctionReferenceTarget(currentFunctionReference, currentFileObject, &fileObjectHoldingFunction, &functionReferenceTarget, true))
				{
					currentFunctionReference->functionReferenceTarget = functionReferenceTarget;
					currentFunctionReference->functionReferenceTargetFileOwner = fileObjectHoldingFunction;
				}
				currentFunctionReference = currentFunctionReference->next;
			}
			
			#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
			currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceListRepeats;
			while(currentFunctionReference->next != NULL)
			{
				CSfile* fileObjectHoldingFunction = NULL;
				CSfunction* functionReferenceTarget = NULL;		//function pertaining to currentFunctionReference
				if(findFunctionReferenceTarget(currentFunctionReference, currentFileObject, &fileObjectHoldingFunction, &functionReferenceTarget, true))
				{
					currentFunctionReference->functionReferenceTarget = functionReferenceTarget;
					currentFunctionReference->functionReferenceTargetFileOwner = fileObjectHoldingFunction;
				}
				currentFunctionReference = currentFunctionReference->next;
			}
			#endif
			
			currentFunctionObject = currentFunctionObject->next;
		}

		if(currentFileObject->firstFileInBelowListContainer != NULL)
		{
			attachFunctionReferenceTargets(currentFileObject->firstFileInBelowListContainer);
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}
}

