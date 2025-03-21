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

#include "testing.h"

class test_harness : public test_base
{
public:
    tlm_initiator_socket out;
    tlm_target_socket in;

    test_harness(const sc_module_name& nm):
        test_base(nm), out("out"), in("in") {
        out.stub(TLM_ADDRESS_ERROR_RESPONSE);
        in.stub();
    }

    virtual void run_test() override {
        u32 data = 0;
        sc_time now = local_time_stamp();
        EXPECT_AE(out.writew(0x1234, data))
            << "stub did not response with programmed status";
        EXPECT_EQ(now, local_time_stamp()) << "stub advanced systemc time";

        in.invalidate_dmi(); // no response, but test for aborts
    }
};

TEST(stubs, transactions) {
    test_harness test("harness");
    sc_core::sc_start();
}
