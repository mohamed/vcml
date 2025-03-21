/******************************************************************************
 *                                                                            *
 * Copyright 2022 Jan Henrik Weinstock                                        *
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

#ifndef VCML_CAN_BACKEND_FILE_H
#define VCML_CAN_BACKEND_FILE_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"

#include "vcml/logging/logger.h"

#include "vcml/models/can/backend.h"
#include "vcml/models/can/bridge.h"

namespace vcml {
namespace can {

class backend_file : public backend
{
private:
    size_t m_count;
    ofstream m_tx;

public:
    backend_file(bridge* br, const string& tx);
    virtual ~backend_file();

    virtual void send_to_host(const can_frame& frame) override;

    static backend* create(bridge* br, const string& type);
};

} // namespace can
} // namespace vcml

#endif
