#include "wasm4.h"
#include "../game.c"

#define COLOR0 0xFF181818
#define COLOR1 0xFF183018
#define COLOR2 0xFF189018
#define COLOR3 0xFF31A6FF

#define SCREEN_HEIGHT (ROWS * SCREEN_SIZE / COLS)
#define FONT_SIZE 8

#define DOUBLE_SCREEN
#ifdef DOUBLE_SCREEN
#define BOTTON_SCREEN (SCREEN_SIZE - SCREEN_HEIGHT)
#endif

uint8_t prev_state = 0;

u32 rgb_color(u32 color)
{
    const r = ((color>>(0*8))&0xFF) << (2*8);
    const g = ((color>>(1*8))&0xFF) << (1*8);
    const b = ((color>>(2*8))&0xFF) << (0*8);
    return r  + g  + b;
}

void start()
{
    PALETTE[0] = rgb_color(COLOR0);
    PALETTE[1] = rgb_color(COLOR1);
    PALETTE[2] = rgb_color(COLOR2);
    PALETTE[3] = rgb_color(COLOR3);
    game_init(SCREEN_SIZE, SCREEN_HEIGHT);
}

void update () {
    const uint8_t just_pressed = *GAMEPAD1 & (*GAMEPAD1 ^ prev_state);
    
    if (just_pressed & BUTTON_DOWN) {
        game_keydown(KEY_DOWN);
    }
    if (just_pressed & BUTTON_UP) {
        game_keydown(KEY_UP);
    }
    if (just_pressed & BUTTON_LEFT) {
        game_keydown(KEY_LEFT);
    }
    if (just_pressed & BUTTON_RIGHT) {
        game_keydown(KEY_RIGHT);
    }
    if (just_pressed & BUTTON_2) {
        game_keydown(KEY_ACCEPT);
    }
    
    prev_state = *GAMEPAD1;

    game_update(1.0f/60.0f);
    game_render();
}

void platform_panic(const char *file_path, int32_t line, const char *message){}

uint16_t get_color(u32 color)
{
    switch (color)
    {
    case COLOR0: return 0x001;
    case COLOR1: return 0x002;
    case COLOR2: return 0x003;
    default: return 0x004;
    }
}

i32 align_offset(Align align, const char *message)
{
    switch (align)
    {
    case ALIGN_CENTER: return strlen(message)/2;
    case ALIGN_RIGHT: return strlen(message);
    default: return 0;
    }
}

void platform_fill_text(i32 x, i32 y, const char *message, u32 size, u32 color, Align align)
{
    *DRAW_COLORS = get_color(color);
    i32 offset = align_offset(align, message) * FONT_SIZE;

#ifdef DOUBLE_SCREEN
    if(BOTTON_SCREEN >= 20)
    {
        y = y * BOTTON_SCREEN / SCREEN_HEIGHT + SCREEN_HEIGHT;
    }
#endif

    text(message, x - offset, y);
}

void platform_log(const char *message)
{
    trace(message);
}

void platform_fill_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
    *DRAW_COLORS = get_color(color);
    rect(x, y, w, h);
}

void platform_stroke_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
    *DRAW_COLORS = (get_color(color)<<4) & 0xff;
    rect(x, y, w, h);
}

void platform_stroke_line(i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
{
    *DRAW_COLORS = get_color(color);
    line(x1, y1, x2, y2);
}