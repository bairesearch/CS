 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSdraw.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_CS_DRAW
#define HEADER_CS_DRAW

#include "LDreferenceClass.hpp"
#include "XMLparserClass.hpp"
#include "CSreferenceContainerClass.hpp"
#include "CSglobalDefs.hpp"
#include "LDsvg.hpp"
#include "LDsprite.hpp"
#include "LDreferenceManipulation.hpp"
#include "LDreferenceManipulation.hpp"
#include "XMLrulesClass.hpp"

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


class CSdrawClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: LDsvgClass LDsvg;
	private: LDspriteClass LDsprite;
	private: CSreferenceContainerClassClass CSreferenceContainerClass;
	public: void initiateMaxXatParticularY();

	public: LDreference* createFileObjectListBoxes(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer, XMLparserTag** currentTag, const bool outputFunctionsConnectivity, const bool traceFunctionUpwards, XMLparserTag* firstTagInGridTag, const bool usePredefinedGrid);
	public: void createFileObjectListBoxesPrintedReset(CSfileContainer* firstObjectInAboveLevelBelowListContainer);
		private: bool hasPreviousReferenceWithThisNameHasBeenPrinted(string name, CSfileContainer* firstObjectInTopLevelBelowListContainer, int *maxYPos);
		#ifdef CS_OPTIMISE_CS_DRAW_YMAXPOS_SEARCH
		private: void hasPreviousReferenceWithThisNameHasBeenPrintedReset(CSfileContainer* firstObjectInTopLevelBelowListContainer);
		#endif

	public: LDreference* createFileObjectListConnections(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfile* aboveLevelObject, XMLparserTag** currentTag, const bool traceFunctionUpwards);
		private: LDreference* createFileObjectConnection(LDreference* currentReferenceInPrintList, CSfile* reference, CSfile* currentReferenceInAboveList, int colour, const bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag** currentTag);

	public: LDreference* createFunctionObjectListBoxesAndConnections(LDreference* currentReferenceInPrintList, CSfile* aboveLevelFileObject, CSfunction* aboveLevelFunctionObject, CSfileContainer* firstObjectInTopLevelBelowListContainer, const int functionLevel, const CSfunction* functionReference, XMLparserTag** currentTag, const bool traceFunctionUpwards, const bool useSingleFileOnly, string* singleFileName, const bool usePredefinedGrid, const bool isTopLevelFunction);
		public: LDreference* printFunctionBox(LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, CSfunction* functionObject, int colour);
		public: LDreference* printFunctionBoxText(LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, CSfunction* functionObject, const int colour, const bool useSingleFileOnly);
		public: void resetPrintedFunctionConnections(constEffective CSfile* aboveLevelFileObject, CSfunction* aboveLevelFunctionObject, const bool useSingleFileOnly, const string* singleFileName);
		private: LDreference* createFunctionObjectConnection(LDreference* currentReferenceInPrintList, CSfunction* reference,  CSfunction* currentReferenceInAboveList, int colour, const bool traceAFunctionUpwardsAndNotCurrentlyTracing, const bool prepareForTrace, XMLparserTag** currentTag, const CSfile* fileObject,  CSfile* currentFileObjectInAboveList);

		private: LDreference* configureFileOrFunctionObjectConnection(LDreference* currentReferenceInPrintList, vec* referencePrintPos, vec* currentReferenceInAboveListPrintPos, int colour, const bool fileOrFunction, const bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag** currentTag, const string* startGroupID, const string* endGroupID);
		private: LDreference* createFileOrFunctionObjectBox(LDreference* currentReferenceInPrintList,  vec* referencePrintPos, string* referenceName, double scaleFactor, int colour, double maxTextLength, double zPosition);
		private: LDreference* createBox(LDreference* currentReferenceInPrintList, vec* vect, const double width, const double height, int colour);

	private: int calculateCSBoxAndConnectionColourBasedUponLevel(const int yIndex);
	private: int calculateCSBoxAndConnectionColourBasedUponFileName(const CSfile* currentFileObject);

	public: bool findPrintedFunctionObjectWithName(const string name, constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** updatedFunctionObject);
	public: LDreference* traceFunctionsUpwardsAndDrawOrHighLightThese(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* currentFunctionBeingTraced, XMLparserTag** currentTag, int generateHTMLdocumentationMode, string* HTMLdocumentationFunctionTraceTableRows);
		public: void traceFunctionsUpwardsAndDrawOrHighLightTheseReset(const CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* currentFunctionBeingTraced);

	private: void writeFileOrFunctionSVGbox(XMLparserTag** currentTag, const vec* pos, const int textLength, const double scaleFactor, const double maxTextLength, const int col, const double boxOutlineWidth);
	private: void writeFileOrFunctionSVGboxTransparent(XMLparserTag** currentTag, const vec* pos, const int textLength, const double scaleFactor, const double maxTextLength, const int col, const double boxOutlineWidth, const double fillOpacity);

	//void fillInCSrulesExternVariables(XMLrulesClass* firstReferenceInCSrulesDraw);
	public: void fillInCSrulesExternVariables();

	private: string createGroupID(const string objectName, const int printX, const int printY);
};



#endif

