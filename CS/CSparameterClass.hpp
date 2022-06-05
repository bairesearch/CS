 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSparameterClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_CS_PARAMETER_CLASS
#define HEADER_CS_PARAMETER_CLASS

#include "CSglobalDefs.hpp"

class CSparameter{
private:
	/*There are currently no private attributes of this class*/
public:

	CSparameter(void); // constructor declaration
	~CSparameter(void);	//  and destructor.
	
	string name;
	string type;
	bool isFunction;
	int functionAccessSpecifierType;
	vector<CSparameter*> functionArgumentsParameterList;
};


#endif

