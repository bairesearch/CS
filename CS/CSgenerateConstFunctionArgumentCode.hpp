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
 * File Name: CSgenerateConstFunctionArgumentCode.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3m4a 12-November-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_GENERATE_CONST_FUNCTION_ARGUMENT_CODE
#define HEADER_CS_GENERATE_CONST_FUNCTION_ARGUMENT_CODE

#include "CSreferenceContainerClass.hpp"
#include "CSglobalDefs.hpp"
#include "SHAREDvars.hpp"

#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
class CSgenerateConstFunctionArgumentCodeClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: bool generateConstFunctionArguments(CSfileContainer* firstObjectInTopLevelBelowListContainer);
		private: bool generateConstFunctionArgumentsRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, const CSfileContainer* firstObjectInTopLevelBelowListContainer);
			private: bool generateConstFunctionArgumentsFile(CSfile* currentFileObject);
				private: bool generateConstFunctionArgumentsFunction(CSfunction* currentFunctionObject);
					private: bool generateConstFunctionArgumentAndSearchForSecondaryReferences(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument);
						private: bool generateConstFunctionArgument(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument);
							private: void checkIfVariableIsBeingModifiedInFunction(CSfunction* currentFunctionObject, CSfunctionArgument* currentFunctionArgumentInFunction, string functionDeclarationArgument, bool* isNotConst, bool* isConstEffective);
								private: bool verifyThatVariableIsNotDeclarationOrNextIteratorAssignmentOrArrayIndex(const CSfunction* currentFunctionObject, const string* functionText, const string textBeforeFunctionArgument, const string functionDeclarationArgument, const int indexOfFunctionArgument, const int indexOfSquareBracketOpen, const int indexOfSquareBracketClose, const int indexOfEndOfCommand, const int indexOfStartOfLine, const int indexOfEqualsSet);
								private: bool functionArgumentReferenceWholeWordCheck(string* functionText, string functionDeclarationArgument, int indexOfFunctionArgument);
								private: bool charInString(const string text, const char* charArray, const int arraySize);
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_DOUBLE_POINTERS
								private: bool detectDoublePointer(string functionArgument);
								#endif
								#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_MAKE_ALL_POINTER_ARRAY_TYPES_NON_CONST
								private: bool detectPointerArray(string functionArgument);
								#endif
							private: string extractFullVariableName(string* functionText, const int indexOfStartOfVariableName);
							private: string extractFullVariableNameReverse(const string* functionText, const int indexOfEndOfVariableName, int* indexOfStartOfVariableName);
							private: string extractFullVariableTypeReverse(const string* functionText, const int indexOfEndOfVariableType, int* indexOfStartOfVariableType);
				private: string addConstToAllOccurancesOfFunctionObjectFunctionArgumentSecondaryAssignmentDeclarationInFunction(const string* functionTextOrig, const string functionArgumentSecondaryAssignmentName, bool* foundAtLeastOneInstance, const string constString);
};

#endif

#endif
