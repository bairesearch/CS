/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateObjectOrientedCode.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3j1a 14-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_GENERATE_OBJECT_ORIENTED_CODE
#define HEADER_CS_GENERATE_OBJECT_ORIENTED_CODE

#include "CSreferenceContainerClass.h"
#include "CSglobalDefs.h"

#ifdef CS_GENERATE_CPP_CLASSES

class ReferencedClass
{
public:
	ReferencedClass(void);
	string className;
	ReferencedClass* next;
};


bool generateCPPclasses(CSfileContainer* firstObjectInTopLevelBelowListContainer);
	bool generateCPPclassesRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, const CSfileContainer* firstObjectInTopLevelBelowListContainer);
		bool generateCPPclassesFile(CSfile* currentFileObject, const CSfileContainer* firstObjectInTopLevelBelowListContainer);
			string replaceAllOccurancesOfFunctionObjectReferenceNameInFunction(const string* functionTextOrig, string functionReferenceName, string functionReferenceNameUpdated, bool* foundAtLeastOneInstance);
			void isFunctionBeingReferencedPublicallyRecurse(const string functionName, const string fileName, const CSfileContainer* firstObjectInAboveLevelBelowListContainer, bool* foundPublicReference);
			string generateClassName(const string headerFileName);
				string generateClassDeclarationName(const string className);
				string generateClassObjectName(const string className);
			string convertFunctionNameToClassFunctionNameSource(string fullFunctionName, const string functionName, const string className, const bool foundPublicReference, bool* foundStaticReference);
			string convertFunctionNameToClassFunctionNameHeader(const string fullFunctionName, const string functionName, const string className, const bool foundPublicReference, const bool foundStaticReference);
			string generateReferencedClassesDeclarations(const ReferencedClass* firstReferencedClassInList);
			bool findReferencedClassInList(const ReferencedClass* firstReferencedClassInList, const string classNameToFind);
			bool moveIncludeFileStatementsToHeader(CSfile* firstReferenceInAboveLevelBelowList);


#endif

#endif
