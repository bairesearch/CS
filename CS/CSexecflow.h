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
 * File Name: CSexecflow.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3c3h 19-November-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_EXECFLOW
#define HEADER_CS_EXECFLOW

#include "CSglobalDefs.h"
#include "CSreferenceClass.h"
#include "LDreferenceManipulation.h"
#include "XMLparserClass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX -400
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY -100
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX 10000
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY 2000
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINX -300
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINY 0
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXX 1000
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXY 300

#define HTML_EXTENSION ".html"
#define SVG_EXTENSION ".svg"

#ifdef LINUX
	#define SYSTEM_COPY_COMMAND "cp"
#else
	#define SYSTEM_COPY_COMMAND "copy"
#endif
				
void printCS(string topLevelFileName, int width, int height, string outputLDRfileName, string outputSVGfileName, string outputPPMfileName, string outputHTMLfileName, bool useOutputLDRfile, bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards);
	string generateHTMLdocumentationHeader(string name, bool htmlHeader, bool isFile);
	string generateHTMLdocumentationFooter(bool htmlFileFooter);
	void generateHTMLdocumentationForAllFunctions(CSfileReference * firstReferenceInAboveLevelBelowList, Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, int generateHTMLdocumentationMode, bool useOutputHTMLfile, string * HTMLdocumentationBody, XMLparserTag * firstTagInSVGFile, XMLparserTag * lastTagInSVGFile, bool traceFunctionUpwards);
		void generateHTMLdocumentationForFunction(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * bottomLevelFunctionToTraceUpwards, string fileNameHoldingFunction, XMLparserTag ** currentTag, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionBody, string * outputSVGfileNameFunction, bool useOutputHTMLfile, string outputHTMLfileName, bool traceFunctionUpwards);
			void generateHTMLdocumentationFunctionSummary(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionSummary);
			void generateHTMLdocumentationFunctionInputArguments(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionInputArguments);
				int findEndPositionOfArgument(string * functionArgumentsRaw, int startPositionOfArgument);
				string ensureHTMLTagSafe(string str);
				string createDescriptionFromCaseSensitiveMultiwordString(string str);
			void generateHTMLdocumentationFunctionReferenceList(CSfunctionReference * function, string * HTMLdocumentationFunctionReferenceList);			
			string generateHTMLdocumentationImagePlaceHolder(string * traceImageFileName, string imageTitle);
		void generateFileDiagramFunctionsHeirachy(CSfileReference * currentFileReference, string outputSVGFileNameFile, CSfileReference * firstReferenceInTopLevelBelowList);
	void writeStringToFileObject(string * s, ofstream * writeFileObject);
	void addToHTMLdocumentationFileFunctionList(CSfunctionReference * currentFunctionReference, string * HTMLdocumentationFileFunctionList, int * previousIndentation, bool * previousIndentationFirst);


#endif
