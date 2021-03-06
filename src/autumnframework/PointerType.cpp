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

#include "PointerType.h"
#include "TypeManager.h"

namespace Autumn{

void* PointerType::createValue(const string& type, const StrValueList& vl,
							   StrIterator& it)const
{
	string basicType = type.substr(0, type.size()-1); // erase the '*'
	IAutumnType* at = this->getTypeManager()->findTypeBean(basicType);

	void* p = at->createValue(basicType, vl, it);

	void** pp = new (void*);
	*pp = p;
	return (void*)pp;
}

void PointerType::freeValue(void* p, const string& type)
{
	string basicType = type.substr(0, type.size()-1); // erase the '*'
	IAutumnType* at = this->getTypeManager()->findTypeBean(basicType);
	
	at->freeValue(*(void**)p, basicType);
	delete (void**)p;
}

} // End namespace Autumn
