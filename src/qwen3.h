// vim: ft=cpp
#pragma once

#include <zion/zion.h>

<<<<<<< HEAD
typedef struct Qwen3Config {
        int vocab_size;      // Vocabulary size
        int context_length;  // Length originally used during training
        int emb_dim;         // Embedding dimension
        int n_heads;         // Number of attention heads
        int n_layers;        // Number of layers
        int hidden_dim;      // Size of intermediate dim in FeedForward
        int head_dim;        // Size of the heads in GQA
        int qk_norm;         // Whether to normalize queries & keys (bool)
        int n_kv_groups;     // Key-Value groups for GQA
        f32 rope_base;       // The base in RoPE's "theta"
} Qwen3Config;
=======
struct Qwen3Config {
        int n_layer;
};
>>>>>>> 14e8972076dfe7834818370011719b94188f72f6

struct Qwen3Model {
        Qwen3Config cfg;

      public:
        ~Qwen3Model( ) = default;

      public:
        zion::Expected<void> load_model( const char *filename );
};
