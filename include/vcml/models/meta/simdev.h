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

#ifndef VCML_META_SIMDEV_H
#define VCML_META_SIMDEV_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"
#include "vcml/core/peripheral.h"

#include "vcml/protocols/tlm.h"

namespace vcml {
namespace meta {

class simdev : public peripheral
{
private:
    void write_stop(u32 val);
    void write_exit(u32 val);
    void write_abrt(u32 val);

    u64 read_sclk();
    u64 read_hclk();

    void write_sout(u32 val);
    void write_serr(u32 val);

    u32 read_prng();

    // disabled
    simdev();
    simdev(const simdev&);

public:
    // simulation control
    reg<u32> stop;
    reg<u32> exit;
    reg<u32> abrt;

    // timing
    reg<u64> sclk;
    reg<u64> hclk;

    // output
    reg<u32> sout;
    reg<u32> serr;

    // random
    reg<u32> prng;

    tlm_target_socket in;

    simdev(const sc_module_name& name);
    virtual ~simdev();
    VCML_KIND(simdev);

    virtual void reset() override;
};

} // namespace meta
} // namespace vcml

#endif
