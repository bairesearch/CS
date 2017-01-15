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
 * Project Version: 3f4a 11-July-2015
 *
 *******************************************************************************/


#include "CSoperations.h"
#include "LDsprite.h"
#include "LDreferenceManipulation.h"




bool getIncludeFileNamesFromCorHfile(CSfileReferenceContainer* firstReferenceInIncludeFileListContainer, CSfileReferenceContainer* topLevelReferenceContainer, CSfileReference* aboveLevelReference, string topLevelReferenceName, int level)
{
	bool fileFound = false;

	string parseFileName = topLevelReferenceName;

	ifstream parseFileObject(parseFileName.c_str());
	CSfileReferenceContainer* currentReferenceInIncludeFileListContainer = firstReferenceInIncludeFileListContainer;

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
		#ifdef CS_GENERATE_CPP_CLASSES
		string fileContentsString = "";
		#endif
		*/
		
		while(parseFileObject.get(c))
		{
			#ifdef CS_DEBUG_VERBOSE
			//cout << c;
			#endif
			
			/*//this implementation will use up too much memory
			#ifdef CS_GENERATE_CPP_CLASSES
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
					if(hashIncludeFileName != aboveLevelReference->name)	//this is added so that do not parse method.h from within method.cpp!
					{
						bool alreadyParsedFile = false;
						CSfileReference* currentReferenceInIncludeFileList = NULL;
						if(findFileReferenceInFileReferenceContainerList(topLevelReferenceContainer->fileReference->firstReferenceInBelowListContainer, hashIncludeFileName, &currentReferenceInIncludeFileList))
						{
							currentReferenceInIncludeFileListContainer->fileReference = currentReferenceInIncludeFileList;
							
							CSfileReferenceContainer* newCSReferenceContainer = new CSfileReferenceContainer();
							newCSReferenceContainer->previous = currentReferenceInIncludeFileListContainer;
							currentReferenceInIncludeFileListContainer->next = newCSReferenceContainer;
						}
						else
						{
							currentReferenceInIncludeFileList = new CSfileReference();
							currentReferenceInIncludeFileListContainer->fileReference = currentReferenceInIncludeFileList;
							currentReferenceInIncludeFileList->name = hashIncludeFileName;
							string hashIncludeFileNameCFile = generateSourceFileNameFromHeaderFileName(hashIncludeFileName, CS_SOURCE_FILE_EXTENSION);
							#ifdef CS_GENERATE_CPP_CLASSES
							currentReferenceInIncludeFileList->sourceFileNameOrig = hashIncludeFileNameCFile;
							currentReferenceInIncludeFileList->headerFileName = generateSourceFileNameFromHeaderFileName(currentReferenceInIncludeFileList->name, CS_GENERATE_CPP_CLASSES_HEADER_FILE_EXTENSION);
							currentReferenceInIncludeFileList->sourceFileName = generateSourceFileNameFromHeaderFileName(currentReferenceInIncludeFileList->sourceFileNameOrig, CS_GENERATE_CPP_CLASSES_SOURCE_FILE_EXTENSION);
							#endif
							currentReferenceInIncludeFileList->id = id;
							currentReferenceInIncludeFileList->level = level;
							currentReferenceInIncludeFileListContainer->aboveLevelReference = aboveLevelReference;
							id++;

							CSfileReferenceContainer* newCSReferenceContainer = new CSfileReferenceContainer();
							newCSReferenceContainer->previous = currentReferenceInIncludeFileListContainer;
							currentReferenceInIncludeFileListContainer->next = newCSReferenceContainer;

							CSfileReferenceContainer* newfirstReferenceInBelowListContainer = new CSfileReferenceContainer();
							currentReferenceInIncludeFileList->firstReferenceInBelowListContainer = newfirstReferenceInBelowListContainer;

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
							//getIncludeFileNamesFromCorHfile{}: this will add the include file references (filenames) within the .h file in "#include x.h" to currentReferenceInIncludeFileList->firstReferenceInBelowListContainer
							bool hFileFound = getIncludeFileNamesFromCorHfile(currentReferenceInIncludeFileList->firstReferenceInBelowListContainer, topLevelReferenceContainer, currentReferenceInIncludeFileList, referenceName, level+1);


							if(hFileFound)
							{
								//parse into .cpp file;
								CSfileReferenceContainer* lastReferenceInBelowListContainer = currentReferenceInIncludeFileList->firstReferenceInBelowListContainer;
								while(lastReferenceInBelowListContainer->next != NULL)
								{
									lastReferenceInBelowListContainer = lastReferenceInBelowListContainer->next;
								}
								string referenceNameCFile = hashIncludeFileNameCFile;
								//getIncludeFileNamesFromCorHfile{}: this will add (append) the include file references (filenames) within the .c equivalent (ie x.c) of the .h file in "#include x.h" to currentReferenceInIncludeFileList->firstReferenceInBelowListContainer [and will recurse]
								bool cFileFound = getIncludeFileNamesFromCorHfile(lastReferenceInBelowListContainer, topLevelReferenceContainer, currentReferenceInIncludeFileList, referenceNameCFile, level+1);

								if(cFileFound)
								{
									//parse the new file reference only if both its ".h" and ".c" file are found

									//fill function and function reference list using .h file;
									CSfunctionReference* newfirstReferenceInFunctionList = new CSfunctionReference();
									currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;

									//getFunctionNamesFromFunctionDeclarationsInHfile{}: this opens the .h file hashIncludeFileName and extracts its function declarations (hashIncludeFileName is the current filename being parsed by this instance of getIncludeFileNamesFromCorHfile, ie "x.h" in "#include x.h")
									bool hFileFound2 = getFunctionNamesFromFunctionDeclarationsInHfile(currentReferenceInIncludeFileList->firstReferenceInFunctionList, referenceName, level);
									if(hFileFound2)
									{
										#ifdef CS_DEBUG
										//cout << "currentReferenceInIncludeFileList->firstReferenceInFunctionList->name = " << currentReferenceInIncludeFileList->firstReferenceInFunctionList->name << endl;
										//cout << "topLevelReferenceName = " << topLevelReferenceName << endl;
										//cout << "getFunctionReferenceNamesFromFunctionsInCfile{}" << endl;
										#endif
										//getFunctionReferenceNamesFromFunctionsInCfile{}: this opens the .c file (hashIncludeFileName equivalent) and locates all the functions corresponding to those declared in its .h file
										getFunctionReferenceNamesFromFunctionsInCfile(firstReferenceInIncludeFileListContainer->fileReference, currentReferenceInIncludeFileList->firstReferenceInFunctionList, currentReferenceInIncludeFileList, referenceNameCFile, level);

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
									CSfunctionReference* newfirstReferenceInFunctionList = new CSfunctionReference();
									currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;
								}
							}
							else
							{
								#ifdef CS_DEBUG
								//cout << "!hFileFound" << endl;
								#endif
								CSfunctionReference* newfirstReferenceInFunctionList = new CSfunctionReference();
								currentReferenceInIncludeFileList->firstReferenceInFunctionList = newfirstReferenceInFunctionList;
							}
						}

						currentReferenceInIncludeFileListContainer = currentReferenceInIncludeFileListContainer->next;


					}
					else
					{
						#ifdef CS_DEBUG
						//cout << "hashIncludeFileName = " << hashIncludeFileName << endl;
						//cout << "aboveLevelReference->name = " << aboveLevelReference->name << endl;
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
		#ifdef CS_GENERATE_CPP_CLASSES
		if(isHeader)
		{
			aboveLevelReference->headerFileText = fileContentsString;
		}
		else
		{
			aboveLevelReference->sourceFileText = fileContentsString;
		}
		#endif
		*/
	}

	return fileFound;
}

bool findFileReferenceInFileReferenceContainerList(CSfileReferenceContainer* firstReferenceContainerInLevel, string fileReferenceName, CSfileReference** fileReferenceFound)
{
	bool foundFileReference = false;
	CSfileReferenceContainer* currentReferenceContainerInLevel = firstReferenceContainerInLevel;
	while(currentReferenceContainerInLevel->next != NULL)
	{
		if(currentReferenceContainerInLevel->fileReference->name == fileReferenceName)
		{
			foundFileReference = true;
			*fileReferenceFound = currentReferenceContainerInLevel->fileReference;
		}
		if(!foundFileReference)
		{
			if(currentReferenceContainerInLevel->fileReference->firstReferenceInBelowListContainer != NULL)
			{
				if(findFileReferenceInFileReferenceContainerList(currentReferenceContainerInLevel->fileReference->firstReferenceInBelowListContainer, fileReferenceName, fileReferenceFound))
				{
					foundFileReference = true;
				}
			}
		}
		currentReferenceContainerInLevel = currentReferenceContainerInLevel->next;
	}
	return foundFileReference;
}


bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunctionReference* firstReferenceInFunctionList, string topLevelFileName, int level)
{
	bool fileFound = false;

	string parseFileName = topLevelFileName;

	ifstream parseFileObject(parseFileName.c_str());
	CSfunctionReference* currentReferenceInFunctionList = firstReferenceInFunctionList;
	CSfunctionReference* currentReferenceInFunctionReferenceList = NULL;

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
						
						currentReferenceInFunctionList->isFunctionReference = true;
						currentReferenceInFunctionList->name = functionName;
						currentReferenceInFunctionList->nameFull = functionNameFull;
						#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
						currentReferenceInFunctionList->functionReferenceIndentation = functionReferenceIndentationInHfileTemp;
						#endif
						CSfunctionReference* newCSReference = new CSfunctionReference();
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

void getFunctionReferenceNamesFromFunctionsInCfile(CSfileReference* firstReferenceInIncludeFileList, CSfunctionReference* firstReferenceInFunctionList, CSfileReference* aboveLevelReference, string topLevelFileName, int level)
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
		CSfunctionReference* currentReference = firstReferenceInFunctionList;

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
				int positionOfFunctionReference = fileContentsString.find(fullFunctionName);

				if(positionOfFunctionReference != CPP_STRING_FIND_RESULT_FAIL_VALUE)
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
							cout << "getFunctionReferenceNamesFromFunctionsInCfile{} error: function definitions in .cpp files must end with a } without any leading white space" << endl;
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
							cout << "getFunctionReferenceNamesFromFunctionsInCfile{} error: function definitions in .cpp files must end with a } without any leading white space" << endl;
							exit(0);
						}

						currentIndexInFunction++;
					}

					int positionOfFunctionReferenceEnd = currentIndexInFunction;
					string functionContentsString = fileContentsString.substr(positionOfFunctionReference+fullFunctionName.length(), (positionOfFunctionReferenceEnd-positionOfFunctionReference)-fullFunctionName.length());
				#endif
					#ifdef CS_GENERATE_CPP_CLASSES
					int positionOfFunctionReferenceEnd = currentIndexInFunction;
					string functionContentsStringRaw = fileContentsString.substr(positionOfFunctionReference+fullFunctionName.length(), (positionOfFunctionReferenceEnd-positionOfFunctionReference)-fullFunctionName.length());
					currentReference->functionTextRaw = functionContentsStringRaw;
					functionContentsStringRaw = "";
					//cout << "functionContentsStringRaw = " << functionContentsStringRaw << endl;
					#endif
					
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
					currentReference->functionText = functionContentsString;
					#endif


					//3. search the functionContentsString for any of the function (not full function names) across all include/header files
					CSfunctionReference* newfirstReferenceInFunctionReferenceList = new CSfunctionReference();
					currentReference->firstReferenceInFunctionReferenceList = newfirstReferenceInFunctionReferenceList;
					CSfunctionReference* currentReferenceInFunctionReferenceList = currentReference->firstReferenceInFunctionReferenceList;
					#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
					CSfunctionReference* newfirstReferenceInFunctionReferenceListRepeats = new CSfunctionReference();
					currentReference->firstReferenceInFunctionReferenceListRepeats = newfirstReferenceInFunctionReferenceListRepeats;
					CSfunctionReference* currentReferenceInFunctionReferenceListRepeats = currentReference->firstReferenceInFunctionReferenceListRepeats;
					#endif
					
					#ifdef CS_DEBUG
					/*
					cout << "file topLevelFileName = " << topLevelFileName << endl;
					cout << "aboveLevelReference->name = " << aboveLevelReference->name << endl;
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

					//cout << "search current file for function references;" << endl;
					#ifdef CS_DEBUG
					//cout << "firstReferenceInIncludeFileList->name = " << firstReferenceInIncludeFileList->name << endl;
					#endif
					//search current file for function references;
					if(!searchFunctionStringForFunctionReferences(firstReferenceInIncludeFileList, aboveLevelReference, &currentReferenceInFunctionReferenceList, &currentReferenceInFunctionReferenceListRepeats, functionContentsString))
					{
					
					}
					
					#ifdef CS_DEBUG
					/*
					CSfunctionReference* temp1acurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
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
					if(!searchFunctionStringForFunctionReferencesRecursive(firstReferenceInIncludeFileList, aboveLevelReference->firstReferenceInBelowListContainer, &currentReferenceInFunctionReferenceList, &currentReferenceInFunctionReferenceListRepeats, functionContentsString))
					{
					
					}
					
					#ifdef CS_DEBUG
					/*
					CSfunctionReference* temp1bcurrentReferenceInFunctionList = currentReference->firstReferenceInFunctionReferenceList;
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
	#ifdef CS_GENERATE_CPP_CLASSES
	aboveLevelReference->sourceFileText = fileContentsString;
	#endif
	*/
}

bool searchFunctionStringForFunctionReferencesRecursive(CSfileReference* firstReferenceInIncludeFileList, CSfileReferenceContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunctionReference** currentReferenceInFunctionReferenceList, CSfunctionReference** currentReferenceInFunctionReferenceListRepeats, string functionContentsString)
{
	bool result = true;
	
	CSfileReferenceContainer* currentReferenceContainer = firstFileNameInLayerContainingFunctionReferencesToSearchFor;

	while(currentReferenceContainer->next != NULL)
	{
		CSfileReference* currentReference = currentReferenceContainer->fileReference;
		
		searchFunctionStringForFunctionReferences(firstReferenceInIncludeFileList, currentReference, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString);

		if(currentReference->firstReferenceInBelowListContainer != NULL)
		{
			if(!searchFunctionStringForFunctionReferencesRecursive(firstReferenceInIncludeFileList, currentReference->firstReferenceInBelowListContainer, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
			{
				result = false;
			}		
		}

		currentReferenceContainer = currentReferenceContainer->next;
	}
	
	return result;
	
}

bool searchFunctionStringForFunctionReferences(CSfileReference* firstReferenceInIncludeFileList, CSfileReference* fileNameContainingFunctionReferencesToSearchFor, CSfunctionReference** currentReferenceInFunctionReferenceList, CSfunctionReference** currentReferenceInFunctionReferenceListRepeats, string functionContentsString)
{
	bool result = true;
	
	//only search those file names which include aboveLevelReference.h

	//find reference to this file

	#ifdef CS_DEBUG
	//cout << "\tcurrentValidFileNameToSearchReferencesFor->name = " << fileNameContainingFunctionReferencesToSearchFor->name << endl;
	#endif

	//now search function string for all functions within referenceFound;
	//CSfunctionReference* currentFunction = foundReference->firstReferenceInFunctionList;
	CSfunctionReference* currentFunction = fileNameContainingFunctionReferencesToSearchFor->firstReferenceInFunctionList;

	while(currentFunction->next != NULL)
	{
		#ifdef CS_DEBUG
		//cout << "\t\tcurrentFunction->name = " << currentFunction->name << endl;
		#endif

		//now search the function string for this function reference;
		
		int startPosOfFunctionReferenceInFunction = 0;
		bool firstTimeFound = true;
		//cout << "function->functionText = " << function->functionText << endl;
		while(CPP_STRING_FIND_RESULT_FAIL_VALUE != (startPosOfFunctionReferenceInFunction = functionContentsString.find((currentFunction->name + CHAR_OPEN_BRACKET), startPosOfFunctionReferenceInFunction)))
		{
			//added condition CS 3f1b - ensure previous character is not a letter (this ensures that ABCfunctionName is not found when searching for functionName)
			if((startPosOfFunctionReferenceInFunction == 0) || !charInCharArray(functionContentsString[startPosOfFunctionReferenceInFunction-1], functionNameCharacters, CS_FUNCTION_NAME_CHARACTERS_NUMBER_OF_TYPES))
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
					(*currentReferenceInFunctionReferenceList)->isFunctionReferenceReference = true;

					CSfunctionReference* newfirstReferenceInFunctionReferenceList = new CSfunctionReference();
					(*currentReferenceInFunctionReferenceList)->next = newfirstReferenceInFunctionReferenceList;
					(*currentReferenceInFunctionReferenceList) = (*currentReferenceInFunctionReferenceList)->next;

					firstTimeFound = false;
				}

				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
				(*currentReferenceInFunctionReferenceListRepeats)->name = currentFunction->name;
				(*currentReferenceInFunctionReferenceListRepeats)->isFunctionReferenceReference = true;

				int indexToFunctionReference = startPosOfFunctionReferenceInFunction;
				int indexOfFunctionReferenceStartOfLine = functionContentsString.rfind("\n", indexToFunctionReference);
				string functionReferenceStartLine = functionContentsString.substr(indexOfFunctionReferenceStartOfLine, indexToFunctionReference-indexOfFunctionReferenceStartOfLine);
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

				CSfunctionReference* newfirstReferenceInFunctionReferenceListRepeats = new CSfunctionReference();
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





























































/*
CSfileReference* findReferenceInIncludeFileList(CSfileReference* firstReferenceInAboveLevelBelowList, string referenceName, CSfileReference* foundReference, bool* referenceFound)
{
	CSfileReference* foundReferenceNew = foundReference;
	CSfileReference* currentReference = firstReferenceInAboveLevelBelowList;
	while(currentReference->next != NULL)
	{
		if(currentReference->name == referenceName)
		{
			*referenceFound = true;
			foundReferenceNew = currentReference;
		}

		if(currentReference->firstReferenceInBelowListContainer != NULL)
		{
			foundReferenceNew = findReferenceInIncludeFileList(currentReference->firstReferenceInBelowListContainer, referenceName, foundReferenceNew, referenceFound);
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

			



