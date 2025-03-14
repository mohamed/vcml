/******************************************************************************
 *                                                                            *
 * Copyright 2020 Jan Henrik Weinstock                                        *
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

#include "testing.h"

class loader_test : public test_base
{
public:
    generic::memory imem;
    generic::memory dmem;

    generic::bus ibus;
    generic::bus dbus;

    meta::loader loader;

    loader_test(const sc_module_name& nm = sc_gen_unique_name("test")):
        test_base(nm),
        imem("imem", 0x2000),
        dmem("dmem", 0x2000),
        ibus("ibus"),
        dbus("dbus"),
        loader("loader", get_resource_path("elf.elf")) {
        ibus.bind(loader.insn);
        ibus.bind(imem.in, { 0x400000, 0x400fff });

        dbus.bind(loader.data);
        dbus.bind(dmem.in, { 0x601000, 0x601fff });

        clk.bind(imem.clk);
        clk.bind(dmem.clk);
        clk.bind(ibus.clk);
        clk.bind(dbus.clk);

        rst.bind(imem.rst);
        rst.bind(dmem.rst);
        rst.bind(ibus.rst);
        rst.bind(dbus.rst);
    }

    virtual void run_test() override {
        u32 code_start = 0;
        u32 global_a = 0;
        u64 global_b = 0;

        ASSERT_OK(loader.insn.readw(0x400000, code_start));
        ASSERT_OK(loader.data.readw(0x601000, global_b));
        ASSERT_OK(loader.data.readw(0x601008, global_a));

        EXPECT_EQ(code_start, fourcc("\x7f"
                                     "ELF"));
        EXPECT_EQ(global_a, 4);
        EXPECT_EQ(global_b, 0x42);
    }
};

TEST(loader, simulate) {
    loader_test stim;
    sc_core::sc_start();
}
