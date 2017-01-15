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
 * File Name: CSdraw.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3c3e 17-November-2012
 *
 *******************************************************************************/

#ifndef HEADER_CS_DRAW
#define HEADER_CS_DRAW

#include "LDreferenceClass.h"
#include "XMLparserClass.h"
#include "CSreferenceClass.h"
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

#define CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR_NAME "CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR"
#define CS_FUNCTION_BOX_HIGHLIGHT_COLOUR_NAME "CS_FUNCTION_BOX_HIGHLIGHT_COLOUR"




void initiateMaxXAtAParticularY();

Reference * createFileReferenceListBoxes(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList, XMLparserTag ** currentTag, bool outputFunctionsConnectivity, bool traceFunctionUpwards);
	bool hasAPreviousReferenceWithThisNameHasbeenPrinted(string name, CSfileReference * firstReferenceInAboveLevelBelowList, int *maxYPos);
	void addShortcutsToPrintedVersionOfReference(CSfileReference * reference, string name, CSfileReference * firstReferenceInAboveLevelBelowList);

Reference * createFileReferenceListConnections(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList, XMLparserTag ** currentTag, bool traceFunctionUpwards);
	Reference * findAndLinkAllFileReferencesWithSameName(Reference * currentReferenceInPrintList, CSfileReference * reference, CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList, XMLparserTag ** currentTag, bool traceFunctionUpwards);
		Reference * createFileReferenceConnection(Reference * currentReferenceInPrintList, CSfileReference * reference,  CSfileReference * currentReferenceInAboveList, int colour, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag ** currentTag);	
	CSfileReference * findPrintedFileReferenceWithName(string name, CSfileReference * reference, CSfileReference * firstReferenceInAboveLevelBelowList, bool * foundPrintedReferenceWithName);

Reference * createFunctionReferenceListBoxesAndConnections(Reference * currentReferenceInPrintList, CSfileReference * aboveLevelFileReference, CSfunctionReference * aboveLevelFunctionReference, CSfileReference * firstReferenceInTopLevelBelowList, int functionLevel, string functionReferenceNameToFind, XMLparserTag ** currentTag, bool traceFunctionUpwards);
	CSreferenceArray findFunctionReferenceInAPrintedFileReferenceRecursive(CSfileReference * fileReferenceToSearchIn, string functionReferenceNameToFind, CSreferenceArray * foundFileAndFunctionReference, bool * referenceFound);
		CSreferenceArray findFunctionReferenceInPrintedFileReference(CSfileReference * fileReferenceToSearchIn, string functionReferenceNameToFind, CSreferenceArray * foundFileAndFunctionReference, bool * referenceFound);
	Reference * createFunctionReferenceConnection(Reference * currentReferenceInPrintList, CSfunctionReference * reference,  CSfunctionReference * currentReferenceInAboveList, int colour, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag ** currentTag);	

	Reference * configureFileOrFunctionReferenceConnection(Reference * currentReferenceInPrintList, vec * referencePrintPos,  vec * currentReferenceInAboveListPrintPos, int colour, bool fileOrFunction, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag ** currentTag);
	Reference * createFileOrFunctionReferenceBox(Reference * currentReferenceInPrintList,  vec * referencePrintPos, string * referenceName, double scaleFactor, int colour, double maxTextLength, double zPosition);
	Reference * createBox(Reference * currentReferenceInPrintList, vec * vect, double width, double height, int colour);

int calculateCSBoxAndConnectionColourBasedUponLevel(int yIndex);

CSfunctionReference * findPrintedFunctionReferenceWithName(string name, CSfunctionReference * reference, CSfileReference * firstReferenceInAboveLevelBelowList, bool * foundPrintedReferenceWithName, string * fileNameHoldingFunction);
Reference * traceFunctionsUpwardsAndDrawOrHighLightThese(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * currentFunctionBeingTraced, XMLparserTag ** currentTag, string topLevelFunctionName, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionTraceTableRows);
	void traceFunctionsUpwardsAndDrawOrHighLightTheseReset(CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * currentFunctionBeingTraced, string topLevelFunctionName);

void writeFileOrFunctionSVGBox(XMLparserTag ** currentTag, vec * pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth);
void writeFileOrFunctionSVGBoxTransparent(XMLparserTag ** currentTag, vec * pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth, double fillOpacity);

//void fillInCSrulesExternVariables(RulesClass * firstReferenceInCSrulesDraw);
void fillInCSrulesExternVariables();


#endif

