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
 * File Name: CSfunctionClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h1b 14-November-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_FUNCTION_CLASS
#define HEADER_CS_FUNCTION_CLASS

#include "CSglobalDefs.h"

#define MAX_INCLUDE_DEPTH_FUNCTION (20)	//for a function within a file

#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
class CSfunctionArgument
{
public:
	CSfunctionArgument(void);
	string argument;	//name and type for function declaration (header) arguments, or function reference argument contents 
	string argumentName;
	string argumentType;
	bool isConst;
	bool constIdentified;
	CSfunctionArgument* next;
};
#endif

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
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	CSfunctionArgument* firstFunctionArgumentInFunction;
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
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	bool functionArgumentConstsIdentified;
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


void printCSFunctionObject(CSfunction* ref);


#endif

