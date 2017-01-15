/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateConstFunctionArgumentCode.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3h1f 14-November-2015
 *
 *******************************************************************************/


#ifndef HEADER_CS_GENERATE_CONST_FUNCTION_ARGUMENT_CODE
#define HEADER_CS_GENERATE_CONST_FUNCTION_ARGUMENT_CODE

#include "CSreferenceContainerClass.h"
#include "CSglobalDefs.h"

#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
bool generateConstFunctionArguments(CSfileContainer* firstObjectInTopLevelBelowListContainer);
	bool generateConstFunctionArgumentsRecurse(CSfileContainer* firstObjectInAboveLevelBelowListContainer, CSfileContainer* firstObjectInTopLevelBelowListContainer);
		bool generateConstFunctionArgumentsFile(CSfile* currentFileObject);
			bool generateConstFunctionArgumentsFunction(CSfunction* currentFunctionObject);

#endif

#endif