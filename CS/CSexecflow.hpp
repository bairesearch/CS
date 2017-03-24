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
 * File Name: CSexecflow.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3k2c 21-March-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_EXECFLOW
#define HEADER_CS_EXECFLOW

#include "CSglobalDefs.hpp"
#include "CSreferenceContainerClass.hpp"
#include "LDreferenceManipulation.hpp"
#include "XMLparserClass.hpp"
#include "CSoperations.hpp"
#include "CSdraw.hpp"
#include "LDsvg.hpp"
#include "LDopengl.hpp"
#include "XMLrulesClass.hpp"
#include "LDparser.hpp"
#include "LDsprite.hpp"
#include "RTpixelMaps.hpp"
#include "CSgenerateHTMLdocumentation.hpp"
#include "CSgenerateObjectOrientedCode.hpp"
#include "CSgenerateConstFunctionArgumentCode.hpp"

#ifdef LINUX
	#define SYSTEM_COPY_COMMAND "cp"
#else
	#define SYSTEM_COPY_COMMAND "copy"
#endif

class CSexecflowClass
{
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: CSoperationsClass CSoperations;
	private: CSdrawClass CSdraw;
	private: LDsvgClass LDsvg;
	private: LDopenglClass LDopengl;
	private: LDparserClass LDparser;
	private: RTpixelMapsClass RTpixelMaps;
	private: CSgenerateHTMLdocumentationClass CSgenerateHTMLdocumentation;
	private: CSgenerateObjectOrientedCodeClass CSgenerateObjectOrientedCode;
	private: CSgenerateConstFunctionArgumentCodeClass CSgenerateConstFunctionArgumentCode;
	public: void generateCodeStructure(const string topLevelFileName, int width, const int height, const string outputLDRfileName, const string outputSVGfileName, const string outputPPMfileName, string outputHTMLfileName, const bool useOutputLDRfile, const bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, const bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, const bool outputFileConnections, const string topLevelFunctionName, const bool generateOOcode, const bool generateConstFunctionArgumentsCode);
};


#endif
