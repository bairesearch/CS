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
 * File Name: CSreferenceContainerClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h2c 23-November-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_REFERENCE_CONTAINER_CLASS
#define HEADER_CS_REFERENCE_CONTAINER_CLASS

#include "CSglobalDefs.h"
#include "CSfileClass.h"
#include "CSfunctionClass.h"

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

int findEndPositionOfArgument(string* functionArgumentsRaw, int startPositionOfArgument, bool* lastArgument);
	
bool findFunctionReferenceTarget(CSfunction* functionReference, CSfile* currentFileObject, CSfile** fileObjectHoldingFunction, CSfunction** functionReferenceTarget, bool countArguments);
	bool findFunctionReferenceTargetRecurse(CSfunction* functionReference, CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfile** fileObjectHoldingFunction, CSfunction** functionReferenceTarget, bool countArguments);
		int countArgumentList(CSfunctionArgument* firstFunctionArgumentInFunction);

#endif

