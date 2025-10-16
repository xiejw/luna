#pragma once

#include "types.h"

error_t qwen3_weights_load( const char *filename, _OUT_ void **pout );
void    qwen3_weights_unload( void * );
