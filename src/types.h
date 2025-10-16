#pragma once

// Primitive Types
#include <stdint.h>

typedef uint64_t      u64;
typedef int64_t       i64;
typedef uint32_t      u32;
typedef int32_t       i32;
typedef uint8_t       u8;
typedef float         f32;
typedef double        f64;
typedef unsigned char byte;

// Function Parameter Annotations
#define _MUT_       // The field might be mutated if new address is allocated
#define _OUT_       // The field will be set with the output
#define _INOUT_     // The field will be passed in and then be set as output
#define _MOVED_IN_  // The ownership is moved into the method
#define _NULLABLE_  // The field is Nullable

typedef int error_t;

#define OK  0
#define ERR -1
