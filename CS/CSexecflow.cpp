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
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3n7d 17-August-2020
 * /
 *******************************************************************************/


#include "CSexecflow.hpp"
#ifdef CS_GENERATE_CPP_CLASSES
#endif
#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
#endif

#ifndef LINUX
	#include <windows.h>
#endif




void CSexecflowClass::generateCodeStructure(const string topLevelFileName, int width, const int height, const string outputLDRfileName, const string outputSVGfileName, const string outputPPMfileName, string outputHTMLfileName, const bool useOutputLDRfile, const bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, const bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, const bool outputFileConnections, const string topLevelFunctionName, const bool generateOOcode, const bool generateConstFunctionArgumentsCode)
{
	bool result = true;

	bool htmlDocumentationGenerationPreventsDisplay = false;


	if(display)
	{
		LDopengl.initiateOpenGL(width, height, 0, 0, false);
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

	bool hFileFound = CSoperations.getIncludeFileNamesFromCorHfile(firstObjectInTopLevelBelowListContainer, topLevelReferenceInListContainer, topLevelReferenceInList, topLevelFileName, 0);
	if(!hFileFound)
	{
		cout << "generateCodeStructure{} error: !hFileFound: " << topLevelFileName << endl;
	}
	//cout << "done getIncludeFileNamesFromCorHfile" << endl;

	CSfile* firstReferenceInTopLevelBelowList = firstObjectInTopLevelBelowListContainer->fileObject;

	CSoperations.attachFunctionReferenceTargets(firstObjectInTopLevelBelowListContainer);	//added 3h1a

	CSdraw.initiateMaxXatParticularY();
	LDreference* firstReferenceInPrintList = new LDreference();

	SHAREDvars.setCurrentDirectory(outputFolder);

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();
	XMLparserTag* currentTagInSVGFile = firstTagInSVGFile;

	//cout << "qt0" << endl;

	#ifdef CS_SUPPORT_PREDEFINED_GRID
	bool usePredefinedGrid = false;
	bool tempResult = true;
	XMLparserTag* firstTagInRulesTag = XMLparserClass.parseTagDownALevel(CSfirstTagInXMLfile, RULES_XML_TAG_rules, &tempResult);
	XMLparserTag* firstTagInGridTag = NULL;
	if(tempResult)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		while(currentTag->nextTag != NULL)
		{
			if(currentTag->name == RULES_XML_TAG_grid)
			{
				firstTagInGridTag = XMLparserClass.parseTagDownALevel(currentTag, RULES_XML_TAG_grid, &tempResult);
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

	//cout << "qt1" << endl;

	LDreference* currentReferenceInPrintList = CSdraw.createFileObjectListBoxes(firstReferenceInPrintList, firstObjectInTopLevelBelowListContainer, firstObjectInTopLevelBelowListContainer, &currentTagInSVGFile, outputFunctionsConnectivity, traceFunctionUpwards, firstTagInGridTag, usePredefinedGrid);
	if(outputFileConnections)
	{
		currentReferenceInPrintList = CSdraw.createFileObjectListConnections(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, NULL, &currentTagInSVGFile, traceFunctionUpwards);
	}
	//cout << "qt2" << endl;
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
				cerr << "error" << endl;
				exit(EXIT_ERROR);
			}

			CSfunction* currentReferenceInFunctionReferenceList = topLevelFunctionObject->firstReferenceInFunctionReferenceList;
			while(currentReferenceInFunctionReferenceList->next != NULL)
			{
				currentReferenceInPrintList = CSdraw.createFunctionObjectListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionObject, firstObjectInTopLevelBelowListContainer, 0, currentReferenceInFunctionReferenceList, &currentTagInSVGFile, traceFunctionUpwards, false, NULL, usePredefinedGrid);
				currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
			}
			CSdraw.resetPrintedFunctionConnections(firstReferenceInTopLevelBelowList, topLevelFunctionObject, false, NULL);

			if(traceFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
			{
				CSfile* fileObjectHoldingFunction = NULL;
				CSfunction* bottomLevelFunctionToTraceUpwards = NULL;
				bool foundBottomLevelFunctionRef = CSdraw.findPrintedFunctionObjectWithName(bottomLevelFunctionNameToTraceUpwards, firstObjectInTopLevelBelowListContainer, &fileObjectHoldingFunction, &bottomLevelFunctionToTraceUpwards);

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
					CSgenerateHTMLdocumentation.generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, bottomLevelFunctionToTraceUpwards, fileNameHoldingFunction, &currentTagInSVGFile, generateHTMLdocumentationMode, &HTMLdocumentationFunctionNOTUSED, &outputSVGfileName, useOutputHTMLfile, outputHTMLfileName, traceFunctionUpwards);
				}
				else
				{
					cerr << "error: foundBottomLevelFunctionRef " << bottomLevelFunctionNameToTraceUpwards << " cannot be found" << endl;
					exit(EXIT_ERROR);
				}
			}
			else
			{
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
				{//generate documentation for all functions...
					CSgenerateHTMLdocumentation.generateHTMLdocumentationFunctions(firstTagInSVGFile, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, traceFunctionUpwards, usePredefinedGrid, outputHTMLfileName);
					htmlDocumentationGenerationPreventsDisplay = true;	//cannot display in OpenGL/save to file, as LD vector graphics references have been deleted
				}
			}
		}
		else
		{
			cerr << "error: !topLevelFunctionNameFound: " << topLevelFunctionName << endl;
			exit(EXIT_ERROR);
		}
	}

	//cout << "qt3" << endl;

	#ifdef CS_GENERATE_CPP_CLASSES
	if(generateOOcode)
	{
		if(!CSgenerateObjectOrientedCode.generateCPPclasses(firstObjectInTopLevelBelowListContainer))
		{
			result = false;
		}
	}
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	if(generateConstFunctionArgumentsCode)
	{
		if(!CSgenerateConstFunctionArgumentCode.generateConstFunctionArguments(firstObjectInTopLevelBelowListContainer))
		{
			result = false;
		}
	}
	#endif


	if(!htmlDocumentationGenerationPreventsDisplay)
	{//do not display if generating html (unless tracing single file)

		if(!LDsvg.writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
		{
			result = false;
		}
		delete firstTagInSVGFile;

		if(useOutputLDRfile || display)
		{
			LDreferenceManipulation.writeReferencesToFile(outputFileNameLDRcharstar, firstReferenceInPrintList);
		}

		if(display)
		{
			//re-parse, then re-write to create a collapsed referencelist file...
			//method1:
			char* topLevelSceneFileName = outputFileNameLDRcharstar;
			char* topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
			LDreference* initialReferenceInSceneFile = new LDreference();
			LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!LDparser.parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cerr << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				exit(EXIT_ERROR);
			}
			LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);


			unsigned char* rgbMap = new unsigned char[width*height*RGB_NUM];

			//setViewPort2Dortho(-100.0, 2000.0, -100.0, 2000.0);
			LDopengl.setViewPort3Dortho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

			//now reparse file
			LDreference* initialReferenceInCollapsedSceneFile = new LDreference();
			LDreference* topLevelReferenceInCollapsedSceneFile = new LDreference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!LDparser.parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
			{//file does not exist
				cerr << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
				exit(EXIT_ERROR);
			}

			LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				//due to opengl code bug, need to execute this function twice.

			if(useOutputPPMfile)
			{
				RTpixelMaps.generatePixmapFromRGBmap(displayFileNamePPMcharstar, width, height, rgbMap);
			}

			delete rgbMap;

		}
		else
		{
			//must use an external program to view the .ldr file (Eg LDView)
		}
	}
	//cout << "qt4" << endl;

	if(display)
	{
		LDopengl.exitOpenGL();
	}

}

