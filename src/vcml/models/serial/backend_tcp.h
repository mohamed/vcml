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

#ifndef VCML_SERIAL_BACKEND_TCP_H
#define VCML_SERIAL_BACKEND_TCP_H

#include "vcml/core/types.h"
#include "vcml/logging/logger.h"
#include "vcml/models/serial/backend.h"

namespace vcml {
namespace serial {

class backend_tcp : public backend
{
private:
    mwr::socket m_socket;

public:
    u16 port() const { return m_socket.port(); }

    backend_tcp(terminal* term, u16 port);
    virtual ~backend_tcp();

    virtual bool read(u8& val) override;
    virtual void write(u8 val) override;

    static backend* create(terminal* term, const string& type);
};

} // namespace serial
} // namespace vcml

#endif
