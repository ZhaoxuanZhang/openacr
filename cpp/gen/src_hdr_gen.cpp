//
// cpp/gen/src_hdr_gen.cpp
// Generated by AMC
//
// (C) AlgoEngineering LLC 2008-2013
// (C) NYSE | Intercontinental Exchange 2013-2016
//


#include "include/algo.h"  // hard-coded include
#include "include/gen/src_hdr_gen.h"
#include "include/gen/src_hdr_gen.inl.h"
#include "include/gen/command_gen.h"
#include "include/gen/command_gen.inl.h"
#include "include/gen/dmmeta_gen.h"
#include "include/gen/dmmeta_gen.inl.h"
#include "include/gen/algo_gen.h"
#include "include/gen/algo_gen.inl.h"
#include "include/gen/dev_gen.h"
#include "include/gen/dev_gen.inl.h"
#include "include/gen/lib_prot_gen.h"
#include "include/gen/lib_prot_gen.inl.h"
#include "include/gen/algo_lib_gen.h"
#include "include/gen/algo_lib_gen.inl.h"
//#pragma endinclude

// Instantiate all libraries linked into this executable,
// in dependency order
algo_lib::FDb   algo_lib::_db;    // dependency found via dev.targdep
src_hdr::FDb    src_hdr::_db;     // dependency found via dev.targdep

namespace src_hdr {
const char *src_hdr_help =
"src_hdr: Update source file / copyright header\n"
"Usage: src_hdr [options]\n"
"    -in              string  Input directory or filename, - for stdin. default: \"data\"\n"
"    -targsrc         string  Regx of targsrc to update. default: \"%\"\n"
"    -write                   Update files in-place. default: false\n"
"    -update_authors          Update authors from git history. default: false\n"
"    -indent                  Indent source files. default: false\n"
"    -verbose                 Enable verbose mode\n"
"    -debug                   Enable debug mode\n"
"    -version                 Show version information\n"
"    -sig                     Print SHA1 signatures for dispatches\n"
"    -help                    Print this screen and exit\n"
;


const char *src_hdr_syntax =
"-in:string=\"data\"\n"
" -targsrc:string=\"%\"\n"
" -write:flag\n"
" -update_authors:flag\n"
" -indent:flag\n"
;
} // namespace src_hdr
namespace src_hdr {
    // Load statically available data into tables, register tables and database.
    static void          InitReflection();
    static bool          targsrc_InputMaybe(dev::Targsrc &elem) __attribute__((nothrow));
    static bool          ns_InputMaybe(dmmeta::Ns &elem) __attribute__((nothrow));
    static bool          nsx_InputMaybe(dmmeta::Nsx &elem) __attribute__((nothrow));
    // find trace by row id (used to implement reflection)
    static algo::ImrowPtr trace_RowidFind(int t) __attribute__((nothrow));
    // Function return 1
    static i32           trace_N() __attribute__((__warn_unused_result__, nothrow, pure));
    static void          SizeCheck();
} // end namespace src_hdr

// --- src_hdr.trace..Print
// print string representation of src_hdr::trace to string LHS, no header -- cprint:src_hdr.trace.String
void src_hdr::trace_Print(src_hdr::trace & row, algo::cstring &str) {
    algo::tempstr temp;
    str << "src_hdr.trace";
    (void)row;//only to avoid -Wunused-parameter
}

// --- src_hdr.FDb._db.MainArgs
// Main function
void src_hdr::MainArgs(int argc, char **argv) {
    Argtuple argtuple;
    Argtuple_ReadArgv(argtuple, argc,argv,src_hdr_syntax, src_hdr_help);
    vrfy(src_hdr_ReadTupleMaybe(src_hdr::_db.cmdline, argtuple.tuple),"where:read_cmdline");
    vrfy(src_hdr::LoadTuplesMaybe(src_hdr::_db.cmdline.in)
    ,tempstr()<<"where:load_input  "<<algo_lib::DetachBadTags());
    src_hdr::Main(); // call through to user-defined main
}

// --- src_hdr.FDb._db.MainLoop
// Main loop.
void src_hdr::MainLoop() {
    SchedTime time(get_cycles());
    algo_lib::_db.clock          = time;
    do {
        algo_lib::_db.next_loop.value = algo_lib::_db.limit;
        algo_lib::Step(); // dependent namespace specified via (dev.targdep)
    } while (algo_lib::_db.next_loop < algo_lib::_db.limit);
}

// --- src_hdr.FDb._db.Step
// Main step
void src_hdr::Step() {
}

// --- src_hdr.FDb._db.InitReflection
// Load statically available data into tables, register tables and database.
static void src_hdr::InitReflection() {
    algo_lib::imdb_InsertMaybe(algo::Imdb("src_hdr", src_hdr::InsertStrptrMaybe, NULL, src_hdr::MainLoop, NULL, algo::Comment()));

    algo::Imtable t_trace;
    t_trace.imtable         = "src_hdr.trace";
    t_trace.ssimfile        = "";
    t_trace.size            = sizeof(src_hdr::trace);
    t_trace.comment.value   = "";
    t_trace.c_RowidFind     = trace_RowidFind;
    t_trace.NItems          = trace_N;
    t_trace.Print           = (algo::ImrowPrintFcn)src_hdr::trace_Print;
    algo_lib::imtable_InsertMaybe(t_trace);


    // -- load signatures of existing dispatches --
    algo_lib::InsertStrptrMaybe("dmmeta.Dispsigcheck  dispsig:'src_hdr.Input'  signature:'a1e7264997d8d742fc7b03d291e743493427663b'");
}

// --- src_hdr.FDb._db.StaticCheck
void src_hdr::StaticCheck() {
    algo_assert(_offset_of(src_hdr::FieldId, value) + sizeof(((src_hdr::FieldId*)0)->value) == sizeof(src_hdr::FieldId));
}

// --- src_hdr.FDb._db.InsertStrptrMaybe
// Parse strptr into known type and add to database.
// Return value is true unless an error occurs. If return value is false, algo_lib::_db.errtext has error text
bool src_hdr::InsertStrptrMaybe(algo::strptr str) {
    bool retval = true;
    src_hdr::TableId table_id(-1);
    value_SetStrptrMaybe(table_id, algo::GetTypeTag(str));
    switch (value_GetEnum(table_id)) {
        case src_hdr_TableId_dev_Targsrc: { // finput:src_hdr.FDb.targsrc
            dev::Targsrc elem;
            retval = dev::Targsrc_ReadStrptrMaybe(elem, str);
            retval = retval && targsrc_InputMaybe(elem);
            break;
        }
        case src_hdr_TableId_dmmeta_Ns: { // finput:src_hdr.FDb.ns
            dmmeta::Ns elem;
            retval = dmmeta::Ns_ReadStrptrMaybe(elem, str);
            retval = retval && ns_InputMaybe(elem);
            break;
        }
        case src_hdr_TableId_dmmeta_Nsx: { // finput:src_hdr.FDb.nsx
            dmmeta::Nsx elem;
            retval = dmmeta::Nsx_ReadStrptrMaybe(elem, str);
            retval = retval && nsx_InputMaybe(elem);
            break;
        }
        default:
        retval = algo_lib::InsertStrptrMaybe(str);
        break;
    } //switch
    if (!retval) {
        algo_lib::NoteInsertErr(str); // increment error counter
    }
    return retval;
}

// --- src_hdr.FDb._db.LoadTuplesMaybe
// Load all finputs from given directory.
bool src_hdr::LoadTuplesMaybe(algo::strptr root) {
    bool retval = true;
    static const char *ssimfiles[] = {
        "dmmeta.ns", "dmmeta.nsx", "dev.targsrc"
        , NULL};
        retval = algo_lib::DoLoadTuples(root, src_hdr::InsertStrptrMaybe, ssimfiles, true);
        return retval;
}

// --- src_hdr.FDb._db.LoadSsimfileMaybe
// Load specified ssimfile.
bool src_hdr::LoadSsimfileMaybe(algo::strptr fname) {
    bool retval = true;
    if (FileQ(fname)) {
        retval = algo_lib::LoadTuplesFile(fname, src_hdr::InsertStrptrMaybe, true);
    }
    return retval;
}

// --- src_hdr.FDb._db.XrefMaybe
// Insert row into all appropriate indices. If error occurs, store error
// in algo_lib::_db.errtext and return false. Caller must Delete or Unref such row.
bool src_hdr::_db_XrefMaybe() {
    bool retval = true;
    return retval;
}

// --- src_hdr.FDb.targsrc.Alloc
// Allocate memory for new default row.
// If out of memory, process is killed.
src_hdr::FTargsrc& src_hdr::targsrc_Alloc() {
    src_hdr::FTargsrc* row = targsrc_AllocMaybe();
    if (UNLIKELY(row == NULL)) {
        FatalErrorExit("src_hdr.out_of_mem  field:src_hdr.FDb.targsrc  comment:'Alloc failed'");
    }
    return *row;
}

// --- src_hdr.FDb.targsrc.AllocMaybe
// Allocate memory for new element. If out of memory, return NULL.
src_hdr::FTargsrc* src_hdr::targsrc_AllocMaybe() {
    src_hdr::FTargsrc *row = (src_hdr::FTargsrc*)targsrc_AllocMem();
    if (row) {
        new (row) src_hdr::FTargsrc; // call constructor
    }
    return row;
}

// --- src_hdr.FDb.targsrc.InsertMaybe
// Create new row from struct.
// Return pointer to new element, or NULL if insertion failed (due to out-of-memory, duplicate key, etc)
src_hdr::FTargsrc* src_hdr::targsrc_InsertMaybe(const dev::Targsrc &value) {
    src_hdr::FTargsrc *row = &targsrc_Alloc(); // if out of memory, process dies. if input error, return NULL.
    targsrc_CopyIn(*row,const_cast<dev::Targsrc&>(value));
    bool ok = targsrc_XrefMaybe(*row); // this may return false
    if (!ok) {
        targsrc_RemoveLast(); // delete offending row, any existing xrefs are cleared
        row = NULL; // forget this ever happened
    }
    return row;
}

// --- src_hdr.FDb.targsrc.AllocMem
// Allocate space for one element. If no memory available, return NULL.
void* src_hdr::targsrc_AllocMem() {
    u64 new_nelems     = _db.targsrc_n+1;
    // compute level and index on level
    u64 bsr   = algo::u64_BitScanReverse(new_nelems);
    u64 base  = u64(1)<<bsr;
    u64 index = new_nelems-base;
    void *ret = NULL;
    // if level doesn't exist yet, create it
    src_hdr::FTargsrc*  lev   = NULL;
    if (bsr < 32) {
        lev = _db.targsrc_lary[bsr];
        if (!lev) {
            lev=(src_hdr::FTargsrc*)algo_lib::malloc_AllocMem(sizeof(src_hdr::FTargsrc) * (u64(1)<<bsr));
            _db.targsrc_lary[bsr] = lev;
        }
    }
    // allocate element from this level
    if (lev) {
        _db.targsrc_n = new_nelems;
        ret = lev + index;
    }
    return ret;
}

// --- src_hdr.FDb.targsrc.RemoveLast
// Delete last element of array. Do nothing if array is empty.
void src_hdr::targsrc_RemoveLast() {
    u64 n = _db.targsrc_n;
    if (n > 0) {
        n -= 1;
        targsrc_qFind(u64(n)).~FTargsrc();
        _db.targsrc_n = n;
    }
}

// --- src_hdr.FDb.targsrc.InputMaybe
static bool src_hdr::targsrc_InputMaybe(dev::Targsrc &elem) {
    bool retval = true;
    retval = targsrc_InsertMaybe(elem);
    return retval;
}

// --- src_hdr.FDb.targsrc.XrefMaybe
// Insert row into all appropriate indices. If error occurs, store error
// in algo_lib::_db.errtext and return false. Caller must Delete or Unref such row.
bool src_hdr::targsrc_XrefMaybe(src_hdr::FTargsrc &row) {
    bool retval = true;
    (void)row;
    src_hdr::FNs* p_target = src_hdr::ind_ns_Find(target_Get(row));
    if (UNLIKELY(!p_target)) {
        algo_lib::ResetErrtext() << "src_hdr.bad_xref  index:src_hdr.FDb.ind_ns" << Keyval("key", target_Get(row));
        return false;
    }
    // targsrc: save pointer to target
    if (true) { // user-defined insert condition
        row.p_ns = p_target;
    }
    // insert targsrc into index c_targsrc
    if (true) { // user-defined insert condition
        c_targsrc_Insert(*p_target, row);
    }
    return retval;
}

// --- src_hdr.FDb.ns.Alloc
// Allocate memory for new default row.
// If out of memory, process is killed.
src_hdr::FNs& src_hdr::ns_Alloc() {
    src_hdr::FNs* row = ns_AllocMaybe();
    if (UNLIKELY(row == NULL)) {
        FatalErrorExit("src_hdr.out_of_mem  field:src_hdr.FDb.ns  comment:'Alloc failed'");
    }
    return *row;
}

// --- src_hdr.FDb.ns.AllocMaybe
// Allocate memory for new element. If out of memory, return NULL.
src_hdr::FNs* src_hdr::ns_AllocMaybe() {
    src_hdr::FNs *row = (src_hdr::FNs*)ns_AllocMem();
    if (row) {
        new (row) src_hdr::FNs; // call constructor
    }
    return row;
}

// --- src_hdr.FDb.ns.InsertMaybe
// Create new row from struct.
// Return pointer to new element, or NULL if insertion failed (due to out-of-memory, duplicate key, etc)
src_hdr::FNs* src_hdr::ns_InsertMaybe(const dmmeta::Ns &value) {
    src_hdr::FNs *row = &ns_Alloc(); // if out of memory, process dies. if input error, return NULL.
    ns_CopyIn(*row,const_cast<dmmeta::Ns&>(value));
    bool ok = ns_XrefMaybe(*row); // this may return false
    if (!ok) {
        ns_RemoveLast(); // delete offending row, any existing xrefs are cleared
        row = NULL; // forget this ever happened
    }
    return row;
}

// --- src_hdr.FDb.ns.AllocMem
// Allocate space for one element. If no memory available, return NULL.
void* src_hdr::ns_AllocMem() {
    u64 new_nelems     = _db.ns_n+1;
    // compute level and index on level
    u64 bsr   = algo::u64_BitScanReverse(new_nelems);
    u64 base  = u64(1)<<bsr;
    u64 index = new_nelems-base;
    void *ret = NULL;
    // if level doesn't exist yet, create it
    src_hdr::FNs*  lev   = NULL;
    if (bsr < 32) {
        lev = _db.ns_lary[bsr];
        if (!lev) {
            lev=(src_hdr::FNs*)algo_lib::malloc_AllocMem(sizeof(src_hdr::FNs) * (u64(1)<<bsr));
            _db.ns_lary[bsr] = lev;
        }
    }
    // allocate element from this level
    if (lev) {
        _db.ns_n = new_nelems;
        ret = lev + index;
    }
    return ret;
}

// --- src_hdr.FDb.ns.RemoveLast
// Delete last element of array. Do nothing if array is empty.
void src_hdr::ns_RemoveLast() {
    u64 n = _db.ns_n;
    if (n > 0) {
        n -= 1;
        ns_qFind(u64(n)).~FNs();
        _db.ns_n = n;
    }
}

// --- src_hdr.FDb.ns.InputMaybe
static bool src_hdr::ns_InputMaybe(dmmeta::Ns &elem) {
    bool retval = true;
    retval = ns_InsertMaybe(elem);
    return retval;
}

// --- src_hdr.FDb.ns.XrefMaybe
// Insert row into all appropriate indices. If error occurs, store error
// in algo_lib::_db.errtext and return false. Caller must Delete or Unref such row.
bool src_hdr::ns_XrefMaybe(src_hdr::FNs &row) {
    bool retval = true;
    (void)row;
    // insert ns into index ind_ns
    if (true) { // user-defined insert condition
        bool success = ind_ns_InsertMaybe(row);
        if (UNLIKELY(!success)) {
            ch_RemoveAll(algo_lib::_db.errtext);
            algo_lib::_db.errtext << "src_hdr.duplicate_key  xref:src_hdr.FDb.ind_ns"; // check for duplicate key
            return false;
        }
    }
    return retval;
}

// --- src_hdr.FDb.ind_ns.Find
// Find row by key. Return NULL if not found.
src_hdr::FNs* src_hdr::ind_ns_Find(const algo::strptr& key) {
    u32 index = Smallstr16_Hash(0, key) & (_db.ind_ns_buckets_n - 1);
    src_hdr::FNs* *e = &_db.ind_ns_buckets_elems[index];
    src_hdr::FNs* ret=NULL;
    do {
        ret       = *e;
        bool done = !ret || (*ret).ns == key;
        if (done) break;
        e         = &ret->ind_ns_next;
    } while (true);
    return ret;
}

// --- src_hdr.FDb.ind_ns.FindX
// Look up row by key and return reference. Throw exception if not found
src_hdr::FNs& src_hdr::ind_ns_FindX(const algo::strptr& key) {
    src_hdr::FNs* ret = ind_ns_Find(key);
    vrfy(ret, tempstr() << "src_hdr.key_error  table:ind_ns  key:'"<<key<<"'  comment:'key not found'");
    return *ret;
}

// --- src_hdr.FDb.ind_ns.GetOrCreate
// Find row by key. If not found, create and x-reference a new row with with this key.
src_hdr::FNs& src_hdr::ind_ns_GetOrCreate(const algo::strptr& key) {
    src_hdr::FNs* ret = ind_ns_Find(key);
    if (!ret) { //  if memory alloc fails, process dies; if insert fails, function returns NULL.
        ret         = &ns_Alloc();
        (*ret).ns = key;
        bool good = ns_XrefMaybe(*ret);
        if (!good) {
            ns_RemoveLast(); // delete offending row, any existing xrefs are cleared
            ret = NULL;
        }
    }
    return *ret;
}

// --- src_hdr.FDb.ind_ns.InsertMaybe
// Insert row into hash table. Return true if row is reachable through the hash after the function completes.
bool src_hdr::ind_ns_InsertMaybe(src_hdr::FNs& row) {
    ind_ns_Reserve(1);
    bool retval = true; // if already in hash, InsertMaybe returns true
    if (LIKELY(row.ind_ns_next == (src_hdr::FNs*)-1)) {// check if in hash already
        u32 index = Smallstr16_Hash(0, row.ns) & (_db.ind_ns_buckets_n - 1);
        src_hdr::FNs* *prev = &_db.ind_ns_buckets_elems[index];
        do {
            src_hdr::FNs* ret = *prev;
            if (!ret) { // exit condition 1: reached the end of the list
                break;
            }
            if ((*ret).ns == row.ns) { // exit condition 2: found matching key
                retval = false;
                break;
            }
            prev = &ret->ind_ns_next;
        } while (true);
        if (retval) {
            row.ind_ns_next = *prev;
            _db.ind_ns_n++;
            *prev = &row;
        }
    }
    return retval;
}

// --- src_hdr.FDb.ind_ns.Remove
// Remove reference to element from hash index. If element is not in hash, do nothing
void src_hdr::ind_ns_Remove(src_hdr::FNs& row) {
    if (LIKELY(row.ind_ns_next != (src_hdr::FNs*)-1)) {// check if in hash already
        u32 index = Smallstr16_Hash(0, row.ns) & (_db.ind_ns_buckets_n - 1);
        src_hdr::FNs* *prev = &_db.ind_ns_buckets_elems[index]; // addr of pointer to current element
        while (src_hdr::FNs *next = *prev) {                          // scan the collision chain for our element
            if (next == &row) {        // found it?
                *prev = next->ind_ns_next; // unlink (singly linked list)
                _db.ind_ns_n--;
                row.ind_ns_next = (src_hdr::FNs*)-1;// not-in-hash
                break;
            }
            prev = &next->ind_ns_next;
        }
    }
}

// --- src_hdr.FDb.ind_ns.Reserve
// Reserve enough room in the hash for N more elements. Return success code.
void src_hdr::ind_ns_Reserve(int n) {
    u32 old_nbuckets = _db.ind_ns_buckets_n;
    u32 new_nelems   = _db.ind_ns_n + n;
    // # of elements has to be roughly equal to the number of buckets
    if (new_nelems > old_nbuckets) {
        int new_nbuckets = i32_Max(BumpToPow2(new_nelems), u32(4));
        u32 old_size = old_nbuckets * sizeof(src_hdr::FNs*);
        u32 new_size = new_nbuckets * sizeof(src_hdr::FNs*);
        // allocate new array. we don't use Realloc since copying is not needed and factor of 2 probably
        // means new memory will have to be allocated anyway
        src_hdr::FNs* *new_buckets = (src_hdr::FNs**)algo_lib::malloc_AllocMem(new_size);
        if (UNLIKELY(!new_buckets)) {
            FatalErrorExit("src_hdr.out_of_memory  field:src_hdr.FDb.ind_ns");
        }
        memset(new_buckets, 0, new_size); // clear pointers
        // rehash all entries
        for (int i = 0; i < _db.ind_ns_buckets_n; i++) {
            src_hdr::FNs* elem = _db.ind_ns_buckets_elems[i];
            while (elem) {
                src_hdr::FNs &row        = *elem;
                src_hdr::FNs* next       = row.ind_ns_next;
                u32 index          = Smallstr16_Hash(0, row.ns) & (new_nbuckets-1);
                row.ind_ns_next     = new_buckets[index];
                new_buckets[index] = &row;
                elem               = next;
            }
        }
        // free old array
        algo_lib::malloc_FreeMem(_db.ind_ns_buckets_elems, old_size);
        _db.ind_ns_buckets_elems = new_buckets;
        _db.ind_ns_buckets_n = new_nbuckets;
    }
}

// --- src_hdr.FDb.nsx.Alloc
// Allocate memory for new default row.
// If out of memory, process is killed.
src_hdr::FNsx& src_hdr::nsx_Alloc() {
    src_hdr::FNsx* row = nsx_AllocMaybe();
    if (UNLIKELY(row == NULL)) {
        FatalErrorExit("src_hdr.out_of_mem  field:src_hdr.FDb.nsx  comment:'Alloc failed'");
    }
    return *row;
}

// --- src_hdr.FDb.nsx.AllocMaybe
// Allocate memory for new element. If out of memory, return NULL.
src_hdr::FNsx* src_hdr::nsx_AllocMaybe() {
    src_hdr::FNsx *row = (src_hdr::FNsx*)nsx_AllocMem();
    if (row) {
        new (row) src_hdr::FNsx; // call constructor
    }
    return row;
}

// --- src_hdr.FDb.nsx.InsertMaybe
// Create new row from struct.
// Return pointer to new element, or NULL if insertion failed (due to out-of-memory, duplicate key, etc)
src_hdr::FNsx* src_hdr::nsx_InsertMaybe(const dmmeta::Nsx &value) {
    src_hdr::FNsx *row = &nsx_Alloc(); // if out of memory, process dies. if input error, return NULL.
    nsx_CopyIn(*row,const_cast<dmmeta::Nsx&>(value));
    bool ok = nsx_XrefMaybe(*row); // this may return false
    if (!ok) {
        nsx_RemoveLast(); // delete offending row, any existing xrefs are cleared
        row = NULL; // forget this ever happened
    }
    return row;
}

// --- src_hdr.FDb.nsx.AllocMem
// Allocate space for one element. If no memory available, return NULL.
void* src_hdr::nsx_AllocMem() {
    u64 new_nelems     = _db.nsx_n+1;
    // compute level and index on level
    u64 bsr   = algo::u64_BitScanReverse(new_nelems);
    u64 base  = u64(1)<<bsr;
    u64 index = new_nelems-base;
    void *ret = NULL;
    // if level doesn't exist yet, create it
    src_hdr::FNsx*  lev   = NULL;
    if (bsr < 32) {
        lev = _db.nsx_lary[bsr];
        if (!lev) {
            lev=(src_hdr::FNsx*)algo_lib::malloc_AllocMem(sizeof(src_hdr::FNsx) * (u64(1)<<bsr));
            _db.nsx_lary[bsr] = lev;
        }
    }
    // allocate element from this level
    if (lev) {
        _db.nsx_n = new_nelems;
        ret = lev + index;
    }
    return ret;
}

// --- src_hdr.FDb.nsx.RemoveLast
// Delete last element of array. Do nothing if array is empty.
void src_hdr::nsx_RemoveLast() {
    u64 n = _db.nsx_n;
    if (n > 0) {
        n -= 1;
        nsx_qFind(u64(n)).~FNsx();
        _db.nsx_n = n;
    }
}

// --- src_hdr.FDb.nsx.InputMaybe
static bool src_hdr::nsx_InputMaybe(dmmeta::Nsx &elem) {
    bool retval = true;
    retval = nsx_InsertMaybe(elem);
    return retval;
}

// --- src_hdr.FDb.nsx.XrefMaybe
// Insert row into all appropriate indices. If error occurs, store error
// in algo_lib::_db.errtext and return false. Caller must Delete or Unref such row.
bool src_hdr::nsx_XrefMaybe(src_hdr::FNsx &row) {
    bool retval = true;
    (void)row;
    src_hdr::FNs* p_ns = src_hdr::ind_ns_Find(row.ns);
    if (UNLIKELY(!p_ns)) {
        algo_lib::ResetErrtext() << "src_hdr.bad_xref  index:src_hdr.FDb.ind_ns" << Keyval("key", row.ns);
        return false;
    }
    // insert nsx into index c_nsx
    if (true) { // user-defined insert condition
        bool success = c_nsx_InsertMaybe(*p_ns, row);
        if (UNLIKELY(!success)) {
            ch_RemoveAll(algo_lib::_db.errtext);
            algo_lib::_db.errtext << "src_hdr.duplicate_key  xref:src_hdr.FNs.c_nsx"; // check for duplicate key
            return false;
        }
    }
    return retval;
}

// --- src_hdr.FDb.trace.RowidFind
// find trace by row id (used to implement reflection)
static algo::ImrowPtr src_hdr::trace_RowidFind(int t) {
    return algo::ImrowPtr(t==0 ? u64(&_db.trace) : u64(0));
}

// --- src_hdr.FDb.trace.N
// Function return 1
inline static i32 src_hdr::trace_N() {
    return 1;
}

// --- src_hdr.FDb..Init
// Set all fields to initial values.
void src_hdr::FDb_Init() {
    // initialize LAry targsrc (src_hdr.FDb.targsrc)
    _db.targsrc_n = 0;
    memset(_db.targsrc_lary, 0, sizeof(_db.targsrc_lary)); // zero out all level pointers
    src_hdr::FTargsrc* targsrc_first = (src_hdr::FTargsrc*)algo_lib::malloc_AllocMem(sizeof(src_hdr::FTargsrc) * (u64(1)<<4));
    if (!targsrc_first) {
        FatalErrorExit("out of memory");
    }
    for (int i = 0; i < 4; i++) {
        _db.targsrc_lary[i]  = targsrc_first;
        targsrc_first    += 1ULL<<i;
    }
    // initialize LAry ns (src_hdr.FDb.ns)
    _db.ns_n = 0;
    memset(_db.ns_lary, 0, sizeof(_db.ns_lary)); // zero out all level pointers
    src_hdr::FNs* ns_first = (src_hdr::FNs*)algo_lib::malloc_AllocMem(sizeof(src_hdr::FNs) * (u64(1)<<4));
    if (!ns_first) {
        FatalErrorExit("out of memory");
    }
    for (int i = 0; i < 4; i++) {
        _db.ns_lary[i]  = ns_first;
        ns_first    += 1ULL<<i;
    }
    // initialize hash table for src_hdr::FNs;
    _db.ind_ns_n             	= 0; // (src_hdr.FDb.ind_ns)
    _db.ind_ns_buckets_n     	= 4; // (src_hdr.FDb.ind_ns)
    _db.ind_ns_buckets_elems 	= (src_hdr::FNs**)algo_lib::malloc_AllocMem(sizeof(src_hdr::FNs*)*_db.ind_ns_buckets_n); // initial buckets (src_hdr.FDb.ind_ns)
    if (!_db.ind_ns_buckets_elems) {
        FatalErrorExit("out of memory"); // (src_hdr.FDb.ind_ns)
    }
    memset(_db.ind_ns_buckets_elems, 0, sizeof(src_hdr::FNs*)*_db.ind_ns_buckets_n); // (src_hdr.FDb.ind_ns)
    // initialize LAry nsx (src_hdr.FDb.nsx)
    _db.nsx_n = 0;
    memset(_db.nsx_lary, 0, sizeof(_db.nsx_lary)); // zero out all level pointers
    src_hdr::FNsx* nsx_first = (src_hdr::FNsx*)algo_lib::malloc_AllocMem(sizeof(src_hdr::FNsx) * (u64(1)<<4));
    if (!nsx_first) {
        FatalErrorExit("out of memory");
    }
    for (int i = 0; i < 4; i++) {
        _db.nsx_lary[i]  = nsx_first;
        nsx_first    += 1ULL<<i;
    }

    src_hdr::InitReflection();
}

// --- src_hdr.FDb..Uninit
void src_hdr::FDb_Uninit() {
    src_hdr::FDb &row = _db; (void)row;

    // src_hdr.FDb.nsx.Uninit (Lary)  //
    // skip destruction in global scope

    // src_hdr.FDb.ind_ns.Uninit (Thash)  //
    // skip destruction of ind_ns in global scope

    // src_hdr.FDb.ns.Uninit (Lary)  //
    // skip destruction in global scope

    // src_hdr.FDb.targsrc.Uninit (Lary)  //
    // skip destruction in global scope
}

// --- src_hdr.FNs.base.CopyOut
// Copy fields out of row
void src_hdr::ns_CopyOut(src_hdr::FNs &row, dmmeta::Ns &out) {
    out.ns = row.ns;
    out.nstype = row.nstype;
    out.comment = row.comment;
}

// --- src_hdr.FNs.base.CopyIn
// Copy fields in to row
void src_hdr::ns_CopyIn(src_hdr::FNs &row, dmmeta::Ns &in) {
    row.ns = in.ns;
    row.nstype = in.nstype;
    row.comment = in.comment;
}

// --- src_hdr.FNs.c_targsrc.Insert
// Insert pointer to row into array. Row must not already be in array.
// If pointer is already in the array, it may be inserted twice.
void src_hdr::c_targsrc_Insert(src_hdr::FNs& ns, src_hdr::FTargsrc& row) {
    if (bool_Update(row.ns_c_targsrc_in_ary,true)) {
        // reserve space
        c_targsrc_Reserve(ns, 1);
        u32 n  = ns.c_targsrc_n;
        u32 at = n;
        src_hdr::FTargsrc* *elems = ns.c_targsrc_elems;
        elems[at] = &row;
        ns.c_targsrc_n = n+1;

    }
}

// --- src_hdr.FNs.c_targsrc.InsertMaybe
// Insert pointer to row in array.
// If row is already in the array, do nothing.
// Return value: whether element was inserted into array.
bool src_hdr::c_targsrc_InsertMaybe(src_hdr::FNs& ns, src_hdr::FTargsrc& row) {
    bool retval = !row.ns_c_targsrc_in_ary;
    c_targsrc_Insert(ns,row); // check is performed in _Insert again
    return retval;
}

// --- src_hdr.FNs.c_targsrc.Remove
// Find element using linear scan. If element is in array, remove, otherwise do nothing
void src_hdr::c_targsrc_Remove(src_hdr::FNs& ns, src_hdr::FTargsrc& row) {
    if (bool_Update(row.ns_c_targsrc_in_ary,false)) {
        int lim = ns.c_targsrc_n;
        src_hdr::FTargsrc* *elems = ns.c_targsrc_elems;
        // search backward, so that most recently added element is found first.
        // if found, shift array.
        for (int i = lim-1; i>=0; i--) {
            src_hdr::FTargsrc* elem = elems[i]; // fetch element
            if (elem == &row) {
                int j = i + 1;
                size_t nbytes = sizeof(src_hdr::FTargsrc*) * (lim - j);
                memmove(elems + i, elems + j, nbytes);
                ns.c_targsrc_n = lim - 1;
                break;
            }
        }
    }
}

// --- src_hdr.FNs.c_targsrc.Reserve
// Reserve space in index for N more elements;
void src_hdr::c_targsrc_Reserve(src_hdr::FNs& ns, u32 n) {
    u32 old_max = ns.c_targsrc_max;
    if (UNLIKELY(ns.c_targsrc_n + n > old_max)) {
        u32 new_max  = u32_Max(4, old_max * 2);
        u32 old_size = old_max * sizeof(src_hdr::FTargsrc*);
        u32 new_size = new_max * sizeof(src_hdr::FTargsrc*);
        void *new_mem = algo_lib::malloc_ReallocMem(ns.c_targsrc_elems, old_size, new_size);
        if (UNLIKELY(!new_mem)) {
            FatalErrorExit("src_hdr.out_of_memory  field:src_hdr.FNs.c_targsrc");
        }
        ns.c_targsrc_elems = (src_hdr::FTargsrc**)new_mem;
        ns.c_targsrc_max = new_max;
    }
}

// --- src_hdr.FNs..Uninit
void src_hdr::FNs_Uninit(src_hdr::FNs& ns) {
    src_hdr::FNs &row = ns; (void)row;
    ind_ns_Remove(row); // remove ns from index ind_ns

    // src_hdr.FNs.c_targsrc.Uninit (Ptrary)  //
    algo_lib::malloc_FreeMem(ns.c_targsrc_elems, sizeof(src_hdr::FTargsrc*)*ns.c_targsrc_max); // (src_hdr.FNs.c_targsrc)
}

// --- src_hdr.FNsx.base.CopyOut
// Copy fields out of row
void src_hdr::nsx_CopyOut(src_hdr::FNsx &row, dmmeta::Nsx &out) {
    out.ns = row.ns;
    out.genthrow = row.genthrow;
    out.correct_getorcreate = row.correct_getorcreate;
    out.pool = row.pool;
    out.sortxref = row.sortxref;
    out.pack = row.pack;
    out.fldoffset_asserts = row.fldoffset_asserts;
    out.comment = row.comment;
}

// --- src_hdr.FNsx.base.CopyIn
// Copy fields in to row
void src_hdr::nsx_CopyIn(src_hdr::FNsx &row, dmmeta::Nsx &in) {
    row.ns = in.ns;
    row.genthrow = in.genthrow;
    row.correct_getorcreate = in.correct_getorcreate;
    row.pool = in.pool;
    row.sortxref = in.sortxref;
    row.pack = in.pack;
    row.fldoffset_asserts = in.fldoffset_asserts;
    row.comment = in.comment;
}

// --- src_hdr.FNsx..Uninit
void src_hdr::FNsx_Uninit(src_hdr::FNsx& nsx) {
    src_hdr::FNsx &row = nsx; (void)row;
    src_hdr::FNs* p_ns = src_hdr::ind_ns_Find(row.ns);
    if (p_ns)  {
        c_nsx_Remove(*p_ns, row);// remove nsx from index c_nsx
    }
}

// --- src_hdr.FTargsrc.base.CopyOut
// Copy fields out of row
void src_hdr::targsrc_CopyOut(src_hdr::FTargsrc &row, dev::Targsrc &out) {
    out.targsrc = row.targsrc;
    out.comment = row.comment;
}

// --- src_hdr.FTargsrc.base.CopyIn
// Copy fields in to row
void src_hdr::targsrc_CopyIn(src_hdr::FTargsrc &row, dev::Targsrc &in) {
    row.targsrc = in.targsrc;
    row.comment = in.comment;
}

// --- src_hdr.FTargsrc.target.Get
algo::Smallstr16 src_hdr::target_Get(src_hdr::FTargsrc& targsrc) {
    algo::Smallstr16 ret(algo::Pathcomp(targsrc.targsrc, "/LL"));
    return ret;
}

// --- src_hdr.FTargsrc.src.Get
algo::Smallstr200 src_hdr::src_Get(src_hdr::FTargsrc& targsrc) {
    algo::Smallstr200 ret(algo::Pathcomp(targsrc.targsrc, "/LR"));
    return ret;
}

// --- src_hdr.FTargsrc.ext.Get
algo::Smallstr10 src_hdr::ext_Get(src_hdr::FTargsrc& targsrc) {
    algo::Smallstr10 ret(algo::Pathcomp(targsrc.targsrc, ".RR"));
    return ret;
}

// --- src_hdr.FTargsrc..Uninit
void src_hdr::FTargsrc_Uninit(src_hdr::FTargsrc& targsrc) {
    src_hdr::FTargsrc &row = targsrc; (void)row;
    src_hdr::FNs* p_target = src_hdr::ind_ns_Find(target_Get(row));
    if (p_target)  {
        c_targsrc_Remove(*p_target, row);// remove targsrc from index c_targsrc
    }
}

// --- src_hdr.FieldId.value.ToCstr
// Convert numeric value of field to one of predefined string constants.
// If string is found, return a static C string. Otherwise, return NULL.
const char* src_hdr::value_ToCstr(const src_hdr::FieldId& parent) {
    const char *ret = NULL;
    switch(value_GetEnum(parent)) {
        case src_hdr_FieldId_value         : ret = "value";  break;
    }
    return ret;
}

// --- src_hdr.FieldId.value.Print
// Convert value to a string. First, attempt conversion to a known string.
// If no string matches, print value as a numeric value.
void src_hdr::value_Print(const src_hdr::FieldId& parent, algo::cstring &lhs) {
    const char *strval = value_ToCstr(parent);
    if (strval) {
        lhs << strval;
    } else {
        lhs << parent.value;
    }
}

// --- src_hdr.FieldId.value.SetStrptrMaybe
// Convert string to field.
// If the string is invalid, do not modify field and return false.
// In case of success, return true
bool src_hdr::value_SetStrptrMaybe(src_hdr::FieldId& parent, algo::strptr rhs) {
    bool ret = false;
    switch (elems_N(rhs)) {
        case 5: {
            switch (u64(ReadLE32(rhs.elems))|(u64(rhs[4])<<32)) {
                case LE_STR5('v','a','l','u','e'): {
                    value_SetEnum(parent,src_hdr_FieldId_value); ret = true; break;
                }
            }
            break;
        }
    }
    return ret;
}

// --- src_hdr.FieldId.value.SetStrptr
// Convert string to field.
// If the string is invalid, set numeric value to DFLT
void src_hdr::value_SetStrptr(src_hdr::FieldId& parent, algo::strptr rhs, src_hdr_FieldIdEnum dflt) {
    if (!value_SetStrptrMaybe(parent,rhs)) value_SetEnum(parent,dflt);
}

// --- src_hdr.FieldId.value.ReadStrptrMaybe
// Convert string to field. Return success value
bool src_hdr::value_ReadStrptrMaybe(src_hdr::FieldId& parent, algo::strptr rhs) {
    bool retval = false;
    retval = value_SetStrptrMaybe(parent,rhs); // try symbol conversion
    if (!retval) { // didn't work? try reading as underlying type
        retval = i32_ReadStrptrMaybe(parent.value,rhs);
    }
    return retval;
}

// --- src_hdr.FieldId..ReadStrptrMaybe
// Read fields of src_hdr::FieldId from an ascii string.
// The format of the string is the format of the src_hdr::FieldId's only field
bool src_hdr::FieldId_ReadStrptrMaybe(src_hdr::FieldId &parent, algo::strptr in_str) {
    bool retval = true;
    retval = retval && src_hdr::value_ReadStrptrMaybe(parent, in_str);
    return retval;
}

// --- src_hdr.FieldId..Print
// print string representation of src_hdr::FieldId to string LHS, no header -- cprint:src_hdr.FieldId.String
void src_hdr::FieldId_Print(src_hdr::FieldId & row, algo::cstring &str) {
    src_hdr::value_Print(row, str);
}

// --- src_hdr.TableId.value.ToCstr
// Convert numeric value of field to one of predefined string constants.
// If string is found, return a static C string. Otherwise, return NULL.
const char* src_hdr::value_ToCstr(const src_hdr::TableId& parent) {
    const char *ret = NULL;
    switch(value_GetEnum(parent)) {
        case src_hdr_TableId_dmmeta_Ns     : ret = "dmmeta.Ns";  break;
        case src_hdr_TableId_dmmeta_Nsx    : ret = "dmmeta.Nsx";  break;
        case src_hdr_TableId_dev_Targsrc   : ret = "dev.Targsrc";  break;
    }
    return ret;
}

// --- src_hdr.TableId.value.Print
// Convert value to a string. First, attempt conversion to a known string.
// If no string matches, print value as a numeric value.
void src_hdr::value_Print(const src_hdr::TableId& parent, algo::cstring &lhs) {
    const char *strval = value_ToCstr(parent);
    if (strval) {
        lhs << strval;
    } else {
        lhs << parent.value;
    }
}

// --- src_hdr.TableId.value.SetStrptrMaybe
// Convert string to field.
// If the string is invalid, do not modify field and return false.
// In case of success, return true
bool src_hdr::value_SetStrptrMaybe(src_hdr::TableId& parent, algo::strptr rhs) {
    bool ret = false;
    switch (elems_N(rhs)) {
        case 9: {
            switch (ReadLE64(rhs.elems)) {
                case LE_STR8('d','m','m','e','t','a','.','N'): {
                    if (memcmp(rhs.elems+8,"s",1)==0) { value_SetEnum(parent,src_hdr_TableId_dmmeta_Ns); ret = true; break; }
                    break;
                }
                case LE_STR8('d','m','m','e','t','a','.','n'): {
                    if (memcmp(rhs.elems+8,"s",1)==0) { value_SetEnum(parent,src_hdr_TableId_dmmeta_ns); ret = true; break; }
                    break;
                }
            }
            break;
        }
        case 10: {
            switch (ReadLE64(rhs.elems)) {
                case LE_STR8('d','m','m','e','t','a','.','N'): {
                    if (memcmp(rhs.elems+8,"sx",2)==0) { value_SetEnum(parent,src_hdr_TableId_dmmeta_Nsx); ret = true; break; }
                    break;
                }
                case LE_STR8('d','m','m','e','t','a','.','n'): {
                    if (memcmp(rhs.elems+8,"sx",2)==0) { value_SetEnum(parent,src_hdr_TableId_dmmeta_nsx); ret = true; break; }
                    break;
                }
            }
            break;
        }
        case 11: {
            switch (ReadLE64(rhs.elems)) {
                case LE_STR8('d','e','v','.','T','a','r','g'): {
                    if (memcmp(rhs.elems+8,"src",3)==0) { value_SetEnum(parent,src_hdr_TableId_dev_Targsrc); ret = true; break; }
                    break;
                }
                case LE_STR8('d','e','v','.','t','a','r','g'): {
                    if (memcmp(rhs.elems+8,"src",3)==0) { value_SetEnum(parent,src_hdr_TableId_dev_targsrc); ret = true; break; }
                    break;
                }
            }
            break;
        }
    }
    return ret;
}

// --- src_hdr.TableId.value.SetStrptr
// Convert string to field.
// If the string is invalid, set numeric value to DFLT
void src_hdr::value_SetStrptr(src_hdr::TableId& parent, algo::strptr rhs, src_hdr_TableIdEnum dflt) {
    if (!value_SetStrptrMaybe(parent,rhs)) value_SetEnum(parent,dflt);
}

// --- src_hdr.TableId.value.ReadStrptrMaybe
// Convert string to field. Return success value
bool src_hdr::value_ReadStrptrMaybe(src_hdr::TableId& parent, algo::strptr rhs) {
    bool retval = false;
    retval = value_SetStrptrMaybe(parent,rhs); // try symbol conversion
    if (!retval) { // didn't work? try reading as underlying type
        retval = i32_ReadStrptrMaybe(parent.value,rhs);
    }
    return retval;
}

// --- src_hdr.TableId..ReadStrptrMaybe
// Read fields of src_hdr::TableId from an ascii string.
// The format of the string is the format of the src_hdr::TableId's only field
bool src_hdr::TableId_ReadStrptrMaybe(src_hdr::TableId &parent, algo::strptr in_str) {
    bool retval = true;
    retval = retval && src_hdr::value_ReadStrptrMaybe(parent, in_str);
    return retval;
}

// --- src_hdr.TableId..Print
// print string representation of src_hdr::TableId to string LHS, no header -- cprint:src_hdr.TableId.String
void src_hdr::TableId_Print(src_hdr::TableId & row, algo::cstring &str) {
    src_hdr::value_Print(row, str);
}

// --- src_hdr...main
int main(int argc, char **argv) {
    try {
        algo_lib::FDb_Init();
        src_hdr::FDb_Init();
        algo_lib::_db.argc = argc;
        algo_lib::_db.argv = argv;
        algo_lib::IohookInit();
        src_hdr::MainArgs(algo_lib::_db.argc,algo_lib::_db.argv); // dmmeta.main:src_hdr
    } catch(algo_lib::ErrorX &x) {
        prerr("src_hdr.error  " << x); // there may be additional hints in DetachBadTags
        algo_lib::_db.exit_code = 1;
    }
    if (algo_lib::_db.last_signal) {
        algo_lib::_db.exit_code = 1;
    }
    try {
        src_hdr::FDb_Uninit();
        algo_lib::FDb_Uninit();
    } catch(algo_lib::ErrorX &x) {
        // don't print anything, might crash
        algo_lib::_db.exit_code = 1;
    }
    return algo_lib::_db.exit_code;
}

// --- src_hdr...SizeCheck
inline static void src_hdr::SizeCheck() {
}
