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
 * Project Version: 3a11b 09-July-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.h"
#include "CSglobalDefs.h"
#include "CSreferenceClass.h"




bool getIncludeFileNamesFromCorHFile(CSReference * firstReferenceInIncludeFileList, CSReference * topLevelReference, string topLevelReferenceName, int level);
	bool getFunctionNamesFromFunctionDeclarationsInHFile(CSReference * firstReferenceInFunctionList, string topLevelFileName, int level);
	void getFunctionReferenceNamesFromFunctionsInCFile(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInFunctionList, CSReference * topLevelReference, string topLevelFileName, int level);
		CSReference * searchFunctionStringForFunctionReferencesRecursive(CSReference * firstReferenceInIncludeFileList, CSReference * firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSReference * firstReferenceInFunctionReferenceList, string functionContentsString);
			CSReference * searchFunctionStringForFunctionReferences(CSReference * firstReferenceInIncludeFileList, CSReference * fileNameContainingFunctionReferencesToSearchFor, CSReference * firstReferenceInFunctionReferenceList, string functionContentsString);


CSReference * findReferenceInIncludeFileList(CSReference * firstReferenceInAboveLevelBelowList, string referenceName, CSReference * foundReference, bool * referenceFound);



#endif

