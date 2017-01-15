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
 * File Name: CSreferenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3b2b 30-September-2012
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

class CSReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSReferenceContainer(void); // constructor declaration
	~CSReferenceContainer();	//  and destructor.

	CSReferenceContainer * next;
	string name;

};

class CSReference{
private:
	/*There are currently no private attributes of this class*/
public:

	CSReference(void); // constructor declaration
	~CSReference();	//  and destructor.

	//file/function reference [shared]
	int id;			//file/function id
	int level;		//file/funciton Level
	string name;		//file/function
	int col;		//	file/function colour + connection colour to all children
	bool printed;		//file/function printed
	int printX;		//file/function printX
	int printY;		//file/function printY
	int printXIndex;	//file/function printXIndex
	int printYIndex;	//file/function printYIndex
	int printTextX;		//file/function printXIndex
	int printTextY;		//file/function printYIndex
	bool HTMLgenerated;	//file/function HTML generated
	
	CSReference * next;
	CSReference * previous;
	CSReferenceContainer * firstReferenceContainerInAboveFileOrFunctionReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation
	
	//file reference only
	CSReference * firstReferenceInAboveList;	//filefirstReferenceInAboveList
	CSReference * firstReferenceInBelowList; 	//filefirstReferenceInBelowList
	CSReference * shortcutToPrintedVersionOfReference;	//used if reference is not printed
	bool isFileReference;
	CSReference * firstReferenceInFunctionList;

	//function reference only
	string nameFull;	//function only
	CSReference * firstReferenceInFunctionReferenceList;
	bool isFunctionReference;
	bool isFunctionReferenceReference;
	int numTopLevelFunctionsInFileAlreadyPrinted;

	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FUNCTION];

};




void printCSReference(CSReference * ref);

typedef struct { CSReference * csref1, * csref2; } CSReferenceArray;

/*
//these functions are not being used:
void addReferenceToReferenceList(CSReference * reference, CSReference * firstReferenceInList);
void mergeIncludeFileReferences(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInTopLevelBelowList);
	void findAndMergeAllIndenticalFileReferences(CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList);
		void mergeReferenceLists(CSReference * firstReferenceInList, CSReference * firstReferenceInListToMerge);
*/

#endif

