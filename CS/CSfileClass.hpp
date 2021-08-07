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
 * File Name: CSfileClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3p4a 07-August-2021
 * /
 *******************************************************************************/


#ifndef HEADER_CS_FILE_CLASS
#define HEADER_CS_FILE_CLASS

#include "CSglobalDefs.hpp"
#include "CSclassClass.hpp"


#define MAX_INCLUDE_DEPTH_FILE (20)	//file


class CSfunction;
class CSfileContainer;
class CSfile{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfile(void); // constructor declaration
	~CSfile(void);	//  and destructor.

	//file/function [shared]
	int id;			//file/function id
	int level;		//file/function Level
	string name;		//file/function (headerFileNameOrig, eg "x.h")
	int col;		//	file/function colour + connection colour to all children
	bool printed;		//file/function printed
	bool printedFileConnections;	//file/function connections printed
	int printX;		//file/function printX
	int printY;		//file/function printY
	int printXIndex;	//file/function printXIndex
	int printYIndex;	//file/function printYIndex
	int printTextX;		//file/function printXIndex
	int printTextY;		//file/function printYIndex
	bool HTMLgenerated;	//file/function HTML generated

	CSfileContainer* firstReferenceContainerInAboveReferenceList;
	bool attachedReferenceTargets;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation
	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FILE];

	//file only
	CSfileContainer* firstFileInBelowListContainer; 	//filefirstReferenceInBelowList
	CSfunction* firstFunctionInFunctionList;

	#ifdef CS_GENERATE_CODE_GENERIC
	bool printedCodeGeneric;			//added @CS3f1a to only print CPP files once (don't overwrite)
	string sourceFileNameOrig;	//eg "x.c"
	string sourceFileTextOrig;
	string headerFileTextOrig;
	string headerFileName;	//eg "x.h"
	string sourceFileName;	//eg "x.c"
	string sourceFileText;
	string headerFileText;
	#endif
	string nameBase;	//added CS3o2a	- name without extension
	
	#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
	bool identifiedFunctionReferences;
	#endif
	#ifdef CS_OPTIMISE_CS_DRAW_YMAXPOS_SEARCH
	bool drawSearched;
	#endif
	#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
	bool functionReferenceTargetSearched;
	#endif
	
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASSES
	vector<CSclass*> classList;
	#endif
};

class CSfileClassClass
{
	private: void printCSFileObject(const CSfile* ref);
};


#endif

