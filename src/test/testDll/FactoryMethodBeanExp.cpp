
#include <string>
#include "BeanWrapperMacro.h"
#include "FactoryMethodBean.h"

int ProductA_M::RefNum = 0;
ProductA_M* ProductA_M::instance = NULL;

int ProductB_M::RefNum = 0;
ProductB_M* ProductB_M::instance = NULL;

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

