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

#include "BeanWrapperMacro.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"

AUTUMNBEAN(ConsoleLogger)
AUTUMNBEAN_CON_PARAMS(ConsoleLogger, 1, int)
AUTUMNBEAN_CON_PARAMS_END()
AUTUMNBEAN_END(ConsoleLogger)
AUTUMN_LOCALBEAN(ConsoleLogger)

AUTUMNBEAN(FileLogger)
AUTUMNBEAN_CON_PARAMS(FileLogger, 2, string)
AUTUMNBEAN_CON_PARAM(int)
AUTUMNBEAN_CON_PARAMS_END()
AUTUMNBEAN_END(FileLogger)
AUTUMN_LOCALBEAN(FileLogger)
