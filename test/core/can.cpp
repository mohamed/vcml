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

#include "testing.h"

TEST(can, to_string) {
    can_frame frame;
    frame.msgid = 0x123;
    frame.dlc = len2dlc(4);
    frame.flags = 0;
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0x33;
    frame.data[3] = 0x44;

    stringstream ss;
    ss << frame;
    std::cout << frame << std::endl;
}

MATCHER_P(can_match_socket, name, "Matches a CAN socket") {
    return strcmp(arg.basename(), name) == 0;
}

MATCHER_P(can_match_frame, frame, "Matches a CAN frame") {
    return arg == frame;
}

class can_bench : public test_base, public can_host
{
public:
    can_initiator_socket can_tx;
    can_base_initiator_socket can_tx_h;
    can_base_target_socket can_rx_h;
    can_target_socket can_rx;

    can_initiator_socket_array<> can_array_tx;
    can_target_socket_array<> can_array_rx;

    can_bench(const sc_module_name& nm):
        test_base(nm),
        can_host(),
        can_tx("can_tx"),
        can_tx_h("can_tx_h"),
        can_rx_h("can_rx_h"),
        can_rx("can_rx"),
        can_array_tx("can_array_tx"),
        can_array_rx("can_array_rx") {
        can_tx.bind(can_tx_h);
        can_rx_h.bind(can_rx);
        can_tx_h.bind(can_rx_h);

        can_array_tx[4].bind(can_array_rx[4]);
        can_array_tx[5].stub();
        can_array_rx[6].stub();

        // did the ports get created?
        EXPECT_TRUE(find_object("can.can_array_tx[4]"));
        EXPECT_TRUE(find_object("can.can_array_rx[4]"));
        EXPECT_TRUE(find_object("can.can_array_tx[5]"));
        EXPECT_TRUE(find_object("can.can_array_rx[6]"));

        // did the stubs get created?
        EXPECT_TRUE(find_object("can.can_array_tx[5]_stub"));
        EXPECT_TRUE(find_object("can.can_array_rx[6]_stub"));
    }

    MOCK_METHOD(void, can_receive, (const can_target_socket&, can_frame&),
                (override));

    virtual void run_test() override {
        wait(SC_ZERO_TIME);

        can_frame frame;
        frame.msgid = 0x123;
        frame.dlc = len2dlc(4);
        frame.flags = 0;
        frame.data[0] = 0x11;
        frame.data[1] = 0x22;
        frame.data[2] = 0x33;
        frame.data[3] = 0x44;

        EXPECT_CALL(*this, can_receive(can_match_socket("can_rx"),
                                       can_match_frame(frame)));
        can_tx.send(frame);
    }
};

TEST(can, simulate) {
    can_bench bench("can");
    sc_core::sc_start();
}
