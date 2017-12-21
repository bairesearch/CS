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
 * File Name: CSreferenceContainerClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3m8a 14-December-2017
 *
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

