#include "vec.h"

/* === --- Implementation --------------------------------------------------- */

error_t
_vec_reserve( size_t **vec, size_t new_cap, size_t unit_size )
{
        const size_t head_size = 2;

        const size_t new_s = (new_cap)*unit_size + head_size * sizeof( size_t );

        if ( *vec ) {
                size_t *new_p =
                    realloc( &( *vec )[-(ptrdiff_t)head_size], new_s );
                if ( new_p == NULL ) return EMALLOC;
                new_p[head_size - 1] = new_cap;  // other head fields unchanged.
                *vec                 = (void *)( new_p + head_size );
        } else {
                size_t *p = malloc( new_s );
                if ( p == NULL ) return EMALLOC;
                p[head_size - 2] = 0;
                p[head_size - 1] = new_cap;
                *vec             = (void *)( p + head_size );
        }
        return OK;
}
