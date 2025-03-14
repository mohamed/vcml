/******************************************************************************
 *                                                                            *
 * Copyright 2018 Jan Henrik Weinstock                                        *
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

#ifndef VCML_OPENCORES_OMPIC_H
#define VCML_OPENCORES_OMPIC_H

#include "vcml/core/types.h"
#include "vcml/core/range.h"
#include "vcml/core/systemc.h"
#include "vcml/core/peripheral.h"

#include "vcml/protocols/tlm.h"
#include "vcml/protocols/gpio.h"

namespace vcml {
namespace opencores {

class ompic : public peripheral
{
private:
    unsigned int m_num_cores;

    u32* m_control;
    u32* m_status;

    u32 read_status(size_t core_idx);
    u32 read_control(size_t core_idx);

    void write_control(u32 val, size_t core_idx);

    // Disabled
    ompic();
    ompic(const ompic&);

public:
    enum control_bits { CTRL_IRQ_GEN = 1 << 30, CTRL_IRQ_ACK = 1 << 31 };

    reg<u32>** control;
    reg<u32>** status;

    gpio_initiator_socket_array<> irq;
    tlm_target_socket in;

    ompic(const sc_module_name& name, unsigned int num_cores);
    virtual ~ompic();
    VCML_KIND(opencores::ompic);
};

} // namespace opencores
} // namespace vcml

#endif
