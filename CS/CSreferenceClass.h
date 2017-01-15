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
 * Project Version: 3c3d 17-November-2012
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



class CSFunctionReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSFunctionReferenceContainer(void); // constructor declaration
	~CSFunctionReferenceContainer();	//  and destructor.

	CSFunctionReferenceContainer * next;
	string name;

};

class CSFunctionReference{
private:
	/*There are currently no private attributes of this class*/
public:

	CSFunctionReference(void); // constructor declaration
	~CSFunctionReference();	//  and destructor.

	//file/function reference [shared]
	int id;			//file/function id
	int level;		//file/function Level
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
	
	CSFunctionReference * next;
	CSFunctionReference * previous;
	CSFunctionReferenceContainer * firstReferenceContainerInAboveReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation
	
	//function reference only
	string nameFull;	//function only
	CSFunctionReference * firstReferenceInFunctionReferenceList;
	bool isFunctionReference;
	bool isFunctionReferenceReference;
	int numTopLevelFunctionsInFileAlreadyPrinted;
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
	int functionReferenceIndentationInHfile;
	#endif
	
	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FUNCTION];

};


class CSFileReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSFileReferenceContainer(void); // constructor declaration
	~CSFileReferenceContainer();	//  and destructor.

	CSFileReferenceContainer * next;
	string name;

};

class CSFileReference{
private:
	/*There are currently no private attributes of this class*/
public:

	CSFileReference(void); // constructor declaration
	~CSFileReference();	//  and destructor.

	//file/function reference [shared]
	int id;			//file/function id
	int level;		//file/function Level
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
	
	CSFileReference * next;
	CSFileReference * previous;
	CSFileReferenceContainer * firstReferenceContainerInAboveReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation
	
	//file reference only
	CSFileReference * firstReferenceInAboveList;	//filefirstReferenceInAboveList
	CSFileReference * firstReferenceInBelowList; 	//filefirstReferenceInBelowList
	CSFileReference * shortcutToPrintedVersionOfReference;	//used if reference is not printed
	CSFunctionReference * firstReferenceInFunctionList;

	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FUNCTION];

};





void printCSFileReference(CSFileReference * ref);
void printCSFunctionReference(CSFunctionReference * ref);

typedef struct { CSFileReference * csref1; CSFunctionReference * csref2; } CSReferenceArray;

/*
//these functions are not being used:
void addReferenceToReferenceList(CSReference * reference, CSReference * firstReferenceInList);
void mergeIncludeFileReferences(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInTopLevelBelowList);
	void findAndMergeAllIndenticalFileReferences(CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList);
		void mergeReferenceLists(CSReference * firstReferenceInList, CSReference * firstReferenceInListToMerge);
*/

#endif

