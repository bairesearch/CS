 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSoperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.hpp"
#include "CSglobalDefs.hpp"
#include "CSreferenceContainerClass.hpp"
#include "CSclassClass.hpp"
#include "LDsprite.hpp"
#include "LDreferenceManipulation.hpp"

class CSoperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: CSreferenceContainerClassClass CSreferenceContainerClass;
	private: CSclassClass CSclassClassObject;
	
	public: bool getIncludeFileNamesFromCorHfile(CSfileContainer* firstReferenceInIncludeFileListContainer, constEffective CSfileContainer* topLevelReferenceContainer, CSfile* aboveLevelObject, const string topLevelReferenceName, const int level, const bool parsingCorHfile);
		#ifdef CS_GENERATE_CODE_GENERIC
		private: bool fileIsHeader(string parseFileName);
		#endif
		private: bool findFileObjectInFileObjectContainerList(constEffective CSfileContainer* firstReferenceContainerInLevel, const string fileReferenceName, constEffective CSfile** fileReferenceFound);
		private: bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfile* fileObject, const string topLevelFileName, const int level);
		private: void getFunctionObjectNamesFromFunctionsInCfile(const CSfile* firstFileInIncludeFileList, CSfunction* firstFunctionInFunctionList, const CSfile* aboveLevelObject, const string topLevelFileName, const int level);
			private: bool searchFunctionStringForFunctionReferencesRecursive(const CSfile* firstFileInIncludeFileList, const CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
			#ifdef CS_OPTIMISE_PREVENT_DUPLICATE_FUNCTION_CONNECTIONS
			private: void searchFunctionStringForFunctionReferencesRecursiveReset(const CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor);
			#endif
				private: bool searchFunctionStringForFunctionReferences(const CSfile* firstFileInIncludeFileList, const CSfile* fileNameContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString, const bool currentFile);
					private: bool searchFunctionStringForFunctionReference(const CSfile* firstFileInIncludeFileList, const CSfunction* currentFunction, const string functionReferenceToFind, bool* firstTimeFound, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString, const bool externalFunction);
						private: void identifyFunctionReferenceArguments(CSfunction* currentReferenceInFunctionReferenceList, string* functionContentsString, int indexToFunctionObject, const string functionReferenceToFind);
							private: string removePrependedWhiteSpace(const string argument);
			#ifdef CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS
			private: void identifyFunctionDeclarationArguments(CSfunction* currentReferenceInFunctionReferenceList, const string* functionNameFull);
			#endif

/*
CSfile* findReferenceInIncludeFileList(CSfile* firstReferenceInAboveLevelBelowList, string referenceName, CSfile* foundReference, bool* referenceFound);
*/

	private: string generateSourceFileNameFromHeaderFileName(const string headerFileName, const string sourceFileNameExtension);
	private: string generateFileNameBaseFromHeaderFileName(const string headerFileName);

	public: void attachFunctionReferenceTargets(CSfileContainer* firstObjectInAboveLevelBelowListContainer, int level);

	private: string extractFunctionArgumentName(string* argumentText, const int indexOfStartOfVariableName);
	
	private: void printLevelIndentation(int level);
	
	#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_REFERENCES_WITH_CLASS
	private: string generateFunctionString(const CSfunction* currentFunction, const bool externalFunction);
	#endif
	
	private: vector<CSparameter*> createParameterList(const string* functionArgumentsRaw);

};

#endif

