#pragma once

#include "types.h"

typedef struct Qwen3Config {
        int n_layer;
} Qwen3Config;

typedef struct Qwen3Model {
        Qwen3Config cfg;
} Qwen3Model;

error_t qwen3_model_load( const char *filename, _INOUT_ Qwen3Model *pout );
void    qwen3_model_unload( Qwen3Model *p );
