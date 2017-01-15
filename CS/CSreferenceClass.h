/*******************************************************************************
 *
 * File Name: CSreferenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a6c 07-May-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_REFERENCE_CLASS
#define HEADER_CS_REFERENCE_CLASS

#include "SHAREDglobalDefs.h"

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

	CSReference * next;
	CSReference * previous;
	CSReferenceContainer * firstReferenceContainerInAboveFileOrFunctionReferenceList;
	bool printedTrace;

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

	//not used;
void addReferenceToReferenceList(CSReference * reference, CSReference * firstReferenceInList);
void mergeIncludeFileReferences(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInTopLevelBelowList);
	void findAndMergeAllIndenticalFileReferences(CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList);
		void mergeReferenceLists(CSReference * firstReferenceInList, CSReference * firstReferenceInListToMerge);


#endif

