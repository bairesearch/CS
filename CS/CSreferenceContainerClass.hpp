 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSreferenceContainerClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
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
	~CSfunctionContainer(void);	//  and destructor.

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
	~CSfileContainer(void);	//  and destructor.

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
		#ifdef CS_OPTIMISE_FUNCTION_REFERENCE_TARGET_SEARCH
		private: void findFunctionReferenceTargetRecurseReset(constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer);
		#endif

};

#endif

