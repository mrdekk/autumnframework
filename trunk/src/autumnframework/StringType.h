/*
* Copyright 2006 the original author or authors.
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*      http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AUTUMN_STRINGTYPEI_H
#define AUTUMN_STRINGTYPEI_H

#include "Basic.h"
#include "IBasicType.h"
#include "AutumnException.h"

/** 
 * ValueType: string. Implementing this type not using template ValueType
 * is due to a VC6's bug.
 * @version 0.1.0
 * @since 2006-11-28
 */
class StringType:public IBasicType{
public:
	/** 
	 * Create a value from StrValueList(from it's first element in fact).
	 * @param vl A Vector<string>
	 * @param it A iterator pointing to the first unused string, it will be changed
	 * in this function.
	 * @return A pointer to a value
	 */
	void* createValue(const StrValueList& vl, StrIterator& it){
		if( it != vl.end() ){
			string *p = new string(*it++);
			return (void*)p; //*p is the value
		}
		throw new NonValueEx("StringType", "createValue",
			"There is no string in vector!");
	}

	/** Free the space where p point*/
	void freeValue(void* p){
		// free space of string
		delete (string*)p;
	}
};

#endif
