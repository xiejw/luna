#pragma once

#include <stdio.h>
#include <stdlib.h>  // exit

#define PANIC( msg ) _PANIC_IMPL( msg, __FILE__, __LINE__ )

#define _PANIC_IMPL( msg, file, lineno )                        \
        do {                                                    \
                printf( "=== PANIC %s:%d===\n", file, lineno ); \
                printf( msg "\n" );                             \
                fflush( stdout );                               \
                exit( -1 );                                     \
        } while ( 0 )
