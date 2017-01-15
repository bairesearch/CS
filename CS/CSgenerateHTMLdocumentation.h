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
 * File Name: CSgenerateHTMLdocumentation.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3e5a 03-September-2014
 *
 *******************************************************************************/

#ifndef HEADER_CS_GENERATE_HTML_DOCUMENTATION
#define HEADER_CS_GENERATE_HTML_DOCUMENTATION

#include "CSglobalDefs.h"
#include "CSreferenceClass.h"
#include "LDreferenceManipulation.h"
#include "XMLparserClass.h"

void generateHTMLdocumentationFunctions(XMLparserTag * firstTagInSVGFile, CSfileReference * firstReferenceInTopLevelBelowList, int generateHTMLdocumentationMode, bool useOutputHTMLfile, bool traceFunctionUpwards, bool usePredefinedGrid, string outputHTMLfileName);
	string generateHTMLdocumentationHeader(string name, bool htmlHeader, bool isFile);
	string generateHTMLdocumentationFooter(bool htmlFileFooter);
	void generateHTMLdocumentationForAllFunctions(CSfileReference * firstReferenceInAboveLevelBelowList, Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, int generateHTMLdocumentationMode, bool useOutputHTMLfile, string * HTMLdocumentationBody, XMLparserTag * firstTagInSVGFile, XMLparserTag * lastTagInSVGFile, bool traceFunctionUpwards, bool usePredefinedGrid);
		void generateHTMLdocumentationForFunction(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * bottomLevelFunctionToTraceUpwards, string fileNameHoldingFunction, XMLparserTag ** currentTag, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionBody, string * outputSVGfileNameFunction, bool useOutputHTMLfile, string outputHTMLfileName, bool traceFunctionUpwards);
			void generateHTMLdocumentationFunctionSummary(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionSummary);
			void generateHTMLdocumentationFunctionInputArguments(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionInputArguments);
				int findEndPositionOfArgument(string * functionArgumentsRaw, int startPositionOfArgument);
				string ensureHTMLTagSafe(string str);
				string createDescriptionFromCaseSensitiveMultiwordString(string str);
			void generateHTMLdocumentationFunctionReferenceList(CSfunctionReference * function, string * HTMLdocumentationFunctionReferenceList);
			string generateHTMLdocumentationImagePlaceHolder(string * traceImageFileName, string imageTitle);
		void generateFileDiagramFunctionsHeirachy(CSfileReference * currentFileReference, string outputSVGFileNameFile, CSfileReference * firstReferenceInTopLevelBelowList, bool usePredefinedGrid);
	void writeStringPointerToFileObject(string * s, ofstream * writeFileObject);
	void addToHTMLdocumentationFileFunctionList(CSfunctionReference * currentFunctionReference, string * HTMLdocumentationFileFunctionList, int * previousIndentation, bool * previousIndentationFirst);

#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT_COMBINED
#define NUMBER_OF_CSCLASSES (9)
static string CSclassesArray[NUMBER_OF_CSCLASSES] = {"NLC.CSclass", "GIA.CSclass", "OR.CSclass", "CS.CSclass", "ANN.CSclass", "RT.CSclass", "LD.CSclass", "SHARED.CSclass", "XML.CSclass"};
#else
#define NUMBER_OF_CSCLASSES (29)
static string CSclassesArray[NUMBER_OF_CSCLASSES] = {"NLCcodeBlockClass.CSclass", "NLCclassDefinitionClass.CSclass", "NLCitemClass.CSclass", "NLCpreprocessorMathLogicalConditions.CSclass", "NLCpreprocessorSentenceClass.CSclass", "GIAconditionNodeClass.CSclass", "GIAentityConnectionClass.CSclass", "GIAentityNodeClass.CSclass", "GIAlrp.CSclass", "GIAnlg.CSclass", "GIAquery.CSclass", "GIAsentenceClass.CSclass", "GIAtranslatorGeneric.CSclass", "ORpolygonList.CSclass", "ORquadraticFit.CSclass", "ORTHimageCategorisationNN.CSclass", "CSreferenceClass.CSclass", "ANNexperienceClass.CSclass", "ANNneuronClass.CSclass", "RToperations.CSclass", "RTparser.CSclass", "RTppm.CSclass", "RTraytracer.CSclass", "RTscene.CSclass", "RTviewinfo.CSclass", "LDreferenceClass.CSclass", "SHAREDvars.CSclass", "XMLparser.CSclass", "XMLrules.CSclass"};
#endif
void generateClassHTMLdocumentationFromCustomCSclassFormat();
string getFunctionNameFromFunctionNameFull(string * functionNameFull);
#endif

#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
void convertIndentedListToHTMLlist();
	bool readIndentedListFile(string indentedListFileName, vector<string> * indentedListVector);
	void generateHTMLdocumentationIndentedList(vector<string> * indentedListVector, string * HTMLdocumentationIndentationList);
#endif


#endif
