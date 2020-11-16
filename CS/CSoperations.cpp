/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSoperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3o3a 16-November-2020
 * /
 *******************************************************************************/


#include "CSoperations.hpp"




bool CSoperationsClass::getIncludeFileNamesFromCorHfile(CSfileContainer* firstReferenceInIncludeFileListContainer, constEffective CSfileContainer* topLevelReferenceContainer, CSfile* aboveLevelObject, const string topLevelReferenceName, const int level, const bool parsingCorHfile)
{
	bool fileFound = false;

	string parseFileName = topLevelReferenceName;

	ifstream parseFileObject(parseFileName.c_str());
	CSfileContainer* currentReferenceInIncludeFileListContainer = firstReferenceInIncludeFileListContainer;

	if(!parseFileObject.rdbuf()->is_open())
	{

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
						charCount++;
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
				charCount++;
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
					//CS does not support relative paths in #include that may include a / or a \ - header h files not be in the same folder as source c files
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

						char tempChar;
						parseFileObject.get(tempChar);
						charCount++;
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
					if(hashIncludeFileName != aboveLevelObject->name)	//this is added so that do not parse method.h from within method.c!
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
							string hashIncludeFileNameCFile = generateSourceFileNameFromHeaderFileName(hashIncludeFileName, CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION);
							#ifdef CS_GENERATE_CODE_GENERIC
							currentReferenceInIncludeFileList->sourceFileNameOrig = hashIncludeFileNameCFile;
							currentReferenceInIncludeFileList->headerFileName = generateSourceFileNameFromHeaderFileName(currentReferenceInIncludeFileList->name, CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION);
							currentReferenceInIncludeFileList->sourceFileName = generateSourceFileNameFromHeaderFileName(currentReferenceInIncludeFileList->sourceFileNameOrig, CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION);
							currentReferenceInIncludeFileList->headerFileTextOrig = SHAREDvars.getFileContents(currentReferenceInIncludeFileList->name);	//headerFileNameOrig
							currentReferenceInIncludeFileList->sourceFileTextOrig = SHAREDvars.getFileContents(currentReferenceInIncludeFileList->sourceFileNameOrig);
							currentReferenceInIncludeFileList->headerFileText = currentReferenceInIncludeFileList->headerFileTextOrig;	//initialise with same source code as original file
							currentReferenceInIncludeFileList->sourceFileText = currentReferenceInIncludeFileList->sourceFileTextOrig;	//initialise with same source code as original file
							//cout << "getIncludeFileNamesFromCorHfile{}: currentReferenceInIncludeFileList->name = " << currentReferenceInIncludeFileList->name << endl;
							//cout << "getIncludeFileNamesFromCorHfile{}: currentReferenceInIncludeFileList->sourceFileNameOrig = " << currentReferenceInIncludeFileList->sourceFileNameOrig << endl;
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
							printLevelIndentation(level);
							cout << hashIncludeFileName << endl;
							//parse into .c file;
							printLevelIndentation(level);
							cout << hashIncludeFileNameCFile << endl;
							#endif

							//parse into .h file;
							string referenceName = hashIncludeFileName;	//hashIncludeFileName is the current filename being parsed by this instance of getIncludeFileNamesFromCorHfile, ie "x.h" in "#include x.h" [and will recurse]
							//getIncludeFileNamesFromCorHfile{}: this will add the include file references (filenames) within the .h file in "#include x.h" to currentReferenceInIncludeFileList->firstFileInBelowListContainer
							bool hFileFound = getIncludeFileNamesFromCorHfile(currentReferenceInIncludeFileList->firstFileInBelowListContainer, topLevelReferenceContainer, currentReferenceInIncludeFileList, referenceName, level+1, false);

							#ifndef CS_DEBUG_DISABLE_FUNCTION_PARSING
							if(hFileFound)
							{
								//parse into .c file;
								CSfileContainer* lastReferenceInBelowListContainer = currentReferenceInIncludeFileList->firstFileInBelowListContainer;
								while(lastReferenceInBelowListContainer->next != NULL)
								{
									lastReferenceInBelowListContainer = lastReferenceInBelowListContainer->next;
								}
								string referenceNameCFile = hashIncludeFileNameCFile;
								//getIncludeFileNamesFromCorHfile{}: this will add (append) the include file references (filenames) within the .c equivalent (ie x.c) of the .h file in "#include x.h" to currentReferenceInIncludeFileList->firstFileInBelowListContainer [and will recurse]
								bool cFileFound = getIncludeFileNamesFromCorHfile(lastReferenceInBelowListContainer, topLevelReferenceContainer, currentReferenceInIncludeFileList, referenceNameCFile, level+1, true);

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
										//getFunctionObjectNamesFromFunctionsInCfile{}: this opens the .c file (hashIncludeFileName equivalent) and locates all the functions corresponding to those declared in its .h file
										//it also locates all function references within those functions
										getFunctionObjectNamesFromFunctionsInCfile(firstReferenceInIncludeFileListContainer->fileObject, currentReferenceInIncludeFileList->firstFunctionInFunctionList, currentReferenceInIncludeFileList, referenceNameCFile, level);
									}
									else
									{
									}
								}
								else
								{
									CSfunction* newfirstReferenceInFunctionList = new CSfunction();
									currentReferenceInIncludeFileList->firstFunctionInFunctionList = newfirstReferenceInFunctionList;
								}
							}
							else
							{
								CSfunction* newfirstReferenceInFunctionList = new CSfunction();
								currentReferenceInIncludeFileList->firstFunctionInFunctionList = newfirstReferenceInFunctionList;
							}
							#endif
						}

						currentReferenceInIncludeFileListContainer = currentReferenceInIncludeFileListContainer->next;
					}
					else
					{
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
	}

	return fileFound;
}

#ifdef CS_GENERATE_CODE_GENERIC
bool CSoperationsClass::fileIsHeader(string parseFileName)
{
	bool isHeader = false;
	string headerFileExtension = string(STRING_FULLSTOP) + CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION;
	string sourceFileExtension = string(STRING_FULLSTOP) + CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION;
	int positionOfHeaderFileExtension = parseFileName.rfind(headerFileExtension);
	int positionOfSourceFileExtension = parseFileName.rfind(sourceFileExtension);
	if(positionOfHeaderFileExtension == parseFileName.length()-headerFileExtension.length())
	{
		isHeader = true;
	}
	else if(positionOfSourceFileExtension == parseFileName.length()-sourceFileExtension.length())
	{
	
	}
	else
	{
		cout << "fileIsHeader{} error: parseFileName is not CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION or CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION" << endl;
	}
	
	return isHeader;
}
#endif
		
bool CSoperationsClass::findFileObjectInFileObjectContainerList(constEffective CSfileContainer* firstReferenceContainerInLevel, const string fileReferenceName, constEffective CSfile** fileReferenceFound)
{
	bool foundFileObject = false;
	constEffective CSfileContainer* currentReferenceContainerInLevel = firstReferenceContainerInLevel;
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


bool CSoperationsClass::getFunctionNamesFromFunctionDeclarationsInHfile(CSfunction* firstFunctionInFunctionList, const string topLevelFileName, const int level)
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
		bool readingWhitespace = true;
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
		int functionReferenceIndentationInHfileTemp = 0;
		#endif
		#ifdef CS_SUPPORT_GENERATED_CPP_CODE
		bool readingFunctionAccessSpecifier = false;
		string functionAccessSpecifier = "";
		#ifdef CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIERS
		int functionAccessSpecifierType = CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIER_UNKNOWN;
		#endif
		bool readingClassType = false;
		bool readingClassName = false;
		string classType = "";
		string className = "";
		bool parsingClassContents = false;
		bool firstCharacterOfLine = true;
		bool validClassType = false;
		#endif
		bool readingFunctionType = false;
		bool readingFunctionName = false;
		bool readingFunctionArguments = false;
		bool waitingForNewLine = false;
		bool readingLargeComment = false;
		bool prepareNewLine = false;
		
		string functionType = "";
		string functionName = "";
		string functionArguments = "";
		string functionNameFull = "";

		while(prepareNewLine || parseFileObject.get(c))
		{
			if(prepareNewLine)
			{
				lineCount++;
				waitingForNewLine = false;
				readingWhitespace = true;
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
				functionReferenceIndentationInHfileTemp = 0;
				#endif
				#ifdef CS_SUPPORT_GENERATED_CPP_CODE
				functionAccessSpecifier = "";
				#ifdef CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIERS
				functionAccessSpecifierType = CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIER_UNKNOWN;
				#endif
				readingClassType = false;
				readingClassName = false;
				classType = "";
				//className = "";
				firstCharacterOfLine = true;
				validClassType = false;
				#endif
				readingFunctionType = false;
				readingFunctionName = false;
				readingFunctionArguments = false;
				functionType = "";
				functionName = "";
				functionArguments = "";
				functionNameFull = "";
				
				prepareNewLine = false;
			}
			else
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
							charCount++;
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
					charCount++;
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
						prepareNewLine = true;
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
						prepareNewLine = true;
					}
					else
					{
						//do nothing, still waiting for new line
					}
				}	
				else if(readingWhitespace)
				{
					#ifdef CS_SUPPORT_GENERATED_CPP_CODE
					if(parsingClassContents && firstCharacterOfLine && (c == '}'))	
					{
						//detect end of class
						char tempChar;
						parseFileObject.get(tempChar);
						charCount++;
						if(tempChar == ';')
						{
							className = "";
							parsingClassContents = false;
							className = "";
						}
					} else
					#endif
					if((c == ' ') || (c == '\t'))
					{
						#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
						if(c == CS_SOURCE_FILE_INDENTATION_CHARACTER)
						{
							functionReferenceIndentationInHfileTemp++;
						}
						#endif
						
						//ignore preceeding white space
						
						#ifdef CS_SUPPORT_GENERATED_CPP_CODE
						firstCharacterOfLine = false;
						#endif
					}
					else if(c == '\n')
					{
						prepareNewLine = true;
					}
					else
					{
						//found start of function declaration
						readingWhitespace = false;
						
						#ifdef CS_SUPPORT_GENERATED_CPP_CODE
						readingFunctionAccessSpecifier = true;
						#else
						readingFunctionType = true;
						#endif

						#ifdef CS_SUPPORT_GENERATED_CPP_CODE
						functionAccessSpecifier = functionAccessSpecifier + c;
						#else
						functionNameFull = functionNameFull + c;
						#endif
					}
				}
				#ifdef CS_SUPPORT_GENERATED_CPP_CODE
				else if(readingFunctionAccessSpecifier)
				{
					if(c == ' ')
					{
						readingFunctionAccessSpecifier = false;
						if(validClassType && (functionAccessSpecifier == CS_CLASS_TYPE))
						{
							functionAccessSpecifier = "";
							readingClassName = true;
						}
						#ifdef CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIERS
						else if(SHAREDvars.textInTextArray(functionAccessSpecifier, functionAccessSpecifierNameArray, CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIER_NUMBER_OF_TYPES, &functionAccessSpecifierType))
						{
							//cout << "functionAccessSpecifierType = " << functionAccessSpecifierType << endl;
							readingFunctionType = true;
							//functionNameFull = functionNameFull + c;	//do not add functionAccessSpecifier to functionNameFull
						}
						#endif
						else
						{
							//no inline function access specifier found (as function type)
							functionType = functionAccessSpecifier;
							functionAccessSpecifier = "";
							functionNameFull = functionNameFull + functionType + c;

							//restart function name fill;
							functionName = "";
							readingFunctionName = true;
						}
					}
					else if(c == '\n')
					{
						prepareNewLine = true;
					}
					else
					{
						if(firstCharacterOfLine)
						{
							validClassType = true;
						}
						
						functionAccessSpecifier = functionAccessSpecifier + c;
					}

				}
				#endif
				#ifdef CS_SUPPORT_GENERATED_CPP_CODE
				else if(readingClassName)
				{
					//only support "class className\n{" and "class className{\n" class definition headers:
					if(c == '\n')
					{
						char tempChar;
						parseFileObject.get(tempChar);
						charCount++;
						if(tempChar == '{')
						{
							parsingClassContents = true;
							//cout << "CS_SUPPORT_GENERATED_CPP_CODE: className = " << className << endl;
						}
						else
						{
							//class declaration detected (not definition)
							className = "";
							/*
							cerr << "CS_SUPPORT_GENERATED_CPP_CODE: CSoperationsClass::getFunctionNamesFromFunctionDeclarationsInHfile error; class className\\n not followed by { character" << endl;
							cerr << "topLevelFileName = " << topLevelFileName << endl;
							exit(EXIT_ERROR);
							*/
						}
						prepareNewLine = true;
					}
					else if(c == '{')
					{
						char tempChar;
						parseFileObject.get(tempChar);
						charCount++;
						if(tempChar == '\n')
						{
							parsingClassContents = true;
							//cout << "CS_SUPPORT_GENERATED_CPP_CODE: className = " << className << endl;
						}
						else
						{
							//class declaration detected (not definition)
							className = "";
							/*
							cerr << "CS_SUPPORT_GENERATED_CPP_CODE: CSoperationsClass::getFunctionNamesFromFunctionDeclarationsInHfile error; class className{ not followed by \\n character" << endl;
							cerr << "topLevelFileName = " << topLevelFileName << endl;
							exit(EXIT_ERROR);
							*/
						}
						prepareNewLine = true;
					}
					else if(c == ':')
					{
						//ignore class inheritance
						parsingClassContents = true;
						waitingForNewLine = true;
						//cout << "CS_SUPPORT_GENERATED_CPP_CODE: className = " << className << endl;
					}
					else
					{
						className = className + c;
					}
				}
				#endif
				else if(readingFunctionType)
				{
					if(c == ' ')
					{
						readingFunctionType = false;

						functionNameFull = functionNameFull + c;

						//restart function name fill;
						functionName = "";
						readingFunctionName = true;
					}
					else if(c == '\n')
					{
						prepareNewLine = true;
					}
					else
					{
						functionType = functionType + c;
						functionNameFull = functionNameFull + c;
					}
				}
				else if(readingFunctionName)
				{
					if(c == ' ')
					{
						functionNameFull = functionNameFull + c;

						//not a function, restart function name fill;
						functionName = "";
					}
					else if(c == '(')
					{
						functionArguments = functionArguments + c;
						functionNameFull = functionNameFull + c;

						readingFunctionName = false;
						readingFunctionArguments = true;
					}
					else if(c == '\n')
					{
						prepareNewLine = true;
					}
					else
					{
						functionName = functionName + c;
						functionNameFull = functionNameFull + c;
					}
				}
				else if(readingFunctionArguments)
				{
					if(c == ')')
					{
						functionArguments = functionArguments + c;
						functionNameFull = functionNameFull + c;

						char newC;
						parseFileObject.get(newC);
						charCount++;
						if(newC == ';')
						{//function reference found

							#ifdef CS_SUPPORT_FUNCTION_RETURN_POINTERS
							if(functionName[0] == '*')
							{
								int functionNameLength = functionName.length();
								functionName = functionName.substr(1, functionNameLength-1);
							}
							#endif

							/*
							if(topLevelFileName == "LDreferenceManipulation.h")
							{
								cout << "functionName = " << functionName << endl;
							}
							*/

							currentReferenceInFunctionList->isFunction = true;
							#ifdef CS_SUPPORT_GENERATED_CPP_CODE
							if(parsingClassContents)
							{
								currentReferenceInFunctionList->name = functionName;	//CHECKTHIS
								currentReferenceInFunctionList->nameFull = functionType + STRING_SPACE + className + CS_CLASS_DELIMITER + functionName + functionArguments;	//full function name to search for in cpp files
								//cout << "parsingClassContents: currentReferenceInFunctionList->nameFull = " << currentReferenceInFunctionList->nameFull << endl;	
								currentReferenceInFunctionList->nameFullHeader = functionNameFull;
							}
							else
							{
								currentReferenceInFunctionList->name = functionName;
								currentReferenceInFunctionList->nameFull = functionNameFull;	
								//cout << "!parsingClassContents: currentReferenceInFunctionList->nameFull = " << currentReferenceInFunctionList->nameFull << endl;	
								currentReferenceInFunctionList->nameFullHeader = functionNameFull;				
							}
							#else
							currentReferenceInFunctionList->name = functionName;
							currentReferenceInFunctionList->nameFull = functionNameFull;
							#endif
							currentReferenceInFunctionList->functionType = functionType;	//not used
							currentReferenceInFunctionList->functionArguments = functionArguments;	//not used
							#ifdef CS_SUPPORT_GENERATED_CPP_CODE
							currentReferenceInFunctionList->className = className;
							#endif
							#ifdef CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIERS
							currentReferenceInFunctionList->functionAccessSpecifierType = functionAccessSpecifierType;
							#endif
							#ifdef CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS
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
						}
						
						prepareNewLine = true;
						lineCount--;	//to account for prepareNewLine expecting new line already detected
						/*
						readingFunctionArguments = false;
						readingWhitespace = true;
						#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
						functionReferenceIndentationInHfileTemp = 0;
						#endif
						#ifdef CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIERS
						functionAccessSpecifier = "";
						functionAccessSpecifierType = CS_SUPPORT_INLINE_FUNCTION_ACCESS_SPECIFIER_UNKNOWN;
						#endif
						functionNameFull = "";
						*/
					}
					else if(c == '\n')
					{
						prepareNewLine = true;
					}
					else
					{
						functionArguments = functionArguments + c;
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
		}
		parseFileObject.close();
	}

	return fileFound;
}



#define MAX_CODE_FILE_SIZE (1000000)

void CSoperationsClass::getFunctionObjectNamesFromFunctionsInCfile(const CSfile* firstFileInIncludeFileList, CSfunction* firstFunctionInFunctionList, const CSfile* aboveLevelObject, const string topLevelFileName, const int level)
{
	string codeFileName = topLevelFileName;
	char* codeFileNamecharstar = const_cast<char*>(codeFileName.c_str());

	int codeFileByteArraySize;
	int codeFileNumberOfLines;

	string fileContentsString = "";

	if(!LDreferenceManipulation.readFileIntoString(codeFileName, &fileContentsString, &codeFileNumberOfLines, &codeFileByteArraySize))
	{
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

				//1. in c file, find fullfilename [identified in header];
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
							cerr << "getFunctionObjectNamesFromFunctionsInCfile{} error: function definitions in .c files must end with a } without any leading white space" << endl;
							exit(EXIT_ERROR);
						}

						currentIndexInFunction++;

					}
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
							cerr << "getFunctionObjectNamesFromFunctionsInCfile{} error: function definitions in .c files must end with a } without any leading white space" << endl;
							exit(EXIT_ERROR);
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

					//search current file for function references within functionContentsString;
					#ifdef CS_DEBUG_FUNCTION_REFERENCE_SEARCH
					cout << "searchFunctionStringForFunctionReferences: currentReference->nameFull = " << currentReference->nameFull << endl;
					#endif
					if(searchFunctionStringForFunctionReferences(firstFileInIncludeFileList, aboveLevelObject, &currentReferenceInFunctionReferenceList, &currentReferenceInFunctionReferenceListRepeats, &functionContentsString, true))
					{

					}
					
					#ifdef CS_DEBUG_IDENTIFY_INFINITE_INCLUDE_RECURSION_OLD
					cout << "searchFunctionStringForFunctionReferencesRecursive start: topLevelFileName = " << topLevelFileName << endl;
					#endif
					
					//search include files for function references within functionContentsString;
					if(searchFunctionStringForFunctionReferencesRecursive(firstFileInIncludeFileList, aboveLevelObject->firstFileInBelowListContainer, &currentReferenceInFunctionReferenceList, &currentReferenceInFunctionReferenceListRepeats, &functionContentsString))
					{

					}
					#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
					searchFunctionStringForFunctionReferencesRecursiveReset(aboveLevelObject->firstFileInBelowListContainer);
					#endif
					
					#ifdef CS_DEBUG_IDENTIFY_INFINITE_INCLUDE_RECURSION_OLD
					cout << "searchFunctionStringForFunctionReferencesRecursive end\n\n\n" << endl;
					#endif
				}
				else
				{
					cerr << "error: function not found in file:" << endl;
					cerr << "fullFunctionName = " << fullFunctionName << endl;
					cerr << "fileName = " << codeFileName << endl;
					exit(EXIT_ERROR);
				}
				currentReference->hasHadFunctionReferencesParsed = true;
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

bool CSoperationsClass::searchFunctionStringForFunctionReferencesRecursive(const CSfile* firstFileInIncludeFileList, const CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString)
{
	bool result = false;

	const CSfileContainer* currentReferenceContainer = firstFileNameInLayerContainingFunctionReferencesToSearchFor;
	//cout << "firstFileNameInLayerContainingFunctionReferencesToSearchFor->fileObject->name = " << firstFileNameInLayerContainingFunctionReferencesToSearchFor->fileObject->name << endl;

	while(currentReferenceContainer->next != NULL)
	{
		CSfile* currentReference = currentReferenceContainer->fileObject;
		
		#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
		if(!(currentReference->identifiedFunctionReferences))
		{
			currentReference->identifiedFunctionReferences = true;
		#endif
			if(searchFunctionStringForFunctionReferences(firstFileInIncludeFileList, currentReference, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString, false))
			{
				result = true;
			}

			if(currentReference->firstFileInBelowListContainer != NULL)
			{
				if(searchFunctionStringForFunctionReferencesRecursive(firstFileInIncludeFileList, currentReference->firstFileInBelowListContainer, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
				{
					result = true;
				}
			}
		#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
		}
		#endif

		currentReferenceContainer = currentReferenceContainer->next;
	}

	return result;

}

#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
void CSoperationsClass::searchFunctionStringForFunctionReferencesRecursiveReset(const CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor)
{
	const CSfileContainer* currentReferenceContainer = firstFileNameInLayerContainingFunctionReferencesToSearchFor;
	while(currentReferenceContainer->next != NULL)
	{
		CSfile* currentReference = currentReferenceContainer->fileObject;
		
		if(currentReference->identifiedFunctionReferences)
		{
			currentReference->identifiedFunctionReferences = false;

			if(currentReference->firstFileInBelowListContainer != NULL)
			{
				searchFunctionStringForFunctionReferencesRecursiveReset(currentReference->firstFileInBelowListContainer);
			}
		}
		
		currentReferenceContainer = currentReferenceContainer->next;
	}
}
#endif


bool CSoperationsClass::searchFunctionStringForFunctionReferences(const CSfile* firstFileInIncludeFileList, const CSfile* fileNameContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString, const bool currentFile)
{
	bool result = false;

	//only search those file names which include aboveLevelObject.h

	//find reference to this file

	//cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: " << endl;
	
	//now search function string for all functions within referenceFound;
	//const CSfunction* currentFunction = foundReference->firstFunctionInFunctionList;
	if(fileNameContainingFunctionReferencesToSearchFor->firstFunctionInFunctionList == NULL)	//added CS3i16b, removed _?, check readded 3o2a
	{
		cerr << "CSoperationsClass::searchFunctionStringForFunctionReferences error: (fileNameContainingFunctionReferencesToSearchFor->firstFunctionInFunctionList == NULL) - potential infinite include loop detected" << endl;
		exit(EXIT_ERROR);
	}
	else
	{
		CSfunction* currentFunction = fileNameContainingFunctionReferencesToSearchFor->firstFunctionInFunctionList;
		
		#ifdef CS_DEBUG_IDENTIFY_INFINITE_INCLUDE_RECURSION_OLD
		cout << "currentFunction->nameFull = " << currentFunction->nameFull << endl;
		#endif
		
		while(currentFunction->next != NULL)
		{
			//now search the function string for this function reference;

			//cout << "currentFunction->functionText = " << currentFunction->functionText << endl;

			bool firstTimeFound = true;
			
			#ifdef CS_SUPPORT_GENERATED_CPP_CODE_NO_BACKWARD_COMPATIBILITY
			if(currentFile)
			{
				//search for private (within class) function executions:			
			#endif
				string functionReferenceToFind = currentFunction->name + CHAR_OPEN_BRACKET;
				if(searchFunctionStringForFunctionReference(firstFileInIncludeFileList, currentFunction, functionReferenceToFind, &firstTimeFound, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
				{
					#ifdef CS_DEBUG_FUNCTION_REFERENCE_SEARCH
					cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: currentFile: local function found; functionReferenceToFind = " << functionReferenceToFind << endl;
					#endif
					result = true;
				}
			#ifdef CS_SUPPORT_GENERATED_CPP_CODE_NO_BACKWARD_COMPATIBILITY
			}
			#endif
			#ifdef CS_SUPPORT_GENERATED_CPP_CODE
			if(!currentFile)
			{
				//search for public (out of class) function executions:
				if(currentFunction->className != "")
				{
					int classNameLength = (currentFunction->className).length();
					string classNameAppendDetected = (currentFunction->className).substr(classNameLength-CS_CLASS_NAME_APPEND.length(),CS_CLASS_NAME_APPEND.length());
					if(classNameAppendDetected == CS_CLASS_NAME_APPEND)
					{
						//function class name ends with "Class" (function is expected to have been referenced via a default class object of name format: variant1/variant2)

						string classObjectNameBase = (currentFunction->className).substr(0, classNameLength-CS_CLASS_NAME_APPEND.length());

						//variant 1; class object declaration: [fileName]Class [fileName], functionNameToFind = [classNameMinusClass].functionName(
						string functionReferenceToFind = classObjectNameBase + CS_OBJECT_DELIMITER + currentFunction->name + CHAR_OPEN_BRACKET;
						//cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: !currentFile: variant 1 search; functionReferenceToFind = " << functionReferenceToFind << endl;
						if(searchFunctionStringForFunctionReference(firstFileInIncludeFileList, currentFunction, functionReferenceToFind, &firstTimeFound, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
						{
							#ifdef CS_DEBUG_FUNCTION_REFERENCE_SEARCH
							cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: !currentFile: variant 1 found; functionReferenceToFind = " << functionReferenceToFind << endl;
							#endif
							result = true;
						}

						//variant 2; class object declaration: [fileNameThatEndsWithClass]Class [fileNameThatEndsWithClass]Object, functionNameToFind = [classNameMinusClass]Object.functionName(
						functionReferenceToFind = classObjectNameBase + CS_CLASS_OBJECT_NAME_APPEND + CS_OBJECT_DELIMITER + currentFunction->name + CHAR_OPEN_BRACKET;
						//cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: !currentFile: variant 2 search; functionReferenceToFind = " << functionReferenceToFind << endl;
						if(searchFunctionStringForFunctionReference(firstFileInIncludeFileList, currentFunction, functionReferenceToFind, &firstTimeFound, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
						{
							#ifdef CS_DEBUG_FUNCTION_REFERENCE_SEARCH
							cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: !currentFile: variant 2 found; functionReferenceToFind = " << functionReferenceToFind << endl;
							#endif
							result = true;
						}
						
						#ifdef CS_SUPPORT_GENERATED_CPP_CODE_SUPPORT_INLINE_CLASS_OBJECT_DECLARATION
						//variant 3; class object declaration inline: [fileName]Class(), functionNameToFind = [classNameMinusClass]().functionName(
						functionReferenceToFind = currentFunction->className + CS_CLASS_OBJECT_INLINE_DECLARATION_APPEND + CS_OBJECT_DELIMITER + currentFunction->name + CHAR_OPEN_BRACKET;
						//cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: !currentFile: variant 3 search; functionReferenceToFind = " << functionReferenceToFind << endl;
						if(searchFunctionStringForFunctionReference(firstFileInIncludeFileList, currentFunction, functionReferenceToFind, &firstTimeFound, currentReferenceInFunctionReferenceList, currentReferenceInFunctionReferenceListRepeats, functionContentsString))
						{
							#ifdef CS_DEBUG_FUNCTION_REFERENCE_SEARCH
							cout << "CSoperationsClass::searchFunctionStringForFunctionReferences: !currentFile: variant 3 found; functionReferenceToFind = " << functionReferenceToFind << endl;
							#endif
							result = true;
						}
						#endif
					}
					else
					{
						//class name does not end with Class (no default class object declared)
					}
				}
			}
			#endif
			
			currentFunction = currentFunction->next;
		}
	}

	return result;
}

bool CSoperationsClass::searchFunctionStringForFunctionReference(const CSfile* firstFileInIncludeFileList, const CSfunction* currentFunction, const string functionReferenceToFind, bool* firstTimeFound, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString)
{
	bool result = false;

	int startPosOfFunctionReferenceInFunction = 0;

	while(CPP_STRING_FIND_RESULT_FAIL_VALUE != (startPosOfFunctionReferenceInFunction = functionContentsString->find(functionReferenceToFind, startPosOfFunctionReferenceInFunction)))
	{
		//added condition CS 3f1b - ensure previous character is not a letter (this ensures that ABCfunctionName is not found when searching for functionName)
		if((startPosOfFunctionReferenceInFunction == 0) || !SHAREDvars.charInCharArray((*functionContentsString)[startPosOfFunctionReferenceInFunction-1], functionOrVariableNameCharacters, CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES))
		{
			result = true;
			
			//function reference found, add it to the function reference list of the funciton;
			
			#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
			if(firstTimeFound)
			{
			#endif
				#ifdef CS_DEBUG_IDENTIFY_CRASH_BAD_FUNCTION_REFERENCE_IN_COMMENTS
				cout << "currentFunction->nameFull = " << currentFunction->nameFull << endl;
				#endif
				
				(*currentReferenceInFunctionReferenceList)->name = currentFunction->name;
				(*currentReferenceInFunctionReferenceList)->isFunctionReference = true;
				#ifdef CS_SUPPORT_GENERATED_CPP_CODE
				(*currentReferenceInFunctionReferenceList)->className = currentFunction->className;
				#endif
				#ifdef CS_MATCH_FUNCTION_REFERENCES_WITH_CORRECT_NUMBER_OF_ARGUMENTS
				identifyFunctionReferenceArguments((*currentReferenceInFunctionReferenceList), functionContentsString, startPosOfFunctionReferenceInFunction, functionReferenceToFind);
				#endif

				CSfunction* newReferenceInFunctionReferenceList = new CSfunction();
				(*currentReferenceInFunctionReferenceList)->next = newReferenceInFunctionReferenceList;
				(*currentReferenceInFunctionReferenceList) = (*currentReferenceInFunctionReferenceList)->next;

				*firstTimeFound = false;
			#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
			}
			#endif
	
			#ifdef CS_DEBUG_IDENTIFY_INFINITE_INCLUDE_RECURSION_OLD
			cout << "found function reference in functionContents: functionReferenceToFind = " << functionReferenceToFind << endl;
			#endif
			
			#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
			(*currentReferenceInFunctionReferenceListRepeats)->name = currentFunction->name;
			(*currentReferenceInFunctionReferenceListRepeats)->isFunctionReference = true;
			#ifdef CS_SUPPORT_GENERATED_CPP_CODE
			(*currentReferenceInFunctionReferenceListRepeats)->className = currentFunction->className;
			#endif
				
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
			identifyFunctionReferenceArguments((*currentReferenceInFunctionReferenceListRepeats), functionContentsString, startPosOfFunctionReferenceInFunction, functionReferenceToFind);
			#endif

			CSfunction* newReferenceInFunctionReferenceListRepeats = new CSfunction();
			(*currentReferenceInFunctionReferenceListRepeats)->next = newReferenceInFunctionReferenceListRepeats;
			(*currentReferenceInFunctionReferenceListRepeats) = (*currentReferenceInFunctionReferenceListRepeats)->next;
			#endif

			startPosOfFunctionReferenceInFunction++;
		}
		else
		{
			startPosOfFunctionReferenceInFunction++;
		}
	}

	return result;
}


//limitations; doesn't support couts containing function references, e.g. "cout << "Error: getFloatArgument(" << keystr << ")" << endl;" (NB '<' is interpreted as CLASS_TYPE_OPEN_TAG)
void CSoperationsClass::identifyFunctionReferenceArguments(CSfunction* currentReferenceInFunctionReferenceList, string* functionContentsString, int indexToFunctionObject, const string functionReferenceToFind)
{
	//designed to support embedded function references (although this is not currently used by CSgenerateConstFunctionArgumentCode)

	CSfunctionArgument* currentFunctionArgumentInFunctionReference = currentReferenceInFunctionReferenceList->firstFunctionArgumentInFunction;

	string functionName = currentReferenceInFunctionReferenceList->name;

	int startPositionOfFunctionBrackets = functionContentsString->find(CHAR_OPEN_BRACKET, indexToFunctionObject+functionReferenceToFind.length()-1);
	int endPositionOfFunctionBracketsTemp = functionContentsString->find(CHAR_CLOSE_BRACKET, indexToFunctionObject+functionReferenceToFind.length()-1);
	if(startPositionOfFunctionBrackets != indexToFunctionObject+functionReferenceToFind.length()-1)
	{
		cerr << "identifyFunctionReferenceArguments{} error: startPositionOfFunctionBrackets != indexToFunctionObject+functionName.length()" << endl;
		cerr << "functionContentsString = " << *functionContentsString << endl;
		cerr << "functionReferenceToFind = " << functionReferenceToFind << endl;
		exit(EXIT_ERROR);
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
		currentReferenceInFunctionReferenceList->nameFull = functionContentsString->substr(indexToFunctionObject, pos-indexToFunctionObject);
	}
	else
	{
		currentReferenceInFunctionReferenceList->nameFull = functionContentsString->substr(indexToFunctionObject, endPositionOfFunctionBracketsTemp-indexToFunctionObject+1);
	}
}

//NB this function is not necessary for the current implementation, but is used for neatness
string CSoperationsClass::removePrependedWhiteSpace(const string argument)
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

#ifdef CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS
void CSoperationsClass::identifyFunctionDeclarationArguments(CSfunction* currentReferenceInFunctionReferenceList, const string* functionNameFull)
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

		int startPositionOfFunctionArguments = startPositionOfFunctionBrackets+1;
		int endPositionOfFunctionArguments = endPositionOfFunctionBrackets;
		string functionArgumentsRaw = functionNameFull->substr(startPositionOfFunctionArguments, endPositionOfFunctionArguments-startPositionOfFunctionArguments+1);

		bool stillParsingArguments = true;
		int startPositionOfArgument = 0;
		while(stillParsingArguments)
		{
			bool lastArgument = false;
			int endPositionOfArgument = CSreferenceContainerClass.findEndPositionOfArgument(&functionArgumentsRaw, startPositionOfArgument, &lastArgument);
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
				cerr << "generateHTMLdocumentationFunctionInputArguments{} error: (startPositionOfArgumentName == CPP_STRING_FIND_RESULT_FAIL_VALUE)" << endl;
				exit(EXIT_ERROR);
			}
			//string currentArgumentName = currentArgument.substr(startPositionOfArgumentName, endPositionOfArgument-startPositionOfArgumentName);
			string currentArgumentName = extractFunctionArgumentName(&currentArgument, startPositionOfArgumentName);	//updated CS3h9b - this is required to support array arguments eg "typeX* arrayName[]"
			string currentArgumentType = currentArgument.substr(0, startPositionOfArgumentName);


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


string CSoperationsClass::generateSourceFileNameFromHeaderFileName(const string headerFileName, const string sourceFileNameExtension)
{
	string sourceFileName = "";
	int positionOfExtension = headerFileName.rfind(CHAR_FULLSTOP);
	if(positionOfExtension != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		sourceFileName = headerFileName.substr(0, positionOfExtension+1) + sourceFileNameExtension;
	}
	return sourceFileName;
}





void CSoperationsClass::attachFunctionReferenceTargets(CSfileContainer* firstObjectInAboveLevelBelowListContainer, int level)
{
	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;
	//cout << "attachFunctionReferenceTargets{}:" << endl;

	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;
		
		#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
		if(!(currentFileObject->attachedReferenceTargets))
		{
			currentFileObject->attachedReferenceTargets = true;
		#endif
		
			#ifdef CS_DISPLAY_INCLUDE_FUNCTION_PARSING
			printLevelIndentation(level);
			cout << "CSoperationsClass::attachFunctionReferenceTargets: currentFileObject = " << currentFileObject->name << endl;
			#endif
			
			CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
			while(currentFunctionObject->next != NULL)
			{
				//cout << "currentFunctionObject = " << currentFunctionObject->name << endl;
				
				CSfunction* currentFunctionReference = currentFunctionObject->firstReferenceInFunctionReferenceList;
				while(currentFunctionReference->next != NULL)
				{
					//printLevelIndentation(level+1);
					//cout << "\t1 currentFunctionReference = " << currentFunctionReference->nameFull << endl;
					
					CSfile* fileObjectHoldingFunction = NULL;
					CSfunction* functionReferenceTarget = NULL;		//function pertaining to currentFunctionReference
					if(CSreferenceContainerClass.findFunctionReferenceTarget(currentFunctionReference, currentFileObject, &fileObjectHoldingFunction, &functionReferenceTarget, true))
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
					//printLevelIndentation(level+1);
					//cout << "\t2 currentFunctionReference = " << currentFunctionReference->nameFull << endl;
					
					CSfile* fileObjectHoldingFunction = NULL;
					CSfunction* functionReferenceTarget = NULL;		//function pertaining to currentFunctionReference
					if(CSreferenceContainerClass.findFunctionReferenceTarget(currentFunctionReference, currentFileObject, &fileObjectHoldingFunction, &functionReferenceTarget, true))
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
				attachFunctionReferenceTargets(currentFileObject->firstFileInBelowListContainer, level+1);
			}
		#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
		}
		#endif

		currentFileObjectContainer = currentFileObjectContainer->next;
	}
}


string CSoperationsClass::extractFunctionArgumentName(string* argumentText, const int indexOfStartOfVariableName)
{
	string fullVariableName = "";
	int i = indexOfStartOfVariableName;
	bool stillFindingVariableNameCharacters = true;
	while(stillFindingVariableNameCharacters)
	{
		bool foundValidChar = false;
		for(int j=0; j<CS_FUNCTION_OR_VARIABLE_NAME_CHARACTERS_NUMBER_OF_TYPES; j++)
		{
			if((*argumentText)[i] == functionOrVariableNameCharacters[j])
			{
				foundValidChar = true;
			}
		}

		if((*argumentText)[i] == '*')
		{
			#ifdef CS_SUPPORT_POINTER_TYPE_DECLARATIONS_WITH_SPACE_BEFORE_RATHER_THAN_AFTER_ASTERIX
			foundValidChar = true;
			#else
			cerr << "!CS_SUPPORT_POINTER_TYPE_DECLARATIONS_WITH_SPACE_BEFORE_RATHER_THAN_AFTER_ASTERIX: extractFunctionArgumentName{} error: * character found; pointer has been defined next to variable name instead of variable type" << endl;
			exit(EXIT_ERROR);
			#endif
		}

		if(foundValidChar)
		{
			fullVariableName = fullVariableName + (*argumentText)[i];
		}
		else
		{
			stillFindingVariableNameCharacters = false;
		}
		if(i >= argumentText->length())
		{
			stillFindingVariableNameCharacters = false;
		}
		i++;
	}
	return fullVariableName;
}

void CSoperationsClass::printLevelIndentation(int level)
{
	for(int i= 0; i<level; i++)
	{
		cout << "\t";
	}
}
			
			
