/*******************************************************************************
 *
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a5g 01-Nov-2011
 *
 *******************************************************************************/

//issues, need to flip vertically

#include "CSexecflow.h"
#include "CSoperations.h"
#include "CSdraw.h"
#include "CSreferenceClass.h"
#include "LDsvg.h"
#include "LDopengl.h"
#include "XMLrulesClass.h"
#include "LDparser.h"
#include "LDsprite.h"
#include "LDreferenceManipulation.h"
#include "RTpixelMaps.h"


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;


void printCS(string topLevelFileName, string topLevelFunctionName, int width, int height, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, bool useOutputLDRFile, bool useOutputPPMFile, bool display, bool outputFunctionsConnectivity, bool traceAFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards)
{
	if(display)
	{
		initiateOpenGL(width, height);
	}
	
	char * outputFileNameLDRcharstar = const_cast<char*>(outputLDRFileName.c_str());
	char * displayFileNamePPMcharstar = const_cast<char*>(outputPPMFileName.c_str());
	char * outputFileNameSVGcharstar = const_cast<char*>(outputSVGFileName.c_str());

	//1. for every f
		//2. for every include f listed
			//3. add to ref list
	//for every reference
		//for every reference, and merge all

	CSReference * topLevelReferenceInList = new CSReference();
	topLevelReferenceInList->name = topLevelFileName;
	CSReference * firstReferenceInTopLevelBelowList = new CSReference();
	topLevelReferenceInList->firstReferenceInBelowList = firstReferenceInTopLevelBelowList;

	getIncludeFileNamesFromCorHFile(firstReferenceInTopLevelBelowList, topLevelReferenceInList, topLevelFileName, 0);

	//cout << "h1" << endl;



	initiateMaxXAtAParticularY();
	Reference * firstReferenceInPrintList = new Reference();

	//CSReference * topLevelFunctionReference = new CSReference();
	//topLevelFunctionReference->name = topLevelFunctionName;


	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif
	
	ofstream writeFileObject(outputFileNameSVGcharstar);
	writeSVGHeader(&writeFileObject);
		//writeFileObject->put(CHAR_SPACE);

	//cout << "h0" << endl;

	Reference * currentReferenceInPrintList = createFileReferenceListBoxes(firstReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &writeFileObject, outputFunctionsConnectivity, traceAFunctionUpwards);
	currentReferenceInPrintList = createFileReferenceListConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &writeFileObject, traceAFunctionUpwards);


	//cout << "h1" << endl;
	if(outputFunctionsConnectivity)
	{
		CSReference * topLevelFunctionReference = firstReferenceInTopLevelBelowList->firstReferenceInFunctionList;
		topLevelFunctionReference->printX = firstReferenceInTopLevelBelowList->printX;
		topLevelFunctionReference->printY = firstReferenceInTopLevelBelowList->printY;
		topLevelFunctionReference->col = firstReferenceInTopLevelBelowList->col;

		//topLevelFunctionReference->printX = firstReferenceInTopLevelBelowList->printX + (firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0])*200;	//CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR
		//topLevelFunctionReference->printY = firstReferenceInTopLevelBelowList->printY + 200*0.3;	//CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR
		//cout << "(firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0])*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR = " << (firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0])*200 << endl;
		//out << "firstReferenceInTopLevelBelowList->printY + CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*0.3 = " << firstReferenceInTopLevelBelowList->printY + 200*0.3 << endl;

		firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] = firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] + 1;
		topLevelFunctionReference->printed = true;
		if(!(firstReferenceInTopLevelBelowList->printed))
		{
			cout << "error" << endl;
			exit(0);
		}
		//cout << "h1b" << endl;
		//cout << "topLevelFunctionReference->firstReferenceInFunctionReferenceList->name = " << topLevelFunctionReference->firstReferenceInFunctionReferenceList->name << endl;

		/*
		cout << "firstReferenceInTopLevelBelowList->col = " << firstReferenceInTopLevelBelowList->col << endl;
		cout << "currentReferenceInPrintList->name = " << currentReferenceInPrintList->name << endl;
		cout << "firstReferenceInTopLevelBelowList->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "topLevelFunctionReference->name = " << topLevelFunctionReference->name << endl;
		cout << "firstReferenceInTopLevelBelowList->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "topLevelFunctionReference->name = " << topLevelFunctionReference->name << endl;
		*/

		/*
		cout << "aboveLevelFileReference->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "aboveLevelFunctionReference->name = " << topLevelFunctionReference->name << endl;
		cout << "firstReferenceInTopLevelBelowList->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "functionLevel = " << 0 << endl;
		cout << "functionReferenceNameToFind = " << topLevelFunctionReference->firstReferenceInFunctionReferenceList->name << endl;
		*/

		currentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionReference, firstReferenceInTopLevelBelowList, 0, topLevelFunctionReference->firstReferenceInFunctionReferenceList->name, &writeFileObject, traceAFunctionUpwards);


		if(traceAFunctionUpwards)
		{
			//cout << "error shouldnt be here" << endl;

			bool foundBottomLevelFunctionRef = false;
			string fileNameHoldingFunction = "";
			CSReference * bottomLevelFunctionToTraceUpwards = findPrintedFunctionReferenceWithName(bottomLevelFunctionNameToTraceUpwards, NULL, firstReferenceInTopLevelBelowList, &foundBottomLevelFunctionRef, &fileNameHoldingFunction);
			if(foundBottomLevelFunctionRef)
			{
				cout << "\nTrace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << bottomLevelFunctionToTraceUpwards->name << "()" << endl;

				//cout << "foundBottomLevelFunctionRef" << endl;
				currentReferenceInPrintList = traceFunctionsUpwardsAndDrawOrHighLightThese(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, &writeFileObject, topLevelFunctionName);
			}
			else
			{
				cout << "error: foundBottomLevelFunctionRef " << bottomLevelFunctionNameToTraceUpwards << " cannot be found" << endl;
				exit(0);
			}
		}

	}



	//cout << "h2" << endl;

	writeSVGFooter(&writeFileObject);
	writeFileObject.close();



	/*
	cout << "h2" <<endl;
	exit(0);
	*/

	if(useOutputLDRFile || display)
	{
		writeReferencesToFile(outputFileNameLDRcharstar, firstReferenceInPrintList);
	}

	if(display)
	{
		//re-parse, then re-write to create a collapsed referencelist file...
		//method1:
		char * topLevelSceneFileName = outputFileNameLDRcharstar;
		char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
		Reference * initialReferenceInSceneFile = new Reference();
		Reference * topLevelReferenceInSceneFile = new Reference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
			exit(0);
		}
		write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
		/* method2: why doesnt this work - "invalid dat file for conversion to rgb"
		char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
		write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, firstReferenceInPrintList);
		*/

		//cout << "has" << endl;


		unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

		//setViewPort2DOrtho(-100.0, 2000.0, -100.0, 2000.0);
		setViewPort3DOrtho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

		//now reparse file
		Reference * initialReferenceInCollapsedSceneFile = new Reference();
		Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
			exit(0);
		}

		drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
		drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			//due to opengl code bug, need to execute this function twice.

		if(useOutputPPMFile)
		{
			generatePixmapFromRGBMap(displayFileNamePPMcharstar, width, height, rgbMap);
		}
		
		delete rgbMap;

	}
	else
	{
		//must use an external program to view the .ldr file (Eg LDView)
	}


}


