/*******************************************************************************
 *
 * File Name: CSreferenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3a7b 09-June-2012
 *
 *******************************************************************************/

#include "CSreferenceClass.h"




CSReference::CSReference(void)
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

	next = NULL;
	previous = NULL;
	firstReferenceContainerInAboveFileOrFunctionReferenceList = new CSReferenceContainer();
	printedTrace = false;

	//file reference only
	firstReferenceInAboveList = NULL;
	firstReferenceInBelowList = NULL;
	shortcutToPrintedVersionOfReference = NULL;
	isFileReference = false;
	firstReferenceInFunctionList = NULL;

	//function reference only
	nameFull = "";
	firstReferenceInFunctionReferenceList = NULL;
	isFunctionReference = false;
	isFunctionReferenceReference = false;
	numTopLevelFunctionsInFileAlreadyPrinted = 0;

	for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}

}


CSReference::~CSReference(void)
{
	if(firstReferenceInBelowList != NULL)
	{
		delete firstReferenceInBelowList;
	}
	if(next != NULL)
	{
		delete next;
	}
	if(firstReferenceContainerInAboveFileOrFunctionReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveFileOrFunctionReferenceList;
	}
}


CSReferenceContainer::CSReferenceContainer(void)
{
	next = NULL;
	name = "";
}

CSReferenceContainer::~CSReferenceContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
}



void printCSReference(CSReference * ref)
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




	//not used;
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

		//cout << "h1" << endl;
		//exit(0);

	}
}


void findAndMergeAllIndenticalFileReferences(CSReference * reference, CSReference * firstReferenceInAboveLevelBelowList)
{
	CSReference * currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		/*
		if(currentReference->name != "")
		{
			for(int i= 0; i<currentReference->level; i++)
			{
				cout << "\t";
			}
			cout << "currentReference->name = " << currentReference->name << endl;
			cout << "here" << endl;

			//cout << "currentReference->firstReferenceInBelowList->name = " << currentReference->firstReferenceInBelowList->name << endl;
		}
		*/



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
				cout << "here2" << endl;

				cout << "sq" << endl;


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

				cout << "sq2" << endl;
				//exit(0);

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




