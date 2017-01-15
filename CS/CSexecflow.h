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
 * Project Version: 3b2a 28-September-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_EXECFLOW
#define HEADER_CS_EXECFLOW

#include "CSglobalDefs.h"
#include "CSreferenceClass.h"
#include "LDreferenceManipulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

#define HTML_EXTENSION ".html"
#define SVG_EXTENSION ".svg"

#ifdef LINUX
	#define SYSTEM_COPY_COMMAND "cp"
#else
	#define SYSTEM_COPY_COMMAND "copy"
#endif
				
void printCS(string topLevelFileName, string topLevelFunctionName, int width, int height, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, string outputHTMLFileName, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputHTMLFile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceAFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards);
	string generateHTMLdocumentationHeader(string name);
	string generateHTMLdocumentationFooter();
	void generateHTMLdocumentationForAllFunctions(CSReference * firstReferenceInAboveLevelBelowList, Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, string topLevelFunctionName, int generateHTMLdocumentationMode, bool useOutputHTMLFile, string * HTMLdocumentationBody, string * outputSVGFileName, bool traceAFunctionUpwards);
		void generateHTMLdocumentationForFunction(Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, CSReference * bottomLevelFunctionToTraceUpwards, string fileNameHoldingFunction, ofstream * writeFileObject, string topLevelFunctionName, int generateHTMLdocumentationMode, string * HTMLdocumentationFunction, string * outputSVGFileNameFunction, bool useOutputHTMLFile, string outputHTMLFileName, bool traceAFunctionUpwards);
			void generateHTMLdocumentationFunctionInputArguments(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionInputArguments);
				int findEndPositionOfArgument(string * functionArgumentsRaw, int startPositionOfArgument);
				string ensureHTMLTagSafe(string str);
				string createDescriptionFromCaseSensitiveMultiwordString(string str);
			string generateHTMLdocumentationFunctionTraceImagePlaceHolder(string * traceImageFileName);
	void writeStringToFileObject(string * s, ofstream * writeFileObject);


#endif
