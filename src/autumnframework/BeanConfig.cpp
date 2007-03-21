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

#include <memory>
#include "BeanConfig.h"
#include "IBeanWrapper.h"
#include "AutumnException.h"

/** erase space char */
string trimString(const string v)
{
	string s=v;
	for(int i=0; i<s.size(); ){
		if( isspace(s[i]) ){
			s.erase(i,1);
		}
		else i++;
	}
	return s;
}


BeanConfig::BeanConfig(TBean& beandef, WrapperMaker* wc, WrapperFreer* wd)
{
	this->Name = beandef.Name;
	this->WrapperCreater = wc;
	this->WrapperDeleter = wd;
	this->Initializable = beandef.Initializable;
	this->Destroyable = beandef.Destroyable;
	this->DependedObjects = beandef.DependedObjects;
	this->Singleton = beandef.Singleton;
	
	this->ConArgs = new PropertyList;
	this->Properties = new PropertyList;
	
	
	int i, argNum;
	TProperty *p;
	string ConArgTypes;

	auto_ptr<IBeanWrapper> pw( wc() );
	
	// the following depends on BeanWrapperMacro implementation.
	// get constructor arguments' types.
	argNum = beandef.ConArgs.size();
	if( argNum > 0 ) {
		typedef void* void_ptr;
		auto_ptr<void_ptr> pp( new void_ptr[1] );
		void_ptr* pv = pp.get();

		string firstArg = beandef.ConArgs[0].Value[0];
		pv[0] = &firstArg;
		ConArgTypes = pw->getConArgTypes(pv, argNum);
	}
	
	int start, end = -1;
	for(i=0; i<beandef.ConArgs.size(); i++){
		p = &beandef.ConArgs[i];
		string tmpType = p->Type;
		
		start = end + 1;
		end = ConArgTypes.find(typeSeparator, start);
		if( end == string::npos ){
			throw new MissDefinitionEx("BeanConfig", "BeanConfig", 
				"the ConArgs' types are less than arguments!");
		}
		if( tmpType.empty() )
			tmpType = ConArgTypes.substr(start, end - start);
		
		this->ConArgs->push_back(new BeanProperty(p->Name,
				trimString(tmpType),
				p->Value, p->IsBeanRef, p->Managed));
	}
	
	for(i=0; i<beandef.Properties.size(); i++){
		p = &beandef.Properties[i];

		string tmpType = p->Type;
		if( tmpType.empty() )
			pw->getBeanPropertyType(p->Name.c_str(), tmpType);

		this->Properties->push_back(new BeanProperty(p->Name, 
				trimString(tmpType), 
				p->Value, p->IsBeanRef, p->Managed));
	}
}

BeanConfig::~BeanConfig()
{
	int i;
	for(i=0; i<this->ConArgs->size(); i++){
		delete (*this->ConArgs)[i];
	}
	for(i=0; i<this->Properties->size(); i++){
		delete (*this->Properties)[i];
	}
	delete this->ConArgs;
	delete this->Properties;	
}

IBeanWrapper* BeanConfig::createWrapper()
{
	IBeanWrapper* p = this->WrapperCreater();
	p->setInitializable(this->Initializable);
	p->setDestroyable(this->Destroyable);
	p->setWrapperDeleter(this->WrapperDeleter);

	return p;
}

