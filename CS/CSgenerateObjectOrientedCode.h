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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3i19e 15-December-2016
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
	bool generateCPPclassesRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer);
		bool generateCPPclassesFile(CSfile* currentFileObject, CSfileContainer* firstObjectInTopLevelBelowListContainer);
			string replaceAllOccurancesOfFunctionObjectReferenceNameInFunction(string* functionTextOrig, string functionReferenceName, string functionReferenceNameUpdated, bool* foundAtLeastOneInstance);
			void isFunctionBeingReferencedPublicallyRecurse(string functionName, string fileName, CSfileContainer* firstObjectInAboveLevelBelowListContainer, bool* foundPublicReference);
			string generateClassName(string headerFileName);
				string generateClassDeclarationName(string className);
				string generateClassObjectName(string className);
			string convertFunctionNameToClassFunctionNameSource(string fullFunctionName, string functionName, string className, bool foundPublicReference, bool* foundStaticReference);
			string convertFunctionNameToClassFunctionNameHeader(string fullFunctionName, string functionName, string className, bool foundPublicReference, bool foundStaticReference);
			string generateReferencedClassesDeclarations(ReferencedClass* firstReferencedClassInList);
			bool findReferencedClassInList(ReferencedClass* firstReferencedClassInList, string classNameToFind);
			bool moveIncludeFileStatementsToHeader(CSfile* firstReferenceInAboveLevelBelowList);


#endif

#endif
