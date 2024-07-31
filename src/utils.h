#pragma once

#include <stdint.h>

void* __fastcall find_pattern(void* base, size_t size, const char* pattern, size_t pattern_size);
void* __fastcall resolve_relative_address(void* instruction, uint32_t offset, uint32_t relative_offset);
void* __fastcall find_function(const char* pattern, size_t pattern_size);
void* __fastcall hook_function(void* target, void* hook, void** trampoline);
