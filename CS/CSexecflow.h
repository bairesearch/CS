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
 * File Name: CSexecflow.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3e3b 01-September-2014
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
#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
#include <vector>
#endif
using namespace std;

#ifdef LINUX
	#define SYSTEM_COPY_COMMAND "cp"
#else
	#define SYSTEM_COPY_COMMAND "copy"
#endif

void generateCodeStructure(string topLevelFileName, int width, int height, string outputLDRfileName, string outputSVGfileName, string outputPPMfileName, string outputHTMLfileName, bool useOutputLDRfile, bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, bool outputFileConnections, string topLevelFunctionName, bool generateOOcode);


#endif
