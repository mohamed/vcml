/******************************************************************************
 *                                                                            *
 * Copyright 2019 Jan Henrik Weinstock                                        *
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

#include "vcml/models/spi/bus.h"

namespace vcml {
namespace spi {

bus::bus(const sc_module_name& nm):
    component(nm), spi_host(), spi_in("spi_in"), spi_out("spi_out"), cs("cs") {
}

bus::~bus() {
    // nothing to do
}

void bus::reset() {
    component::reset();
}

bool bus::is_valid(unsigned int port) const {
    if (!cs.exists(port) || !spi_out.exists(port))
        return false;
    if (!stl_contains(m_csmode, port))
        return false;
    return true;
}

bool bus::is_active(unsigned int port) const {
    if (!is_valid(port))
        return false;
    return cs[port].read() == m_csmode.at(port);
}

bool bus::is_active_high(unsigned int port) const {
    if (!is_valid(port))
        return false;
    return m_csmode.at(port);
}

bool bus::is_active_low(unsigned int port) const {
    if (!is_valid(port))
        return false;
    return !m_csmode.at(port);
}

void bus::spi_transport(const spi_target_socket&, spi_payload& spi) {
    for (auto port : cs) {
        if (is_active(port.first))
            spi_out[port.first].transport(spi);
    }
}

unsigned int bus::next_free() const {
    unsigned int idx = 0;
    while (spi_out.exists(idx) || cs.exists(idx))
        VCML_ERROR_ON(++idx == 0, "no free ports");
    return idx;
}

void bus::bind(spi_initiator_socket& initiator) {
    spi_in.bind(initiator);
}

unsigned int bus::bind(spi_target_socket& target, gpio_initiator_socket& s,
                       bool cs_active_high) {
    unsigned int port = next_free();
    spi_out[port].bind(target);
    s.bind(cs[port]);
    m_csmode[port] = cs_active_high;
    return port;
}

} // namespace spi
} // namespace vcml
