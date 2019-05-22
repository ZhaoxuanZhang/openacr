// (C) 2017-2019 NYSE | Intercontinental Exchange
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contacting ICE: <https://www.theice.com/contact>
//
// Target: abt (exe) -- Algo Build Tool (build system)
// Exceptions: NO
// Header: include/abt.h -- Main header
//
// Created By: alexei.lebedev
// Recent Changes: alexei.lebedev
//

#include "include/gen/abt_gen.h"
#include "include/gen/abt_gen.inl.h"

namespace abt { // update-hdr
    // Dear human:
    //     Text from here to the closing curly brace was produced by scanning
    //     source files. Editing this text is futile.
    //     To refresh the contents of this section, run 'update-hdr'.
    //     To convert this section to a hand-written section, remove the word 'update-hdr' from namespace line.

    // -------------------------------------------------------------------
    // cpp/abt/build.cpp -- Build dag execution
    //
    void Main_Build();

    // -------------------------------------------------------------------
    // cpp/abt/disas.cpp -- Disassemble
    //
    void Main_Disas();

    // -------------------------------------------------------------------
    // cpp/abt/main.cpp -- Algo Build Tool - Main file
    //
    bool HeaderExtQ(strptr ext);

    // how are we using this execkey???
    i64 execkey_Get(abt::FSyscmd &cmd);
    abt::FSyscmd& NewCmd(abt::FSyscmd *start, abt::FSyscmd *end);

    // compute obj key by replace path components
    // with .
    // So, cpp/abt/main.cpp becomes cpp.abt.main.cpp
    // Next step will be to replace the extension
    tempstr GetObjkey(strptr source);

    // Return true if this file is a precompiled header file
    bool PchQ(abt::FSrcfile &srcfile);

    // Return a canonic name describing current configuration
    // This is not the actual build directory. That one is called "out_dir".
    // Builddir is just the primary key of a build directory
    tempstr GetBuilddir();
    void Main();

    // -------------------------------------------------------------------
    // cpp/abt/opt.cpp -- Calculate compiler options
    //

    // check if given opt value is in the set
    bool Matches(abt::FToolOpt& opt);
    tempstr EvalSrcfileOpts(abt::FTarget &target, abt::FSrcfile &srcfile);
    tempstr EvalLinkCmdline(abt::FTarget &target);
}
