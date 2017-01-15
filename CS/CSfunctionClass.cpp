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
 * File Name: CSfunctionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h1e 14-November-2015
 *
 *******************************************************************************/

#include "CSfunctionClass.h"



#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
CSfunctionArgument::CSfunctionArgument(void)
{
	argumentName = "";
	argumentType = "";
	isConst = false;
	constIdentified = false;
	next = NULL;
}
#endif

CSfunction::CSfunction(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	printedFunctionConnections = false;
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
	singleFileOnlyPrinted = false;
	#endif
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	filePrintXrecordForSingleFileOnly = 0;
	filePrintYrecordForSingleFileOnly = 0;
	HTMLgenerated = false;

	firstReferenceContainerInAboveReferenceList = NULL;	//new CSfunctionContainer()  - no longer possible with forward declaration of CSfunctionContainer
	printedTrace = false;
	printedTraceReset = false;
	for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}
	
	//function/functionReference
	isFunction = false;
	isFunctionReference = false;
	nameFull = "";
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	firstFunctionArgumentInFunction = new CSfunctionArgument();
	#endif
		
	//function only
	next = NULL;
	previous = NULL;
	firstReferenceInFunctionReferenceList = NULL;
	hasHadFunctionReferencesParsed = false;
	numTopLevelFunctionsInFileAlreadyPrinted = 0;
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	string functionText;
	firstReferenceInFunctionReferenceListRepeats = NULL;
	#endif
	#ifdef CS_GENERATE_CODE_GENERIC 
	functionTextRaw = "";
	headerFunctionNameFullUpdated = "";
	sourceFunctionNameFullUpdated = "";
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	functionArgumentConstsIdentified = false;
	#endif
	
	//function reference only
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	functionReferenceCharacterIndex = 0;
	#endif
	#ifdef CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
	functionReferenceIndentation = 0;
	#endif
	functionReferenceTarget = NULL;
	functionReferenceTargetFileOwner = NULL;
	
}

CSfunction::~CSfunction(void)
{
	/*
	if(firstFileInBelowListContainer != NULL)
	{
		delete firstFileInBelowListContainer;
	}
	*/
	if(next != NULL)
	{
		delete next;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
	if(firstReferenceInFunctionReferenceList != NULL)
	{
		delete firstReferenceInFunctionReferenceList;	//added 3e5a
	}
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	if(firstReferenceInFunctionReferenceListRepeats != NULL)
	{
		delete firstReferenceInFunctionReferenceListRepeats;	//added 3e5a
	}
	#endif
}


void printCSFunctionObject(CSfunction* ref)
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
	cout << "nameFull = " << ref->nameFull << endl;
}


