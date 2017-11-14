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
 * but WITHOUT ANY WARRANTY; without even trhe implied warranty of
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
 * File Name: CSfunctionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3m3a 18-July-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_FUNCTION_CLASS
#define HEADER_CS_FUNCTION_CLASS

#include "CSglobalDefs.hpp"

#define MAX_INCLUDE_DEPTH_FUNCTION (20)	//for a function within a file

class CSfunctionArgument
{
public:
	CSfunctionArgument(void);
	string argument;	//for function declaration (header) arguments (includes type and name), and for function reference argument contents
	string argumentName;	//for function declaration (header) arguments, not for function references arguments
	string argumentType;	//for function declaration (header) arguments, not for function references arguments
	bool isNotConst;	//for function declaration (header) arguments, not for function references arguments
	//#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_SUPPORT_PSEUDO_CONSTS
	bool isConstEffective;	//for function declaration (header) arguments, not for function references arguments
	//#endif
	bool constIdentified;	//for function declaration (header) arguments, not for function references arguments
	CSfunctionArgument* next;	//for function declaration (header) arguments (includes type and name), and for function reference argument contents
	vector<string> argumentNameAliasList;	//for function declaration (header) arguments, not for function references arguments
};

class CSfile;
class CSfunctionContainer;
class CSfunction{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfunction(void); // constructor declaration
	~CSfunction();	//  and destructor.

	//file/function [shared]
	int id;			//file/function id
	int level;		//file/function Level
	string name;		//file/function
	int col;		//	file/function colour + connection colour to all children
	bool printed;		//file/function printed
	bool printedFunctionConnections; 	//file/function connections printed
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
	bool singleFileOnlyPrinted;	//file/function printed
	#endif
	int printX;		//file/function printX
	int printY;		//file/function printY
	int printXIndex;	//file/function printXIndex
	int printYIndex;	//file/function printYIndex
	int printTextX;		//file/function printXIndex
	int printTextY;		//file/function printYIndex
	int filePrintXrecordForSingleFileOnly;	//file/function printX
	int filePrintYrecordForSingleFileOnly;	//file/function printY
	bool HTMLgenerated;	//file/function HTML generated

	CSfunctionContainer* firstReferenceContainerInAboveReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation
	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FUNCTION];

	//function/functionReference
	bool isFunction;	//always true?
	bool isFunctionReference;
	string nameFull;

	CSfunctionArgument* firstFunctionArgumentInFunction;
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	bool functionArgumentConstsIdentified;
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	bool parseSecondaryReferencesOnly;	//added 3h6a
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_PERFORM_SEPARATE_PASSES_TO_SUPPORT_RECURSION
	bool parseSecondaryReferencesAndModificationsOnly;
	#endif
	#endif

	//function only
	CSfunction* next;
	CSfunction* previous;
	CSfunction* firstReferenceInFunctionReferenceList;
	bool hasHadFunctionReferencesParsed;
	int numTopLevelFunctionsInFileAlreadyPrinted;
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	string functionText;
	CSfunction* firstReferenceInFunctionReferenceListRepeats;
	#endif
	#ifdef CS_GENERATE_CODE_GENERIC
	string functionTextRaw;	//function contents with comments?
	string headerFunctionNameFullUpdated;
	string sourceFunctionNameFullUpdated;
	#endif

	//function reference only
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	int functionReferenceCharacterIndex;
	#endif
	#ifdef CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
	int functionReferenceIndentation;
	#endif
	CSfunction* functionReferenceTarget;	//added 3h1a
	CSfile* functionReferenceTargetFileOwner;	//added 3h1a
};


class CSfunctionClassClass
{
	private: void printCSFunctionObject(const CSfunction* ref);
};


#endif

