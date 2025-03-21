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

#ifndef VCML_DEBUGGING_TARGET_H
#define VCML_DEBUGGING_TARGET_H

#include "vcml/core/types.h"
#include "vcml/core/thctl.h"
#include "vcml/core/range.h"
#include "vcml/core/systemc.h"

#include "vcml/logging/logger.h"
#include "vcml/debugging/symtab.h"
#include "vcml/debugging/subscriber.h"
#include "vcml/debugging/suspender.h"

namespace vcml {
namespace debugging {

class target;

struct cpureg {
    u64 regno;
    string name;
    u64 size;
    int prot;
    target* host;

    cpureg(const cpureg&) = default;

    cpureg(): regno(~0ul), name(), size(), prot(), host() {}

    cpureg(u64 no, const string& nm, u64 sz, int p):
        regno(no), name(nm), size(sz), prot(p), host() {}

    u64 width() const { return size * 8; }

    bool is_readable() const { return is_read_allowed(prot); }
    bool is_writeable() const { return is_write_allowed(prot); }
    bool is_read_only() const { return prot == VCML_ACCESS_READ; }
    bool is_write_only() const { return prot == VCML_ACCESS_WRITE; }
    bool is_read_write() const { return prot == VCML_ACCESS_READ_WRITE; }

    u64 read() const;
    void write(u64 val) const;
};

struct disassembly {
    string code;
    u8 insn[16];
    u64 addr;
    u64 size;
    const symbol* sym;
};

struct stackframe {
    u64 program_counter;
    u64 frame_pointer;
    const symbol* sym;
};

class target
{
private:
    mutable mutex m_mtx;

    string m_name;

    endianess m_endian;
    unordered_map<u64, cpureg> m_cpuregs;
    symtab m_symbols;

    vector<subscriber*> m_steppers;

    vector<breakpoint*> m_breakpoints;
    vector<watchpoint*> m_watchpoints;

    static unordered_map<string, target*> s_targets;

protected:
    virtual void define_cpuregs(const vector<cpureg>& regs);

    virtual bool insert_breakpoint(u64 addr);
    virtual bool remove_breakpoint(u64 addr);

    virtual bool insert_watchpoint(const range& addr, vcml_access prot);
    virtual bool remove_watchpoint(const range& addr, vcml_access prot);

    void notify_breakpoint_hit(u64 addr);
    void notify_watchpoint_read(const range& addr);
    void notify_watchpoint_write(const range& addr, u64 newval);

public:
    void set_little_endian();
    void set_big_endian();

    bool is_little_endian() const;
    bool is_big_endian() const;
    bool is_host_endian() const;

    const symtab& symbols() const;
    u64 load_symbols_from_elf(const string& file);

    const char* target_name() const { return m_name.c_str(); }

    target();
    virtual ~target();

    vector<cpureg> cpuregs() const;

    const cpureg* find_cpureg(u64 regno) const;
    const cpureg* find_cpureg(const string& name) const;

    virtual bool read_cpureg_dbg(const cpureg& reg, u64& val);
    virtual bool write_cpureg_dbg(const cpureg& reg, u64 val);

    virtual u64 read_pmem_dbg(u64 addr, void* buffer, u64 size);
    virtual u64 write_pmem_dbg(u64 addr, const void* buffer, u64 size);

    virtual u64 read_vmem_dbg(u64 addr, void* buffer, u64 size);
    virtual u64 write_vmem_dbg(u64 addr, const void* buffer, u64 size);

    virtual bool page_size(u64& size);
    virtual bool virt_to_phys(u64 vaddr, u64& paddr);

    virtual const char* arch();

    virtual u64 core_id();
    virtual u64 program_counter();
    virtual u64 link_register();
    virtual u64 stack_pointer();
    virtual u64 frame_pointer();

    virtual void stacktrace(vector<stackframe>& trace, size_t limit = 64);

    virtual bool disassemble(u8* ibuf, u64& addr, string& code);
    virtual bool disassemble(u64 addr, u64 count, vector<disassembly>& s);
    virtual bool disassemble(const range& addr, vector<disassembly>& s);

    const vector<breakpoint*>& breakpoints() const;
    const vector<watchpoint*>& watchpoints() const;

    const breakpoint* lookup_breakpoint(u64 addr);
    const breakpoint* insert_breakpoint(u64 addr, subscriber* subscr);
    bool remove_breakpoint(const breakpoint* bp, subscriber* subscr);
    bool remove_breakpoint(u64 addr, subscriber* subscr);

    bool insert_watchpoint(const range& mem, vcml_access a, subscriber* s);
    bool remove_watchpoint(const range& mem, vcml_access a, subscriber* s);

    bool is_stepping() const;
    void request_singlestep(subscriber* subscr);
    void notify_singlestep();

    static vector<target*> all();
    static target* find(const string& name);
};

inline void target::set_little_endian() {
    m_endian = ENDIAN_LITTLE;
}

inline void target::set_big_endian() {
    m_endian = ENDIAN_BIG;
}

inline bool target::is_little_endian() const {
    return m_endian == ENDIAN_LITTLE;
}

inline bool target::is_big_endian() const {
    return m_endian == ENDIAN_BIG;
}

inline bool target::is_host_endian() const {
    return m_endian == host_endian();
}

inline const symtab& target::symbols() const {
    return m_symbols;
}

inline u64 target::load_symbols_from_elf(const string& file) {
    return m_symbols.load_elf(file);
}

inline const vector<breakpoint*>& target::breakpoints() const {
    return m_breakpoints;
}

inline const vector<watchpoint*>& target::watchpoints() const {
    return m_watchpoints;
}

inline bool target::is_stepping() const {
    lock_guard<mutex> guard(m_mtx);
    return !m_steppers.empty();
}

inline void target::request_singlestep(subscriber* subscr) {
    lock_guard<mutex> guard(m_mtx);
    stl_add_unique(m_steppers, subscr);
}

} // namespace debugging
} // namespace vcml

#endif
