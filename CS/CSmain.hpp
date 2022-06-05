 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSmain.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_CS_MAIN
#define HEADER_CS_MAIN


#include "CSexecflow.hpp"
#include "CSdataflow.hpp"
#include "CSpreprocessorFilter.hpp"
#include "CSdraw.hpp"
#include "CSgenerateHTMLdocumentation.hpp"
#include "XMLrulesClass.hpp"
#include "LDsprite.hpp"
#define CS_MODE_OUTPUT_EXECUTION_FLOW (1)
#define CS_MODE_OUTPUT_DATA_FLOW (2)
#define CS_MODE_FILTER_CODE_USING_PREPROCESSOR_DEFINITIONS (3)

int main(const int argc, const char** argv);

class CSmainClass
{
};

#endif
