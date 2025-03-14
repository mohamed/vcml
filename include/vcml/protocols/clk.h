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

#ifndef VCML_PROTOCOLS_CLK_H
#define VCML_PROTOCOLS_CLK_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"

#include "vcml/protocols/base.h"

namespace vcml {

struct clk_payload {
    clock_t oldhz;
    clock_t newhz;
};

constexpr bool success(const clk_payload& tx) {
    return true;
}

constexpr bool failed(const clk_payload& tx) {
    return false;
}

ostream& operator<<(ostream& os, const clk_payload& clk);

class clk_fw_transport_if : public sc_core::sc_interface
{
public:
    typedef clk_payload protocol_types;
    virtual void clk_transport(const clk_payload& tx) = 0;
};

class clk_bw_transport_if : public sc_core::sc_interface
{
public:
    typedef clk_payload protocol_types;
    virtual clock_t clk_get_hz() = 0;
};

class clk_base_initiator_socket;
class clk_base_target_socket;
class clk_initiator_socket;
class clk_target_socket;
class clk_initiator_stub;
class clk_target_stub;

class clk_host
{
public:
    friend class clk_initiator_socket;
    friend class clk_target_socket;

    typedef vector<clk_initiator_socket*> clk_initiator_sockets;
    typedef vector<clk_target_socket*> clk_target_sockets;

    const clk_initiator_sockets& all_clk_initiator_sockets() const;
    const clk_target_sockets& all_clk_target_sockets() const;

    clk_target_sockets all_clk_target_sockets(address_space as) const;

    clk_host() = default;

    virtual ~clk_host() = default;

    virtual void clk_notify(const clk_target_socket&, const clk_payload&) = 0;

private:
    clk_initiator_sockets m_initiator_sockets;
    clk_target_sockets m_target_sockets;
};

inline const clk_host::clk_initiator_sockets&
clk_host::all_clk_initiator_sockets() const {
    return m_initiator_sockets;
}

inline const clk_host::clk_target_sockets& clk_host::all_clk_target_sockets()
    const {
    return m_target_sockets;
}

typedef multi_initiator_socket<clk_fw_transport_if, clk_bw_transport_if>
    clk_base_initiator_socket_b;

typedef multi_target_socket<clk_fw_transport_if, clk_bw_transport_if>
    clk_base_target_socket_b;

class clk_base_initiator_socket : public clk_base_initiator_socket_b
{
private:
    clk_target_stub* m_stub;

public:
    clk_base_initiator_socket(const char*, address_space = VCML_AS_DEFAULT);
    virtual ~clk_base_initiator_socket();
    VCML_KIND(clk_base_initiator_socket);

    using clk_base_initiator_socket_b::bind;
    virtual void bind(clk_base_target_socket& socket);

    bool is_stubbed() const { return m_stub != nullptr; }
    void stub();
};

class clk_base_target_socket : public clk_base_target_socket_b
{
private:
    clk_initiator_stub* m_stub;

public:
    clk_base_target_socket(const char*, address_space = VCML_AS_DEFAULT);
    virtual ~clk_base_target_socket();
    VCML_KIND(clk_base_target_socket);

    using clk_base_target_socket_b::bind;
    virtual void bind(clk_base_initiator_socket& other);
    virtual void complete_binding(clk_base_initiator_socket& socket) {}

    bool is_stubbed() const { return m_stub != nullptr; }
    void stub(clock_t hz = 100 * MHz);
};

template <const size_t MAX_PORTS = SIZE_MAX>
using clk_base_initiator_socket_array = socket_array<clk_base_initiator_socket,
                                                     MAX_PORTS>;

template <const size_t MAX_PORTS = SIZE_MAX>
using clk_base_target_socket_array = socket_array<clk_base_target_socket,
                                                  MAX_PORTS>;

class clk_initiator_socket : public clk_base_initiator_socket
{
public:
    clk_initiator_socket(const char* nm, address_space as = VCML_AS_DEFAULT);
    virtual ~clk_initiator_socket();
    VCML_KIND(clk_initiator_socket);

    clock_t get() const { return m_hz; }
    void set(clock_t hz);

    operator clock_t() const { return get(); }
    clk_initiator_socket& operator=(clock_t hz);

    sc_time cycle() const;
    sc_time cycles(size_t n) const { return cycle() * n; }

private:
    clk_host* m_host;
    clock_t m_hz;

    struct clk_bw_transport : public clk_bw_transport_if {
        clk_initiator_socket* socket;
        clk_bw_transport(clk_initiator_socket* s):
            clk_bw_transport_if(), socket(s) {}
        clock_t clk_get_hz() override { return socket->get(); }
    } m_transport;

    void clk_transport(const clk_payload& tx);
};

inline sc_time clk_initiator_socket::cycle() const {
    if (m_hz == 0)
        return SC_ZERO_TIME;
    return sc_time(1.0 / m_hz, SC_SEC);
}

class clk_target_socket : public clk_base_target_socket
{
public:
    clk_target_socket(const char* nm, address_space as = VCML_AS_DEFAULT);
    virtual ~clk_target_socket();
    VCML_KIND(clk_target_socket);

    using clk_base_target_socket::bind;
    virtual void bind(clk_base_target_socket& other);
    virtual void complete_binding(clk_base_initiator_socket& socket) override;

    clock_t read() const;
    operator clock_t() const { return read(); }

    sc_time cycle() const;
    sc_time cycles(size_t n) const { return cycle() * n; }

private:
    clk_host* m_host;

    struct clk_fw_transport : public clk_fw_transport_if {
        clk_target_socket* socket;
        clk_fw_transport(clk_target_socket* s):
            clk_fw_transport_if(), socket(s) {}

        virtual void clk_transport(const clk_payload& tx) override {
            socket->clk_transport(tx);
        }
    } m_transport;

    clk_base_initiator_socket* m_initiator;
    vector<clk_base_target_socket*> m_targets;

    void clk_transport(const clk_payload& tx);
};

template <const size_t MAX_PORTS = SIZE_MAX>
using clk_initiator_socket_array = socket_array<clk_initiator_socket,
                                                MAX_PORTS>;

template <const size_t MAX_PORTS = SIZE_MAX>
using clk_target_socket_array = socket_array<clk_target_socket, MAX_PORTS>;

class clk_initiator_stub : private clk_bw_transport_if
{
private:
    clock_t m_hz;

    virtual clock_t clk_get_hz() override { return m_hz; }

public:
    clk_base_initiator_socket clk_out;
    clk_initiator_stub(const char* nm, clock_t hz);
    virtual ~clk_initiator_stub() = default;
};

class clk_target_stub : private clk_fw_transport_if
{
private:
    virtual void clk_transport(const clk_payload& tx) override;

public:
    clk_base_target_socket clk_in;
    clk_target_stub(const char* nm);
    virtual ~clk_target_stub() = default;
};

} // namespace vcml

#endif
