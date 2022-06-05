 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSexecflow.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
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
	#ifdef CS_GENERATE_CPP_CLASSES
	private: CSgenerateObjectOrientedCodeClass CSgenerateObjectOrientedCode;
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	private: CSgenerateConstFunctionArgumentCodeClass CSgenerateConstFunctionArgumentCode;
	#endif
	public: void generateCodeStructure(const string topLevelFileName, int width, const int height, const string outputLDRfileName, const string outputSVGfileName, const string outputPPMfileName, string outputHTMLfileName, const bool useOutputLDRfile, const bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, const bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, const bool outputFileConnections, const string topLevelFunctionName, const bool generateOOcode, const bool generateConstFunctionArgumentsCode);
	#ifdef CS_SUPPORT_PREDEFINED_GRID
	private: bool initialiseGrid(XMLparserTag** firstTagInGridTag);
	#endif
};


#endif
