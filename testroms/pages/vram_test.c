#include "../system.h"
#include "../memory_map.h"
#include "../page.h"

#include "../util.h"
#include "../tilemap.h"
#include "../igs023.h"
#include "../gui.h"
#include "../color.h"

static u16 attrib = 0;

static inline void write_64(void *ptr)
{
    __asm volatile (
        "movem.l %%d0-%%d7/%%a0-%%a7, (%0)" 
        : // No outputs
        : "a" (ptr)   // Input: pointer to memory
        : "memory"   // Clobbers: tell GCC these registers changed
    );
}

static void init()
{
    igs023_init();
    text_reset();
    set_default_palette();

    IGS023_FG_X_SET(15);
    IGS023_FG_Y_SET(8);
    IGS023_BG_CTRL_SET(0xffff);

    memset(VRAM->bg, 0, sizeof(VRAM->bg));
}

static void vram_update()
{
    igs023_wait_vblank();

    //while(IGS023_SCANLINE_RAW() != 0) {};
    while(IGS023_SCANLINE_RAW() != 50) write_64(&VRAM->fg[0].code);
    while(IGS023_SCANLINE_RAW() < 100) write_64(&VRAM->bg[0].attrib);
}

PAGE_REGISTER(vram_test, init, vram_update, NULL);

