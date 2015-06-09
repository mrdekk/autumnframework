**[Manual Main Page](AutumnManual.md)** | **[Previous Chapter](AutumnManual.md)**

# 1. A Sample #
## 1.1. Write the sample without Autumn ##
This smaple is very simple and is included in the download package. It defines a interface as following, Action.h:
```
class Action {
public:
	virtual const char* excute(const char* s) = 0;
};
```
The interface has two implementations, each has a attribute, Message. The LowerAction.h and UpperAction.h:
```
class LowerAction: public Action{
private:
	string Message;

public:
	const char* getMessage();
	void  setMessage(const char* msg);
	const char* excute(const char* s);
};

class UpperAction: public Action{
private:
	string Message;

public:
	const char* getMessage();
	void  setMessage(const char* msg);
	const char* excute(const char* s);
};
```
The member functions implementations are in LowerAction.cpp and UpperAction.cpp:
```
const char* LowerAction::getMessage()
{
	return this->Message.c_str();
}

void  LowerAction::setMessage(const char* msg)
{
	this->Message = msg;
}

const char* LowerAction::excute(const char* s)
{
	cout<<string("Lower: ") + this->getMessage() + ", " + s<<endl;;
	return "Lower";
}

const char* UpperAction::getMessage()
{
	return this->Message.c_str();
}

void  UpperAction::setMessage(const char* msg)
{
	this->Message = msg;
}

const char* UpperAction::excute(const char* s)
{
	cout<<string("Upper: ") + this->getMessage() + ", " + s<<endl;;
	return "Upper";
}
```
If you want use LowerAction as Action, you can write some simple codes like:
```
Action* getAction();
void main()
{
	Action* pa = getAction();
	cout<<pa->excute()<<endl;
	delete pa;
}

Action* getAction()
{
	LowerAction* pa = new LowerAction();
	pa->setMessage("Some Message for LowerAction");
	return pa;
}
```
If you want use UpperAction as Action later, you should rewrite the getAction() function:
```
Action* getAction()
{
	UpperAction* pa = new UpperAction();
	pa->setMessage("Some Message for UpperAction");
	return pa;
}
```
## 1.2. Write the sample with Autumn ##
If you write the sample with Autumn, you needn't rewrite any code if you want use UpperAction to replace LowerAction. You can use dependence injection to set LowerAction or UpperAction as Action, and set their Message attribute. There has some difference between Writing it with and without Autumn. But the interface and its implementations will not be changed because Autumn is not invasive for them.
### 1.2.1. Implement wrapper ###
We should implement a wrapper for each of LowerAction and UpperAction. We can do this easily using macros defined in Autumn framework. Write the wrappers in ActionExp.cpp like:
```
#include "BeanWrapperMacro.h"
#include "LowerAction.h"
#include "UpperAction.h"

AUTUMNBEAN(LowerAction)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(LowerAction)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Message, char*)
AUTUMNBEAN_SETTER_END()
AUTUMNBEAN_END(LowerAction)

AUTUMN_LOCALBEAN(LowerAction)

AUTUMNBEAN(UpperAction)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(UpperAction)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Message, char*)
AUTUMNBEAN_SETTER_END()
AUTUMNBEAN_END(UpperAction)
AUTUMN_LOCALBEAN(UpperAction)
```
File BeanWrapperMacro.h is in _include_ directory.
### 1.2.2. Implement main() ###
In main(), we should get IBeanFactory first, and use it to get and free Action's implementation. Like this:
```
#include <iostream>
#include <exception>
#include "Action.h"
#include "IBeanFactory.h"

using namespace std;

void main(int argc, char* argv[])
{
	try{
		IBeanFactory* bf = getBeanFactoryWithXML("AutumnDemo.xml");
		Action* pa = (Action*)bf->getBean("TheAction");
		cout<<pa->excute("Vicent")<<endl;
		bf->freeBean(pa);
		
		deleteBeanFactory(bf);
		cout<<"OK!"<<endl;
	}catch (exception* e ) {
		cout<<"Exception: "<<e->what()<<endl;
		delete e;
	}
}
```
AutumnDemo.xml is Autumn configuration file for this sample.When compiling, you should link Autumn library AutumnFramework.lib.
### 1.2.3. Configuration file ###
Configuration file AutumnDemo.xml defines injection rule:
```
<?xml version="1.0" encoding="UTF-8"?>
<autumn xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="E:\Workplace\AutumnFramework\doc\Design\autumn.xsd">
	<library path="local">
		<beans>
			<bean class="LowerAction" name="TheAction">
				<properties>
					<property name="Message">
						<value>Hello</value>
					</property>
				</properties>
			</bean>
		</beans>
	</library>
</autumn>
```
Because LowerAction and UpperAction are in main process, not in dynamic libraries, so the library's path is "local". This config file uses LowerAction as Action. If you want to use UpperAction, you can replace the bean's attribute value of "class" with "UpperAction". If you want to configure the Message property, you can modify its value where "Hello" is.
### 1.2.4. Use dynamic library ###
To improve agility, we can implement Action in a dynamic library. We generate the dynamic library(injectBeanImpl.dll) with LowerAction.cpp UpperAction.cpp and ActionExp.cpp. Because we use dynamic library, the implemetations of Action are not in the main process, so we should (1)Delete AUTUMN\_LOCALBEAN macro from ActionExp.cpp; (2)Library's path should be the dynamic library path, not "local"; (3)Other programs needn't be changed; (4)Needn't link injectBeanImpl.dll when compile main program.
The ActionExp.cpp file like:
```
#include "BeanWrapperMacro.h"
#include "LowerAction.h"
#include "UpperAction.h"


AUTUMNBEAN(LowerAction)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(LowerAction)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Message, char*)
AUTUMNBEAN_SETTER_END()
AUTUMNBEAN_END(LowerAction)

AUTUMNBEAN(UpperAction)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(UpperAction)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Message, char*)
AUTUMNBEAN_SETTER_END()
AUTUMNBEAN_END(UpperAction)

```
The configuration file like:
```
<?xml version="1.0" encoding="UTF-8"?>
<autumn xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="E:\Workplace\AutumnFramework\doc\Design\autumn.xsd">
	<library path="../lib/injectBeanImpl.dll" name="injectBeanImpl">
		<beans>
			<bean class="LowerAction" name="TheAction">
				<properties>
					<property name="Message">
						<value>Hello</value>
					</property>
				</properties>
			</bean>
		</beans>
	</library>
</autumn>
```

**[Manual Main Page](AutumnManual.md)** | **[Next Chapter](Manual_2_WhatAutumnHas.md)**