 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSfunctionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#include "CSfunctionClass.hpp"



CSfunctionArgument::CSfunctionArgument(void)
{
	argument = "";
	argumentName = "";
	argumentType = "";
	isNotConst = false;
	//#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
	isConstEffective = false;
	//#endif
	constIdentified = false;
	next = NULL;
}

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
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE
	nameFullHeader = "";
	#endif

	firstFunctionArgumentInFunction = new CSfunctionArgument();
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	functionArgumentConstsIdentified = false;
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	parseSecondaryReferencesOnly = false;
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION
	parseSecondaryReferencesAndModificationsOnly = false;
	#endif
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


	//function reference only
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	functionReferenceCharacterIndex = 0;
	#endif
	#ifdef CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
	functionReferenceIndentation = 0;
	#endif
	functionReferenceTarget = NULL;
	functionReferenceTargetFileOwner = NULL;

	functionType = "";
	#ifdef CS_USE_FUNCTION_ARGUMENTS_STRING
	functionArgumentsString = "";
	#endif
	#ifdef CS_USE_FUNCTION_ARGUMENTS_PARAMETER_LIST
	//functionArgumentsParameterList = NULL;
	#endif
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE
	className = "";
	#ifdef CS_SUPPORT_FUNCTION_ACCESS_SPECIFIERS
	functionAccessSpecifierType = CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_UNKNOWN;
	#endif
	//nameWithClass = "";
	#endif
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


void CSfunctionClassClass::printCSFunctionObject(const CSfunction* ref)
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


