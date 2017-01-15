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
 * but WITHOUT ANY WARRANTY; without even trhe implied warranty of
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
 * File Name: CSreferenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3e4a 02-September-2014
 *
 *******************************************************************************/

#ifndef HEADER_CS_REFERENCE_CLASS
#define HEADER_CS_REFERENCE_CLASS

#include "CSglobalDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;


#define MAX_INCLUDE_DEPTH (20)	//file
#define MAX_INCLUDE_DEPTH_FUNCTION (20)	//for a function within a file


#define CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF (0)
#define CS_GENERATE_HTML_DOCUMENTATION_MODE_ON (1)

/*
#define CS_GENERATE_HTML_DOCUMENTATION_MODE_SINGLE_FUNCTION
#define CS_GENERATE_HTML_DOCUMENTATION_MODE_ALL_FUNCTIONS
*/



class CSfunctionReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfunctionReferenceContainer(void); // constructor declaration
	~CSfunctionReferenceContainer();	//  and destructor.

	CSfunctionReferenceContainer * next;
	string name;

};

class CSfunctionReference{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfunctionReference(void); // constructor declaration
	~CSfunctionReference();	//  and destructor.

	//file/function reference [shared]
	int id;			//file/function id
	int level;		//file/function Level
	string name;		//file/function
	int col;		//	file/function colour + connection colour to all children
	bool printed;		//file/function printed
	bool printedFunctionConnections; 	//file/function connections printed
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
	bool singleFileOnlyPrinted;	//file/function printed
	#endif
	int printX;		//file/function printX
	int printY;		//file/function printY
	int printXIndex;	//file/function printXIndex
	int printYIndex;	//file/function printYIndex
	int printTextX;		//file/function printXIndex
	int printTextY;		//file/function printYIndex
	int filePrintXrecordForSingleFileOnly;	//file/function printX
	int filePrintYrecordForSingleFileOnly;	//file/function printY
	bool HTMLgenerated;	//file/function HTML generated

	CSfunctionReference * next;
	CSfunctionReference * previous;
	CSfunctionReferenceContainer * firstReferenceContainerInAboveReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation

	//function reference only
	string nameFull;	//function only
	CSfunctionReference * firstReferenceInFunctionReferenceList;
	bool hasHadFunctionReferencesParsed;
	bool isFunctionReference;
	bool isFunctionReferenceReference;
	int numTopLevelFunctionsInFileAlreadyPrinted;
	#ifdef CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
	int functionReferenceIndentation;
	#endif
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	string functionText;
	#endif
	#ifdef CS_GENERATE_CPP_CLASSES 
	string functionTextRaw;	//function contents without comments
	string headerFunctionNameFullUpdated;
	string sourceFunctionNameFullUpdated;
	#endif
	
	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FUNCTION];
};


class CSfileReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfileReferenceContainer(void); // constructor declaration
	~CSfileReferenceContainer();	//  and destructor.

	CSfileReferenceContainer * next;
	string name;

};

class CSfileReference{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfileReference(void); // constructor declaration
	~CSfileReference();	//  and destructor.

	//file/function reference [shared]
	int id;			//file/function id
	int level;		//file/function Level
	string name;		//file/function (headerFileNameOrig, eg "x.h")
	int col;		//	file/function colour + connection colour to all children
	bool printed;		//file/function printed
	bool printedFileConnections;	//file/function connections printed
	bool printedFileConnection;
	int printX;		//file/function printX
	int printY;		//file/function printY
	int printXIndex;	//file/function printXIndex
	int printYIndex;	//file/function printYIndex
	int printTextX;		//file/function printXIndex
	int printTextY;		//file/function printYIndex
	bool HTMLgenerated;	//file/function HTML generated

	CSfileReference * next;
	CSfileReference * previous;
	CSfileReferenceContainer * firstReferenceContainerInAboveReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation

	//file reference only
	CSfileReference * firstReferenceInAboveList;	//filefirstReferenceInAboveList
	CSfileReference * firstReferenceInBelowList; 	//filefirstReferenceInBelowList
	CSfileReference * shortcutToPrintedVersionOfReference;	//used if reference is not printed
	CSfunctionReference * firstReferenceInFunctionList;

	#ifdef CS_GENERATE_CPP_CLASSES
	string sourceFileNameOrig;	//eg "x.c"
	string sourceFileTextOrig;
	string headerFileTextOrig;
	string headerFileName;	//eg "x.hpp"
	string sourceFileName;	//eg "x.cpp"
	string sourceFileText;
	string headerFileText;
	#endif
	
	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FUNCTION];
};





void printCSFileReference(CSfileReference * ref);
void printCSFunctionReference(CSfunctionReference * ref);

typedef struct { CSfileReference * csref1; CSfunctionReference * csref2; } CSreferenceArray;

/*
//these functions are not being used:
void addReferenceToReferenceList(CSReference * reference, CSReference * firstReferenceInList);
void mergeIncludeFileReferences(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInTopLevelBelowList);
	void findAndMergeAllIndenticalFileReferences(CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList);
		void mergeReferenceLists(CSReference * firstReferenceInList, CSReference * firstReferenceInListToMerge);
*/

#endif

