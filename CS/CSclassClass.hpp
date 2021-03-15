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
 * File Name: CSclassClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3p1a 15-March-2021
 * /
 *******************************************************************************/


#ifndef HEADER_CS_CLASS_CLASS
#define HEADER_CS_CLASS_CLASS

#include "CSglobalDefs.hpp"
#include "CSparameterClass.hpp"


class CSclass{
private:
	/*There are currently no private attributes of this class*/
public:

	CSclass(void); // constructor declaration
	~CSclass(void);	//  and destructor.
	
	string name;
	vector<CSparameter*> parameterList;
	#ifdef CS_SUPPORT_GENERATED_CPP_CODE_IDENTIFY_CLASS_PARAMETERS_FUNCTIONS_IGNORE_PRIMARY_FILE_CLASS
	bool primaryFileClass;
	#endif
};

class CSclassClass
{
	public: bool getClassNameBase(const string className, string* classNameBase);
};



#endif

