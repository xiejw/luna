#pragma once

#include "types.h"

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

typedef struct Qwen3Model {
        Qwen3Config cfg;
} Qwen3Model;

error_t qwen3_model_load( const char *filename, _INOUT_ Qwen3Model *pout );
void    qwen3_model_unload( Qwen3Model *p );
