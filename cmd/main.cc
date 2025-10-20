#include "src/qwen3.h"

int
main( void )
{
        Qwen3Model model{ };
        auto       rc = model.load_model( "weights.bin" );
        if ( !rc ) PANIC( "fail to load weights." );

        INFO( "model loaded." );
}
