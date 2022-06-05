 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSclassClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
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

