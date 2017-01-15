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
 * File Name: CSmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h13c 11-December-2015
 *
 *******************************************************************************/

//issues, need to flip vertically


#include "CSmain.h"
#include "CSexecflow.h"
#include "CSdataflow.h"
#include "CSpreprocessorFilter.h"
#include "CSdraw.h"
#include "CSgenerateHTMLdocumentation.h"
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
"\n\t-ohtml [string]         : code structure display .html output filename (def: codeStructureNet.html) (use single file, else HTML file names are auto generated on a per C file basis)"
"\n\t-oall [string]          : code structure display .svg/.ldr/.ppm default generic output filename (def: codeStructureNet)"
"\n\t-file [string]          : top level source file name (eg, main.cpp) [compulsory]"
"\n\t-function [string]      : top level function name referenced within file {recommended: not defined in file, declared within include h file} (eg, x for int x()) [compulsory]"
"\n\t-notshow                : do not display output in opengl"
"\n\t-width [int]            : raster graphics width in pixels (def: 1600)"
"\n\t-height [int]           : raster graphics height in pixels (def: 1000)"
"\n\t-enablefunctions        : output function connectivity on top of file connectivity"
"\n\t-disablefileconnections : disable output of file connections"
"\n\t-mode [int]             : mode (1: execution flow, 2: data flow, 3: filter code based upon preprocessor definitions) (def: 1)"
"\n\t-trace                  : trace a function's usage upwards"
"\n\t-tracefunction [string] : bottom level function name to trace upwards / document (eg, y for int y())"
"\n\t-html                   : generate html documentation (user must specify tracefunction, else will document all functions)"
#ifdef CS_GENERATE_CPP_CLASSES
"\n\t-generateoo             : generate object oriented C++ code (must specify tempfolder else input files will be overwritten)"
#endif
#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
"\n\t-generateconst          : generate const function arguments code (must specify tempfolder else input files will be overwritten)"
#endif
"\n"
"\n\t-workingfolder [string] : working directory name for input files (def: same as exe)"
"\n\t-exefolder [string]     : exe directory name for executables; OpenCS.exe (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n"
"\n\t-version                : print version"
"\n"
"\n\tThe program generates vector graphics based upon code structure of a given function in the C/C++ source directory."
"\n"
"\n";




int main(int argc,char* *argv)
{
#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
	convertIndentedListToHTMLlist();
#elif defined CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	generateClassHTMLdocumentationFromCustomCSclassFormat();
#else

	bool useOutputLDRfile = false;
	string outputLDRfileName = "codeStructureNet.ldr";

	bool useOutputPPMfile = false;
	string outputPPMfileName = "codeStructureNet.ppm";

	bool useOutputSVGFile = false;
	string outputSVGfileName = "codeStructureNet.svg";

	bool useOutputHTMLfile = false;
	string outputHTMLfileName = "codeStructureNet.html";

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

	int generateHTMLdocumentationMode = CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF;

	//#ifdef CS_GENERATE_CPP_CLASSES
	bool generateOOcode = false;
	//#endif
	//#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	bool generateConstFunctionArgumentsCode = false;
	//#endif
	
	bool passInputReq = true;
	bool outputFunctionsConnectivity = false;
	bool traceFunctionUpwards = false;

	int rasterImageWidth = 1600;
	int rasterImageHeight = 1000;

	bool outputFileConnections = true;

	if (argumentExists(argc,argv,"-mode"))
	{
		mode = getFloatArgument(argc,argv,"-mode");
	}

	if(argumentExists(argc,argv,"-oldr"))
	{
		outputLDRfileName=getStringArgument(argc,argv,"-oldr");
		useOutputLDRfile = true;
		printOutput = true;
	}

	if(argumentExists(argc,argv,"-oppm"))
	{
		outputPPMfileName=getStringArgument(argc,argv,"-oppm");
		useOutputPPMfile = true;
		printOutput = true;
	}

	if(argumentExists(argc,argv,"-osvg"))
	{
		outputSVGfileName=getStringArgument(argc,argv,"-osvg");
		useOutputSVGFile = true;
		printOutput = true;
	}

	if(argumentExists(argc,argv,"-ohtml"))
	{
		outputHTMLfileName=getStringArgument(argc,argv,"-ohtml");
		useOutputHTMLfile = true;
	}

	if(argumentExists(argc,argv,"-oall"))
	{
		outputAllFileName=getStringArgument(argc,argv,"-oall");
		useOutputAllFile = true;
		printOutput = true;
	}

	if(argumentExists(argc,argv,"-file"))
	{
		topLevelFileName=getStringArgument(argc,argv,"-file");
	}
	else
	{
		passInputReq = false;
	}
	if(argumentExists(argc,argv,"-function"))
	{
		topLevelFunctionName=getStringArgument(argc,argv,"-function");
	}
	else
	{
		passInputReq = false;
	}




	if(argumentExists(argc,argv,"-notshow"))
	{
		displayInOpenGLAndOutputScreenshot = false;
	}

	if(argumentExists(argc,argv,"-width"))
	{
		rasterImageWidth = getFloatArgument(argc,argv,"-width");
	}

	if(argumentExists(argc,argv,"-height"))
	{
		rasterImageHeight = getFloatArgument(argc,argv,"-height");
	}

	if(argumentExists(argc,argv,"-enablefunctions"))
	{
		outputFunctionsConnectivity = true;
	}
	if(argumentExists(argc,argv,"-disablefileconnections"))
	{
		outputFileConnections = false;
	}

	if(argumentExists(argc,argv,"-trace"))
	{
		if(outputFunctionsConnectivity)
		{
			traceFunctionUpwards = true;

		}
		else
		{
			cout << "error: -trace declared without -enablefunctions" << endl;
			passInputReq = false;
		}
	}
	if(argumentExists(argc,argv,"-tracefunction"))
	{
		bottomLevelFunctionNameToTraceUpwards=getStringArgument(argc,argv,"-tracefunction");
	}


	if(argumentExists(argc,argv,"-html"))
	{
		generateHTMLdocumentationMode = CS_GENERATE_HTML_DOCUMENTATION_MODE_ON;
		//getFloatArgument(argc,argv,"-html");
	}

	#ifdef CS_GENERATE_CPP_CLASSES
	if(argumentExists(argc,argv,"-generateoo"))
	{
		generateOOcode = true;
	}
	#endif	
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	if(argumentExists(argc,argv,"-generateconst"))
	{
		generateConstFunctionArgumentsCode = true;
	}
	#endif		

	string currentFolder = getCurrentDirectory();

	if(argumentExists(argc,argv,"-workingfolder"))
	{
		workingFolder=getStringArgument(argc,argv,"-workingfolder");
	}
	else
	{
		workingFolder = currentFolder;
	}
	if(argumentExists(argc,argv,"-exefolder"))
	{
		exeFolder=getStringArgument(argc,argv,"-exefolder");
	}
	else
	{
		exeFolder = currentFolder;
	}
	if(argumentExists(argc,argv,"-tempfolder"))
	{
		tempFolder=getStringArgument(argc,argv,"-tempfolder");
	}
	else
	{
		tempFolder = currentFolder;
	}

	setCurrentDirectory(workingFolder);

	if(argumentExists(argc,argv,"-version"))
	{
		cout << "OpenCS.exe - Project Version: 3h13c 11-December-2015" << endl;
		exit(1);
	}

	if(!passInputReq)
	{
		printf(errmessage);
		cout << "**** Known Limitations:* ****" << endl;
		cout << "all c/cpp and h/hpp files that wish to be parsed/added to tree must be contained in the same directory" << endl;
		cout << "if the CS program does not stop, there might be loops in the include file structure (eg a.cpp includes b.cpp, b.cpp includes a.cpp)" << endl;
		cout << "CS has been upgraded to support commenting (previously illegal functions and include files had to be be removed completely)" << endl;
		cout << "function definitions in .cpp files must not have leading white space, and should be contained on a single line" << endl;
		cout << "function definitions in .cpp files must end with a } without any leading white space" << endl;
		cout << "function declarations for functions that wish to be parsed/added to tree must be contained in .h files, can have preceding white space, but must be contained on a single line" << endl;
		cout << "CS does not support relative paths in #include." << endl;
		cout << "CS requires include/header files that wish to be parsed/added to tree to to be delimited with " " rather than < >" << endl;
		cout << "CS requires a single space between #include and \"this.h\"" << endl;
		cout << "CS may produce large SVG files (Eg when functions are enabled via enablefunctions) which must be viewed with a viewer capable of dynamic zoom, eg, inkscape" << endl;
		cout << "CS is not designed to operate with function pointers (and object orientated code) - it is argued that code is easier to read without function pointers and should only be used where heavy optimisation is required" << endl;
		cout << "the first include file in the top level source file (eg PROJECTmain.cpp) must declare the top level function name (eg main)" << endl;
		cout << "make sure the temp folder is clear of all output files (ie svg/html files of the same name as expected output files)" << endl;
		cout << "function contents cannot include a reference to themselves in comments (required for HTML generation function reference list and generateoo)" << endl;
		cout << "function contents cannot include a reference to themselves in cout statements (e.g. cout << \"dothis()\")" << endl;
		cout << "function contents cannot include a reference to their name in cout statements followed by an equals sign, unless it is referenced at the start of the comment (e.g. cout << \"generateHTMLdocumentationMode = \" ...)" << endl;				
		cout << "CS does not support 2 identical function declarations (with identical arguments) for a single function in a header file (separated by preprocessor definitions)" << endl;
		cout << "CS does not support 2 function declarations with different arguments for a single function in a header file (separated by preprocessor defintions)" << endl;
		cout << "CS doesn't support overloaded functions with the same number of arguments (required for precise referencing)" << endl;
		cout << "****************************" << endl;
		exit(0);
	}


	if(printOutput)
	{
		if(!useOutputLDRfile)
		{
			if(useOutputAllFile || displayInOpenGLAndOutputScreenshot)		//LDR output is always required when displaying semantic network in OpenGL and outputing screenshot
			{
				useOutputLDRfile = true;
				outputLDRfileName = outputAllFileName + ".ldr";
			}
		}
		if(!useOutputSVGFile)
		{
			useOutputSVGFile = true;	//SVG output is always required when printing/drawing semantic network
			outputSVGfileName = outputAllFileName + ".svg";
		}
		if(!useOutputPPMfile)
		{
			if(useOutputAllFile)
			{
				useOutputPPMfile = true;
				outputPPMfileName = outputAllFileName + ".ppm";
			}
		}
	}
	/*
	if(generateHTMLdocumentationMode != CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
	{//only print html when explicitly set to do so
		if(!useOutputHTMLfile)
		{
			if(useOutputAllFile)
			{
				useOutputHTMLfile = true;
				outputHTMLfileName = outputAllFileName + ".html";
			}
		}
	}
	*/



	if(!parseCSrulesXMLfile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInLDspriteExternVariables();
	fillInCSrulesExternVariables();

	if(mode == CS_MODE_OUTPUT_EXECUTION_FLOW)
	{
		generateCodeStructure(topLevelFileName, rasterImageWidth, rasterImageHeight, outputLDRfileName, outputSVGfileName, outputPPMfileName, outputHTMLfileName, useOutputLDRfile, useOutputPPMfile, useOutputHTMLfile, generateHTMLdocumentationMode, displayInOpenGLAndOutputScreenshot, outputFunctionsConnectivity, traceFunctionUpwards, bottomLevelFunctionNameToTraceUpwards, outputFileConnections, topLevelFunctionName, generateOOcode, generateConstFunctionArgumentsCode);
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
#endif
}



