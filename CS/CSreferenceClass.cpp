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
 * File Name: CSreferenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3c6c 22-July-2013
 *
 *******************************************************************************/

#include "CSreferenceClass.h"




CSfileReferenceContainer::CSfileReferenceContainer(void)
{
	next = NULL;
	name = "";
}

CSfileReferenceContainer::~CSfileReferenceContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
}

CSfileReference::CSfileReference(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;	
	HTMLgenerated = false;
	
	next = NULL;
	previous = NULL;
	firstReferenceContainerInAboveReferenceList = new CSfileReferenceContainer();
	printedTrace = false;
	printedTraceReset = false;
	
	//file reference only
	firstReferenceInAboveList = NULL;
	firstReferenceInBelowList = NULL;
	shortcutToPrintedVersionOfReference = NULL;
	firstReferenceInFunctionList = NULL;

	for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}

}

CSfileReference::~CSfileReference(void)
{
	if(firstReferenceInBelowList != NULL)
	{
		delete firstReferenceInBelowList;
	}
	if(next != NULL)
	{
		delete next;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
}



CSfunctionReferenceContainer::CSfunctionReferenceContainer(void)
{
	next = NULL;
	name = "";
}

CSfunctionReferenceContainer::~CSfunctionReferenceContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
}

CSfunctionReference::CSfunctionReference(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
	singleFileOnlyPrinted = false;
	#endif	
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	filePrintXrecordForSingleFileOnly = 0;
	filePrintYrecordForSingleFileOnly = 0;	
	HTMLgenerated = false;
	
	next = NULL;
	previous = NULL;
	firstReferenceContainerInAboveReferenceList = new CSfunctionReferenceContainer();
	hasHadFunctionReferencesParsed = false;
	printedTrace = false;
	printedTraceReset = false;

	//function reference only
	nameFull = "";
	firstReferenceInFunctionReferenceList = NULL;
	isFunctionReference = false;
	isFunctionReferenceReference = false;
	numTopLevelFunctionsInFileAlreadyPrinted = 0;
	#ifdef CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
	functionReferenceIndentation = 0;
	#endif
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	string functionText;
	#endif
		
	for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}

}

CSfunctionReference::~CSfunctionReference(void)
{
	/*
	if(firstReferenceInBelowList != NULL)
	{
		delete firstReferenceInBelowList;
	}
	*/
	if(next != NULL)
	{
		delete next;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
}




void printCSFileReference(CSfileReference * ref)
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

void printCSFunctionReference(CSfunctionReference * ref)
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
	cout << "nameFull = " << ref->nameFull << endl;
}



/*
//these functions are not being used:

void addReferenceToReferenceList(CSReference * reference, CSReference * firstReferenceInList)
{
	CSReference * currentReferenceInIncludeFileList = firstReferenceInList;

	bool foundMatch = false;
	while(currentReferenceInIncludeFileList->next != NULL)
	{
		if(reference->name == currentReferenceInIncludeFileList->name)
		{
			foundMatch = true;
		}

		if(currentReferenceInIncludeFileList->next->next == NULL)
		{
			if(!foundMatch)
			{
				currentReferenceInIncludeFileList->next = reference;
				currentReferenceInIncludeFileList->next->next = NULL;
				currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
			}
			else
			{
				currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
			}
		}
		else
		{
			currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
		}
	}
}


void mergeIncludeFileReferences(CSReference * firstReferenceInIncludeFileList, CSReference * firstReferenceInTopLevelBelowList)
{
	CSReference * currentReferenceInIncludeFileList = firstReferenceInIncludeFileList;

	while(currentReferenceInIncludeFileList->next != NULL)
	{
		findAndMergeAllIndenticalFileReferences(currentReferenceInIncludeFileList, firstReferenceInTopLevelBelowList);

		if(currentReferenceInIncludeFileList->firstReferenceInBelowList != NULL)
		{
			mergeIncludeFileReferences(currentReferenceInIncludeFileList->firstReferenceInBelowList, firstReferenceInTopLevelBelowList);
		}
		currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
	}
}


void findAndMergeAllIndenticalFileReferences(CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList)
{
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		#ifdef CS_DEBUG
		if(currentReference->name != "")
		{
			for(int i= 0; i<currentReference->level; i++)
			{
				cout << "\t";
			}
			cout << "currentReference->name = " << currentReference->name << endl;
			//cout << "currentReference->firstReferenceInBelowList->name = " << currentReference->firstReferenceInBelowList->name << endl;
		}
		#endif

		if(currentReference->name == reference->name)
		{//identical file reference found;

			if(currentReference->id != reference->id)
			{

				for(int i= 0; i<reference->level; i++)
				{
					cout << "\t";
				}
				cout << "reference->name = " << reference->name << endl;
				for(int i= 0; i<currentReference->level; i++)
				{
					cout << "\t";
				}
				cout << "currentReference->name = " << currentReference->name << endl;

				if(reference->level < currentReference->level)
				{
					reference->level = currentReference->level;
				}

				if(reference->firstReferenceInAboveList != NULL)
				{
					cout << "reference->firstReferenceInAboveList->name = " << reference->firstReferenceInAboveList->name << endl;
					cout << "currentReference->currentReference->name = " << currentReference->firstReferenceInAboveList->name << endl;

					mergeReferenceLists(reference->firstReferenceInAboveList, currentReference->firstReferenceInAboveList);
				}

				currentReference->previous->next = currentReference->next;
				delete currentReference;
			}
		}

		if(currentReference->firstReferenceInBelowList != NULL)
		{

			findAndMergeAllIndenticalFileReferences(reference, currentReference->firstReferenceInBelowList);
		}
		currentReference = currentReference->next;
	}



}


void mergeReferenceLists(CSReference * firstReferenceInList, CSReference * firstReferenceInListToMerge)
{
	CSReference * currentReferenceInReferenceListToMerge = firstReferenceInListToMerge;

	while(currentReferenceInReferenceListToMerge->next != NULL)
	{
		bool foundMatch = false;


		CSReference * currentReference = firstReferenceInList;

		while(currentReference->next != NULL)
		{
			if(currentReference->name == currentReferenceInReferenceListToMerge->name)
			{
				foundMatch = true;
			}
			currentReference = currentReference->next;
		}

		if(!foundMatch)
		{
			CSReference * currentReference = firstReferenceInList;
			while(currentReference->next != NULL)
			{
				if(currentReference->next->next == NULL)
				{
					currentReference->next = currentReferenceInReferenceListToMerge;
					currentReference->next->next = NULL;
					currentReference->next->previous = currentReference;
					currentReference = currentReference->next;
				}
				else
				{
					currentReference = currentReference->next;
				}
			}
		}

		currentReferenceInReferenceListToMerge = currentReferenceInReferenceListToMerge->next;
	}
}

*/



