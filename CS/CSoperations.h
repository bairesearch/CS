/*******************************************************************************
 *
 * File Name: CSoperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a6a 20-Mar-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.h"
#include "SHAREDglobalDefs.h"
#include "CSreferenceClass.h"




void getIncludeFileNamesFromCorHFile(CSReference * firstReferenceInIncludeFileList, CSReference * topLevelReference, string topLevelReferenceName, int level);
	void getFunctionNamesFromFunctionDeclarationsInHFile(CSReference * firstReferenceInFunctionList, string topLevelFileName, int level);
	void getFunctionReferenceNamesFromFunctionsInCFile(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInFunctionList, CSReference * topLevelReference, string topLevelFileName, int level);
		CSReference * searchFunctionStringForFunctionReferencesRecursive(CSReference * firstReferenceInIncludeFileList, CSReference * firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSReference * firstReferenceInFunctionReferenceList, string functionContentsString);
			CSReference * searchFunctionStringForFunctionReferences(CSReference * firstReferenceInIncludeFileList, CSReference * fileNameContainingFunctionReferencesToSearchFor, CSReference * firstReferenceInFunctionReferenceList, string functionContentsString);


CSReference * findReferenceInIncludeFileList(CSReference * firstReferenceInAboveLevelBelowList, string referenceName, CSReference * foundReference, bool * referenceFound);



#endif

