# 1. 简介 #
Autumn Framework是一个C++的依赖注入（DI）框架，它的功能是模仿JAVA领域中的Spring框架来的。但它的功能没有Spring那么强大，当前只是提供依赖注入功能，而且尽量做到对源代码的不侵入。因为C++没有象JAVA那样的动态反射机制，不能很容易地在运行时得到注入的一些相关信息，所以在使用Autumn的时候，需要做一些工作来让Autumn框架知道注入时所需要的一些信息，即需要程序员来定义一些Wrapper。

通过使用Autumn，可以将动态库很容易地跟原来的程序部署在一起，而不需要重新编译原有程序。

## 1.1. Autumn名字的由来 ##
其实没有很什么的意义，只是对应于Spring（当然Spring框架中的Spring也许并没有当春天来解）。秋天是收获的季节，希望这个框架能有所收获，能给C++的程序员带来便利。

# 2. Autumn机制 #
## 2.1. 基本概念 ##
### Bean ###
Bean借用于Spring，是指一个需要注入的对象，它是一个POCO（Plain Old C++ Object）。在Autumn中，必须为Bean指定一个类名，Autumn将根据这个类名去找相应的Wrapper，以来创建这个类，并对其进行注入。如果将某个Bean设置为单例，则其在整个框架中只有一个实例；否则，将会在需要它的时候就创建实例，在框架中可能会存在若干它的实例。
### BeanFactory ###
BeanFactory是创建和维护Bean的一个对象。所有需要注入的对象，也就是Bean，都是由它来创建并进行注入，也由它负责释放。BeanFacotry依据Bean的配置来创建Bean，当创建一个Bean时，它所需要的其它Bean也会被创建。当释放一个Bean时，它所使用的其它Bean也会被释放。
### Library ###
Library是实现Bean的动态库或程序。一般情况下将Bean放在动态库中，这样变动起来比较方便。当然，如果您就是要把Bean的实现放在主程序中，Autumn也是允许的，但这时候，配置文件中库的名称只能为“local”。
### Type ###
Type是指注入数据的类型。Autumn除了支持C++的基本类型（void除外）外，还支持用户自定义类型。如果用户有自定义的数据类型用来注入，需要在配置文件中进行配置一个Type。
## 2.2. Autumn结构 ##
![http://autumnframework.googlecode.com/svn/trunk/doc/img/Autumn%20Structure.png](http://autumnframework.googlecode.com/svn/trunk/doc/img/Autumn%20Structure.png)

Autumn的结构如上图所示。Autumn核心主要包括：
  * Bean Factory，用来创建和释放Bean。
  * Bean Manager，用来管理所有创建的Bean。
  * Type Manager，管理着Autumn所支持的数据类型，用来创建作注入用的数据。
  * Autumn Config，管理着所有配置信息，包括Bean的配置、库的配置。Bean Factory依靠这些配置信息来创建Bean。

作为用户编写的应用主要包括动态库及主程序。用户程序跟Autumn核心之间的交互就是由Bean Wrapper和配置文件来完成的。
  * Bean Wrapper，封装了跟Bean打交道的一些方法和信息。
  * Configuration File，Autumn的配置文件，描述了所有的库及Bean的信息。

# 3. 使用Autumn #
## 3.1. 写POCO ##
写普通的C++程序，用和不用Autumn，写法基本上是一样的，不需要继承什么特殊的接口或基类。但有两点需要注意：
  1. **当注入类型为指针类型时，不要释放注入的数据，因为这些数据由Autumn创建及释放。**
  1. **属性设置方法的命名遵循setXXX的形式，XXX是属性名称。**
## 3.2. 写Bean Wrapper ##
Wrapper是Autumn用来创建、释放Bean，及对Bean进行注入的设施。程序员使用Autumn提供的宏定义来完成Wrapper的编写，然后跟应用程序一起编译成动态库，或可执行程序。这些宏在头文件[BeanWrapperMacro.h](http://autumnframework.googlecode.com/svn/trunk/doc/CppDoc/projects/autumnframework/BeanWrapperMacro.h.html)中定义。定义Wrapper，除了必需的AUTUMNBEAN(_beanName_)和AUTUMNBEAN\_END(_beanName_)首尾两个宏以外，还有Bean的实例化方式、属性设置方法、初始化方法、销毁方法、释放方法以及本地Bean的定义等。Wrapper都写在一个.cpp文件中，一个文件中可以定义多个Bean的Wrapper，需要包括的头文件有：
  * _BeanWrapperMacro.h_
  * 用户定义Bean的头文件
  * 如果是本地Bean，要包含\_LocalBean.h_下面是一个Wrapper的例子：
```
#include "BeanWrapperMacro.h"
#include "BeanTypeBean.h"

//Wrapper的开始，必须用AUTUMNBEAN
AUTUMNBEAN(BeanTypeBean)

//定义Bean的实例化方式，这个Bean有两个构造方法：一个没有参数，一个有1个参数
AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(BeanTypeBean)

AUTUMNBEAN_CONSTRUCTOR_1P(BeanTypeBean, BasicTypesBean*)
AUTUMNBEAN_CREATOR_END()

//定义Bean的属性设置方法，有两个属性：Int和CStr
AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Int, BasicTypesBean*)
AUTUMNBEAN_PROPERTY(CStr, BasicTypesBean)
AUTUMNBEAN_SETTER_END()

//Wrapper定义的结束，用AUTUMNBEAN_END
AUTUMNBEAN_END(BeanTypeBean)

//如果BeanTypeBean是本地Bean，则加下面的声明
//AUTUMN_LOCALBEAN(BeanTypeBean)
```_

下面逐一介绍Wrapper的各个方面：
### 3.2.1. 开始 ###
每个Wrapper定义的开始必须是AUTUMNBEAN(beanName)，其中beanName是Bean的名称，它必须是POCO的类名。如果你看一下头文件\_BeanWrapperMacro.h_就会知道为什么了。因为Wrapper中将用它来声明成员、来创建Bean等。
### 3.2.2. 实例化 ###
Bean实例化方式的定义由宏AUTUMNBEAN\_CREATOR()开头，由AUTUMNBEAN\_CREATOR\_END()结尾。
实例化Bean有三种方式：构造方法、静态工厂方法、实例工厂。在Autumn中，这三种方式只能选用其一。一个bean的wrapper不能同时支持两种或两种以上的实例化方式。但当选用某一种方式时，可以有多个实例化的方法。比如，Autumn支持参数个数不同的多个构造方法，或方法名称不同的多个工厂方法。
#### 1. 构造方法 ####
构造方法由AUTUMNBEAN\_CONSTRUCTOR(beanName)的一系列宏来完成。如果构造方法没有参数，直接用
```
AUTUMNBEAN_CONSTRUCTOR(beanName)
```
就可以了。如果构造方法有一个参数，可以用如下宏定义：
```
AUTUMNBEAN_CONSTRUCTOR_1P(beanName, type)
```
其中type指出了构造参数的类型，该类型应该是Autumn支持的类型，或是Bean的类型（即类名），不应该是自定义类型，除非你让Autumn知道你的自定义类型（见3.7. 自定义类型）。如果有构造方法有两个参数，可以用如下宏来定义：
```
AUTUMNBEAN_CONSTRUCTOR_2P(beanName, type1, type2)
```
其中type1, type2分别指出了构造方法两个参数的类型。如果构造方法有更多的参数，现在\_BeanWrapperMacro.h_ 中还没有相对应的宏，不过，可以参照上面这几个宏，来定义具有更多参数的构造方法的宏。（为了测试的需要，我在\_BeanWrapperMacro.h_中定义了一个具有13个构造参数的宏。）
如前面的那个例子所示，我们可以为一个Bean定义多个构造方法，但是这些构造方法的参数个数必须不同。现在不支持相同参数个数、不同参数类型的构造方法重载。下面的静态工厂方法和实例工厂也一样，支持参数个数不同的方法重载，不支持仅参数类型不同的方法重载。
#### 2. 静态工厂方法 ####
静态工厂方法是指如Singleton模式的创建方法，这些方法都是POCO类的静态方法。采用静态工厂方法创建实例的类，一般都不允许直接使用构造方法来创建实例，如Singleton模式。静态工厂方法由AUTUMNBEAN\_CON\_METHOD(beanName, method)一系列宏来完成。如果没有构造参数，可直接用
```
AUTUMNBEAN_CON_METHOD(beanName, method)
```
method是静态工厂方法的名称。如果有一个构造参数，可以用如下宏定义：
```
AUTUMNBEAN_CON_METHOD_1P(beanName, method, type)
```
其中type指出了构造参数的类型。如果有两个构造参数，就用AUTUMNBEAN\_CON\_METHOD\_2P(beanName, method, type1, type2)。如果有更多构造参数，可以自己参照以上这些宏来定义相应的宏，进行使用。静态工厂方法的例子如下：
```
#include "BeanWrapperMacro.h"
#include "FactoryMethodBean.h"

AUTUMNBEAN(ProductA_M)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CON_METHOD_1P(ProductA_M, getInstance, string)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_DELETOR(ProductA_M, deleteInstance)
AUTUMNBEAN_END(ProductA_M)

AUTUMNBEAN(ProductB_M)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CON_METHOD(ProductB_M, getInstance)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_DELETOR(ProductB_M, deleteInstance)
AUTUMNBEAN_END(ProductB_M)
```
#### 3. 实例工厂 ####
实例工厂对应于抽象工厂模式。可以使用AUTUMNBEAN\_CON\_FACTORY(factory, method)一系列宏来完成。因为是由工厂及其方法来创建Bean，所以这些宏不需要Bean的Name，而是需要工厂的类型及工厂的方法。没有构造参数时，可直接用如下宏定义：
```
AUTUMNBEAN_CON_FACTORY(factory, method)
```
其中，factory指出了抽象工厂的类名，method指出了抽象工厂创建这个产品（Bean）的方法。如果有一个构造参数，用如下宏定义：
```
AUTUMNBEAN_CON_FACTORY_1P(factory, method, type)
```
其中type指出了构造参数的类型。跟前面的构造方法及静态工厂方法类似，如果有两个构造参数，就用AUTUMNBEAN\_CON\_FACTORY\_2P(factory, method, type1, type2)。如果有更多构造参数，就自己定义相对应的宏。_

为了使用实例工厂，需要把具体工厂的类也定义为Bean。这样，Autumn才能够创建工厂的实例，用工厂的实例来创建产品的实例。实例工厂的Wrapper例子如下，先是具体工厂Bean的Wrapper，然后是产品Bean的Wrapper：
```
AUTUMNBEAN(Factory1)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(Factory1)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_END(Factory1)

AUTUMNBEAN(Factory2)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(Factory2)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_END(Factory2)
```
这儿，Factory1和Facotry2都是具体工厂，是抽象工厂IFacotry的具体实现。
```
//product A
AUTUMNBEAN(IProductA)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CON_FACTORY_1P(IFactory, createProductA, string)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_END(IProductA)

//product B
AUTUMNBEAN(IProductB)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CON_FACTORY_1P(IFactory, createProductB, string)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_END(IProductB)
```
### 3.2.3. 属性设置 ###
属性设置由宏AUTUMNBEAN\_SETTER()开始，由宏AUTUMNBEAN\_SETTER\_END()结束。对于每一个属性，采用如下宏定义：
```
AUTUMNBEAN_PROPERTY(pname, ptype)
```
pname是属性的名称，对应于POCO的setXXX()方法中的XXX部分；ptype是属性的类型，跟构造时的参数类型一样，ptype应该是Autumn支持的类型，或者是类名。现在，Autumn不支持属性设置方法的重载，即对同一个属性，Wrapper中只能定义一次。下面是一个属性设置的例子：
```
AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Char,char)	
AUTUMNBEAN_PROPERTY(UInt,unsigned int)
AUTUMNBEAN_PROPERTY(CStr,char*)	
AUTUMNBEAN_PROPERTY(IntP,int*)	
AUTUMNBEAN_SETTER_END()
```
这四个属性的名称分别为Char、UInt、CStr和IntP，对应的参数类型分别为char、unsigned int、char\*和int**。
### 3.2.4. 初始化及析构回调 ###
#### 1. 初始化回调 ####
有些POCO在创建之后，不能马上被使用，需要把相关属性设置完后，执行一个初始化方法，然后再进行使用。如果在Wrapper中说明了这个初始化方法，Autumn就会在创建Bean，并将各属性注入完成后，回调该初始化方法来对Bean进行初始化。初始化方法用下面这个宏来说明：
```
AUTUMNBEAN_INIT(initMethod)
```
其中initMethod是POCO的一个方法，用来进行初始化操作。**

只要在Wrapper中说明了初始化方法，Autumn就会回调这个方法，不需要在Autumn的配置文件中进行配置。
#### 2. 析构回调 ####
析构回调跟初始化回调相对应，是在调用delete释放Bean之前调用的一个方法，一般用来对Bean所用的资源进行释放或预处理，以保证Bean正确地被释放。析构回调方法用下面这个宏来说明：
```
AUTUMNBEAN_DESTROY(destroyMethod)
```
其中destroyMethod是POCO的一个方法，用来进行析构前的预处理。

同样，只要在Wrapper中说明了析构回调方法，Autumn就会回调这个方法，不需要在Autumn的配置文件中进行配置。
### 3.2.5. 销毁 ###
一般情况下，Autumn调用delete来释放Bean。但在有些情况下，直接调用delete可能不合适，特别是那些用静态工厂方法创建的Bean，它们可能不能随便delete，只有在某些情况下才允许delete。所以这些POCO需要实现一个用来销毁实例的静态方法（对应于创建实例的静态工厂方法）。如果在Wrapper中说明了这个方法，Autumn将调用这个方法来释放Bean，而不是调用delete。用下面这个宏来说明销毁实例的方法：
```
AUTUMNBEAN_DELETOR(beanName, deleteMethod)
```
其中，deleteMethod是POCO的一个静态方法，它有一个参数，是这个POCO的指针类型。
### 3.2.6. 结束 ###
至此，Wrapper对象中可以定义的内容都介绍完了，在Wrapper定义的最后，必须用下面这个宏来结束：
```
AUTUMNBEAN_END(beanName)
```
### 3.2.7. 本地化 ###
如果一个Bean的实现不是放在动态库中，而是在主程序中，那么必须将这个Bean声明为本地化的Bean。即在Bean的Wrapper定义完成之后，再用下面这个宏声明一下：
```
AUTUMN_LOCALBEAN(beanName)
```
## 3.3. 写主程序 ##
主程序中使用BeanFacotry来取得Bean。需要包含头文件IBeanFactory.h。Autumn中现在没有应用上下文之类的东西。使用如下方法来取得和释放BeanFactory：
```
/** Get IBeanFactory instance */
IBeanFactory* getBeanFactoryWithXML(const char* file);
/** Delete IBeanFactory instance */
void deleteBeanFactory(IBeanFactory* p);
```
其中file是Autumn的配置文件名。BeanFactory不是单例模式，每次调用getBeanFactoryWithXML()方法都会产生一个新的实例。BeanFactory有下面几个方法可供使用：
```
	/** 
	 * Get a bean with bean's name
	 * @param name Bean's name
	 * @return A pointer to the bean
	 */
	virtual void* getBean(string name);

	/** 
	 * Free a bean
	 * @param p A pointer to the bean
	 */
	virtual void freeBean(void* p);
	
	/** 
	 * A bean exists or not
	 * @param name Bean's name
	 * @return True if it exists, or false.
	 */
	virtual bool containsBean(string name);

	/** 
	 * A bean is a singleton or not
	 * @param name Bean's name
	 * @return True if it's a singleton, or false.
	 */
	virtual bool isSingleton(string name);
```
下面是一段主程序的例子：
```
#include <iostream>
#include <exception>
#include "Action.h"
#include "IBeanFactory.h"

using namespace std;

int main(int argc, char* argv[])
{
	try{
		IBeanFactory* bf;
		bf = getBeanFactoryWithXML("autumnDemo.xml");

		Action* pa = (Action*)bf->getBean("TheAction");
		cout<<pa->excute("Vicent")<<endl;
		bf->freeBean(pa);
		
		deleteBeanFactory(bf);
		cout<<"OK!"<<endl;
	}catch (exception* e ) {
		cout<<"Exception: "<<e->what()<<endl;
		delete e;
	}
	return 0;
}
```

## 3.4. 编译程序 ##
采用模块形式，我们的应用会由多个动态库和一个简单的主程序组成。
### 3.4.1. 编译动态库 ###
一般情况下，因为Wrapper跟POCO关系比较大，所以我们将Wrapper编译在动态库里。编译时需要连接Autumn的动态库，在windows平台上是AutumnFramework.dll，在其它平台上是libAutumn.so。
### 3.4.2. 编译主程序 ###
因为应用的动态库是由Autumn动态装入的，所以在编译主程序时不需要连接应用的动态库，但需要连接Autumn的动态库。
## 3.5. 写配置文件 ##
当前，Autumn的配置文件是比较简单的，具体可以参见[schema](http://autumnframework.googlecode.com/files/Autumn-0.3.0.xsd)。可用任何XML文件编辑器来编写Autumn的配置文件。根标签是

&lt;autumn&gt;

，其下由若干

&lt;library&gt;

组成。
### 3.5.1. 库 ###
库，就是指动态库，它有三个属性：name、path和namespace，其中path是必需的，它设置不正确，会使Autumn找不到动态库。namespace当前没有用处。

Autumn中有一个特殊的库：主程序，它的path定死了，为"local“。



&lt;library&gt;

标签下是

&lt;beans&gt;

标签和

&lt;types&gt;

标签，其下分别放置本库所有的Bean和自定义Type。
### 3.5.2. Bean ###
一个Bean共有以下几个方面需要配置。
#### 1. Bean属性 ####


&lt;bean&gt;

的属性有以下几个：
  * **name:** Bean的名称，调用BeanFactory的getBean()方法，传入的就是这个名字。它不是必需的，如果没有定义，就采用class的值。
  * **class:** Bean的POCO类名，是必需的。Autumn将根据这个类名查找相应的Wrapper，来进行Bean的创建、注入等操作。
  * **singleton:** 指明该Bean是否为单例，如果值为true，则在BeanFactory中只保留一个实例，不管对其调用多少次getBean()。如果值为false，则每次对该Bean调用getBean()时都创建一个新的实例。如果一个POCO本身采用了单例模式实现，在这儿最好也将本属性设为true，否则可能会出错。（当然，如果你在Wrapper中也指定了销毁方法的话，可能不会出错。）
  * **factory-bean:** 指明工厂实例的Bean名称。它必须是在Autumn中的Bean，所以采用实例工厂创建Bean时，必须为具体的工厂实现定义Wrapper，并在Autumn中定义为Bean。
  * **factory-method:** 当采用静态工厂方法或工厂实例的方式来创建Bean时，用来指明工厂方法。
#### 2. 构造参数 ####
Autumn支持构造子注入。构造参数

&lt;argument&gt;

有一个属性type和两个元素

&lt;value&gt;

、

&lt;ref&gt;

：
  * **type:** 它不是必须的，如果指定了type，Autumn将按照它来创建构造参数，否则，将按照Wrapper中定义的类型来创建构造参数。
  * **value:** 用来指定参数的值。它可以是多个，但在当前Autumn，只有第一个有用。
  * **ref:** 如果注入的是Bean的话，就使用

&lt;ref&gt;

，它的属性bean指出了Bean的名称。



&lt;value&gt;

和

&lt;ref&gt;

只能选用其一。
#### 3. 属性注入 ####
Autumn也支持属性注入。属性

&lt;property&gt;

的配置跟构造参数类似，只是多了一个必须的属性：name。name指出了Bean属性的名称，它必须跟Wrapper中定义的属性名称相同。
#### 4. 依赖 ####
Autumn允许为一个Bean指定它所依赖的Bean，以保证先创建这些被依赖的Bean。被依赖的Bean应该是一些单例，即其singleton属性应该设为true，否则没有意义。
### 3.5.3. Type ###
一个库中除了Bean，还可以有自定义类型，由

&lt;type&gt;

标签来定义。它只有两个属性：
  * **name:** 用来指出类型的名称，不是必须的，如果没有指明的话，就采用class的值。
  * **class:** 用来指出自定义类型的类名，是必须的。Autumn将根据这个类名查找相应的Wrapper。
## 3.6. 运行程序 ##
运行程序没有特别之处，只是将Autumn的动态库放在系统能够找到得的地方。

如果配置了名为“AutumnFrameworkLog”的Bean，Autumn将会采用这个Bean来记日志。Autumn提供了一个日志实现：FileLogger，可以为它配置日志文件名称及日志级别（示例可以参见源码目录下test\FrameworkTest.xml）。如果定义了日志，Autumn注入过程的一些信息为记录下来。
## 3.7. 自定义类型 ##
Autumn支持自定义类型。自定义类型的实现需要继承接口类IAutumnType，并实现其中的纯虚函数。在实现时，可以使用TypeManager。

自定义类型的Wrapper用一个宏就可以定义：AUTUMNTYPE(typeName)。
# 4. 注意事项 #
  * 因为Autumn框架负责注入数据的创建及释放，所以，如果在应用程序中释放注入数据的话，可能会在程序运行时出错。