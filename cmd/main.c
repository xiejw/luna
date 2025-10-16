#include "src/utils.h"
#include "src/weights.h"

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
