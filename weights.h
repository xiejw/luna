#pragma once

#define _OUT_

typedef int error_t

#define OK 0
#define ERR -1

error_t qwen3_weights_load(const char* filename, _OUT_ void** pout);
void qwen3_weights_unload(void*);
