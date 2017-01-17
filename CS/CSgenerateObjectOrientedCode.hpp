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
 * File Name: CSgenerateObjectOrientedCode.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3j2a 17-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_GENERATE_OBJECT_ORIENTED_CODE
#define HEADER_CS_GENERATE_OBJECT_ORIENTED_CODE

#include "CSreferenceContainerClass.hpp"
#include "CSglobalDefs.hpp"
#include "CSdraw.hpp"
#include "SHAREDvars.hpp"

#ifdef CS_GENERATE_CPP_CLASSES

class ReferencedClass
{
public:
	ReferencedClass(void);
	string className;
	ReferencedClass* next;
};


class CSgenerateObjectOrientedCodeClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: bool generateCPPclasses(CSfileContainer* firstObjectInTopLevelBelowListContainer);
		private: bool generateCPPclassesRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, const CSfileContainer* firstObjectInTopLevelBelowListContainer);
			private: bool generateCPPclassesFile(CSfile* currentFileObject, const CSfileContainer* firstObjectInTopLevelBelowListContainer);
				private: string replaceAllOccurancesOfFunctionObjectReferenceNameInFunction(const string* functionTextOrig, string functionReferenceName, string functionReferenceNameUpdated, bool* foundAtLeastOneInstance);
				private: void isFunctionBeingReferencedPublicallyRecurse(const string functionName, const string fileName, const CSfileContainer* firstObjectInAboveLevelBelowListContainer, bool* foundPublicReference);
				private: string generateClassName(const string headerFileName);
					private: string generateClassDeclarationName(const string className);
					private: string generateClassObjectName(const string className);
				private: string convertFunctionNameToClassFunctionNameSource(string fullFunctionName, const string functionName, const string className, const bool foundPublicReference, bool* foundStaticReference);
				private: string convertFunctionNameToClassFunctionNameHeader(const string fullFunctionName, const string functionName, const string className, const bool foundPublicReference, const bool foundStaticReference);
				private: string generateReferencedClassesDeclarations(const ReferencedClass* firstReferencedClassInList);
				private: bool findReferencedClassInList(const ReferencedClass* firstReferencedClassInList, const string classNameToFind);
				private: bool moveIncludeFileStatementsToHeader(CSfile* firstReferenceInAboveLevelBelowList);
};


#endif

#endif
