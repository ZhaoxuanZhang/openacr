//
// include/gen/atf_norm_gen.inl.h
// Generated by AMC
//
// (C) AlgoEngineering LLC 2008-2013
// (C) NYSE | Intercontinental Exchange 2013-2016
//


#pragma once
#include "include/gen/command_gen.inl.h"
#include "include/gen/atfdb_gen.inl.h"
#include "include/gen/algo_gen.inl.h"
#include "include/gen/dmmeta_gen.inl.h"
#include "include/gen/dev_gen.inl.h"
//#pragma endinclude
inline atf_norm::trace::trace() {
}


// --- atf_norm.FDb.normcheck.EmptyQ
// Return true if index is empty
inline bool atf_norm::normcheck_EmptyQ() {
    return _db.normcheck_n == 0;
}

// --- atf_norm.FDb.normcheck.Find
// Look up row by row id. Return NULL if out of range
inline atf_norm::FNormcheck* atf_norm::normcheck_Find(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    atf_norm::FNormcheck *retval = NULL;
    if (LIKELY(x <= u64(_db.normcheck_n))) {
        retval = &_db.normcheck_lary[bsr][index];
    }
    return retval;
}

// --- atf_norm.FDb.normcheck.Last
// Return pointer to last element of array, or NULL if array is empty
inline atf_norm::FNormcheck* atf_norm::normcheck_Last() {
    return normcheck_Find(u64(_db.normcheck_n-1));
}

// --- atf_norm.FDb.normcheck.N
// Return number of items in the pool
inline i32 atf_norm::normcheck_N() {
    return _db.normcheck_n;
}

// --- atf_norm.FDb.normcheck.qFind
// 'quick' Access row by row id. No bounds checking.
inline atf_norm::FNormcheck& atf_norm::normcheck_qFind(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    return _db.normcheck_lary[bsr][index];
}

// --- atf_norm.FDb.ssimfile.EmptyQ
// Return true if index is empty
inline bool atf_norm::ssimfile_EmptyQ() {
    return _db.ssimfile_n == 0;
}

// --- atf_norm.FDb.ssimfile.Find
// Look up row by row id. Return NULL if out of range
inline atf_norm::FSsimfile* atf_norm::ssimfile_Find(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    atf_norm::FSsimfile *retval = NULL;
    if (LIKELY(x <= u64(_db.ssimfile_n))) {
        retval = &_db.ssimfile_lary[bsr][index];
    }
    return retval;
}

// --- atf_norm.FDb.ssimfile.Last
// Return pointer to last element of array, or NULL if array is empty
inline atf_norm::FSsimfile* atf_norm::ssimfile_Last() {
    return ssimfile_Find(u64(_db.ssimfile_n-1));
}

// --- atf_norm.FDb.ssimfile.N
// Return number of items in the pool
inline i32 atf_norm::ssimfile_N() {
    return _db.ssimfile_n;
}

// --- atf_norm.FDb.ssimfile.qFind
// 'quick' Access row by row id. No bounds checking.
inline atf_norm::FSsimfile& atf_norm::ssimfile_qFind(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    return _db.ssimfile_lary[bsr][index];
}

// --- atf_norm.FDb.ind_ssimfile.EmptyQ
// Return true if hash is empty
inline bool atf_norm::ind_ssimfile_EmptyQ() {
    return _db.ind_ssimfile_n == 0;
}

// --- atf_norm.FDb.ind_ssimfile.N
// Return number of items in the hash
inline i32 atf_norm::ind_ssimfile_N() {
    return _db.ind_ssimfile_n;
}

// --- atf_norm.FDb.scriptfile.EmptyQ
// Return true if index is empty
inline bool atf_norm::scriptfile_EmptyQ() {
    return _db.scriptfile_n == 0;
}

// --- atf_norm.FDb.scriptfile.Find
// Look up row by row id. Return NULL if out of range
inline atf_norm::FScriptfile* atf_norm::scriptfile_Find(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    atf_norm::FScriptfile *retval = NULL;
    if (LIKELY(x <= u64(_db.scriptfile_n))) {
        retval = &_db.scriptfile_lary[bsr][index];
    }
    return retval;
}

// --- atf_norm.FDb.scriptfile.Last
// Return pointer to last element of array, or NULL if array is empty
inline atf_norm::FScriptfile* atf_norm::scriptfile_Last() {
    return scriptfile_Find(u64(_db.scriptfile_n-1));
}

// --- atf_norm.FDb.scriptfile.N
// Return number of items in the pool
inline i32 atf_norm::scriptfile_N() {
    return _db.scriptfile_n;
}

// --- atf_norm.FDb.scriptfile.qFind
// 'quick' Access row by row id. No bounds checking.
inline atf_norm::FScriptfile& atf_norm::scriptfile_qFind(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    return _db.scriptfile_lary[bsr][index];
}

// --- atf_norm.FDb.ind_scriptfile.EmptyQ
// Return true if hash is empty
inline bool atf_norm::ind_scriptfile_EmptyQ() {
    return _db.ind_scriptfile_n == 0;
}

// --- atf_norm.FDb.ind_scriptfile.N
// Return number of items in the hash
inline i32 atf_norm::ind_scriptfile_N() {
    return _db.ind_scriptfile_n;
}

// --- atf_norm.FDb.ns.EmptyQ
// Return true if index is empty
inline bool atf_norm::ns_EmptyQ() {
    return _db.ns_n == 0;
}

// --- atf_norm.FDb.ns.Find
// Look up row by row id. Return NULL if out of range
inline atf_norm::FNs* atf_norm::ns_Find(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    atf_norm::FNs *retval = NULL;
    if (LIKELY(x <= u64(_db.ns_n))) {
        retval = &_db.ns_lary[bsr][index];
    }
    return retval;
}

// --- atf_norm.FDb.ns.Last
// Return pointer to last element of array, or NULL if array is empty
inline atf_norm::FNs* atf_norm::ns_Last() {
    return ns_Find(u64(_db.ns_n-1));
}

// --- atf_norm.FDb.ns.N
// Return number of items in the pool
inline i32 atf_norm::ns_N() {
    return _db.ns_n;
}

// --- atf_norm.FDb.ns.qFind
// 'quick' Access row by row id. No bounds checking.
inline atf_norm::FNs& atf_norm::ns_qFind(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    return _db.ns_lary[bsr][index];
}

// --- atf_norm.FDb.ind_ns.EmptyQ
// Return true if hash is empty
inline bool atf_norm::ind_ns_EmptyQ() {
    return _db.ind_ns_n == 0;
}

// --- atf_norm.FDb.ind_ns.N
// Return number of items in the hash
inline i32 atf_norm::ind_ns_N() {
    return _db.ind_ns_n;
}

// --- atf_norm.FDb.readme.EmptyQ
// Return true if index is empty
inline bool atf_norm::readme_EmptyQ() {
    return _db.readme_n == 0;
}

// --- atf_norm.FDb.readme.Find
// Look up row by row id. Return NULL if out of range
inline atf_norm::FReadme* atf_norm::readme_Find(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    atf_norm::FReadme *retval = NULL;
    if (LIKELY(x <= u64(_db.readme_n))) {
        retval = &_db.readme_lary[bsr][index];
    }
    return retval;
}

// --- atf_norm.FDb.readme.Last
// Return pointer to last element of array, or NULL if array is empty
inline atf_norm::FReadme* atf_norm::readme_Last() {
    return readme_Find(u64(_db.readme_n-1));
}

// --- atf_norm.FDb.readme.N
// Return number of items in the pool
inline i32 atf_norm::readme_N() {
    return _db.readme_n;
}

// --- atf_norm.FDb.readme.qFind
// 'quick' Access row by row id. No bounds checking.
inline atf_norm::FReadme& atf_norm::readme_qFind(u64 t) {
    u64 x = t + 1;
    u64 bsr   = algo::u64_BitScanReverse(x);
    u64 base  = u64(1)<<bsr;
    u64 index = x-base;
    return _db.readme_lary[bsr][index];
}

// --- atf_norm.FDb.normcheck_curs.Reset
// cursor points to valid item
inline void atf_norm::_db_normcheck_curs_Reset(_db_normcheck_curs &curs, atf_norm::FDb &parent) {
    curs.parent = &parent;
    curs.index = 0;
}

// --- atf_norm.FDb.normcheck_curs.ValidQ
// cursor points to valid item
inline bool atf_norm::_db_normcheck_curs_ValidQ(_db_normcheck_curs &curs) {
    return curs.index < _db.normcheck_n;
}

// --- atf_norm.FDb.normcheck_curs.Next
// proceed to next item
inline void atf_norm::_db_normcheck_curs_Next(_db_normcheck_curs &curs) {
    curs.index++;
}

// --- atf_norm.FDb.normcheck_curs.Access
// item access
inline atf_norm::FNormcheck& atf_norm::_db_normcheck_curs_Access(_db_normcheck_curs &curs) {
    return normcheck_qFind(u64(curs.index));
}

// --- atf_norm.FDb.ssimfile_curs.Reset
// cursor points to valid item
inline void atf_norm::_db_ssimfile_curs_Reset(_db_ssimfile_curs &curs, atf_norm::FDb &parent) {
    curs.parent = &parent;
    curs.index = 0;
}

// --- atf_norm.FDb.ssimfile_curs.ValidQ
// cursor points to valid item
inline bool atf_norm::_db_ssimfile_curs_ValidQ(_db_ssimfile_curs &curs) {
    return curs.index < _db.ssimfile_n;
}

// --- atf_norm.FDb.ssimfile_curs.Next
// proceed to next item
inline void atf_norm::_db_ssimfile_curs_Next(_db_ssimfile_curs &curs) {
    curs.index++;
}

// --- atf_norm.FDb.ssimfile_curs.Access
// item access
inline atf_norm::FSsimfile& atf_norm::_db_ssimfile_curs_Access(_db_ssimfile_curs &curs) {
    return ssimfile_qFind(u64(curs.index));
}

// --- atf_norm.FDb.scriptfile_curs.Reset
// cursor points to valid item
inline void atf_norm::_db_scriptfile_curs_Reset(_db_scriptfile_curs &curs, atf_norm::FDb &parent) {
    curs.parent = &parent;
    curs.index = 0;
}

// --- atf_norm.FDb.scriptfile_curs.ValidQ
// cursor points to valid item
inline bool atf_norm::_db_scriptfile_curs_ValidQ(_db_scriptfile_curs &curs) {
    return curs.index < _db.scriptfile_n;
}

// --- atf_norm.FDb.scriptfile_curs.Next
// proceed to next item
inline void atf_norm::_db_scriptfile_curs_Next(_db_scriptfile_curs &curs) {
    curs.index++;
}

// --- atf_norm.FDb.scriptfile_curs.Access
// item access
inline atf_norm::FScriptfile& atf_norm::_db_scriptfile_curs_Access(_db_scriptfile_curs &curs) {
    return scriptfile_qFind(u64(curs.index));
}

// --- atf_norm.FDb.ns_curs.Reset
// cursor points to valid item
inline void atf_norm::_db_ns_curs_Reset(_db_ns_curs &curs, atf_norm::FDb &parent) {
    curs.parent = &parent;
    curs.index = 0;
}

// --- atf_norm.FDb.ns_curs.ValidQ
// cursor points to valid item
inline bool atf_norm::_db_ns_curs_ValidQ(_db_ns_curs &curs) {
    return curs.index < _db.ns_n;
}

// --- atf_norm.FDb.ns_curs.Next
// proceed to next item
inline void atf_norm::_db_ns_curs_Next(_db_ns_curs &curs) {
    curs.index++;
}

// --- atf_norm.FDb.ns_curs.Access
// item access
inline atf_norm::FNs& atf_norm::_db_ns_curs_Access(_db_ns_curs &curs) {
    return ns_qFind(u64(curs.index));
}

// --- atf_norm.FDb.readme_curs.Reset
// cursor points to valid item
inline void atf_norm::_db_readme_curs_Reset(_db_readme_curs &curs, atf_norm::FDb &parent) {
    curs.parent = &parent;
    curs.index = 0;
}

// --- atf_norm.FDb.readme_curs.ValidQ
// cursor points to valid item
inline bool atf_norm::_db_readme_curs_ValidQ(_db_readme_curs &curs) {
    return curs.index < _db.readme_n;
}

// --- atf_norm.FDb.readme_curs.Next
// proceed to next item
inline void atf_norm::_db_readme_curs_Next(_db_readme_curs &curs) {
    curs.index++;
}

// --- atf_norm.FDb.readme_curs.Access
// item access
inline atf_norm::FReadme& atf_norm::_db_readme_curs_Access(_db_readme_curs &curs) {
    return readme_qFind(u64(curs.index));
}
inline atf_norm::FNormcheck::FNormcheck() {
    atf_norm::FNormcheck_Init(*this);
}


// --- atf_norm.FNormcheck.step.Call
// Invoke function by pointer
inline void atf_norm::step_Call(atf_norm::FNormcheck& normcheck) {
    if (normcheck.step) {
        normcheck.step();
    }
}

// --- atf_norm.FNormcheck..Init
// Set all fields to initial values.
inline void atf_norm::FNormcheck_Init(atf_norm::FNormcheck& normcheck) {
    normcheck.nerr = i32(0);
    normcheck.step = NULL;
}
inline atf_norm::FNs::FNs() {
    atf_norm::FNs_Init(*this);
}

inline atf_norm::FNs::~FNs() {
    atf_norm::FNs_Uninit(*this);
}


// --- atf_norm.FNs..Init
// Set all fields to initial values.
inline void atf_norm::FNs_Init(atf_norm::FNs& ns) {
    ns.ind_ns_next = (atf_norm::FNs*)-1; // (atf_norm.FDb.ind_ns) not-in-hash
}
inline atf_norm::FReadme::FReadme() {
}

inline atf_norm::FScriptfile::FScriptfile() {
    atf_norm::FScriptfile_Init(*this);
}

inline atf_norm::FScriptfile::~FScriptfile() {
    atf_norm::FScriptfile_Uninit(*this);
}


// --- atf_norm.FScriptfile..Init
// Set all fields to initial values.
inline void atf_norm::FScriptfile_Init(atf_norm::FScriptfile& scriptfile) {
    scriptfile.ind_scriptfile_next = (atf_norm::FScriptfile*)-1; // (atf_norm.FDb.ind_scriptfile) not-in-hash
}
inline atf_norm::FSsimfile::FSsimfile() {
    atf_norm::FSsimfile_Init(*this);
}

inline atf_norm::FSsimfile::~FSsimfile() {
    atf_norm::FSsimfile_Uninit(*this);
}


// --- atf_norm.FSsimfile..Init
// Set all fields to initial values.
inline void atf_norm::FSsimfile_Init(atf_norm::FSsimfile& ssimfile) {
    ssimfile.ind_ssimfile_next = (atf_norm::FSsimfile*)-1; // (atf_norm.FDb.ind_ssimfile) not-in-hash
}
inline atf_norm::FieldId::FieldId(i32                            in_value)
    : value(in_value)
{
}
inline atf_norm::FieldId::FieldId(atf_norm_FieldIdEnum arg) { this->value = i32(arg); }
inline atf_norm::FieldId::FieldId() {
    atf_norm::FieldId_Init(*this);
}


// --- atf_norm.FieldId.value.GetEnum
// Get value of field as enum type
inline atf_norm_FieldIdEnum atf_norm::value_GetEnum(const atf_norm::FieldId& parent) {
    return atf_norm_FieldIdEnum(parent.value);
}

// --- atf_norm.FieldId.value.SetEnum
// Set value of field from enum type.
inline void atf_norm::value_SetEnum(atf_norm::FieldId& parent, atf_norm_FieldIdEnum rhs) {
    parent.value = i32(rhs);
}

// --- atf_norm.FieldId.value.Cast
inline atf_norm::FieldId::operator atf_norm_FieldIdEnum () const {
    return atf_norm_FieldIdEnum((*this).value);
}

// --- atf_norm.FieldId..Init
// Set all fields to initial values.
inline void atf_norm::FieldId_Init(atf_norm::FieldId& parent) {
    parent.value = i32(-1);
}
inline atf_norm::TableId::TableId(i32                            in_value)
    : value(in_value)
{
}
inline atf_norm::TableId::TableId(atf_norm_TableIdEnum arg) { this->value = i32(arg); }
inline atf_norm::TableId::TableId() {
    atf_norm::TableId_Init(*this);
}


// --- atf_norm.TableId.value.GetEnum
// Get value of field as enum type
inline atf_norm_TableIdEnum atf_norm::value_GetEnum(const atf_norm::TableId& parent) {
    return atf_norm_TableIdEnum(parent.value);
}

// --- atf_norm.TableId.value.SetEnum
// Set value of field from enum type.
inline void atf_norm::value_SetEnum(atf_norm::TableId& parent, atf_norm_TableIdEnum rhs) {
    parent.value = i32(rhs);
}

// --- atf_norm.TableId.value.Cast
inline atf_norm::TableId::operator atf_norm_TableIdEnum () const {
    return atf_norm_TableIdEnum((*this).value);
}

// --- atf_norm.TableId..Init
// Set all fields to initial values.
inline void atf_norm::TableId_Init(atf_norm::TableId& parent) {
    parent.value = i32(-1);
}

inline algo::cstring &algo::operator <<(algo::cstring &str, const atf_norm::trace &row) {// cfmt:atf_norm.trace.String
    atf_norm::trace_Print(const_cast<atf_norm::trace&>(row), str);
    return str;
}

inline algo::cstring &algo::operator <<(algo::cstring &str, const atf_norm::FieldId &row) {// cfmt:atf_norm.FieldId.String
    atf_norm::FieldId_Print(const_cast<atf_norm::FieldId&>(row), str);
    return str;
}

inline algo::cstring &algo::operator <<(algo::cstring &str, const atf_norm::TableId &row) {// cfmt:atf_norm.TableId.String
    atf_norm::TableId_Print(const_cast<atf_norm::TableId&>(row), str);
    return str;
}
