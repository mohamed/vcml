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

#ifndef VCML_TRACER_TERM_H
#define VCML_TRACER_TERM_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"

#include "vcml/tracing/tracer.h"

namespace vcml {

class tracer_term : public tracer
{
private:
    bool m_colors;
    ostream& m_os;

    template <typename PAYLOAD>
    void do_trace(const activity<PAYLOAD>& msg);

public:
    bool has_colors() const { return m_colors; }
    void set_colors(bool set = true) { m_colors = set; }

    virtual void trace(const activity<tlm_generic_payload>&) override;
    virtual void trace(const activity<gpio_payload>&) override;
    virtual void trace(const activity<clk_payload>&) override;
    virtual void trace(const activity<pci_payload>&) override;
    virtual void trace(const activity<i2c_payload>&) override;
    virtual void trace(const activity<spi_payload>&) override;
    virtual void trace(const activity<sd_command>&) override;
    virtual void trace(const activity<sd_data>&) override;
    virtual void trace(const activity<vq_message>&) override;
    virtual void trace(const activity<serial_payload>&) override;
    virtual void trace(const activity<eth_frame>&) override;
    virtual void trace(const activity<can_frame>&) override;

    tracer_term(bool use_cerr = false);
    tracer_term(bool use_cerr, bool use_colors);
    virtual ~tracer_term();

    static size_t trace_name_length;
    static size_t trace_indent_incr;
    static size_t trace_curr_indent;

    static array<const char*, NUM_PROTOCOLS> colors;
};

} // namespace vcml

#endif
