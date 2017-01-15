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
 * File Name: CSmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a12a 31-July-2012
 *
 *******************************************************************************/

//issues, need to flip vertically
#include "CSmain.h"
#include "CSexecflow.h"
#include "CSdataflow.h"
#include "CSpreprocessorfilter.h"
#include "CSdraw.h"
#include "XMLrulesClass.h"
#include "LDsprite.h"

#ifndef LINUX
	#include <windows.h>
#endif


static char errmessage[] = "Usage:  OpenCS.exe [options]"
"\n"
"\n\twhere options are any of the following"
"\n"
"\n\t-oldr [string]          : code structure display .ldr output filename (def: codeStructureNet.ldr)"
"\n\t-osvg [string]          : code structure display .svg output filename (def: codeStructureNet.svg)"
"\n\t-oppm [string]          : code structure display .ppm output filename (def: codeStructureNet.ppm)"
"\n\t-oall [string]          : code structure display .svg/.ldr/.ppm default generic output filename (def: codeStructureNet)"
"\n\t-file [string]          : top level source file name (eg, main.cpp) [compulsory]"
"\n\t-function [string]      : top level function name referenced within file {recommended: not defined in file, declared within include h file} (eg, x for int x()) [compulsory]"
"\n\t-notshow                : do not display output in opengl"
"\n\t-width [int]            : raster graphics width in pixels (def: 640)"
"\n\t-height [int]           : raster graphics height in pixels (def: 480)"
"\n\t-enablefunctions        : output function connectivity on top of file connectivity"
"\n\t-mode [int]             : mode (1: execution flow, 2: data flow, 3: filter code based upon preprocessor definitions) (def: 1)"
"\n\t-trace                  : trace a function's usage upwards"
"\n\t-tracefunction [string] : bottom level function name to trace upwards (eg, y for int y())"
"\n"
"\n\t-workingfolder [string] : working directory name for input files (def: same as exe)"
"\n\t-exefolder [string]     : exe directory name for executables OpenCS.exe and (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n"
"\n\t-version                : print version"
"\n"
"\n\tThe program generates vector graphics based upon code structure of a given function in the C/C++ source directory."
"\n"
"\n";

#define CS_MODE_OUTPUT_EXECUTION_FLOW (1)
#define CS_MODE_OUTPUT_DATA_FLOW (2)
#define CS_MODE_FILTER_CODE_USING_PREPROCESSOR_DEFINITIONS (3)

int main(int argc,char **argv)
{
	bool useOutputLDRFile = false;
	string outputLDRFileName = "codeStructureNet.ldr";

	bool useOutputPPMFile = false;
	string outputPPMFileName = "codeStructureNet.ppm";

	bool useOutputSVGFile = false;
	string outputSVGFileName = "codeStructureNet.svg";

	bool useOutputAllFile = false;
	string outputAllFileName = "codeStructureNet";

	bool useTopLevelFile = false;
	string topLevelFileName = "main.cpp";

	bool useTopLevelFunction = false;
	string topLevelFunctionName = "main";

	bool useBottomLevelFunctionNameToTraceUpwards = false;
	string bottomLevelFunctionNameToTraceUpwards = "";

	bool printOutput = false;
	bool displayInOpenGLAndOutputScreenshot = true;

	int mode = CS_MODE_OUTPUT_EXECUTION_FLOW;	//1. output execution flow, 2. output data flow, 3. filter code based upon preprocessor definitions

	bool passInputReq = true;
	bool outputFunctionsConnectivity = false;
	bool traceAFunctionUpwards = false;

	int rasterImageWidth = 1600;
	int rasterImageHeight = 1000;

	if (exists_argument(argc,argv,"-mode"))
	mode=get_float_argument(argc,argv,"-mode");

	if(exists_argument(argc,argv,"-oldr"))
	{
		outputLDRFileName=get_char_argument(argc,argv,"-oldr");
		useOutputLDRFile = true;
		printOutput = true;
	}

	if(exists_argument(argc,argv,"-oppm"))
	{
		outputPPMFileName=get_char_argument(argc,argv,"-oppm");
		useOutputPPMFile = true;
		printOutput = true;
	}

	if(exists_argument(argc,argv,"-osvg"))
	{
		outputSVGFileName=get_char_argument(argc,argv,"-osvg");
		useOutputSVGFile = true;
		printOutput = true;
	}

	if(exists_argument(argc,argv,"-oall"))
	{
		outputAllFileName=get_char_argument(argc,argv,"-oall");
		useOutputAllFile = true;
		printOutput = true;
	}

	if (exists_argument(argc,argv,"-file"))
	{
		topLevelFileName=get_char_argument(argc,argv,"-file");
	}
	else
	{
		passInputReq = false;
	}
	if (exists_argument(argc,argv,"-function"))
	{
		topLevelFunctionName=get_char_argument(argc,argv,"-function");
	}
	else
	{
		passInputReq = false;
	}




	if (exists_argument(argc,argv,"-notshow"))
	{
		displayInOpenGLAndOutputScreenshot = false;
	}

	if (exists_argument(argc,argv,"-width"))
	rasterImageWidth=get_float_argument(argc,argv,"-width");

	if (exists_argument(argc,argv,"-height"))
	rasterImageHeight=get_float_argument(argc,argv,"-height");

	if (exists_argument(argc,argv,"-enablefunctions"))
	{
		outputFunctionsConnectivity = true;
	}

	if (exists_argument(argc,argv,"-trace"))
	{
		if(outputFunctionsConnectivity)
		{
			if (exists_argument(argc,argv,"-tracefunction"))
			{
				bottomLevelFunctionNameToTraceUpwards=get_char_argument(argc,argv,"-tracefunction");
				traceAFunctionUpwards = true;
				//cout << "here" MM
			}
			else
			{
				cout << "error: -trace declared without a -tracefunction being provided" << endl;
				passInputReq = false;
			}
		}
		else
		{
			cout << "error: -trace declared without -enablefunctions" << endl;
			passInputReq = false;
		}
	}

	char currentFolder[EXE_FOLDER_PATH_MAX_LENGTH];
	#ifdef LINUX
	getcwd(currentFolder, EXE_FOLDER_PATH_MAX_LENGTH);
	#else
	::GetCurrentDirectory(EXE_FOLDER_PATH_MAX_LENGTH, currentFolder);
	#endif

	if (exists_argument(argc,argv,"-workingfolder"))
	{
		workingFolderCharStar=get_char_argument(argc,argv,"-workingfolder");
	}
	else
	{
		workingFolderCharStar = currentFolder;
	}
	if (exists_argument(argc,argv,"-exefolder"))
	{
		exeFolderCharStar=get_char_argument(argc,argv,"-exefolder");
	}
	else
	{
		exeFolderCharStar = currentFolder;
	}
	if (exists_argument(argc,argv,"-tempfolder"))
	{
		tempFolderCharStar=get_char_argument(argc,argv,"-tempfolder");
	}
	else
	{
		tempFolderCharStar = currentFolder;
	}

	#ifdef LINUX
	chdir(workingFolderCharStar);
	#else
	::SetCurrentDirectory(workingFolderCharStar);
	#endif

	if (exists_argument(argc,argv,"-version"))
	{
		cout << "OpenCS.exe version: 2a4a" << endl;
		exit(1);
	}

	if(!passInputReq)
	{
		printf(errmessage);
		cout << "**** Known Limitations: *****" << endl;
		cout << "all c/cpp and h/hpp files that wish to be parsed/added to tree must be contained in the same directory" << endl;
		cout << "if the CS program does not stop, there might be loops in the include file structure (eg a.cpp includes b.cpp, b.cpp includes a.cpp)" << endl;
		cout << "CS does not support commenting (illegal functions and include files have to be be removed completely)" << endl;	//CS has been upgraded to support commenting (previously illegal functions and include files had to be be removed completely)
		cout << "function definitions in .cpp files must not have leading white space, and should be contained on a single line" << endl;
		cout << "function definitions in .cpp files must end with a } without any leading white space" << endl;
		cout << "function declarations for functions that wish to be parsed/added to tree must be contained in .h files, can have preceeding white space, but must be contained on a single line" << endl;
		cout << "CS does not support relative paths in #include." << endl;
		cout << "CS requires include/header files that wish to be parsed/added to tree to to be delimited with " " rather than < >" << endl;
		cout << "CS requires a single space between #include and \"this.h\"" << endl;
		cout << "CS may produce large SVG files (Eg when functions are enabled via enablefunctions) which must be viewed with a viewer capable of dynamic zoom, eg, Konqueror" << endl;
		cout << "CS is not designed to operate with function pointers - it is argued that code is easier to read without function pointers and should only be used where heavy optimisation is required" << endl;
		cout << "the first include file in the top level source file (eg PROJECTmain.cpp must declare the top level function name (eg main)" << endl;
		cout << "****************************" << endl;
		exit(0);
	}


	if(printOutput)
	{
		if(!useOutputLDRFile)
		{
			if(useOutputAllFile || displayInOpenGLAndOutputScreenshot)		//LDR output is always required when displaying semantic network in OpenGL and outputing screenshot
			{
				useOutputLDRFile = true;
				outputLDRFileName = outputAllFileName + ".ldr";
			}
		}
		if(!useOutputSVGFile)
		{
			useOutputSVGFile = true;	//SVG output is always required when printing/drawing semantic network
			outputSVGFileName = outputAllFileName + ".svg";
		}
		if(!useOutputPPMFile)
		{
			if(useOutputAllFile)
			{
				useOutputPPMFile = true;
				outputPPMFileName = outputAllFileName + ".ppm";
			}
		}
	}


	if(!parseCSRulesXMLFile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInLDSpriteExternVariables();
	fillInCSRulesExternVariables();


	if(mode == CS_MODE_OUTPUT_EXECUTION_FLOW)
	{
		printCS(topLevelFileName, topLevelFunctionName, rasterImageWidth, rasterImageHeight, outputLDRFileName, outputSVGFileName, outputPPMFileName, useOutputLDRFile, useOutputPPMFile, displayInOpenGLAndOutputScreenshot, outputFunctionsConnectivity, traceAFunctionUpwards, bottomLevelFunctionNameToTraceUpwards);
	}
	else if(mode == CS_MODE_OUTPUT_DATA_FLOW)
	{
		cout << "error: invalid operation mode" << endl;
		exit(0);
	}
	else if(mode == CS_MODE_FILTER_CODE_USING_PREPROCESSOR_DEFINITIONS)
	{
		cout << "error: invalid operation mode" << endl;
		exit(0);
	}
	else
	{
		cout << "error: invalid operation mode" << endl;
		exit(0);
	}
}
