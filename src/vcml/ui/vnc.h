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

#ifndef VCML_UI_VNC_H
#define VCML_UI_VNC_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"

#include "vcml/logging/logger.h"

#include "vcml/ui/keymap.h"
#include "vcml/ui/video.h"
#include "vcml/ui/display.h"

#include <rfb/rfb.h>
#include <rfb/keysym.h>

namespace vcml {
namespace ui {

class vnc : public display
{
private:
    u16 m_port;
    u32 m_buttons;
    u32 m_ptr_x;
    u32 m_ptr_y;
    atomic<bool> m_running;
    mutex m_mutex;
    rfbScreenInfo* m_screen;
    thread m_thread;

    void run();

public:
    u16 port() const { return m_port; }

    vnc(u32 nr);
    virtual ~vnc();

    virtual void init(const videomode& mode, u8* fb) override;
    virtual void render(u32 x, u32 y, u32 w, u32 h) override;
    virtual void render() override;
    virtual void shutdown() override;

    void key_event(u32 sym, bool down);
    void ptr_event(u32 mask, u32 x, u32 y);

    static display* create(u32 nr);
};

} // namespace ui
} // namespace vcml

#endif
