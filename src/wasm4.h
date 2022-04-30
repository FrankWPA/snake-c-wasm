//
// WASM-4: https://wasm4.org/docs

#pragma once

#include "../game.h"

typedef unsigned short u16;
typedef short i16;

#define WASM_EXPORT(name) __attribute__((export_name(name)))
#define WASM_IMPORT(name) __attribute__((import_name(name)))

WASM_EXPORT("start") void start ();
WASM_EXPORT("update") void update ();

// ┌───────────────────────────────────────────────────────────────────────────┐
// │                                                                           │
// │ Platform Constants                                                        │
// │                                                                           │
// └───────────────────────────────────────────────────────────────────────────┘

#define SCREEN_SIZE 160

// ┌───────────────────────────────────────────────────────────────────────────┐
// │                                                                           │
// │ Memory Addresses                                                          │
// │                                                                           │
// └───────────────────────────────────────────────────────────────────────────┘

#define PALETTE ((u32*)0x04)
#define DRAW_COLORS ((u16*)0x14)
#define GAMEPAD1 ((const u8*)0x16)
#define GAMEPAD2 ((const u8*)0x17)
#define GAMEPAD3 ((const u8*)0x18)
#define GAMEPAD4 ((const u8*)0x19)
#define MOUSE_X ((const i16*)0x1a)
#define MOUSE_Y ((const i16*)0x1c)
#define MOUSE_BUTTONS ((const u8*)0x1e)
#define SYSTEM_FLAGS ((u8*)0x1f)
#define FRAMEBUFFER ((u8*)0xa0)

#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_LEFT 16
#define BUTTON_RIGHT 32
#define BUTTON_UP 64
#define BUTTON_DOWN 128

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4

#define SYSTEM_PRESERVE_FRAMEBUFFER 1
#define SYSTEM_HIDE_GAMEPAD_OVERLAY 2

// ┌───────────────────────────────────────────────────────────────────────────┐
// │                                                                           │
// │ Drawing Functions                                                         │
// │                                                                           │
// └───────────────────────────────────────────────────────────────────────────┘

/** Copies pixels to the framebuffer. */
WASM_IMPORT("blit")
void blit (const u8* data, i32 x, i32 y, u32 width, u32 height, u32 flags);

/** Copies a subregion within a larger sprite atlas to the framebuffer. */
WASM_IMPORT("blitSub")
void blitSub (const u8* data, i32 x, i32 y, u32 width, u32 height,
    u32 srcX, u32 srcY, u32 stride, u32 flags);

#define BLIT_2BPP 1
#define BLIT_1BPP 0
#define BLIT_FLIP_X 2
#define BLIT_FLIP_Y 4
#define BLIT_ROTATE 8

/** Draws a line between two points. */
WASM_IMPORT("line")
void line (i32 x, i32 y, u32 width, u32 height);

/** Draws a horizontal line. */
WASM_IMPORT("hline")
void hline (i32 x, i32 y, u32 len);

/** Draws a vertical line. */
WASM_IMPORT("vline")
void vline (i32 x, i32 y, u32 len);

/** Draws an oval (or circle). */
WASM_IMPORT("oval")
void oval (i32 x, i32 y, u32 width, u32 height);

/** Draws a rectangle. */
WASM_IMPORT("rect")
void rect (i32 x, i32 y, u32 width, u32 height);

/** Draws text using the built-in system font. */
WASM_IMPORT("text")
void text (const char* text, i32 x, i32 y);

// ┌───────────────────────────────────────────────────────────────────────────┐
// │                                                                           │
// │ Sound Functions                                                           │
// │                                                                           │
// └───────────────────────────────────────────────────────────────────────────┘

/** Plays a sound tone. */
WASM_IMPORT("tone")
void tone (u32 frequency, u32 duration, u32 volume, u32 flags);

#define TONE_PULSE1 0
#define TONE_PULSE2 1
#define TONE_TRIANGLE 2
#define TONE_NOISE 3
#define TONE_MODE1 0
#define TONE_MODE2 4
#define TONE_MODE3 8
#define TONE_MODE4 12
#define TONE_PAN_LEFT 16
#define TONE_PAN_RIGHT 32

// ┌───────────────────────────────────────────────────────────────────────────┐
// │                                                                           │
// │ Storage Functions                                                         │
// │                                                                           │
// └───────────────────────────────────────────────────────────────────────────┘

/** Reads up to `size` bytes from persistent storage into the pointer `dest`. */
WASM_IMPORT("diskr")
u32 diskr (void* dest, u32 size);

/** Writes up to `size` bytes from the pointer `src` into persistent storage. */
WASM_IMPORT("diskw")
u32 diskw (const void* src, u32 size);

/** Prints a message to the debug console. */
WASM_IMPORT("trace") void trace (const char* str);

/** Prints a message to the debug console. */
__attribute__((__format__ (__printf__, 1, 2)))
WASM_IMPORT("tracef") void tracef (const char* fmt, ...);