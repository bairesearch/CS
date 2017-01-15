/*******************************************************************************
 *
 * File Name: CSexecflow.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a7d 11-June-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_EXECFLOW
#define HEADER_CS_EXECFLOW

#include "CSglobalDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;


void printCS(string topLevelFileName, string topLevelFunctionName, int width, int height, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, bool useOutputLDRFile, bool useOutputPPMFile, bool display, bool outputFunctionsConnectivity, bool traceAFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards);

#endif
