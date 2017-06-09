/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: CSfileClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3l1f 01-June-2017
 *
 *******************************************************************************/

#include "CSfileClass.hpp"


CSfile::CSfile(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	printedFileConnections = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	HTMLgenerated = false;

	firstReferenceContainerInAboveReferenceList = NULL;	//new CSfileContainer()  - no longer possible with forward declaration of CSfileContainer
	printedTrace = false;
	printedTraceReset = false;
	for(int i=0; i<MAX_INCLUDE_DEPTH_FILE; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}

	//file only
	firstFileInBelowListContainer = NULL;
	firstFunctionInFunctionList = NULL;

	#ifdef CS_GENERATE_CODE_GENERIC
	printedCodeGeneric = false;
	sourceFileNameOrig = "";
	sourceFileTextOrig = "";
	headerFileTextOrig = "";
	headerFileName = "";
	sourceFileName = "";
	sourceFileText = "";
	headerFileText = "";
	#endif
}

CSfile::~CSfile(void)
{
	if(firstFileInBelowListContainer != NULL)
	{
		delete firstFileInBelowListContainer;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
}



void CSfileClassClass::printCSFileObject(const CSfile* ref)
{
	cout << "col = " << ref->col << endl;
	cout << "id = " << ref->id << endl;
	cout << "printed = " << ref->printed << endl;
	cout << "printX = " << ref->printX << endl;
	cout << "printY = " << ref->printY << endl;
	cout << "printXIndex = " << ref->printXIndex << endl;
	cout << "printYIndex = " << ref->printYIndex << endl;
	cout << "printTextX = " << ref->printTextX << endl;
	cout << "printTextY = " << ref->printTextY << endl;
	cout << "level = " << ref->level << endl;
	cout << "name = " << ref->name << endl;
}

