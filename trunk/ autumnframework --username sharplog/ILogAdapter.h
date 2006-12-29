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

#ifndef AUTUMN_ILOGADAPTER_H
#define AUTUMN_ILOGADAPTER_H

#include <string>

using namespace std;

/** 
 * Adapter for external log implementation
 * @version 0.1.0
 * @since 2006-11-30
 */

class ILogAdapter {
public:
	virtual void debug(const string&) = 0;
	virtual void info(const string&) = 0;
	virtual void warn(const string&) = 0;
	virtual void error(const string&) = 0;
	virtual void fatal(const string&) = 0;
	virtual void log(int, const string&) = 0;
	
	/** 
	 * Destructor
	 */
	virtual ~ILogAdapter(){}
};
#endif
