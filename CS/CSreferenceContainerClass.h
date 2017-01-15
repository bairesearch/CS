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
 * Project Version: 3f7d 16-August-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_REFERENCE_CONTAINER_CLASS
#define HEADER_CS_REFERENCE_CONTAINER_CLASS

#include "CSglobalDefs.h"
#include "CSreferenceClass.h"


class functionReference;
class CSfunctionReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfunctionReferenceContainer(void); // constructor declaration
	~CSfunctionReferenceContainer();	//  and destructor.

	CSfunctionReferenceContainer* next;
	CSfunctionReferenceContainer* previous;
	
	CSfunctionReference* functionReference;
	CSfileReference* fileReferenceHoldingFunction;	//this should really be moved to CSfunctionReference (but is added to CSfunctionReferenceContainer instead to prevent CSfileReference/CSfunctionReference from bidirectionally accessing each other)
};

class fileReference;
class CSfileReferenceContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfileReferenceContainer(void); // constructor declaration
	~CSfileReferenceContainer();	//  and destructor.

	CSfileReferenceContainer* next;
	CSfileReferenceContainer* previous;
	
	CSfileReference* fileReference;
	CSfileReference* aboveLevelReference;
};

#endif

