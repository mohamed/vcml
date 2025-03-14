/******************************************************************************
 *                                                                            *
 * Copyright 2021 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_SERIAL_BACKEND_H
#define VCML_SERIAL_BACKEND_H

#include "vcml/core/types.h"

namespace vcml {
namespace serial {

class terminal;

class backend
{
protected:
    terminal* m_term;
    string m_type;

public:
    backend(terminal* term, const string& type);
    virtual ~backend();

    backend() = delete;
    backend(const backend&) = delete;
    backend(backend&&) = default;

    terminal* term() const { return m_term; }

    const char* type() const { return m_type.c_str(); }

    virtual bool read(u8& val) = 0;
    virtual void write(u8 val) = 0;

    static backend* create(terminal* term, const string& type);
};

} // namespace serial
} // namespace vcml

#endif
