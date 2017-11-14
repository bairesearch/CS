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
 * File Name: CSgenerateHTMLdocumentation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3m4a 12-November-2017
 *
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
	this->generateHTMLdocumentationForAllFunctions(firstObjectInTopLevelBelowListContainer, firstReferenceInPrintListFunction, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, &HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceFunctionUpwards, usePredefinedGrid);
	delete firstReferenceInPrintListFunction;	//should delete this, as it will contain far too many LD vector graphics references (ie a set of traced references for each function)

	if(useOutputHTMLfile)
	{//use single html file for project (rather than unique html file per file in project)
		string HTMLdocumentationHeader = this->generateHTMLdocumentationHeader("Software Project", true, false);
		string HTMLdocumentationFooter = this->generateHTMLdocumentationFooter(true);
		string HTMLdocumentation = HTMLdocumentationHeader + HTMLdocumentationBody + HTMLdocumentationFooter;
		ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
		this->writeStringPointerToFileObject(&HTMLdocumentation, &writeFileObjectHTML);
	}
}

string CSgenerateHTMLdocumentationClass::generateHTMLdocumentationHeader(const string name, const bool htmlHeader, const bool isFile)
{
	string HTMLdocumentationHeader = "";
	if(htmlHeader)
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<html><head><title>" + name + " Documentation</title><style type=\"text/css\">TD { font-size:75%; } </style></head><body><h3>" + name + " Documentation</h3><p>Automatically generated with Code Structure Viewer (CS), Project Version: 3m4a 12-November-2017<p>\n";
	}
	else
	{
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<h3>" + name + " Documentation</h3>\n";
	}
	if(isFile)
	{
		int indexOfFileExtension = name.find(".");
		string fileNameWithoutExtension = name.substr(0, indexOfFileExtension);
		string HTMLdocumentationFileDescription = this->createDescriptionFromCaseSensitiveMultiwordString(fileNameWithoutExtension);
		HTMLdocumentationHeader = HTMLdocumentationHeader + "<p><b>File description:</b> " + HTMLdocumentationFileDescription + "</p>";
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
		string outputHTMLfileName = currentFileObject->name + HTML_EXTENSION;

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
				string HTMLdocumentationFileFunctionList = "<p>\n<b>File function List</b>\n<ul><li>";
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
							}

							string HTMLdocumentationFunction = "";
							this->generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, currentFunctionObject, fileNameHoldingFunction, &currentTagInSVGFileFunction, generateHTMLdocumentationMode, &HTMLdocumentationFunction, &outputSVGfileNameFunction, false, "", traceFunctionUpwards);
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
							this->addToHTMLdocumentationFileFunctionList(currentFunctionObject, &HTMLdocumentationFileFunctionList, &previousIndentation, &previousIndentationFirst);
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
				string outputSVGFileNameFile = currentFileObject->name + SVG_EXTENSION;
				this->generateFileDiagramFunctionsHeirachy(currentFileObject, outputSVGFileNameFile, firstObjectInTopLevelBelowListContainer, usePredefinedGrid);
				string HTMLdocumentationFileImagePlaceHolder = this->generateHTMLdocumentationImagePlaceHolder(&outputSVGFileNameFile, "File Diagram (functions heirachy)");
				HTMLdocumentationFileIntroduction = HTMLdocumentationFileIntroduction + HTMLdocumentationFileImagePlaceHolder;
				#endif
				HTMLdocumentationFileBody = HTMLdocumentationFileIntroduction + HTMLdocumentationFileBody;

				string HTMLdocumentationFileHeader = this->generateHTMLdocumentationHeader(currentFileObject->name, !useOutputHTMLfile, true);
				string HTMLdocumentationFileFooter = this->generateHTMLdocumentationFooter(!useOutputHTMLfile);
				string HTMLdocumentationFile = "";
				HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;


				if(!useOutputHTMLfile)
				{//create multiple html files (ie, a single HTML file per parsed source file)
					string outputHTMLfileName = currentFileObject->name + HTML_EXTENSION;
					ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
					this->writeStringPointerToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);
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
					this->generateHTMLdocumentationForAllFunctions(currentFileObject->firstFileInBelowListContainer, currentReferenceInPrintList, firstObjectInTopLevelBelowListContainer, generateHTMLdocumentationMode, useOutputHTMLfile, HTMLdocumentationBody, firstTagInSVGFile, lastTagInSVGFile, traceFunctionUpwards, usePredefinedGrid);
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
		HTMLdocumentationFunctionTitle = HTMLdocumentationFunctionTitle + "<h4>" + (bottomLevelFunctionToTraceUpwards->name) + "()</h4>";
		string HTMLdocumentationFunctionSummary = "";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_SUMMARY
		this->generateHTMLdocumentationFunctionSummary(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionSummary);
		#endif

		string HTMLdocumentationFunctionInputArguments = "";
		this->generateHTMLdocumentationFunctionInputArguments(&(bottomLevelFunctionToTraceUpwards->name), &(bottomLevelFunctionToTraceUpwards->nameFull), &HTMLdocumentationFunctionInputArguments);

		string HTMLdocumentationFunctionReferenceList = "";
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
		this->generateHTMLdocumentationFunctionReferenceList(bottomLevelFunctionToTraceUpwards, &HTMLdocumentationFunctionReferenceList);
		#endif

		*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody + HTMLdocumentationFunctionTitle + HTMLdocumentationFunctionSummary + HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionReferenceList + "<br />";

		if(traceFunctionUpwards)
		{
			string HTMLdocumentationFunctionTraceTableHeader = "";
			string HTMLdocumentationFunctionTraceTableFooter = "";
			HTMLdocumentationFunctionTraceTableHeader = HTMLdocumentationFunctionTraceTableHeader + "\t<b>Function Trace</b><br /><table border=\"1\"><tr><th>" + "location" + "</th><th>" + "current function being traced" + "</th></tr>\n";
			HTMLdocumentationFunctionTraceTableFooter = HTMLdocumentationFunctionTraceTableFooter + "\t</table>";
			string HTMLdocumentationFunctionTraceTable = HTMLdocumentationFunctionTraceTableHeader + HTMLdocumentationFunctionTraceTableRows + HTMLdocumentationFunctionTraceTableFooter;

			string HTMLdocumentationFunctionTraceImagePlaceHolder = this->generateHTMLdocumentationImagePlaceHolder(outputSVGfileNameFunction, "Trace Diagram (magenta)");

			*HTMLdocumentationFunctionBody = *HTMLdocumentationFunctionBody +  HTMLdocumentationFunctionTraceTable + HTMLdocumentationFunctionTraceImagePlaceHolder;
		}

		if(useOutputHTMLfile)
		{//use single html file for function
			string HTMLdocumentationFunctionHeader = this->generateHTMLdocumentationHeader(bottomLevelFunctionToTraceUpwards->name, true, false);
			string HTMLdocumentationFunctionFooter = this->generateHTMLdocumentationFooter(true);
			string HTMLdocumentationFunction = HTMLdocumentationFunctionHeader +* HTMLdocumentationFunctionBody + HTMLdocumentationFunctionFooter;

			ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
			this->writeStringPointerToFileObject(&HTMLdocumentationFunction, &writeFileObjectHTML);
		}
	}


}

void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionSummary(string* functionName, const string* functionNameFull, string* HTMLdocumentationFunctionSummary)
{
	*HTMLdocumentationFunctionSummary = "";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t<p><b>Function Summary</b><br /><table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "return type" + "</th><th>" + "description" + "</th></tr>\n";
	string HTMLdocumentationFunctionDescription = this->createDescriptionFromCaseSensitiveMultiwordString(*functionName);
	int endPositionOfReturnType = functionNameFull->find(*functionName);
	int startPositionOfReturnType = 0;
	string HTMLdocumentationFunctionReturnType = functionNameFull->substr(startPositionOfReturnType, endPositionOfReturnType-startPositionOfReturnType);
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t\t<tr><td>" +* functionName + "</td><td>" + HTMLdocumentationFunctionReturnType + "</td><td>" + HTMLdocumentationFunctionDescription + "</td></tr>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</table>\n";
	*HTMLdocumentationFunctionSummary = *HTMLdocumentationFunctionSummary + "\t</p>\n";
}

void CSgenerateHTMLdocumentationClass::generateHTMLdocumentationFunctionInputArguments(const string* functionName, const string* functionNameFull, string* HTMLdocumentationFunctionInputArguments)
{
	*HTMLdocumentationFunctionInputArguments = "";
	#ifdef CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<b>Class Data</b><br />";
	#else
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t<b>Function Arguments</b><br />";
	#endif
	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "<table border=\"1\">\n\t\t<tr><th>" + "name" + "</th><th>" + "type" + "</th><th>" + "description" + "</th></tr>\n";

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
			string currentArgumentDescription = this->createDescriptionFromCaseSensitiveMultiwordString(currentArgumentName);


			currentArgumentName = this->ensureHTMLTagSafe(currentArgumentName);
			currentArgumentType = this->ensureHTMLTagSafe(currentArgumentType);
			currentArgumentDescription = this->ensureHTMLTagSafe(currentArgumentDescription);

			string HTMLdocumentationFunctionInputArgumentsRow = "\t\t<tr><td>" + currentArgumentName + "</td><td>" + currentArgumentType + "</td><td>" + currentArgumentDescription + "</td></tr>\n";
			*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + HTMLdocumentationFunctionInputArgumentsRow;

			startPositionOfArgument = endPositionOfArgument+1;
		}
	}
	else
	{
	}

	*HTMLdocumentationFunctionInputArguments = *HTMLdocumentationFunctionInputArguments + "\t</table>\n";
}


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
	*HTMLdocumentationFunctionReferenceList = *HTMLdocumentationFunctionReferenceList + "\t<p><b>Function reference List</b><br />\n";
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

	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION_ADVANCED
		/*
		if(function->name == "createImage")
		{
			cout << "currentReferenceInFunctionReferenceList; " << currentReferenceInFunctionReferenceList->name << ", = " << currentReferenceInFunctionReferenceList->functionReferenceIndentation << endl;
		}
		*/
		this->addToHTMLdocumentationFileFunctionList(currentReferenceInFunctionReferenceList, &HTMLdocumentationFunctionObjectListBody, &previousIndentation, &previousIndentationFirst);
	#else
		#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST_WITH_INDENTATION
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\t\t<li>";
		for(int i=0; i<currentReferenceInFunctionReferenceList->functionReferenceIndentation; i++)
		{
			HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "&#160&#160&#160&#160&#160";
		}
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + currentReferenceInFunctionReferenceList->name + "</li>\n";
		#else
		HTMLdocumentationFunctionObjectListBody = HTMLdocumentationFunctionObjectListBody + "\t\t<li>" + currentReferenceInFunctionReferenceList->name + "</li>\n";
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
	string HTMLdocumentationFunctionTraceImagePlaceHolder = "\t<br /><b>" + imageTitle + "</b><br /><img width=\"1300px\" src=\"" +* traceImageFileName  + "\">\n";
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

void CSgenerateHTMLdocumentationClass::addToHTMLdocumentationFileFunctionList(CSfunction* currentFunctionObject, string* HTMLdocumentationFileFunctionList, int* previousIndentation, bool* previousIndentationFirst)
{
	this->addToHTMLdocumentationIndentedList(currentFunctionObject->name, currentFunctionObject->functionReferenceIndentation, HTMLdocumentationFileFunctionList, previousIndentation, previousIndentationFirst);
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
				cout << currentTopLevelFunctionObject->name << endl;
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
					currentReferenceInPrintList = CSdraw.createFunctionObjectListBoxesAndConnections(currentReferenceInPrintList, currentFileObject, currentTopLevelFunctionObject, firstObjectInTopLevelBelowListContainer, 0, currentReferenceInFunctionReferenceList, &currentTagInSVGFile, false, true, &(currentFileObject->name), usePredefinedGrid);

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
			string HTMLdocumentationFileBody = "";

			string currentClassBeingParsed = "";
			bool finishedParsingObject = false;
			char currentToken;
			while((currentToken = (&CSclassFileObject)->get()) != EOF)
			{
				if(currentToken == CHAR_NEWLINE)
				{
					string className = this->getFunctionNameFromFunctionNameFull(&currentClassBeingParsed);

					string HTMLdocumentationClassDescription = this->createDescriptionFromCaseSensitiveMultiwordString(className);

					string HTMLdocumentationClassParameters = "";
					string functionNameNOTUSED = "";
					this->generateHTMLdocumentationFunctionInputArguments(&functionNameNOTUSED, &currentClassBeingParsed, &HTMLdocumentationClassParameters);
					currentClassBeingParsed = "";

					string HTMLdocumentationClassTitle = "";
					HTMLdocumentationClassTitle = HTMLdocumentationClassTitle + "<h4>Class " + className + "</h4>\n";
					string HTMLdocumentationClassHeader = "";
					HTMLdocumentationClassHeader = HTMLdocumentationClassHeader + "<p><b>Class description:</b> " + HTMLdocumentationClassDescription + "</p>\n";

					HTMLdocumentationFileBody = HTMLdocumentationFileBody + HTMLdocumentationClassTitle + HTMLdocumentationClassHeader + HTMLdocumentationClassParameters;

				}
				else
				{
					currentClassBeingParsed = currentClassBeingParsed + currentToken;
				}
			}

			CSclassFileObject.close();

			string HTMLdocumentationFileHeader = this->generateHTMLdocumentationHeader(CSclassFileName, true, false);
			string HTMLdocumentationFileFooter = this->generateHTMLdocumentationFooter(true);
			string HTMLdocumentationFile = "";
			HTMLdocumentationFile = HTMLdocumentationFile + HTMLdocumentationFileHeader + HTMLdocumentationFileBody + HTMLdocumentationFileFooter;

			string outputHTMLfileName = CSclassFileName + HTML_EXTENSION;
			ofstream writeFileObjectHTML(outputHTMLfileName.c_str());
			this->writeStringPointerToFileObject(&HTMLdocumentationFile, &writeFileObjectHTML);


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

#endif






#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
void CSgenerateHTMLdocumentationClass::convertIndentedListToHTMLlist()
{
	string indentedListFileName = "requirementsLayout.txt";
	string htmlListFileName = string("requirementsLayout") + HTML_EXTENSION;

	vector<string> indentedListVector;
	if(this->readIndentedListFile(indentedListFileName, &indentedListVector))
	{
		string HTMLdocumentationIndentedList = "";
		this->generateHTMLdocumentationIndentedList(&indentedListVector, &HTMLdocumentationIndentedList);

		ofstream writeFileObjectHTML(htmlListFileName.c_str());
		this->writeStringPointerToFileObject(&HTMLdocumentationIndentedList, &writeFileObjectHTML);
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

		this->addToHTMLdocumentationIndentedList(currentEntry, indentationLevel, &HTMLdocumentationIndentationListBody, &previousIndentation, &previousIndentationFirst);

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
