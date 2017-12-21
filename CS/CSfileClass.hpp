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
 * File Name: CSfileClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3m9a 16-December-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_FILE_CLASS
#define HEADER_CS_FILE_CLASS

#include "CSglobalDefs.hpp"

#define MAX_INCLUDE_DEPTH_FILE (20)	//file

class CSfunction;
class CSfileContainer;
class CSfile{
private:
	/*There are currently no private attributes of this class*/
public:

	CSfile(void); // constructor declaration
	~CSfile();	//  and destructor.

	//file/function [shared]
	int id;			//file/function id
	int level;		//file/function Level
	string name;		//file/function (headerFileNameOrig, eg "x.h")
	int col;		//	file/function colour + connection colour to all children
	bool printed;		//file/function printed
	bool printedFileConnections;	//file/function connections printed
	int printX;		//file/function printX
	int printY;		//file/function printY
	int printXIndex;	//file/function printXIndex
	int printYIndex;	//file/function printYIndex
	int printTextX;		//file/function printXIndex
	int printTextY;		//file/function printYIndex
	bool HTMLgenerated;	//file/function HTML generated

	CSfileContainer* firstReferenceContainerInAboveReferenceList;
	bool printedTrace;
	bool printedTraceReset;		//used for html generation
	int maxFunctionPrintXAtAParticularY[MAX_INCLUDE_DEPTH_FILE];

	//file only
	CSfileContainer* firstFileInBelowListContainer; 	//filefirstReferenceInBelowList
	CSfunction* firstFunctionInFunctionList;

	#ifdef CS_GENERATE_CODE_GENERIC
	bool printedCodeGeneric;			//added @CS3f1a to only print CPP files once (don't overwrite)
	string sourceFileNameOrig;	//eg "x.c"
	string sourceFileTextOrig;
	string headerFileTextOrig;
	string headerFileName;	//eg "x.h"
	string sourceFileName;	//eg "x.c"
	string sourceFileText;
	string headerFileText;
	#endif
};

class CSfileClassClass
{
	private: void printCSFileObject(const CSfile* ref);
};


#endif

