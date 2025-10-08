// // SPDX-License-Identifier: GPL-2.0-only
// // Copyright (C) 2022, Input Labs Oy.

#pragma once

#include "ctrl.h"
#include "button.h"
// #include "glyph.h"

// #define THUMBSTICK_BASELINE_SATURATION 1.65
// #define THUMBSTICK_INNER_RADIUS 0.75
// #define THUMBSTICK_ADDITIONAL_DEADZONE_FOR_BUTTONS 0.05

typedef enum ThumbstickMode_enum {
    THUMBSTICK_MODE_OFF,
    THUMBSTICK_MODE_4DIR,
    THUMBSTICK_MODE_ALPHANUMERIC,
    THUMBSTICK_MODE_8DIR,
} ThumbstickMode;

typedef enum ThumbstickDistance_enum {
    THUMBSTICK_DISTANCE_AXIAL,
    THUMBSTICK_DISTANCE_RADIAL,
} ThumbstickDistance;

// typedef struct ThumbstickPosition_struct {
//     float x;
//     float y;
//     float angle;
//     float radius;
// } ThumbstickPosition;

// typedef enum Dir4Mask_enum {
//     DIR4_MASK_LEFT = 1,
//     DIR4_MASK_RIGHT = 2,
//     DIR4_MASK_UP = 4,
//     DIR4_MASK_DOWN = 8,
// } Dir4Mask;

// typedef enum Dir4_enum {
//     DIR4_NONE,
//     DIR4_LEFT,
//     DIR4_RIGHT,
//     DIR4_UP,
//     DIR4_DOWN,
// } Dir4;

// typedef enum Dir8_enum {
//     DIR8_CENTER,
//     DIR8_LEFT,
//     DIR8_RIGHT,
//     DIR8_UP,
//     DIR8_DOWN,
//     DIR8_UP_LEFT,
//     DIR8_UP_RIGHT,
//     DIR8_DOWN_LEFT,
//     DIR8_DOWN_RIGHT,
// } Dir8;

typedef struct Thumbstick_struct Thumbstick_t;
struct Thumbstick_struct {
    uint8_t index;
    uint8_t pin_x;
    uint8_t pin_y;
    bool invert_x;
    bool invert_y;
    ThumbstickMode mode;
    ThumbstickDistance distance_mode;
    bool deadzone_override;
    float deadzone;
    float antideadzone;
    float overlap;
    float saturation;
    Button_t left;
    Button_t right;
    Button_t up;
    Button_t down;
    Button_t ul;
    Button_t ur;
    Button_t dl;
    Button_t dr;
    Button_t push;
    Button_t inner;
    Button_t outer;
    uint8_t glyphstick_index;
//     Glyph glyphstick_glyphs[44];
//     Actions glyphstick_actions[44];
//     Actions daisywheel[8][4];
};



class Thumbstick : public Button// Forward declaration
{
private:
    Thumbstick_t thumbstick;
public:
    Thumbstick(){};
    void from_ctrl(CtrlProfile *ctrl, uint8_t index);
    void report();

};
