#include <stdio.h>
#include <stdlib.h>  // exit

#include "src/weights.h"

#define PANIC( msg )                         \
        do {                                 \
                printf( "=== panic ===\n" ); \
                printf( msg "\n" );          \
                fflush( stdout );            \
                exit( -1 );                  \
        } while ( 0 )

int
main( )
{
        error_t err = OK;
        void   *ptr = NULL;
        err         = qwen3_weights_load( "weights.bin", &ptr );
        if ( err != OK ) PANIC( "fail to load weights." );

cleanup:
        qwen3_weights_unload( ptr );
}
