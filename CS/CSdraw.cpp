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
 * File Name: CSdraw.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3b2a 28-September-2012
 *
 *******************************************************************************/

//issues, need to flip vertically



#include "CSdraw.h"
#include "LDsvg.h"
#include "LDsprite.h"
#include "LDreferenceManipulation.h"
#include "LDreferenceManipulation.h"
#include "XMLrulesClass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;




static double CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS;
static double CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX;
static double CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS;
static double CS_OUTPUT_Z_POSITION_FILE_BOX;
static double CS_OUTPUT_Z_POSITION_FUNCTION_BOX;
static double CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX;
static double CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

static double CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X;
static double CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR;
static double CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG;
static double CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B;
static double CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG;
static double CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG;

static double CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR;
static double CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR;
static double CS_FILE_MAX_TEXT_LENGTH;
static double CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH;
static double CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION;
static double CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
static double CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION;
static double CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
static double CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X;
static double CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B;
static double CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B;
static double CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C;
static double CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG;

static double CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
static double CS_FUNCTION_MAX_TEXT_LENGTH;
static double CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH;
static double CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
static double CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG;

static int CS_LAYER_0_COLOUR;
static int CS_LAYER_1_COLOUR;
static int CS_LAYER_2_COLOUR;
static int CS_LAYER_3_COLOUR;
static int CS_LAYER_4_COLOUR;
static int CS_LAYER_5_COLOUR;
static int CS_LAYER_6_COLOUR;
static int CS_LAYER_7_COLOUR;
static int CS_LAYER_8_COLOUR;
static int CS_LAYER_9_COLOUR;
static int CS_LAYER_10_COLOUR;
static int CS_LAYER_11_COLOUR;
static int CS_LAYER_12_COLOUR;

static int CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR;
static int CS_FUNCTION_BOX_HIGHLIGHT_COLOUR;



void fillInCSRulesExternVariables()
{
	//extract common sprite variables from either xml file (LRRC or ANN)

	RulesClass * currentReferenceRulesClass = CSrulesDraw;

	while(currentReferenceRulesClass->next != NULL)
	{

		if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS_NAME)
		{
			CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX_NAME)
		{
			CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS_NAME)
		{
			CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FILE_BOX_NAME)
		{
			CS_OUTPUT_Z_POSITION_FILE_BOX = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FUNCTION_BOX_NAME)
		{
			CS_OUTPUT_Z_POSITION_FUNCTION_BOX = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX_NAME)
		{
			CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT_NAME)
		{
			CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X_NAME)
		{
			CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR_NAME)
		{
			CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR  = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_NAME)
		{
			CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B_NAME)
		{
			CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG_NAME)
		{
			CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG_NAME)
		{
			CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR_NAME)
		{
			CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_MAX_TEXT_LENGTH_NAME)
		{
			CS_FILE_MAX_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH_NAME)
		{
			CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION_NAME)
		{
			CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME)
		{
			CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C_NAME)
		{
			CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG_NAME)
		{
			CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG = currentReferenceRulesClass->fractionalValue;
		}



		else if(currentReferenceRulesClass->name == CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR_NAME)
		{
			CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FUNCTION_MAX_TEXT_LENGTH_NAME)
		{
			CS_FUNCTION_MAX_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH_NAME)
		{
			CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME)
		{
			CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG_NAME)
		{
			CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == CS_LAYER_0_COLOUR_NAME)
		{
			CS_LAYER_0_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_1_COLOUR_NAME)
		{
			CS_LAYER_1_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_2_COLOUR_NAME)
		{
			CS_LAYER_2_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_3_COLOUR_NAME)
		{
			CS_LAYER_3_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_4_COLOUR_NAME)
		{
			CS_LAYER_4_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_5_COLOUR_NAME)
		{
			CS_LAYER_5_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_6_COLOUR_NAME)
		{
			CS_LAYER_6_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_7_COLOUR_NAME)
		{
			CS_LAYER_7_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_8_COLOUR_NAME)
		{
			CS_LAYER_8_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_9_COLOUR_NAME)
		{
			CS_LAYER_9_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_10_COLOUR_NAME)
		{
			CS_LAYER_10_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_11_COLOUR_NAME)
		{
			CS_LAYER_11_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_12_COLOUR_NAME)
		{
			CS_LAYER_12_COLOUR = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR_NAME)
		{
			CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FUNCTION_BOX_HIGHLIGHT_COLOUR_NAME)
		{
			CS_FUNCTION_BOX_HIGHLIGHT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

}




int maxXAtAParticularY[MAX_INCLUDE_DEPTH];

void initiateMaxXAtAParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_INCLUDE_DEPTH; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}






Reference * createFileReferenceListBoxes(Reference * currentReferenceInPrintList, CSReference * firstReferenceInAboveLevelBelowList, CSReference * firstReferenceInTopLevelBelowList, ofstream * writeFileObject, bool outputFunctionsConnectivity, bool traceAFunctionUpwards)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	CSReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{


		/*
		if(currentFileReference->name != "")
		{
			for(int i= 0; i<currentFileReference->level; i++)
			{
				cout << "\t";
			}
			cout << "currentFileReference->name = " << currentFileReference->name << endl;
		}
		*/


		int maxYPos = 0;	//always print boxes with max Y pos...
		bool aPreviousReferenceWithThisNameHasbeenPrinted = hasAPreviousReferenceWithThisNameHasbeenPrinted(currentFileReference->name, firstReferenceInTopLevelBelowList, &maxYPos);
		int xPos = maxXAtAParticularY[maxYPos];

		if(!aPreviousReferenceWithThisNameHasbeenPrinted)
		{//add box and connections

			currentFileReference->printXIndex = xPos;
			currentFileReference->printYIndex = maxYPos;

			double vectorObjectsScaleFactor;
			if(outputFunctionsConnectivity)
			{
				vectorObjectsScaleFactor = CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR;
				currentFileReference->printX = xPos * (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X);
				currentFileReference->printY = (double)maxYPos * (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION);
			}
			else
			{
				vectorObjectsScaleFactor = CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR;
				currentFileReference->printX = xPos * (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION);
				currentFileReference->printY = (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION) * (double)maxYPos;
			}

			if(outputFunctionsConnectivity)
			{
				//print large box
				vec bigFileBoxPosition;
				bigFileBoxPosition.x = currentFileReference->printX;
				bigFileBoxPosition.z = CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX;
				bigFileBoxPosition.y = currentFileReference->printY + ((vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B) * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C);
				newCurrentReferenceInPrintList = createBox(newCurrentReferenceInPrintList, &bigFileBoxPosition, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B, DAT_FILE_COLOUR_LIGHTGREY);	//add box

					writeSVGBox(writeFileObject, &bigFileBoxPosition, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B, DAT_FILE_COLOUR_LIGHTGREY, CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG, false);

			}


			#ifdef CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES
			int colour = calculateCSBoxAndConnectionColourBasedUponLevel(maxYPos);
			#else
			int colour = DAT_FILE_COLOUR_CYAN;
			#endif
			currentFileReference->col = colour;

				//print box

			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceAFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif
				newCurrentReferenceInPrintList = createFileOrFunctionReferenceBox(newCurrentReferenceInPrintList, currentFileReference, vectorObjectsScaleFactor, colour, CS_FILE_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FILE_BOX);	//add box
					vec pos;
					pos.x = currentFileReference->printX;
					pos.y = currentFileReference->printY;
					writeFileOrFunctionSVGBox(writeFileObject, &pos, currentFileReference->name.length(), vectorObjectsScaleFactor, CS_FILE_MAX_TEXT_LENGTH, colour, CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG);
			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif



			//print box text
			int numSpritesAdded;
			vec position;

			#ifdef CS_CENTRE_BOXES
			position.x = currentFileReference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*vectorObjectsScaleFactor*(currentFileReference->name.length()/(CS_FILE_MAX_TEXT_LENGTH*CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH));		//should actually be 25; CS_FILE_MAX_TEXT_LENGTH+5
			#else
			position.x = currentFileReference->printX;
			#endif
			position.y = currentFileReference->printY;
			position.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

			currentFileReference->printTextX = position.x;
			currentFileReference->printTextY = position.y;

			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceAFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif

				newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&(currentFileReference->name), newCurrentReferenceInPrintList, &position, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, vectorObjectsScaleFactor/CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR);	//add sprite text within box	//

					position.y = position.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*vectorObjectsScaleFactor;
					writeSVGText(writeFileObject, (currentFileReference->name), &position, vectorObjectsScaleFactor/15, DAT_FILE_COLOUR_BLACK);
			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif

			currentFileReference->printed = true;
			currentFileReference->shortcutToPrintedVersionOfReference = currentFileReference;

			maxXAtAParticularY[maxYPos] = maxXAtAParticularY[maxYPos]+1;
			//xPos++;

			//go through and add shortcutToPrintedVersionOfReference for all file references with same name - this is used later to optimised function list drawing
			addShortcutsToPrintedVersionOfReference(currentFileReference, currentFileReference->name, firstReferenceInTopLevelBelowList);
		}

		if(currentFileReference->firstReferenceInBelowList != NULL)
		{
			newCurrentReferenceInPrintList = createFileReferenceListBoxes(newCurrentReferenceInPrintList, currentFileReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList, writeFileObject, outputFunctionsConnectivity, traceAFunctionUpwards);
		}

		currentFileReference = currentFileReference->next;

	}

	return newCurrentReferenceInPrintList;
}

bool hasAPreviousReferenceWithThisNameHasbeenPrinted(string name, CSReference * firstReferenceInAboveLevelBelowList, int *maxYPos)
{
	bool aPreviousReferenceWithThisNameHasbeenPrinted = false;

	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		if(currentReference->name == name)
		{
			if(currentReference->printed)
			{
				aPreviousReferenceWithThisNameHasbeenPrinted = true;
			}

			if(currentReference->level > *maxYPos)
			{
				*maxYPos = currentReference->level;
			}
		}


		if(currentReference->firstReferenceInBelowList != NULL)
		{
			if(hasAPreviousReferenceWithThisNameHasbeenPrinted(name, currentReference->firstReferenceInBelowList, maxYPos))
			{
				aPreviousReferenceWithThisNameHasbeenPrinted = true;
			}
		}

		currentReference = currentReference->next;
	}

	return aPreviousReferenceWithThisNameHasbeenPrinted;
}

void addShortcutsToPrintedVersionOfReference(CSReference * reference, string name, CSReference * firstReferenceInAboveLevelBelowList)
{
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		if(currentReference->name == name)
		{
			if(!(currentReference->printed))
			{
				currentReference->shortcutToPrintedVersionOfReference = reference;
			}
		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			addShortcutsToPrintedVersionOfReference(reference, name, currentReference->firstReferenceInBelowList);

		}

		currentReference = currentReference->next;
	}

}













Reference * createFileReferenceListConnections(Reference * currentReferenceInPrintList, CSReference * firstReferenceInAboveLevelBelowList, CSReference * firstReferenceInTopLevelBelowList, ofstream * writeFileObject, bool traceAFunctionUpwards)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{

		//print connections
		if(currentReference->printed)
		{//only create connections to printed boxes
			newCurrentReferenceInPrintList = findAndLinkAllFileReferencesWithSameName(newCurrentReferenceInPrintList, currentReference, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, writeFileObject, traceAFunctionUpwards);
		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			newCurrentReferenceInPrintList = createFileReferenceListConnections(newCurrentReferenceInPrintList, currentReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList, writeFileObject, traceAFunctionUpwards);
		}

		currentReference = currentReference->next;

	}

	return newCurrentReferenceInPrintList;
}




Reference * findAndLinkAllFileReferencesWithSameName(Reference * currentReferenceInPrintList, CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList, CSReference * firstReferenceInTopLevelBelowList, ofstream * writeFileObject, bool traceAFunctionUpwards)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		if(currentReference->name == reference->name)
		{
			//if(currentReference->printed)
			//{//only create connections to printed boxes

			if(currentReference->firstReferenceInAboveList != NULL)
			{
				//cout << "currentReference->firstReferenceInAboveList->name = " << currentReference->firstReferenceInAboveList->name << endl;
				CSReference * currentReferenceFirstReferenceInAboveListPrinted = NULL;
				string currentReferenceFirstReferenceInAboveListName = currentReference->firstReferenceInAboveList->name;
				bool foundPrintedReferenceWithName = false;
				currentReferenceFirstReferenceInAboveListPrinted = findPrintedFileReferenceWithName(currentReferenceFirstReferenceInAboveListName, currentReferenceFirstReferenceInAboveListPrinted, firstReferenceInTopLevelBelowList, &foundPrintedReferenceWithName);

				if(foundPrintedReferenceWithName)
				{
					//cout << "h" << endl;
					//cout << "currentReferenceFirstReferenceInAboveListPrinted->name = " << currentReferenceFirstReferenceInAboveListPrinted->name << endl;
					int colour = calculateCSBoxAndConnectionColourBasedUponLevel(currentReferenceFirstReferenceInAboveListPrinted->printYIndex);


					newCurrentReferenceInPrintList = createFileOrFunctionReferenceConnection(newCurrentReferenceInPrintList, reference, currentReferenceFirstReferenceInAboveListPrinted, colour, true, traceAFunctionUpwards, writeFileObject);	//add line

				}
			}
			//}

		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			newCurrentReferenceInPrintList = findAndLinkAllFileReferencesWithSameName(newCurrentReferenceInPrintList, reference, currentReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList, writeFileObject, traceAFunctionUpwards);
		}

		currentReference = currentReference->next;
	}

	return newCurrentReferenceInPrintList;
}

CSReference * findPrintedFileReferenceWithName(string name, CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList, bool * foundPrintedReferenceWithName)
{
	CSReference * updatedReference = reference;
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		if(currentReference->name == name)
		{
			if(currentReference->printed)
			{//only create connections to printed boxes

				updatedReference = currentReference;
				*foundPrintedReferenceWithName = true;
			}

		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			updatedReference = findPrintedFileReferenceWithName(name, updatedReference, currentReference->firstReferenceInBelowList, foundPrintedReferenceWithName);
		}

		currentReference = currentReference->next;
	}

	return updatedReference;
}
























Reference * createFunctionReferenceListBoxesAndConnections(Reference * currentReferenceInPrintList, CSReference * aboveLevelFileReference, CSReference * aboveLevelFunctionReference, CSReference * firstReferenceInTopLevelBelowList, int functionLevel, string functionReferenceNameToFind, ofstream * writeFileObject, bool traceAFunctionUpwards)
{
	int newFunctionLevel = functionLevel;

	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSReferenceArray newFoundFileAndFunctionReference;

	bool referenceFound = false;

	CSReference * functionReference = NULL;
	CSReference * fileReference = NULL;

	newFoundFileAndFunctionReference = findFunctionReferenceInAPrintedFileReferenceRecursive(aboveLevelFileReference, functionReferenceNameToFind, &newFoundFileAndFunctionReference, &referenceFound);
	if(referenceFound)
	{
		fileReference = newFoundFileAndFunctionReference.csref1;

		functionReference = newFoundFileAndFunctionReference.csref2;
	}
	else
	{
		 cout << "error; functon reference not found in file or include files" << endl;
		 cout << "aboveLevelFileReference->name = " << aboveLevelFileReference->name << endl;
		 cout << "aboveLevelFunctionReference->name = " << aboveLevelFunctionReference->name << endl;
		 exit(0);

	}

	/*
	cout << "functionReferenceNameToFind = " << functionReferenceNameToFind << endl;
	cout << "fileReference->name = " << fileReference << endl;
	cout << "functionReference->name = " << functionReference << endl;
	cout << "aboveLevelFunctionReference->name = " << aboveLevelFunctionReference->name << endl;
	cout << "aboveLevelFileReference->name = " << aboveLevelFileReference->name << endl;
	*/

	if(!(fileReference->printed))
	{
		cout << "error2" << endl;
		exit(0);
	}

	if(functionReference->printed == false)
	{
		if(fileReference == aboveLevelFileReference)
		{//function found in existing file

		}
		else
		{//function found in a new file
			newFunctionLevel = 0;
		}




		//now print function;
		functionReference->printXIndex = (fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel]);
		functionReference->printYIndex = newFunctionLevel;

		functionReference->printX = fileReference->printX + (fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel])*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/0.5 - (CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR*0.5 * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X * 0.5);
		functionReference->printY = fileReference->printY + (1+newFunctionLevel)*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
		fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel] = fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel] + 1;


		/*
		cout << "aboveLevelFunctionReference->printX = " << aboveLevelFunctionReference->printX << endl;
		cout << "aboveLevelFunctionReference->printY = " << aboveLevelFunctionReference->printY << endl;
		cout << "functionReference->printX  = " << functionReference->printX << endl;
		cout << "functionReference->printY  = " << functionReference->printY << endl;
		cout << "fileReference->printX  = " << fileReference->printX << endl;
		cout << "fileReference->printY  = " << fileReference->printY << endl;
		cout << "fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel]  = " << fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel] << endl;
		cout << "fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel]  = " << fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel] << endl;
		*/


		#ifdef CS_USE_RAINBOW_COLOURS_FOR_FUNCTION_BOXES
		int aboveConnectionColour = aboveLevelFunctionReference->col;	//or =calculateCSBoxAndConnectionColourBasedUponLevel(aboveLevelFunctionReference->printYIndex);	[same thing]
		int colour = calculateCSBoxAndConnectionColourBasedUponLevel(functionReference->printYIndex);
		#else
			#ifdef CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES
			//new: use same colour as file box
				int aboveConnectionColour = aboveLevelFunctionReference->col;		//OLDWRONG; int aboveConnectionColour = aboveLevelFunctionReference->col;	//OLDWRONG2; same as aboveLevelFileReference->colour;
				int colour = fileReference->col;					//OLDWRONG; int colour = aboveLevelFileReference->col;
			#else
				int aboveConnectionColour = DAT_FILE_COLOUR_MAGENTA;
				int colour = DAT_FILE_COLOUR_CYAN;
			#endif
		#endif
		functionReference->col = colour;


		//print function connections;
		newCurrentReferenceInPrintList = createFileOrFunctionReferenceConnection(newCurrentReferenceInPrintList, functionReference, aboveLevelFunctionReference, aboveConnectionColour, false, traceAFunctionUpwards, writeFileObject);


		#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		if(!traceAFunctionUpwards)
		{//only print connections when not tracing a bottom level function upwards - saves space
		#endif
			//print function box
			newCurrentReferenceInPrintList = createFileOrFunctionReferenceBox(newCurrentReferenceInPrintList, functionReference, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, colour, CS_FUNCTION_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FUNCTION_BOX);	//add box
				vec pos;
				pos.x = functionReference->printX;
				pos.y = functionReference->printY;
				writeFileOrFunctionSVGBox(writeFileObject, &pos, functionReference->name.length(), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, CS_FUNCTION_MAX_TEXT_LENGTH, colour, CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG);
		#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		}
		#endif

		//print box text
		int numSpritesAdded;
		vec position;

		#ifdef CS_CENTRE_BOXES
		position.x = functionReference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*(functionReference->name.length()/(CS_FUNCTION_MAX_TEXT_LENGTH*CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH));
		#else
		position.x = functionReference->printX;
		#endif
		position.y = functionReference->printY;
		position.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;
		functionReference->printTextX = position.x;
		functionReference->printTextY = position.y;

		#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		if(!traceAFunctionUpwards)
		{//only print connections when not tracing a bottom level function upwards - saves space
		#endif
			newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&(functionReference->name), newCurrentReferenceInPrintList, &position, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR);	//add sprite text within box

				position.y = position.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
				writeSVGText(writeFileObject, (functionReference->name), &position, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B, DAT_FILE_COLOUR_BLACK);

		#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		}
		#endif

		functionReference->printed = true;

		/*
		for(int i= 0; i<newFunctionLevel+fileReference->level; i++)
		{
			cout << "\t";
		}
		cout << "file = " << fileReference->name << endl;
		for(int i= 0; i<newFunctionLevel+fileReference->level; i++)
		{
			cout << "\t";
		}
		cout << "function = " << functionReference->name << endl;
		*/

		/*
		if(functionReference->printX > 3000)
		{
			cout << "functionReferenceNameToFind = " << functionReferenceNameToFind << endl;
			cout << "fileReference->name = " << fileReference->name << endl;
			cout << "functionReference->name = " << functionReference->name << endl;
			cout << "aboveLevelFunctionReference->name = " << aboveLevelFunctionReference->name << endl;
			cout << "aboveLevelFileReference->name = " << aboveLevelFileReference->name << endl;
			exit(0);
		}
		*/


		//now for each function Reference Reference within function reference;
		CSReference * currentfunctionReferenceReference = functionReference->firstReferenceInFunctionReferenceList;
		while(currentfunctionReferenceReference->next != NULL)
		{
			/*
			for(int i= 0; i<newFunctionLevel+fileReference->level+1; i++)
			{
				cout << "\t";
			}
			cout << "function reference to find = " << currentfunctionReferenceReference->name << endl;
			for(int i= 0; i<newFunctionLevel+fileReference->level+1; i++)
			{
				cout << "\t";
			}
			cout << "top level include file to search = " << fileReference->name << endl;
			*/

			//recurse
			newCurrentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(newCurrentReferenceInPrintList, fileReference, functionReference, firstReferenceInTopLevelBelowList, (newFunctionLevel+1), currentfunctionReferenceReference->name, writeFileObject, traceAFunctionUpwards);

			currentfunctionReferenceReference = currentfunctionReferenceReference->next;
		}
	}
	else
	{

		//print function connections;
		newCurrentReferenceInPrintList = createFileOrFunctionReferenceConnection(newCurrentReferenceInPrintList, functionReference, aboveLevelFunctionReference, aboveLevelFunctionReference->col, false, traceAFunctionUpwards, writeFileObject);

	}

	return newCurrentReferenceInPrintList;

}

CSReferenceArray findFunctionReferenceInAPrintedFileReferenceRecursive(CSReference * fileReferenceToSearchIn, string functionReferenceNameToFind, CSReferenceArray * foundFileAndFunctionReference, bool * referenceFound)
{
	CSReferenceArray newFoundFileAndFunctionReference;
	newFoundFileAndFunctionReference.csref1 = foundFileAndFunctionReference->csref1;
	newFoundFileAndFunctionReference.csref2 = foundFileAndFunctionReference->csref2;

	//search current file;
	newFoundFileAndFunctionReference = findFunctionReferenceInPrintedFileReference(fileReferenceToSearchIn, functionReferenceNameToFind, &newFoundFileAndFunctionReference, referenceFound);

	//search include files;
	if(fileReferenceToSearchIn->firstReferenceInBelowList != NULL)
	{
		CSReference * currentReference = fileReferenceToSearchIn->firstReferenceInBelowList;
		while(currentReference->next != NULL)
		{
			newFoundFileAndFunctionReference = findFunctionReferenceInAPrintedFileReferenceRecursive(currentReference, functionReferenceNameToFind, &newFoundFileAndFunctionReference, referenceFound);

			currentReference = currentReference->next;
		}
	}

	return newFoundFileAndFunctionReference;
}

CSReferenceArray findFunctionReferenceInPrintedFileReference(CSReference * fileReferenceToSearchIn, string functionReferenceNameToFind, CSReferenceArray * foundFileAndFunctionReference, bool * referenceFound)
{
	CSReferenceArray newFoundFileAndFunctionReference;
	newFoundFileAndFunctionReference.csref1 = foundFileAndFunctionReference->csref1;
	newFoundFileAndFunctionReference.csref2 = foundFileAndFunctionReference->csref2;

	CSReference * currentReferenceEnsuredPrinted = NULL;
	if(!(fileReferenceToSearchIn->printed))
	{
		currentReferenceEnsuredPrinted = fileReferenceToSearchIn->shortcutToPrintedVersionOfReference;

		/*
		if(fileReferenceToSearchIn->name != currentReferenceEnsuredPrinted->name)
		{
			cout << "er" << endl;
			exit(0);
		}
		*/
	}
	else
	{
		currentReferenceEnsuredPrinted = fileReferenceToSearchIn;
	}

	CSReference * currentFunctionReference = currentReferenceEnsuredPrinted->firstReferenceInFunctionList;
	while(currentFunctionReference->next != NULL)
	{
		if(currentFunctionReference->name == functionReferenceNameToFind)
		{
			/*
			cout << "match found:" << endl;
			cout << "currentFunctionReference->name = " << currentFunctionReference->name << endl;
			cout << "functionReferenceNameToFind = " << functionReferenceNameToFind << endl;
			cout << "currentReferenceEnsuredPrinted->name = " << currentReferenceEnsuredPrinted->name << endl;
			*/
			newFoundFileAndFunctionReference.csref1 = currentReferenceEnsuredPrinted;
			newFoundFileAndFunctionReference.csref2 = currentFunctionReference;
			*referenceFound = true;
		}
		currentFunctionReference = currentFunctionReference->next;
	}

	return newFoundFileAndFunctionReference;
}





















Reference * createFileOrFunctionReferenceBox(Reference * currentReferenceInPrintList, CSReference * reference, double scaleFactor, int colour, double maxTextLength, double zPosition)
{
	//cout << "zPosition = " << zPosition << endl;

	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	newCurrentReferenceInPrintList->type = REFERENCE_TYPE_QUAD;
	newCurrentReferenceInPrintList->colour = colour;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex1relativePosition.x = reference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*reference->name.length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex1relativePosition.x = reference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex1relativePosition.y = reference->printY + CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex1relativePosition.z = zPosition;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex2relativePosition.x = reference->printX + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*reference->name.length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex2relativePosition.x = reference->printX + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex2relativePosition.y = reference->printY + CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex2relativePosition.z = zPosition;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex3relativePosition.x = reference->printX + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*reference->name.length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex3relativePosition.x = reference->printX + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex3relativePosition.y = reference->printY - CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex3relativePosition.z = zPosition;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex4relativePosition.x = reference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*reference->name.length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex4relativePosition.x = reference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex4relativePosition.y = reference->printY - CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex4relativePosition.z = zPosition;

	/*
	cout << "createFileOrFunctionReferenceBox():" << endl;
	cout << "reference->name = " << reference->name << endl;
	cout << "newCurrentReferenceInPrintList->type = " << newCurrentReferenceInPrintList->type << endl;
	cout << "newCurrentReferenceInPrintList->colour = " << newCurrentReferenceInPrintList->colour << endl;
	cout << "newCurrentReferenceInPrintList->vertex1relativePosition.x = " << newCurrentReferenceInPrintList->vertex1relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex1relativePosition.y = " << newCurrentReferenceInPrintList->vertex1relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex1relativePosition.z = " << newCurrentReferenceInPrintList->vertex1relativePosition.z << endl;
	cout << "newCurrentReferenceInPrintList->vertex2relativePosition.x = " << newCurrentReferenceInPrintList->vertex2relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex2relativePosition.y = " << newCurrentReferenceInPrintList->vertex2relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex2relativePosition.z = " << newCurrentReferenceInPrintList->vertex2relativePosition.z << endl;
	cout << "newCurrentReferenceInPrintList->vertex3relativePosition.x = " << newCurrentReferenceInPrintList->vertex3relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex3relativePosition.y = " << newCurrentReferenceInPrintList->vertex3relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex3relativePosition.z = " << newCurrentReferenceInPrintList->vertex3relativePosition.z << endl;
	cout << "newCurrentReferenceInPrintList->vertex4relativePosition.x = " << newCurrentReferenceInPrintList->vertex4relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex4relativePosition.y = " << newCurrentReferenceInPrintList->vertex4relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex4relativePosition.z = " << newCurrentReferenceInPrintList->vertex4relativePosition.z << endl;
	*/

	Reference * newDispayReference = new Reference();
	newCurrentReferenceInPrintList->next = newDispayReference;
	newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

	return newCurrentReferenceInPrintList;
}


Reference * createFileOrFunctionReferenceConnection(Reference * currentReferenceInPrintList, CSReference * reference,  CSReference * currentReferenceInAboveList, int colour, bool fileOrFunction, bool traceAFunctionUpwardsAndNotCurrentlyTracing, ofstream * writeFileObject)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;


	//cout << "h1" << endl;
	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace
	CSReferenceContainer * currentReferenceContainerInAboveFileOrFunctionReferenceList = reference->firstReferenceContainerInAboveFileOrFunctionReferenceList;
	while(currentReferenceContainerInAboveFileOrFunctionReferenceList->next != NULL)
	{
		currentReferenceContainerInAboveFileOrFunctionReferenceList = currentReferenceContainerInAboveFileOrFunctionReferenceList->next;
	}
	currentReferenceContainerInAboveFileOrFunctionReferenceList->name = currentReferenceInAboveList->name;
	CSReferenceContainer * newCSRefContainer = new CSReferenceContainer();
	currentReferenceContainerInAboveFileOrFunctionReferenceList->next = newCSRefContainer;
	//cout << "h2" << endl;


	bool draw = false;
	if(fileOrFunction)
	{
		#ifdef CS_DO_NOT_DRAW_ALL_FILE_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		if(!traceAFunctionUpwardsAndNotCurrentlyTracing)
		{//only print connections when not tracing a bottom level function upwards - saves space
		#endif
			draw = true;
		#ifdef CS_DO_NOT_DRAW_ALL_FILE_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		}
		#endif
	}
	else
	{
		#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		if(!traceAFunctionUpwardsAndNotCurrentlyTracing)
		{//only print connections when not tracing a bottom level function upwards - saves space
		#endif
			draw = true;
		#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		}
		#endif
	}

	if(draw)
	{
		//cout << "drawing connection" << endl;

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = colour;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = currentReferenceInAboveList->printX;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = currentReferenceInAboveList->printY;
		if(fileOrFunction)
		{
			newCurrentReferenceInPrintList->vertex1relativePosition.z = CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS;	//why doesnt this work
		}
		else
		{
			newCurrentReferenceInPrintList->vertex1relativePosition.z = CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS;	//why doesnt this work
		}

		newCurrentReferenceInPrintList->vertex2relativePosition.x = reference->printX;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = reference->printY;
		if(fileOrFunction)
		{
			newCurrentReferenceInPrintList->vertex2relativePosition.z = CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS;	//why doesnt this work
		}
		else
		{
			newCurrentReferenceInPrintList->vertex2relativePosition.z = CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS;	//why doesnt this work
		}
		/*
		cout << "createFileOrFunctionReferenceConnection():" << endl;
		cout << "currentReferenceInAboveList->name = " << currentReferenceInAboveList->name << endl;
		cout << "reference->name = " << reference->name << endl;
		cout << "newCurrentReferenceInPrintList->type = " << newCurrentReferenceInPrintList->type << endl;
		cout << "newCurrentReferenceInPrintList->colour = " << newCurrentReferenceInPrintList->colour << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.x = " << newCurrentReferenceInPrintList->vertex1relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.y = " << newCurrentReferenceInPrintList->vertex1relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.z = " << newCurrentReferenceInPrintList->vertex1relativePosition.z << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.x = " << newCurrentReferenceInPrintList->vertex2relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.y = " << newCurrentReferenceInPrintList->vertex2relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.z = " << newCurrentReferenceInPrintList->vertex2relativePosition.z << endl;
		*/

		Reference * newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		vec pos1;
		pos1.x = reference->printX;
		pos1.y = reference->printY;
		vec pos2;
		pos2.x = currentReferenceInAboveList->printX;
		pos2.y = currentReferenceInAboveList->printY;
		writeSVGLine(writeFileObject, &pos1, &pos2, colour);

	}

	return newCurrentReferenceInPrintList;
}

Reference * createBox(Reference * currentReferenceInPrintList, vec * vect, double width, double height, int colour)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	newCurrentReferenceInPrintList->type = REFERENCE_TYPE_QUAD;
	newCurrentReferenceInPrintList->colour = colour;

	newCurrentReferenceInPrintList->vertex1relativePosition.x = vect->x - width/2.0;
	newCurrentReferenceInPrintList->vertex1relativePosition.y = vect->y + height/2.0;
	newCurrentReferenceInPrintList->vertex1relativePosition.z = vect->z;

	newCurrentReferenceInPrintList->vertex2relativePosition.x = vect->x + width/2.0;
	newCurrentReferenceInPrintList->vertex2relativePosition.y = vect->y + height/2.0;
	newCurrentReferenceInPrintList->vertex2relativePosition.z = vect->z;

	newCurrentReferenceInPrintList->vertex3relativePosition.x = vect->x + width/2.0;
	newCurrentReferenceInPrintList->vertex3relativePosition.y = vect->y - height/2.0;
	newCurrentReferenceInPrintList->vertex3relativePosition.z = vect->z;

	newCurrentReferenceInPrintList->vertex4relativePosition.x = vect->x - width/2.0;
	newCurrentReferenceInPrintList->vertex4relativePosition.y = vect->y - height/2.0;
	newCurrentReferenceInPrintList->vertex4relativePosition.z = vect->z;

	/*
	cout << "createFileOrFunctionReferenceBox():" << endl;
	cout << "reference->name = " << reference->name << endl;
	cout << "newCurrentReferenceInPrintList->type = " << newCurrentReferenceInPrintList->type << endl;
	cout << "newCurrentReferenceInPrintList->colour = " << newCurrentReferenceInPrintList->colour << endl;
	cout << "newCurrentReferenceInPrintList->vertex1relativePosition.x = " << newCurrentReferenceInPrintList->vertex1relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex1relativePosition.y = " << newCurrentReferenceInPrintList->vertex1relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex1relativePosition.z = " << newCurrentReferenceInPrintList->vertex1relativePosition.z << endl;
	cout << "newCurrentReferenceInPrintList->vertex2relativePosition.x = " << newCurrentReferenceInPrintList->vertex2relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex2relativePosition.y = " << newCurrentReferenceInPrintList->vertex2relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex2relativePosition.z = " << newCurrentReferenceInPrintList->vertex2relativePosition.z << endl;
	cout << "newCurrentReferenceInPrintList->vertex3relativePosition.x = " << newCurrentReferenceInPrintList->vertex3relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex3relativePosition.y = " << newCurrentReferenceInPrintList->vertex3relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex3relativePosition.z = " << newCurrentReferenceInPrintList->vertex3relativePosition.z << endl;
	cout << "newCurrentReferenceInPrintList->vertex4relativePosition.x = " << newCurrentReferenceInPrintList->vertex4relativePosition.x << endl;
	cout << "newCurrentReferenceInPrintList->vertex4relativePosition.y = " << newCurrentReferenceInPrintList->vertex4relativePosition.y << endl;
	cout << "newCurrentReferenceInPrintList->vertex4relativePosition.z = " << newCurrentReferenceInPrintList->vertex4relativePosition.z << endl;
	*/

	Reference * newDispayReference = new Reference();
	newCurrentReferenceInPrintList->next = newDispayReference;
	newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

	return newCurrentReferenceInPrintList;
}














int calculateCSBoxAndConnectionColourBasedUponLevel(int yIndex)
{
	int colour;

	if(yIndex > 32)
	{
		cout << "warning: excessive nesting deteted, yIndex > 32, yIndex = " << yIndex << endl;
	}

	if(yIndex%13 == 0)
	{
		colour = CS_LAYER_0_COLOUR;	//DAT_FILE_COLOUR_BROWN
	}
	else if(yIndex%13 == 1)
	{
		colour = CS_LAYER_1_COLOUR;	//DAT_FILE_COLOUR_BLUE
	}
	else if(yIndex%13 == 2)
	{
		colour = CS_LAYER_2_COLOUR;	//DAT_FILE_COLOUR_GREEN
	}
	else if(yIndex%13 == 3)
	{
		colour = CS_LAYER_3_COLOUR;	//DAT_FILE_COLOUR_CYAN
	}
	else if(yIndex%13 == 4)
	{
		colour = CS_LAYER_4_COLOUR;	//DAT_FILE_COLOUR_RED
	}
	else if(yIndex%13 == 5)
	{
		colour = CS_LAYER_5_COLOUR;	//DAT_FILE_COLOUR_PURPLE
	}
	else if(yIndex%13 == 6)
	{
		colour = CS_LAYER_6_COLOUR;	//DAT_FILE_COLOUR_ORANGE
	}
	else if(yIndex%13 == 7)
	{
		colour = CS_LAYER_7_COLOUR;	//DAT_FILE_COLOUR_YELLOW
	}
	else if(yIndex%13 == 8)
	{
		colour = CS_LAYER_8_COLOUR;	//DAT_FILE_COLOUR_MAGENTA
	}
	else if(yIndex%13 == 9)
	{
		colour = CS_LAYER_9_COLOUR;	//DAT_FILE_COLOUR_LIGHTGREY
	}
	else if(yIndex%13 == 10)
	{
		colour = CS_LAYER_10_COLOUR;	//DAT_FILE_COLOUR_DARKGREY
	}
	else if(yIndex%13 == 11)
	{
		colour = CS_LAYER_11_COLOUR;	//DAT_FILE_COLOUR_BLACK
	}
	else if(yIndex%13 == 12)
	{
		colour = CS_LAYER_12_COLOUR;	//DAT_FILE_COLOUR_WHITE
	}
	else
	{
		cout << "invalid CS reference level for conversion to colour" << endl;
		cout << "yIndex = " << yIndex << endl;
		exit(0);
	}

	return colour;

}


















CSReference * findPrintedFunctionReferenceWithName(string name, CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList, bool * foundPrintedReferenceWithName, string * fileNameHoldingFunction)
{
	CSReference * updatedFunctionReference = reference;
	CSReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{
		CSReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
		while(currentFunctionReference->next != NULL)
		{
			if(currentFunctionReference->name == name)
			{
				if(currentFunctionReference->printed)
				{//only create connections to printed boxes

					updatedFunctionReference = currentFunctionReference;
					*foundPrintedReferenceWithName = true;
					*fileNameHoldingFunction = currentFileReference->name;
				}
			}
			currentFunctionReference = currentFunctionReference->next;
		}

		if(currentFileReference->firstReferenceInBelowList != NULL)
		{
			updatedFunctionReference = findPrintedFunctionReferenceWithName(name, updatedFunctionReference, currentFileReference->firstReferenceInBelowList, foundPrintedReferenceWithName, fileNameHoldingFunction);
		}

		currentFileReference = currentFileReference->next;
	}

	return updatedFunctionReference;
}

Reference * traceFunctionsUpwardsAndDrawOrHighLightThese(Reference * currentReferenceInPrintList, CSReference * firstReferenceInTopLevelBelowList, CSReference * currentFunctionBeingTraced, ofstream * writeFileObject, string topLevelFunctionName, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionTraceTableRows)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	//print function box;
	int functionBoxColour;
	#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		functionBoxColour = currentFunctionBeingTraced->col;
	#else
		functionBoxColour = CS_FUNCTION_BOX_HIGHLIGHT_COLOUR;
		//just highlight the box (already drawn)
	#endif
	newCurrentReferenceInPrintList = createFileOrFunctionReferenceBox(newCurrentReferenceInPrintList, currentFunctionBeingTraced, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, functionBoxColour, CS_FUNCTION_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX);	//add box
		vec pos;
		pos.x = currentFunctionBeingTraced->printX;
		pos.y = currentFunctionBeingTraced->printY;
		writeFileOrFunctionSVGBoxTransparent(writeFileObject, &pos, currentFunctionBeingTraced->name.length(), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, CS_FUNCTION_MAX_TEXT_LENGTH, functionBoxColour, CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG, 0.5);

	//print function text;
	#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		int numSpritesAdded;	//not used
		vec textPosition;
		textPosition.x = currentFunctionBeingTraced->printTextX;
		textPosition.y = currentFunctionBeingTraced->printTextY;
		textPosition.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

		newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&(currentFunctionBeingTraced->name), newCurrentReferenceInPrintList, &textPosition, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/(200.0));	//add sprite text within box

		textPosition.y = textPosition.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
		writeSVGText(writeFileObject, (currentFunctionBeingTraced->name), &textPosition, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B, DAT_FILE_COLOUR_BLACK);

	#endif

	currentFunctionBeingTraced->printedTrace = true;
	currentFunctionBeingTraced->printedTraceReset = false;

	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace

	CSReferenceContainer * currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList = currentFunctionBeingTraced->firstReferenceContainerInAboveFileOrFunctionReferenceList;
	while(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->next != NULL)	// && (currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name != "main")
	{
		//cout << "currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name = " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name << endl;

		bool higherLevelFunctionFound = false;
		string fileNameHoldingFunction = "";
		CSReference * higherLevelFunction = findPrintedFunctionReferenceWithName(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name, NULL, firstReferenceInTopLevelBelowList, &higherLevelFunctionFound, &fileNameHoldingFunction);

		if(higherLevelFunctionFound)
		{
			if(higherLevelFunction->printedTrace == false)
			{
				//cout << "higherLevelFunctionFound, " << higherLevelFunction->name << endl;
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
				{
					#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
					cout << "Trace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << currentFunctionBeingTraced->name << "()" << endl;
					#endif
				}
				else if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
				{
					string HTMLdocumentationFunctionTraceTableRow = "\t\t<tr><td>" + fileNameHoldingFunction + "</td><td>" + currentFunctionBeingTraced->name + "</td></tr>\n";
					*HTMLdocumentationFunctionTraceTableRows = *HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableRow;
				}
				
				//print function connection;
				int functionConnectionColour;
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
					#ifdef CS_DO_NOT_DRAW_ALL_FILE_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
						functionConnectionColour = higherLevelFunction->col;
					#else
						functionConnectionColour = CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR;
					#endif
				#else
					functionConnectionColour = CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR;
					//just highlight the connection (already drawn)
				#endif
				newCurrentReferenceInPrintList = createFileOrFunctionReferenceConnection(newCurrentReferenceInPrintList, currentFunctionBeingTraced, higherLevelFunction, functionConnectionColour, false, false, writeFileObject);

				newCurrentReferenceInPrintList = traceFunctionsUpwardsAndDrawOrHighLightThese(newCurrentReferenceInPrintList, firstReferenceInTopLevelBelowList, higherLevelFunction, writeFileObject, topLevelFunctionName, generateHTMLdocumentationMode, HTMLdocumentationFunctionTraceTableRows);
			}
		}
		else
		{
			cout << "error: traceFunctionsUpwardsAndDrawOrHighLightThese(): higherLevelFunctionFound " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name << " cannot be found" << endl;
			exit(0);
		}

		currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->next;
	}

	return newCurrentReferenceInPrintList;
}



void traceFunctionsUpwardsAndDrawOrHighLightTheseReset(CSReference * firstReferenceInTopLevelBelowList, CSReference * currentFunctionBeingTraced, string topLevelFunctionName)
{
	currentFunctionBeingTraced->printedTrace = false;
	currentFunctionBeingTraced->printedTraceReset = true;

	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace

	CSReferenceContainer * currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList = currentFunctionBeingTraced->firstReferenceContainerInAboveFileOrFunctionReferenceList;
	while(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->next != NULL)	// && (currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name != "main")
	{
		//cout << "currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name = " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name << endl;

		bool higherLevelFunctionFound = false;
		string fileNameHoldingFunction = "";
		CSReference * higherLevelFunction = findPrintedFunctionReferenceWithName(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name, NULL, firstReferenceInTopLevelBelowList, &higherLevelFunctionFound, &fileNameHoldingFunction);

		if(higherLevelFunctionFound)
		{
			if(higherLevelFunction->printedTraceReset == false)
			{
				traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstReferenceInTopLevelBelowList, higherLevelFunction, topLevelFunctionName);
			}
		}
		else
		{
			cout << "error: traceFunctionsUpwardsAndDrawOrHighLightThese(): higherLevelFunctionFound " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name << " cannot be found" << endl;
			exit(0);
		}

		currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->next;
	}
}




void writeFileOrFunctionSVGBox(ofstream * writeFileObject, vec * pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth)
{
#ifdef CS_CENTRE_BOXES
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor*(double)textLength/maxTextLength;
#else
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor;
#endif
	double height = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG*scaleFactor;

	writeSVGBox(writeFileObject,  pos, width, height, col, boxOutlineWidth, false);
}

void writeFileOrFunctionSVGBoxTransparent(ofstream * writeFileObject, vec * pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth, double fillOpacity)
{
#ifdef CS_CENTRE_BOXES
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor*(double)textLength/maxTextLength;
#else
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor;
#endif
	double height = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG*scaleFactor;

	writeSVGBoxTransparent(writeFileObject,  pos, width, height, col, boxOutlineWidth, false, fillOpacity);
}





