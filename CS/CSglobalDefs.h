/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3c9a 06-February-2014
 * Description: CS specific global definitions
 *
 *******************************************************************************/

#ifndef HEADER_CS_GLOBAL_DEFS
#define HEADER_CS_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

#define CS_SUPPORT_PREDEFINED_GRID

#define CS_WRITE_SVG_INKSCAPE_CONNECTORS
#ifdef CS_WRITE_SVG_INKSCAPE_CONNECTORS
	#define CS_WRITE_SVG_GROUPS
#endif

//#define CS_CONVERT_INDENTED_LIST_TO_HTML_LIST					//temporary compile condition for class HTML documentation generation

#define CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT	//temporary compile condition for class HTML documentation generation
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

#endif
