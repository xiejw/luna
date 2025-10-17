#include "src/qwen3.h"
#include "src/utils.h"

int
main( void )
{
        error_t    err = OK;
        Qwen3Model ptr;
        err = qwen3_model_load( "weights.bin", &ptr );
        if ( err != OK ) PANIC( "fail to load weights." );

        goto cleanup;

cleanup:
        qwen3_model_unload( &ptr );
}
