/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: CSdraw.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3l1a 02-June-2017
 *
 *******************************************************************************/

//issues, need to flip vertically


#include "CSdraw.hpp"




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



void CSdrawClass::fillInCSrulesExternVariables()
{
	//extract common sprite variables from either xml file (LRRC or ANN)

	XMLrulesClass* currentReferenceRulesClass = CSrulesDraw;

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




int maxXAtAParticularY[MAX_INCLUDE_DEPTH_FILE];

void CSdrawClass::initiateMaxXatParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_INCLUDE_DEPTH_FILE; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}






LDreference* CSdrawClass::createFileObjectListBoxes(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer, XMLparserTag** currentTag, bool outputFunctionsConnectivity, bool traceFunctionUpwards, XMLparserTag* firstTagInGridTag, bool usePredefinedGrid)
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;

		/*
		if(currentFileObjectContainer->name != "")
		{
			for(int i= 0; i<currentFileObject->level; i++)
			{
				cout << "\t";
			}
			cout << "currentFileObject->name = " << currentFileObject->name << endl;
		}
		*/


		int maxYPos = 0;	//always print boxes with max Y pos...
		int xPos = maxXAtAParticularY[maxYPos];

		if(!(currentFileObject->printed))
		{//add box and connections

			#ifdef CS_SUPPORT_PREDEFINED_GRID
			if(usePredefinedGrid)
			{
				bool foundGridCoords = false;
				XMLparserTag* currentFileTag = firstTagInGridTag;
				while(currentFileTag->nextTag != NULL)
				{
					if(currentFileTag->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
					{
						if(currentFileTag->firstAttribute->value == currentFileObject->name)
						{
							XMLparserAttribute* currentAttributeTag = currentFileTag->firstAttribute;
							foundGridCoords = true;
							while(currentAttributeTag->nextAttribute != NULL)
							{
								if(currentAttributeTag->name == RULES_XML_ATTRIBUTE_x)
								{
									xPos = SHAREDvars.convertStringToInt(currentAttributeTag->value);
								}
								if(currentAttributeTag->name == RULES_XML_ATTRIBUTE_y)
								{
									maxYPos = SHAREDvars.convertStringToInt(currentAttributeTag->value);
								}
								currentAttributeTag = currentAttributeTag->nextAttribute;
							}
						}
					}
					currentFileTag = currentFileTag->nextTag;
				}
				if(!foundGridCoords)
				{
					cout << "error: !foundGridCoords: currentFileObject->name = " << currentFileObject->name << endl;
				}
			}
			#endif

			currentFileObject->printXIndex = xPos;
			currentFileObject->printYIndex = maxYPos;

			double vectorObjectsScaleFactor;
			if(outputFunctionsConnectivity)
			{
				vectorObjectsScaleFactor = CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR;
				double xOffset = 0.0;
				#ifdef CS_DRAW_APPLY_EVEN_ODD_X_OFFSET
				#ifdef CS_SUPPORT_PREDEFINED_GRID
				if(!usePredefinedGrid)
				{
				#endif
					if((maxYPos % 2) == 0)
					{
						xOffset = (vectorObjectsScaleFactor* CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION* CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X) / 2.0;
					}
				#ifdef CS_SUPPORT_PREDEFINED_GRID
				}
				#endif
				#endif
				currentFileObject->printX = xPos* (vectorObjectsScaleFactor* CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION* CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X) + xOffset;
				currentFileObject->printY = (double)maxYPos* (vectorObjectsScaleFactor* CS_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION);
			}
			else
			{
				vectorObjectsScaleFactor = CS_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR;
				double xOffset = 0.0;
				#ifdef CS_DRAW_APPLY_EVEN_ODD_X_OFFSET
				#ifdef CS_SUPPORT_PREDEFINED_GRID
				if(!usePredefinedGrid)
				{
				#endif
					if((maxYPos % 2) == 0)
					{
						xOffset = (vectorObjectsScaleFactor* CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION) / 2.0;
					}
				#ifdef CS_SUPPORT_PREDEFINED_GRID
				}
				#endif
				#endif
				currentFileObject->printX = xPos* (vectorObjectsScaleFactor* CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION) + xOffset;
				currentFileObject->printY = (vectorObjectsScaleFactor* CS_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION)* (double)maxYPos;
			}

			if(outputFunctionsConnectivity)
			{
				//print large box
				vec bigFileBoxPosition;
				bigFileBoxPosition.x = currentFileObject->printX;
				bigFileBoxPosition.z = CS_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX;
				bigFileBoxPosition.y = currentFileObject->printY + ((vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B)* CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C);
				newCurrentReferenceInPrintList = this->createBox(newCurrentReferenceInPrintList, &bigFileBoxPosition, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B* CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B, DAT_FILE_COLOUR_LIGHTGREY);	//add box

				LDsvg.writeSVGbox(currentTag, &bigFileBoxPosition, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B* CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X, vectorObjectsScaleFactor*CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B, DAT_FILE_COLOUR_LIGHTGREY, CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG, false);
			}


			#ifdef CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES
			int colour = this->calculateCSBoxAndConnectionColourBasedUponLevel(maxYPos);
			#else
			int colour = this->calculateCSBoxAndConnectionColourBasedUponFileName(currentFileObject);
			#endif
			currentFileObject->col = colour;

				//print box

			#ifdef CS_WRITE_SVG_GROUPS
			XMLparserTag* nextTagOnOriginalLayer = NULL;
			#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif

				string groupID = this->createGroupID(currentFileObject->name, currentFileObject->printX, currentFileObject->printY);
				nextTagOnOriginalLayer = LDsvg.writeSVGgroup(currentTag, &groupID);
			#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif
			#endif

			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif
				vec pos;
				pos.x = currentFileObject->printX;
				pos.y = currentFileObject->printY;
				pos.z = currentFileObject->printY;
				newCurrentReferenceInPrintList = this->createFileOrFunctionObjectBox(newCurrentReferenceInPrintList, &pos, &(currentFileObject->name), vectorObjectsScaleFactor, colour, CS_FILE_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FILE_BOX);	//add box

				this->writeFileOrFunctionSVGbox(currentTag, &pos, currentFileObject->name.length(), vectorObjectsScaleFactor, CS_FILE_MAX_TEXT_LENGTH, colour, CS_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG);
			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			}
			#endif



			//print box text
			int numSpritesAdded;
			vec position;

			#ifdef CS_CENTRE_BOXES
			position.x = currentFileObject->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*vectorObjectsScaleFactor*(currentFileObject->name.length()/(CS_FILE_MAX_TEXT_LENGTH*CS_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH));		//should actually be 25; CS_FILE_MAX_TEXT_LENGTH+5
			#else
			position.x = currentFileObject->printX;
			#endif
			position.y = currentFileObject->printY;
			position.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

			currentFileObject->printTextX = position.x;
			currentFileObject->printTextY = position.y;

			#ifdef CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
			if(!traceFunctionUpwards)
			{//only print connections when not tracing a bottom level function upwards - saves space
			#endif

				newCurrentReferenceInPrintList = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(currentFileObject->name, newCurrentReferenceInPrintList, &position, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, vectorObjectsScaleFactor/CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR);	//add sprite text within box	//

				position.y = position.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*vectorObjectsScaleFactor;
				LDsvg.writeSVGtext(currentTag, (currentFileObject->name), &position, vectorObjectsScaleFactor/15, DAT_FILE_COLOUR_BLACK);
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

			currentFileObject->printed = true;

			maxXAtAParticularY[maxYPos] = maxXAtAParticularY[maxYPos]+1;
			//xPos++;
		}

		if(currentFileObject->firstFileInBelowListContainer != NULL)
		{
			newCurrentReferenceInPrintList = this->createFileObjectListBoxes(newCurrentReferenceInPrintList, currentFileObject->firstFileInBelowListContainer, firstObjectInTopLevelBelowListContainer, currentTag, outputFunctionsConnectivity, traceFunctionUpwards, firstTagInGridTag, usePredefinedGrid);
		}

		currentFileObjectContainer = currentFileObjectContainer->next;

	}

	return newCurrentReferenceInPrintList;
}














LDreference* CSdrawClass::createFileObjectListConnections(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfile* aboveLevelObject, XMLparserTag** currentTag, bool traceFunctionUpwards)
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;
	CSfileContainer* currentReferenceContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentReferenceContainer->next != NULL)
	{
		CSfile* currentReference = currentReferenceContainer->fileObject;

		//print connections
		#ifdef CS_ASSUME_TOP_LEVEL_FILE_IS_SOURCE_FILE
		if(aboveLevelObject != NULL)
		{
		#endif
			if(currentReference->printed)
			{//only create connections to printed boxes
				#ifdef CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES
				int colour = this->calculateCSBoxAndConnectionColourBasedUponLevel(aboveLevelObject->printYIndex);
				#else
				int colour = this->calculateCSBoxAndConnectionColourBasedUponFileName(aboveLevelObject);
				#endif

				newCurrentReferenceInPrintList = this->createFileObjectConnection(newCurrentReferenceInPrintList, currentReference, aboveLevelObject, colour, traceFunctionUpwards, currentTag);	//add line
			}
		#ifdef CS_ASSUME_TOP_LEVEL_FILE_IS_SOURCE_FILE
		}
		#endif

		if(currentReference->firstFileInBelowListContainer != NULL)
		{
			if(!(currentReference->printedFileConnections))
			{
				newCurrentReferenceInPrintList = this->createFileObjectListConnections(newCurrentReferenceInPrintList, currentReference->firstFileInBelowListContainer, currentReference, currentTag, traceFunctionUpwards);
			}
		}

		currentReferenceContainer = currentReferenceContainer->next;
	}

	#ifdef CS_ASSUME_TOP_LEVEL_FILE_IS_SOURCE_FILE
	if(aboveLevelObject != NULL)
	{
		aboveLevelObject->printedFileConnections = true;
	}
	#endif

	return newCurrentReferenceInPrintList;
}



























LDreference* CSdrawClass::createFunctionObjectListBoxesAndConnections(LDreference* currentReferenceInPrintList, CSfile* aboveLevelFileObject, CSfunction* aboveLevelFunctionObject, CSfileContainer* firstObjectInTopLevelBelowListContainer, const int functionLevel, const CSfunction* functionReference, XMLparserTag** currentTag, bool traceFunctionUpwards, bool useSingleFileOnly, string* singleFileName, bool usePredefinedGrid)
{
	string functionReferenceNameToFind = functionReference->name;
	#ifdef CS_MATCH_FUNCTION_REFERENCES_WITH_CORRECT_NUMBER_OF_ARGUMENTS
	bool countArguments = true;
	#else
	bool countArguments = false;
	#endif

	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

	if(!(aboveLevelFunctionObject->printedFunctionConnections))	//added CS 3d2g
	{
		int newFunctionLevel = functionLevel;

		CSfunction* functionObject = NULL;
		CSfile* fileObject = NULL;

		if(CSreferenceContainerClass.findFunctionReferenceTarget(functionReference, aboveLevelFileObject, &fileObject, &functionObject, countArguments))
		{

			if(!(fileObject->printed))
			{
				cout << "createFunctionObjectListBoxesAndConnections{} error: (!(fileObject->printed))" << endl;
				exit(EXIT_ERROR);
			}

			bool prepareForTrace = false;
			bool printedPassSingleFileOnly = false;
			if(useSingleFileOnly)
			{

				if(fileObject->name == *singleFileName)
				{
					if(functionObject->printed)
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
				bool printedPass = false;
				if(useSingleFileOnly)
				{
					if(functionObject->singleFileOnlyPrinted == false)
					{
						printedPass = true;
						functionObject->singleFileOnlyPrinted = true;
					}
				}
				else
				{
					if(functionObject->printed == false)
					{
						printedPass = true;
						functionObject->printed = true;
					}
				}


				vec positionOriginalSingleFileOnly;
				vec positionAboveLevelOriginalSingleFileOnly;
				if(useSingleFileOnly)
				{

					positionOriginalSingleFileOnly.x = functionObject->printX;
					positionOriginalSingleFileOnly.y = functionObject->printY;
					positionAboveLevelOriginalSingleFileOnly.x = aboveLevelFunctionObject->printX;
					positionAboveLevelOriginalSingleFileOnly.y = aboveLevelFunctionObject->printY;

					functionObject->printX = functionObject->printX - fileObject->printX;
					functionObject->printY = functionObject->printY - fileObject->printY;
					aboveLevelFunctionObject->printX = aboveLevelFunctionObject->printX - fileObject->printX;
					aboveLevelFunctionObject->printY = aboveLevelFunctionObject->printY - fileObject->printY;
				}

				if(printedPass)
				{
					int aboveConnectionColour;
					int colour = functionObject->col;
					if(fileObject == aboveLevelFileObject)
					{//function found in existing file

					}
					else
					{//function found in a new file
						newFunctionLevel = 0;
					}

					if(!useSingleFileOnly)
					{

						//now print function;
						functionObject->printXIndex = (fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel]);
						functionObject->printYIndex = newFunctionLevel;

						double xOffset = 0.0;
						#ifdef CS_DRAW_APPLY_EVEN_ODD_X_OFFSET
						/*
						#ifdef CS_SUPPORT_PREDEFINED_GRID
						if(!usePredefinedGrid)
						{
						#endif
						*/
							if((newFunctionLevel % 2) == 0)
							{
								xOffset = CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/0.5 / 2.0;
							}
						/*
						#ifdef CS_SUPPORT_PREDEFINED_GRID
						}
						#endif
						*/
						#endif
						functionObject->printX = fileObject->printX + (fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel])*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/0.5 + xOffset - (CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR*0.5* CS_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X* 0.5);
						functionObject->printY = fileObject->printY + (1+newFunctionLevel)*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
						functionObject->filePrintXrecordForSingleFileOnly = fileObject->printX;
						functionObject->filePrintYrecordForSingleFileOnly = fileObject->printY;

						fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel] = fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel] + 1;

						/*
						cout << "aboveLevelFunctionObject->printX = " << aboveLevelFunctionObject->printX << endl;
						cout << "aboveLevelFunctionObject->printY = " << aboveLevelFunctionObject->printY << endl;
						cout << "functionObject->printX  = " << functionObject->printX << endl;
						cout << "functionObject->printY  = " << functionObject->printY << endl;
						cout << "fileObject->printX  = " << fileObject->printX << endl;
						cout << "fileObject->printY  = " << fileObject->printY << endl;
						cout << "fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel]  = " << fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel] << endl;
						cout << "fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel]  = " << fileObject->maxFunctionPrintXAtAParticularY[newFunctionLevel] << endl;
						*/


						#ifdef CS_USE_RAINBOW_COLOURS_FOR_FUNCTION_BOXES
						aboveConnectionColour = aboveLevelFunctionObject->col;	//or =this->calculateCSBoxAndConnectionColourBasedUponLevel(aboveLevelFunctionObject->printYIndex);	[same thing]
						colour = this->calculateCSBoxAndConnectionColourBasedUponLevel(functionObject->printYIndex);
						#else
							//new: use same colour as file box
							aboveConnectionColour = aboveLevelFunctionObject->col;		//OLDWRONG; int aboveConnectionColour = aboveLevelFunctionObject->col;	//OLDWRONG2; same as aboveLevelFileObject->colour;
							colour = fileObject->col;					//OLDWRONG; int colour = aboveLevelFileObject->col;
						#endif
						functionObject->col = colour;
					}
					else
					{
						//assume print coordinates already set
						aboveConnectionColour = aboveLevelFunctionObject->col;
					}

					/*
					//DEBUG:
					if(useSingleFileOnly)
					{
						if(functionObject->name == "traceFunctionsUpwardsAndDrawOrHighLightTheseReset")
						{
							cout << "traceFunctionsUpwardsAndDrawOrHighLightTheseReset" << endl;
							cout << "colour = " << colour << endl;
							cout << "aboveConnectionColour = " << aboveConnectionColour << endl;
							cout << "functionObject->printX = " << functionObject->printX << endl;
							cout << "functionObject->printY = " << functionObject->printY << endl;
							cout << "aboveLevelFunctionObject->printX = " << aboveLevelFunctionObject->printX << endl;
							cout << "aboveLevelFunctionObject->printY = " << aboveLevelFunctionObject->printY << endl;
							cout << "fileObject->name = " << fileObject->name << endl;
							cout << "\t functionObject->name = " << functionObject->name << endl;
							cout << "\t aboveLevelFunctionObject->name = " << aboveLevelFunctionObject->name << endl;
							cout << "\t\t fileObject->printX = " << fileObject->printX << endl;
							cout << "\t\t fileObject->printY = " << fileObject->printY << endl;
							cout << "\t\t aboveLevelFileObject->printX = " << aboveLevelFileObject->printX << endl;
							cout << "\t\t aboveLevelFileObject->printY = " << aboveLevelFileObject->printY << endl;
							cout << "\t\t functionObject->filePrintXrecordForSingleFileOnly = " << functionObject->filePrintXrecordForSingleFileOnly << endl;
							cout << "\t\t functionObject->filePrintYrecordForSingleFileOnly = " << functionObject->filePrintYrecordForSingleFileOnly << endl;
							cout << "\t\t aboveLevelFunctionObject->filePrintXrecordForSingleFileOnly = " << aboveLevelFunctionObject->filePrintXrecordForSingleFileOnly << endl;
							cout << "\t\t aboveLevelFunctionObject->filePrintYrecordForSingleFileOnly = " << aboveLevelFunctionObject->filePrintYrecordForSingleFileOnly << endl;
						}
					}
					*/

					//print function connections;
					newCurrentReferenceInPrintList = this->createFunctionObjectConnection(newCurrentReferenceInPrintList, functionObject, aboveLevelFunctionObject, aboveConnectionColour, traceFunctionUpwards, prepareForTrace, currentTag, fileObject, aboveLevelFileObject);


					//apply hack
					if(useSingleFileOnly)
					{
						if(functionLevel == 0)
						{
							aboveLevelFunctionObject->printX = aboveLevelFunctionObject->printX + fileObject->printX;
							aboveLevelFunctionObject->printY = aboveLevelFunctionObject->printY + fileObject->printY;
						}
					}

					#ifdef CS_WRITE_SVG_GROUPS
					XMLparserTag* nextTagOnOriginalLayer = NULL;
					#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
					if(!traceFunctionUpwards)
					{//only print connections when not tracing a bottom level function upwards - saves space
					#endif
						string groupID = this->createGroupID(fileObject->name, functionObject->printX, functionObject->printY);	//OLD: functionObject->name
						nextTagOnOriginalLayer = LDsvg.writeSVGgroup(currentTag, &groupID);
					#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
					}
					#endif
					#endif

					#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
					if(!traceFunctionUpwards)
					{//only print connections when not tracing a bottom level function upwards - saves space
					#endif
						//print function box
						newCurrentReferenceInPrintList = this->printFunctionBox(newCurrentReferenceInPrintList, currentTag, functionObject, colour);
					#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
					}
					#endif


					#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
					if(!traceFunctionUpwards)
					{//only print connections when not tracing a bottom level function upwards - saves space
					#endif
						newCurrentReferenceInPrintList = this->printFunctionBoxText(newCurrentReferenceInPrintList, currentTag, functionObject, colour, useSingleFileOnly);
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
							aboveLevelFunctionObject->printX = aboveLevelFunctionObject->printX - fileObject->printX;
							aboveLevelFunctionObject->printY = aboveLevelFunctionObject->printY - fileObject->printY;
						}
					}

					if(useSingleFileOnly)
					{
						functionObject->printX = positionOriginalSingleFileOnly.x;
						functionObject->printY = positionOriginalSingleFileOnly.y;
						aboveLevelFunctionObject->printX = positionAboveLevelOriginalSingleFileOnly.x;
						aboveLevelFunctionObject->printY = positionAboveLevelOriginalSingleFileOnly.y;
					}

					//functionObject->printed = true;	//has been moved;

					/*
					for(int i= 0; i<newFunctionLevel+fileObject->level; i++)
					{
						cout << "\t";
					}
					cout << "file = " << fileObject->name << endl;
					for(int i= 0; i<newFunctionLevel+fileObject->level; i++)
					{
						cout << "\t";
					}
					cout << "function = " << functionObject->name << endl;
					*/

					/*
					if(functionObject->printX > 3000)
					{
						cout << "functionReferenceNameToFind = " << functionReferenceNameToFind << endl;
						cout << "fileObject->name = " << fileObject->name << endl;
						cout << "functionObject->name = " << functionObject->name << endl;
						cout << "aboveLevelFunctionObject->name = " << aboveLevelFunctionObject->name << endl;
						cout << "aboveLevelFileObject->name = " << aboveLevelFileObject->name << endl;
						exit(EXIT_ERROR);
					}
					*/

					//now for each function LDreference LDreference within function reference;
					CSfunction* currentFunctionReference = functionObject->firstReferenceInFunctionReferenceList;
					while(currentFunctionReference->next != NULL)
					{
						/*
						for(int i= 0; i<newFunctionLevel+fileObject->level+1; i++)
						{
							cout << "\t";
						}
						cout << "function reference to find = " << currentFunctionReference->name << endl;
						for(int i= 0; i<newFunctionLevel+fileObject->level+1; i++)
						{
							cout << "\t";
						}
						cout << "top level include file to search = " << fileObject->name << endl;
						*/

						//recurse
						newCurrentReferenceInPrintList = this->createFunctionObjectListBoxesAndConnections(newCurrentReferenceInPrintList, fileObject, functionObject, firstObjectInTopLevelBelowListContainer, (newFunctionLevel+1), currentFunctionReference, currentTag, traceFunctionUpwards, useSingleFileOnly, singleFileName, usePredefinedGrid);

						currentFunctionReference = currentFunctionReference->next;
					}
					functionObject->printedFunctionConnections = true;
				}
				else
				{
					//print function connections;
					newCurrentReferenceInPrintList = this->createFunctionObjectConnection(newCurrentReferenceInPrintList, functionObject, aboveLevelFunctionObject, aboveLevelFunctionObject->col, traceFunctionUpwards, prepareForTrace, currentTag, fileObject, aboveLevelFileObject);

					if(useSingleFileOnly)
					{
						functionObject->printX = positionOriginalSingleFileOnly.x;
						functionObject->printY = positionOriginalSingleFileOnly.y;
						aboveLevelFunctionObject->printX = positionAboveLevelOriginalSingleFileOnly.x;
						aboveLevelFunctionObject->printY = positionAboveLevelOriginalSingleFileOnly.y;
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
				cout << "\createFunctionObjectListBoxesAndConnections{}:" << endl;
				cout << "functionReferenceNameToFind functionReference->name = " << functionReference->name << endl;
				cout << "aboveLevelFileObject->name = " << aboveLevelFileObject->name << endl;
				cout << "aboveLevelFunctionObject->name = " << aboveLevelFunctionObject->name << endl;
				cout << "countArguments = " << countArguments << endl;
				cout << "fileObject->name = " << fileObject->name << endl;
				cout << "functionObject->name = " << functionObject->name << endl;
				exit(EXIT_ERROR);
			#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED
			}
			#endif
		}
	}

	return newCurrentReferenceInPrintList;
}

LDreference* CSdrawClass::printFunctionBox(LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, CSfunction* functionObject, int colour)
{
	vec pos;
	pos.x = functionObject->printX;
	pos.y = functionObject->printY;
	currentReferenceInPrintList = this->createFileOrFunctionObjectBox(currentReferenceInPrintList, &pos, &(functionObject->name), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, colour, CS_FUNCTION_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FUNCTION_BOX);	//add box
	this->writeFileOrFunctionSVGbox(currentTag, &pos, functionObject->name.length(), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, CS_FUNCTION_MAX_TEXT_LENGTH, colour, CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG);
	return currentReferenceInPrintList;
}

LDreference* CSdrawClass::printFunctionBoxText(LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, CSfunction* functionObject, const int colour, const bool useSingleFileOnly)
{
	//print box text
	int numSpritesAdded;
	vec positionText;

	#ifdef CS_CENTRE_BOXES
	positionText.x = functionObject->printX - CS_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*(functionObject->name.length()/(CS_FUNCTION_MAX_TEXT_LENGTH*CS_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH));
	#else
	positionText.x = functionObject->printX;
	#endif
	positionText.y = functionObject->printY;
	positionText.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;
	if(!useSingleFileOnly)
	{
		functionObject->printTextX = positionText.x;
		functionObject->printTextY = positionText.y;
	}

	currentReferenceInPrintList = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(functionObject->name, currentReferenceInPrintList, &positionText, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/CS_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR);	//add sprite text within box

	positionText.y = positionText.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
	LDsvg.writeSVGtext(currentTag, (functionObject->name), &positionText, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B, DAT_FILE_COLOUR_BLACK);
	return currentReferenceInPrintList;
}


void CSdrawClass::resetPrintedFunctionConnections(constEffective CSfile* aboveLevelFileObject, CSfunction* aboveLevelFunctionObject, const bool useSingleFileOnly, const string* singleFileName)
{
	CSfunction* currentFunctionReference = aboveLevelFunctionObject->firstReferenceInFunctionReferenceList;
	while(currentFunctionReference->next != NULL)
	{
		bool referenceFound = false;
		CSfunction* functionObject = NULL;
		CSfile* fileObject = NULL;
		#ifdef CS_MATCH_FUNCTION_REFERENCES_WITH_CORRECT_NUMBER_OF_ARGUMENTS
		bool countArguments = true;
		#else
		bool countArguments = false;
		#endif
		if(CSreferenceContainerClass.findFunctionReferenceTarget(currentFunctionReference, aboveLevelFileObject, &fileObject, &functionObject, countArguments))
		{
			if(currentFunctionReference->printedFunctionConnections)
			{
				currentFunctionReference->printedFunctionConnections = false;

				this->resetPrintedFunctionConnections(fileObject, functionObject, useSingleFileOnly, singleFileName);
			}
		}
		currentFunctionReference = currentFunctionReference->next;
	}
}





















LDreference* CSdrawClass::createFileOrFunctionObjectBox(LDreference* currentReferenceInPrintList,  vec* referencePrintPos, string* referenceName, double scaleFactor, int colour, double maxTextLength, double zPosition)
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

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
	cout << "createFileOrFunctionObjectBox{}:" << endl;
	cout << "referenceName = " <<* referenceName << endl;
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

	LDreference* newDispayReference = new LDreference();
	newCurrentReferenceInPrintList->next = newDispayReference;
	newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

	return newCurrentReferenceInPrintList;
}


LDreference* CSdrawClass::createFileObjectConnection(LDreference* currentReferenceInPrintList, CSfile* reference, CSfile* currentReferenceInAboveList, int colour, const bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag** currentTag)
{
	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace
	if(reference->firstReferenceContainerInAboveReferenceList == NULL)
	{
		reference->firstReferenceContainerInAboveReferenceList = new CSfileContainer();
	}
	CSfileContainer* currentReferenceContainerInAboveFileOrFunctionObjectList = reference->firstReferenceContainerInAboveReferenceList;
	while(currentReferenceContainerInAboveFileOrFunctionObjectList->next != NULL)
	{
		currentReferenceContainerInAboveFileOrFunctionObjectList = currentReferenceContainerInAboveFileOrFunctionObjectList->next;
	}
	currentReferenceContainerInAboveFileOrFunctionObjectList->fileObject = currentReferenceInAboveList;
	CSfileContainer* newCSRefContainer = new CSfileContainer();
	currentReferenceContainerInAboveFileOrFunctionObjectList->next = newCSRefContainer;

	vec referencePrintPos;
	referencePrintPos.x = reference->printX;
	referencePrintPos.y = reference->printY;
	vec currentReferenceInAboveListPrintPos;
	currentReferenceInAboveListPrintPos.x = currentReferenceInAboveList->printX;
	currentReferenceInAboveListPrintPos.y = currentReferenceInAboveList->printY;
 	string startGroupID = this->createGroupID(reference->name, reference->printX, reference->printY);
 	string endGroupID = this->createGroupID(currentReferenceInAboveList->name, currentReferenceInAboveList->printX, currentReferenceInAboveList->printY);

	return this->configureFileOrFunctionObjectConnection(currentReferenceInPrintList, &referencePrintPos, &currentReferenceInAboveListPrintPos, colour, true, traceAFunctionUpwardsAndNotCurrentlyTracing, currentTag, &startGroupID, &endGroupID);
}

LDreference* CSdrawClass::createFunctionObjectConnection(LDreference* currentReferenceInPrintList, CSfunction* reference,  CSfunction* currentReferenceInAboveList, int colour, const bool traceAFunctionUpwardsAndNotCurrentlyTracing, const bool prepareForTrace, XMLparserTag** currentTag, const CSfile* fileObject,  CSfile* currentFileObjectInAboveList)
{
	if(prepareForTrace)
	{
		//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace
		if(reference->firstReferenceContainerInAboveReferenceList == NULL)
		{
			reference->firstReferenceContainerInAboveReferenceList = new CSfunctionContainer();
		}
		CSfunctionContainer* currentReferenceContainerInAboveFileOrFunctionObjectList = reference->firstReferenceContainerInAboveReferenceList;
		while(currentReferenceContainerInAboveFileOrFunctionObjectList->next != NULL)
		{
			currentReferenceContainerInAboveFileOrFunctionObjectList = currentReferenceContainerInAboveFileOrFunctionObjectList->next;
		}
		currentReferenceContainerInAboveFileOrFunctionObjectList->functionObject = currentReferenceInAboveList;
		currentReferenceContainerInAboveFileOrFunctionObjectList->fileObjectHoldingFunction = currentFileObjectInAboveList;
		CSfunctionContainer* newCSRefContainer = new CSfunctionContainer();
		currentReferenceContainerInAboveFileOrFunctionObjectList->next = newCSRefContainer;
	}

	vec referencePrintPos;
	referencePrintPos.x = reference->printX;
	referencePrintPos.y = reference->printY;
	vec currentReferenceInAboveListPrintPos;
	currentReferenceInAboveListPrintPos.x = currentReferenceInAboveList->printX;
	currentReferenceInAboveListPrintPos.y = currentReferenceInAboveList->printY;
 	string startGroupID = this->createGroupID(fileObject->name, reference->printX, reference->printY);	//OLD: reference->name
 	string endGroupID = this->createGroupID(currentFileObjectInAboveList->name, currentReferenceInAboveList->printX, currentReferenceInAboveList->printY);	//OLD: currentReferenceInAboveList->name

	return this->configureFileOrFunctionObjectConnection(currentReferenceInPrintList, &referencePrintPos, &currentReferenceInAboveListPrintPos, colour, false, traceAFunctionUpwardsAndNotCurrentlyTracing, currentTag, &startGroupID, &endGroupID);
}




LDreference* CSdrawClass::configureFileOrFunctionObjectConnection(LDreference* currentReferenceInPrintList, vec* referencePrintPos, vec* currentReferenceInAboveListPrintPos, int colour, const bool fileOrFunction, const bool traceAFunctionUpwardsAndNotCurrentlyTracing, XMLparserTag** currentTag, const string* startGroupID, const string* endGroupID)
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

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
		cout << "configureFileOrFunctionObjectConnection{}:" << endl;
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

		LDreference* newDispayReference = new LDreference();
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

		LDsvg.writeSVGconnector(currentTag, &pos1, &pos2, colour, startGroupID, endGroupID, strokeWidth);
		#else
		LDsvg.writeSVGline(currentTag, &pos1, &pos2, colour);
		#endif


	}

	return newCurrentReferenceInPrintList;
}

LDreference* CSdrawClass::createBox(LDreference* currentReferenceInPrintList, vec* vect, const double width, const double height, int colour)
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

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
	cout << "createFileOrFunctionObjectBox{}:" << endl;
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

	LDreference* newDispayReference = new LDreference();
	newCurrentReferenceInPrintList->next = newDispayReference;
	newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

	return newCurrentReferenceInPrintList;
}














int CSdrawClass::calculateCSBoxAndConnectionColourBasedUponLevel(const int yIndex)
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
		exit(EXIT_ERROR);
	}

	return colour;

}

int CSdrawClass::calculateCSBoxAndConnectionColourBasedUponFileName(const CSfile* currentFileObject)
{
	string fileName = currentFileObject->name;
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



















bool CSdrawClass::findPrintedFunctionObjectWithName(const string name, constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** updatedFunctionObject)
{
	bool foundPrintedReferenceWithName = false;

	constEffective CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		constEffective CSfile* currentFileObject = currentFileObjectContainer->fileObject;

		constEffective CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
		while(currentFunctionObject->next != NULL)
		{
			if(currentFunctionObject->name == name)
			{
				if(currentFunctionObject->printed)
				{//only create connections to printed boxes

					*updatedFunctionObject = currentFunctionObject;
					foundPrintedReferenceWithName = true;
					*fileObjectHoldingFunction = currentFileObject;
				}
			}
			currentFunctionObject = currentFunctionObject->next;
		}

		if(currentFileObject->firstFileInBelowListContainer != NULL)
		{
			if(this->findPrintedFunctionObjectWithName(name, currentFileObject->firstFileInBelowListContainer, fileObjectHoldingFunction, updatedFunctionObject))
			{
				foundPrintedReferenceWithName = true;
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}

	return foundPrintedReferenceWithName;
}

LDreference* CSdrawClass::traceFunctionsUpwardsAndDrawOrHighLightThese(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* currentFunctionBeingTraced, XMLparserTag** currentTag, int generateHTMLdocumentationMode, string* HTMLdocumentationFunctionTraceTableRows)
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

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
	newCurrentReferenceInPrintList = this->createFileOrFunctionObjectBox(newCurrentReferenceInPrintList, &pos, &(currentFunctionBeingTraced->name), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, functionBoxColour, CS_FUNCTION_MAX_TEXT_LENGTH, CS_OUTPUT_Z_POSITION_FUNCTION_TRACE_BOX);	//add box

	this->writeFileOrFunctionSVGboxTransparent(currentTag, &pos, currentFunctionBeingTraced->name.length(), CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR, CS_FUNCTION_MAX_TEXT_LENGTH, functionBoxColour, CS_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG, 0.5);

	//print function text;
	#ifdef CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
		int numSpritesAdded;	//not used
		vec textPosition;
		textPosition.x = currentFunctionBeingTraced->printTextX;
		textPosition.y = currentFunctionBeingTraced->printTextY;
		textPosition.z = CS_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

		newCurrentReferenceInPrintList = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(currentFunctionBeingTraced->name, newCurrentReferenceInPrintList, &textPosition, &numSpritesAdded, false, DAT_FILE_COLOUR_WHITE, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR/(200.0));	//add sprite text within box

		textPosition.y = textPosition.y + CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG*CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
		LDsvg.writeSVGtext(currentTag, (currentFunctionBeingTraced->name), &textPosition, CS_FUNCTION_VECTOROBJECTS_SCALE_FACTOR*CS_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B, DAT_FILE_COLOUR_BLACK);

	#endif

	currentFunctionBeingTraced->printedTrace = true;
	currentFunctionBeingTraced->printedTraceReset = false;

	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace

	if(currentFunctionBeingTraced->firstReferenceContainerInAboveReferenceList != NULL)
	{
		CSfunctionContainer* currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList = currentFunctionBeingTraced->firstReferenceContainerInAboveReferenceList;
		while(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->next != NULL)	// && (currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->name != "main")
		{

			CSfile* fileObjectHoldingFunction = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->fileObjectHoldingFunction;
			CSfunction* higherLevelFunction = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->functionObject;
			bool higherLevelFunctionFound = true;

			if(higherLevelFunctionFound)
			{
				string fileNameHoldingFunction = fileObjectHoldingFunction->name;

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

					CSfile fileReferenceUnkown;
					fileReferenceUnkown.name = "";
					CSfile fileReferenceInAboveLevelListUnkown;
					fileReferenceInAboveLevelListUnkown.name = "";
					newCurrentReferenceInPrintList = this->createFunctionObjectConnection(newCurrentReferenceInPrintList, currentFunctionBeingTraced, higherLevelFunction, functionConnectionColour, false, false, currentTag, &fileReferenceUnkown, &fileReferenceInAboveLevelListUnkown);

					if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
					{
						#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
						cout << "Trace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << currentFunctionBeingTraced->name << "{}" << endl;
						#endif
					}
					else if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
					{
						string HTMLdocumentationFunctionTraceTableRow = "\t\t<tr><td>" + fileNameHoldingFunction + "</td><td>" + currentFunctionBeingTraced->name + "</td></tr>\n";
						*HTMLdocumentationFunctionTraceTableRows = *HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableRow;
					}

					newCurrentReferenceInPrintList = this->traceFunctionsUpwardsAndDrawOrHighLightThese(newCurrentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, higherLevelFunction, currentTag, generateHTMLdocumentationMode, HTMLdocumentationFunctionTraceTableRows);
				}
			}
			else
			{
				cout << "error: traceFunctionsUpwardsAndDrawOrHighLightThese{}: higherLevelFunctionFound " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->functionObject->name << " cannot be found" << endl;
				exit(EXIT_ERROR);
			}

			currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->next;
		}
	}

	return newCurrentReferenceInPrintList;
}



void CSdrawClass::traceFunctionsUpwardsAndDrawOrHighLightTheseReset(const CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* currentFunctionBeingTraced)
{
	currentFunctionBeingTraced->printedTrace = false;
	currentFunctionBeingTraced->printedTraceReset = true;

	//add currentReferenceInAboveList to reference's above list for reverse lookup / function upwards trace

	if(currentFunctionBeingTraced->firstReferenceContainerInAboveReferenceList != NULL)
	{
		CSfunctionContainer* currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList = currentFunctionBeingTraced->firstReferenceContainerInAboveReferenceList;
		while(currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->next != NULL)	// && (currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->name != "main")
		{
			CSfile* fileObjectHoldingFunction = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->fileObjectHoldingFunction;
			CSfunction* higherLevelFunction = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->functionObject;
			bool higherLevelFunctionFound = true;

			if(higherLevelFunctionFound)
			{
				string fileNameHoldingFunction = fileObjectHoldingFunction->name;

				if(higherLevelFunction->printedTraceReset == false)
				{
					this->traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstObjectInTopLevelBelowListContainer, higherLevelFunction);
				}
			}
			else
			{
				cout << "error: traceFunctionsUpwardsAndDrawOrHighLightThese{}: higherLevelFunctionFound " << currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->functionObject->name << " cannot be found" << endl;
				exit(EXIT_ERROR);
			}

			currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList = currentReferenceContainerInCurrentFunctionBeingTracedAboveFunctionObjectList->next;
		}
	}
}




void CSdrawClass::writeFileOrFunctionSVGbox(XMLparserTag** currentTag, const vec* pos, const int textLength, const double scaleFactor, const double maxTextLength, const int col, const double boxOutlineWidth)
{
#ifdef CS_CENTRE_BOXES
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor*(double)textLength/maxTextLength;
#else
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor;
#endif
	double height = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG*scaleFactor;

	LDsvg.writeSVGbox(currentTag,  pos, width, height, col, boxOutlineWidth, false);
}

void CSdrawClass::writeFileOrFunctionSVGboxTransparent(XMLparserTag** currentTag, const vec* pos, const int textLength, const double scaleFactor, const double maxTextLength, const int col, const double boxOutlineWidth, const double fillOpacity)
{
#ifdef CS_CENTRE_BOXES
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor*(double)textLength/maxTextLength;
#else
	double width = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG*scaleFactor;
#endif
	double height = CS_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG*scaleFactor;

	LDsvg.writeSVGboxTransparent(currentTag,  pos, width, height, col, boxOutlineWidth, false, fillOpacity);
}

string CSdrawClass::createGroupID(const string objectName, const int printX, const int printY)
{
	string groupID = "";
	groupID = groupID + objectName + SHAREDvars.convertIntToString(printX) + SHAREDvars.convertIntToString(printY);
	//groupID = groupID + SHAREDvars.convertIntToString(printX) + SHAREDvars.convertIntToString(printY);
	return groupID;
}

