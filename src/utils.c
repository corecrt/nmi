#include "utils.h"

void* __fastcall find_pattern(void* base, size_t size, const char* pattern, size_t pattern_size) {
    unsigned char* data = (unsigned char*)base;
    for (size_t i = 0; i < size - pattern_size; i++) {
        int found = 1;
        for (size_t j = 0; j < pattern_size; j++) {
            if (pattern[j] != '\x00' && data[i + j] != pattern[j]) {
                found = 0;
                break;
            }
        }
        if (found) return (void*)(data + i);
    }
    return NULL;
}

// simple RVA check^^
void* __fastcall resolve_relative_address(void* instruction, uint32_t offset, uint32_t relative_offset) {
    int32_t* addr = (int32_t*)((unsigned char*)instruction + offset);
    return (void*)((unsigned char*)instruction + offset + sizeof(int32_t) + *addr + relative_offset);
}

void* __fastcall find_function(const char* pattern, size_t pattern_size) {
    void* module_base = (void*)helper::map::get_kernel_base():
    size_t module_size = helper::map::get_kernel_size();
    return find_pattern(module_base, module_size, pattern, pattern_size);
}

void* __fastcall hook_function(void* target, void* hook, void** trampoline) {
    //add your own hook here
    //be sure for it to not get detected by KPP either
    return *trampoline;
}
