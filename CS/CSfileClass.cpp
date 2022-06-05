 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSfileClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#include "CSfileClass.hpp"

CSfile::CSfile(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	printedFileConnections = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	HTMLgenerated = false;

	firstReferenceContainerInAboveReferenceList = NULL;	//new CSfileContainer()  - no longer possible with forward declaration of CSfileContainer
	attachedReferenceTargets = false;
	printedTrace = false;
	printedTraceReset = false;
	for(int i=0; i<MAX_INCLUDE_DEPTH_FILE; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}

	//file only
	firstFileInBelowListContainer = NULL;
	firstFunctionInFunctionList = NULL;

	#ifdef CS_GENERATE_CODE_GENERIC
	printedCodeGeneric = false;
	sourceFileNameOrig = "";
	sourceFileTextOrig = "";
	headerFileTextOrig = "";
	headerFileName = "";
	sourceFileName = "";
	sourceFileText = "";
	headerFileText = "";
	#endif
	nameBase = "";
	
	#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
	identifiedFunctionReferences = false;
	#endif
	#ifdef CS_OPTIMISE_CS_DRAW_YMAXPOS_SEARCH
	drawSearched = false;
	#endif
	#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
	functionReferenceTargetSearched = false;
	#endif
	
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASSES
	//classList = NULL;
	#endif
}

CSfile::~CSfile(void)
{
	if(firstFileInBelowListContainer != NULL)
	{
		delete firstFileInBelowListContainer;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
}



void CSfileClassClass::printCSFileObject(const CSfile* ref)
{
	cout << "col = " << ref->col << endl;
	cout << "id = " << ref->id << endl;
	cout << "printed = " << ref->printed << endl;
	cout << "printX = " << ref->printX << endl;
	cout << "printY = " << ref->printY << endl;
	cout << "printXIndex = " << ref->printXIndex << endl;
	cout << "printYIndex = " << ref->printYIndex << endl;
	cout << "printTextX = " << ref->printTextX << endl;
	cout << "printTextY = " << ref->printTextY << endl;
	cout << "level = " << ref->level << endl;
	cout << "name = " << ref->name << endl;
}

