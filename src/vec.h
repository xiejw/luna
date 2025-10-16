#pragma once

#include <assert.h>  // assert
#include <stddef.h>  // ptrdiff_t
#include <stdlib.h>  // free
#include <string.h>  // memcpy

#include "types.h"

// === --- Design (Inspired by sds from redis) ---------------------------------
//
// +----+----+------
// |size|cap |buf
// +----+----+------
//           |
//  -2   -1   `-- ptr
//
// - Fast assessing:       Instead of a field lookup, `ptr` points to the buf
//                         head so ptr[x] can be used. With proper vec_reserve,
//                         the address is safe.
//
// - Lazy initialization:  vec_push or vec_reserve allocate the real memory.
//                         The vec must be initialized as NULL (vec_new).
//
// - Dynamic growth:       If the space is not enough, buf will be expanded to
//                         hold more elements in future.
//
// - Iteration:            for(size_t i = 0; i < vec_size(v); i++) { v[i] };
//
// - Fast modification:    Reserve a proper cap. Sets all values directly.
//                         Calls vec_set_size to corrects the size. Use with
//                         caution.
//
// === --- Ownership -----------------------------------------------------------
//
// 1. Container `vec` does not own any elements. So caller should free them.
// 2. Must call vec_free to release the memory, used by 'vec', on heap.
//
// === --- Contracts and Limitations -------------------------------------------
//
// 1. As the buf might be re-allocated (for growth), pass &vec for
//    modificaitons, e.g., vec_reserve, vec_push, vec_extend.
// 2. When use vec_set_size, ensure the new values are initialized or old values
//    are freed properly.
//

// === --- APIs ----------------------------------------------------------------
//
// Conventions
// - vec is the plain pointer variable created by vec_new.
// - pvec is the address of vec.

#define vec_t( type ) type *

#define vec_new( )      NULL
#define vec_free( vec ) _VEC_FREE_IMPL( vec )

#define vec_size( vec )     ( ( vec ) ? ( (size_t *)( vec ) )[-2] : (size_t)0 )
#define vec_cap( vec )      ( ( vec ) ? ( (size_t *)( vec ) )[-1] : (size_t)0 )
#define vec_is_empty( vec ) ( vec_size( ( vec ) ) == 0 )

/* All following APIs return error_t to check errors. */
#define vec_set_size( vec, new_s ) _VEC_SET_SIZE_IMPL( vec, new_s )
#define vec_reserve( pvec, count ) _VEC_RESERVE_IMPL( pvec, count )
#define vec_push( pvec, v )        _VEC_PUSH_BACK_IMPL( pvec, v )
#define vec_extend( pdst, src )    _VEC_EXTEND_IMPL( pdst, src )

/* Return last item. Must be non-empty. */
#define vec_pop( vec ) _VEC_POP_BACK_IMPL( vec )

// === --- Private Macros and Prototypes ---------------------------------------
//

#define VEC_INIT_BUF_SIZE 16

extern error_t _vec_reserve( _MUT_ size_t **vec, size_t new_cap,
                             size_t unit_size );

static inline error_t _vec_grow( _MUT_ size_t **vec, size_t unit_size );

static inline error_t _vec_extend( _MUT_ size_t **pdst, size_t dst_size,
                                   size_t unit_size, size_t *src,
                                   size_t src_size );

#define _VEC_FREE_IMPL( vec )                               \
        do {                                                \
                if ( ( vec ) != NULL ) {                    \
                        free( &( (size_t *)( vec ) )[-2] ); \
                }                                           \
        } while ( 0 )

#define _VEC_SET_SIZE_IMPL( vec, new_s ) \
        ( ( vec ) ? ( ( (size_t *)vec )[-2] = ( new_s ), OK ) : ENOTEXIST )

#define _VEC_RESERVE_IMPL( pvec, count )          \
        _vec_reserve( (size_t **)( pvec ), count, \
                      /*unit_size=*/sizeof( **( pvec ) ) )

#define _VEC_PUSH_BACK_IMPL( pvec, v )                                  \
        ( _vec_grow( (size_t **)( pvec ),                               \
                     /*unit_size=*/sizeof( **( pvec ) ) ) ||            \
          ( ( ( *( pvec ) )[( (size_t *)( *( pvec ) ) )[-2]] = ( v ) ), \
            ( (size_t *)( *( pvec ) ) )[-2]++, OK ) )

#define _VEC_EXTEND_IMPL( pdst, src )                                       \
        _vec_extend( (size_t **)( pdst ), vec_size( *( pdst ) ),            \
                     /*unit_size=*/sizeof( **( pdst ) ), (size_t *)( src ), \
                     vec_size( ( src ) ) )

#define _VEC_POP_BACK_IMPL( vec )    \
        ( assert( ( vec ) != NULL ), \
          *( ( vec ) + --( ( (size_t *)( vec ) )[-2] ) ) )

error_t
_vec_grow( _MUT_ size_t **vec, size_t unit_size )
{
        if ( !*vec ) {
                return _vec_reserve( vec, VEC_INIT_BUF_SIZE, unit_size );
        }

        const size_t cap  = ( *vec )[-1];
        const size_t size = ( *vec )[-2];
        assert( size <= cap );
        if ( cap != size ) return OK;
        return _vec_reserve( vec, 2 * cap, unit_size );
}

error_t
_vec_extend( _MUT_ size_t **pdst, size_t dst_size, size_t unit_size,
             size_t *src, size_t src_size )
{
        const size_t new_size = dst_size + src_size;
        error_t      err      = _vec_reserve( pdst, new_size, unit_size );
        if ( err ) return err;
        memcpy( ( (char *)( *pdst ) ) + unit_size * dst_size, src,
                unit_size * src_size );
        ( *pdst )[-2] = new_size;
        return OK;
}
