#define _XOPEN_SOURCE 500

#include <stdbool.h>
#include <stdio.h>
#include "extapp_api.h"

#include "inc/chip8.h"
#include "inc/peripherals.h"
#include "inc/selector.h"

void extapp_main(void) {
    char * rom_filename = select_rom();

    init_display();
    extapp_drawTextSmall("[PENDING] Initializing CHIP-8 arch...", 0, 20, 0x0000, 0xFFFF, false);
    init_cpu();
    extapp_drawTextSmall("[OK] Done!", 0, 20*2, 0x0000, 0xFFFF, false);

    extapp_drawTextSmall("[PENDING] Loading rom...", 0, 20*3, 0x0000, 0xFFFF, false);

    int status = load_rom(rom_filename);

    if (status != 0) {
        extapp_drawTextSmall("[FAILED] Error while loading rom", 0, 20*4, 0x0000, 0xFFFF, false);
        waitForKeyPressed();
        return;
    }

    extapp_drawTextSmall("[OK] Rom loaded successfully!", 0, 20*4, 0x0000, 0xFFFF, false);

    char * memory_str;
    for (int i = 0; i < 12; ++i) {
        sprintf(memory_str, "%x", memory[0x200+i]);
        extapp_drawTextSmall(memory_str, 15*i, 20*6, 0x0000, 0xFFFF, false);
    }

    while (1) {
        if (draw_flag) {
            draw(display, rom_filename);
        }
        
        emulate_cycle();
        kbd_handler();

        if (should_quit()) {
            break;
        }
    }

    return;
}
