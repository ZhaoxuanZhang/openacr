// (C) AlgoEngineering LLC 2008-2013
// (C) 2013-2019 NYSE | Intercontinental Exchange
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
// Source: cpp/abt/main.cpp -- Algo Build Tool - Main file
//
// Created By: alexei.lebedev mark.page
// Authors: alexei.lebedev
// Recent Changes: alexei.lebedev
//
// TODO Create entries for object files and add them as input dependencies to link step.
// Currently, the out-of-date flag propagation is unconvincing

#include <sys/wait.h>
#include <sys/utsname.h>
#include <unistd.h>
#include "include/abt.h"

// -----------------------------------------------------------------------------

bool abt::HeaderExtQ(strptr ext) {
    return ext == ".h" || ext == ".hpp";
}

// -----------------------------------------------------------------------------

static abt::FFilestat &Filestat(strptr fname) {
    abt::FFilestat *filestat=abt::ind_filestat_Find(fname);
    if (!filestat) {
        filestat =&abt::filestat_Alloc();
        vrfy(filestat, "abt.filestat_out_of_mem");
        filestat->filename=fname;
        (void)filestat_XrefMaybe(*filestat);
        struct stat S;
        if (0==stat(Zeroterm(filestat->filename),&S)) {
            filestat->modtime = ToUnTime(UnixTime(S.st_mtime));
            filestat->size = S.st_size;
            filestat->isdir = S_ISDIR(S.st_mode);
            filestat->exists = true;
        }
    }
    return *filestat;
}

// -----------------------------------------------------------------------------

// how are we using this execkey???
i64 abt::execkey_Get(abt::FSyscmd &cmd) {
    return (i64(cmd.nprereq) << 32) | (i64(cmd.syscmd) & 0xffffffff);
}

// -----------------------------------------------------------------------------

static bool SourceQ(abt::FTargsrc &targsrc) {
    return ext_Get(targsrc)=="cpp"
        || ext_Get(targsrc)=="c"
        || targsrc.p_target->p_ns->nstype == dmmeta_Nstype_nstype_pch;
}

// -----------------------------------------------------------------------------

abt::FSyscmd& abt::NewCmd(abt::FSyscmd *start, abt::FSyscmd *end) {
    abt::FSyscmd *cmd = &abt::ind_syscmd_GetOrCreate(abt::syscmd_N());
    if (start) {
        abt::syscmddep_InsertMaybe(dev::Syscmddep(cmd->rowid, start->rowid));//child,parent
    }
    if (end) {
        abt::syscmddep_InsertMaybe(dev::Syscmddep(end->rowid, cmd->rowid));
    }
    return *cmd;
}

// -----------------------------------------------------------------------------

static void ComputeCumMod(abt::FSrcfile &srcfile) {
    if (!srcfile.cum_mod_visited) {
        srcfile.cum_mod_visited = true;
        UnTime hdrtime;

        ind_beg(abt::srcfile_zd_include_curs, include, srcfile) {
            ComputeCumMod(*include.p_header);
            hdrtime.value = i64_Max(hdrtime.value, include.p_header->cum_modtime.value);
        }ind_end;

        srcfile.cum_modtime.value = i64_Max(srcfile.mod_time.value, hdrtime.value);
    }
}

static void ComputeOod(abt::FTarget &target) {
    if (!target.ood_visited) {
        target.ood_visited = true;
        ind_beg(abt::target_c_targdep_curs, targdep, target) {
            ComputeOod(*targdep.p_parent);
            target.ood |= targdep.p_parent->ood;
        }ind_end;
    }
}

// -----------------------------------------------------------------------------

static void Main_ComputeAlldep(abt::FTarget &target, abt::FTarget &parent) {
    if (c_alldep_ScanInsertMaybe(target,parent)) {
        ind_beg(abt::target_c_targdep_curs, targdep, parent) {
            Main_ComputeAlldep(target,*targdep.p_parent);
        }ind_end;
    }
}

// -----------------------------------------------------------------------------

static void Main_ComputeAlllib() {// transitively collect all libraries for target
    ind_beg(abt::_db_target_curs, target, abt::_db) {
        ind_beg(abt::target_c_alldep_curs, dep, target) {
            ind_beg(abt::target_c_targsyslib_curs, targsyslib, dep) {
                if (Regx_Match(targsyslib.uname, abt::_db.cmdline.uname)) {
                    c_alllib_ScanInsertMaybe(target,*targsyslib.p_syslib);
                }
            }ind_end;
        }ind_end;
    }ind_end;
}

// -----------------------------------------------------------------------------

static void Main_SelectTarget() {
    // create list of targets to consider:
    // either all targets, or one target as specified on command line
    ind_beg(abt::_db_target_curs, target,abt::_db) if (Regx_Match(abt::_db.cmdline.target, target.target)) {
        abt::zs_origsel_target_Insert(target);
        abt::zs_sel_target_Insert(target);
    }ind_end;

    // recursively select target dependencies; the list zs_sel_target grows as this scanning proceeds.
    for(abt::FTarget *target = abt::zs_sel_target_First(); target; target=target->zs_sel_target_next) {
        ind_beg(abt::target_c_targdep_curs, dep,*target) {
            abt::zs_sel_target_Insert(*dep.p_parent);// modifies NEXT pointer possibly!
        }ind_end;
    }

    // Check if no match occured
    if (ch_N(abt::_db.cmdline.target.expr) && abt::zs_sel_target_N() == 0) {
        prerr("abt.nomatch"
              <<Keyval("target", abt::_db.cmdline.target)
              <<Keyval("comment","No target matches specified pattern"));
        abt::_db.report.n_err++;
    }
}

// -----------------------------------------------------------------------------

static abt::FInclude *FindFile(abt::FSrcfile &srcfile, strptr incl, bool is_sys) {
    strptr dir = GetDirName(srcfile.srcfile);
    tempstr fname(incl);
    bool existed = abt::ind_srcfile_Find(incl);
    bool found = existed || Filestat(incl).exists;

    if (!found) {// try path relative to include point directory
        fname = tempstr() << dir << MaybeDirSep << incl;
        found = Filestat(fname).exists;
    }
    if (!found && is_sys) {
        ind_beg(abt::_db_sysincl_curs,sysincl,abt::_db) {
            fname = tempstr() << sysincl << MaybeDirSep << incl;
            found = Filestat(fname).exists;
            if (found) {
                break;
            }
        }ind_end;
    }
    tempstr key =tempstr()<<srcfile.srcfile<<":"<<fname;
    abt::FInclude *include = abt::ind_include_Find(key);// include -- relation on two srcfiles
    if (found) {
        abt::ind_srcfile_GetOrCreate(fname); // create missing srcfile
        include          = &abt::include_Alloc();
        include->include = key;
        include->sys     = is_sys;
        include_XrefMaybe(*include);
    }
    return include;
}

// -----------------------------------------------------------------------------

static void Main_ComputeSysincl() {
    bool success=false;
    if (abt::_db.cmdline.printcmd) {
        success=true;
        // do nothing -- system includes won't be used
    } else {
        // this it he command line that returns the list
        // of system include paths on this machine
        tempstr cmd;
        cmd << abt::_db.cmdline.compiler << " -x c++ -E -v - </dev/null 2>&1";
        tempstr text(SysEval(cmd,FailokQ(true),1024*1024));
        // parse it for the paths...
        bool inside = false;
        ind_beg(Line_curs,line,text) {
            if (StartsWithQ(line, "#include <...>")) {
                inside = true;
            }
            if (StartsWithQ(line, "End of search list")) {
                inside = false;
                success = true;
            }
            strptr path = Trimmed(line);
            if (inside && Filestat(path).isdir) {
                abt::sysincl_Alloc() = Trimmed(path);
            }
        }ind_end;
    }
    vrfy(success,
         tempstr("abt.sysincl_warning")
         <<Keyval("compiler",abt::_db.cmdline.compiler)
         <<Keyval("comment", "cannot compute list of system include paths"));
}

// -----------------------------------------------------------------------------

static void line_n_Update(abt::FSrcfile *srcfile) {
    if (srcfile->line_n == 0) {
        MmapFile file;
        MmapFile_Load(file,srcfile->srcfile);
        ind_beg(Line_curs, curs, file.text) {
            (void)curs;
            srcfile->line_n++;
        }ind_end;
    }
}

// -----------------------------------------------------------------------------

static void ScanHeaders(abt::FSrcfile *srcfile) {
    MmapFile file;
    MmapFile_Load(file,srcfile->srcfile);
    u64 bytes_not_scanned = 0;
    u64 bytes_total = file.text.n_elems;
    ind_beg(Line_curs, curs, file.text) {
        StringIter line(curs);
        if (SkipChar(line, '#')) {
            bool is_include = SkipStrptr(line.Ws(), "include");
            bool is_quote = SkipChar(line.Ws(), '\"');
            bool is_sys   = SkipChar(line.Ws(), '<');
            is_include = is_include && (is_quote || is_sys);
            if (is_include && !is_sys) {
                strptr incl(GetTokenChar(line, is_quote ? '\"' : '>'));
                int lineno = ind_curs(curs).i + 1;
                abt::FInclude *include = FindFile(*srcfile,incl,is_sys);
                if (!include && !is_sys) {
                    prerr("abt.badinclude"
                          <<Keyval("location", tempstr()<<srcfile->srcfile<<":"<<lineno)
                          <<Keyval("include",incl)
                          <<Keyval("comment","Cannot locate include file"));
                    algo_lib::_db.exit_code++;
                }
            }
        } else if (SkipStrptr(line, "//#pragma endinclude")) {
            bytes_not_scanned += file.text.n_elems - (curs.elems-file.text.elems);
            // take a hint that no further includes will occur in this file
            // (saves a bunch of page faults when scanning)
            // this can only be done in headers because otherwise line count is off
            break;
        }
    }ind_end;
    verblog("abt.scanfile"
            <<Keyval("srcfile",srcfile->srcfile)
            <<Keyval("bytes_n", bytes_total)
            <<Keyval("bytes_not_scanned", bytes_not_scanned));
}

// -----------------------------------------------------------------------------

// compute obj key by replace path components
// with .
// So, cpp/abt/main.cpp becomes cpp.abt.main.cpp
// Next step will be to replace the extension
tempstr abt::GetObjkey(strptr source) {
    tempstr ret(source);
    Translate(ret, "/", ".");
    return ret;
}

// -----------------------------------------------------------------------------

// Return true if this file is a precompiled header file
bool abt::PchQ(abt::FSrcfile &srcfile) {
    return srcfile.p_target && srcfile.p_target->p_ns->nstype == dmmeta_Nstype_nstype_pch;
}

// -----------------------------------------------------------------------------

static void ComputeObjpath(abt::FSrcfile &srcfile) {
    srcfile.objkey = abt::GetObjkey(srcfile.srcfile);
    // Replace extension
    strptr tgt_ext;
    strptr ext = GetFileExt(srcfile.srcfile);
    if (ext == ".cpp" || ext == ".c") {
        tgt_ext = ".o";
    } else if (ext == ".rc") {
        tgt_ext = ".res";
    } else if (abt::PchQ(srcfile)) {
        // normally we don't compile headers; but
        // we do compile pch headers
        tgt_ext = ".gch";
    }
    if (ch_N(tgt_ext)) {
        srcfile.objpath << abt::_db.cmdline.out_dir << MaybeDirSep;
        if (!abt::HeaderExtQ(ext)) {
            srcfile.objpath << StripExt(srcfile.objkey);
        } else {
            srcfile.objpath << srcfile.objkey;
        }
        srcfile.objpath << tgt_ext;
    }
}

// -----------------------------------------------------------------------------

// This function always scans the entire graph of all sources
static void Main_ReadSrcfile() {
    // walk over target sources; each file corresponds to a single file; create Srcfile records
    ind_beg(abt::_db_targsrc_curs, targsrc, abt::_db) if (SourceQ(targsrc)) {
        tempstr pathname(src_Get(targsrc));
        vrfy(Filestat(pathname).exists, tempstr()
             <<Keyval("src",src_Get(targsrc))
             <<Keyval("comment","file not found"));
        abt::FSrcfile& src_rec = abt::ind_srcfile_GetOrCreate(pathname);
        src_rec.p_target = targsrc.p_target;
        c_srcfile_Insert(*targsrc.p_target, src_rec);
    }ind_end;

    // Recursively read headers for each srcfile: these also become Srcfiles but they are
    // not connected to the target.
    // compute mod time for each srcfile
    while(abt::FSrcfile *srcfile = abt::zs_srcfile_read_First()) {
        ComputeObjpath(*srcfile);
        // compute mod time
        srcfile->mod_time = Filestat(srcfile->srcfile).modtime;

        // read file, scan includes
        ScanHeaders(srcfile);
        abt::zs_srcfile_read_RemoveFirst();
    }
    if (algo_lib::_db.exit_code > 0) {
        _exit(1); // exit right away. nothing smarter can be done. TODO: something smarter.
    }
}

// -----------------------------------------------------------------------------

static void Main_Clean() {
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        DeleteFile(target.outfile);
        ind_beg(abt::target_c_srcfile_curs, src,target) if (ch_N(src.objpath)) {
            DeleteFile(src.objpath);
            // clean coverage files
            tempstr gcdafile(ReplaceExt(src.objpath, ".gcda"));
            tempstr gcnofile(ReplaceExt(src.objpath, ".gcno"));
            DeleteFile(gcdafile);
            DeleteFile(gcnofile);
        }ind_end;
    }ind_end;
}

// -----------------------------------------------------------------------------

static void Main_List() {
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        bool list_targ = true;
        if (abt::_db.cmdline.ood) { // filter by out-of-date
            list_targ &= target.ood;
        }
        if (list_targ) {
            dev::Target out;
            target_CopyOut(target,out);
            prlog(out);
        }
        ind_beg(abt::target_c_srcfile_curs, src,target) {
            bool list_src = true;
            if (abt::_db.cmdline.ood) {
                list_src      &= src.ood;
            }
            if (list_src) {
                dev::Srcfile out;
                srcfile_CopyOut(src,out);
                prlog(out);
            }
        }ind_end;
    }ind_end;
}

// -----------------------------------------------------------------------------

static void ListIncl(abt::FSrcfile &srcfile) {
    if (!srcfile.printed) {
        srcfile.printed=true;
        ind_beg(abt::srcfile_zd_include_curs,include,srcfile) {
            include.wantprint=true;
            ListIncl(*include.p_header);
        }ind_end;
    }
}

// List includes relevant to selected targets
static void Main_ListIncl() {
    ind_beg(abt::_db_zs_sel_target_curs,target,abt::_db) {// list includes for all sources of selected targets
        ind_beg(abt::target_c_targsrc_curs,targsrc,target) if (SourceQ(targsrc)) {
            abt::FSrcfile *srcfile=abt::ind_srcfile_Find(src_Get(targsrc));
            if (srcfile) {
                ListIncl(*srcfile);
            }
        }ind_end;
    }ind_end;
    ind_beg(abt::_db_include_curs,include,abt::_db) if (include.wantprint) {// print selected includes
        dev::Include out;
        abt::include_CopyOut(include,out);
        prlog(out);
    }ind_end;
}

// -----------------------------------------------------------------------------

static void Main_Testgen(algo_lib::Replscope &R) {
    Set(R, "$sandbox", "temp/testgen");
    cstring cmd;
    Ins(&R,cmd, "set -e");// exit on first error
    if (algo_lib::_db.cmdline.verbose) {
        Ins(&R,cmd, "set -x");
    }
    Ins(&R,cmd, "echo '# ... Updating testgen area'");
    Ins(&R,cmd, "mkdir -p $sandbox");
    Ins(&R,cmd, "rsync --delete -a bin cpp build include extern data lock $sandbox/");
    Ins(&R,cmd, "mkdir -p $sandbox/temp");
    Ins(&R,cmd, "echo '# ... Running newly built amc in testgen area'");
    Ins(&R,cmd, "$out_dir/amc -in_dir:data -out_dir:$sandbox");
    Ins(&R,cmd, "echo '# ... Here are the diffs'");
    Ins(&R,cmd, "(diff -I signature -u -r ./cpp $sandbox/cpp; "
        "diff -I signature -u -r ./include $sandbox/include) | hilite -d | limit-output 10000");
    Ins(&R,cmd, "echo '# ... Entering sandbox'");
    Ins(&R,cmd, "cd $sandbox");
    Ins(&R,cmd, "echo '# ... Building newly generated code'");
    Ins(&R,cmd, "bin/abt -uname:$uname -compiler:$compiler -cfg:$cfg -arch:$arch %");
    Ins(&R,cmd, "echo '# ... Running unit tests'");
    Ins(&R,cmd, "$out_dir/atf_unit % > temp/atf_unit.out 2>&1 || cat temp/atf_unit.out");
    Ins(&R,cmd, "echo '# ... Everything looks good'");
    SysCmd(cmd,FailokQ(false));
}

// -----------------------------------------------------------------------------

static void Main_Helpscreen() {
    algo_lib::FTxttbl tbl;
    AddCol(tbl, "Target");
    AddCol(tbl, "Comment");
    tempstr all_tgts;
    int len_before=0;
    ListSep ls;
    ind_beg(abt::_db_target_curs, target,abt::_db) {
        all_tgts << ls;
        if (ch_N(all_tgts) > len_before + 80) { all_tgts << "\n    "; len_before=ch_N(all_tgts); }
        all_tgts << target.target;
    }ind_end;
    prlog("all targets:" <<all_tgts);
}

// -----------------------------------------------------------------------------

static void Main_CalcPrecomp() {
    ind_beg(abt::_db_target_curs, target,abt::_db) {
        ind_beg(abt::target_c_targdep_curs, dep,target) {
            if (dep.p_parent->p_ns->nstype == dmmeta_Nstype_nstype_pch) {
                vrfy(target.p_ns->nstype != dmmeta_Nstype_nstype_pch, "can't use precompiled header here");
                abt::FSrcfile& precomp_src = *c_srcfile_Find(*dep.p_parent, 0);
                vrfy(!target.c_precomp, "target cannot have more than one precompiled header");
                target.c_precomp = &precomp_src;
                // make each source file of this target dependent on the precompiled header.
                ind_beg(abt::target_c_srcfile_curs, srcfile, target) {
                    abt::ind_include_GetOrCreate(tempstr()<<srcfile.srcfile<<":"<<precomp_src.srcfile);
                }ind_end;
            }
        }ind_end;
    }ind_end;
}

// -----------------------------------------------------------------------------

// Create commands between START and END for building target TARGET.
//
// compilation workflow for each target:
// cmd_start : start
// comptarg  : begin of compile all files
// linktarg  : begin of link all files
// end       : end of target workflow
//
// start -> compile -> link -> end
//
// all actual commands are allocated to a range between two nodes,
// for instance {start,comptarg}, or {comp,linktarg}
//
static void Main_CreateCmds(algo_lib::Replscope &R, abt::FTarget &target, abt::FSyscmd *start, abt::FSyscmd *end) {
    abt::FSyscmd& targ_start    = NewCmd(start,end);
    abt::FSyscmd& targ_comptarg = NewCmd(&targ_start,end);
    abt::FSyscmd& targ_linktarg = NewCmd(&targ_comptarg,end);
    abt::FSyscmd& targ_insttarg = NewCmd(&targ_linktarg,end);
    abt::FSyscmd& targ_end      = NewCmd(&targ_insttarg,end);
    target.targ_start   = &targ_start;
    target.targ_compile = &targ_comptarg;
    target.targ_link    = &targ_linktarg;
    target.targ_end     = &targ_end;
    tempstr base_cmd;
    Set(R, "$target", target.target);
    if (abt::_db.gcache) {
        base_cmd << "gcache ";
    } else if (abt::_db.ccache) {
        base_cmd << "ccache ";
    }

    base_cmd << abt::_db.cmdline.compiler;

    // create compile commands for each source file.
    ind_beg(abt::target_c_srcfile_curs, srcfile,target) {
        // build source-specific base command
        // switch g++ into C mode when compiling C files.
        // this is specific to g++.
        tempstr opts = abt::EvalSrcfileOpts(target,srcfile);
        // preprocess source file
        if (abt::_db.cmdline.preproc) {
            tempstr outfile(ReplaceExt(srcfile.objpath, ".i"));
            abt::FSyscmd& cmd_preproc = NewCmd(&targ_start, &targ_comptarg);
            cmd_preproc.command << base_cmd << " "<<opts << " -c "<< srcfile.srcfile << " -o "<<outfile<<" -E ";
            prlog("abt.outfile"
                  <<Keyval("srcfile",srcfile.srcfile)
                  <<Keyval("outfile",outfile));
        }
        // compile source file or precompiled header
        if (abt::_db.cmdline.build && srcfile.ood) {
            // for coverage config
            // remove .gcda files - when .o and .gcno gets recompiled,
            // .gcda is no longer valid, and MUST be removed
            tempstr gcdafile(ReplaceExt(srcfile.objpath, ".gcda"));
            DeleteFile(gcdafile);
            abt::FSyscmd& cmd_compile = NewCmd(&targ_start, &targ_comptarg);
            cmd_compile.outfile = srcfile.objpath;
            cmd_compile.command << base_cmd << " " << opts << " -c "<< srcfile.srcfile << " -o "<<srcfile.objpath<<" ";
            line_n_Update(&srcfile);
            cmd_compile.line_n += srcfile.line_n;
        }
    }ind_end;

    bool can_link = target.ood;
    can_link     &= abt::_db.cmdline.build;

    // do not perform final linking step if operation was narrowed down by srcfile
    tempstr cmd;
    if (can_link && target.p_ns->nstype == dmmeta_Nstype_nstype_pch) {
        // do nothing -- the precompiled header has been created
    } else if (can_link && target.p_ns->nstype == dmmeta_Nstype_nstype_lib) {
        // recreate lib from scratch,
        // otherwise old stale archive members don't get removed
        abt::FSyscmd& cmd_ar = NewCmd(&targ_linktarg, &targ_end);
        cmd_ar.command << "rm -f "<<target.outfile;
        cmd_ar.command << "; ar cr "<<target.outfile;
        cmd_ar.outfile = target.outfile;
        ind_beg(abt::target_c_srcfile_curs, srcfile, target) {
            if (!abt::HeaderExtQ(GetFileExt(srcfile.srcfile))) {
                cmd_ar.command << " "<< srcfile.objpath;
            }
        }ind_end;

        abt::FSyscmd& cmd_ranlib = NewCmd(&cmd_ar, &targ_end);
        cmd_ranlib.command<<abt::_db.c_compiler->ranlib<<" "<<target.outfile;
        cmd_ranlib.outfile = target.outfile;
    }
    if (can_link && target.p_ns->nstype == dmmeta_Nstype_nstype_exe) {
        abt::FSyscmd& cmd_link = NewCmd(&targ_linktarg, &targ_insttarg);
        cmd_link.outfile = target.outfile;
        cmd_link.command = abt::EvalLinkCmdline(target);
    }

    // install step
    if (abt::_db.cmdline.install && target.p_ns->nstype == dmmeta_Nstype_nstype_exe) {
        abt::FSyscmd& cmd_inst = NewCmd(&targ_insttarg, &targ_end);
        cmd_inst.redirect = false;
        Ins(&R, cmd_inst.command, "ln -sf ../build/$cfg/$target bin/$target");
    }
}

// -----------------------------------------------------------------------------

static tempstr ReadLink(strptr path) {
    tempstr ret;
    char buf[1024];
    int n=readlink(Zeroterm(tempstr(path)),buf,sizeof(buf));
    if (n>0) {
        ret << strptr(buf,n);
    }
    return ret;
}

// -----------------------------------------------------------------------------

static void MaybeSetVar(algo::Smallstr50 &var, strptr value, strptr source) {
    if (var=="" && value != "") {
        var = value;
        verblog("# abt: selecting "<<source<<" = "<<value);
    }
}

// -----------------------------------------------------------------------------

static void Main_GuessParams() {
    // pick a default config (release most of the time)
    if (!ch_N(abt::_db.cmdline.cfg)) {
        abt::_db.cmdline.cfg = abt::_db.cmdline.testgen ? dev_Cfg_cfg_debug : dev_Cfg_cfg_release;
    }

    // fill in uname, compiler, arch from defaults
    // provided by bootstrap
    // For instance, build/release -> Linux-clang++.release-x86_64
    // means that Linux,clang++ and x86_64 can be guessed
    // from cfg parameter alone
    tempstr default_builddir(ReadLink(tempstr("build/")<<abt::_db.cmdline.cfg));
    MaybeSetVar(abt::_db.cmdline.uname, dev::Builddir_uname_Get(default_builddir), "uname from softlink in build/");
    MaybeSetVar(abt::_db.cmdline.compiler, dev::Builddir_compiler_Get(default_builddir), "compiler from softlink in build/");
    MaybeSetVar(abt::_db.cmdline.arch, dev::Builddir_arch_Get(default_builddir), "arch from softlink in build/");

    if (!ch_N(abt::_db.cmdline.arch) || !ch_N(abt::_db.cmdline.uname)) {
        struct utsname un;
        uname(&un);
        if (!ch_N(abt::_db.cmdline.arch)) abt::_db.cmdline.arch   = un.machine;
        if (!ch_N(abt::_db.cmdline.uname)) abt::_db.cmdline.uname = un.sysname;
    }

    // cause errors if any of these guys are not found
    vrfy(abt::ind_uname_Find   (abt::_db.cmdline.uname)   , tempstr()<<"abt.bad_uname"   <<Keyval("uname",abt::_db.cmdline.uname));
    vrfy(abt::ind_compiler_Find(abt::_db.cmdline.compiler), tempstr()<<"abt.bad_compiler"<<Keyval("compiler",abt::_db.cmdline.compiler));
    vrfy(abt::ind_cfg_Find     (abt::_db.cmdline.cfg)     , tempstr()<<"abt.bad_cfg"     <<Keyval("cfg",abt::_db.cmdline.cfg));
    vrfy(abt::ind_arch_Find    (abt::_db.cmdline.arch)    , tempstr()<<"abt.bad_arch"    <<Keyval("arch",abt::_db.cmdline.arch));
}

// -----------------------------------------------------------------------------

static void Main_ComputeTimestamps() {
    // compute cumulative modification time for each file
    ind_beg(abt::_db_srcfile_curs, srcfile, abt::_db) {
        ComputeCumMod(srcfile);
    }ind_end;

    // compute out-of-date flag for each file
    ind_beg(abt::_db_srcfile_curs, srcfile, abt::_db) {
        UnTime src_out_modtime = Filestat(srcfile.objpath).modtime;
        srcfile.ood  = UnTime_Lt(src_out_modtime, srcfile.cum_modtime);
        srcfile.ood |= abt::_db.cmdline.force;
        srcfile.ood |= Filestat(srcfile.objpath).size == 0;// #AL# zero-sized file is out of date
    }ind_end;

    // compute target cumulative modification time
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        ind_beg(abt::target_c_srcfile_curs, src,target) {
            target.cum_modtime.value = i64_Max(target.cum_modtime.value, src.cum_modtime.value);
        }ind_end;
    }ind_end;

    // compute out-of-date flag for each target (collect it from source files)
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        ind_beg(abt::target_c_srcfile_curs, src,target) {
            target.ood |= src.ood;
        }ind_end;
        target.ood |= abt::_db.cmdline.force;
        if (ch_N(target.outfile)) {
            UnTime targ_out_modtime = Filestat(target.outfile).modtime;
            target.ood |= target.cum_modtime.value > targ_out_modtime.value;
            target.ood |= Filestat(target.outfile).size == 0;// #AL# zero sized file is out of date
        }
    }ind_end;

    // propagate out-of-date flag across targets
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        ComputeOod(target);
    }ind_end;
}

// -----------------------------------------------------------------------------

static void CreateTmpdir() {
    errno_vrfy(CreateDirRecurse("temp")
               ,tempstr()<<"abt.createdir"
               <<Keyval("comment","failed to create temp directory"));
    if (abt::_db.cmdline.printcmd) {
        prlog("mkdir -p temp");
    }
    setenv("TMPDIR","temp",1);
}

// -----------------------------------------------------------------------------

static void DetectCcache() {
    // do not enable ccache on debug builds -- waste of space
    // especially don't enable in coverity phase -- disables coverity
    // ignore in 'printcmd' (bootstrap) mode
    if (abt::_db.cmdline.cfg == dev_Cfg_cfg_release && !abt::_db.cmdline.printcmd) {
        if (DirectoryQ(".gcache/")) {
            abt::_db.gcache=true;
        } else if (DirectoryQ(".ccache/")) {
            abt::_db.ccache=true;
        }
    }
}

// -----------------------------------------------------------------------------

static void CheckActed() {
    bool acted = false;
    acted |= abt::_db.cmdline.list;
    acted |= abt::_db.cmdline.listincl;
    acted |= ch_N(abt::_db.cmdline.disas.expr)>0;
    acted |= abt::_db.cmdline.preproc;
    acted |= abt::_db.cmdline.build;
    acted |= abt::_db.cmdline.clean;
    acted |= abt::_db.cmdline.testgen;
    acted |= abt::_db.cmdline.install;

    if (!acted) {
        prerr("abt.noaction"
              <<Keyval("comment","no action specified. use -help for more information"));
    }
    // display list of groups, and available targets.
    if (!acted) {
        Main_Helpscreen();
    }
}

// -----------------------------------------------------------------------------

static void RewriteOpts() {
    if (!ch_N(abt::_db.cmdline.target.expr) && abt::_db.cmdline.testgen) {
        prlog("abt.testgen  Compiling debug version of amc");
        Regx_ReadSql(abt::_db.cmdline.target, dmmeta_Ns_ns_amc, true);
    }
    if (!abt::_db.cmdline.build
        && !abt::_db.cmdline.list
        && !abt::_db.cmdline.listincl
        && !abt::_db.cmdline.ood
        && !abt::_db.cmdline.clean
        && !abt::_db.cmdline.preproc
        && ch_N(abt::_db.cmdline.target.expr)) {
        abt::_db.cmdline.build=true; // pick a reasonable default
    }
    // pick reasonable maxjobs
    // can we limit these compilers to a group so they don't suck up all cpu on a machine?
    if (abt::_db.cmdline.maxjobs == 0) {
        abt::_db.cmdline.maxjobs = i32_Max(4,sysconf(_SC_NPROCESSORS_ONLN)/2);
    }
    abt::_db.cmdline.maxjobs = i32_Max(abt::_db.cmdline.maxjobs, 1);

    // implies
    if (abt::_db.cmdline.ood) {
        abt::_db.cmdline.list = true;
    }
    // install implies build
    if (abt::_db.cmdline.install) {
        abt::_db.cmdline.build = true;
    }
    if (abt::_db.cmdline.printcmd) {
        prlog("set -e");// exit on first error
        prlog("set -x");// show commands
        abt::_db.cmdline.maxjobs = 1;// deterministic output
        abt::_db.cmdline.force = true;// make everything out-of-date
    }
}

// -----------------------------------------------------------------------------

// Return a canonic name describing current configuration
// This is not the actual build directory. That one is called "out_dir".
// Builddir is just the primary key of a build directory
tempstr abt::GetBuilddir() {
    return dev::Builddir_Concat_uname_compiler_cfg_arch(abt::_db.cmdline.uname
                                                        ,abt::_db.cmdline.compiler
                                                        ,abt::_db.cmdline.cfg
                                                        ,abt::_db.cmdline.arch);
}

// -----------------------------------------------------------------------------

static void Main_ShowOod() {
    bool realexec = !abt::_db.cmdline.dry_run && !abt::_db.cmdline.printcmd;
    int ood_pch=0, ood_src=0, ood_lib=0, ood_exe=0;
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        ood_lib += target.p_ns->nstype == dmmeta_Nstype_nstype_lib && target.ood;
        ood_exe += target.p_ns->nstype == dmmeta_Nstype_nstype_exe && target.ood;
        ind_beg(abt::target_c_srcfile_curs, srcfile, target) {
            ood_src += srcfile.ood;
            ood_pch += srcfile.ood && srcfile.p_target && srcfile.p_target->p_ns->nstype == dmmeta_Nstype_nstype_pch;
        }ind_end;
    }ind_end;
    if (realexec && abt::zs_sel_target_N() > 0 && abt::_db.cmdline.report) {
        prlog("abt.config"
              <<Keyval("config",abt::GetBuilddir())
              <<Keyval("cache",(abt::_db.ccache ? "ccache" : abt::_db.gcache ? "gcache" : "none"))
              <<Keyval("out_dir",abt::_db.cmdline.out_dir)
              );
        prlog("abt.outofdate"
              <<Keyval("pch", ood_pch)
              <<Keyval("src", ood_src)
              <<Keyval("lib", ood_lib)
              <<Keyval("exe", ood_exe));
    }
}

// -----------------------------------------------------------------------------

static void Main_Dry() {
    if (abt::_db.cmdline.dry_run) {
        ind_beg(abt::_db_syscmd_curs, syscmd,abt::_db) {
            dev::Syscmd temp;
            abt::syscmd_CopyOut(syscmd, temp);
            prlog(temp);
        }ind_end;
        ind_beg(abt::_db_syscmddep_curs, syscmddep,abt::_db) {
            dev::Syscmddep temp;
            abt::syscmddep_CopyOut(syscmddep, temp);
            prlog(temp);
        }ind_end;
    }
}

// -----------------------------------------------------------------------------

static void Main_Cmddep() {
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        ind_beg(abt::target_c_targdep_curs, dep,target) {
            // precompiled header -- must wait for it to finish linking
            if (dep.p_parent->p_ns->nstype == dmmeta_Nstype_nstype_pch) {
                abt::syscmddep_InsertMaybe(dev::Syscmddep(target.targ_start->rowid, dep.p_parent->targ_end->rowid));
            }
            // library -- can compile in parallel with it, must wait for link step
            if (dep.p_parent->p_ns->nstype == dmmeta_Nstype_nstype_lib) {
                abt::syscmddep_InsertMaybe(dev::Syscmddep(target.targ_link->rowid, dep.p_parent->targ_end->rowid));
            }
            // other target types -- e.g. group -- no real dependency here.
        }ind_end;
    }ind_end;
}

static void Main_BuildParams(algo_lib::Replscope& R) {
    // pick compiler params
    abt::_db.c_compiler = abt::ind_compiler_Find(abt::_db.cmdline.compiler);
    vrfy(abt::_db.c_compiler , tempstr()<<"abt.compiler bad choice :"<<abt::_db.cmdline.compiler);
    // pick default output directory
    if (!ch_N(abt::_db.cmdline.out_dir)) {
        abt::_db.cmdline.out_dir
            << "build/" << abt::_db.cmdline.uname
            << "-" << abt::_db.cmdline.compiler
            << "." << abt::_db.cmdline.cfg
            << "-" << abt::_db.cmdline.arch;
    }
    //initialize replacements
    Set(R,"$uname"   ,abt::_db.cmdline.uname);
    Set(R,"$compiler",abt::_db.cmdline.compiler);
    Set(R,"$cfg"     ,abt::_db.cmdline.cfg);
    Set(R,"$arch"    ,abt::_db.cmdline.arch);
    Set(R,"$ranlib"  ,abt::_db.c_compiler->ranlib);
    Set(R,"$out_dir" ,abt::_db.cmdline.out_dir);
}

// -----------------------------------------------------------------------------

static void Main_ValidatePch() {
    // validate pch targets
    ind_beg(abt::_db_target_curs, target,abt::_db) if (target.p_ns->nstype == dmmeta_Nstype_nstype_pch) {
        vrfy(c_targsrc_N(target) == 1, "precompiled header target must have 1 source");
    }ind_end;
}

// -----------------------------------------------------------------------------

static void Main_ComputeOutfile() {
    ind_beg(abt::_db_target_curs, tgt,abt::_db) {
        vrfy(!ch_N(tgt.outfile), "internal error");
        if (tgt.p_ns->nstype == dmmeta_Nstype_nstype_lib) {
            tgt.outfile << abt::_db.cmdline.out_dir << MaybeDirSep << tgt.target << "-"
                        << abt::_db.cmdline.arch << ".a";
        } else if (tgt.p_ns->nstype == dmmeta_Nstype_nstype_exe) {
            tgt.outfile << abt::_db.cmdline.out_dir << MaybeDirSep << tgt.target;
        } else if (tgt.p_ns->nstype == dmmeta_Nstype_nstype_pch) {
            vrfy(c_srcfile_N(tgt) > 0, tempstr()<<"precompiled header target ["
                 <<tgt.target<<"] requries at least one source file");
            tgt.outfile << abt::_db.cmdline.out_dir << MaybeDirSep << c_srcfile_Find(tgt,0)->objkey << ".gch";
        }
    }ind_end;
}

// -----------------------------------------------------------------------------

static void Main_ShowReport() {
    abt::_db.report.n_target = abt::zs_sel_target_N();
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        if (abt::_db.cmdline.install) {
            abt::_db.report.n_install += target.targ_end->status ==0;
        }
    }ind_end;
    if (abt::_db.cmdline.report) {
        prlog(abt::_db.report);
    }
}

// -----------------------------------------------------------------------------

void abt::Main() {
    SchedTime cycles = CurrSchedTime();
    SetupExitSignals();

    RewriteOpts();
    DetectCcache();
    if (abt::_db.cmdline.build) {
        CreateTmpdir();
    }

    Main_ValidatePch();
    Main_GuessParams();
    Main_ComputeSysincl();

    algo_lib::Replscope R;
    Main_BuildParams(R);

    Main_SelectTarget();
    if (abt::_db.report.n_err) {
        return;
    }

    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        Main_ComputeAlldep(target,target);
    }ind_end;

    Main_ComputeAlllib();
    // Scan headers for selected targets only
    Main_ReadSrcfile();
    // compute output file name for each target
    Main_ComputeOutfile();

    // create top-level build directory
    algo_lib::FLockfile lockfile;
    if (abt::_db.cmdline.build) {
        CreateDirRecurse(abt::_db.cmdline.out_dir);
        if (abt::_db.cmdline.printcmd) {
            prlog("mkdir -p "<<abt::_db.cmdline.out_dir);
        }
        // create lock file
        LockFileInit(lockfile, DirFileJoin(abt::_db.cmdline.out_dir, "abt.lock"));
    }

    // calculate c_precomp for each target.
    // (optional precompiled header)
    // (unfortunately, precompiled headers are modeled as targets)
    Main_CalcPrecomp();

    // clean -- note, all cleaning has to be done before all compiling
    // or out-of-date determination
    if (abt::_db.cmdline.clean) {
        Main_Clean();
    }

    Main_ComputeTimestamps();

    // list (print to stdout)
    if (abt::_db.cmdline.list) {
        Main_List();
    }

    if (abt::_db.cmdline.listincl) {
        Main_ListIncl();
    }

    abt::FSyscmd *start=NULL,*end=NULL;
    ind_beg(abt::_db_zs_sel_target_curs, target,abt::_db) {
        Main_CreateCmds(R,target,start,end);
    }ind_end;

    // add dependencies between targets.
    Main_Cmddep();

    // compute number of executable commands
    Main_ShowOod();

    if (abt::_db.cmdline.dry_run) {
        Main_Dry();
    } else {
        abt::Main_Build();
    }

    // compiled amc? now apply it to see if it works.
    if (abt::_db.report.n_err == 0 && abt::_db.cmdline.testgen) {
        Refurbish(lockfile);
        Main_Testgen(R);
    }

    // check if we actually did anything, and offer help if we didn't.
    CheckActed();

    if (ch_N(abt::_db.cmdline.disas.expr)) {
        abt::Main_Disas();// show disassembly
    }

    // report abt warnings and errors, and execution time
    abt::_db.report.time = UnDiffSecs(algo::ElapsedSecs(cycles,CurrSchedTime()));
    Main_ShowReport();
    verblog(abt::_db.trace);

    if (abt::_db.report.n_err > 0) {
        algo_lib::_db.exit_code = 1;
    }
}
