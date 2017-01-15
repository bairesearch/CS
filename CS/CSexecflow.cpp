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
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h11c 10-December-2015
 *
 *******************************************************************************/

//issues, need to flip vertically

#include "CSexecflow.h"
#include "CSoperations.h"
#include "CSdraw.h"
#include "LDsvg.h"
#include "LDopengl.h"
#include "XMLrulesClass.h"
#include "LDparser.h"
#include "LDsprite.h"
#include "RTpixelMaps.h"
#include "CSgenerateHTMLdocumentation.h"
#ifdef CS_GENERATE_CPP_CLASSES
#include "CSgenerateObjectOrientedCode.h"
#endif
#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
#include "CSgenerateConstFunctionArgumentCode.h"
#endif

#ifndef LINUX
	#include <windows.h>
#endif




void generateCodeStructure(string topLevelFileName, int width, int height, string outputLDRfileName, string outputSVGfileName, string outputPPMfileName, string outputHTMLfileName, bool useOutputLDRfile, bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, bool outputFileConnections, string topLevelFunctionName, bool generateOOcode, bool generateConstFunctionArgumentsCode)
{
	bool result = true;

	bool htmlDocumentationGenerationPreventsDisplay = false;

	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "useOutputHTMLfile = " << int(useOutputHTMLfile) << endl;
	cout << "outputHTMLfileName = " << outputHTMLfileName << endl;
	cout << "generateHTMLdocumentationMode = " << generateHTMLdocumentationMode << endl;
	cout << "traceFunctionUpwards = " << traceFunctionUpwards << endl;
	#endif

	if(display)
	{
		initiateOpenGL(width, height, 0, 0, false);
	}

	char* outputFileNameLDRcharstar = const_cast<char*>(outputLDRfileName.c_str());
	char* displayFileNamePPMcharstar = const_cast<char*>(outputPPMfileName.c_str());
	char* outputFileNameSVGcharstar = const_cast<char*>(outputSVGfileName.c_str());

	//1. for every f
		//2. for every include f listed
			//3. add to ref list
	//for every reference
		//for every reference, and merge all

	CSfileContainer* topLevelReferenceInListContainer = new CSfileContainer();
	CSfile* topLevelReferenceInList = new CSfile();
	topLevelReferenceInList->name = topLevelFileName;
	topLevelReferenceInListContainer->fileObject = topLevelReferenceInList;
	CSfileContainer* firstObjectInTopLevelBelowListContainer = new CSfileContainer();
	topLevelReferenceInList->firstFileInBelowListContainer = firstObjectInTopLevelBelowListContainer;

	//cout << "start getIncludeFileNamesFromCorHfile{}" << endl;
	bool hFileFound = getIncludeFileNamesFromCorHfile(firstObjectInTopLevelBelowListContainer, topLevelReferenceInListContainer, topLevelReferenceInList, topLevelFileName, 0);
	//cout << "finish getIncludeFileNamesFromCorHfile{}" << endl;
	if(!hFileFound)
	{
		cout << "generateCodeStructure{} error: !hFileFound: " << topLevelFileName << endl;
	}
	
	CSfile* firstReferenceInTopLevelBelowList = firstObjectInTopLevelBelowListContainer->fileObject;

	attachFunctionReferenceTargets(firstObjectInTopLevelBelowListContainer);	//added 3h1a

	initiateMaxXatParticularY();
	LDreference* firstReferenceInPrintList = new LDreference();

	setCurrentDirectory(tempFolder);

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();
	XMLparserTag* currentTagInSVGFile = firstTagInSVGFile;


	#ifdef CS_SUPPORT_PREDEFINED_GRID
	bool usePredefinedGrid = false;
	bool tempResult = true;
	XMLparserTag* firstTagInRulesTag = parseTagDownALevel(CSfirstTagInXMLfile, RULES_XML_TAG_rules, &tempResult);
	XMLparserTag* firstTagInGridTag = NULL;
	if(tempResult)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		while(currentTag->nextTag != NULL)
		{
			if(currentTag->name == RULES_XML_TAG_grid)
			{
				firstTagInGridTag = parseTagDownALevel(currentTag, RULES_XML_TAG_grid, &tempResult);
				if(tempResult)
				{
					usePredefinedGrid = true;
				}
			}
			currentTag = currentTag->nextTag;
		}
		if(!usePredefinedGrid)
		{
			cout << "error: CS_SUPPORT_PREDEFINED_GRID: !foundGridTag - check CSrules.xml" << endl;
		}
	}
	#endif

	LDreference* currentReferenceInPrintList = createFileObjectListBoxes(firstReferenceInPrintList, firstObjectInTopLevelBelowListContainer, firstObjectInTopLevelBelowListContainer, &currentTagInSVGFile, outputFunctionsConnectivity, traceFunctionUpwards, firstTagInGridTag, usePredefinedGrid);
	if(outputFileConnections)
	{
		currentReferenceInPrintList = createFileObjectListConnections(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, NULL, &currentTagInSVGFile, traceFunctionUpwards);
	}
	if(outputFunctionsConnectivity)
	{
		CSfunction* currentObjectInTopLevelBelowList = firstReferenceInTopLevelBelowList->firstFunctionInFunctionList;
		bool topLevelFunctionNameFound = false;
		CSfunction* topLevelFunctionObject = NULL;
		while(currentObjectInTopLevelBelowList->next != NULL)
		{
			if(currentObjectInTopLevelBelowList->name == topLevelFunctionName)
			{
				topLevelFunctionObject = currentObjectInTopLevelBelowList;
				topLevelFunctionNameFound = true;
			}
			currentObjectInTopLevelBelowList = currentObjectInTopLevelBelowList->next;
		}
		if(topLevelFunctionNameFound)
		{
			//CSfunction* topLevelFunctionObject = firstReferenceInTopLevelBelowList->firstFunctionInFunctionList;
			topLevelFunctionObject->printX = firstReferenceInTopLevelBelowList->printX;
			topLevelFunctionObject->printY = firstReferenceInTopLevelBelowList->printY;
			topLevelFunctionObject->col = firstReferenceInTopLevelBelowList->col;

			firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] = firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] + 1;
			topLevelFunctionObject->printed = true;
			if(!(firstReferenceInTopLevelBelowList->printed))
			{
				cout << "error" << endl;
				exit(0);
			}

			CSfunction* currentReferenceInFunctionReferenceList = topLevelFunctionObject->firstReferenceInFunctionReferenceList;
			while(currentReferenceInFunctionReferenceList->next != NULL)
			{
				currentReferenceInPrintList = createFunctionObjectListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionObject, firstObjectInTopLevelBelowListContainer, 0, currentReferenceInFunctionReferenceList, &currentTagInSVGFile, traceFunctionUpwards, false, NULL, usePredefinedGrid);
				currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
			}
			resetPrintedFunctionConnections(firstReferenceInTopLevelBelowList, topLevelFunctionObject, false, NULL);

			if(traceFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
			{
				CSfile* fileObjectHoldingFunction = NULL;
				CSfunction* bottomLevelFunctionToTraceUpwards = NULL;
				bool foundBottomLevelFunctionRef = findPrintedFunctionObjectWithName(bottomLevelFunctionNameToTraceUpwards, firstObjectInTopLevelBelowListContainer, &fileObjectHoldingFunction, &bottomLevelFunctionToTraceUpwards);
				
				if(foundBottomLevelFunctionRef)
				{
					string fileNameHoldingFunction = fileObjectHoldingFunction->name;

					if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
					{
						if(!useOutputHTMLfile)
						{//still use single html file for function, but automatically generate html file name (based on function name)
							useOutputHTMLfile = true;
							outputHTMLfileName = bottomLevelFunctionNameToTraceUpwards + HTML_EXTENSION;	//automatically generate html file name (based on function name)
						}
					}
					string HTMLdocumentationFunctionNOTUSED = "";
					generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, bottomLevelFunctionToTraceUpwards, fileNameHoldingFunction, &currentTagInSVGFile, generateHTMLdocumentationMode, &HTMLdocumentationFunctionNOTUSED, &outputSVGfileName, useOutputHTMLfile, outputHTMLfileName, traceFunctionUpwards);
				}
				else
				{
					cout << "error: foundBottomLevelFunctionRef " << bottomLevelFunctionNameToTraceUpwards << " cannot be found" << endl;
					exit(0);
				}
			}
			else
			{
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
				{//generate documentation for all functions...
					generateHTMLdocumentationFunctions(firstTagInSVGFile, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, traceFunctionUpwards, usePredefinedGrid, outputHTMLfileName);
					htmlDocumentationGenerationPreventsDisplay = true;	//cannot display in OpenGL/save to file, as LD vector graphics references have been deleted
				}
			}
		}
		else
		{
			cout << "error: !topLevelFunctionNameFound: " << topLevelFunctionName << endl;
			exit(0);
		}
	}
	
	#ifdef CS_GENERATE_CPP_CLASSES
	if(generateOOcode)
	{
		if(!generateCPPclasses(firstObjectInTopLevelBelowListContainer))
		{
			result = false;
		}
	}
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	if(generateConstFunctionArgumentsCode)
	{
		if(!generateConstFunctionArguments(firstObjectInTopLevelBelowListContainer))
		{
			result = false;
		}
	}
	#endif	
	
	
	if(!htmlDocumentationGenerationPreventsDisplay)
	{//do not display if generating html (unless tracing single file)

		if(!writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
		{
			result = false;
		}
		delete firstTagInSVGFile;

		if(useOutputLDRfile || display)
		{
			writeReferencesToFile(outputFileNameLDRcharstar, firstReferenceInPrintList);
		}

		if(display)
		{
			//re-parse, then re-write to create a collapsed referencelist file...
			//method1:
			char* topLevelSceneFileName = outputFileNameLDRcharstar;
			char* topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
			LDreference* initialReferenceInSceneFile = new LDreference();
			LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				exit(0);
			}
			write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);


			unsigned char* rgbMap = new unsigned char[width*height*RGB_NUM];

			//setViewPort2Dortho(-100.0, 2000.0, -100.0, 2000.0);
			setViewPort3Dortho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

			//now reparse file
			LDreference* initialReferenceInCollapsedSceneFile = new LDreference();
			LDreference* topLevelReferenceInCollapsedSceneFile = new LDreference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
				exit(0);
			}

			drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				//due to opengl code bug, need to execute this function twice.

			if(useOutputPPMfile)
			{
				generatePixmapFromRGBmap(displayFileNamePPMcharstar, width, height, rgbMap);
			}

			delete rgbMap;

		}
		else
		{
			//must use an external program to view the .ldr file (Eg LDView)
		}
	}
	
	if(display)
	{
		exitOpenGL();
	}	

}

