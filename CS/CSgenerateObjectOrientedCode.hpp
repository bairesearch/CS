/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateObjectOrientedCode.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3p5a 14-March-2022
 * /
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
