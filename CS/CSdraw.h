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
 * File Name: CSdraw.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h10a 09-December-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_DRAW
#define HEADER_CS_DRAW

#include "LDreferenceClass.h"
#include "XMLparserClass.h"
#include "CSreferenceContainerClass.h"
#include "CSglobalDefs.h"

/*
#define CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS (0.6)
#define CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX (0.5)
#define CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS (0.4)
#define CS_OUTPUT_Z_POSITION_FILE_BOX (0.35)
#define CS_OUTPUT_Z_POSITION_FUNCTION_BOX (0.3)
#define CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX (0.25)
#define CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT (0.2)

#define CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X (0.4)
#define CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR (0.05)
#define CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG (0.03)
#define CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B (0.067)
#define CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG (0.8)
#define CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG (0.1)

#define CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR (500)
#define CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR (200)
#define CS_FILE_MAX_TEXT_LENGTH (20)
#define CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH (1.25)
#define CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION (0.9)
#define CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION (0.3)
#define CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION (0.7)
#define CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION (1.0)
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X (5.0)
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B (0.65)
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B (0.9)
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C (0.4)
#define CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG (0.3)

#define CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR (50)
#define CS_FUNCTION_MAX_TEXT_LENGTH (25)
#define CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH (1.00)
#define CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION (0.7)
#define CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG (0.1)

#define CS_FILE_CONNECTOR_STROKE_WIDTH_SVG (1.0)
#define CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG (0.2)

#define CS_LAYER_0_COLOUR (6)
#define CS_LAYER_1_COLOUR (1)
#define CS_LAYER_2_COLOUR (2)
#define CS_LAYER_3_COLOUR (3)
#define CS_LAYER_4_COLOUR (4)
#define CS_LAYER_5_COLOUR (22)
#define CS_LAYER_6_COLOUR (25)
#define CS_LAYER_7_COLOUR (14)
#define CS_LAYER_8_COLOUR (5)
#define CS_LAYER_9_COLOUR (7)
#define CS_LAYER_10_COLOUR (8)
#define CS_LAYER_11_COLOUR (0)
#define CS_LAYER_12_COLOUR (15)

#define CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR (5)
#define CS_FUNCTION_BOX_HIGHLIGHT_COLOUR (5)
*/

#define CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS_NAME "CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS"
#define CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX_NAME "CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX"
#define CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS_NAME "CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS"
#define CS_OUTPUT_Z_POSITION_FILE_BOX_NAME "CS_OUTPUT_Z_POSITION_FILE_BOX"
#define CS_OUTPUT_Z_POSITION_FUNCTION_BOX_NAME "CS_OUTPUT_Z_POSITION_FUNCTION_BOX"
#define CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX_NAME "CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX"
#define CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT_NAME "CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT"

#define CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X_NAME "CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X"
#define CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR_NAME "CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR"
#define CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_NAME "CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG"
#define CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B_NAME "CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B"
#define CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG_NAME "CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG"
#define CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG_NAME "CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG"

#define CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR_NAME "CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR"
#define CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR_NAME "CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR"
#define CS_FILE_MAX_TEXT_LENGTH_NAME "CS_FILE_MAX_TEXT_LENGTH"
#define CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH_NAME "CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH"
#define CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION_NAME "CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION"
#define CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME "CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION"
#define CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION_NAME "CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION"
#define CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME "CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION"
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_NAME "CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X"
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B_NAME "CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B"
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B_NAME "CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B"
#define CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C_NAME "CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C"
#define CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG_NAME "CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG"

#define CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR_NAME "CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR"
#define CS_FUNCTION_MAX_TEXT_LENGTH_NAME "CS_FUNCTION_MAX_TEXT_LENGTH"
#define CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH_NAME "CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH"
#define CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME "CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION"
#define CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG_NAME "CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG"

#define CS_FILE_CONNECTOR_STROKE_WIDTH_SVG_NAME "CS_FILE_CONNECTOR_STROKE_WIDTH_SVG"
#define CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG_NAME "CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG"

#define CS_LAYER_0_COLOUR_NAME "CS_LAYER_0_COLOUR"
#define CS_LAYER_1_COLOUR_NAME "CS_LAYER_1_COLOUR"
#define CS_LAYER_2_COLOUR_NAME "CS_LAYER_2_COLOUR"
#define CS_LAYER_3_COLOUR_NAME "CS_LAYER_3_COLOUR"
#define CS_LAYER_4_COLOUR_NAME "CS_LAYER_4_COLOUR"
#define CS_LAYER_5_COLOUR_NAME "CS_LAYER_5_COLOUR"
#define CS_LAYER_6_COLOUR_NAME "CS_LAYER_6_COLOUR"
#define CS_LAYER_7_COLOUR_NAME "CS_LAYER_7_COLOUR"
#define CS_LAYER_8_COLOUR_NAME "CS_LAYER_8_COLOUR"
#define CS_LAYER_9_COLOUR_NAME "CS_LAYER_9_COLOUR"
#define CS_LAYER_10_COLOUR_NAME "CS_LAYER_10_COLOUR"
#define CS_LAYER_11_COLOUR_NAME "CS_LAYER_11_COLOUR"
#define CS_LAYER_12_COLOUR_NAME "CS_LAYER_12_COLOUR"
#define CS_LAYER_13_COLOUR_NAME "CS_LAYER_0_COLOUR"
#define CS_LAYER_14_COLOUR_NAME "CS_LAYER_1_COLOUR"
#define CS_LAYER_15_COLOUR_NAME "CS_LAYER_2_COLOUR"
#define CS_LAYER_16_COLOUR_NAME "CS_LAYER_3_COLOUR"
#define CS_LAYER_17_COLOUR_NAME "CS_LAYER_4_COLOUR"
#define CS_LAYER_18_COLOUR_NAME "CS_LAYER_5_COLOUR"
#define CS_LAYER_19_COLOUR_NAME "CS_LAYER_6_COLOUR"
#define CS_LAYER_20_COLOUR_NAME "CS_LAYER_7_COLOUR"
#define CS_LAYER_21_COLOUR_NAME "CS_LAYER_8_COLOUR"
#define CS_LAYER_22_COLOUR_NAME "CS_LAYER_9_COLOUR"
#define CS_LAYER_23_COLOUR_NAME "CS_LAYER_10_COLOUR"
#define CS_LAYER_24_COLOUR_NAME "CS_LAYER_11_COLOUR"
#define CS_LAYER_25_COLOUR_NAME "CS_LAYER_12_COLOUR"
#define CS_LAYER_NUMBER_OF_LAYERS (26)

#define CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR_NAME "CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR"
#define CS_FUNCTION_BOX_HIGHLIGHT_COLOUR_NAME "CS_FUNCTION_BOX_HIGHLIGHT_COLOUR"

#ifdef CS_SUPPORT_PREDEFINED_GRID
	#define RULES_XML_TAG_grid ((string)"grid")
	#define RULES_XML_ATTRIBUTE_x ((string)"x")
	#define RULES_XML_ATTRIBUTE_y ((string)"y")
#endif


void initiateMaxXatParticularY();

LDreference* createFileObjectListBoxes(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer, XMLparserTag** currentTag, bool outputFunctionsConnectivity, bool traceFunctionUpwards, XMLparserTag* firstTagInGridTag, bool usePredefinedGrid);

LDreference* createFileObjectListConnections(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfile* aboveLevelObject, XMLparserTag** currentTag, bool traceFunctionUpwards);
	LDreference* createFileObjectConnection(LDreference* currentReferenceInPrintList, CSfile* reference, CSfile* currentReferenceInAboveList, int colour, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag** currentTag);

LDreference* createFunctionObjectListBoxesAndConnections(LDreference* currentReferenceInPrintList, CSfile* aboveLevelFileObject, CSfunction* aboveLevelFunctionObject, CSfileContainer* firstObjectInTopLevelBelowListContainer, int functionLevel, CSfunction* functionReference, XMLparserTag** currentTag, bool traceFunctionUpwards, bool useSingleFileOnly, string* singleFileName, bool usePredefinedGrid);
	LDreference* printFunctionBox(LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, CSfunction* functionObject, int colour);
	LDreference* printFunctionBoxText(LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, CSfunction* functionObject, int colour, bool useSingleFileOnly);
	void resetPrintedFunctionConnections(CSfile* aboveLevelFileObject, CSfunction* aboveLevelFunctionObject, bool useSingleFileOnly, string* singleFileName);
	LDreference* createFunctionObjectConnection(LDreference* currentReferenceInPrintList, CSfunction* reference,  CSfunction* currentReferenceInAboveList, int colour, bool traceAFunctionUpwardsAndNotCurrentlyTracing, bool prepareForTrace, XMLparserTag** currentTag, CSfile* fileObject,  CSfile* currentFileObjectInAboveList);

	LDreference* configureFileOrFunctionObjectConnection(LDreference* currentReferenceInPrintList, vec* referencePrintPos, vec* currentReferenceInAboveListPrintPos, int colour, bool fileOrFunction, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag** currentTag, string* startGroupID, string* endGroupID);
	LDreference* createFileOrFunctionObjectBox(LDreference* currentReferenceInPrintList,  vec* referencePrintPos, string* referenceName, double scaleFactor, int colour, double maxTextLength, double zPosition);
	LDreference* createBox(LDreference* currentReferenceInPrintList, vec* vect, double width, double height, int colour);

int calculateCSBoxAndConnectionColourBasedUponLevel(int yIndex);
int calculateCSBoxAndConnectionColourBasedUponFileName(CSfile* currentFileObject);

bool findPrintedFunctionObjectWithName(string name, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfile** fileObjectHoldingFunction, CSfunction** updatedFunctionObject);
LDreference* traceFunctionsUpwardsAndDrawOrHighLightThese(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* currentFunctionBeingTraced, XMLparserTag** currentTag, int generateHTMLdocumentationMode, string* HTMLdocumentationFunctionTraceTableRows);
	void traceFunctionsUpwardsAndDrawOrHighLightTheseReset(CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* currentFunctionBeingTraced);

void writeFileOrFunctionSVGbox(XMLparserTag** currentTag, vec* pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth);
void writeFileOrFunctionSVGboxTransparent(XMLparserTag** currentTag, vec* pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth, double fillOpacity);

//void fillInCSrulesExternVariables(XMLrulesClass* firstReferenceInCSrulesDraw);
void fillInCSrulesExternVariables();

string createGroupID(string objectName, int printX, int printY);



#endif

