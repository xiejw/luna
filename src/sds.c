#include "sds.h"

#include <stdio.h>  // vsnprintf
#include <stdlib.h>
#include <string.h>  // memcpy/memset

// === --- Implementation -------------------------------------------------- ===
//
static const int SDS_DEFAULT_ALLOCATE_SPACE = 16;

static inline sds_t _sdsRaw( const void *init, size_t len, size_t cap );

sds_t
sds_empty_with_cap( size_t cap )
{
        sds_t            s    = _sdsRaw( NULL, 0, cap );
        struct sds_head *phdr = _SDS_HEAD( s );
        phdr->len             = 0;
        s[0]                  = '\0';
        return s;
}

sds_t
sds_new( const char *init )
{
        size_t initlen = ( init == NULL ) ? 0 : strlen( init );
        size_t cap     = initlen;

        sds_t            s    = _sdsRaw( init, initlen, cap );
        struct sds_head *phdr = _SDS_HEAD( s );
        phdr->len             = initlen;
        s[initlen]            = '\0';
        return s;
}

sds_t
sds_empty( void )
{
        return sds_new( "" );
}

sds_t
sds_dup( const sds_t s )
{
        size_t len   = sds_len( s );
        sds_t  new_s = _sdsRaw( s, len, sds_cap( s ) );

        struct sds_head *phdr = _SDS_HEAD( new_s );
        phdr->len             = len;
        new_s[len]            = '\0';
        return new_s;
}

void
sds_free( sds_t s )
{
        if ( s == NULL ) return;
        free( (void *)_SDS_HEAD( s ) );
}

void
sds_reserve( sds_t *s, size_t new_len )
{
        size_t cap = sds_cap( *s );
        if ( cap >= new_len ) return;

        new_len *= 2;

        size_t hdrlen = sizeof( struct sds_head );
        void  *buf    = _SDS_HEAD( *s );
        buf           = realloc( buf, hdrlen + new_len + 1 );
        if ( buf == NULL ) {
                *s = NULL;
                return;
        }
        *s = (sds_t)buf + hdrlen;
        sds_set_cap( *s, new_len );
}

void
sds_cat_len( sds_t *s, const void *t, size_t len )
{
        size_t curlen = sds_len( *s );
        size_t newlen = curlen + len;
        sds_reserve( s, newlen );
        if ( *s == NULL ) return;
        memcpy( ( *s ) + curlen, t, len );
        sds_set_len( *s, newlen );
        ( *s )[newlen] = '\0';
}

void
sds_cat( sds_t *s, const char *t )
{
        sds_cat_len( s, t, strlen( t ) );
}

void
sds_cat_sds( sds_t *s, const sds_t t )
{
        sds_cat_len( s, t, sds_len( t ) );
}

void
sds_cat_printf( sds_t *s, const char *fmt, ... )
{
        va_list ap;
        va_start( ap, fmt );
        sds_cat_vprintf( s, fmt, ap );
        va_end( ap );
}

void
sds_cat_vprintf( sds_t *s, const char *fmt, va_list ap )
{
        va_list cpy;
        char    staticbuf[1024], *buf = staticbuf;
        size_t  buflen = strlen( fmt ) * 2;

        assert( buflen >= 2 );
        // fast path first. Use the remaining area if possible for speed.
        {
                size_t avail = sds_avail( *s );
                if ( buflen <= avail ) {
                        size_t cur_len = sds_len( *s );

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
                        char *buf       = ( *s ) + cur_len;
                        buf[buflen - 2] = '\0';
                        va_copy( cpy, ap );
                        vsnprintf( buf, buflen, fmt, cpy );
                        va_end( cpy );
                        if ( buf[buflen - 2] == '\0' ) {
                                size_t inc_len = strlen( buf );
                                sds_set_len( *s, cur_len + inc_len );
                                return;
                        }
                        // fall through
#pragma clang diagnostic pop
                }
        }

        /* We try to start using a static buffer for speed.
         * If not possible we revert to heap allocation. */
        if ( buflen > sizeof( staticbuf ) ) {
                buf = malloc( buflen );
                if ( buf == NULL ) {
                        *s = NULL;
                        return;
                }
        } else {
                buflen = sizeof( staticbuf );
        }

        /* Try with buffers two times bigger every time we fail to
         * fit the string in the current buffer size. */
        while ( 1 ) {
                buf[buflen - 2] = '\0';
                va_copy( cpy, ap );
                vsnprintf( buf, buflen, fmt, cpy );
                va_end( cpy );
                if ( buf[buflen - 2] != '\0' ) {
                        if ( buf != staticbuf ) free( buf );
                        buflen *= 2;
                        buf = malloc( buflen );
                        if ( buf == NULL ) {
                                *s = NULL;
                                return;
                        }
                        continue;
                }
                break;
        }

        /* Finally concat the obtained string to the SDS string and return it.
         */
        sds_cat( s, buf );
        if ( buf != staticbuf ) free( buf );
}

void
sds_cpy_len( sds_t *s, const char *t, size_t len )
{
        sds_reserve( s, len );
        if ( *s == NULL ) return;
        memcpy( *s, t, len );
        ( *s )[len] = '\0';
        sds_set_len( *s, len );
        return;
}
void
sds_cpy( sds_t *s, const char *t )
{
        sds_cpy_len( s, t, strlen( t ) );
}

int
sds_cmp( const sds_t s1, const sds_t s2 )
{
        size_t l1, l2, minlen;
        int    cmp;

        l1     = sds_len( s1 );
        l2     = sds_len( s2 );
        minlen = ( l1 < l2 ) ? l1 : l2;
        cmp    = memcmp( s1, s2, minlen );
        if ( cmp == 0 ) return l1 > l2 ? 1 : ( l1 < l2 ? -1 : 0 );
        return cmp;
}

// create a raw sds without filling len.
//
// - the space is at least as large as `cap`. cap will be set.
// - allocate `cap` size but only initialize `len` size.
// - the final `\0` is not set.
sds_t
_sdsRaw( const void *init, size_t len, size_t cap )
{
        cap =
            cap > SDS_DEFAULT_ALLOCATE_SPACE ? cap : SDS_DEFAULT_ALLOCATE_SPACE;
        size_t hdrlen = sizeof( struct sds_head );
        void  *buf    = malloc( hdrlen + cap + 1 );
        if ( buf == NULL ) return NULL;

        sds_t s                           = (sds_t)buf + hdrlen;
        ( (struct sds_head *)buf )->alloc = cap;

        if ( len ) {
                if ( init == NULL )
                        memset( s, 0, len );
                else
                        memcpy( s, init, len );
        }

        return s;
}
