#include "wasm4.h"
#include "../game.c"

#define COLOR0 CELL1_COLOR
#define COLOR1 CELL2_COLOR
#define COLOR2 SNAKE_BODY_COLOR
#define COLOR3 EGG_BODY_COLOR

#define SCREEN_HEIGHT (ROWS * SCREEN_SIZE / COLS) 
#define FONT_SIZE 8

#define DOUBLE_SCREEN
#if defined(DOUBLE_SCREEN) && (SCREEN_SIZE - SCREEN_HEIGHT > 20)
#define BOTTON_SCREEN (SCREEN_SIZE - SCREEN_HEIGHT)
#endif

u8 prev_state = 0;

u32 rgb_color(u32 color)
{
    const int r = ((color>>(0*8))&0xFF) << (2*8);
    const int g = ((color>>(1*8))&0xFF) << (1*8);
    const int b = ((color>>(2*8))&0xFF) << (0*8);
    return r  + g  + b;
}

void start()
{
    PALETTE[0] = rgb_color(COLOR0);
    PALETTE[1] = rgb_color(COLOR1);
    PALETTE[2] = rgb_color(COLOR2);
    PALETTE[3] = rgb_color(COLOR3);
    game_init(SCREEN_SIZE * 10, SCREEN_HEIGHT * 10);
}

void update () {
    const u8 just_pressed = *GAMEPAD1 & (*GAMEPAD1 ^ prev_state);
    
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

void platform_panic(const char *file_path, int line, const char *message)
{
    tracef("%s:%d: GAME ASSERTION FAILED: %s\n", file_path, line, message);
}

u16 get_color(u32 color)
{
    switch (color)
    {
    case COLOR0: return 0x001;
    case COLOR1: return 0x002;
    case COLOR2: return 0x003;
    case COLOR3: default: return 0x004;
    }
}

i32 strlen(const char *str) {
    i32 len = 0;
    while (*str != 0){
        len++;
        str++;
    }
    return len;
}

i32 align_offset(Align align, const char *message)
{
    switch (align)
    {
    case ALIGN_CENTER: return strlen(message)/2;
    case ALIGN_RIGHT: return strlen(message);
    case ALIGN_LEFT: default: return 0;
    }
}

void platform_fill_text(i32 x, i32 y, const char *message, u32 size, u32 color, Align align)
{
    (void)size;
    *DRAW_COLORS = get_color(color);
    x = x/10 - align_offset(align, message) * FONT_SIZE;

#ifdef BOTTON_SCREEN
    y = y/10 * BOTTON_SCREEN / SCREEN_HEIGHT + SCREEN_HEIGHT;
#else
    y = y/10;
#endif

    text(message, x, y);
}

void platform_log(const char *message)
{
    tracef("[LOG] %s", message);
}

void platform_fill_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
    *DRAW_COLORS = get_color(color);

    x /= 10;
    y /= 10;
    w /= 10;
    h = (y + h/10 > SCREEN_HEIGHT) ? SCREEN_HEIGHT - y : h/10;

    rect(x, y, w, h);
}

void platform_stroke_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
    *DRAW_COLORS = (get_color(color)<<4) & 0xff;

    x /= 10;
    y /= 10;
    w /= 10;
    h = (y + h/10 > SCREEN_HEIGHT) ? SCREEN_HEIGHT - y : h/10;

    rect(x, y, w, h);
}

void platform_stroke_line(i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
{
    *DRAW_COLORS = get_color(color);
    line(x1/10, y1/10, x2/10, y2/10);
}