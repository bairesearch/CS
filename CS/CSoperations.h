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
 * File Name: CSoperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3c9a 06-February-2014
 *
 *******************************************************************************/

#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.h"
#include "CSglobalDefs.h"
#include "CSreferenceClass.h"

#define CS_SOURCE_FILE_EXTENSION_CHARACTER_1 'c'
#define CS_SOURCE_FILE_EXTENSION_CHARACTER_2 'p'
#define CS_SOURCE_FILE_EXTENSION_CHARACTER_3 'p'
#define CS_SOURCE_FILE_INDENTATION_CHARACTER '\t'

bool getIncludeFileNamesFromCorHfile(CSfileReference * firstReferenceInIncludeFileList, CSfileReference * topLevelReference, string topLevelReferenceName, int level);
	bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunctionReference * firstReferenceInFunctionList, string topLevelFileName, int level);
	void getFunctionReferenceNamesFromFunctionsInCfile(CSfileReference * firstReferenceInIncludeFileList, CSfunctionReference * firstReferenceInFunctionList, CSfileReference * topLevelReference, string topLevelFileName, int level);
		CSfunctionReference * searchFunctionStringForFunctionReferencesRecursive(CSfileReference * firstReferenceInIncludeFileList, CSfileReference * firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunctionReference * firstReferenceInFunctionReferenceList, string functionContentsString);
			CSfunctionReference * searchFunctionStringForFunctionReferences(CSfileReference * firstReferenceInIncludeFileList, CSfileReference * fileNameContainingFunctionReferencesToSearchFor, CSfunctionReference * firstReferenceInFunctionReferenceList, string functionContentsString);


CSfileReference * findReferenceInIncludeFileList(CSfileReference * firstReferenceInAboveLevelBelowList, string referenceName, CSfileReference * foundReference, bool * referenceFound);



#endif

