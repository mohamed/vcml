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

#ifndef VCML_GENERIC_BUS
#define VCML_GENERIC_BUS

#include "vcml/core/types.h"
#include "vcml/core/range.h"
#include "vcml/core/systemc.h"
#include "vcml/core/component.h"

#include "vcml/protocols/tlm.h"

namespace vcml {
namespace generic {

class bus : public component
{
private:
    enum : size_t {
        TARGET_NONE = SIZE_MAX,
        SOURCE_ANY = SIZE_MAX,
    };

    struct mapping {
        size_t target;
        size_t source;
        range addr;
        u64 offset;

        bool operator<(const mapping& m) const;
    };

    std::map<size_t, sc_object*> m_target_peers;
    std::map<size_t, sc_object*> m_source_peers;

    size_t find_target_port(sc_object& peer) const;
    size_t find_source_port(sc_object& peer) const;

    const char* target_peer_name(size_t port) const;
    const char* source_peer_name(size_t port) const;

    set<mapping> m_mappings;
    mapping m_default;

    const mapping& lookup(tlm_target_socket& src, const range& addr) const;

    bool cmd_mmap(const vector<string>& args, ostream& os);

protected:
    virtual void b_transport(tlm_target_socket& origin,
                             tlm_generic_payload& tx, sc_time& dt) override;

    virtual unsigned int transport_dbg(tlm_target_socket& origin,
                                       tlm_generic_payload& tx) override;

    virtual bool get_direct_mem_ptr(tlm_target_socket& origin,
                                    tlm_generic_payload& tx,
                                    tlm_dmi& dmi) override;

    virtual void invalidate_direct_mem_ptr(tlm_initiator_socket& origin,
                                           u64 start, u64 end) override;

public:
    tlm_target_socket_array<> in;
    tlm_initiator_socket_array<> out;

    void map(size_t target, const range& addr);
    void map(size_t target, const range& addr, u64 offset);
    void map(size_t target, const range& addr, u64 offset, size_t source);

    void map(size_t target, u64 lo, u64 hi);
    void map(size_t target, u64 lo, u64 hi, u64 offset);
    void map(size_t target, u64 lo, u64 hi, u64 offset, size_t source);

    void map_default(size_t target, u64 offset = 0);

    template <typename SOURCE>
    size_t bind(SOURCE& s);

    template <typename TARGET>
    size_t bind(TARGET& s, const range& addr, u64 offset = 0);
    template <typename TARGET>
    size_t bind(TARGET& s, u64 lo, u64 hi, u64 offset = 0);
    template <typename SOURCE, typename TARGET>
    size_t bind(SOURCE& s, TARGET& d, const range& addr, u64 offset = 0);
    template <typename SOURCE, typename TARGET>
    size_t bind(SOURCE& source, TARGET& d, u64 lo, u64 hi, u64 offset = 0);

    template <typename SOCKET>
    size_t bind_default(SOCKET& s, u64 offset = 0);

    bus() = delete;
    bus(const sc_module_name& nm);
    virtual ~bus();
    VCML_KIND(bus);
};

inline void bus::map(size_t target, const range& addr) {
    map(target, addr, 0);
}

inline void bus::map(size_t target, const range& addr, u64 offset) {
    map(target, addr, offset, SOURCE_ANY);
}

inline void bus::map(size_t target, u64 lo, u64 hi) {
    map(target, range(lo, hi));
}

inline void bus::map(size_t target, u64 lo, u64 hi, u64 offset) {
    map(target, range(lo, hi), offset);
}

inline void bus::map(size_t target, u64 lo, u64 hi, u64 offset, size_t src) {
    map(target, range(lo, hi), offset, src);
}

template <typename SOURCE>
size_t bus::bind(SOURCE& source) {
    size_t port = find_source_port(source);
    if (port == TARGET_NONE) {
        port = in.next_index();
        in[port].bind(source);
        m_source_peers[port] = &source;
    }

    return port;
}

template <typename TARGET>
size_t bus::bind(TARGET& target, const range& addr, u64 offset) {
    size_t port = find_target_port(target);
    if (port == TARGET_NONE) {
        port = out.next_index();
        out[port].bind(target);
        m_target_peers[port] = &target;
    }

    map(port, addr, offset);
    return port;
}

template <typename TARGET>
size_t bus::bind(TARGET& target, u64 lo, u64 hi, u64 offset) {
    return bind(target, range(lo, hi), offset);
}

template <typename SOURCE, typename TARGET>
size_t bus::bind(SOURCE& source, TARGET& target, const range& addr, u64 off) {
    size_t source_port = bind(source);
    size_t target_port = find_target_port(target);
    if (target_port == TARGET_NONE) {
        target_port = out.next_index();
        out[target_port].bind(target);
        m_target_peers[target_port] = &target;
    }

    map(target_port, addr, off, source_port);
    return target_port;
}

template <typename SOURCE, typename TARGET>
size_t bus::bind(SOURCE& source, TARGET& target, u64 lo, u64 hi, u64 offset) {
    return bind(source, target, range(lo, hi), offset);
}

template <typename TARGET>
size_t bus::bind_default(TARGET& target, u64 offset) {
    size_t port = out.next_index();
    map_default(port, offset);
    out[port].bind(target);
    m_target_peers[port] = &target;
    return port;
}

} // namespace generic
} // namespace vcml

#endif
