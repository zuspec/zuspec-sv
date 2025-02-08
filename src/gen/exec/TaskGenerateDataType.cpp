/*
 * TaskGenerateDataType.cpp
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */
#include "ICustomGen.h"
#include "TaskGenerate.h"
#include "TaskGenerateDataType.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateDataType::TaskGenerateDataType(
        TaskGenerate        *gen,
        IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {

}

TaskGenerateDataType::~TaskGenerateDataType() {

}

void TaskGenerateDataType::generate(vsc::dm::IDataType *t) {
    t->accept(m_this);
}

void TaskGenerateDataType::visitDataTypeBool(vsc::dm::IDataTypeBool *t) {
    m_out->write("bit");
}

void TaskGenerateDataType::visitDataTypeEnum(vsc::dm::IDataTypeEnum *t) {
    m_out->write("%s", m_gen->getNameMap()->getName(t).c_str());
}

void TaskGenerateDataType::visitDataTypeInt(vsc::dm::IDataTypeInt *t) {
    int32_t w = t->getWidth();

    if (w <= 64 && !(w%8)) {
        // Use simple types
        if (w > 32) {
            m_out->write("longint%s", t->isSigned()?"":" unsigned");
        } else if (w > 16) {
            m_out->write("int%s", t->isSigned()?"":" unsigned");
        } else if (w > 8) {
            m_out->write("shortint%s", t->isSigned()?"":" unsigned");
        } else {
            m_out->write("byte%s", t->isSigned()?"":" unsigned");
        }
    } else {
        // Use sized types
        m_out->write("bit%s[%d:0]", t->isSigned()?" signed":"", (w-1));
    }
}

void TaskGenerateDataType::visitDataTypePtr(vsc::dm::IDataTypePtr *t) {

}

void TaskGenerateDataType::visitDataTypeString(vsc::dm::IDataTypeString *t) {
    m_out->write("string");
}

void TaskGenerateDataType::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    m_out->write("%s", m_gen->getNameMap()->getName(t).c_str());
}

}
}
}
}
