/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSreferenceContainerClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3n1c 10-August-2019
 * /
 *******************************************************************************/


#ifndef HEADER_CS_REFERENCE_CONTAINER_CLASS
#define HEADER_CS_REFERENCE_CONTAINER_CLASS

#include "CSglobalDefs.hpp"
#include "CSfileClass.hpp"
#include "CSfunctionClass.hpp"

class CSfunctionContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfunctionContainer(void); // constructor declaration
	~CSfunctionContainer();	//  and destructor.

	CSfunctionContainer* next;
	CSfunctionContainer* previous;

	CSfunction* functionObject;
	CSfile* fileObjectHoldingFunction;	//this should really be moved to CSfunction (but is added to CSfunctionContainer instead to prevent CSfile/CSfunction from bidirectionally accessing each other)
};

class CSfileContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfileContainer(void); // constructor declaration
	~CSfileContainer();	//  and destructor.

	CSfileContainer* next;
	CSfileContainer* previous;

	CSfile* fileObject;
	CSfile* aboveLevelObject;
};

class CSreferenceContainerClassClass
{
	public: int findEndPositionOfArgument(const string* functionArgumentsRaw, const int startPositionOfArgument, bool* lastArgument);

	public: bool findFunctionReferenceTarget(const CSfunction* functionReference, constEffective CSfile* currentFileObject, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** functionReferenceTarget, const bool countArguments);
		private: bool findFunctionReferenceTargetRecurse(const CSfunction* functionReference, constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** functionReferenceTarget, const bool countArguments);
			private: int countArgumentList(const CSfunctionArgument* firstFunctionArgumentInFunction);
};

#endif

