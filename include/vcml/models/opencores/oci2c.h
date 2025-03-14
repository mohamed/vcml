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

#ifndef VCML_OPENCORES_OCI2C_H
#define VCML_OPENCORES_OCI2C_H

#include "vcml/core/types.h"
#include "vcml/core/range.h"
#include "vcml/core/systemc.h"
#include "vcml/core/peripheral.h"

#include "vcml/protocols/tlm.h"
#include "vcml/protocols/gpio.h"
#include "vcml/protocols/i2c.h"

namespace vcml {
namespace opencores {

class oci2c : public peripheral
{
private:
    clock_t m_hz;

    u8 m_tx;
    u8 m_rx;

    void write_ctr(u8 val);
    void write_cmd(u8 val);

    void update();

public:
    enum ctr_bits : u8 {
        CTR_EN = 1u << 7,
        CTR_IEN = 1u << 6,
        CTR_MASK = CTR_EN | CTR_IEN,
    };

    enum tx_bits : u8 {
        TX_RNW = 1u << 0,
    };

    enum cmdr_bits : u8 {
        CMD_STA = 1u << 7,
        CMD_STO = 1u << 6,
        CMD_RD = 1u << 5,
        CMD_WR = 1u << 4,
        CMD_NACK = 1u << 3,
        CMD_IACK = 1u << 0,
        CMD_MASK = CMD_STA | CMD_STO | CMD_RD | CMD_WR | CMD_NACK | CMD_IACK,
    };

    enum sr_bits : u8 {
        SR_NACK = 1u << 7,
        SR_BUSY = 1u << 6,
        SR_AL = 1u << 5,
        SR_TIP = 1u << 1,
        SR_IF = 1u << 0,
        SR_MASK = SR_NACK | SR_BUSY | SR_AL | SR_TIP | SR_IF,
    };

    reg<u8> prerlo;
    reg<u8> prerhi;
    reg<u8> ctr;
    reg<u8> rxr;
    reg<u8> sr;

    tlm_target_socket in;
    gpio_initiator_socket irq;
    i2c_initiator_socket i2c;

    clock_t bus_hz() const { return m_hz; }

    oci2c(const sc_module_name& nm, u8 reg_shift = 0);
    virtual ~oci2c();
    VCML_KIND(opencores::oci2c);
    virtual void reset() override;
};

} // namespace opencores
} // namespace vcml

#endif
