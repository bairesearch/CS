 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
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

	//bool htmlDocumentationGenerationPreventsDisplay = false;
	bool generateHTMLdocumentationModeCombined = false;
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
	{
		if(traceFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
		{
			generateHTMLdocumentationModeCombined = false;	//separate
		}
		else
		{
			generateHTMLdocumentationModeCombined = true;
			//htmlDocumentationGenerationPreventsDisplay = true;
		}
	}
				
					
	if(display)
	{
		LDopengl.initiateOpenGL(width, height, 0, 0, false);
	}

	//1. for every file
		//2. for every include file listed
			//3. add to ref list
	//for every reference
		//for every reference, and merge all

	CSfileContainer* topLevelReferenceInListContainer = new CSfileContainer();	//cFile (eg, main.c)
	CSfile* topLevelReferenceInList = new CSfile();
	topLevelReferenceInList->name = topLevelFileName;
	topLevelReferenceInListContainer->fileObject = topLevelReferenceInList;
	CSfileContainer* firstObjectInTopLevelBelowListContainer = new CSfileContainer();	//hFile (eg, main.h)
	topLevelReferenceInList->firstFileInBelowListContainer = firstObjectInTopLevelBelowListContainer;

	bool hFileFound = CSoperations.getIncludeFileNamesFromCorHfile(firstObjectInTopLevelBelowListContainer, topLevelReferenceInListContainer, topLevelReferenceInList, topLevelFileName, 0, true);
	if(!hFileFound)
	{
		cout << "generateCodeStructure{} error: !hFileFound: " << topLevelFileName << endl;
	}

	CSfile* firstReferenceInTopLevelBelowList = firstObjectInTopLevelBelowListContainer->fileObject;

	#ifndef CS_DEBUG_DISABLE_FUNCTION_PARSING
	CSoperations.attachFunctionReferenceTargets(firstObjectInTopLevelBelowListContainer, 0);	//added 3h1a
	#endif
	
	CSdraw.initiateMaxXatParticularY();
	LDreference* firstReferenceInPrintList = new LDreference();

	SHAREDvars.setCurrentDirectory(outputFolder);

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();
	XMLparserTag* currentTagInSVGFile = firstTagInSVGFile;

	bool usePredefinedGrid = false;
	XMLparserTag* firstTagInGridTag = NULL;
	#ifdef CS_SUPPORT_PREDEFINED_GRID
	usePredefinedGrid = initialiseGrid(&firstTagInGridTag);
	#endif

	#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY
	if(generateHTMLdocumentationModeCombined)
	{
		//create independent file hierarchy
		bool outputFunctionsConnectivityTemp = false;
		bool traceFunctionUpwardsTemp = false;	//should already be false
		LDreference* currentReferenceInPrintListTemp = CSdraw.createFileObjectListBoxes(firstReferenceInPrintList, firstObjectInTopLevelBelowListContainer, firstObjectInTopLevelBelowListContainer, &currentTagInSVGFile, outputFunctionsConnectivityTemp, traceFunctionUpwardsTemp, firstTagInGridTag, usePredefinedGrid);
		if(outputFileConnections)
		{
			currentReferenceInPrintListTemp = CSdraw.createFileObjectListConnections(currentReferenceInPrintListTemp, firstObjectInTopLevelBelowListContainer, NULL, &currentTagInSVGFile, traceFunctionUpwards);
		}
		CSdraw.createFileObjectListBoxesPrintedReset(firstObjectInTopLevelBelowListContainer);
	
		string codeStructureFilesSVGfileName = CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY_FILES_FILE_NAME;
		if(!LDsvg.writeSVGfile(codeStructureFilesSVGfileName, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
		{
			result = false;
		}
		
		delete firstTagInSVGFile;
		firstTagInSVGFile = new XMLparserTag();
		currentTagInSVGFile = firstTagInSVGFile;
		
		delete firstReferenceInPrintList;
		firstReferenceInPrintList = new LDreference();
	}
	#endif
	
	LDreference* currentReferenceInPrintList = CSdraw.createFileObjectListBoxes(firstReferenceInPrintList, firstObjectInTopLevelBelowListContainer, firstObjectInTopLevelBelowListContainer, &currentTagInSVGFile, outputFunctionsConnectivity, traceFunctionUpwards, firstTagInGridTag, usePredefinedGrid);
	if(outputFileConnections)
	{
		currentReferenceInPrintList = CSdraw.createFileObjectListConnections(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, NULL, &currentTagInSVGFile, traceFunctionUpwards);
	}
	
	#ifndef CS_DEBUG_DISABLE_FUNCTION_PARSING
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
			//cout << "topLevelFunctionName = " << topLevelFunctionName << endl;
			
			//CSfunction* topLevelFunctionObject = firstReferenceInTopLevelBelowList->firstFunctionInFunctionList;
			
			#ifdef CS_DRAW_PRINT_TOP_LEVEL_FUNCTION
			CSfunction* entryPointFunctionObjectArtificial = new CSfunction();
			entryPointFunctionObjectArtificial->firstReferenceInFunctionReferenceList = topLevelFunctionObject;
			#else
			CSfunction* entryPointFunctionObjectArtificial = topLevelFunctionObject;
			#endif
			
			entryPointFunctionObjectArtificial->printX = firstReferenceInTopLevelBelowList->printX;
			entryPointFunctionObjectArtificial->printY = firstReferenceInTopLevelBelowList->printY;
			entryPointFunctionObjectArtificial->col = firstReferenceInTopLevelBelowList->col;

			firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] = firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] + 1;
			entryPointFunctionObjectArtificial->printed = true;
			if(!(firstReferenceInTopLevelBelowList->printed))
			{
				cerr << "error" << endl;
				exit(EXIT_ERROR);
			}

			#ifdef CS_DRAW_PRINT_TOP_LEVEL_FUNCTION
			currentReferenceInPrintList = CSdraw.createFunctionObjectListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, entryPointFunctionObjectArtificial, firstObjectInTopLevelBelowListContainer, 0, topLevelFunctionObject, &currentTagInSVGFile, traceFunctionUpwards, false, NULL, usePredefinedGrid, true);
			#else
			CSfunction* currentReferenceInFunctionReferenceList = topLevelFunctionObjectArtificial->firstReferenceInFunctionReferenceList;
			while(currentReferenceInFunctionReferenceList->next != NULL)
			{
				//cout << "topLevelFunctionObject->firstReferenceInFunctionReferenceList" << endl;
				currentReferenceInPrintList = CSdraw.createFunctionObjectListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionObjectArtificial, firstObjectInTopLevelBelowListContainer, 0, currentReferenceInFunctionReferenceList, &currentTagInSVGFile, traceFunctionUpwards, false, NULL, usePredefinedGrid, false);
				currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
			}
			#endif
			CSdraw.resetPrintedFunctionConnections(firstReferenceInTopLevelBelowList, topLevelFunctionObject, false, NULL);

			#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY
			if(generateHTMLdocumentationModeCombined)
			{
				string codeStructureFunctionsSVGfileName = CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY_FUNCTIONS_FILE_NAME;
				if(!LDsvg.writeSVGfile(codeStructureFunctionsSVGfileName, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
				{
					result = false;
				}
			}
			#endif
	
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
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)	//generateHTMLdocumentationModeCombined
				{//generate documentation for all functions...
					CSgenerateHTMLdocumentation.generateHTMLdocumentationFunctions(firstTagInSVGFile, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, traceFunctionUpwards, usePredefinedGrid, outputHTMLfileName);
					//cannot display in OpenGL, as LD vector graphics references have been deleted (already saved to file)
					//htmlDocumentationGenerationPreventsDisplay = true;
				}
			}
			
			#ifdef CS_DRAW_PRINT_TOP_LEVEL_FUNCTION
			delete entryPointFunctionObjectArtificial;
			#endif
		}
		else
		{
			cerr << "error: !topLevelFunctionNameFound: " << topLevelFunctionName << endl;
			exit(EXIT_ERROR);
		}
	}

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
	
	#endif

	if(!generateHTMLdocumentationModeCombined)
	{//do not display if generating html (unless tracing single file)

		if(!LDsvg.writeSVGfile(outputSVGfileName, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
		{
			result = false;
		}
		delete firstTagInSVGFile;
		
		if(useOutputLDRfile || display)
		{
			LDreferenceManipulation.writeReferencesToFile(outputLDRfileName, firstReferenceInPrintList);
		}

		if(display)
		{
			//re-parse, then re-write to create a collapsed referencelist file...
			//method1:
			string topLevelSceneFileName = outputLDRfileName;
			string topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
			LDreference* initialReferenceInSceneFile = new LDreference();
			LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!LDparser.parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cerr << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				exit(EXIT_ERROR);
			}
			LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);

			uchar* rgbMap = new uchar[width*height*RGB_NUM];

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
				RTpixelMaps.generatePixmapFromRGBmap(outputPPMfileName, width, height, rgbMap);
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
		LDopengl.exitOpenGL();
	}
}

#ifdef CS_SUPPORT_PREDEFINED_GRID
bool CSexecflowClass::initialiseGrid(XMLparserTag** firstTagInGridTag)
{	
	bool usePredefinedGrid = false;
	
	bool tempResult = true;
	XMLparserTag* firstTagInRulesTag = XMLparserClass.parseTagDownALevel(CSfirstTagInXMLfile, RULES_XML_TAG_rules, &tempResult);
	if(tempResult)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		while(currentTag->nextTag != NULL)
		{
			if(currentTag->name == RULES_XML_TAG_grid)
			{
				*firstTagInGridTag = XMLparserClass.parseTagDownALevel(currentTag, RULES_XML_TAG_grid, &tempResult);
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
	
	#ifdef CS_SUPPORT_PREDEFINED_GRID_VERIFY_INTEGRITY
	XMLparserTag* currentFileTag = *firstTagInGridTag;
	while(currentFileTag->nextTag != NULL)
	{
		int xPos;
		int yPos;
		
		if(currentFileTag->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
		{
			XMLparserAttribute* currentAttributeTag = currentFileTag->firstAttribute;
			while(currentAttributeTag->nextAttribute != NULL)
			{
				if(currentAttributeTag->name == RULES_XML_ATTRIBUTE_x)
				{
					xPos = SHAREDvars.convertStringToInt(currentAttributeTag->value);
				}
				if(currentAttributeTag->name == RULES_XML_ATTRIBUTE_y)
				{
					xPos = SHAREDvars.convertStringToInt(currentAttributeTag->value);
				}
				currentAttributeTag = currentAttributeTag->nextAttribute;
			}
		}

		XMLparserTag* currentFileTag2 = *firstTagInGridTag;
		while(currentFileTag2->nextTag != NULL)
		{
			if(currentFileTag2 != currentFileTag)
			{
				int xPos2;
				int yPos2;
				
				if(currentFileTag2->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
				{
					XMLparserAttribute* currentAttributeTag = currentFileTag2->firstAttribute;
					while(currentAttributeTag->nextAttribute != NULL)
					{
						if(currentAttributeTag->name == RULES_XML_ATTRIBUTE_x)
						{
							xPos2 = SHAREDvars.convertStringToInt(currentAttributeTag->value);
						}
						if(currentAttributeTag->name == RULES_XML_ATTRIBUTE_y)
						{
							xPos2 = SHAREDvars.convertStringToInt(currentAttributeTag->value);
						}
						currentAttributeTag = currentAttributeTag->nextAttribute;
					}
				}
				
				if((xPos == xPos2) && (yPos == yPos2))
				{
					usePredefinedGrid = false;
					cerr << "CS_SUPPORT_PREDEFINED_GRID: CSexecflowClass::initialiseGrid error: <grid> <file> tag found with identical x/y coordinates to another <file> tag" << endl;
					exit(EXIT_ERROR);
				}
			}
		
			currentFileTag2 = currentFileTag2->nextTag;
		}
		
		currentFileTag = currentFileTag->nextTag;
	}				
	#endif
	
	return usePredefinedGrid;
}
#endif
	
