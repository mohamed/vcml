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

#include "vcml/ui/vnc.h"

namespace vcml {
namespace ui {

static const unordered_map<u32, u32> VNC_KEYSYMS = {
    { XK_0, KEYSYM_0 },
    { XK_1, KEYSYM_1 },
    { XK_2, KEYSYM_2 },
    { XK_3, KEYSYM_3 },
    { XK_4, KEYSYM_4 },
    { XK_5, KEYSYM_5 },
    { XK_6, KEYSYM_6 },
    { XK_7, KEYSYM_7 },
    { XK_8, KEYSYM_8 },
    { XK_9, KEYSYM_9 },

    { XK_A, KEYSYM_A },
    { XK_B, KEYSYM_B },
    { XK_C, KEYSYM_C },
    { XK_D, KEYSYM_D },
    { XK_E, KEYSYM_E },
    { XK_F, KEYSYM_F },
    { XK_G, KEYSYM_G },
    { XK_H, KEYSYM_H },
    { XK_I, KEYSYM_I },
    { XK_J, KEYSYM_J },
    { XK_K, KEYSYM_K },
    { XK_L, KEYSYM_L },
    { XK_M, KEYSYM_M },
    { XK_N, KEYSYM_N },
    { XK_O, KEYSYM_O },
    { XK_P, KEYSYM_P },
    { XK_Q, KEYSYM_Q },
    { XK_R, KEYSYM_R },
    { XK_S, KEYSYM_S },
    { XK_T, KEYSYM_T },
    { XK_U, KEYSYM_U },
    { XK_V, KEYSYM_V },
    { XK_W, KEYSYM_W },
    { XK_X, KEYSYM_X },
    { XK_Y, KEYSYM_Y },
    { XK_Z, KEYSYM_Z },
    { XK_a, KEYSYM_a },
    { XK_b, KEYSYM_b },
    { XK_c, KEYSYM_c },
    { XK_d, KEYSYM_d },
    { XK_e, KEYSYM_e },
    { XK_f, KEYSYM_f },
    { XK_g, KEYSYM_g },
    { XK_h, KEYSYM_h },
    { XK_i, KEYSYM_i },
    { XK_j, KEYSYM_j },
    { XK_k, KEYSYM_k },
    { XK_l, KEYSYM_l },
    { XK_m, KEYSYM_m },
    { XK_n, KEYSYM_n },
    { XK_o, KEYSYM_o },
    { XK_p, KEYSYM_p },
    { XK_q, KEYSYM_q },
    { XK_r, KEYSYM_r },
    { XK_s, KEYSYM_s },
    { XK_t, KEYSYM_t },
    { XK_u, KEYSYM_u },
    { XK_v, KEYSYM_v },
    { XK_w, KEYSYM_w },
    { XK_x, KEYSYM_x },
    { XK_y, KEYSYM_y },
    { XK_z, KEYSYM_z },

    { XK_exclam, KEYSYM_EXCLAIM },
    { XK_quotedbl, KEYSYM_DBLQUOTE },
    { XK_numbersign, KEYSYM_HASH },
    { XK_dollar, KEYSYM_DOLLAR },
    { XK_percent, KEYSYM_PERCENT },
    { XK_ampersand, KEYSYM_AMPERSAND },
    { XK_apostrophe, KEYSYM_QUOTE },
    { XK_parenleft, KEYSYM_LEFTPAR },
    { XK_parenright, KEYSYM_RIGHTPAR },
    { XK_asterisk, KEYSYM_ASTERISK },
    { XK_plus, KEYSYM_PLUS },
    { XK_comma, KEYSYM_COMMA },
    { XK_minus, KEYSYM_MINUS },
    { XK_period, KEYSYM_DOT },
    { XK_slash, KEYSYM_SLASH },
    { XK_colon, KEYSYM_COLON },
    { XK_semicolon, KEYSYM_SEMICOLON },
    { XK_less, KEYSYM_LESS },
    { XK_equal, KEYSYM_EQUAL },
    { XK_greater, KEYSYM_GREATER },
    { XK_question, KEYSYM_QUESTION },
    { XK_at, KEYSYM_AT },
    { XK_bracketleft, KEYSYM_LEFTBRACKET },
    { XK_backslash, KEYSYM_BACKSLASH },
    { XK_bracketright, KEYSYM_RIGHTBRACKET },
    { XK_asciicircum, KEYSYM_CARET },
    { XK_underscore, KEYSYM_UNDERSCORE },
    { XK_quoteleft, KEYSYM_BACKQUOTE },
    { XK_braceleft, KEYSYM_LEFTBRACE },
    { XK_bar, KEYSYM_PIPE },
    { XK_braceright, KEYSYM_RIGHTBRACE },
    { XK_asciitilde, KEYSYM_TILDE },

    { XK_Escape, KEYSYM_ESC },
    { XK_Return, KEYSYM_ENTER },
    { XK_BackSpace, KEYSYM_BACKSPACE },
    { XK_space, KEYSYM_SPACE },
    { XK_Tab, KEYSYM_TAB },
    { XK_Shift_L, KEYSYM_LEFTSHIFT },
    { XK_Shift_R, KEYSYM_RIGHTSHIFT },
    { XK_Control_L, KEYSYM_LEFTCTRL },
    { XK_Control_R, KEYSYM_RIGHTCTRL },
    { XK_Alt_L, KEYSYM_LEFTALT },
    { XK_Alt_R, KEYSYM_RIGHTALT },
    { XK_Meta_L, KEYSYM_LEFTMETA },
    { XK_Meta_R, KEYSYM_RIGHTMETA },
    { XK_Menu, KEYSYM_MENU },
    { XK_Caps_Lock, KEYSYM_CAPSLOCK },

    { XK_F1, KEYSYM_F1 },
    { XK_F2, KEYSYM_F2 },
    { XK_F3, KEYSYM_F3 },
    { XK_F4, KEYSYM_F4 },
    { XK_F5, KEYSYM_F5 },
    { XK_F6, KEYSYM_F6 },
    { XK_F7, KEYSYM_F7 },
    { XK_F8, KEYSYM_F8 },
    { XK_F9, KEYSYM_F9 },
    { XK_F10, KEYSYM_F10 },
    { XK_F11, KEYSYM_F11 },
    { XK_F12, KEYSYM_F12 },

    { XK_Print, KEYSYM_PRINT },
    { XK_Scroll_Lock, KEYSYM_SCROLLOCK },
    { XK_Pause, KEYSYM_PAUSE },

    { XK_Insert, KEYSYM_INSERT },
    { XK_Delete, KEYSYM_DELETE },
    { XK_Home, KEYSYM_HOME },
    { XK_End, KEYSYM_END },
    { XK_Page_Up, KEYSYM_PAGEUP },
    { XK_Page_Down, KEYSYM_PAGEDOWN },

    { XK_Left, KEYSYM_LEFT },
    { XK_Right, KEYSYM_RIGHT },
    { XK_Up, KEYSYM_UP },
    { XK_Down, KEYSYM_DOWN },

    { XK_Num_Lock, KEYSYM_NUMLOCK },
    { XK_KP_0, KEYSYM_KP0 },
    { XK_KP_1, KEYSYM_KP1 },
    { XK_KP_2, KEYSYM_KP2 },
    { XK_KP_3, KEYSYM_KP3 },
    { XK_KP_4, KEYSYM_KP4 },
    { XK_KP_5, KEYSYM_KP5 },
    { XK_KP_6, KEYSYM_KP6 },
    { XK_KP_7, KEYSYM_KP7 },
    { XK_KP_8, KEYSYM_KP8 },
    { XK_KP_9, KEYSYM_KP9 },
    { XK_KP_Enter, KEYSYM_KPENTER },
    { XK_KP_Add, KEYSYM_KPPLUS },
    { XK_KP_Subtract, KEYSYM_KPMINUS },
    { XK_KP_Multiply, KEYSYM_KPMUL },
    { XK_KP_Divide, KEYSYM_KPDIV },
    { XK_KP_Separator, KEYSYM_KPDOT },
    { XK_KP_Up, KEYSYM_KPUP },
    { XK_KP_Down, KEYSYM_KPDOWN },
    { XK_KP_Left, KEYSYM_KPLEFT },
    { XK_KP_Right, KEYSYM_KPRIGHT },
    { XK_KP_Home, KEYSYM_KPHOME },
    { XK_KP_End, KEYSYM_KPEND },
    { XK_KP_Page_Up, KEYSYM_KPPAGEUP },
    { XK_KP_Page_Down, KEYSYM_KPPAGEDOWN },
    { XK_KP_Insert, KEYSYM_KPINSERT },
    { XK_KP_Delete, KEYSYM_KPDELETE },
};

static u32 vnc_keysym_to_vcml_keysym(u32 keysym) {
    const auto it = VNC_KEYSYMS.find(keysym);
    if (it != VNC_KEYSYMS.end())
        return it->second;
    return KEYSYM_NONE;
}

static void rfb_log_func(const char* format, ...) {
    va_list args;
    va_start(args, format);
    string str = vmkstr(format, args);
    va_end(args);
    log_debug("%s", trim(str).c_str());
}

static void rfb_err_func(const char* format, ...) {
    va_list args;
    va_start(args, format);
    string str = vmkstr(format, args);
    va_end(args);
    log_error("%s", trim(str).c_str());
}

static void rfb_key_func(rfbBool down, rfbKeySym sym, rfbClientPtr cl) {
    int port = cl->screen->port;
    auto disp = display::lookup(mkstr("vnc:%d", port));
    VCML_ERROR_ON(!disp, "no display found for port %d", port);
    auto vnc_server = dynamic_cast<vnc*>(disp.get());
    VCML_ERROR_ON(!vnc_server, "no vnc server found for port %d", port);
    vnc_server->key_event((u32)sym, (bool)down);
}

static void rfb_ptr_func(int mask, int x, int y, rfbClientPtr cl) {
    int port = cl->screen->port;
    auto disp = display::lookup(mkstr("vnc:%d", port));
    VCML_ERROR_ON(!disp, "no display found for port %d", port);
    auto vnc_server = dynamic_cast<vnc*>(disp.get());
    VCML_ERROR_ON(!vnc_server, "no vnc server found for port %d", port);
    vnc_server->ptr_event((u32)mask, (u32)x, (u32)y);
}

void vnc::run() {
    const videomode& fbm = mode();

    u32 samples = 0;
    if (fbm.a.size > 0)
        samples++;
    if (fbm.r.size > 0)
        samples++;
    if (fbm.g.size > 0)
        samples++;
    if (fbm.b.size > 0)
        samples++;

    m_screen = rfbGetScreen(nullptr, nullptr, fbm.xres, fbm.yres, fbm.r.size,
                            samples, fbm.bpp);

    m_screen->frameBuffer = (char*)framebuffer();
    m_screen->desktopName = name();
    m_screen->port = m_screen->ipv6port = m_port;
    m_screen->kbdAddEvent = &rfb_key_func;
    m_screen->ptrAddEvent = &rfb_ptr_func;

    rfbInitServer(m_screen);

    rfbNewFramebuffer(m_screen, m_screen->frameBuffer, fbm.xres, fbm.yres,
                      fbm.r.size, samples, fbm.bpp);

    m_screen->serverFormat.redShift = fbm.r.offset;
    m_screen->serverFormat.greenShift = fbm.g.offset;
    m_screen->serverFormat.blueShift = fbm.b.offset;

    m_screen->serverFormat.redMax = (1 << fbm.r.size) - 1;
    m_screen->serverFormat.greenMax = (1 << fbm.g.size) - 1;
    m_screen->serverFormat.blueMax = (1 << fbm.b.size) - 1;

    m_screen->serverFormat.bitsPerPixel = fbm.bpp * 8;
    m_screen->serverFormat.bigEndian = fbm.endian == ENDIAN_BIG;

    log_debug("starting vnc server on port %d", m_screen->port);

    while (m_running && rfbIsActive(m_screen) && sim_running())
        rfbProcessEvents(m_screen, 1000);

    log_debug("terminating vnc server on port %d", m_screen->port);

    rfbShutdownServer(m_screen, true);
    rfbScreenCleanup(m_screen);
}

vnc::vnc(u32 no):
    display("vnc", no),
    m_port(no), // vnc port = display number
    m_buttons(),
    m_ptr_x(),
    m_ptr_y(),
    m_running(false),
    m_mutex(),
    m_screen(),
    m_thread() {
    VCML_ERROR_ON(no != (u32)m_port, "invalid port specified: %u", no);

    rfbLog = &rfb_log_func;
    rfbErr = &rfb_err_func;
}

vnc::~vnc() {
    // nothing to do
}

void vnc::init(const videomode& mode, u8* fb) {
    display::init(mode, fb);

    m_running = true;
    m_thread = thread(&vnc::run, this);
    mwr::set_thread_name(m_thread, name());
}

void vnc::render(u32 x, u32 y, u32 w, u32 h) {
    if (x + w > xres())
        w = xres() - x;
    if (y + h > yres())
        h = yres() - y;

    rfbMarkRectAsModified(m_screen, x, y, x + w, y + h);
}

void vnc::render() {
    render(0, 0, xres(), yres());
}

void vnc::shutdown() {
    if (m_thread.joinable()) {
        m_running = false;
        m_thread.join();
    }

    display::shutdown();
}

void vnc::key_event(u32 sym, bool down) {
    u32 symbol = vnc_keysym_to_vcml_keysym(sym);
    if (symbol != KEYSYM_NONE)
        notify_key(symbol, down);
}

enum vnc_buttons : u32 {
    VNC_BTN_LEFT = 1 << 0,
    VNC_BTN_MIDDLE = 1 << 1,
    VNC_BTN_RIGHT = 1 << 2,
};

void vnc::ptr_event(u32 mask, u32 x, u32 y) {
    u32 change = mask ^ m_buttons;
    if (change & VNC_BTN_LEFT)
        notify_btn(BUTTON_LEFT, mask & VNC_BTN_LEFT);
    if (change & VNC_BTN_MIDDLE)
        notify_btn(BUTTON_MIDDLE, mask & VNC_BTN_MIDDLE);
    if (change & VNC_BTN_RIGHT)
        notify_btn(BUTTON_RIGHT, mask & VNC_BTN_RIGHT);
    m_buttons = mask;

    if (m_ptr_x != x || m_ptr_y != y) {
        display::notify_pos(x, y);
        m_ptr_x = x;
        m_ptr_y = y;
    }
}

display* vnc::create(u32 nr) {
    return new vnc(nr);
}

} // namespace ui
} // namespace vcml
