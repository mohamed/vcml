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

#include <iostream>
#include <cstring>
#include <gtest/gtest.h>
#include "vcml.h"

using namespace vcml;
using namespace vcml::ui;

TEST(display, videomode) {
    u32 resx = 800;
    u32 resy = 600;
    videomode mode;

    mode = videomode::a8r8g8b8(resx, resy);
    EXPECT_EQ(mode.xres, resx);
    EXPECT_EQ(mode.yres, resy);
    EXPECT_EQ(mode.size, resx * resy * 4);

    mode = videomode::b8g8r8a8(resx, resy);
    EXPECT_EQ(mode.xres, resx);
    EXPECT_EQ(mode.yres, resy);
    EXPECT_EQ(mode.size, resx * resy * 4);

    mode = videomode::r8g8b8(resx, resy);
    EXPECT_EQ(mode.xres, resx);
    EXPECT_EQ(mode.yres, resy);
    EXPECT_EQ(mode.size, resx * resy * 3);

    mode = videomode::b8g8r8(resx, resy);
    EXPECT_EQ(mode.xres, resx);
    EXPECT_EQ(mode.yres, resy);
    EXPECT_EQ(mode.size, resx * resy * 3);

    mode = videomode::r5g6b5(resx, resy);
    EXPECT_EQ(mode.xres, resx);
    EXPECT_EQ(mode.yres, resy);
    EXPECT_EQ(mode.size, resx * resy * 2);

    mode = videomode::gray8(resx, resy);
    EXPECT_EQ(mode.xres, resx);
    EXPECT_EQ(mode.yres, resy);
    EXPECT_EQ(mode.size, resx * resy * 1);
}

TEST(display, server) {
    u16 port1 = 40000;
    u16 port2 = 40001;

    shared_ptr<display> p1 = display::lookup("null:40000");
    shared_ptr<display> p2 = display::lookup("null:40000");
    shared_ptr<display> p3 = display::lookup("null:40001");
    shared_ptr<display> p4 = display::lookup("null:40001");
    shared_ptr<display> p5 = display::lookup("null:40001");

    EXPECT_EQ(p1->dispno(), port1);
    EXPECT_EQ(p2->dispno(), port1);
    EXPECT_EQ(p3->dispno(), port2);
    EXPECT_EQ(p4->dispno(), port2);
    EXPECT_EQ(p5->dispno(), port2);

    EXPECT_EQ(p1, p2);
    EXPECT_EQ(p3, p4);
    EXPECT_EQ(p4, p5);

    EXPECT_NE(p1, p3);
    EXPECT_NE(p1, p4);
    EXPECT_NE(p1, p5);

    EXPECT_NE(p2, p3);
    EXPECT_NE(p2, p4);
    EXPECT_NE(p2, p5);

    EXPECT_EQ(p1.use_count(), 3);
    EXPECT_EQ(p2.use_count(), 3);
    EXPECT_EQ(p3.use_count(), 4);
    EXPECT_EQ(p4.use_count(), 4);
    EXPECT_EQ(p5.use_count(), 4);

    p1->shutdown();
    p2->shutdown();
    p3->shutdown();
    p4->shutdown();
    p5->shutdown();
}
