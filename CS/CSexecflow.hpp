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
 * File Name: CSexecflow.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3n3c 28-May-2020
 * /
 *******************************************************************************/


#ifndef HEADER_CS_EXECFLOW
#define HEADER_CS_EXECFLOW

#include "CSglobalDefs.hpp"
#include "CSreferenceContainerClass.hpp"
#include "LDreferenceManipulation.hpp"
#include "XMLparserClass.hpp"
#include "CSoperations.hpp"
#include "CSdraw.hpp"
#include "LDsvg.hpp"
#include "LDopengl.hpp"
#include "XMLrulesClass.hpp"
#include "LDparser.hpp"
#include "LDsprite.hpp"
#include "RTpixelMaps.hpp"
#include "CSgenerateHTMLdocumentation.hpp"
#include "CSgenerateObjectOrientedCode.hpp"
#include "CSgenerateConstFunctionArgumentCode.hpp"

#ifdef LINUX
	#define SYSTEM_COPY_COMMAND "cp"
#else
	#define SYSTEM_COPY_COMMAND "copy"
#endif

class CSexecflowClass
{
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: CSoperationsClass CSoperations;
	private: CSdrawClass CSdraw;
	private: LDsvgClass LDsvg;
	private: LDopenglClass LDopengl;
	private: LDparserClass LDparser;
	private: RTpixelMapsClass RTpixelMaps;
	private: CSgenerateHTMLdocumentationClass CSgenerateHTMLdocumentation;
	private: CSgenerateObjectOrientedCodeClass CSgenerateObjectOrientedCode;
	private: CSgenerateConstFunctionArgumentCodeClass CSgenerateConstFunctionArgumentCode;
	public: void generateCodeStructure(const string topLevelFileName, int width, const int height, const string outputLDRfileName, const string outputSVGfileName, const string outputPPMfileName, string outputHTMLfileName, const bool useOutputLDRfile, const bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, const bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, const bool outputFileConnections, const string topLevelFunctionName, const bool generateOOcode, const bool generateConstFunctionArgumentsCode);
};


#endif
