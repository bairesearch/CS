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
 * Project Version: 3c5c 21-December-2012
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
static double CS_FILE_CONNECTOR_STROKE_WIDTH_SVG;
static double CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG;

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

static string CS_LAYER_0_COLOUR_PROJECT_NAME;
static string CS_LAYER_1_COLOUR_PROJECT_NAME;
static string CS_LAYER_2_COLOUR_PROJECT_NAME;
static string CS_LAYER_3_COLOUR_PROJECT_NAME;
static string CS_LAYER_4_COLOUR_PROJECT_NAME;
static string CS_LAYER_5_COLOUR_PROJECT_NAME;
static string CS_LAYER_6_COLOUR_PROJECT_NAME;
static string CS_LAYER_7_COLOUR_PROJECT_NAME;
static string CS_LAYER_8_COLOUR_PROJECT_NAME;
static string CS_LAYER_9_COLOUR_PROJECT_NAME;
static string CS_LAYER_10_COLOUR_PROJECT_NAME;
static string CS_LAYER_11_COLOUR_PROJECT_NAME;
static string CS_LAYER_12_COLOUR_PROJECT_NAME;

static int CSlayerColourArray[CS_LAYER_NUMBER_OF_LAYERS];
static string CSlayerColourProjectNameArray[CS_LAYER_NUMBER_OF_LAYERS];

static int CS_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR;
static int CS_FUNCTION_BOX_HIGHLIGHT_COLOUR;



void fillInCSrulesExternVariables()
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

		else if(currentReferenceRulesClass->name == CS_FILE_CONNECTOR_STROKE_WIDTH_SVG_NAME)
		{
			CS_FILE_CONNECTOR_STROKE_WIDTH_SVG = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG_NAME)
		{
			CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG = currentReferenceRulesClass->fractionalValue;
		}

		else if(currentReferenceRulesClass->name == CS_LAYER_0_COLOUR_NAME)
		{
			CS_LAYER_0_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_0_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[0] = CS_LAYER_0_COLOUR;
			CSlayerColourProjectNameArray[0] = CS_LAYER_0_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_1_COLOUR_NAME)
		{
			CS_LAYER_1_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_1_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[1] = CS_LAYER_1_COLOUR;
			CSlayerColourProjectNameArray[1] = CS_LAYER_1_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_2_COLOUR_NAME)
		{
			CS_LAYER_2_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_2_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[2] = CS_LAYER_2_COLOUR;
			CSlayerColourProjectNameArray[2] = CS_LAYER_2_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_3_COLOUR_NAME)
		{
			CS_LAYER_3_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_3_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[3] = CS_LAYER_3_COLOUR;
			CSlayerColourProjectNameArray[3] = CS_LAYER_3_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_4_COLOUR_NAME)
		{
			CS_LAYER_4_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_4_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[4] = CS_LAYER_4_COLOUR;
			CSlayerColourProjectNameArray[4] = CS_LAYER_4_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_5_COLOUR_NAME)
		{
			CS_LAYER_5_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_5_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[5] = CS_LAYER_5_COLOUR;
			CSlayerColourProjectNameArray[5] = CS_LAYER_5_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_6_COLOUR_NAME)
		{
			CS_LAYER_6_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_6_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[6] = CS_LAYER_6_COLOUR;
			CSlayerColourProjectNameArray[6] = CS_LAYER_6_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_7_COLOUR_NAME)
		{
			CS_LAYER_7_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_7_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[7] = CS_LAYER_7_COLOUR;
			CSlayerColourProjectNameArray[7] = CS_LAYER_7_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_8_COLOUR_NAME)
		{
			CS_LAYER_8_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_8_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[8] = CS_LAYER_8_COLOUR;
			CSlayerColourProjectNameArray[8] = CS_LAYER_8_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_9_COLOUR_NAME)
		{
			CS_LAYER_9_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_9_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[9] = CS_LAYER_9_COLOUR;
			CSlayerColourProjectNameArray[9] = CS_LAYER_9_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_10_COLOUR_NAME)
		{
			CS_LAYER_10_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_10_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[10] = CS_LAYER_10_COLOUR;
			CSlayerColourProjectNameArray[10] = CS_LAYER_10_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_11_COLOUR_NAME)
		{
			CS_LAYER_11_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_11_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[11] = CS_LAYER_11_COLOUR;
			CSlayerColourProjectNameArray[11] = CS_LAYER_11_COLOUR_PROJECT_NAME;			
		}
		else if(currentReferenceRulesClass->name == CS_LAYER_12_COLOUR_NAME)
		{
			CS_LAYER_12_COLOUR = currentReferenceRulesClass->fractionalValue;
			CS_LAYER_12_COLOUR_PROJECT_NAME = currentReferenceRulesClass->stringValue;
			CSlayerColourArray[12] = CS_LAYER_12_COLOUR;
			CSlayerColourProjectNameArray[12] = CS_LAYER_12_COLOUR_PROJECT_NAME;			
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

void initiateMaxXatParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_INCLUDE_DEPTH; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}






Reference * createFileReferenceListBoxes(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList, XMLparserTag ** currentTag, bool outputFunctionsConnectivity, bool traceFunctionUpwards)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	CSfileReference * currentFileReference = firstReferenceInAboveLevelBelowList;

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
		bool aPreviousReferenceWithThisNameHasbeenPrinted = hasPreviousReferenceWithThisNameHasbeenPrinted(currentFileReference->name, firstReferenceInTopLevelBelowList, &maxYPos);
		int xPos = maxXAtAParticularY[maxYPos];

		if(!aPreviousReferenceWithThisNameHasbeenPrinted)
		{//add box and connections

			currentFileReference->printXIndex = xPos;
			currentFileReference->printYIndex = maxYPos;

			double vectorObjectsScaleFactor;
			if(outputFunctionsConnectivity)
			{
				vectorObjectsScaleFactor = CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR;			
				double xOffset = 0.0;
				#ifdef CS_DRAW_APPLY_EVEN_ODD_X_OFFSET
				if((maxYPos % 2) == 0)
				{
					xOffset = (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X) / 2.0;
				}
				#endif
				currentFileReference->printX = xPos * (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X) + xOffset;
				currentFileReference->printY = (double)maxYPos * (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION);
			}
			else
			{
				vectorObjectsScaleFactor = CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR;			
				double xOffset = 0.0;
				#ifdef CS_DRAW_APPLY_EVEN_ODD_X_OFFSET
				if((maxYPos % 2) == 0)
				{
					xOffset = (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION) / 2.0;
				}
				#endif				
				currentFileReference->printX = xPos * (vectorObjectsScaleFactor * CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION) + xOffset;
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

				writeSVGbox(currentTag, &bigFileBoxPosition, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B, DAT_FILE_COLOUR_LIGHTGREY, CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG, false);
			}


			#ifdef CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES
			int colour = calculateCSBoxAndConnectionColourBasedUponLevel(maxYPos);
			#else
			int colour = calculateCSBoxAndConnectionColourBasedUponFileName(currentFileReference);
			#endif
			currentFileReference->col = colour;

				//print box

			#ifdef CS_WRITE_SVG_GROUPS
			XMLparserTag * nextTagOnOriginalLayer = NULL;
			#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif
				
				string groupID = createGroupID(currentFileReference->name, currentFileReference->printX, currentFileReference->printY);
				nextTagOnOriginalLayer = writeSVGgroup(currentTag, &groupID);
			#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif	
			#endif	
						
			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif
				vec pos;
				pos.x = currentFileReference->printX;
				pos.y = currentFileReference->printY;			
				pos.z = currentFileReference->printY;	
				newCurrentReferenceInPrintList = createFileOrFunctionReferenceBox(newCurrentReferenceInPrintList, &pos, &(currentFileReference->name), vectorObjectsScaleFactor, colour, CS_FILE_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FILE_BOX);	//add box

				writeFileOrFunctionSVGbox(currentTag, &pos, currentFileReference->name.length(), vectorObjectsScaleFactor, CS_FILE_MAX_TEXT_LENGTH, colour, CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG);
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
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif

				newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&(currentFileReference->name), newCurrentReferenceInPrintList, &position, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, vectorObjectsScaleFactor/CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR);	//add sprite text within box	//

				position.y = position.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*vectorObjectsScaleFactor;
				writeSVGtext(currentTag, (currentFileReference->name), &position, vectorObjectsScaleFactor/15, DAT_FILE_COLOUR_BLACK);
			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif

			#ifdef CS_WRITE_SVG_GROUPS
			#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif
				(*currentTag) = nextTagOnOriginalLayer;
			#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif	
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
			newCurrentReferenceInPrintList = createFileReferenceListBoxes(newCurrentReferenceInPrintList, currentFileReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList, currentTag, outputFunctionsConnectivity, traceFunctionUpwards);
		}

		currentFileReference = currentFileReference->next;

	}

	return newCurrentReferenceInPrintList;
}

bool hasPreviousReferenceWithThisNameHasbeenPrinted(string name, CSfileReference * firstReferenceInAboveLevelBelowList, int *maxYPos)
{
	bool aPreviousReferenceWithThisNameHasbeenPrinted = false;

	CSfileReference * currentReference = firstReferenceInAboveLevelBelowList;

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
			if(hasPreviousReferenceWithThisNameHasbeenPrinted(name, currentReference->firstReferenceInBelowList, maxYPos))
			{
				aPreviousReferenceWithThisNameHasbeenPrinted = true;
			}
		}

		currentReference = currentReference->next;
	}

	return aPreviousReferenceWithThisNameHasbeenPrinted;
}

void addShortcutsToPrintedVersionOfReference(CSfileReference * reference, string name, CSfileReference * firstReferenceInAboveLevelBelowList)
{
	CSfileReference * currentReference = firstReferenceInAboveLevelBelowList;

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













Reference * createFileReferenceListConnections(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList, XMLparserTag ** currentTag, bool traceFunctionUpwards)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSfileReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{

		//print connections
		if(currentReference->printed)
		{//only create connections to printed boxes
			newCurrentReferenceInPrintList = findAndLinkAllFileReferencesWithSameName(newCurrentReferenceInPrintList, currentReference, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, currentTag, traceFunctionUpwards);
		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			newCurrentReferenceInPrintList = createFileReferenceListConnections(newCurrentReferenceInPrintList, currentReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList, currentTag, traceFunctionUpwards);
		}

		currentReference = currentReference->next;

	}

	return newCurrentReferenceInPrintList;
}




Reference * findAndLinkAllFileReferencesWithSameName(Reference * currentReferenceInPrintList, CSfileReference * reference, CSfileReference * firstReferenceInAboveLevelBelowList, CSfileReference * firstReferenceInTopLevelBelowList, XMLparserTag ** currentTag, bool traceFunctionUpwards)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSfileReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		if(currentReference->name == reference->name)
		{
			//if(currentReference->printed)
			//{//only create connections to printed boxes

			if(currentReference->firstReferenceInAboveList != NULL)
			{
				//cout << "currentReference->firstReferenceInAboveList->name = " << currentReference->firstReferenceInAboveList->name << endl;
				CSfileReference * currentReferenceFirstReferenceInAboveListPrinted = NULL;
				string currentReferenceFirstReferenceInAboveListName = currentReference->firstReferenceInAboveList->name;
				bool foundPrintedReferenceWithName = false;
				currentReferenceFirstReferenceInAboveListPrinted = findPrintedFileReferenceWithName(currentReferenceFirstReferenceInAboveListName, currentReferenceFirstReferenceInAboveListPrinted, firstReferenceInTopLevelBelowList, &foundPrintedReferenceWithName);

				if(foundPrintedReferenceWithName)
				{
					//cout << "h" << endl;
					//cout << "currentReferenceFirstReferenceInAboveListPrinted->name = " << currentReferenceFirstReferenceInAboveListPrinted->name << endl;
					#ifdef CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES
					int colour = calculateCSBoxAndConnectionColourBasedUponLevel(currentReferenceFirstReferenceInAboveListPrinted->printYIndex);
					#else
					int colour = calculateCSBoxAndConnectionColourBasedUponFileName(currentReferenceFirstReferenceInAboveListPrinted);					
					#endif
					
					newCurrentReferenceInPrintList = createFileReferenceConnection(newCurrentReferenceInPrintList, reference, currentReferenceFirstReferenceInAboveListPrinted, colour, traceFunctionUpwards, currentTag);	//add line
				}
			}
			//}

		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{
			newCurrentReferenceInPrintList = findAndLinkAllFileReferencesWithSameName(newCurrentReferenceInPrintList, reference, currentReference->firstReferenceInBelowList, firstReferenceInTopLevelBelowList, currentTag, traceFunctionUpwards);
		}

		currentReference = currentReference->next;
	}

	return newCurrentReferenceInPrintList;
}

CSfileReference * findPrintedFileReferenceWithName(string name, CSfileReference * reference, CSfileReference * firstReferenceInAboveLevelBelowList, bool * foundPrintedReferenceWithName)
{
	CSfileReference * updatedReference = reference;
	CSfileReference * currentReference = firstReferenceInAboveLevelBelowList;

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
























Reference * createFunctionReferenceListBoxesAndConnections(Reference * currentReferenceInPrintList, CSfileReference * aboveLevelFileReference, CSfunctionReference * aboveLevelFunctionReference, CSfileReference * firstReferenceInTopLevelBelowList, int functionLevel, string functionReferenceNameToFind, XMLparserTag ** currentTag, bool traceFunctionUpwards, bool useSingleFileOnly, string * singleFileName)
{
	int newFunctionLevel = functionLevel;

	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSreferenceArray newFoundFileAndFunctionReference;

	bool referenceFound = false;

	CSfunctionReference * functionReference = NULL;
	CSfileReference * fileReference = NULL;

	newFoundFileAndFunctionReference = findFunctionReferenceInAPrintedFileReferenceRecursive(aboveLevelFileReference, functionReferenceNameToFind, &newFoundFileAndFunctionReference, &referenceFound, useSingleFileOnly, singleFileName);
	if(referenceFound)
	{
		fileReference = newFoundFileAndFunctionReference.csref1;

		functionReference = newFoundFileAndFunctionReference.csref2;

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

		bool prepareForTrace = false;
		bool printedPassSingleFileOnly = false;
		if(useSingleFileOnly)
		{
			//cout << "*singleFileName = " << *singleFileName << endl;

			if(fileReference->name == *singleFileName)
			{
				if(functionReference->printed)
				{				
					printedPassSingleFileOnly = true;
				}
			}
		}
		else
		{
			prepareForTrace = true;
			printedPassSingleFileOnly = true;
		}
		if(printedPassSingleFileOnly)
		{	
			//cout << "\tprintedPassSingleFileOnly" << endl;	
			bool printedPass = false;
			if(useSingleFileOnly)
			{
				if(functionReference->singleFileOnlyPrinted == false)
				{				
					printedPass = true;
					functionReference->singleFileOnlyPrinted = true;
				}	
			}
			else
			{
				if(functionReference->printed == false)
				{
					printedPass = true;
					functionReference->printed = true;			
				}
			}
			
			
			vec positionOriginalSingleFileOnly;
			vec positionAboveLevelOriginalSingleFileOnly;	
			if(useSingleFileOnly)
			{
				/*	
				cout << "fileReference->name = " << fileReference->name << endl;
				cout << "\t functionReference->name = " << functionReference->name << endl;	
				cout << "\t aboveLevelFunctionReference->name = " << aboveLevelFunctionReference->name << endl;	
				cout << "\t\t fileReference->printX = " << fileReference->printX << endl;	
				cout << "\t\t fileReference->printY = " << fileReference->printY << endl;
				cout << "\t\t aboveLevelFileReference->printX = " << aboveLevelFileReference->printX << endl;	
				cout << "\t\t aboveLevelFileReference->printY = " << aboveLevelFileReference->printY << endl;				
				cout << "\t\t functionReference->filePrintXrecordForSingleFileOnly = " << functionReference->filePrintXrecordForSingleFileOnly << endl;	
				cout << "\t\t functionReference->filePrintYrecordForSingleFileOnly = " << functionReference->filePrintYrecordForSingleFileOnly << endl;
				cout << "\t\t aboveLevelFunctionReference->filePrintXrecordForSingleFileOnly = " << aboveLevelFunctionReference->filePrintXrecordForSingleFileOnly << endl;	
				cout << "\t\t aboveLevelFunctionReference->filePrintYrecordForSingleFileOnly = " << aboveLevelFunctionReference->filePrintYrecordForSingleFileOnly << endl;
				*/
											
				positionOriginalSingleFileOnly.x = functionReference->printX;
				positionOriginalSingleFileOnly.y = functionReference->printY;				
				positionAboveLevelOriginalSingleFileOnly.x = aboveLevelFunctionReference->printX;
				positionAboveLevelOriginalSingleFileOnly.y = aboveLevelFunctionReference->printY;	
				
				functionReference->printX = functionReference->printX - fileReference->printX;
				functionReference->printY = functionReference->printY - fileReference->printY;
				aboveLevelFunctionReference->printX = aboveLevelFunctionReference->printX - fileReference->printX;
				aboveLevelFunctionReference->printY = aboveLevelFunctionReference->printY - fileReference->printY;						      
			}
							
			if(printedPass)
			{
				//cout << "\tprintedPass" << endl;			
				int aboveConnectionColour;
				int colour = functionReference->col;
				if(fileReference == aboveLevelFileReference)
				{//function found in existing file

				}
				else
				{//function found in a new file
					newFunctionLevel = 0;
				}	

				if(!useSingleFileOnly)
				{

					//now print function;
					functionReference->printXIndex = (fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel]);
					functionReference->printYIndex = newFunctionLevel;

					double xOffset = 0.0;
					#ifdef CS_DRAW_APPLY_EVEN_ODD_X_OFFSET
					if((newFunctionLevel % 2) == 0)
					{
						xOffset = CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/0.5 / 2.0;
					}
					#endif	
					functionReference->printX = fileReference->printX + (fileReference->maxFunctionPrintXAtAParticularY[newFunctionLevel])*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/0.5 + xOffset - (CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR*0.5 * CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X * 0.5);
					functionReference->printY = fileReference->printY + (1+newFunctionLevel)*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
					functionReference->filePrintXrecordForSingleFileOnly = fileReference->printX;
					functionReference->filePrintYrecordForSingleFileOnly = fileReference->printY;
					
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
					aboveConnectionColour = aboveLevelFunctionReference->col;	//or =calculateCSBoxAndConnectionColourBasedUponLevel(aboveLevelFunctionReference->printYIndex);	[same thing]
					colour = calculateCSBoxAndConnectionColourBasedUponLevel(functionReference->printYIndex);
					#else
						//new: use same colour as file box
						aboveConnectionColour = aboveLevelFunctionReference->col;		//OLDWRONG; int aboveConnectionColour = aboveLevelFunctionReference->col;	//OLDWRONG2; same as aboveLevelFileReference->colour;
						colour = fileReference->col;					//OLDWRONG; int colour = aboveLevelFileReference->col;
					#endif
					functionReference->col = colour;
				}
				else
				{
					//assume print coordinates already set
					aboveConnectionColour = aboveLevelFunctionReference->col;	
				}

				/*
				//DEBUG:
				if(useSingleFileOnly)
				{
					if(functionReference->name == "traceFunctionsUpwardsAndDrawOrHighLightTheseReset")
					{
						cout << "traceFunctionsUpwardsAndDrawOrHighLightTheseReset" << endl; 
						cout << "colour = " << colour << endl; 
						cout << "aboveConnectionColour = " << aboveConnectionColour << endl;		       
						cout << "functionReference->printX = " << functionReference->printX << endl; 
						cout << "functionReference->printY = " << functionReference->printY << endl;
						cout << "aboveLevelFunctionReference->printX = " << aboveLevelFunctionReference->printX << endl; 
						cout << "aboveLevelFunctionReference->printY = " << aboveLevelFunctionReference->printY << endl;
						cout << "fileReference->name = " << fileReference->name << endl;
						cout << "\t functionReference->name = " << functionReference->name << endl;	
						cout << "\t aboveLevelFunctionReference->name = " << aboveLevelFunctionReference->name << endl;	
						cout << "\t\t fileReference->printX = " << fileReference->printX << endl;	
						cout << "\t\t fileReference->printY = " << fileReference->printY << endl;
						cout << "\t\t aboveLevelFileReference->printX = " << aboveLevelFileReference->printX << endl;	
						cout << "\t\t aboveLevelFileReference->printY = " << aboveLevelFileReference->printY << endl;				
						cout << "\t\t functionReference->filePrintXrecordForSingleFileOnly = " << functionReference->filePrintXrecordForSingleFileOnly << endl;	
						cout << "\t\t functionReference->filePrintYrecordForSingleFileOnly = " << functionReference->filePrintYrecordForSingleFileOnly << endl;
						cout << "\t\t aboveLevelFunctionReference->filePrintXrecordForSingleFileOnly = " << aboveLevelFunctionReference->filePrintXrecordForSingleFileOnly << endl;	
						cout << "\t\t aboveLevelFunctionReference->filePrintYrecordForSingleFileOnly = " << aboveLevelFunctionReference->filePrintYrecordForSingleFileOnly << endl;						
					}
				}
				*/

				//print function connections;
				newCurrentReferenceInPrintList = createFunctionReferenceConnection(newCurrentReferenceInPrintList, functionReference, aboveLevelFunctionReference, aboveConnectionColour, traceFunctionUpwards, prepareForTrace, currentTag, fileReference, aboveLevelFileReference);
						
			
				//apply hack
				if(useSingleFileOnly)
				{				
					if(functionLevel == 0)
					{      
						aboveLevelFunctionReference->printX = aboveLevelFunctionReference->printX + fileReference->printX;
						aboveLevelFunctionReference->printY = aboveLevelFunctionReference->printY + fileReference->printY;						      
					}
				}
				
				#ifdef CS_WRITE_SVG_GROUPS
				XMLparserTag * nextTagOnOriginalLayer = NULL;
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				if(!traceFunctionUpwards)
				{//only print connections when not tracing a bottom level function upwards - saves space
				#endif
					string groupID = createGroupID(fileReference->name, functionReference->printX, functionReference->printY);	//OLD: functionReference->name
					nextTagOnOriginalLayer = writeSVGgroup(currentTag, &groupID);
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				}
				#endif	
				#endif	
								
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				if(!traceFunctionUpwards)
				{//only print connections when not tracing a bottom level function upwards - saves space
				#endif
					//print function box
					vec pos;
					pos.x = functionReference->printX;
					pos.y = functionReference->printY;			
					newCurrentReferenceInPrintList = createFileOrFunctionReferenceBox(newCurrentReferenceInPrintList, &pos, &(functionReference->name), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, colour, CS_FUNCTION_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FUNCTION_BOX);	//add box

					writeFileOrFunctionSVGbox(currentTag, &pos, functionReference->name.length(), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, CS_FUNCTION_MAX_TEXT_LENGTH, colour, CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG);
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				}
				#endif
				
				//print box text
				int numSpritesAdded;
				vec positionText;
				
				#ifdef CS_CENTRE_BOXES
				positionText.x = functionReference->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*(functionReference->name.length()/(CS_FUNCTION_MAX_TEXT_LENGTH*CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH));
				#else
				positionText.x = functionReference->printX;
				#endif
				positionText.y = functionReference->printY;
				positionText.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;
				if(!useSingleFileOnly)
				{
					functionReference->printTextX = positionText.x;
					functionReference->printTextY = positionText.y;
				}

				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				if(!traceFunctionUpwards)
				{//only print connections when not tracing a bottom level function upwards - saves space
				#endif
					newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&(functionReference->name), newCurrentReferenceInPrintList, &positionText, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR);	//add sprite text within box

						positionText.y = positionText.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
						writeSVGtext(currentTag, (functionReference->name), &positionText, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B, DAT_FILE_COLOUR_BLACK);

				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				}
				#endif
				
				
				#ifdef CS_WRITE_SVG_GROUPS
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				if(!traceFunctionUpwards)
				{//only print connections when not tracing a bottom level function upwards - saves space
				#endif
					(*currentTag) = nextTagOnOriginalLayer;
				#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
				}
				#endif	
				#endif				

				//apply hack
				if(useSingleFileOnly)
				{				
					if(functionLevel == 0)
					{      
						aboveLevelFunctionReference->printX = aboveLevelFunctionReference->printX - fileReference->printX;
						aboveLevelFunctionReference->printY = aboveLevelFunctionReference->printY - fileReference->printY;						      
					}
				}
				
				if(useSingleFileOnly)
				{							
					functionReference->printX = positionOriginalSingleFileOnly.x;
					functionReference->printY = positionOriginalSingleFileOnly.y;	
					aboveLevelFunctionReference->printX = positionAboveLevelOriginalSingleFileOnly.x;
					aboveLevelFunctionReference->printY = positionAboveLevelOriginalSingleFileOnly.y;		
				}
				
				//functionReference->printed = true;	//has been moved;

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
				CSfunctionReference * currentfunctionReferenceReference = functionReference->firstReferenceInFunctionReferenceList;
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
					newCurrentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(newCurrentReferenceInPrintList, fileReference, functionReference, firstReferenceInTopLevelBelowList, (newFunctionLevel+1), currentfunctionReferenceReference->name, currentTag, traceFunctionUpwards, useSingleFileOnly, singleFileName);

					currentfunctionReferenceReference = currentfunctionReferenceReference->next;
				}			
			}
			else
			{	
				//print function connections;
				newCurrentReferenceInPrintList = createFunctionReferenceConnection(newCurrentReferenceInPrintList, functionReference, aboveLevelFunctionReference, aboveLevelFunctionReference->col, traceFunctionUpwards, prepareForTrace, currentTag, fileReference, aboveLevelFileReference);
			
				if(useSingleFileOnly)
				{			
					functionReference->printX = positionOriginalSingleFileOnly.x;
					functionReference->printY = positionOriginalSingleFileOnly.y;	
					aboveLevelFunctionReference->printX = positionAboveLevelOriginalSingleFileOnly.x;
					aboveLevelFunctionReference->printY = positionAboveLevelOriginalSingleFileOnly.y;							
				}			
			}
		}
	}
	else
	{
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED		
		if(!useSingleFileOnly)
		{
		#endif
			cout << "error; function reference not found in file or include files" << endl;
			cout << "aboveLevelFileReference->name = " << aboveLevelFileReference->name << endl;
			cout << "aboveLevelFunctionReference->name = " << aboveLevelFunctionReference->name << endl;
			exit(0);
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED					
		}
		#endif
	}
	
	return newCurrentReferenceInPrintList;

}

CSreferenceArray findFunctionReferenceInAPrintedFileReferenceRecursive(CSfileReference * fileReferenceToSearchIn, string functionReferenceNameToFind, CSreferenceArray * foundFileAndFunctionReference, bool * referenceFound, bool useSingleFileOnly, string * singleFileName)
{
	CSreferenceArray newFoundFileAndFunctionReference;
	newFoundFileAndFunctionReference.csref1 = foundFileAndFunctionReference->csref1;
	newFoundFileAndFunctionReference.csref2 = foundFileAndFunctionReference->csref2;

	//search current file;
	newFoundFileAndFunctionReference = findFunctionReferenceInPrintedFileReference(fileReferenceToSearchIn, functionReferenceNameToFind, &newFoundFileAndFunctionReference, referenceFound, useSingleFileOnly, singleFileName);

	//search include files;
	if(fileReferenceToSearchIn->firstReferenceInBelowList != NULL)
	{
		CSfileReference * currentReference = fileReferenceToSearchIn->firstReferenceInBelowList;
		while(currentReference->next != NULL)
		{
			newFoundFileAndFunctionReference = findFunctionReferenceInAPrintedFileReferenceRecursive(currentReference, functionReferenceNameToFind, &newFoundFileAndFunctionReference, referenceFound, useSingleFileOnly, singleFileName);

			currentReference = currentReference->next;
		}
	}

	return newFoundFileAndFunctionReference;
}

CSreferenceArray findFunctionReferenceInPrintedFileReference(CSfileReference * fileReferenceToSearchIn, string functionReferenceNameToFind, CSreferenceArray * foundFileAndFunctionReference, bool * referenceFound, bool useSingleFileOnly, string * singleFileName)
{
	CSreferenceArray newFoundFileAndFunctionReference;
	newFoundFileAndFunctionReference.csref1 = foundFileAndFunctionReference->csref1;
	newFoundFileAndFunctionReference.csref2 = foundFileAndFunctionReference->csref2;

	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED
	bool filePassedSingleFileOnly = false;
	if(useSingleFileOnly)
	{
		if(fileReferenceToSearchIn->name == *singleFileName)
		{
			filePassedSingleFileOnly = true;
		}
	}
	else
	{
		filePassedSingleFileOnly = true;
	}
	if(filePassedSingleFileOnly)
	{
	#endif	
		CSfileReference * currentReferenceEnsuredPrinted = NULL;
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

		CSfunctionReference * currentFunctionReference = currentReferenceEnsuredPrinted->firstReferenceInFunctionList;
		while(currentFunctionReference->next != NULL)
		{
			if(currentFunctionReference->name == functionReferenceNameToFind)
			{
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED
				bool functionPassedSingleFileOnly = false;
				if(useSingleFileOnly)
				{
					if(currentFunctionReference->printed)
					{
						functionPassedSingleFileOnly = true;
					}
				}
				else
				{
					functionPassedSingleFileOnly = true;
				}

				if(functionPassedSingleFileOnly)
				{
				#endif
					/*
					cout << "match found:" << endl;
					cout << "currentFunctionReference->name = " << currentFunctionReference->name << endl;
					cout << "functionReferenceNameToFind = " << functionReferenceNameToFind << endl;
					cout << "currentReferenceEnsuredPrinted->name = " << currentReferenceEnsuredPrinted->name << endl;
					*/
					newFoundFileAndFunctionReference.csref1 = currentReferenceEnsuredPrinted;
					newFoundFileAndFunctionReference.csref2 = currentFunctionReference;
					*referenceFound = true;
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED
				}
				#endif
			}
			currentFunctionReference = currentFunctionReference->next;
		}
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED
	}
	#endif
	return newFoundFileAndFunctionReference;
}





















Reference * createFileOrFunctionReferenceBox(Reference * currentReferenceInPrintList,  vec * referencePrintPos, string * referenceName, double scaleFactor, int colour, double maxTextLength, double zPosition)
{
	//cout << "zPosition = " << zPosition << endl;

	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	newCurrentReferenceInPrintList->type = REFERENCE_TYPE_QUAD;
	newCurrentReferenceInPrintList->colour = colour;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex1relativePosition.x = referencePrintPos->x - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*referenceName->length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex1relativePosition.x = referencePrintPos->x - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex1relativePosition.y = referencePrintPos->y + CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex1relativePosition.z = zPosition;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex2relativePosition.x = referencePrintPos->x + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*referenceName->length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex2relativePosition.x = referencePrintPos->x + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex2relativePosition.y = referencePrintPos->y + CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex2relativePosition.z = zPosition;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex3relativePosition.x = referencePrintPos->x + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*referenceName->length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex3relativePosition.x = referencePrintPos->x + CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex3relativePosition.y = referencePrintPos->y - CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex3relativePosition.z = zPosition;

#ifdef CS_CENTRE_BOXES
	newCurrentReferenceInPrintList->vertex4relativePosition.x = referencePrintPos->x - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor*referenceName->length()/maxTextLength;
#else
	newCurrentReferenceInPrintList->vertex4relativePosition.x = referencePrintPos->x - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*scaleFactor;
#endif
	newCurrentReferenceInPrintList->vertex4relativePosition.y = referencePrintPos->y - CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR*scaleFactor;
	newCurrentReferenceInPrintList->vertex4relativePosition.z = zPosition;

	/*
	cout << "createFileOrFunctionReferenceBox():" << endl;
	cout << "referenceName = " << *referenceName << endl;
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


Reference * createFileReferenceConnection(Reference * currentReferenceInPrintList, CSfileReference * reference,  CSfileReference * currentReferenceInAboveList, int colour, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag ** currentTag)
{
	//cout << "h1" << endl;
	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace
	CSfileReferenceContainer * currentReferenceContainerInAboveFileOrFunctionReferenceList = reference->firstReferenceContainerInAboveReferenceList;
	while(currentReferenceContainerInAboveFileOrFunctionReferenceList->next != NULL)
	{
		currentReferenceContainerInAboveFileOrFunctionReferenceList = currentReferenceContainerInAboveFileOrFunctionReferenceList->next;
	}
	currentReferenceContainerInAboveFileOrFunctionReferenceList->name = currentReferenceInAboveList->name;
	CSfileReferenceContainer * newCSRefContainer = new CSfileReferenceContainer();
	currentReferenceContainerInAboveFileOrFunctionReferenceList->next = newCSRefContainer;
	//cout << "h2" << endl;
	
	vec referencePrintPos;
	referencePrintPos.x = reference->printX;
	referencePrintPos.y = reference->printY;
	vec currentReferenceInAboveListPrintPos;
	currentReferenceInAboveListPrintPos.x = currentReferenceInAboveList->printX;
	currentReferenceInAboveListPrintPos.y = currentReferenceInAboveList->printY;
 	string startGroupID = createGroupID(reference->name, reference->printX, reference->printY);
 	string endGroupID = createGroupID(currentReferenceInAboveList->name, currentReferenceInAboveList->printX, currentReferenceInAboveList->printY);
 		
	return configureFileOrFunctionReferenceConnection(currentReferenceInPrintList, &referencePrintPos, &currentReferenceInAboveListPrintPos, colour, true, traceAFunctionUpwardsAndNotCurrentlyTracing, currentTag, &startGroupID, &endGroupID);		
}

Reference * createFunctionReferenceConnection(Reference * currentReferenceInPrintList, CSfunctionReference * reference,  CSfunctionReference * currentReferenceInAboveList, int colour, bool traceAFunctionUpwardsAndNotCurrentlyTracing, bool prepareForTrace, XMLparserTag ** currentTag, CSfileReference * fileReference,  CSfileReference * currentFileReferenceInAboveList)
{
	if(prepareForTrace)
	{
		//cout << "h1" << endl;
		//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace
		CSfunctionReferenceContainer * currentReferenceContainerInAboveFileOrFunctionReferenceList = reference->firstReferenceContainerInAboveReferenceList;
		while(currentReferenceContainerInAboveFileOrFunctionReferenceList->next != NULL)
		{
			currentReferenceContainerInAboveFileOrFunctionReferenceList = currentReferenceContainerInAboveFileOrFunctionReferenceList->next;
		}
		currentReferenceContainerInAboveFileOrFunctionReferenceList->name = currentReferenceInAboveList->name;
		CSfunctionReferenceContainer * newCSRefContainer = new CSfunctionReferenceContainer();
		currentReferenceContainerInAboveFileOrFunctionReferenceList->next = newCSRefContainer;
		//cout << "h2" << endl;
	}
	
	vec referencePrintPos;
	referencePrintPos.x = reference->printX;
	referencePrintPos.y = reference->printY;
	vec currentReferenceInAboveListPrintPos;
	currentReferenceInAboveListPrintPos.x = currentReferenceInAboveList->printX;
	currentReferenceInAboveListPrintPos.y = currentReferenceInAboveList->printY;
 	string startGroupID = createGroupID(fileReference->name, reference->printX, reference->printY);	//OLD: reference->name
 	string endGroupID = createGroupID(currentFileReferenceInAboveList->name, currentReferenceInAboveList->printX, currentReferenceInAboveList->printY);	//OLD: currentReferenceInAboveList->name

	return configureFileOrFunctionReferenceConnection(currentReferenceInPrintList, &referencePrintPos, &currentReferenceInAboveListPrintPos, colour, false, traceAFunctionUpwardsAndNotCurrentlyTracing, currentTag, &startGroupID, &endGroupID);		
}




Reference * configureFileOrFunctionReferenceConnection(Reference * currentReferenceInPrintList, vec * referencePrintPos,  vec * currentReferenceInAboveListPrintPos, int colour, bool fileOrFunction, bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag ** currentTag, string * startGroupID, string * endGroupID)
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

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

		newCurrentReferenceInPrintList->vertex1relativePosition.x = currentReferenceInAboveListPrintPos->x;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = currentReferenceInAboveListPrintPos->y;
		if(fileOrFunction)
		{
			newCurrentReferenceInPrintList->vertex1relativePosition.z = CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS;	//why doesnt this work
		}
		else
		{
			newCurrentReferenceInPrintList->vertex1relativePosition.z = CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS;	//why doesnt this work
		}

		newCurrentReferenceInPrintList->vertex2relativePosition.x = referencePrintPos->x;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = referencePrintPos->y;
		if(fileOrFunction)
		{
			newCurrentReferenceInPrintList->vertex2relativePosition.z = CS_OUTPUT_Z_POSITION_FILE_CONNECTIONS;	//why doesnt this work
		}
		else
		{
			newCurrentReferenceInPrintList->vertex2relativePosition.z = CS_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS;	//why doesnt this work
		}
		/*
		cout << "configureFileOrFunctionReferenceConnection():" << endl;
		cout << "currentReferenceInAboveList->name = " << currentReferenceInAboveList->name << endl;
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
		pos1.x = referencePrintPos->x;
		pos1.y = referencePrintPos->y;
		vec pos2;
		pos2.x = currentReferenceInAboveListPrintPos->x;
		pos2.y = currentReferenceInAboveListPrintPos->y;
		
		#ifdef CS_WRITE_SVG_INKSCAPE_CONNECTORS
		double strokeWidth;
		if(fileOrFunction)
		{	
			strokeWidth = CS_FILE_CONNECTOR_STROKE_WIDTH_SVG;
		}
		else
		{
			strokeWidth = CS_FUNCTION_CONNECTOR_STROKE_WIDTH_SVG;
		}
		
		writeSVGconnector(currentTag, &pos1, &pos2, colour, startGroupID, endGroupID, strokeWidth);		
		#else
		writeSVGline(currentTag, &pos1, &pos2, colour);
		#endif


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

int calculateCSBoxAndConnectionColourBasedUponFileName(CSfileReference * currentFileReference)
{
	string fileName = currentFileReference->name;
	int colour;
	bool foundProjectAbbreviation = false;
	
	for(int i=0; i<CS_LAYER_NUMBER_OF_LAYERS ; i++)
	{
		if(CSlayerColourProjectNameArray[i] != "")
		{
			int positionOfProjectAbbreviation = fileName.find(CSlayerColourProjectNameArray[i]);
			if(positionOfProjectAbbreviation == 0)
			{
				foundProjectAbbreviation = true;
				colour = CSlayerColourArray[i];
			}
		}
	}
	if(!foundProjectAbbreviation)
	{
		colour = DAT_FILE_COLOUR_DARKRED;
	}

	return colour;

}



















CSfunctionReference * findPrintedFunctionReferenceWithName(string name, CSfunctionReference * reference, CSfileReference * firstReferenceInAboveLevelBelowList, bool * foundPrintedReferenceWithName, string * fileNameHoldingFunction)
{
	CSfunctionReference * updatedFunctionReference = reference;
	CSfileReference * currentFileReference = firstReferenceInAboveLevelBelowList;

	while(currentFileReference->next != NULL)
	{
		CSfunctionReference * currentFunctionReference = currentFileReference->firstReferenceInFunctionList;
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

Reference * traceFunctionsUpwardsAndDrawOrHighLightThese(Reference * currentReferenceInPrintList, CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * currentFunctionBeingTraced, XMLparserTag ** currentTag, int generateHTMLdocumentationMode, string * HTMLdocumentationFunctionTraceTableRows)
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
	vec pos;
	pos.x = currentFunctionBeingTraced->printX;
	pos.y = currentFunctionBeingTraced->printY;
	newCurrentReferenceInPrintList = createFileOrFunctionReferenceBox(newCurrentReferenceInPrintList, &pos, &(currentFunctionBeingTraced->name), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, functionBoxColour, CS_FUNCTION_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX);	//add box

	writeFileOrFunctionSVGboxTransparent(currentTag, &pos, currentFunctionBeingTraced->name.length(), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, CS_FUNCTION_MAX_TEXT_LENGTH, functionBoxColour, CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG, 0.5);

	//print function text;
	#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		int numSpritesAdded;	//not used
		vec textPosition;
		textPosition.x = currentFunctionBeingTraced->printTextX;
		textPosition.y = currentFunctionBeingTraced->printTextY;
		textPosition.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

		newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&(currentFunctionBeingTraced->name), newCurrentReferenceInPrintList, &textPosition, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/(200.0));	//add sprite text within box

		textPosition.y = textPosition.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
		writeSVGtext(currentTag, (currentFunctionBeingTraced->name), &textPosition, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B, DAT_FILE_COLOUR_BLACK);

	#endif

	currentFunctionBeingTraced->printedTrace = true;
	currentFunctionBeingTraced->printedTraceReset = false;

	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace

	CSfunctionReferenceContainer * currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList = currentFunctionBeingTraced->firstReferenceContainerInAboveReferenceList;
	while(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->next != NULL)	// && (currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name != "main")
	{
		//cout << "currentFunctionBeingTraced->name = " << currentFunctionBeingTraced->name << endl;
		//cout << "\tcurrentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name = " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name << endl;
	
		bool higherLevelFunctionFound = false;
		string fileNameHoldingFunction = "";
		CSfunctionReference * higherLevelFunction = findPrintedFunctionReferenceWithName(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name, NULL, firstReferenceInTopLevelBelowList, &higherLevelFunctionFound, &fileNameHoldingFunction);

		if(higherLevelFunctionFound)
		{
			if(higherLevelFunction->printedTrace == false)
			{
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
				
				CSfileReference fileReferenceUnkown;
				fileReferenceUnkown.name = "";
				CSfileReference fileReferenceInAboveLevelListUnkown;
				fileReferenceInAboveLevelListUnkown.name = "";
				newCurrentReferenceInPrintList = createFunctionReferenceConnection(newCurrentReferenceInPrintList, currentFunctionBeingTraced, higherLevelFunction, functionConnectionColour, false, false, currentTag, &fileReferenceUnkown, &fileReferenceInAboveLevelListUnkown);

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

				newCurrentReferenceInPrintList = traceFunctionsUpwardsAndDrawOrHighLightThese(newCurrentReferenceInPrintList, firstReferenceInTopLevelBelowList, higherLevelFunction, currentTag, generateHTMLdocumentationMode, HTMLdocumentationFunctionTraceTableRows);
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



void traceFunctionsUpwardsAndDrawOrHighLightTheseReset(CSfileReference * firstReferenceInTopLevelBelowList, CSfunctionReference * currentFunctionBeingTraced)
{
	currentFunctionBeingTraced->printedTrace = false;
	currentFunctionBeingTraced->printedTraceReset = true;

	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace

	CSfunctionReferenceContainer * currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList = currentFunctionBeingTraced->firstReferenceContainerInAboveReferenceList;
	while(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->next != NULL)	// && (currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name != "main")
	{
		//cout << "currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name = " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name << endl;

		bool higherLevelFunctionFound = false;
		string fileNameHoldingFunction = "";
		CSfunctionReference * higherLevelFunction = findPrintedFunctionReferenceWithName(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionReferenceList->name, NULL, firstReferenceInTopLevelBelowList, &higherLevelFunctionFound, &fileNameHoldingFunction);

		if(higherLevelFunctionFound)
		{
			if(higherLevelFunction->printedTraceReset == false)
			{
				traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstReferenceInTopLevelBelowList, higherLevelFunction);
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




void writeFileOrFunctionSVGbox(XMLparserTag ** currentTag, vec * pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth)
{
#ifdef CS_CENTRE_BOXES
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor*(double)textLength/maxTextLength;
#else
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor;
#endif
	double height = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG*scaleFactor;

	writeSVGbox(currentTag,  pos, width, height, col, boxOutlineWidth, false);
}

void writeFileOrFunctionSVGboxTransparent(XMLparserTag ** currentTag, vec * pos, int textLength, double scaleFactor, double maxTextLength, int col, double boxOutlineWidth, double fillOpacity)
{
#ifdef CS_CENTRE_BOXES
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor*(double)textLength/maxTextLength;
#else
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor;
#endif
	double height = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG*scaleFactor;

	writeSVGboxTransparent(currentTag,  pos, width, height, col, boxOutlineWidth, false, fillOpacity);
}

string createGroupID(string objectName, int printX, int printY)
{
	string groupID = "";
	groupID = groupID + objectName + intToString(printX) + intToString(printY);
	//groupID = groupID + intToString(printX) + intToString(printY);
	return groupID;
}

string intToString(int integer)
{
	char stringCharStar[100];
	sprintf(stringCharStar, "%d", integer);
	return string(stringCharStar);
}


