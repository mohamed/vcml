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

#include "testing.h"

class bus_harness : public test_base
{
public:
    generic::memory mem1;
    generic::memory mem2;
    generic::bus bus;

    tlm_initiator_socket out1;
    tlm_initiator_socket out2;
    tlm_target_socket in;

    MOCK_METHOD(void, invalidate, (u64, u64));

    virtual void invalidate_direct_mem_ptr(tlm_initiator_socket& origin,
                                           u64 start, u64 end) override {
        if (sc_get_status() > SC_END_OF_ELABORATION)
            invalidate(start, end);
        tlm_host::invalidate_direct_mem_ptr(origin, start, end);
    }

    bus_harness(const sc_module_name& nm):
        test_base(nm),
        mem1("mem1", 0x2000),
        mem2("mem2", 0x2000),
        bus("bus"),
        out1("out1"),
        out2("out2"),
        in("in") {
        clk.bind(mem1.clk);
        clk.bind(mem2.clk);
        clk.bind(bus.clk);

        rst.bind(mem1.rst);
        rst.bind(mem2.rst);
        rst.bind(bus.rst);

        bus.bind(out1);
        bus.bind(out2);

        bus.bind(mem1.in, 0x0000, 0x1fff, 0);
        bus.bind(mem2.in, 0x2000, 0x3fff, 0);
        bus.bind(mem1.in, 0x6000, 0x7fff, 0);
        bus.bind(in, 0x8000, 0x9fff, 0x10000);

        bus.bind(out1, mem1.in, 0xa000, 0xbfff);
        bus.bind(out2, mem2.in, 0xc000, 0xdfff);
    }

    virtual void run_test() override {
        u32 data;

        ASSERT_OK(out1.writew<u32>(0x0000, 0x11111111ul))
            << "cannot write 0x0000 (mem1 + 0x0)";
        ASSERT_OK(out1.writew<u32>(0x0004, 0xfffffffful))
            << "cannot write 0x0004 (mem1 + 0x4)";
        ASSERT_OK(out1.writew<u32>(0x2000, 0x55555555ul))
            << "cannot write 0x2000 (mem2 + 0x0)";
        ASSERT_OK(out1.writew<u32>(0x2004, 0xbbbbbbbbul))
            << "cannot write 0x2004 (mem2 + 0x4)";
        ASSERT_AE(out1.writew<u16>(0x4000, 0x1234ul))
            << "bus reported success for writing to unmapped address";
        EXPECT_OK(out1.readw<u32>(0x6000, data));
        EXPECT_EQ(data, 0x11111111ul);

        ASSERT_OK(out1.readw<u32>(0x0000, data))
            << "cannot read 0x0000 (mem1 + 0x0)";
        EXPECT_EQ(data, 0x11111111ul)
            << "read invalid data from 0x0000 (mem1 + 0x0)";
        ASSERT_OK(out1.readw<u32>(0x0004, data))
            << "cannot read 0x0004 (mem1 + 0x4)";
        EXPECT_EQ(data, 0xfffffffful)
            << "read invalid data from 0x0004 (mem1 + 0x4)";
        ASSERT_OK(out1.readw<u32>(0x2000, data))
            << "cannot read 0x2000 (mem2 + 0x0)";
        EXPECT_EQ(data, 0x55555555ul)
            << "read invalid data from 0x2000 (mem2 + 0x0)";
        ASSERT_OK(out1.readw<u32>(0x2004, data))
            << "cannot read 0x2004 (mem2 + 0x4)";
        EXPECT_EQ(data, 0xbbbbbbbbul)
            << "read invalid data from 0x2000 (mem2 + 0x4)";
        ASSERT_AE(out1.readw<u32>(0x4000, data))
            << "bus reported success for reading from unmapped address";

        tlm_dmi dmi;
        tlm_dmi_cache& cache = out1.dmi_cache();
        EXPECT_TRUE(cache.lookup(0x0000, 0x2000, TLM_READ_COMMAND, dmi))
            << "bus did not forward DMI region of mem1";
        EXPECT_TRUE(cache.lookup(0x2000, 0x2000, TLM_READ_COMMAND, dmi))
            << "bus did not forward DMI region of mem2";

        if (cache.get_entries().size() > 1) {
            EXPECT_NE(cache.get_entries()[0].get_start_address(),
                      cache.get_entries()[1].get_start_address())
                << "bus forwarded overlapping DMI regions";
            EXPECT_NE(cache.get_entries()[0].get_dmi_ptr(),
                      cache.get_entries()[1].get_dmi_ptr())
                << "bus forwarded overlapping DMI pointers";
        }

        EXPECT_CALL(*this, invalidate(0x0000, 0x1fff)).Times(2);
        EXPECT_CALL(*this, invalidate(0x6000, 0x7fff)).Times(2);
        EXPECT_CALL(*this, invalidate(0xa000, 0xbfff)).Times(1);
        mem1.unmap_dmi(0, 0x1fff);
        ASSERT_EQ(cache.get_entries().size(), 1)
            << "bus did not forward DMI invalidation";
        EXPECT_EQ(cache.get_entries()[0].get_start_address(), 0x2000)
            << "bus invalidated wrong DMI region";

        EXPECT_CALL(*this, invalidate(0x8000, 0x9fff)).Times(2);
        in->invalidate_direct_mem_ptr(0, ~0ull);

        EXPECT_CALL(*this, invalidate(0x8100, 0x8fff)).Times(2);
        in->invalidate_direct_mem_ptr(0x10100, 0x10fff);

        EXPECT_CALL(*this, invalidate(_, _)).Times(0);
        in->invalidate_direct_mem_ptr(0, 0x9fff);

        EXPECT_CALL(*this, invalidate(0x8000, 0x800f)).Times(2);
        in->invalidate_direct_mem_ptr(0x9000, 0x1000f);

        EXPECT_AE(out1.readw<u32>(0xc000, data))
            << "bus transaction went through for area unmapped for out1";
        EXPECT_AE(out2.readw<u32>(0xa000, data))
            << "bus transaction went through for area unmapped for out2";

        EXPECT_OK(out1.readw<u32>(0xa000, data))
            << "cannot access memory at privately mapped area";
        EXPECT_EQ(data, 0x11111111)
            << "unexpected data from memory at privately mapped area";
        EXPECT_OK(out2.readw<u32>(0xc000, data))
            << "cannot access memory at privately mapped area";
        EXPECT_EQ(data, 0x55555555)
            << "unexpected data from memory at privately mapped area";

        bus.execute("mmap", std::cout);
        std::cout << std::endl;
    }
};

TEST(generic_bus, transfer) {
    bus_harness test("test");
    sc_core::sc_start();
}
