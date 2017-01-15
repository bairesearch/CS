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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3d5b 27-July-2014
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
#include <map>		//required for use with generateHTMLdocumentationFunctionReferenceList
#include <utility>
using namespace std;

#ifndef LINUX
	#include <windows.h>
#endif




void printCS(string topLevelFileName, int width, int height, string outputLDRfileName, string outputSVGfileName, string outputPPMfileName, string outputHTMLfileName, bool useOutputLDRfile, bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, bool outputFileConnections, string topLevelFunctionName)
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

	char * outputFileNameLDRcharstar = const_cast<char*>(outputLDRfileName.c_str());
	char * displayFileNamePPMcharstar = const_cast<char*>(outputPPMfileName.c_str());
	char * outputFileNameSVGcharstar = const_cast<char*>(outputSVGfileName.c_str());

	//1. for every f
		//2. for every include f listed
			//3. add to ref list
	//for every reference
		//for every reference, and merge all

	CSfileReference * topLevelReferenceInList = new CSfileReference();
	topLevelReferenceInList->name = topLevelFileName;
	CSfileReference * firstReferenceInTopLevelBelowList = new CSfileReference();
	topLevelReferenceInList->firstReferenceInBelowList = firstReferenceInTopLevelBelowList;

	//cout << "start getIncludeFileNamesFromCorHfile()" << endl;
	bool hFileFound = getIncludeFileNamesFromCorHfile(firstReferenceInTopLevelBelowList, topLevelReferenceInList, topLevelFileName, 0);
	//cout << "finish getIncludeFileNamesFromCorHfile()" << endl;
	if(!hFileFound)
	{
		cout << "printCS() error: !hFileFound: " << topLevelFileName << endl;
	}

	initiateMaxXatParticularY();
	Reference * firstReferenceInPrintList = new Reference();

	setCurrentDirectory(tempFolderCharStar);

	XMLparserTag * firstTagInSVGFile = new XMLparserTag();
	XMLparserTag * currentTagInSVGFile = firstTagInSVGFile;


	#ifdef CS_SUPPORT_PREDEFINED_GRID
	bool usePredefinedGrid = false;
	bool tempResult = true;
	XMLparserTag * firstTagInRulesTag = parseTagDownALevel(CSfirstTagInXMLfile, RULES_XML_TAG_rules, &tempResult);
	XMLparserTag * firstTagInGridTag = NULL;
	if(tempResult)
	{
		XMLparserTag * currentTag = firstTagInRulesTag;
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

	Reference * currentReferenceInPrintList = createFileReferenceListBoxes(firstReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &currentTagInSVGFile, outputFunctionsConnectivity, traceFunctionUpwards, firstTagInGridTag, usePredefinedGrid);
	if(outputFileConnections)
	{
		currentReferenceInPrintList = createFileReferenceListConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &currentTagInSVGFile, traceFunctionUpwards);
	}
	if(outputFunctionsConnectivity)
	{
		CSfunctionReference * currentReferenceInTopLevelBelowList = firstReferenceInTopLevelBelowList->firstReferenceInFunctionList;
		bool topLevelFunctionNameFound = false;
		CSfunctionReference * topLevelFunctionReference = NULL;
		while(currentReferenceInTopLevelBelowList->next != NULL)
		{
			//cout << "currentReferenceInTopLevelBelowList->name = " << currentReferenceInTopLevelBelowList->name << endl;
			if(currentReferenceInTopLevelBelowList->name == topLevelFunctionName)
			{
				topLevelFunctionReference = currentReferenceInTopLevelBelowList;
				topLevelFunctionNameFound = true;
			}
			currentReferenceInTopLevelBelowList = currentReferenceInTopLevelBelowList->next;
		}
		if(topLevelFunctionNameFound)
		{
			//CSfunctionReference * topLevelFunctionReference = firstReferenceInTopLevelBelowList->firstReferenceInFunctionList;
			topLevelFunctionReference->printX = firstReferenceInTopLevelBelowList->printX;
			topLevelFunctionReference->printY = firstReferenceInTopLevelBelowList->printY;
			topLevelFunctionReference->col = firstReferenceInTopLevelBelowList->col;

			firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] = firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] + 1;
			topLevelFunctionReference->printed = true;
			if(!(firstReferenceInTopLevelBelowList->printed))
			{
				cout << "error" << endl;
				exit(0);
			}

			CSfunctionReference * currentReferenceInFunctionReferenceList = topLevelFunctionReference->firstReferenceInFunctionReferenceList;
			while(currentReferenceInFunctionReferenceList->next != NULL)
			{
				currentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionReference, firstReferenceInTopLevelBelowList, 0, currentReferenceInFunctionReferenceList->name, &currentTagInSVGFile, traceFunctionUpwards, false, NULL, usePredefinedGrid);
				currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
			}
			resetPrintedFunctionConnections(firstReferenceInTopLevelBelowList, topLevelFunctionReference, false, NULL);

			if(traceFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
			{
				bool foundBottomLevelFunctionRef = false;
				string fileNameHoldingFunction = "";
				CSfunctionReference * bottomLevelFunctionToTraceUpwards = findPrintedFunctionReferenceWithName(bottomLevelFunctionNameToTraceUpwards, NULL, firstReferenceInTopLevelBelowList, &foundBottomLevelFunctionRef, &fileNameHoldingFunction);
				if(foundBottomLevelFunctionRef)
				{
					if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
					{
						if(!useOutputHTMLfile)
						{//still use single html file for function, but automatically generate html file name (based on function name)
							useOutputHTMLfile = true;
							outputHTMLfileName = bottomLevelFunctionNameToTraceUpwards + HTML_EXTENSION;	//automatically generate html file name (based on function name)
						}
					}
					string HTMLdocumentationFunctionNOTUSED = "";
					generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, fileNameHoldingFunction, &currentTagInSVGFile, generateHTMLdocumentationMode, &HTMLdocumentationFunctionNOTUSED, &outputSVGfileName, useOutputHTMLfile, outputHTMLfileName, traceFunctionUpwards);
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

					//find last tag in svg file;
					XMLparserTag * lastTagInSVGFile = firstTagInSVGFile;
					XMLparserTag * tempTagInSVGFile = firstTagInSVGFile;
					while(tempTagInSVGFile->nextTag != NULL)
					{
						lastTagInSVGFile = tempTagInSVGFile;
						tempTagInSVGFile = tempTagInSVGFile->nextTag;
					}

					string HTMLdocumentationBody = "";
					Reference * firstReferenceInPrintListFunction = new Reference();
					generateHTMLdocumentationForAllFunctions(firstReferenceInTopLevelBelowList, firstReferenceInPrintListFunction, firstReferenceInTopLevelBelowList, generateHTMLdocumentationMode, useOutputHTMLfile, &HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceFunctionUpwards, usePredefinedGrid);
					delete firstReferenceInPrintListFunction;	//should delete this, as it will contain far too many LD vector graphics references (ie a set of traced references for each function)
					htmlDocumentationGenerationPreventsDisplay = true;	//cannot display in OpenGL/save to file, as LD vector graphics references have been deleted

					if(useOutputHTMLfile)
					{//use single html file for project (rather than unique html file per file in project)
						string HTMLdocumentationHeader = generateHTMLdocumentationHeader("Software Project", true, false);
						string HTMLdocumentationFooter = generateHTMLdocumentationFooter(true);
						string HTMLdocumentation = HTMLdocumentationHeader + HTMLdocumentationBody + HTMLdocumentationFooter;
						ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
						writeStringToFileObject(&HTMLdocumentation, &writeFileObjectHTML);
					}
				}
			}
		}
		else
		{
			cout << "error: !topLevelFunctionNameFound" << endl;
			exit(0);
		}
	}

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
			char * topLevelSceneFileName = outputFileNameLDRcharstar;
			char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
			Reference * initialReferenceInSceneFile = new Reference();
			Reference * topLevelReferenceInSceneFile = new Reference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				exit(0);
			}
			write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);


			unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

			//setViewPort2Dortho(-100.0, 2000.0, -100.0, 2000.0);
			setViewPort3Dortho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

			//now reparse file
			Reference * initialReferenceInCollapsedSceneFile = new Reference();
			Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
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

}

string generateHTMLdocumentationHeader(string name, bool htmlHeader, bool isFile)
{
	string HTMLdocumentationHeader = "";
	if(htmlHeader)
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<html><head><title>" + name + " Documentation</title><style type=\"text/css\">TD { font-size:75%; } </style></head><body><h3>" + name + " Documentation</h3><p>Automatically generated with Code Structure Viewer (OpenCS), Project Version: 3d5b 27-July-2014<p>\n";
	}
	else
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<h3>" + name + " Documentation</h3>\n";
	}
	if(isFile)
	{
		int indexOfFileExtension = name.find(".");
		string fileNameWithoutExtension = name.substr(0, indexOfFileExtension);
		string HTMLdocumentationFileDescription = createDescriptionFromCaseSensitiveMultiwordString(fileNameWithoutExtension);
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p><b>File description:</b> " + HTMLdocumentationFileDescription + "</p>";
	}
	return HTMLdocumentationHeader;
}

string generateHTMLdocumentationFooter(bool htmlFileFooter)
{
	string HTMLdocumentationFooter = "";
	if(htmlFileFooter)
	{
		HTMLdocumentationFooter = "</body></html>\n";
	}
	else
	{
		HTMLdocumentationFooter = "\n";
	}
	return HTMLdocumentationFooter;
}

void writeStringToFileObject(string * s, ofstream * writeFileObject)
{
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject->put((*s)[i]);
	}
}

void generateHTMLdocumentationForAllFunctions(CSfileReference * firstReferenceInAboveLevelBelowList, Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, int generateHTMLdocumentationMode, bool useOutputHTMLfile, string * HTMLdocumentationBody, XMLparserTag * firstTagInSVGFile, XMLparserTag * lastTagInSVGFile, bool traceFunctionUpwards, bool usePredefinedGrid)
{
	bool result = true;

	CSfileReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{
		string fileNameHoldingFunction = currentFileReference->name;
		string outputHTMLfileName = currentFileReference->name + HTML_EXTENSION;

		ifstream tempFileStream(outputHTMLfileName.c_str());

		bool passed = false;
		if(!useOutputHTMLfile)
		{
			if(!(tempFileStream.good()))
			{//file does not exist
				passed = true;
			}
		}
		else
		{
			passed = true;
		}
		if(passed)
		{

			if(!(currentFileReference->HTMLgenerated))	//this constraint doesnt work as planned
			{//do not parse a file twice
				currentFileReference->HTMLgenerated = true;

				if(!useOutputHTMLfile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					*HTMLdocumentationBody = "";
				}
				string HTMLdocumentationFileBody = "";
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
				string HTMLdocumentationFileFunctionList = "<p>\n<b>File function List</b>\n<ul><li>";
				int previousIndentation = 0;
				bool previousIndentationFirst = true;
				#endif
				bool HTMLgeneratedSafe = false;

				CSfunctionReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
				while(currentFunctionReference->next != NULL)
				{
					if(currentFunctionReference->printed)
					{//only create connections to printed boxes

						if(!(currentFunctionReference->HTMLgenerated))
						{//do not parse a function twice
							HTMLgeneratedSafe = true;
							currentFunctionReference->HTMLgenerated = true;

							string outputSVGfileNameFunction = "";	//only used with traceFunctionUpwards
							XMLparserTag * firstTagInSVGFileFunction = new XMLparserTag();	//only used with traceFunctionUpwards
							XMLparserTag * currentTagInSVGFileFunction = firstTagInSVGFileFunction;
							if(traceFunctionUpwards)
							{
								outputSVGfileNameFunction = currentFunctionReference->name + SVG_EXTENSION;
							}

							string HTMLdocumentationFunction = "";
							generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, currentFunctionReference, fileNameHoldingFunction, &currentTagInSVGFileFunction, generateHTMLdocumentationMode, &HTMLdocumentationFunction, &outputSVGfileNameFunction, false, "", traceFunctionUpwards);
							HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationFunction;
							#ifdef CS_DEBUG_HTML_DOCUMENTATION
							//cout << "HTMLdocumentationFunction = " << HTMLdocumentationFunction << endl;
							#endif

							if(traceFunctionUpwards)
							{
								lastTagInSVGFile->nextTag = firstTagInSVGFileFunction;
								if(!writeSVGfile(outputSVGfileNameFunction, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
								{
									result = false;
								}
								lastTagInSVGFile->nextTag = NULL;
								delete firstTagInSVGFileFunction;
							}

							#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
							addToHTMLdocumentationFileFunctionList(currentFunctionReference, &HTMLdocumentationFileFunctionList, &previousIndentation, &previousIndentationFirst);
							#endif
						}
					}

					currentFunctionReference = currentFunctionReference->next;
				}

				string HTMLdocumentationFileIntroduction = "";
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
				for(int i=0; i<previousIndentation; i++)
				{
					HTMLdocumentationFileFunctionList = HTMLdocumentationFileFunctionList + "</li></ul>";
				}
				HTMLdocumentationFileFunctionList = HTMLdocumentationFileFunctionList + "</li></ul>\n</p>";
				HTMLdocumentationFileIntroduction = HTMLdocumentationFileIntroduction + HTMLdocumentationFileFunctionList;
				#endif
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
				string outputSVGFileNameFile = currentFileReference->name + SVG_EXTENSION;
				generateFileDiagramFunctionsHeirachy(currentFileReference, outputSVGFileNameFile, firstReferenceInTopLevelBelowList, usePredefinedGrid);
				string HTMLdocumentationFileImagePlaceHolder = generateHTMLdocumentationImagePlaceHolder(&outputSVGFileNameFile, "File Diagram (functions heirachy)");
				HTMLdocumentationFileIntroduction = HTMLdocumentationFileIntroduction + HTMLdocumentationFileImagePlaceHolder;
				#endif
				HTMLdocumentationFileBody = HTMLdocumentationFileIntroduction + HTMLdocumentationFileBody;

				string HTMLdocumentationFileHeader = generateHTMLdocumentationHeader(currentFileReference->name, !useOutputHTMLfile, true);
				string HTMLdocumentationFileFooter = generateHTMLdocumentationFooter(!useOutputHTMLfile);
				string HTMLdocumentationFile = "";
				HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;


				if(!useOutputHTMLfile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					#ifdef CS_DEBUG_HTML_DOCUMENTATION
					//cout << "HTMLdocumentationFile = " << HTMLdocumentationFile << endl;
					#endif
					string outputHTMLfileName = currentFileReference->name + HTML_EXTENSION;
					ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
					writeStringToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);
				}
				else
				{
					if(HTMLgeneratedSafe)
					{
						*HTMLdocumentationBody = *HTMLdocumentationBody + HTMLdocumentationFile;
					}
				}

				if(currentFileReference->firstReferenceInBelowList != NULL)
				{
					generateHTMLdocumentationForAllFunctions(currentFileReference->firstReferenceInBelowList, currentReferenceInPrintList, firstReferenceInTopLevelBelowList, generateHTMLdocumentationMode, useOutputHTMLfile, HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceFunctionUpwards, usePredefinedGrid);
				}
			}
		}

		currentFileReference = currentFileReference->next;
	}
}

void generateHTMLdocumentationForFunction(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * bottomLevelFunctionToTraceUpwards, string fileNameHoldingFunction, XMLparserTag ** currentTag, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionBody, string * outputSVGfileNameFunction, bool useOutputHTMLfile, string outputHTMLfileName, bool traceFunctionUpwards)
{
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
	{
		#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
		cout << "\nStart Trace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << bottomLevelFunctionToTraceUpwards->name << "()" << endl;
		cout << bottomLevelFunctionToTraceUpwards->nameFull << endl;
		#endif
	}

	string HTMLdocumentationFunctionTraceTableRows = "";
	if(traceFunctionUpwards)
	{
		currentReferenceInPrintList = traceFunctionsUpwardsAndDrawOrHighLightThese(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, currentTag, generateHTMLdocumentationMode, &HTMLdocumentationFunctionTraceTableRows);

		traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards);	//this is required for multiple traces per CS execution
	}

	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
	{
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		//cout << "bottomLevelFunctionToTraceUpwards->name = " << bottomLevelFunctionToTraceUpwards->name << endl;
		//cout << "bottomLevelFunctionToTraceUpwards->nameFull = " << bottomLevelFunctionToTraceUpwards->nameFull << endl;
		#endif

		string HTMLdocumentationFunctionTitle = "";
		HTMLdocumentationFunctionTitle = HTMLdocumentationFunctionTitle + "<h4>" + (bottomLevelFunctionToTraceUpwards->name) + "()</h4>";
		string HTMLdocumentationFunctionSummary = "";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_SUMMARY
		generateHTMLdocumentationFunctionSummary(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionSummary);
		#endif

		string HTMLdocumentationFunctionInputArguments = "";
		generateHTMLdocumentationFunctionInputArguments(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionInputArguments);

		string HTMLdocumentationFunctionReferenceList = "";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
		generateHTMLdocumentationFunctionReferenceList(bottomLevelFunctionToTraceUpwards, &HTMLdocumentationFunctionReferenceList);
		#endif

		*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody + HTMLdocumentationFunctionTitle + HTMLdocumentationFunctionSummary + HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionReferenceList + "<br />";

		if(traceFunctionUpwards)
		{
			string HTMLdocumentationFunctionTraceTableHeader = "";
			string HTMLdocumentationFunctionTraceTableFooter = "";
			HTMLdocumentationFunctionTraceTableHeader = HTMLdocumentationFunctionTraceTableHeader + "\t<b>Function Trace</b><br /><table border=\"1\"><tr><th>" + "location" + "</th><th>" + "current function being traced" + "</th></tr>\n";
			HTMLdocumentationFunctionTraceTableFooter = HTMLdocumentationFunctionTraceTableFooter + "\t</table>";
			string HTMLdocumentationFunctionTraceTable = HTMLdocumentationFunctionTraceTableHeader + HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableFooter;

			string HTMLdocumentationFunctionTraceImagePlaceHolder = generateHTMLdocumentationImagePlaceHolder(outputSVGfileNameFunction, "Trace Diagram (magenta)");

			*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody +  HTMLdocumentationFunctionTraceTable + HTMLdocumentationFunctionTraceImagePlaceHolder;
		}

		if(useOutputHTMLfile)
		{//use single html file for function
			string HTMLdocumentationFunctionHeader = generateHTMLdocumentationHeader(bottomLevelFunctionToTraceUpwards->name, true, false);
			string HTMLdocumentationFunctionFooter = generateHTMLdocumentationFooter(true);
			string HTMLdocumentationFunction = HTMLdocumentationFunctionHeader + *HTMLdocumentationFunctionBody + HTMLdocumentationFunctionFooter;

			ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
			writeStringToFileObject(&HTMLdocumentationFunction, &writeFileObjectHTML);
		}
	}


}

void generateHTMLdocumentationFunctionSummary(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionSummary)
{
	*HTMLdocumentationFunctionSummary = "";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t<p><b>Function Summary</b><br /><table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "return type" + "</th><th>" + "description" + "</th></tr>\n";
	string HTMLdocumentationFunctionDescription = createDescriptionFromCaseSensitiveMultiwordString(*functionName);
	int endPositionOfReturnType = functionNameFull->find(*functionName);
	int startPositionOfReturnType = 0;
	string HTMLdocumentationFunctionReturnType = functionNameFull->substr(startPositionOfReturnType, endPositionOfReturnType-startPositionOfReturnType);
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t\t<tr><td>" + *functionName + "</td><td>" + HTMLdocumentationFunctionReturnType + "</td><td>" + HTMLdocumentationFunctionDescription + "</td></tr>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</table>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</p>\n";
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "HTMLdocumentationFunctionSummary: " << *HTMLdocumentationFunctionSummary << endl;
	#endif
}

void generateHTMLdocumentationFunctionInputArguments(string * functionName, string * functionNameFull, string * HTMLdocumentationFunctionInputArguments)
{
	*HTMLdocumentationFunctionInputArguments = "";
	#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<b>Class Data</b><br />";
	#else
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<b>Function Arguments</b><br />";
	#endif
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "<table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th></tr>\n";

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
			if(*functionName == "...")
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
		if(*functionName == "...")
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
		bool removeLowerCase = false;
		if(i >= 2)
		{
			string alreadyExtractedDescription = description.substr(0, i-2);
			char currentChar1 = str[i];
			char currentChar2 = str[i-1];
			char currentChar3 = str[i-2];
			bool currentChar1IsUpper = !(bool(islower(currentChar1)));	//Changed 19 November 2012: consider numbers as upper case also (http://www.cplusplus.com/reference/clibrary/cctype/islower/)
			bool currentChar2IsUpper = !(bool(islower(currentChar2)));	//Changed 19 November 2012: consider numbers as upper case also (http://www.cplusplus.com/reference/clibrary/cctype/islower/)
			bool currentChar3IsUpper = !(bool(islower(currentChar3)));	//Changed 19 November 2012: consider numbers as upper case also (http://www.cplusplus.com/reference/clibrary/cctype/islower/)
			if(currentChar1IsUpper && !currentChar2IsUpper)
			{
				//aA
				description = description + CHAR_SPACE;
				if(i < str.length()-1)
				{
					char nextChar = str[i+1];
					if(islower(nextChar))
					{//aAa
						removeLowerCase = true;
					}
				}
			}
			else if(!currentChar1IsUpper && currentChar2IsUpper && currentChar3IsUpper)
			{
				//ABa
				description = description + CHAR_SPACE;
			}
		}
		char characterToAdd = str[i];
		if(removeLowerCase)
		{
			characterToAdd = tolower(characterToAdd); 	//Changed 19 November 2012: reduce all non abbreviations to lower case (ie all strings that do not wholy consist of uppercase/numerical characters)
		}
		description = description + characterToAdd;
	}
	return description;
}


//limitation; only supports a single level of class type assignment (ie, multiple <> tags not supported in a given variable type, eg <><>)
int findEndPositionOfArgument(string * functionArgumentsRaw, int startPositionOfArgument)
{
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	//cout << "functionArgumentsRaw = " << *functionArgumentsRaw << endl;
	//cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
	#endif

	int startPositionOfArgumentTemp = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	int endPositionOfArgument = -1;
	while(stillFindingEndPositionOfArgument)
	{
		endPositionOfArgument = functionArgumentsRaw->find(CHAR_COMMA, startPositionOfArgumentTemp);	//find next comma
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		//cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
		#endif

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
						#ifdef CS_DEBUG_HTML_DOCUMENTATION
						//cout << "startPositionOfArgumentTemp = " << startPositionOfArgumentTemp << endl;
						#endif
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

void generateHTMLdocumentationFunctionReferenceList(CSfunctionReference * function, string * HTMLdocumentationFunctionReferenceList)
{
	//generate list

	*HTMLdocumentationFunctionReferenceList = "";
	*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "\t<p><b>Function Reference List</b><br />\n";
	CSfunctionReference * currentReferenceInFunctionReferenceList = function->firstReferenceInFunctionReferenceList;
	string HTMLdocumentationFunctionReferenceListBody = "\t<ul>\n";
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
	HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "<li>";
	int previousIndentation = 0;
	bool previousIndentationFirst = true;
	#endif
	int referenceIndex=0;
	bool foundReferences = false;

	map<int, CSfunctionReference*> CSfunctionReferenceListOrdered;
	while(currentReferenceInFunctionReferenceList->next != NULL)
	{
		//check reference has not already been added (NB if the reference is the function name itself - it may be a comment and not actually indicate recursion)
		CSfunctionReference * currentReferenceInFunctionReferenceListTemp = function->firstReferenceInFunctionReferenceList;
		int referenceIndexTemp = 0;
		bool alreadyRecordedReference = false;
		while(currentReferenceInFunctionReferenceListTemp->next != NULL)
		{
			if((referenceIndexTemp < referenceIndex) && (currentReferenceInFunctionReferenceListTemp->name == currentReferenceInFunctionReferenceList->name))
			{
				alreadyRecordedReference = true;
			}

			currentReferenceInFunctionReferenceListTemp = currentReferenceInFunctionReferenceListTemp->next;
			referenceIndexTemp++;
		}

		if(!alreadyRecordedReference)
		{
			//now find all instances of the reference in the function text, and for each instance found add to CSfunctionReferenceListOrdered
			int startPosOfFunctionReferenceInFunction = 0;
			//cout << "function->functionText = " << function->functionText << endl;
			while(std::string::npos != (startPosOfFunctionReferenceInFunction = (function->functionText).find(currentReferenceInFunctionReferenceList->name + '(', startPosOfFunctionReferenceInFunction)))
			{
				//cout << "startPosOfFunctionReferenceInFunction = " << startPosOfFunctionReferenceInFunction << endl;
				int functionCharacterIndexOfReference = startPosOfFunctionReferenceInFunction;
				CSfunctionReferenceListOrdered.insert(pair<int, CSfunctionReference*>(functionCharacterIndexOfReference, currentReferenceInFunctionReferenceList));
				startPosOfFunctionReferenceInFunction++;
			}
		}
		foundReferences = true;

		currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
		referenceIndex++;

	}

	for(map<int, CSfunctionReference*>::iterator CSfunctionReferenceListOrderedIter = CSfunctionReferenceListOrdered.begin(); CSfunctionReferenceListOrderedIter != CSfunctionReferenceListOrdered.end(); CSfunctionReferenceListOrderedIter++)
	{
		CSfunctionReference * currentReferenceInFunctionReferenceList = CSfunctionReferenceListOrderedIter->second;

	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
		addToHTMLdocumentationFileFunctionList(currentReferenceInFunctionReferenceList, &HTMLdocumentationFunctionReferenceListBody, &previousIndentation, &previousIndentationFirst);
		//cout << "currentReferenceInFunctionReferenceList->name = " << currentReferenceInFunctionReferenceList->name << endl;
		//cout << "HTMLdocumentationFunctionReferenceList = " << *HTMLdocumentationFunctionReferenceList << endl;
	#else
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
		HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "\t\t<li>";
		for(int i=0; i<currentReferenceInFunctionReferenceList->functionReferenceIndentation; i++)
		{
			HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "&#160&#160&#160&#160&#160";
		}
		HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + currentReferenceInFunctionReferenceList->name + "</li>\n";
		#else
		HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "\t\t<li>" + currentReferenceInFunctionReferenceList->name + "</li>\n";
		#endif
	#endif
	}


	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
	for(int i=0; i<previousIndentation; i++)
	{
		HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "</li></ul>";
	}
	HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "</li>\n";
	#endif
	HTMLdocumentationFunctionReferenceListBody = HTMLdocumentationFunctionReferenceListBody + "\t</ul>";

	if(foundReferences)
	{
		*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + HTMLdocumentationFunctionReferenceListBody;
	}
	else
	{
		*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "N/A";
	}
	*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "</p>\n";

	//exit(0);
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "HTMLdocumentationFunctionReferenceList: " << *HTMLdocumentationFunctionReferenceList << endl;
	#endif
}

string generateHTMLdocumentationImagePlaceHolder(string * traceImageFileName, string imageTitle)
{
	string HTMLdocumentationFunctionTraceImagePlaceHolder = "\t<br /><b>" + imageTitle + "</b><br /><img width=\"1300px\" src=\"" + *traceImageFileName  + "\">\n";
	return HTMLdocumentationFunctionTraceImagePlaceHolder;
}



void addToHTMLdocumentationIndentedList(string currentFunctionReferenceName, int currentFunctionReferenceIndentation, string * HTMLdocumentationFileFunctionList, int * previousIndentation, bool * previousIndentationFirst)
{
	string functionNameCompact = currentFunctionReferenceName;	// + "()";

	int differenceBetweenPreviousIndentation = currentFunctionReferenceIndentation - *previousIndentation;
	string rawIndentationText = "\n";
	for(int i=0; i<currentFunctionReferenceIndentation; i++)
	{
		rawIndentationText = rawIndentationText + "\t";
	}
	if(differenceBetweenPreviousIndentation > 0)
	{
		for(int i=0; i<differenceBetweenPreviousIndentation; i++)
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + rawIndentationText + "<ul><li>";
		}
		*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + functionNameCompact;
	}
	else if(differenceBetweenPreviousIndentation < 0)
	{
		for(int i=0; i<abs(differenceBetweenPreviousIndentation); i++)
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + "</li></ul>";
		}
		*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + "</li>" + rawIndentationText + "<li>" + functionNameCompact;
	}
	else
	{
		if(*previousIndentationFirst)
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + functionNameCompact;

		}
		else
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + "</li>" + rawIndentationText + "<li>" + functionNameCompact;
		}
	}
	*previousIndentationFirst = false;
	*previousIndentation = currentFunctionReferenceIndentation;
}

void addToHTMLdocumentationFileFunctionList(CSfunctionReference * currentFunctionReference, string * HTMLdocumentationFileFunctionList, int * previousIndentation, bool * previousIndentationFirst)
{
	addToHTMLdocumentationIndentedList(currentFunctionReference->name, currentFunctionReference->functionReferenceIndentation, HTMLdocumentationFileFunctionList, previousIndentation, previousIndentationFirst);
}




void generateFileDiagramFunctionsHeirachy(CSfileReference * currentFileReference, string outputSVGFileNameFile, CSfileReference * firstReferenceInTopLevelBelowList, bool usePredefinedGrid)
{
	bool result = true;

	ifstream tempFileStream(outputSVGFileNameFile.c_str());
	if(!(tempFileStream.good()))
	{//file does not exist

		initiateMaxXatParticularY();
		Reference * firstReferenceInPrintList = new Reference();
		Reference * currentReferenceInPrintList = firstReferenceInPrintList;

		setCurrentDirectory(tempFolderCharStar);

		XMLparserTag * firstTagInSVGFile = new XMLparserTag();
		XMLparserTag * currentTagInSVGFile = firstTagInSVGFile;

		for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
		{
			currentFileReference->maxFunctionPrintXAtAParticularY[0] = 1;
		}

		CSfunctionReference * currentTopLevelFunctionReference = currentFileReference->firstReferenceInFunctionList;
		while(currentTopLevelFunctionReference->next != NULL)
		{
			if(currentTopLevelFunctionReference->printed)
			{
				cout << currentTopLevelFunctionReference->name << endl;
				//cout << "currentTopLevelFunctionReference->name = " << currentTopLevelFunctionReference->name << endl;
					
				//print function box for top level functions (added 3d3b);
				//cout << "currentTopLevelFunctionReference->level = " << currentTopLevelFunctionReference->level << endl;
				if(currentTopLevelFunctionReference->printYIndex == 0)
				{
					//correction applied with useSingleFileOnly
					currentTopLevelFunctionReference->printX = currentTopLevelFunctionReference->printX - currentFileReference->printX;
					currentTopLevelFunctionReference->printY = currentTopLevelFunctionReference->printY - currentFileReference->printY;

					//cout << "currentTopLevelFunctionReference->printYIndex = " << currentTopLevelFunctionReference->printYIndex << endl;
					currentReferenceInPrintList = printFunctionBox(currentReferenceInPrintList, &currentTagInSVGFile, currentTopLevelFunctionReference, currentTopLevelFunctionReference->col);
					currentReferenceInPrintList = printFunctionBoxText(currentReferenceInPrintList, &currentTagInSVGFile, currentTopLevelFunctionReference, currentTopLevelFunctionReference->col, true);

					currentTopLevelFunctionReference->printX = currentTopLevelFunctionReference->printX + currentFileReference->printX;
					currentTopLevelFunctionReference->printY = currentTopLevelFunctionReference->printY + currentFileReference->printY;
				}

				CSfunctionReference * currentReferenceInFunctionReferenceList = currentTopLevelFunctionReference->firstReferenceInFunctionReferenceList;
				currentTopLevelFunctionReference->printedFunctionConnections = false;
				while(currentReferenceInFunctionReferenceList->next != NULL)
				{
					//cout << "currentReferenceInFunctionReferenceList->name = " << currentReferenceInFunctionReferenceList->name << endl;
					currentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(currentReferenceInPrintList, currentFileReference, currentTopLevelFunctionReference, firstReferenceInTopLevelBelowList, 0, currentReferenceInFunctionReferenceList->name, &currentTagInSVGFile, false, true, &(currentFileReference->name), usePredefinedGrid);

					currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
				}
				resetPrintedFunctionConnections(currentFileReference, currentTopLevelFunctionReference, true, &(currentFileReference->name));
			}

			currentTopLevelFunctionReference = currentTopLevelFunctionReference->next;
		}

		char * outputFileNameSVGcharstar = const_cast<char*>(outputSVGFileNameFile.c_str());
		if(!writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINX, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXX, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINY, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXY))
		{
			result = false;
		}
	}
}

#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT

void generateClassHTMLdocumentationFromCustomCSclassFormat()
{
	bool result = true;
	for(int i=0; i<NUMBER_OF_CSCLASSES; i++)
	{
		string CSclassFileName = CSclassesArray[i];
		cout << "CSclassFileName = " << CSclassFileName << endl;

		ifstream CSclassFileObject(CSclassFileName.c_str());

		if(!CSclassFileObject.rdbuf( )->is_open( ))
		{
			//xml file does not exist in current directory.
			cout << "Error: CSclass File does not exist in current directory: " << CSclassFileName << endl;
			result = false;
		}
		else
		{
			string HTMLdocumentationFileBody = "";

			string currentClassBeingParsed = "";
			bool finishedParsingObject = false;
			char currentToken;
			while((currentToken = (&CSclassFileObject)->get()) != EOF)
			{
				if(currentToken == CHAR_NEWLINE)
				{
					string className = getFunctionNameFromFunctionNameFull(&currentClassBeingParsed);

					string HTMLdocumentationClassDescription = createDescriptionFromCaseSensitiveMultiwordString(className);

					string HTMLdocumentationClassParameters = "";
					string functionNameNOTUSED = "";
					generateHTMLdocumentationFunctionInputArguments(&functionNameNOTUSED, &currentClassBeingParsed, &HTMLdocumentationClassParameters);
					currentClassBeingParsed = "";

					string HTMLdocumentationClassTitle = "";
					HTMLdocumentationClassTitle = HTMLdocumentationClassTitle + "<h4>Class " + className + "</h4>\n";
					string HTMLdocumentationClassHeader = "";
					HTMLdocumentationClassHeader = HTMLdocumentationClassHeader + "<p><b>Class description:</b> " + HTMLdocumentationClassDescription + "</p>\n";

					HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationClassTitle + HTMLdocumentationClassHeader + HTMLdocumentationClassParameters;

					//cout << "HTMLdocumentationFileBody = " << HTMLdocumentationFileBody << endl;
				}
				else
				{
					currentClassBeingParsed = currentClassBeingParsed + currentToken;
				}
			}

			CSclassFileObject.close();

			string HTMLdocumentationFileHeader = generateHTMLdocumentationHeader(CSclassFileName, true, false);
			string HTMLdocumentationFileFooter = generateHTMLdocumentationFooter(true);
			string HTMLdocumentationFile = "";
			HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;

			string outputHTMLfileName = CSclassFileName + HTML_EXTENSION;
			ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
			writeStringToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);


		}
	}
}

string getFunctionNameFromFunctionNameFull(string * functionNameFull)
{
	//cout << "functionNameFull = " << *functionNameFull << endl;

	int startPositionOfFunctionBrackets = functionNameFull->find(CHAR_OPEN_BRACKET);
	string functionTypeAndName = functionNameFull->substr(0, startPositionOfFunctionBrackets);
	//cout << "functionTypeAndName = " << functionTypeAndName << endl;

	int startPositionOfFunctionName = functionTypeAndName.rfind(CHAR_SPACE) + 1;
	string functionName = functionTypeAndName.substr(startPositionOfFunctionName, functionTypeAndName.length()-startPositionOfFunctionName);
	#ifdef CS_SUPPORT_FUNCTION_RETURN_POINTERS
	if(functionName[0] == '*')
	{
		int functionNameLength = functionName.length();
		functionName = functionName.substr(1, functionNameLength-1);
	}
	#endif
	//cout << "functionName = " << functionName << endl;

	return functionName;
}

#endif






#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
void convertIndentedListToHTMLlist()
{
	string indentedListFileName = "requirementsLayout.txt";
	string htmlListFileName = string("requirementsLayout") + HTML_EXTENSION;

	vector<string> indentedListVector;
	if(readIndentedListFile(indentedListFileName, &indentedListVector))
	{
		string HTMLdocumentationIndentedList = "";
		generateHTMLdocumentationIndentedList(&indentedListVector, &HTMLdocumentationIndentedList);

		ofstream writeFileObjectHTML(htmlListFileName.c_str());
		writeStringToFileObject(&HTMLdocumentationIndentedList, &writeFileObjectHTML);
	}
}

bool readIndentedListFile(string indentedListFileName, vector<string> * indentedListVector)
{
	bool result = true;

	ifstream parseFileObject(indentedListFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//txt file does not exist in current directory.
		cout << "Error: indented list File does not exist in current directory: " << indentedListFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		string currentEntry = "";
		int entryIndex = 0;
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				indentedListVector->push_back(currentEntry);
				//cout << "1. currentEntry = " << currentEntry << endl;
				currentEntry = "";
				entryIndex++;
			}
			else
			{
				currentEntry = currentEntry + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}

	return result;
}

//generic software to convert an indented list into an HTML list
void generateHTMLdocumentationIndentedList(vector<string> * indentedListVector, string * HTMLdocumentationIndentationList)
{
	//generate list
	*HTMLdocumentationIndentationList = "";
	*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + "\t<p><b>Generic Indented List</b><br />\n";
	string HTMLdocumentationIndentationListBody = "\t<ul>\n";
	HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "<li>";
	int previousIndentation = 0;
	bool previousIndentationFirst = true;
	int referenceIndex=0;
	bool foundReferences = false;

	for(vector<string>::iterator indentedListVectorIter = indentedListVector->begin(); indentedListVectorIter != indentedListVector->end(); indentedListVectorIter++)
	{
		foundReferences = true;

		string currentEntry = *indentedListVectorIter;
		int currentEntryLengthOld = currentEntry.length();
		int indentationLevel=0;
		while(currentEntry[indentationLevel] == CHAR_TAB)
		{
			indentationLevel++;
		}
		currentEntry = currentEntry.substr(indentationLevel, currentEntryLengthOld-indentationLevel);
		//cout << "2. currentEntry = " << currentEntry << endl;
		//cout << "indentationLevel = " << indentationLevel << endl;

		addToHTMLdocumentationIndentedList(currentEntry, indentationLevel, &HTMLdocumentationIndentationListBody, &previousIndentation, &previousIndentationFirst);
		//cout << "HTMLdocumentationIndentationListBody = " << HTMLdocumentationIndentationListBody << endl;

	}

	for(int i=0; i<previousIndentation; i++)
	{
		HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "</li></ul>";
	}
	HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "</li>\n";
	HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "\t</ul>";

	if(foundReferences)
	{
		*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + HTMLdocumentationIndentationListBody;
	}
	else
	{
		*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + "N/A";
	}
	*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + "</p>\n";

	//cout << "HTMLdocumentationIndentationList: " << *HTMLdocumentationIndentationList << endl;
}
#endif
