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
 * File Name: CSoperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3m13c 22-February-2018
 * /
 *******************************************************************************/


#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.hpp"
#include "CSglobalDefs.hpp"
#include "CSreferenceContainerClass.hpp"
#include "LDsprite.hpp"
#include "LDreferenceManipulation.hpp"

class CSoperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: CSreferenceContainerClassClass CSreferenceContainerClass;
	public: bool getIncludeFileNamesFromCorHfile(CSfileContainer* firstReferenceInIncludeFileListContainer, constEffective CSfileContainer* topLevelReferenceContainer, CSfile* aboveLevelObject, const string topLevelReferenceName, const int level);
		#ifdef CS_GENERATE_CODE_GENERIC
		private: bool fileIsHeader(string parseFileName);
		#endif
		private: bool findFileObjectInFileObjectContainerList(constEffective CSfileContainer* firstReferenceContainerInLevel, const string fileReferenceName, constEffective CSfile** fileReferenceFound);
		private: bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunction* firstFunctionInFunctionList, const string topLevelFileName, const int level);
		private: void getFunctionObjectNamesFromFunctionsInCfile(const CSfile* firstFileInIncludeFileList, CSfunction* firstFunctionInFunctionList, const CSfile* aboveLevelObject, const string topLevelFileName, const int level);
			private: bool searchFunctionStringForFunctionReferencesRecursive(const CSfile* firstFileInIncludeFileList, const CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
				private: bool searchFunctionStringForFunctionReferences(const CSfile* firstFileInIncludeFileList, const CSfile* fileNameContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
					private: void identifyFunctionReferenceArguments(CSfunction* currentReferenceInFunctionReferenceList, string* functionContentsString, int indexToFunctionObject);
						private: string removePrependedWhiteSpace(const string argument);
					#ifdef CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS
					private: void identifyFunctionDeclarationArguments(CSfunction* currentReferenceInFunctionReferenceList, const string* functionNameFull);
					#endif

/*
CSfile* findReferenceInIncludeFileList(CSfile* firstReferenceInAboveLevelBelowList, string referenceName, CSfile* foundReference, bool* referenceFound);
*/

	private: string generateSourceFileNameFromHeaderFileName(const string headerFileName, const string sourceFileNameExtension);

	public: void attachFunctionReferenceTargets(CSfileContainer* firstObjectInAboveLevelBelowListContainer);

	private: string extractFunctionArgumentName(string* argumentText, const int indexOfStartOfVariableName);
};

#endif

