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
 * Project Version: 3h5a 03-December-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.h"
#include "CSglobalDefs.h"
#include "CSreferenceContainerClass.h"

bool getIncludeFileNamesFromCorHfile(CSfileContainer* firstReferenceInIncludeFileListContainer, CSfileContainer* topLevelReferenceContainer, CSfile* aboveLevelObject, string topLevelReferenceName, int level);
	bool findFileObjectInFileObjectContainerList(CSfileContainer* firstReferenceContainerInLevel, string fileReferenceName, CSfile** fileReferenceFound);
	bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunction* firstFunctionInFunctionList, string topLevelFileName, int level);
	void getFunctionObjectNamesFromFunctionsInCfile(CSfile* firstFileInIncludeFileList, CSfunction* firstFunctionInFunctionList, CSfile* aboveLevelObject, string topLevelFileName, int level);
		bool searchFunctionStringForFunctionReferencesRecursive(CSfile* firstFileInIncludeFileList, CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
			bool searchFunctionStringForFunctionReferences(CSfile* firstFileInIncludeFileList, CSfile* fileNameContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
				void identifyFunctionReferenceArguments(CSfunction* currentReferenceInFunctionReferenceListRepeats, string* functionContentsString, int indexToFunctionObject);
				#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS	
					string removePrependedWhiteSpace(string argument);
				void identifyFunctionDeclarationArguments(CSfunction* currentReferenceInFunctionReferenceList, string* functionNameFull);
				#endif

/*
CSfile* findReferenceInIncludeFileList(CSfile* firstReferenceInAboveLevelBelowList, string referenceName, CSfile* foundReference, bool* referenceFound);
*/

string generateSourceFileNameFromHeaderFileName(string headerFileName, string sourceFileNameExtension);

void attachFunctionReferenceTargets(CSfileContainer* firstObjectInAboveLevelBelowListContainer);

#endif

