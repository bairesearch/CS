 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSfunctionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_CS_FUNCTION_CLASS
#define HEADER_CS_FUNCTION_CLASS

#include "CSglobalDefs.hpp"
#include "CSparameterClass.hpp"

#define MAX_INCLUDE_DEPTH_FUNCTION (20)	//for a function within a file


#ifdef CS_SUPPORT_FUNCTION_ACCESS_SPECIFIERS
	//#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_UNKNOWN 0
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PUBLIC 1
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PRIVATE 2
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PROTECTED 3
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_UNKNOWN_NAME "unknown:"
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PUBLIC_NAME "public:"
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PRIVATE_NAME "private:"
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PROTECTED_NAME "protected:"
	#define CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_NUMBER_OF_TYPES (4)
	static string functionAccessSpecifierNameArray[CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_NUMBER_OF_TYPES] = {CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_UNKNOWN_NAME, CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PUBLIC_NAME, CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PRIVATE_NAME, CS_SUPPORT_FUNCTION_ACCESS_SPECIFIER_PROTECTED_NAME};
#endif
	
	
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
	~CSfunction(void);	//  and destructor.

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
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE
	string nameFullHeader;
	#endif

	CSfunctionArgument* firstFunctionArgumentInFunction;
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	bool functionArgumentConstsIdentified;
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS_DETECT_ASSIGNMENT_OF_ALIASES
	bool parseSecondaryReferencesOnly;	//added CS3h6a
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
	CSfunction* functionReferenceTarget;	//added CS3h1a
	CSfile* functionReferenceTargetFileOwner;	//added CS3h1a
	
	string functionType;	//added CS3o2a
	#ifdef CS_USE_FUNCTION_ARGUMENTS_STRING
	string functionArgumentsString;	//added CS3o2a
	#endif
	#ifdef CS_USE_FUNCTION_ARGUMENTS_PARAMETER_LIST
	vector<CSparameter*> functionArgumentsParameterList;	//added CS3o4b
	#endif
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE
	string className;
	#ifdef CS_SUPPORT_FUNCTION_ACCESS_SPECIFIERS
	int functionAccessSpecifierType;
	#endif
	//string nameWithClass;	//added CS3o2a
	#endif
};


class CSfunctionClassClass
{
	private: void printCSFunctionObject(const CSfunction* ref);
};


#endif

