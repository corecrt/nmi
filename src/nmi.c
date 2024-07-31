#include "ntoskrnl.h"
#include "utils.h"
#include "nmi.h"

static void* ki_check_for_freeze_execution;
static void* ki_check_for_freeze_execution_trampoline;
static void* hal_send_nmi;
static void* ki_process_nmi;
static void* hal_send_nmi_trampoline;
static void* ki_process_nmi_trampoline;

static volatile bool block_nmi = false; // flag to block NMI

static char __fastcall ki_check_for_freeze_execution_hook(uint64_t a1) {
    if ((a1 >= (uintptr_t)hal_send_nmi && a1 < ((uintptr_t)hal_send_nmi + 0x1000))) {
        return 1;
    }
    return ((char (*)(uint64_t))(ki_check_for_freeze_execution_trampoline))(a1);
}

static void __fastcall hal_send_nmi_hook(void) {
    if (block_nmi) {
        return;
    }
    ((void (*)(void))(hal_send_nmi_trampoline))();
}

static void __fastcall ki_process_nmi_hook(uint64_t a1, uint64_t a2) {
    uintptr_t driver_base = (uintptr_t)helper::map::get_driver_base();
    uintptr_t driver_end = driver_base + helper::map::get_driver_size();
    uintptr_t rip = (uintptr_t)trap_frame->Rip;

    if (block_nmi && (rip >= driver_base && rip < driver_end)) {
        block_nmi = true;
        return;
    }

    ((void (*)(_KTRAP_FRAME *, _KEXCEPTION_FRAME *))(ki_process_nmi_trampoline))(trap_frame, kernel_exception_frame);
}

void __fastcall monitor_nmi() {
    const char* pattern_ki_check_for_freeze_execution = "\xE8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\xE9";
    ki_check_for_freeze_execution = find_function(pattern_ki_check_for_freeze_execution, sizeof(pattern_ki_check_for_freeze_execution) - 1);
    if (ki_check_for_freeze_execution) {
        hook_function(ki_check_for_freeze_execution, ki_check_for_freeze_execution_hook, &ki_check_for_freeze_execution_trampoline);
    }

    const char* pattern_hal_send_nmi = "\xE8\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0";
    hal_send_nmi = find_function(pattern_hal_send_nmi, sizeof(pattern_hal_send_nmi) - 1);
    if (hal_send_nmi) {
        hook_function(hal_send_nmi, hal_send_nmi_hook, &hal_send_nmi_trampoline);
    }

    const char* pattern_ki_process_nmi = "\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x68\x10\x48\x89\x70\x18\x48\x89\x78\x20\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00";
    ki_process_nmi = find_function(pattern_ki_process_nmi, sizeof(pattern_ki_process_nmi) - 1);
    if (ki_process_nmi) {
        hook_function(ki_process_nmi, ki_process_nmi_hook, &ki_process_nmi_trampoline);
    }
}
