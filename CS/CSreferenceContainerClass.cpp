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
 * File Name: CSreferenceContainerClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3f1a 10-May-2015
 *
 *******************************************************************************/

#include "CSreferenceContainerClass.h"




CSfunctionReferenceContainer::CSfunctionReferenceContainer(void)
{
	next = NULL;
	functionReference = NULL;
	fileReferenceHoldingFunction = NULL;
}

CSfunctionReferenceContainer::~CSfunctionReferenceContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
	if(functionReference != NULL)
	{
		delete functionReference;
	}
}

CSfileReferenceContainer::CSfileReferenceContainer(void)
{
	next = NULL;
	fileReference = NULL;
	aboveLevelReference = NULL;
}

CSfileReferenceContainer::~CSfileReferenceContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
	if(fileReference != NULL)
	{
		delete fileReference;
	}
}


