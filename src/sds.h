#pragma once

#include <assert.h>
#include <stdarg.h>  // va_list
#include <stdlib.h>

#include "types.h"

// === --- Design ---------------------------------------------------------- ===
//
// +----+------+------
// |len |alloc |buf
// +----+------+------
//             |
//  -2   -1     `-- sds_t
//
// - Fast assessing:   sds_t is basically the c string ('\0' terminated).
// - Dynamic growth:   If the space is not enough, buf will be expanded to hold
//                     more elements in future.
// - Fast CatPrintf:   Use remaing space to printf whenever possible.
//
// === --- Invariance and Caveats ------------------------------------------ ===
// - The following should be true:
//
//   strlen(sds) = sds_len(sds) <= sds_cap(sds).
//
// - So to use sds_set_len, sds_set_cap, sds_inc_len, caller must set the \0'
//   correctly and maintain the invariance above.

// === --- APIs ------------------------------------------------------------ ===
typedef char *sds_t;

struct sds_head {
        size_t len;
        size_t alloc;
};

// Allocator and free fns.
//
// - sds_new             copies a NULL-able str and allocates enough space for
// that
// - sds_empty           is basically sds_new("");
// - sds_empty_with_cap  good for creating a sds with known cap.
// - sds_dup             dups a sds.
sds_t sds_new( const char *init );
sds_t sds_empty( void );
sds_t sds_empty_with_cap( size_t cap );
sds_t sds_dup( const sds_t s );
void  sds_free( sds_t s );

#define sds_clear( s ) _sds_clear( s )  // clear to 0-len str
#define sds_len( s )   _sds_len( s )
#define sds_cap( s )   _sds_cap( s )
#define sds_avail( s ) _sds_avail( s )
#define sds_set_len( s, l ) \
        _sds_set_len( ( s ), ( l ) )  // caller checks invariance
#define sds_set_cap( s, l ) \
        _sds_set_cap( ( s ), ( l ) )  // caller checks invariance
#define sds_inc_len( s, inc ) \
        _sds_inc_len( ( s ), ( inc ) )  // caller checks invariance

// Allocate enough space (typically larger) for the new_len.
//
// - NOTE: All ps must be address of sds_t
void sds_reserve( _MUT_ sds_t *ps, size_t new_len );

// Concatenate at the end of sds.
//
// - NOTE: All ps must be address of sds_t
void sds_cat( _MUT_ sds_t *ps, const char *t );
void sds_cat_sds( _MUT_ sds_t *ps, const sds_t t );
void sds_cat_len( _MUT_ sds_t *ps, const void *t, size_t len );
void sds_cat_vprintf( _MUT_ sds_t *ps, const char *fmt, va_list ap );
void sds_cat_printf( _MUT_ sds_t *ps, const char *fmt, ... );

// Copy t to sds (overwriting existing content).
//
// - NOTE: All ps must be address of sds_t
void sds_cpy_len( _MUT_ sds_t *ps, const char *t, size_t len );
void sds_cpy( _MUT_ sds_t *ps, const char *t );

// Compare two sds strings s1 and s2 with memcmp().
//
// Return value:
//
//     positive if s1 > s2.
//     negative if s1 < s2.
//     0 if s1 and s2 are exactly the same binary string.
//
// If two strings share exactly the same prefix, but one of the two has
// additional characters, the longer string is considered to be greater than
// the smaller one.
//
int sds_cmp( const sds_t s1, const sds_t s2 );

// === --- Private Static Inline Fns --------------------------------------- ===

#define _SDS_HEAD( s ) \
        ( (struct sds_head *)( ( s ) - ( sizeof( struct sds_head ) ) ) )
static inline size_t
_sds_len( const sds_t s )
{
        return s == NULL ? 0 : _SDS_HEAD( s )->len;
}
static inline size_t
_sds_cap( const sds_t s )
{
        return s == NULL ? 0 : _SDS_HEAD( s )->alloc;
}
static inline size_t
_sds_avail( const sds_t s )
{
        if ( s == NULL ) return 0;
        struct sds_head *p = _SDS_HEAD( s );
        return p->alloc - p->len;
}
static inline void
_sds_set_len( const sds_t s, size_t newlen )
{
        assert( s != NULL );
        _SDS_HEAD( s )->len = newlen;
}
static inline void
_sds_inc_len( const sds_t s, size_t inc )
{
        assert( s != NULL );
        _SDS_HEAD( s )->len += inc;
}
static inline void
_sds_set_cap( const sds_t s, size_t newcap )
{
        assert( s != NULL );
        _SDS_HEAD( s )->alloc = newcap;
}
static inline void
_sds_clear( sds_t s )
{
        assert( s != NULL );
        sds_set_len( s, 0 );
        s[0] = '\0';
}
