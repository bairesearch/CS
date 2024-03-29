 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateHTMLdocumentation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_CS_GENERATE_HTML_DOCUMENTATION
#define HEADER_CS_GENERATE_HTML_DOCUMENTATION

#include "CSglobalDefs.hpp"
#include "CSreferenceContainerClass.hpp"
#include "CSdraw.hpp"
#include "CSclassClass.hpp"
#include "LDreferenceManipulation.hpp"
#include "XMLparserClass.hpp"
#include "LDsvg.hpp"
#include "XMLrulesClass.hpp"

#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT_COMBINED
#define NUMBER_OF_CSCLASSES (9)
static string CSclassesArray[NUMBER_OF_CSCLASSES] = {"NLC.CSclass", "GIA.CSclass", "OR.CSclass", "CS.CSclass", "ANN.CSclass", "RT.CSclass", "LD.CSclass", "SHARED.CSclass", "XML.CSclass"};
#else
#define NUMBER_OF_CSCLASSES (33)
static string CSclassesArray[NUMBER_OF_CSCLASSES] = {"ANNexperienceClass.CSclass", "ANNneuronClass.CSclass", "ANNneuronConnectionClass.CSclass", "CSfileClass.CSclass", "CSfunctionClass.CSclass", "CSreferenceContainerClass.CSclass", "GIAconditionNodeClass.CSclass", "GIAentityConnectionClass.CSclass", "GIAentityNodeClass.CSclass", "GIAlrp.CSclass", "GIAnlg.CSclass", "GIAquery.CSclass", "GIAsentenceClass.CSclass", "GIAtranslatorGeneric.CSclass", "LDreferenceClass.CSclass", "NLCclassDefinitionClass.CSclass", "NLCcodeBlockClass.CSclass", "NLCitemClass.CSclass", "NLCpreprocessorMathLogicalConditions.CSclass", "NLCpreprocessorSentenceClass.CSclass", "NLCtranslator.CSclass", "ORpolygonList.CSclass", "ORquadraticFit.CSclass", "ORTHimageCategorisationNN.CSclass", "RToperations.CSclass", "RTparser.CSclass", "RTppm.CSclass", "RTraytracer.CSclass", "RTscene.CSclass", "RTviewinfo.CSclass", "SHAREDvars.CSclass", "XMLparser.CSclass", "XMLrules.CSclass"};
#endif
#endif

class CSgenerateHTMLdocumentationClass
{
	private: LDsvgClass LDsvg;
	private: CSdrawClass CSdraw;
	private: CSreferenceContainerClassClass CSreferenceContainerClass;
	private: CSclassClass CSclassClassObject;
	private: SHAREDvarsClass SHAREDvars;
	
	public: void generateHTMLdocumentationFunctions(XMLparserTag* firstTagInSVGFile, CSfileContainer* firstObjectInTopLevelBelowListContainer, int generateHTMLdocumentationMode, const bool useOutputHTMLfile, const bool traceFunctionUpwards, bool usePredefinedGrid, const string outputHTMLfileName);
		private: string generateHTMLdocumentationHeader(const string name, const bool htmlHeader, const bool isFile);
			private: string generateHTMLdocumentationHeader(const string titleName, const string fileName, const bool htmlHeader, const bool isFile);
		private: string generateHTMLdocumentationFooter(const bool htmlFileFooter);
		private: void generateHTMLdocumentationForAllFunctions(CSfileContainer* firstObjectInAboveLevelBelowListContainer, LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, int generateHTMLdocumentationMode, const bool useOutputHTMLfile, string* HTMLdocumentationBody, const XMLparserTag* firstTagInSVGFile, XMLparserTag* lastTagInSVGFile, const bool traceFunctionUpwards, bool usePredefinedGrid);
			public: void generateHTMLdocumentationForFunction(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* bottomLevelFunctionToTraceUpwards, const string fileNameHoldingFunction, XMLparserTag** currentTag, int generateHTMLdocumentationMode, string* HTMLdocumentationFunctionBody, const string* outputSVGfileNameFunction, const bool useOutputHTMLfile, const string outputHTMLfileName, const bool traceFunctionUpwards);
				private: void generateHTMLdocumentationFunctionSummary(const CSfunction* currentFunction, string* HTMLdocumentationFunctionSummary);
				private: void generateHTMLdocumentationFunctionInputArguments(const CSfunction* currentFunction, string* HTMLdocumentationFunctionInputArguments);
					private: void generateHTMLdocumentationFunctionOrClassArgumentsFromFunctionNameFull(const bool classDataOrFunctionArguments, const string* functionNameFull, string* HTMLdocumentationFunctionInputArguments);
						private: void generateHTMLdocumentationFunctionOrClassArgumentsFromString(const bool classDataOrFunctionArguments, const string functionArgumentsRaw, string* HTMLdocumentationFunctionInputArguments);
				#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS
				private: void generateHTMLdocumentationFunctionOrClassArgumentsFromParameterList(const bool classDataOrFunctionArguments, vector<CSparameter*>* parameterList, string* HTMLdocumentationParameterList);
				#endif
					private: string ensureHTMLTagSafe(const string str);
					private: string createDescriptionFromCaseSensitiveMultiwordString(string str);
				private: void generateHTMLdocumentationFunctionReferenceList(CSfunction* function, string* HTMLdocumentationFunctionReferenceList);
				private: string generateHTMLdocumentationImagePlaceHolder(const string* traceImageFileName, const string imageTitle);
			private: void generateFileDiagramFunctionsHeirachy(CSfile* currentFileObject, const string outputSVGFileNameFile, CSfileContainer* firstObjectInTopLevelBelowListContainer, bool usePredefinedGrid);
		private: void writeStringPointerToFileObject(string* s, ofstream* writeFileObject);
		private: void addToHTMLdocumentationFileFunctionList(CSfunction* currentFunctionObject, string* HTMLdocumentationFileFunctionList, int* previousIndentation, bool* previousIndentationFirst, const bool isFunctionOrFunctionReferenceList, const bool externalFunction);


	#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	public: void generateClassHTMLdocumentationFromCustomCSclassFormat();
	private: string getFunctionNameFromFunctionNameFull(const string* functionNameFull);
	private: void generateClassHTMLdocumentationFromCustomCSclassFormat(const string CSclassFileName, const vector<string>* CSclassFileContents);
	#endif
	#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_AUTOMATICALLY
	private: void generateClassHTMLdocumentationFromCSclassList(const string CSclassesFileName, const vector<CSclass*>* classObjectList);
	#endif

	#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
	public: void convertIndentedListToHTMLlist();
		private: bool readIndentedListFile(const string indentedListFileName, vector<string>* indentedListVector);
		private: void generateHTMLdocumentationIndentedList(vector<string>* indentedListVector, string* HTMLdocumentationIndentationList);
	#endif

	private: void addToHTMLdocumentationIndentedList(const string currentFunctionObjectName, int currentFunctionObjectIndentation, string* HTMLdocumentationFileFunctionList, int* previousIndentation, bool* previousIndentationFirst);
	
	#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_NAME_WITH_CLASS
	private: string generateFunctionString(const CSfunction* currentFunction, const bool isFunctionOrFunctionReferenceList, const bool externalFunction);
	#endif
};

#endif
