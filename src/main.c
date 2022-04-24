#include "wasm4.h"
#include "../game.c"

#define COLOR0 0xFF181818
#define COLOR1 0xFF183018
#define COLOR2 0xFF189018
#define COLOR3 0xFF31A6FF

#define SCREEN_HEIGHT ROWS * 10

uint8_t prev_state = 0;

void start()
{
    PALETTE[0] = COLOR0;
    PALETTE[1] = COLOR1;
    PALETTE[2] = COLOR2;
    PALETTE[3] = COLOR3;
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

    game_update(1.0f/30.0f);
    game_render();
}

void platform_panic(const char *file_path, int32_t line, const char *message){}

u32 get_color(u32 color)
{
    switch (color)
    {
    case COLOR0: return 0x001;
    case COLOR1: return 0x002;
    case COLOR2: return 0x003;
    default: return 0x004;
    }
}

void platform_draw_text(i32 x, i32 y, const char *message, u32 size, u32 color, Align align)
{
    *DRAW_COLORS = get_color(color);
    text(message, x/10, y/5 + SCREEN_HEIGHT);
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