//
// Created by wenzel on 28.05.20.
//

#ifndef YOSYS_FIRRTL_H
#define YOSYS_FIRRTL_H

#include <kernel/register.h>

namespace Yosys::Firrtl {
    const char *make_id(IdString id);

    struct FirrtlWorker {
        Module *module;
        std::ostream &f;

        dict<SigBit, pair<string, int>> reverse_wire_map;
        string unconn_id;
        RTLIL::Design *design;
        std::string indent;

        // Define read/write ports and memories.
        // We'll collect their definitions and emit the corresponding FIRRTL definitions at the appropriate point in module construction.
        // For the moment, we don't handle $readmemh or $readmemb.
        // These will be part of a subsequent PR.
        struct read_port {
            string name;
            bool clk_enable;
            bool clk_parity;
            bool transparent;
            RTLIL::SigSpec clk;
            RTLIL::SigSpec ena;
            RTLIL::SigSpec addr;

            read_port(string name, bool clk_enable, bool clk_parity, bool transparent, RTLIL::SigSpec clk,
                      RTLIL::SigSpec ena, RTLIL::SigSpec addr);

            string gen_read(const char *indent);
        };

        struct write_port : read_port {
            RTLIL::SigSpec mask;

            write_port(string name, bool clk_enable, bool clk_parity, bool transparent, RTLIL::SigSpec clk,
                       RTLIL::SigSpec ena, RTLIL::SigSpec addr, RTLIL::SigSpec mask);

            string gen_read(const char * /* indent */);

            string gen_write(const char *indent);
        };

        /* Memories defined within this module. */
        struct memory {
            Cell *pCell;                    // for error reporting
            string name;                    // memory name
            int abits;                        // number of address bits
            int size;                        // size (in units) of the memory
            int width;                        // size (in bits) of each element
            int read_latency;
            int write_latency;
            vector<read_port> read_ports;
            vector<write_port> write_ports;
            std::string init_file;
            std::string init_file_srcFileSpec;
            string srcLine;

            memory(Cell *pCell, string name, int abits, int size, int width);

            // We need a default constructor for the dict insert.
            memory();

            const char *atLine();

            void add_memory_read_port(read_port &rp);

            void add_memory_write_port(write_port &wp);

            void add_memory_file(std::string init_file, std::string init_file_srcFileSpec);

        };

        dict<string, memory> memories;

        void register_memory(memory &m);

        void register_reverse_wire_map(string id, SigSpec sig);

        FirrtlWorker(Module *module, std::ostream &f, RTLIL::Design *theDesign);

        static string make_expr(const SigSpec &sig);

        std::string fid(RTLIL::IdString internal_id);

        std::string cellname(RTLIL::Cell *cell);

        void process_instance(RTLIL::Cell *cell, vector<string> &wire_exprs);

        // Given an expression for a shift amount, and a maximum width,
        //  generate the FIRRTL expression for equivalent dynamic shift taking into account FIRRTL shift semantics.
        std::string gen_dshl(const string b_expr, const int b_width);

        void run();
    };

}
#endif //YOSYS_FIRRTL_H
