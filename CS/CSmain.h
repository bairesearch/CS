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
 * File Name: CSmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3j1b 14-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_MAIN
#define HEADER_CS_MAIN


#include "CSexecflow.h"
#include "CSdataflow.h"
#include "CSpreprocessorFilter.h"
#include "CSdraw.h"
#include "CSgenerateHTMLdocumentation.h"
#include "XMLrulesClass.h"
#include "LDsprite.h"
#define CS_MODE_OUTPUT_EXECUTION_FLOW (1)
#define CS_MODE_OUTPUT_DATA_FLOW (2)
#define CS_MODE_FILTER_CODE_USING_PREPROCESSOR_DEFINITIONS (3)

int main(const int argc, const char** argv);

class CSmainClass
{
};

#endif
