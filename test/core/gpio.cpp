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

TEST(gpio, to_string) {
    gpio_payload tx;
    tx.vector = 42;
    tx.state = true;

    // no formatting checks, just make sure it compiles
    std::cout << tx << std::endl;
}

MATCHER_P(gpio, name, "Matches a gpio socket by name") {
    return strcmp(arg.basename(), name) == 0;
}

class gpio_test_harness : public test_base
{
public:
    gpio_initiator_socket out;
    gpio_initiator_socket out2;
    gpio_target_socket_array<> in;

    // for testing hierarchical binding
    gpio_base_initiator_socket h_out;
    gpio_base_target_socket h_in;

    // for adapter testing
    gpio_initiator_socket a_out;
    sc_signal<bool> signal;
    gpio_target_socket a_in;

    gpio_test_harness(const sc_module_name& nm):
        test_base(nm),
        out("out"),
        out2("out2"),
        in("in"),
        h_out("h_out"),
        h_in("h_in"),
        a_out("a_out"),
        signal("signal"),
        a_in("a_in") {
        out.bind(in[0]);

        // check hierarchical binding: out -> h_out -> h_in -> in[1]
        out.bind(h_out);
        h_in.bind(in[1]);
        h_out.bind(h_in);

        // check stubbing
        out2.stub();
        in[2].stub();
        EXPECT_TRUE(out2.is_stubbed());
        EXPECT_TRUE(in[2].is_stubbed());

        // check adapters
        a_out.bind(signal);
        a_in.bind(signal);

        // did the port get created?
        EXPECT_TRUE(find_object("gpio.out2"));
        EXPECT_TRUE(find_object("gpio.in[2]"));

        // did the stubs get created?
        EXPECT_TRUE(find_object("gpio.out2_stub"));
        EXPECT_TRUE(find_object("gpio.in[2]_stub"));

        // did the adapters get created?
        EXPECT_TRUE(find_object("gpio.a_out_adapter"));
        EXPECT_TRUE(find_object("gpio.a_in_adapter"));

        auto initiators = all_gpio_initiator_sockets();
        auto targets = all_gpio_target_sockets();
        auto sockets = all_gpio_target_sockets(0);

        EXPECT_EQ(initiators.size(), 3) << "gpio initiators did not register";
        EXPECT_EQ(targets.size(), 5) << "gpio targets did not register";
        EXPECT_FALSE(sockets.empty()) << "gpio targets in wrong address space";
    }

    MOCK_METHOD(void, gpio_notify,
                (const gpio_target_socket&, bool, gpio_vector));

    virtual void run_test() override {
        // this also forces construction of IN[0]'s default event so that it
        // it can be triggered later on
        EXPECT_TRUE(in[0].default_event().name());

        enum test_vectors : gpio_vector {
            TEST_VECTOR = 0x42,
        };

        // out is bound to in[0] and in[1], so we have to expect each callback
        // to be invoked twice.

        // test callbacks
        EXPECT_CALL(*this, gpio_notify(gpio("in[0]"), true, GPIO_NO_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[1]"), true, GPIO_NO_VECTOR));
        out = true;
        out = true; // should not trigger a second time
        EXPECT_TRUE(in[0]);
        EXPECT_TRUE(in[1]);

        EXPECT_CALL(*this, gpio_notify(gpio("in[0]"), false, GPIO_NO_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[1]"), false, GPIO_NO_VECTOR));
        out = false;
        out = false; // should not trigger a second time
        EXPECT_FALSE(in[0]);
        EXPECT_FALSE(in[1]);

        // test vectors
        EXPECT_CALL(*this, gpio_notify(gpio("in[0]"), true, TEST_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[1]"), true, TEST_VECTOR));
        out.raise(TEST_VECTOR);
        EXPECT_TRUE(in[0][TEST_VECTOR]);
        EXPECT_TRUE(in[1][TEST_VECTOR]);

        EXPECT_CALL(*this, gpio_notify(gpio("in[0]"), false, TEST_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[1]"), false, TEST_VECTOR));
        out.lower(TEST_VECTOR);
        EXPECT_FALSE(in[0].read(TEST_VECTOR));
        EXPECT_FALSE(in[1].read(TEST_VECTOR));

        // test default events
        EXPECT_CALL(*this, gpio_notify(gpio("in[0]"), true, GPIO_NO_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[1]"), true, GPIO_NO_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[0]"), false, GPIO_NO_VECTOR));
        EXPECT_CALL(*this, gpio_notify(gpio("in[1]"), false, GPIO_NO_VECTOR));
        out.pulse();
        wait(in[0].default_event());
        EXPECT_FALSE(in[0]);
        EXPECT_FALSE(in[1]);

        // test adapters
        EXPECT_CALL(*this, gpio_notify(gpio("a_in"), true, GPIO_NO_VECTOR));
        a_out.raise();
        wait(signal.default_event());
        EXPECT_TRUE(a_in);
    }
};

TEST(gpio, sockets) {
    gpio_test_harness test("gpio");
    sc_core::sc_start();
}
