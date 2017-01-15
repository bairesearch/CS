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
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3b1a 27-September-2012
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


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

#ifndef LINUX
	#include <windows.h>
#endif


void printCS(string topLevelFileName, string topLevelFunctionName, int width, int height, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, string outputHTMLFileName, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputHTMLFile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceAFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards)
{
	bool htmlDocumentationGenerationPreventsDisplay = false;
	
	cout << "useOutputHTMLFile = " << int(useOutputHTMLFile) << endl;
	cout << "outputHTMLFileName = " << outputHTMLFileName << endl;
	cout << "generateHTMLdocumentationMode = " << generateHTMLdocumentationMode << endl;
	cout << "traceAFunctionUpwards = " << traceAFunctionUpwards << endl;
						
	if(display)
	{
		initiateOpenGL(width, height, 0, 0, false);
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

	bool hFileFound = getIncludeFileNamesFromCorHFile(firstReferenceInTopLevelBelowList, topLevelReferenceInList, topLevelFileName, 0);
	if(!hFileFound)
	{
		cout << "printCS() error: !hFileFound: " << topLevelFileName << endl;
	}

	//cout << "q4" << endl;
	//exit(0);

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
		/*
		cout << "h1b" << endl;

		cout << "firstReferenceInTopLevelBelowList->col = " << firstReferenceInTopLevelBelowList->col << endl;
		cout << "currentReferenceInPrintList->name = " << currentReferenceInPrintList->name << endl;
		cout << "firstReferenceInTopLevelBelowList->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "topLevelFunctionReference->name = " << topLevelFunctionReference->name << endl;

		cout << "aboveLevelFileReference->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "aboveLevelFunctionReference->name = " << topLevelFunctionReference->name << endl;
		cout << "firstReferenceInTopLevelBelowList->name = " << firstReferenceInTopLevelBelowList->name << endl;
		cout << "functionLevel = " << 0 << endl;
		cout << "functionReferenceNameToFind = " << topLevelFunctionReference->firstReferenceInFunctionReferenceList->name << endl;

		cout << "topLevelFunctionReference->firstReferenceInFunctionReferenceList->name = " << topLevelFunctionReference->firstReferenceInFunctionReferenceList->name << endl;
		*/

		//cout << "g1" << endl;
		CSReference * currentReferenceInFunctionReferenceList = topLevelFunctionReference->firstReferenceInFunctionReferenceList;
		while(currentReferenceInFunctionReferenceList->next != NULL)
		{
			//cout << "g1b" << endl;
			currentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionReference, firstReferenceInTopLevelBelowList, 0, currentReferenceInFunctionReferenceList->name, &writeFileObject, traceAFunctionUpwards);
			currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
		}
		//cout << "g2" << endl;

		if(traceAFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
		{
			//cout << "error shouldnt be here" << endl;

			bool foundBottomLevelFunctionRef = false;
			string fileNameHoldingFunction = "";
			CSReference * bottomLevelFunctionToTraceUpwards = findPrintedFunctionReferenceWithName(bottomLevelFunctionNameToTraceUpwards, NULL, firstReferenceInTopLevelBelowList, &foundBottomLevelFunctionRef, &fileNameHoldingFunction);
			if(foundBottomLevelFunctionRef)
			{	
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
				{
					if(!useOutputHTMLFile)
					{//still use single html file for function, but automatically generate html file name (based on function name)
						useOutputHTMLFile = true;
						outputHTMLFileName = bottomLevelFunctionNameToTraceUpwards + HTML_EXTENSION;	//automatically generate html file name (based on function name)
					}
				}
				string HTMLdocumentationFunctionNOTUSED = "";
				generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, fileNameHoldingFunction, &writeFileObject, topLevelFunctionName, generateHTMLdocumentationMode, &HTMLdocumentationFunctionNOTUSED, &outputSVGFileName, useOutputHTMLFile, outputHTMLFileName, traceAFunctionUpwards);							
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
				
				writeFileObject.close();
						
				string HTMLdocumentationBody = "";
				Reference * firstReferenceInPrintListFunction = new Reference();								
				generateHTMLdocumentationForAllFunctions(firstReferenceInTopLevelBelowList, firstReferenceInPrintListFunction, firstReferenceInTopLevelBelowList, topLevelFunctionName, generateHTMLdocumentationMode, useOutputHTMLFile, &HTMLdocumentationBody, &outputSVGFileName, traceAFunctionUpwards);
				delete firstReferenceInPrintListFunction;	//should delete this, as it will contain far too many LD vector graphics references (ie a set of traced references for each function)
				htmlDocumentationGenerationPreventsDisplay = true;	//cannot display in OpenGL/save to file, as LD vector graphics references have been deleted
				
				if(useOutputHTMLFile)
				{//use single html file for project (rather than unique html file per file in project)
					string HTMLdocumentationHeader = generateHTMLdocumentationHeader("Software Project");					
					string HTMLdocumentationFooter = generateHTMLdocumentationFooter();
					string HTMLdocumentation = HTMLdocumentationHeader + HTMLdocumentationBody + HTMLdocumentationFooter;
					ofstream writeFileObjectHTML(outputHTMLFileName.c_str());
					writeStringToFileObject(&HTMLdocumentation, &writeFileObjectHTML);					
				}				
			}		
		}
	}
	

	/*
	cout << "h2" <<endl;
	exit(0);
	*/
	
	if(!htmlDocumentationGenerationPreventsDisplay)
	{//do not display if generating html (unless tracing single file)
		
		writeSVGFooter(&writeFileObject);
		writeFileObject.close();
			
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

}

string generateHTMLdocumentationHeader(string name)
{
	string HTMLdocumentationHeader = "";
	HTMLdocumentationHeader = HTMLdocumentationHeader + "<html><head><title>" + name + " Documentation</title><style type=\"text/css\">TD { font-size:75%; } </style></head><body><h2>" + name + " Documentation</h2><p>Automatically generated with Code Structure Viewer (OpenCS), Project Version: 3b1a 27-September-2012<p>\n";
	return HTMLdocumentationHeader;
}

string generateHTMLdocumentationFooter()
{
	return "</body></html>\n";
}

void writeStringToFileObject(string * s, ofstream * writeFileObject)
{
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject->put((*s)[i]);
	}
}

void generateHTMLdocumentationForAllFunctions(CSReference * firstReferenceInAboveLevelBelowList, Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, string topLevelFunctionName, int generateHTMLdocumentationMode, bool useOutputHTMLFile, string * HTMLdocumentationBody, string * outputSVGFileName, bool traceAFunctionUpwards)
{
	CSReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{
		string fileNameHoldingFunction = currentFileReference->name;
		string outputHTMLFileName = currentFileReference->name + HTML_EXTENSION;
		ifstream tempFileStream(outputHTMLFileName.c_str());
		if(!(tempFileStream.good()))
		{//file does not exist
		
			if(!(currentFileReference->HTMLgenerated))	//this constraint doesnt work as planned
			{//do not parse a file twice
				currentFileReference->HTMLgenerated = true;

				if(!useOutputHTMLFile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					*HTMLdocumentationBody = "";
				}

				CSReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
				while(currentFunctionReference->next != NULL)
				{	
					if(currentFunctionReference->printed)
					{//only create connections to printed boxes

						if(!(currentFunctionReference->HTMLgenerated))
						{//do not parse a function twice
							currentFunctionReference->HTMLgenerated = true;

							string outputSVGFileNameFunction = "";	//only used with traceAFunctionUpwards
							ofstream writeFileObjectFunction;	//only used with traceAFunctionUpwards
							if(traceAFunctionUpwards)
							{
								outputSVGFileNameFunction = currentFunctionReference->name + SVG_EXTENSION;

								string commandCopyOutputSVGFileName = "";
								commandCopyOutputSVGFileName = commandCopyOutputSVGFileName + SYSTEM_COPY_COMMAND + " " + *outputSVGFileName + " " + outputSVGFileNameFunction;	//this is required because Stanford CoreNLP cannot output a file of a given name, it can only output a file with a modified extension
								//#ifdef CS_DEBUG_HTML_DOCUMENTATION
								cout << "commandCopyOutputSVGFileName = " << commandCopyOutputSVGFileName << endl;				
								//#endif
								system(commandCopyOutputSVGFileName.c_str());

								writeFileObjectFunction.open(outputSVGFileNameFunction.c_str(), ofstream::app);
							}

							string HTMLdocumentationFunction = "";
							generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, currentFunctionReference, fileNameHoldingFunction, &writeFileObjectFunction, topLevelFunctionName, generateHTMLdocumentationMode, &HTMLdocumentationFunction, &outputSVGFileNameFunction, false, "", traceAFunctionUpwards);		
							*HTMLdocumentationBody = *HTMLdocumentationBody + HTMLdocumentationFunction; 
							//cout << "HTMLdocumentationFunction = " << HTMLdocumentationFunction << endl;

							if(traceAFunctionUpwards)
							{						
								writeSVGFooter(&writeFileObjectFunction);
								writeFileObjectFunction.close();
							}
						}

					}

					currentFunctionReference = currentFunctionReference->next;
				}

				if(!useOutputHTMLFile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					string HTMLdocumentationHeader = generateHTMLdocumentationHeader(currentFileReference->name);					
					string HTMLdocumentationFooter = generateHTMLdocumentationFooter();
					string HTMLdocumentation = "";
					HTMLdocumentation = HTMLdocumentation + HTMLdocumentationHeader + *HTMLdocumentationBody + HTMLdocumentationFooter;
					#ifdef CS_DEBUG_HTML_DOCUMENTATION
					//cout << "*HTMLdocumentationBody = " << *HTMLdocumentationBody << endl;
					#endif
					string outputHTMLFileName = currentFileReference->name + HTML_EXTENSION;
					ofstream writeFileObjectHTML(outputHTMLFileName.c_str());
					writeStringToFileObject(&HTMLdocumentation, &writeFileObjectHTML);
				}

				if(currentFileReference->firstReferenceInBelowList != NULL)
				{
					generateHTMLdocumentationForAllFunctions(currentFileReference->firstReferenceInBelowList, currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionName, generateHTMLdocumentationMode, useOutputHTMLFile, HTMLdocumentationBody, outputSVGFileName, traceAFunctionUpwards);
				}
			}
		}
		
		currentFileReference = currentFileReference->next;
	}
}

void generateHTMLdocumentationForFunction(Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, CSReference * bottomLevelFunctionToTraceUpwards, string fileNameHoldingFunction, ofstream * writeFileObject, string topLevelFunctionName, int generateHTMLdocumentationMode, string * HTMLdocumentationFunction, string * outputSVGFileNameFunction, bool useOutputHTMLFile, string outputHTMLFileName, bool traceAFunctionUpwards)
{	
	//cout << "higherLevelFunctionFound, " << higherLevelFunction->name << endl;
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
	{
		#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
		cout << "\nStart Trace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << bottomLevelFunctionToTraceUpwards->name << "()" << endl;
		cout << bottomLevelFunctionToTraceUpwards->nameFull << endl;					
		#endif
	}

	string HTMLdocumentationFunctionTraceTableRows = "";
	if(traceAFunctionUpwards)
	{
		//cout << "foundBottomLevelFunctionRef" << endl;
		currentReferenceInPrintList = traceFunctionsUpwardsAndDrawOrHighLightThese(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, writeFileObject, topLevelFunctionName, generateHTMLdocumentationMode, &HTMLdocumentationFunctionTraceTableRows);
	
		traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, topLevelFunctionName);	//this is required for multiple traces per CS execution
	}
	
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
	{
		//cout << "bottomLevelFunctionToTraceUpwards->name = " << bottomLevelFunctionToTraceUpwards->name << endl;
		//cout << "bottomLevelFunctionToTraceUpwards->nameFull = " << bottomLevelFunctionToTraceUpwards->nameFull << endl;
		
		string HTMLdocumentationFunctionInputIntroduction = "";
		HTMLdocumentationFunctionInputIntroduction = HTMLdocumentationFunctionInputIntroduction + "<h3>" + (bottomLevelFunctionToTraceUpwards->name) + "()</h3>";
			
		string HTMLdocumentationFunctionInputArguments = "";
		generateHTMLdocumentationFunctionInputArguments(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionInputArguments);

		*HTMLdocumentationFunction = *HTMLdocumentationFunction + HTMLdocumentationFunctionInputIntroduction + HTMLdocumentationFunctionInputArguments + "<br />";

		if(traceAFunctionUpwards)
		{
			string HTMLdocumentationFunctionTraceTableHeader = "";
			string HTMLdocumentationFunctionTraceTableFooter = "";
			HTMLdocumentationFunctionTraceTableHeader = HTMLdocumentationFunctionTraceTableHeader + "\t<b>Function Trace</b><br /><table border=\"1\"><tr><th>" + "location" + "</th><th>" + "current function being traced" + "</th></tr>\n";
			HTMLdocumentationFunctionTraceTableFooter = HTMLdocumentationFunctionTraceTableFooter + "\t</table>";					
			string HTMLdocumentationFunctionTraceTable = HTMLdocumentationFunctionTraceTableHeader + HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableFooter;
		
			string HTMLdocumentationFunctionTraceImagePlaceHolder = generateHTMLdocumentationFunctionTraceImagePlaceHolder(outputSVGFileNameFunction);	
			
			*HTMLdocumentationFunction = *HTMLdocumentationFunction +  HTMLdocumentationFunctionTraceTable + HTMLdocumentationFunctionTraceImagePlaceHolder;			
		}
		
		if(useOutputHTMLFile)
		{//use single html file for function
			string HTMLdocumentationHeader = generateHTMLdocumentationHeader(bottomLevelFunctionToTraceUpwards->name);					
			string HTMLdocumentationFooter = generateHTMLdocumentationFooter();	
			string HTMLdocumentation = HTMLdocumentationHeader + *HTMLdocumentationFunction + HTMLdocumentationFooter;

			ofstream writeFileObjectHTML(outputHTMLFileName.c_str());
			writeStringToFileObject(&HTMLdocumentation, &writeFileObjectHTML);
		}	
	}

	
}


void generateHTMLdocumentationFunctionInputArguments(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionInputArguments)
{
	*HTMLdocumentationFunctionInputArguments = "";
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<b>Function Arguments</b><br /><table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th></tr>\n";

	int startPositionOfFunctionBrackets = functionNameFull->find(CHAR_OPEN_BRACKET);
	int endPositionOfFunctionBrackets = functionNameFull->find(CHAR_CLOSE_BRACKET);
	bool functionHasArguments = false;
	if(startPositionOfFunctionBrackets != endPositionOfFunctionBrackets-1)
	{
		functionHasArguments = true;
	}
	if(functionHasArguments)
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		cout << "functionHasArguments: functionNameFull = " << *functionNameFull << endl;
		#endif
		
		int startPositionOfFunctionArguments = startPositionOfFunctionBrackets+1;
		int endPositionOfFunctionArguments = endPositionOfFunctionBrackets-1;
		string functionArgumentsRaw = functionNameFull->substr(startPositionOfFunctionArguments, endPositionOfFunctionArguments-startPositionOfFunctionArguments+1);

		bool stillParsingArguments = true;
		int startPositionOfArgument = 0;
		int endPositionOfArgument = findEndPositionOfArgument(&functionArgumentsRaw, 0);
		bool lastArgument = false;
		if(endPositionOfArgument == string::npos)
		{//only 1 argument in function
			endPositionOfArgument = endPositionOfFunctionArguments;
			lastArgument = true;
		}
		else
		{
			endPositionOfArgument = endPositionOfArgument-1;
		}
		
		while(stillParsingArguments)
		{
			if(lastArgument)
			{
				stillParsingArguments = false;
			}
					
			string currentArgument = functionArgumentsRaw.substr(startPositionOfArgument, endPositionOfArgument-startPositionOfArgument+1);
			
			int startPositionOfArgumentName = currentArgument.rfind(CHAR_SPACE) + 1;	//last space
			if(startPositionOfArgumentName == string::npos)
			{
				cout << "generateHTMLdocumentationFunctionInputArguments() error: (startPositionOfArgumentName == string::npos)" << endl;
				exit(0);
			}
			string currentArgumentName = currentArgument.substr(startPositionOfArgumentName, endPositionOfArgument-startPositionOfArgumentName+1);
			string currentArgumentType = currentArgument.substr(0, startPositionOfArgumentName);
			string currentArgumentDescription = createDescriptionFromCaseSensitiveMultiwordString(currentArgumentName);
			
			#ifdef CS_DEBUG_HTML_DOCUMENTATION
			/*
			if(*functionName == "writeSemanticNetXMLFileOptimised")
			{
				cout << "functionArgumentsRaw = " << functionArgumentsRaw << endl;
				cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
				cout << "endPositionOfArgument = " << endPositionOfArgument << endl;		      
				cout << "currentArgument = " << currentArgument << endl;
				cout << "startPositionOfArgumentName = " << startPositionOfArgumentName << endl;
				cout << "currentArgumentName = " << currentArgumentName << endl;
				cout << "currentArgumentType = " << currentArgumentType << endl;
				cout << "currentArgumentDescription = " << currentArgumentDescription << endl;				
				cout << "currentArgumentTypeLength = " << startPositionOfArgumentName << endl;
			}
			*/		
			#endif
			
			currentArgumentName = ensureHTMLTagSafe(currentArgumentName);
			currentArgumentType = ensureHTMLTagSafe(currentArgumentType);
			currentArgumentDescription = ensureHTMLTagSafe(currentArgumentDescription);
			
			string HTMLdocumentationFunctionInputArgumentsRow = "\t\t<tr><td>" + currentArgumentName + "</td><td>" + currentArgumentType + "</td><td>" + currentArgumentDescription + "</td></tr>\n";
			*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionInputArgumentsRow;
			
			startPositionOfArgument = endPositionOfArgument+2;
			endPositionOfArgument = findEndPositionOfArgument(&functionArgumentsRaw, startPositionOfArgument);
				
			if(endPositionOfArgument == string::npos)
			{
				lastArgument = true;
				endPositionOfArgument = endPositionOfFunctionArguments;
			}
			else
			{
				endPositionOfArgument = endPositionOfArgument-1;
			}			
		}
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		/*
		if(*functionName == "writeSemanticNetXMLFileOptimised")
		{
			exit(0);
		}
		*/		
		#endif
	}
	else
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		cout << "!functionHasArguments: functionNameFull = " << *functionNameFull << endl;
		#endif	
	}
	
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t</table>\n";
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "HTMLdocumentationFunctionInputArguments: " << *HTMLdocumentationFunctionInputArguments << endl;
	#endif
}

#define H '<'
#define CHAR_LESS_THAN '<'
#define CHAR_GREATER_THAN '>'
#define CLASS_TYPE_OPEN_TAG (CHAR_LESS_THAN)
#define CLASS_TYPE_CLOSE_TAG (CHAR_GREATER_THAN)
#define HTML_TAG_OPEN "&lt;"
#define HTML_TAG_CLOSE "&gt;"
string ensureHTMLTagSafe(string str)
{
	string htmlSafeString = "";
	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == CLASS_TYPE_OPEN_TAG)
		{
			htmlSafeString = htmlSafeString + HTML_TAG_OPEN;
		}
		else if(str[i] == CLASS_TYPE_CLOSE_TAG)
		{
			htmlSafeString = htmlSafeString + HTML_TAG_CLOSE;
		}
		else
		{
			htmlSafeString = htmlSafeString + str[i];
		}
	}
	return htmlSafeString;
}

/*
createDescriptionFromCaseSensitiveMultiwordString();
split file/function names (eg thisThing) into pseudo file/function descriptions (eg this thing)
	add a space after l of instance ..lU...
	add a space before l of instance ..UUl...
*/
string createDescriptionFromCaseSensitiveMultiwordString(string str)
{
	string description = "";
	for(int i=0; i<str.length(); i++)
	{
		if(i >= 2)
		{
			string alreadyExtractedDescription = description.substr(0, i-2);
			char currentChar1 = str[i];
			char currentChar2 = str[i-1];
			char currentChar3 = str[i-2];
			bool currentChar1IsUpper = bool(isupper(currentChar1));
			bool currentChar2IsUpper = bool(isupper(currentChar2));
			bool currentChar3IsUpper = bool(isupper(currentChar3));
			if(currentChar1IsUpper && !currentChar2IsUpper)
			{
				description = description + CHAR_SPACE;
			}
			else if(!currentChar1IsUpper && currentChar2IsUpper && currentChar3IsUpper)
			{
				description = description + CHAR_SPACE;
			}
		}
		description = description + str[i];
		
	}
	return description;	
}


//limitation; only supports a single level of class type assignment (ie, multiple <> tags not supported in a given variable type, eg <><>)
int findEndPositionOfArgument(string * functionArgumentsRaw, int startPositionOfArgument)
{
	//cout << "functionArgumentsRaw = " << *functionArgumentsRaw << endl;
	//cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
	
	int startPositionOfArgumentTemp = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	int endPositionOfArgument = -1;
	while(stillFindingEndPositionOfArgument)
	{
		endPositionOfArgument = functionArgumentsRaw->find(CHAR_COMMA, startPositionOfArgumentTemp);	//find next comma
		//cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
		
		if(endPositionOfArgument == string::npos)
		{
			stillFindingEndPositionOfArgument = false;
		}
		else
		{		
			int nextPositionOfClassTypeOpenTag = functionArgumentsRaw->find(CLASS_TYPE_OPEN_TAG, startPositionOfArgumentTemp);	//find next comma
			int nextPositionOfClassTypeCloseTag = functionArgumentsRaw->find(CLASS_TYPE_CLOSE_TAG, startPositionOfArgumentTemp);	//find next comma
			if(nextPositionOfClassTypeOpenTag == string::npos)
			{
				stillFindingEndPositionOfArgument = false;
			}
			else
			{
				if(nextPositionOfClassTypeOpenTag < endPositionOfArgument)
				{
					bool foundCloseTag = false;
					
					if(nextPositionOfClassTypeCloseTag != string::npos)
					{
						if(nextPositionOfClassTypeCloseTag < endPositionOfArgument)
						{					
							foundCloseTag = true;
						}
					}	
					if(!foundCloseTag)
					{				
						startPositionOfArgumentTemp = endPositionOfArgument+1;
						//cout << "startPositionOfArgumentTemp = " << startPositionOfArgumentTemp << endl;
					}
					else
					{
						stillFindingEndPositionOfArgument = false;
					}
				}
				else
				{
					stillFindingEndPositionOfArgument = false;
				}
			}
		}		
	}
	return endPositionOfArgument;
}

string generateHTMLdocumentationFunctionTraceImagePlaceHolder(string * traceImageFileName)
{
	string HTMLdocumentationFunctionTraceImagePlaceHolder = "\t<br /><b>Trace Diagram (magenta)</b><br /><img width=\"1300px\" src=\"" + *traceImageFileName  + "\">\n";
	return HTMLdocumentationFunctionTraceImagePlaceHolder;
}


