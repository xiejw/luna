#pragma once

#include "types.h"

void ops_matmul( f32 *restrict out, f32 *restrict a, f32 *restrict b, int m,
                 int n, int k );
