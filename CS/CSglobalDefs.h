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
 * File Name: CSglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3e1b 27-August-2014
 * Description: CS specific global definitions
 *
 *******************************************************************************/

#ifndef HEADER_CS_GLOBAL_DEFS
#define HEADER_CS_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

#define CS_GENERATE_CPP_CLASSES	//added 3e1a/27-August-2014
#ifdef CS_GENERATE_CPP_CLASSES
	#define CS_GENERATE_CPP_CLASSES_SOURCE_FILE_EXTENSION "cpp"
	#define CS_GENERATE_CPP_CLASSES_HEADER_FILE_EXTENSION "hpp"
	#define CS_GENERATE_CPP_CLASSES_FUNCTION_PRIVATE "private: "
	#define CS_GENERATE_CPP_CLASSES_FUNCTION_PUBLIC "public: "
	#define CS_GENERATE_CPP_CLASSES_CLASS_PERMISSIONS_IDENTIFIER "::"	//class membership
	#define CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_LOCAL "this->"
	#define CS_GENERATE_CPP_CLASSES_FUNCTION_REFERENCE_CONTEXT_DELIMITER "."	//OLD: "->"
	#define CS_GENERATE_CPP_CLASSES_CLASS_OBJECT_APPENDITION ""	//OLD: "Obj"
	#define CS_GENERATE_CPP_CLASSES_CLASS_DECLARATION_APPENDITION "Class"	//OLD: ""
	#define CS_GENERATE_CPP_CLASSES_CLASS_HEADER_PART1 "class "
	#define CS_GENERATE_CPP_CLASSES_CLASS_HEADER_PART2 "\n{\n"
	#define CS_GENERATE_CPP_CLASSES_CLASS_FOOTER "\n};"
	#define CS_GENERATE_CPP_CLASSES_FUNCTION_MAIN_NAME "main"
#endif

#define CS_SUPPORT_PREDEFINED_GRID

#define CS_WRITE_SVG_INKSCAPE_CONNECTORS
#ifdef CS_WRITE_SVG_INKSCAPE_CONNECTORS
	#define CS_WRITE_SVG_GROUPS
#endif

//#define CS_CONVERT_INDENTED_LIST_TO_HTML_LIST					//temporary compile condition for class HTML documentation generation

//#define CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT	//temporary compile condition for class HTML documentation generation
#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	#define CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT_COMBINED	//use project wide CSclass files
#endif


#define CS_IGNORE_COMMENTS_IN_FUNCTIONS

#define CS_SUPPORT_FUNCTION_RETURN_POINTERS

#define CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
	//#define CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS_UPDATES_MAY_NOT_BE_REQUIRED
#endif

#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
		#define CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
		#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
	#endif
#endif
#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
	#define CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION

#endif

#define CS_DRAW_APPLY_EVEN_ODD_X_OFFSET

//#define CS_DEBUG
//#define CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
//#define CS_DEBUG_VERBOSE
//#define CS_DEBUG_HTML_DOCUMENTATION
#define CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_SUMMARY

//#define CS_USE_RAINBOW_COLOURS_FOR_FILE_BOXES		//optional
//#define CS_USE_RAINBOW_COLOURS_FOR_FUNCTION_BOXES	//optional
#define CS_DISPLAY_INCLUDE_FILE_PARSING
#define CS_CENTRE_BOXES
#define CS_DO_NOT_DRAW_ALL_FUNCTION_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
//#define CS_DO_NOT_DRAW_ALL_FUNCTION_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
//#define CS_DO_NOT_DRAW_ALL_FILE_CONNECTIONS_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS
//#define CS_DO_NOT_DRAW_ALL_FILE_BOXES_AND_TEXT_WHEN_TRACING_A_BOTTOM_LEVEL_FUNCTION_UPWARDS

#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX -400
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY -100
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX 10000
#define CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY 3500	//CS <=3d2c: 2000
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINX -300
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINY 0
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXX 1000
#define CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXY 300

#define HTML_EXTENSION ".html"
#define SVG_EXTENSION ".svg"

#define CS_SOURCE_FILE_EXTENSION "c"
#define CS_SOURCE_FILE_INDENTATION_CHARACTER '\t'

#endif
