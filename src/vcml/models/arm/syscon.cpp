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

#include "vcml/models/arm/syscon.h"

namespace vcml {
namespace arm {

syscon::syscon(const sc_module_name& nm):
    peripheral(nm),
    sys_id("sys_id", 0x00),
    lockval("lockval", 0x20),
    cfgdata0("cfgdata0", 0x24),
    cfgdata1("cfgdata1", 0x28),
    flags_s("flags_s", 0x30),
    flags_c("flags_c", 0x34),
    nvflags_s("nvflags_s", 0x38),
    nvflags_c("nvflags_c", 0x3c),
    clck24("clck24", 0x5c),
    proc_id("proc_id", 0x84),
    sys_cfgdata("sys_cfgdata", 0xa0),
    sys_cfgctrl("sys_cfgctrl", 0xa4),
    sys_cfgstat("sys_cfgstat", 0xa8),
    in("IN") {
    sys_id.allow_read_only();

    lockval.allow_read_write();
    lockval.on_write(&syscon::write_lockval);

    cfgdata0.allow_read_write();
    cfgdata1.allow_read_write();

    flags_s.allow_read_write();
    flags_s.on_write(&syscon::write_flags_s);
    flags_c.allow_read_write();
    flags_c.on_write(&syscon::write_flags_c);

    nvflags_s.allow_read_write();
    nvflags_s.on_write(&syscon::write_nvflags_s);
    nvflags_c.allow_read_write();
    nvflags_c.on_write(&syscon::write_nvflags_c);

    clck24.allow_read_only();
    clck24.on_read(&syscon::read_clc_k24);

    proc_id.allow_read_write();

    sys_cfgdata.allow_read_write();

    sys_cfgctrl.allow_read_write();
    sys_cfgctrl.on_write(&syscon::write_sys_cfgctrl);

    sys_cfgstat.allow_read_write();
    sys_cfgstat.on_write(&syscon::write_sys_cfgstat);
}

syscon::~syscon() {
    // nothing to do
}

u32 syscon::read_clc_k24() {
    return sc_time_stamp().to_seconds() * 24.0 * MHz;
}

void syscon::write_lockval(u16 val) {
    if (val != LOCKVAL_LOCK)
        val &= LOCKVAL_M;
    lockval = val;
}

void syscon::write_flags_s(u32 val) {
    flags_s |= val;
    flags_c |= val;
}

void syscon::write_flags_c(u32 val) {
    flags_s &= ~val;
    flags_c &= ~val;
}

void syscon::write_nvflags_s(u32 val) {
    nvflags_s |= val;
    nvflags_c |= val;
}

void syscon::write_nvflags_c(u32 val) {
    nvflags_s &= ~val;
    nvflags_c &= ~val;
}

void syscon::write_sys_cfgctrl(u32 val) {
    if (val & (1 << 31))
        log_warn("SYS_CFGCTRL write trigger action not implemented");
    sys_cfgctrl = val & ~((3 << 18) | (1 << 31));
}

void syscon::write_sys_cfgstat(u32 val) {
    sys_cfgstat = val & 0x3;
}

} // namespace arm
} // namespace vcml
