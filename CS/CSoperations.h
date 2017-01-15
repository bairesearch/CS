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
 * File Name: CSoperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3f3a 10-July-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.h"
#include "CSglobalDefs.h"
#include "CSreferenceClass.h"
#include "CSreferenceContainerClass.h"

bool getIncludeFileNamesFromCorHfile(CSfileReferenceContainer* firstReferenceInIncludeFileListContainer, CSfileReferenceContainer* topLevelReferenceContainer, CSfileReference* aboveLevelReference, string topLevelReferenceName, int level);
	bool findFileReferenceInFileReferenceContainerList(CSfileReferenceContainer* firstReferenceContainerInLevel, string fileReferenceName, CSfileReference** fileReferenceFound);
	bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunctionReference* firstReferenceInFunctionList, string topLevelFileName, int level);
	void getFunctionReferenceNamesFromFunctionsInCfile(CSfileReference* firstReferenceInIncludeFileList, CSfunctionReference* firstReferenceInFunctionList, CSfileReference* aboveLevelReference, string topLevelFileName, int level);
		bool searchFunctionStringForFunctionReferencesRecursive(CSfileReference* firstReferenceInIncludeFileList, CSfileReferenceContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunctionReference** currentReferenceInFunctionReferenceList, CSfunctionReference** currentReferenceInFunctionReferenceListRepeats, string functionContentsString);
			bool searchFunctionStringForFunctionReferences(CSfileReference* firstReferenceInIncludeFileList, CSfileReference* fileNameContainingFunctionReferencesToSearchFor, CSfunctionReference** currentReferenceInFunctionReferenceList, CSfunctionReference** currentReferenceInFunctionReferenceListRepeats, string functionContentsString);

/*
CSfileReference* findReferenceInIncludeFileList(CSfileReference* firstReferenceInAboveLevelBelowList, string referenceName, CSfileReference* foundReference, bool* referenceFound);
*/

string generateSourceFileNameFromHeaderFileName(string headerFileName, string sourceFileNameExtension);

#endif

