// vim: ft=cpp
#pragma once

#include <zion/zion.h>

struct Qwen3Config {
        int n_layer;
};

struct Qwen3Model {
        Qwen3Config cfg;

      public:
        ~Qwen3Model( ) = default;

      public:
        zion::Expected<void> load_model( const char *filename );
};
