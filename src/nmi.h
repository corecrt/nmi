#pragma once

#include <stdint.h>

void __fastcall monitor_nmi(void);
void __fastcall ki_check_for_freeze_execution_hook(uint64_t a1);
void __fastcall hal_send_nmi_hook(void);
void __fastcall ki_process_nmi_hook(void);
