#include "src/qwen3.h"
#include "src/utils.h"

int
main( void )
{
        error_t err = OK;
        // 0.6 billion parameters
        Qwen3Model ptr;

      "vocab_size": 151_936,     # Vocabulary size
      "context_length": 40_960,  # Length originally used during training
      "emb_dim": 1024,           # Embedding dimension
      "n_heads": 16,             # Number of attention heads
      "n_layers": 28,            # Number of layers
      "hidden_dim": 3072,        # Size of intermediate dim in FeedForward
      "head_dim": 128,           # Size of the heads in GQA
      "qk_norm": True,           # Whether to normalize queries & keys in GQA
      "n_kv_groups": 8,          # Key-Value groups for GQA
      "rope_base": 1_000_000.0,  # The base in RoPE's "theta"

        err = qwen3_model_load( "weights.bin", &ptr );
      if ( err != OK ) PANIC( "fail to load weights." );

      goto cleanup;

cleanup:
        qwen3_model_unload( &ptr );
}
