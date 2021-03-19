/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSgenerateHTMLdocumentation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3p3a 19-March-2021
 * /
 *******************************************************************************/


#include "CSgenerateHTMLdocumentation.hpp"


//generate documentation for all functions...
void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctions(XMLparserTag* firstTagInSVGFile, CSfileContainer* firstObjectInTopLevelBelowListContainer, int generateHTMLdocumentationMode, const bool useOutputHTMLfile, const bool traceFunctionUpwards, bool usePredefinedGrid, const string outputHTMLfileName)
{
	//find last tag in svg file;
	XMLparserTag* lastTagInSVGFile = firstTagInSVGFile;
	XMLparserTag* tempTagInSVGFile = firstTagInSVGFile;
	while(tempTagInSVGFile->nextTag != NULL)
	{
		lastTagInSVGFile = tempTagInSVGFile;
		tempTagInSVGFile = tempTagInSVGFile->nextTag;
	}

	string HTMLdocumentationBody = "";
	LDreference* firstReferenceInPrintListFunction = new LDreference();
	generateHTMLdocumentationForAllFunctions(firstObjectInTopLevelBelowListContainer, firstReferenceInPrintListFunction, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, &HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceFunctionUpwards, usePredefinedGrid);
	delete firstReferenceInPrintListFunction;	//should delete this, as it will contain far too many LD vector graphics references (ie a set of traced references for each function)

	if(useOutputHTMLfile)
	{//use single html file for project (rather than unique html file per file in project)
	
		string HTMLdocumentationHeader = generateHTMLdocumentationHeader("Software Project", true, false);
		string HTMLdocumentationFooter = generateHTMLdocumentationFooter(true);
		
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY
		string HTMLdocumentationProjectCodeStructureDiagrams = "";
		string codeStructureFilesSVGfileName = CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY_FILES_FILE_NAME;
		string HTMLdocumentationFileImagePlaceHolder = generateHTMLdocumentationImagePlaceHolder(&codeStructureFilesSVGfileName, "Software Project Code Structure - Files");	//Software Project Code Structure Diagram
		HTMLdocumentationProjectCodeStructureDiagrams = HTMLdocumentationProjectCodeStructureDiagrams + HTMLdocumentationFileImagePlaceHolder;
		string codeStructureFunctionsSVGfileName = CS_GENERATE_HTML_DOCUMENTATION_PRINT_PROJECT_CODE_STRUCTURE_HIERARCHY_FUNCTIONS_FILE_NAME;
		HTMLdocumentationFileImagePlaceHolder = generateHTMLdocumentationImagePlaceHolder(&codeStructureFunctionsSVGfileName, "Software Project Code Structure - Functions");	//Software Project Code Structure Diagram (with Functions)
		HTMLdocumentationProjectCodeStructureDiagrams = HTMLdocumentationProjectCodeStructureDiagrams + HTMLdocumentationFileImagePlaceHolder;					
		HTMLdocumentationHeader = HTMLdocumentationHeader + HTMLdocumentationProjectCodeStructureDiagrams;
		#endif
		
		string HTMLdocumentation = HTMLdocumentationHeader + HTMLdocumentationBody + HTMLdocumentationFooter;
		
		ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
		writeStringPointerToFileObject(&HTMLdocumentation, &writeFileObjectHTML);
	}
}

string CSgenerateHTMLdocumentationClass::generateHTMLdocumentationHeader(const string name, const bool htmlHeader, const bool isFile)
{
	return generateHTMLdocumentationHeader(name, name, htmlHeader, isFile);
}
string CSgenerateHTMLdocumentationClass::generateHTMLdocumentationHeader(const string titleName, const string fileName, const bool htmlHeader, const bool isFile)
{
	string HTMLdocumentationHeader = "";
		
	if(htmlHeader)
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<html><head><title>" + titleName + " Documentation</title><style type=\"text/css\">TD { font-size:75%; } </style></head><body><h3>" + titleName + " Documentation</h3><p>Automatically generated with Code Structure Viewer (CS), Project Version: 3p3a 19-March-2021</p>\n";
		#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_ADD_COPYRIGHT
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p>Copyright &copy; 2020, BAI Research. All Rights Reserved.</p>\n";
		#endif
	}
	else
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<h3>" + titleName + " Documentation</h3>\n";
	}
	if(isFile)
	{		
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_BASE_TITLE
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_SOURCE_AND_HEADER
		string HTMLdocumentationFileNameSource = fileName + CS_FILE_EXTENSION_DELIMITER + CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION + "/" + CS_FILE_EXTENSION_DELIMITER + CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION;
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p><b>File Name:</b> " + HTMLdocumentationFileNameSource + "</p>";
		#endif
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_SOURCE
		string HTMLdocumentationFileNameSource = fileName + CS_FILE_EXTENSION_DELIMITER + CS_GENERATE_CODE_GENERIC_SOURCE_FILE_EXTENSION;
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p><b>Source File Name :</b> " + HTMLdocumentationFileNameSource + "</p>";
		#endif
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_HEADER
		string HTMLdocumentationFileNameHeader = fileName + CS_FILE_EXTENSION_DELIMITER + CS_GENERATE_CODE_GENERIC_HEADER_FILE_EXTENSION;
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p><b>Header File Name :</b> " + HTMLdocumentationFileNameHeader + "</p>";
		#endif
		string fileNameWithoutExtension = fileName;
		#else
		int indexOfFileExtension = fileName.find(CS_FILE_EXTENSION_DELIMITER);
		string fileNameWithoutExtension = fileName.substr(0, indexOfFileExtension);	//CSoperationsClass::generateFileNameBaseFromHeaderFileName
		#endif
		string HTMLdocumentationFileDescription = createDescriptionFromCaseSensitiveMultiwordString(fileNameWithoutExtension);
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p><b>File Description:</b> " + HTMLdocumentationFileDescription + "</p>";
	}
	return HTMLdocumentationHeader;
}

string CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFooter(const bool htmlFileFooter)
{
	string HTMLdocumentationFooter = "";
	if(htmlFileFooter)
	{
		HTMLdocumentationFooter = "</body></html>\n";
	}
	else
	{
		HTMLdocumentationFooter = "\n";
	}
	return HTMLdocumentationFooter;
}

void CSgenerateHTMLdocumentationClass::writeStringPointerToFileObject(string* s, ofstream* writeFileObject)
{
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject->put((*s)[i]);
	}
}

void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationForAllFunctions(CSfileContainer* firstObjectInAboveLevelBelowListContainer, LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, int generateHTMLdocumentationMode, const bool useOutputHTMLfile, string* HTMLdocumentationBody, const XMLparserTag* firstTagInSVGFile, XMLparserTag* lastTagInSVGFile, const bool traceFunctionUpwards, bool usePredefinedGrid)
{
	bool result = true;

	CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		CSfile* currentFileObject = currentFileObjectContainer->fileObject;

		string fileNameHoldingFunction = currentFileObject->name;
		
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_BASE_TITLE
		string outputHTMLfileName = currentFileObject->nameBase + HTML_EXTENSION;
		#else
		string outputHTMLfileName = currentFileObject->name + HTML_EXTENSION;
		#endif
		
		ifstream tempFileStream(outputHTMLfileName.c_str());

		bool passed = false;
		if(!useOutputHTMLfile)
		{
			if(!(tempFileStream.good()))
			{//file does not exist
				passed = true;
			}
		}
		else
		{
			passed = true;
		}
		if(passed)
		{

			if(!(currentFileObject->HTMLgenerated))	//this constraint doesnt work as planned
			{//do not parse a file twice
				currentFileObject->HTMLgenerated = true;

				if(!useOutputHTMLfile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					*HTMLdocumentationBody = "";
				}
				string HTMLdocumentationFileBody = "";
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
				string HTMLdocumentationFileFunctionList = "<p>\n<h4>File Function List</h4>\n<ul><li>";
				int previousIndentation = 0;
				bool previousIndentationFirst = true;
				#endif
				bool HTMLgeneratedSafe = false;

				CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
				while(currentFunctionObject->next != NULL)
				{
					if(currentFunctionObject->printed)
					{//only create connections to printed boxes

						if(!(currentFunctionObject->HTMLgenerated))
						{//do not parse a function twice
							HTMLgeneratedSafe = true;
							currentFunctionObject->HTMLgenerated = true;

							string outputSVGfileNameFunction = "";	//only used with traceFunctionUpwards
							XMLparserTag* firstTagInSVGFileFunction = new XMLparserTag();	//only used with traceFunctionUpwards
							XMLparserTag* currentTagInSVGFileFunction = firstTagInSVGFileFunction;
							if(traceFunctionUpwards)
							{
								outputSVGfileNameFunction = currentFunctionObject->name + SVG_EXTENSION;
								#ifdef CS_GENERATE_HTML_DOCUMENTATION_OUTPUT_FILENAMES_WITH_CLASS
								string classObjectNameBase = currentFunctionObject->className;
								if(CSclassClassObject.getClassNameBase(currentFunctionObject->className, &classObjectNameBase))
								{									
									outputSVGfileNameFunction = classObjectNameBase + STRING_FULLSTOP + currentFunctionObject->name + SVG_EXTENSION;
								}
								#endif
							}

							string HTMLdocumentationFunction = "";
							generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, currentFunctionObject, fileNameHoldingFunction, &currentTagInSVGFileFunction, generateHTMLdocumentationMode, &HTMLdocumentationFunction, &outputSVGfileNameFunction, false, "", traceFunctionUpwards);
							HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationFunction;

							if(traceFunctionUpwards)
							{
								lastTagInSVGFile->nextTag = firstTagInSVGFileFunction;
								if(!LDsvg.writeSVGfile(outputSVGfileNameFunction, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
								{
									result = false;
								}
								lastTagInSVGFile->nextTag = NULL;
								delete firstTagInSVGFileFunction;
							}

							#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
							#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_LIST_WITH_CLASS
							bool externalFunction = true;
							#else
							bool externalFunction = false;
							#endif
							addToHTMLdocumentationFileFunctionList(currentFunctionObject, &HTMLdocumentationFileFunctionList, &previousIndentation, &previousIndentationFirst, true, externalFunction);
							#endif
						}
					}

					currentFunctionObject = currentFunctionObject->next;
				}

				string HTMLdocumentationFileIntroduction = "";
				
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_LIST_WITH_INDENTATION
				for(int i=0; i<previousIndentation; i++)
				{
					HTMLdocumentationFileFunctionList = HTMLdocumentationFileFunctionList + "</li></ul>";
				}
				HTMLdocumentationFileFunctionList = HTMLdocumentationFileFunctionList + "</li></ul>\n";
				HTMLdocumentationFileIntroduction = HTMLdocumentationFileIntroduction + HTMLdocumentationFileFunctionList;
				#endif
				
				#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
				#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_BASE_TITLE
				string outputSVGFileNameFile = currentFileObject->nameBase + SVG_EXTENSION;
				#else
				string outputSVGFileNameFile = currentFileObject->name + SVG_EXTENSION;
				#endif
				generateFileDiagramFunctionsHeirachy(currentFileObject, outputSVGFileNameFile, firstObjectInTopLevelBelowListContainer, usePredefinedGrid);
				string HTMLdocumentationFileImagePlaceHolder = generateHTMLdocumentationImagePlaceHolder(&outputSVGFileNameFile, "File Diagram (functions hierarchy)");
				HTMLdocumentationFileIntroduction = HTMLdocumentationFileIntroduction + HTMLdocumentationFileImagePlaceHolder;
				#endif
				
				#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_AUTOMATICALLY
				string HTMLdocumentationFileClassList = "\n<p>\n";	//<h4>File Class List</h4>\n
				for(int i=0; i<currentFileObject->classList.size(); i++)
				{
					CSclass* classObject = (currentFileObject->classList)[i];
					
					#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS_FUNCTIONS_IGNORE_PRIMARY_FILE_CLASS
					if(!(classObject->primaryFileClass))
					{
					#endif
						HTMLdocumentationFileClassList = HTMLdocumentationFileClassList + "<h4>" + CS_CLASS_NAME_APPEND + STRING_SPACE + classObject->name + "</h4>\n";

						string HTMLdocumentationParameterList = "";
						generateHTMLdocumentationFunctionOrClassArgumentsFromParameterList(true, &(classObject->parameterList), &HTMLdocumentationParameterList);

						HTMLdocumentationFileClassList = HTMLdocumentationFileClassList + HTMLdocumentationParameterList + "\n";
					#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS_FUNCTIONS_IGNORE_PRIMARY_FILE_CLASS
					}
					#endif	
				}
				HTMLdocumentationFileClassList = HTMLdocumentationFileClassList + "</p>\n";
				//cout << "HTMLdocumentationFileClassList = " << HTMLdocumentationFileClassList << endl;
				HTMLdocumentationFileIntroduction = HTMLdocumentationFileIntroduction + HTMLdocumentationFileClassList;
				#endif
				
				HTMLdocumentationFileBody = HTMLdocumentationFileIntroduction + HTMLdocumentationFileBody;

				#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_BASE_TITLE
				string fileNameToPrint = currentFileObject->nameBase;
				#else
				string fileNameToPrint = currentFileObject->name;
				#endif
				string fileNameTitleToPrint = string("File ") + fileNameToPrint;
				
				string HTMLdocumentationFileHeader = generateHTMLdocumentationHeader(fileNameTitleToPrint, fileNameToPrint, !useOutputHTMLfile, true);
				string HTMLdocumentationFileFooter = generateHTMLdocumentationFooter(!useOutputHTMLfile);
				string HTMLdocumentationFile = "";
				HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;


				if(!useOutputHTMLfile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FILE_NAME_BASE_TITLE
					string outputHTMLfileName = currentFileObject->nameBase + HTML_EXTENSION;
					#else
					string outputHTMLfileName = currentFileObject->name + HTML_EXTENSION;
					#endif
					ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
					writeStringPointerToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);
				}
				else
				{
					if(HTMLgeneratedSafe)
					{
						*HTMLdocumentationBody = *HTMLdocumentationBody + HTMLdocumentationFile;
					}
				}

				if(currentFileObject->firstFileInBelowListContainer != NULL)
				{
					generateHTMLdocumentationForAllFunctions(currentFileObject->firstFileInBelowListContainer, currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceFunctionUpwards, usePredefinedGrid);
				}
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}
}

void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationForFunction(LDreference* currentReferenceInPrintList, CSfileContainer* firstObjectInTopLevelBelowListContainer, CSfunction* bottomLevelFunctionToTraceUpwards, const string fileNameHoldingFunction, XMLparserTag** currentTag, int generateHTMLdocumentationMode, string* HTMLdocumentationFunctionBody, const string* outputSVGfileNameFunction, const bool useOutputHTMLfile, const string outputHTMLfileName, const bool traceFunctionUpwards)
{
	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
	{
		#ifdef CS_DISPLAY_INCLUDE_FILE_PARSING
		cout << "\nStart Trace upwards: \t\t" << fileNameHoldingFunction << "\t\t" << bottomLevelFunctionToTraceUpwards->name << "{}" << endl;
		cout << bottomLevelFunctionToTraceUpwards->nameFull << endl;
		#endif
	}

	string HTMLdocumentationFunctionTraceTableRows = "";
	if(traceFunctionUpwards)
	{
		currentReferenceInPrintList = CSdraw.traceFunctionsUpwardsAndDrawOrHighLightThese(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, bottomLevelFunctionToTraceUpwards, currentTag, generateHTMLdocumentationMode, &HTMLdocumentationFunctionTraceTableRows);

		CSdraw.traceFunctionsUpwardsAndDrawOrHighLightTheseReset(firstObjectInTopLevelBelowListContainer, bottomLevelFunctionToTraceUpwards);	//this is required for multiple traces per CS execution
	}

	if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
	{

		string HTMLdocumentationFunctionTitle = "";
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_NAME_WITH_CLASS
		bool functionHasClass = true;
		if(bottomLevelFunctionToTraceUpwards->className == "")
		{
			functionHasClass = false;
		}
		string functionNameTitleToPrint = string("Function ") + generateFunctionString(bottomLevelFunctionToTraceUpwards, true, functionHasClass) + CS_GENERATE_CLASS_HTML_DOCUMENTATION_FUNCTION_NAME_APPEND; 
		#else
		string functionNameTitleToPrint = string("Function ") + bottomLevelFunctionToTraceUpwards->name + CS_GENERATE_CLASS_HTML_DOCUMENTATION_FUNCTION_NAME_APPEND;
		#endif
		HTMLdocumentationFunctionTitle = HTMLdocumentationFunctionTitle + "<h4>" + functionNameTitleToPrint + "</h4>";
		string HTMLdocumentationFunctionSummary = "";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_SUMMARY
		generateHTMLdocumentationFunctionSummary(bottomLevelFunctionToTraceUpwards, &HTMLdocumentationFunctionSummary);
		#endif

		string HTMLdocumentationFunctionInputArguments = "";
		generateHTMLdocumentationFunctionInputArguments(bottomLevelFunctionToTraceUpwards, &HTMLdocumentationFunctionInputArguments);

		string HTMLdocumentationFunctionReferenceList = "";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
		generateHTMLdocumentationFunctionReferenceList(bottomLevelFunctionToTraceUpwards, &HTMLdocumentationFunctionReferenceList);
		#endif

		*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody + HTMLdocumentationFunctionTitle + HTMLdocumentationFunctionSummary + HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionReferenceList + "<br />";

		if(traceFunctionUpwards)
		{
			string HTMLdocumentationFunctionTraceTableHeader = "";
			string HTMLdocumentationFunctionTraceTableFooter = "";
			HTMLdocumentationFunctionTraceTableHeader = HTMLdocumentationFunctionTraceTableHeader + "\t<b>Function Trace</b><br /><table border=\"1\"><tr><th>" + "location" + "</th><th>" + "current function being traced" + "</th></tr>\n";
			HTMLdocumentationFunctionTraceTableFooter = HTMLdocumentationFunctionTraceTableFooter + "\t</table>";
			string HTMLdocumentationFunctionTraceTable = HTMLdocumentationFunctionTraceTableHeader + HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableFooter;

			string HTMLdocumentationFunctionTraceImagePlaceHolder = generateHTMLdocumentationImagePlaceHolder(outputSVGfileNameFunction, "Trace Diagram (magenta)");

			*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody +  HTMLdocumentationFunctionTraceTable + HTMLdocumentationFunctionTraceImagePlaceHolder;
		}

		if(useOutputHTMLfile)
		{//use single html file for function
			string HTMLdocumentationFunctionHeader = generateHTMLdocumentationHeader(functionNameTitleToPrint, true, false);
			string HTMLdocumentationFunctionFooter = generateHTMLdocumentationFooter(true);
			string HTMLdocumentationFunction = HTMLdocumentationFunctionHeader +* HTMLdocumentationFunctionBody + HTMLdocumentationFunctionFooter;

			ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
			writeStringPointerToFileObject(&HTMLdocumentationFunction, &writeFileObjectHTML);
		}
	}


}

void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionSummary(const CSfunction* currentFunction, string* HTMLdocumentationFunctionSummary)
{
	string functionName = currentFunction->name;
	string functionNameFull = currentFunction->nameFull;
	
	*HTMLdocumentationFunctionSummary = "";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t<p><h5>Function Summary</h5><table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "return type" + "</th><th>" + "description" + "</th></tr>\n";
	string HTMLdocumentationFunctionDescription = createDescriptionFromCaseSensitiveMultiwordString(functionName);
	int endPositionOfReturnType = functionNameFull.find(functionName);
	int startPositionOfReturnType = 0;
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE
	string HTMLdocumentationFunctionReturnType = currentFunction->functionType;
	#else
	string HTMLdocumentationFunctionReturnType = functionNameFull.substr(startPositionOfReturnType, endPositionOfReturnType-startPositionOfReturnType);
	#endif
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t\t<tr><td>" + functionName + "</td><td>" + HTMLdocumentationFunctionReturnType + "</td><td>" + HTMLdocumentationFunctionDescription + "</td></tr>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</table>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</p>\n";
}

void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionInputArguments(const CSfunction* currentFunction, string* HTMLdocumentationFunctionInputArguments)
{
	#ifdef CS_USE_FUNCTION_ARGUMENTS_PARAMETER_LIST
	return generateHTMLdocumentationFunctionOrClassArgumentsFromParameterList(false, &(currentFunction->functionArgumentsParameterList), HTMLdocumentationFunctionInputArguments);
	#else
	#ifdef CS_USE_FUNCTION_ARGUMENTS_STRING
	return generateHTMLdocumentationFunctionOrClassArgumentsFromString(false, currentFunction->functionArguments, HTMLdocumentationFunctionInputArguments);
	#else
	return generateHTMLdocumentationFunctionOrClassArgumentsFromFunctionNameFull(false, &(currentFunction->nameFull), HTMLdocumentationFunctionInputArguments);
	#endif
	#endif
}
void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionOrClassArgumentsFromFunctionNameFull(const bool classDataOrFunctionArguments, const string* functionNameFull, string* HTMLdocumentationFunctionInputArguments)
{
	int startPositionOfFunctionBrackets = functionNameFull->find(CHAR_OPEN_BRACKET);
	int endPositionOfFunctionBrackets = functionNameFull->find(CHAR_CLOSE_BRACKET);
	bool functionHasArguments = false;
	if(startPositionOfFunctionBrackets != endPositionOfFunctionBrackets-1)
	{
		functionHasArguments = true;
	}
	if(functionHasArguments)
	{
		int startPositionOfFunctionArguments = startPositionOfFunctionBrackets+1;
		int endPositionOfFunctionArguments = endPositionOfFunctionBrackets;
		string functionArgumentsRaw = functionNameFull->substr(startPositionOfFunctionArguments, endPositionOfFunctionArguments-startPositionOfFunctionArguments+1);
	
		CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionOrClassArgumentsFromString(classDataOrFunctionArguments, functionArgumentsRaw, HTMLdocumentationFunctionInputArguments);
	}
}
void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionOrClassArgumentsFromString(const bool classDataOrFunctionArguments, const string functionArgumentsRaw, string* HTMLdocumentationFunctionInputArguments)
{
	*HTMLdocumentationFunctionInputArguments = "";
	if(classDataOrFunctionArguments)
	{
		*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<h5>Class Data</h5>";
	}
	else
	{
		*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<h5>Function Arguments</h5>";
	}
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "<table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th></tr>\n";

	bool stillParsingArguments = true;
	int startPositionOfArgument = 0;
	while(stillParsingArguments)
	{
		bool lastArgument = false;
		int endPositionOfArgument = CSreferenceContainerClass.findEndPositionOfArgument(&functionArgumentsRaw, startPositionOfArgument, &lastArgument);
		if(lastArgument)
		{
			stillParsingArguments = false;
		}

		string currentArgument = functionArgumentsRaw.substr(startPositionOfArgument, endPositionOfArgument-startPositionOfArgument);

		int startPositionOfArgumentName = currentArgument.rfind(CHAR_SPACE) + 1;	//last space
		if(startPositionOfArgumentName == CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			cerr << "generateHTMLdocumentationFunctionInputArguments{} error: (startPositionOfArgumentName == CPP_STRING_FIND_RESULT_FAIL_VALUE)" << endl;
			exit(EXIT_ERROR);
		}
		string currentArgumentName = currentArgument.substr(startPositionOfArgumentName, endPositionOfArgument-startPositionOfArgumentName);
		string currentArgumentType = currentArgument.substr(0, startPositionOfArgumentName);
		string currentArgumentDescription = createDescriptionFromCaseSensitiveMultiwordString(currentArgumentName);

		currentArgumentName = ensureHTMLTagSafe(currentArgumentName);
		currentArgumentType = ensureHTMLTagSafe(currentArgumentType);
		currentArgumentDescription = ensureHTMLTagSafe(currentArgumentDescription);

		string HTMLdocumentationFunctionInputArgumentsRow = "\t\t<tr><td>" + currentArgumentName + "</td><td>" + currentArgumentType + "</td><td>" + currentArgumentDescription + "</td></tr>\n";
		*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionInputArgumentsRow;

		startPositionOfArgument = endPositionOfArgument+1;
	}
	
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t</table>\n";
}


#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS
void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionOrClassArgumentsFromParameterList(const bool classDataOrFunctionArguments, vector<CSparameter*>* parameterList, string* HTMLdocumentationParameterList)
{
	*HTMLdocumentationParameterList = "";
	
	if(classDataOrFunctionArguments)
	{
		*HTMLdocumentationParameterList = *HTMLdocumentationParameterList + "\t<h5>Class Data</h5>";
	}
	else
	{
		*HTMLdocumentationParameterList = *HTMLdocumentationParameterList + "\t<h5>Function Arguments</h5>";
	}
	#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_AUTOMATICALLY_CLASS_PARAMETER_FUNCTIONS
	*HTMLdocumentationParameterList = *HTMLdocumentationParameterList + "<table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th><th>" + "functionArguments" + "</th></tr>\n";
	#else
	*HTMLdocumentationParameterList = *HTMLdocumentationParameterList + "<table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th></tr>\n";
	#endif
	
	for(int i=0; i<parameterList->size(); i++)
	{
		CSparameter* parameterObject = (*parameterList)[i];
		
		bool parameterTypeCorrect = true;
		if(classDataOrFunctionArguments)
		{
			#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS_FUNCTIONS
			#ifndef CS_GENERATE_CLASS_HTML_DOCUMENTATION_AUTOMATICALLY_CLASS_PARAMETER_FUNCTIONS
			if(parameterObject->isFunction)
			{
				parameterTypeCorrect = false;
			}
			#endif
			#endif
		}
		else
		{
			//assume parameter list only contains functions
		}
		if(parameterTypeCorrect)
		{
			string currentParameterName = parameterObject->name;
			string currentParameterType = parameterObject->type;
			string currentParameterDescription = createDescriptionFromCaseSensitiveMultiwordString(currentParameterName);
			string currentParameterFunctionArguments = "";
			
			currentParameterName = ensureHTMLTagSafe(currentParameterName);
			currentParameterType = ensureHTMLTagSafe(currentParameterType);
			currentParameterDescription = ensureHTMLTagSafe(currentParameterDescription);

			#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_AUTOMATICALLY_CLASS_PARAMETER_FUNCTIONS
			if(parameterObject->isFunction)
			{
				currentParameterName = currentParameterName + CS_GENERATE_CLASS_HTML_DOCUMENTATION_FUNCTION_NAME_APPEND;	//indicate that the class parameter is a function
				
				bool firstFunctionArgument = true;
				for(int j=0;j<parameterObject->functionArgumentsParameterList.size(); j++)
				{
					if(!firstFunctionArgument)
					{
						currentParameterFunctionArguments = currentParameterFunctionArguments + STRING_COMMA;	
					}
					CSparameter* functionArgument = (parameterObject->functionArgumentsParameterList)[j];
					currentParameterFunctionArguments = currentParameterFunctionArguments + functionArgument->type + STRING_SPACE + functionArgument->name;
				}
				
				currentParameterFunctionArguments = ensureHTMLTagSafe(currentParameterFunctionArguments);
			}
			string HTMLdocumentationParameterListRow = "\t\t<tr><td>" + currentParameterName + "</td><td>" + currentParameterType + "</td><td>" + currentParameterDescription + "</td><td>" + currentParameterFunctionArguments + "</td></tr>\n";	
			#else
			string HTMLdocumentationParameterListRow = "\t\t<tr><td>" + currentParameterName + "</td><td>" + currentParameterType + "</td><td>" + currentParameterDescription + "</td></tr>\n";	
			#endif

			*HTMLdocumentationParameterList = *HTMLdocumentationParameterList + HTMLdocumentationParameterListRow;	
		}	
	}

	*HTMLdocumentationParameterList = *HTMLdocumentationParameterList + "\t</table>\n";
}
#endif


string CSgenerateHTMLdocumentationClass::ensureHTMLTagSafe(const string str)
{
	string htmlSafeString = "";
	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == CLASS_TYPE_OPEN_TAG)
		{
			htmlSafeString = htmlSafeString + HTML_TAG_OPEN;
		}
		else if(str[i] == CLASS_TYPE_CLOSE_TAG)
		{
			htmlSafeString = htmlSafeString + HTML_TAG_CLOSE;
		}
		else
		{
			htmlSafeString = htmlSafeString + str[i];
		}
	}
	return htmlSafeString;
}

/*
createDescriptionFromCaseSensitiveMultiwordString();
split file/function names (eg thisThing) into pseudo file/function descriptions (eg this thing)
	add a space after l of instance ..lU...
	add a space before l of instance ..UUl...
*/
string CSgenerateHTMLdocumentationClass::createDescriptionFromCaseSensitiveMultiwordString(string str)
{
	string description = "";
	for(int i=0; i<str.length(); i++)
	{
		bool removeLowerCase = false;
		if(i >= 2)
		{
			string alreadyExtractedDescription = description.substr(0, i-2);
			char currentChar1 = str[i];
			char currentChar2 = str[i-1];
			char currentChar3 = str[i-2];
			bool currentChar1IsUpper = !(bool(islower(currentChar1)));	//Changed 19 November 2012: consider numbers as upper case also (http://www.cplusplus.com/reference/clibrary/cctype/islower/)
			bool currentChar2IsUpper = !(bool(islower(currentChar2)));	//Changed 19 November 2012: consider numbers as upper case also (http://www.cplusplus.com/reference/clibrary/cctype/islower/)
			bool currentChar3IsUpper = !(bool(islower(currentChar3)));	//Changed 19 November 2012: consider numbers as upper case also (http://www.cplusplus.com/reference/clibrary/cctype/islower/)
			if(currentChar1IsUpper && !currentChar2IsUpper)
			{
				//aA
				description = description + CHAR_SPACE;
				if(i < str.length()-1)
				{
					char nextChar = str[i+1];
					if(islower(nextChar))
					{//aAa
						removeLowerCase = true;
					}
				}
			}
			else if(!currentChar1IsUpper && currentChar2IsUpper && currentChar3IsUpper)
			{
				//ABa
				description = description + CHAR_SPACE;
			}
		}
		char characterToAdd = str[i];
		if(removeLowerCase)
		{
			characterToAdd = tolower(characterToAdd); 	//Changed 19 November 2012: reduce all non abbreviations to lower case (ie all strings that do not wholy consist of uppercase/numerical characters)
		}
		description = description + characterToAdd;
	}
	return description;
}




void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionReferenceList(CSfunction* function, string* HTMLdocumentationFunctionReferenceList)
{
	//generate list

	*HTMLdocumentationFunctionReferenceList = "";
	*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "\t<p><h5>Function Reference List</h5>\n";
	CSfunction* currentReferenceInFunctionReferenceListRepeats = function->firstReferenceInFunctionReferenceListRepeats;
	string HTMLdocumentationFunctionObjectListBody = "\t<ul>\n";
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
	HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\t<li>";
	int previousIndentation = 0;
	bool previousIndentationFirst = true;
	#endif
	bool foundReferences = false;

	map<int, CSfunction*> CSfunctionReferenceListOrdered;

	/*
	if(function->name == "createImage")
	{
		cout << "function = " << function->name << endl;
	}
	*/
	while(currentReferenceInFunctionReferenceListRepeats->next != NULL)
	{

		foundReferences = true;

		CSfunctionReferenceListOrdered.insert(pair<int, CSfunction*>(currentReferenceInFunctionReferenceListRepeats->functionReferenceCharacterIndex, currentReferenceInFunctionReferenceListRepeats));

		currentReferenceInFunctionReferenceListRepeats = currentReferenceInFunctionReferenceListRepeats->next;
	}

	for(map<int, CSfunction*>::iterator CSfunctionReferenceListOrderedIter = CSfunctionReferenceListOrdered.begin(); CSfunctionReferenceListOrderedIter != CSfunctionReferenceListOrdered.end(); CSfunctionReferenceListOrderedIter++)
	{
		CSfunction* currentReferenceInFunctionReferenceList = CSfunctionReferenceListOrderedIter->second;

		bool externalFunction = true;
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_NAME_WITH_CLASS
		if(function->className == currentReferenceInFunctionReferenceList->className)
		{
			externalFunction = false;
		}
		#endif
		
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
		/*
		if(function->name == "createImage")
		{
			cout << "currentReferenceInFunctionReferenceList; " << currentReferenceInFunctionReferenceList->name << ", = " << currentReferenceInFunctionReferenceList->functionReferenceIndentation << endl;
		}
		*/
		addToHTMLdocumentationFileFunctionList(currentReferenceInFunctionReferenceList, &HTMLdocumentationFunctionObjectListBody, &previousIndentation, &previousIndentationFirst, false, externalFunction);
	#else
		
		string functionReferenceName = currentReferenceInFunctionReferenceList->name;
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_NAME_WITH_CLASS
		functionReferenceName = generateFunctionString(currentReferenceInFunctionReferenceList, false, externalFunction);
		#endif
		
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\t\t<li>";
		for(int i=0; i<currentReferenceInFunctionReferenceList->functionReferenceIndentation; i++)
		{
			HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "&#160&#160&#160&#160&#160";
		}
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + functionReferenceName + "</li>\n";
		#else
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\t\t<li>" + functionReferenceName + "</li>\n";
		#endif
	#endif
	}

	/*
	if(function->name == "createImage")
	{
		exit(EXIT_ERROR);
	}
	*/

	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
	for(int i=0; i<previousIndentation; i++)
	{
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "</li></ul>";
	}
	HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\n\t</li>\n";
	#endif
	HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\t</ul>";

	if(foundReferences)
	{
		*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + HTMLdocumentationFunctionObjectListBody;
	}
	else
	{
		*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "N/A";
	}
	*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "</p>\n";

}

string CSgenerateHTMLdocumentationClass::generateHTMLdocumentationImagePlaceHolder(const string* traceImageFileName, const string imageTitle)
{
	string HTMLdocumentationFunctionTraceImagePlaceHolder = "\t<br /><h4>" + imageTitle + "</h4><br /><img width=\"1300px\" src=\"" +* traceImageFileName  + "\">\n";
	return HTMLdocumentationFunctionTraceImagePlaceHolder;
}



void CSgenerateHTMLdocumentationClass::addToHTMLdocumentationIndentedList(const string currentFunctionObjectName, int currentFunctionObjectIndentation, string* HTMLdocumentationFileFunctionList, int* previousIndentation, bool* previousIndentationFirst)
{
	string functionNameCompact = currentFunctionObjectName;	// + "{}";

	int differenceBetweenPreviousIndentation = currentFunctionObjectIndentation - *previousIndentation;
	string rawIndentationText = "\n";
	for(int i=0; i<currentFunctionObjectIndentation; i++)
	{
		rawIndentationText = rawIndentationText + "\t";
	}
	if(differenceBetweenPreviousIndentation > 0)
	{
		for(int i=0; i<differenceBetweenPreviousIndentation; i++)
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + rawIndentationText + "<ul><li>";
		}
		*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + functionNameCompact;
	}
	else if(differenceBetweenPreviousIndentation < 0)
	{
		for(int i=0; i<abs(differenceBetweenPreviousIndentation); i++)
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + "</li></ul>";
		}
		*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + "</li>" + rawIndentationText + "<li>" + functionNameCompact;
	}
	else
	{
		if(*previousIndentationFirst)
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + functionNameCompact;

		}
		else
		{
			*HTMLdocumentationFileFunctionList = *HTMLdocumentationFileFunctionList + "</li>" + rawIndentationText + "<li>" + functionNameCompact;
		}
	}
	*previousIndentationFirst = false;
	*previousIndentation = currentFunctionObjectIndentation;
}

void CSgenerateHTMLdocumentationClass::addToHTMLdocumentationFileFunctionList(CSfunction* currentFunctionObject, string* HTMLdocumentationFileFunctionList, int* previousIndentation, bool* previousIndentationFirst, const bool isFunctionOrFunctionReferenceList, const bool externalFunction)
{
	string functionReferenceName = currentFunctionObject->name;
	#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_NAME_WITH_CLASS
	functionReferenceName = generateFunctionString(currentFunctionObject, isFunctionOrFunctionReferenceList, externalFunction);
	#endif
	
	addToHTMLdocumentationIndentedList(functionReferenceName, currentFunctionObject->functionReferenceIndentation, HTMLdocumentationFileFunctionList, previousIndentation, previousIndentationFirst);
}




void CSgenerateHTMLdocumentationClass::generateFileDiagramFunctionsHeirachy(CSfile* currentFileObject, const string outputSVGFileNameFile, CSfileContainer* firstObjectInTopLevelBelowListContainer, bool usePredefinedGrid)
{
	bool result = true;

	ifstream tempFileStream(outputSVGFileNameFile.c_str());
	if(!(tempFileStream.good()))
	{//file does not exist

		CSdraw.initiateMaxXatParticularY();
		LDreference* firstReferenceInPrintList = new LDreference();
		LDreference* currentReferenceInPrintList = firstReferenceInPrintList;

		SHAREDvars.setCurrentDirectory(outputFolder);

		XMLparserTag* firstTagInSVGFile = new XMLparserTag();
		XMLparserTag* currentTagInSVGFile = firstTagInSVGFile;

		for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
		{
			currentFileObject->maxFunctionPrintXAtAParticularY[0] = 1;
		}

		CSfunction* currentTopLevelFunctionObject = currentFileObject->firstFunctionInFunctionList;
		while(currentTopLevelFunctionObject->next != NULL)
		{
			if(currentTopLevelFunctionObject->printed)
			{
				#ifdef CS_DISPLAY_FUNCTION_WHILE_GENERATING_DOCUMENTATION
				cout << currentTopLevelFunctionObject->name << endl;
				#endif
				
				//print function box for top level functions (added 3d3b);
				if(currentTopLevelFunctionObject->printYIndex == 0)
				{
					//correction applied with useSingleFileOnly
					currentTopLevelFunctionObject->printX = currentTopLevelFunctionObject->printX - currentFileObject->printX;
					currentTopLevelFunctionObject->printY = currentTopLevelFunctionObject->printY - currentFileObject->printY;

					currentReferenceInPrintList = CSdraw.printFunctionBox(currentReferenceInPrintList, &currentTagInSVGFile, currentTopLevelFunctionObject, currentTopLevelFunctionObject->col);
					currentReferenceInPrintList = CSdraw.printFunctionBoxText(currentReferenceInPrintList, &currentTagInSVGFile, currentTopLevelFunctionObject, currentTopLevelFunctionObject->col, true);

					currentTopLevelFunctionObject->printX = currentTopLevelFunctionObject->printX + currentFileObject->printX;
					currentTopLevelFunctionObject->printY = currentTopLevelFunctionObject->printY + currentFileObject->printY;
				}

				CSfunction* currentReferenceInFunctionReferenceList = currentTopLevelFunctionObject->firstReferenceInFunctionReferenceList;
				currentTopLevelFunctionObject->printedFunctionConnections = false;
				while(currentReferenceInFunctionReferenceList->next != NULL)
				{
					currentReferenceInPrintList = CSdraw.createFunctionObjectListBoxesAndConnections(currentReferenceInPrintList, currentFileObject, currentTopLevelFunctionObject, firstObjectInTopLevelBelowListContainer, 0, currentReferenceInFunctionReferenceList, &currentTagInSVGFile, false, true, &(currentFileObject->name), usePredefinedGrid, false);

					currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
				}
				CSdraw.resetPrintedFunctionConnections(currentFileObject, currentTopLevelFunctionObject, true, &(currentFileObject->name));
			}

			currentTopLevelFunctionObject = currentTopLevelFunctionObject->next;
		}

		char* outputFileNameSVGcharstar = const_cast<char*>(outputSVGFileNameFile.c_str());
		if(!LDsvg.writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINX, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXX, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MINY, CS_CODE_STRUCTURE_SINGLE_FILE_DIAGRAM_MAXY))
		{
			result = false;
		}
	}
}

#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
void CSgenerateHTMLdocumentationClass::generateClassHTMLdocumentationFromCustomCSclassFormat()
{
	bool result = true;
	for(int i=0; i<NUMBER_OF_CSCLASSES; i++)
	{
		string CSclassFileName = CSclassesArray[i];
		cout << "CSclassFileName = " << CSclassFileName << endl;

		ifstream CSclassFileObject(CSclassFileName.c_str());

		if(!CSclassFileObject.rdbuf( )->is_open( ))
		{
			//xml file does not exist in current directory.
			cout << "Error: CSclass File does not exist in current directory: " << CSclassFileName << endl;
			result = false;
		}
		else
		{	
			vector<string> CSclassFileContents;
						
			string currentClassBeingParsed = "";
			bool finishedParsingObject = false;
			char currentToken;
			while((currentToken = (&CSclassFileObject)->get()) != EOF)
			{
				if(currentToken == CHAR_NEWLINE)
				{
					CSclassFileContents.push_back(currentClassBeingParsed);
					currentClassBeingParsed = "";
				}
				else
				{
					currentClassBeingParsed = currentClassBeingParsed + currentToken;
				}
			}

			generateClassHTMLdocumentationFromCustomCSclassFormat(CSclassFileName, &CSclassFileContents);
		}
	}
}

string CSgenerateHTMLdocumentationClass::getFunctionNameFromFunctionNameFull(const string* functionNameFull)
{

	int startPositionOfFunctionBrackets = functionNameFull->find(CHAR_OPEN_BRACKET);
	string functionTypeAndName = functionNameFull->substr(0, startPositionOfFunctionBrackets);
	//cout << "functionTypeAndName = " << functionTypeAndName << endl;

	int startPositionOfFunctionName = functionTypeAndName.rfind(CHAR_SPACE) + 1;
	string functionName = functionTypeAndName.substr(startPositionOfFunctionName, functionTypeAndName.length()-startPositionOfFunctionName);
	#ifdef CS_SUPPORT_FUNCTION_RETURN_POINTERS
	if(functionName[0] == '*')
	{
		int functionNameLength = functionName.length();
		functionName = functionName.substr(1, functionNameLength-1);
	}
	#endif


	return functionName;
}

void CSgenerateHTMLdocumentationClass::generateClassHTMLdocumentationFromCustomCSclassFormat(const string CSclassFileName, const vector<string>* CSclassFileContents)
{
	bool result = true;

	string HTMLdocumentationFileBody = "";

	for(int i=0; i<CSclassFileContents.size(); i++)
	{
		string currentClassBeingParsed = CSclassFileContents[i];
		string className = getFunctionNameFromFunctionNameFull(&currentClassBeingParsed);

		string HTMLdocumentationClassDescription = createDescriptionFromCaseSensitiveMultiwordString(className);

		string HTMLdocumentationClassParameters = "";
		string functionNameNOTUSED = "";
		generateHTMLdocumentationFunctionOrClassArgumentsFromFunctionNameFull(true, &currentClassBeingParsed, &HTMLdocumentationClassParameters);

		string HTMLdocumentationClassTitle = "";
		HTMLdocumentationClassTitle = HTMLdocumentationClassTitle + "<h4>Class " + className + "</h4>\n";
		string HTMLdocumentationClassHeader = "";
		HTMLdocumentationClassHeader = HTMLdocumentationClassHeader + "<p><b>Class description:</b> " + HTMLdocumentationClassDescription + "</p>\n";

		HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationClassTitle + HTMLdocumentationClassHeader + HTMLdocumentationClassParameters;	
	}

	CSclassFileObject.close();

	string HTMLdocumentationFileHeader = generateHTMLdocumentationHeader(CSclassFileName, true, false);
	string HTMLdocumentationFileFooter = generateHTMLdocumentationFooter(true);
	string HTMLdocumentationFile = "";
	HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;

	string outputHTMLfileName = CSclassFileName + HTML_EXTENSION;
	ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
	writeStringPointerToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);
	
}
#endif

#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_AUTOMATICALLY
void CSgenerateHTMLdocumentationClass::generateClassHTMLdocumentationFromCSclassList(const string CSclassesFileName, const vector<CSclass*>* classObjectList)
{
	bool result = true;

	string HTMLdocumentationFileBody = "";
	
	for(int i=0; i<classObjectList->size(); i++)
	{
		CSclass* classObject = (*classObjectList)[i];
		
		string className = classObject->name;

		string HTMLdocumentationClassDescription = createDescriptionFromCaseSensitiveMultiwordString(className);

		string HTMLdocumentationClassParameters = "";
		string functionNameNOTUSED = "";
		generateHTMLdocumentationFunctionOrClassArgumentsFromParameterList(true, &(classObject->parameterList), &HTMLdocumentationClassParameters);

		string HTMLdocumentationClassTitle = "";
		HTMLdocumentationClassTitle = HTMLdocumentationClassTitle + "<h4>Class " + className + "</h4>\n";
		string HTMLdocumentationClassHeader = "";
		HTMLdocumentationClassHeader = HTMLdocumentationClassHeader + "<p><b>Class description:</b> " + HTMLdocumentationClassDescription + "</p>\n";
		
		HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationClassTitle + HTMLdocumentationClassHeader + HTMLdocumentationClassParameters;	
	}
	
	string HTMLdocumentationFileHeader = generateHTMLdocumentationHeader(CSclassesFileName, true, false);
	string HTMLdocumentationFileFooter = generateHTMLdocumentationFooter(true);
	string HTMLdocumentationFile = "";
	HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;

	string outputHTMLfileName = CSclassesFileName + HTML_EXTENSION;
	ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
	writeStringPointerToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);
}
#endif




#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
void CSgenerateHTMLdocumentationClass::convertIndentedListToHTMLlist()
{
	string indentedListFileName = "requirementsLayout.txt";
	string htmlListFileName = string("requirementsLayout") + HTML_EXTENSION;

	vector<string> indentedListVector;
	if(readIndentedListFile(indentedListFileName, &indentedListVector))
	{
		string HTMLdocumentationIndentedList = "";
		generateHTMLdocumentationIndentedList(&indentedListVector, &HTMLdocumentationIndentedList);

		ofstream writeFileObjectHTML(htmlListFileName.c_str());
		writeStringPointerToFileObject(&HTMLdocumentationIndentedList, &writeFileObjectHTML);
	}
}

bool CSgenerateHTMLdocumentationClass::readIndentedListFile(const string indentedListFileName, vector<string>* indentedListVector)
{
	bool result = true;

	ifstream parseFileObject(indentedListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//txt file does not exist in current directory.
		cout << "Error: indented list File does not exist in current directory: " << indentedListFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		string currentEntry = "";
		int entryIndex = 0;
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				indentedListVector->push_back(currentEntry);
				currentEntry = "";
				entryIndex++;
			}
			else
			{
				currentEntry = currentEntry + currentToken;
			}

			charCount++;
		}
		parseFileObject.close();
	}

	return result;
}

//generic software to convert an indented list into an HTML list
void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationIndentedList(vector<string>* indentedListVector, string* HTMLdocumentationIndentationList)
{
	//generate list
	*HTMLdocumentationIndentationList = "";
	*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + "\t<p><b>Generic Indented List</b><br />\n";
	string HTMLdocumentationIndentationListBody = "\t<ul>\n";
	HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "\t<li>";
	int previousIndentation = 0;
	bool previousIndentationFirst = true;
	int referenceIndex=0;
	bool foundReferences = false;

	for(vector<string>::iterator indentedListVectorIter = indentedListVector->begin(); indentedListVectorIter != indentedListVector->end(); indentedListVectorIter++)
	{
		string currentEntry = *indentedListVectorIter;

		foundReferences = true;

		int currentEntryLengthOld = currentEntry.length();
		int indentationLevel=0;
		while(currentEntry[indentationLevel] == CHAR_TAB)
		{
			indentationLevel++;
		}
		currentEntry = currentEntry.substr(indentationLevel, currentEntryLengthOld-indentationLevel);

		addToHTMLdocumentationIndentedList(currentEntry, indentationLevel, &HTMLdocumentationIndentationListBody, &previousIndentation, &previousIndentationFirst);

	}

	for(int i=0; i<previousIndentation; i++)
	{
		HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "</li></ul>";
	}
	HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "\n\t</li>\n";
	HTMLdocumentationIndentationListBody = HTMLdocumentationIndentationListBody + "\t</ul>";

	if(foundReferences)
	{
		*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + HTMLdocumentationIndentationListBody;
	}
	else
	{
		*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + "N/A";
	}
	*HTMLdocumentationIndentationList = *HTMLdocumentationIndentationList + "</p>\n";

}
#endif


#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_NAME_WITH_CLASS
string CSgenerateHTMLdocumentationClass::generateFunctionString(const CSfunction* currentFunction, const bool isFunctionOrFunctionReferenceList, const bool externalFunction)
{
	string functionName = "";
	
	if(isFunctionOrFunctionReferenceList)
	{
		//function list
		if(externalFunction)
		{
			functionName = currentFunction->className + CS_CLASS_DELIMITER + currentFunction->name;
		}
		else
		{
			functionName = currentFunction->name;
		}
	}
	else
	{
		//cout << "!isFunctionOrFunctionReferenceList" << endl;
		
		//function reference list
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_REFERENCES_WITH_CLASS
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_REFERENCES_WITH_CLASS_EXTERNAL_ONLY
		if(externalFunction)
		{
		#endif
			functionName = currentFunction->className + CS_CLASS_DELIMITER + currentFunction->name;
		#ifdef CS_GENERATE_HTML_DOCUMENTATION_PRINT_FUNCTION_REFERENCES_WITH_CLASS_EXTERNAL_ONLY
		}
		else
		{
			functionName = currentFunction->name;
		}
		#endif
		#else
		functionName = currentFunction->name;
		#endif
	}
	
	return functionName;
}
#endif


