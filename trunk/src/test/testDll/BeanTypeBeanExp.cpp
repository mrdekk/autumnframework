
#include "BeanWrapperMacro.h"
#include "BeanTypeBean.h"

AUTUMNBEAN(BeanTypeBean)

AUTUMNBEAN_CREATOR()
AUTUMNBEAN_CONSTRUCTOR(BeanTypeBean)

AUTUMNBEAN_CONSTRUCTOR_1P(BeanTypeBean, BasicTypesBean*)
AUTUMNBEAN_CREATOR_END()

AUTUMNBEAN_SETTER()
AUTUMNBEAN_PROPERTY(Int, BasicTypesBean*)
AUTUMNBEAN_PROPERTY(CStr, BasicTypesBean)
AUTUMNBEAN_SETTER_END()

AUTUMNBEAN_END(BeanTypeBean)