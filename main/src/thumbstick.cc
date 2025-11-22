// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include "button.h"
#include "thumbstick.h"
#include "Board.h"
#include <cmath>
#include "hid.h"
#include "vars.h"


void Thumbstick::report_axis(uint8_t axis, float value) {
    if      (axis == GAMEPAD_AXIS_LX)     hid::gamepad_axis(LX, value);
    else if (axis == GAMEPAD_AXIS_LY)     hid::gamepad_axis(LY, value);
    else if (axis == GAMEPAD_AXIS_RX)     hid::gamepad_axis(RX, value);
    else if (axis == GAMEPAD_AXIS_RY)     hid::gamepad_axis(RY, value);
    else if (axis == GAMEPAD_AXIS_LX_NEG) hid::gamepad_axis(LX, -value);
    else if (axis == GAMEPAD_AXIS_LY_NEG) hid::gamepad_axis(LY, -value);
    else if (axis == GAMEPAD_AXIS_RX_NEG) hid::gamepad_axis(RX, -value);
    else if (axis == GAMEPAD_AXIS_RY_NEG) hid::gamepad_axis(RY, -value);
    else if (axis == GAMEPAD_AXIS_LZ)     hid::gamepad_axis(LZ, value);
    else if (axis == GAMEPAD_AXIS_RZ)     hid::gamepad_axis(RZ, value);
}

uint8_t Thumbstick::get_direction(float angle, float overlap) {
    float a = 45 * (1 - overlap);
    float b = 180 - a;
    uint8_t mask = 0;
    if (is_between(angle, -b, -a)) mask += DIR4_MASK_LEFT;
    if (is_between(angle, a, b)) mask += DIR4_MASK_RIGHT;
    if (fabs(angle) <= (90 - a)) mask += DIR4_MASK_UP;
    if (fabs(angle) >= (90 + a)) mask += DIR4_MASK_DOWN;
    return mask;
}

void Thumbstick::from_ctrl(CtrlProfile *ctrl, uint8_t index) {
    const uint8_t SECTION_STICK_SETTINGS = index ? SECTION_RSTICK_SETTINGS : SECTION_LSTICK_SETTINGS;
    const uint8_t SECTION_STICK_LEFT = index ? SECTION_RSTICK_LEFT : SECTION_LSTICK_LEFT;
    const uint8_t SECTION_STICK_RIGHT = index ? SECTION_RSTICK_RIGHT : SECTION_LSTICK_RIGHT;
    const uint8_t SECTION_STICK_UP = index ? SECTION_RSTICK_UP : SECTION_LSTICK_UP;
    const uint8_t SECTION_STICK_DOWN = index ? SECTION_RSTICK_DOWN : SECTION_LSTICK_DOWN;
    const uint8_t SECTION_STICK_UL = index ? SECTION_RSTICK_UL : SECTION_LSTICK_UL;
    const uint8_t SECTION_STICK_UR = index ? SECTION_RSTICK_UR : SECTION_LSTICK_UR;
    const uint8_t SECTION_STICK_DL = index ? SECTION_RSTICK_DL : SECTION_LSTICK_DL;
    const uint8_t SECTION_STICK_DR = index ? SECTION_RSTICK_DR : SECTION_LSTICK_DR;
    const uint8_t SECTION_STICK_PUSH = index ? SECTION_RSTICK_PUSH : SECTION_LSTICK_PUSH;
    const uint8_t SECTION_STICK_INNER = index ? SECTION_RSTICK_INNER : SECTION_LSTICK_INNER;
    const uint8_t SECTION_STICK_OUTER = index ? SECTION_RSTICK_OUTER : SECTION_LSTICK_OUTER;
    const uint8_t BUTTON_PUSH = index ? BUTTON_R3 : BUTTON_L3;



    CtrlThumbstick ctrl_thumbtick = ctrl->sections[SECTION_STICK_SETTINGS].thumbstick;

    thumbstick.index = index;
    thumbstick.mode = (ThumbstickMode)ctrl_thumbtick.mode;
    thumbstick.distance_mode = (ThumbstickDistance)ctrl_thumbtick.distance_mode;
    thumbstick.deadzone_override = ctrl_thumbtick.deadzone_override;
    thumbstick.deadzone = ctrl_thumbtick.deadzone / 100.0;
    thumbstick.antideadzone = ctrl_thumbtick.antideadzone / 100.0;
    thumbstick.overlap = (int8_t)ctrl_thumbtick.overlap / 100.0;
    thumbstick.saturation = ctrl_thumbtick.saturation > 0 ?   100.0 / ctrl_thumbtick.saturation : 1.0;
    // thumbstick.glyphstick_index = 0;

    if (ctrl_thumbtick.mode == THUMBSTICK_MODE_4DIR) {
        thumbstick.left = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_LEFT]);
        thumbstick.right = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_RIGHT]);
        thumbstick.up = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_UP]);
        thumbstick.down = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_DOWN]);
        thumbstick.push = Button_from_ctrl(BUTTON_PUSH,    ctrl->sections[SECTION_STICK_PUSH]);
        thumbstick.inner = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_INNER]);
        thumbstick.outer = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_OUTER]);
    }
    if (ctrl_thumbtick.mode == THUMBSTICK_MODE_8DIR) {
        thumbstick.left = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_LEFT]);
        thumbstick.right = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_RIGHT]);
        thumbstick.up = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_UP]);
        thumbstick.down = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_DOWN]);
        thumbstick.ul = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_UL]);
        thumbstick.ur = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_UR]);
        thumbstick.dl = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_DL]);
        thumbstick.dr = Button_from_ctrl(BUTTON_VIRTUAL, ctrl->sections[SECTION_STICK_DR]);
        thumbstick.push = Button_from_ctrl(BUTTON_PUSH,    ctrl->sections[SECTION_STICK_PUSH]);
    }
    if (ctrl_thumbtick.mode == THUMBSTICK_MODE_ALPHANUMERIC) {
    }
}

void Thumbstick::report_4dir_axial(ThumbstickPosition pos) {
    // Evaluate virtual buttons.
    if (pos.radius > THUMBSTICK_ADDITIONAL_DEADZONE_FOR_BUTTONS) {
        if (pos.radius < THUMBSTICK_INNER_RADIUS) thumbstick.inner.virtual_press = true;
        else thumbstick.outer.virtual_press = true;
        uint8_t direction = get_direction(pos.angle, thumbstick.overlap);
        if (direction & DIR4_MASK_LEFT)  thumbstick.left.virtual_press = true;
        if (direction & DIR4_MASK_RIGHT) thumbstick.right.virtual_press = true;
        if (direction & DIR4_MASK_UP)    thumbstick.up.virtual_press = true;
        if (direction & DIR4_MASK_DOWN)  thumbstick.down.virtual_press = true;
    }
    // Report directional virtual buttons or axis.
    //// Left.
    if (!hid::is_axis(thumbstick.left.actions[0])) Button::report(&thumbstick.left);
    else report_axis(thumbstick.left.actions[0], -constrain(pos.x, -1, 0));
    //// Right.
    if (!hid::is_axis(thumbstick.right.actions[0])) Button::report(&thumbstick.right);
    else report_axis(thumbstick.right.actions[0], constrain(pos.x, 0, 1));
    //// Up.
    if (!hid::is_axis(thumbstick.up.actions[0])) Button::report(&thumbstick.up);
    else report_axis(thumbstick.up.actions[0], -constrain(pos.y, -1, 0));
    //// Down.
    if (!hid::is_axis(thumbstick.down.actions[0])) Button::report(&thumbstick.down);
    else report_axis(thumbstick.down.actions[0], constrain(pos.y, 0, 1));
    // Report inner and outer.
    Button::report(&thumbstick.inner);
    Button::report(&thumbstick.outer);
    // Report push.
    Button::report(&thumbstick.push);
}


void Thumbstick::report_4dir_radial(ThumbstickPosition pos) {
    uint8_t direction = get_direction(pos.angle, thumbstick.overlap);
    report_axis(thumbstick.left.actions[0],  (direction & DIR4_MASK_LEFT)  ? pos.radius : 0);
    report_axis(thumbstick.right.actions[0], (direction & DIR4_MASK_RIGHT) ? pos.radius : 0);
    report_axis(thumbstick.up.actions[0],    (direction & DIR4_MASK_UP)    ? pos.radius : 0);
    report_axis(thumbstick.down.actions[0],  (direction & DIR4_MASK_DOWN)  ? pos.radius : 0);
    Button::report(&thumbstick.push);
}

void Thumbstick::report_8dir(ThumbstickPosition pos) {
    // Evaluate virtual buttons.
    if (pos.radius > THUMBSTICK_ADDITIONAL_DEADZONE_FOR_BUTTONS) {
        uint8_t direction = get_direction(pos.angle, 0.5); // Fixed overlap.
        if      (direction == DIR4_MASK_LEFT)  thumbstick.left.virtual_press = true;
        else if (direction == DIR4_MASK_RIGHT) thumbstick.right.virtual_press = true;
        else if (direction == DIR4_MASK_UP)    thumbstick.up.virtual_press = true;
        else if (direction == DIR4_MASK_DOWN)  thumbstick.down.virtual_press = true;
        else if (direction == (DIR4_MASK_UP   + DIR4_MASK_LEFT))  thumbstick.ul.virtual_press = true;
        else if (direction == (DIR4_MASK_UP   + DIR4_MASK_RIGHT)) thumbstick.ur.virtual_press = true;
        else if (direction == (DIR4_MASK_DOWN + DIR4_MASK_LEFT))  thumbstick.dl.virtual_press = true;
        else if (direction == (DIR4_MASK_DOWN + DIR4_MASK_RIGHT)) thumbstick.dr.virtual_press = true;
    }
    // Report directional virtual buttons.
    Button::report(&thumbstick.left);
    Button::report(&thumbstick.right);
    Button::report(&thumbstick.up);
    Button::report(&thumbstick.down);
    Button::report(&thumbstick.ul);
    Button::report(&thumbstick.ur);
    Button::report(&thumbstick.dl);
    Button::report(&thumbstick.dr);
    // Report push.
    Button::report(&thumbstick.push);
}

void Thumbstick::report_daisywheel(Dir8 dir) {
    static const Actions daisywheel_key[2][9][6] = {
        {
            {   {KEY_NONE},{KEY_ESCAPE},{KEY_TAB},{KEY_SPACE},{KEY_BACKSPACE},{KEY_ENTER}   },
            {   {KEY_1},{KEY_2},{KEY_A},{KEY_B},{KEY_C},{KEY_D}   },
            {   {KEY_3},{KEY_4},{KEY_E},{KEY_F},{KEY_G},{KEY_H}   },
            {   {KEY_5},{KEY_6},{KEY_I},{KEY_J},{KEY_K},{KEY_L}   },
            {   {KEY_7},{KEY_8},{KEY_M},{KEY_N},{KEY_O},{KEY_P}   },
            {   {KEY_9},{KEY_0},{KEY_Q},{KEY_R},{KEY_S},{KEY_T}   },
            {   {KEY_MINUS},{KEY_EQUALS},{KEY_U},{KEY_V},{KEY_W},{KEY_X}   },
            {   {KEY_BRACKET_LEFT},{KEY_BRACKET_RIGHT},{KEY_Y},{KEY_Z},{KEY_BACKSLASH},{KEY_QUOTE}   },
            {   {KEY_SEMICOLON},{KEY_COMMA},{KEY_PERIOD},{KEY_SLASH},{KEY_CONTROL_LEFT, KEY_C},{KEY_CONTROL_LEFT, KEY_V}   },
        },
        {
            {   {KEY_NONE},{KEY_ESCAPE},{KEY_TAB},{KEY_SPACE},{KEY_BACKSPACE},{KEY_ENTER}   },
            {   {KEY_SHIFT_LEFT,KEY_1},{KEY_SHIFT_LEFT,KEY_2},{KEY_SHIFT_LEFT,KEY_A},{KEY_SHIFT_LEFT,KEY_B},{KEY_SHIFT_LEFT,KEY_C},{KEY_SHIFT_LEFT,KEY_D}   },
            {   {KEY_SHIFT_LEFT,KEY_3},{KEY_SHIFT_LEFT,KEY_4},{KEY_SHIFT_LEFT,KEY_E},{KEY_SHIFT_LEFT,KEY_F},{KEY_SHIFT_LEFT,KEY_G},{KEY_SHIFT_LEFT,KEY_H}   },
            {   {KEY_SHIFT_LEFT,KEY_5},{KEY_SHIFT_LEFT,KEY_6},{KEY_SHIFT_LEFT,KEY_I},{KEY_SHIFT_LEFT,KEY_J},{KEY_SHIFT_LEFT,KEY_K},{KEY_SHIFT_LEFT,KEY_L}   },
            {   {KEY_SHIFT_LEFT,KEY_7},{KEY_SHIFT_LEFT,KEY_8},{KEY_SHIFT_LEFT,KEY_M},{KEY_SHIFT_LEFT,KEY_N},{KEY_SHIFT_LEFT,KEY_O},{KEY_SHIFT_LEFT,KEY_P}   },
            {   {KEY_SHIFT_LEFT,KEY_9},{KEY_SHIFT_LEFT,KEY_0},{KEY_SHIFT_LEFT,KEY_Q},{KEY_SHIFT_LEFT,KEY_R},{KEY_SHIFT_LEFT,KEY_S},{KEY_SHIFT_LEFT,KEY_T}   },
            {   {KEY_SHIFT_LEFT,KEY_MINUS},{KEY_SHIFT_LEFT,KEY_EQUALS},{KEY_SHIFT_LEFT,KEY_U},{KEY_SHIFT_LEFT,KEY_V},{KEY_SHIFT_LEFT,KEY_W},{KEY_SHIFT_LEFT,KEY_X}   },
            {   {KEY_SHIFT_LEFT,KEY_BRACKET_LEFT},{KEY_SHIFT_LEFT,KEY_BRACKET_RIGHT},{KEY_SHIFT_LEFT,KEY_Y},{KEY_SHIFT_LEFT,KEY_Z},{KEY_SHIFT_LEFT,KEY_BACKSLASH},{KEY_SHIFT_LEFT,KEY_QUOTE}   },
            {   {KEY_SHIFT_LEFT,KEY_SEMICOLON},{KEY_SHIFT_LEFT,KEY_COMMA},{KEY_SHIFT_LEFT,KEY_PERIOD},{KEY_SHIFT_LEFT,KEY_SLASH},{KEY_VOLUME_UP},{KEY_VOLUME_DOWN}   },
        }
    };
    const uint8_t BUTTON[6] = {BUTTON_START_1,BUTTON_START_2,BUTTON_Y,BUTTON_X,BUTTON_B,BUTTON_A};
    set_var_keymouse_zu(dir);
    uint8_t keymouse_aa = get_var_keymouse_aa();
    static uint8_t BUTTON_START[6] = {0,0,0,0,0};
    for(uint8_t i=0;i<6;i++)
    {
        if(Board::get_key_value(BUTTON[i]))
        {
            if(BUTTON_START[i] ==0)
            {
                if((i==0)&&(dir==DIR8_CENTER))
                {
                    if(keymouse_aa)set_var_keymouse_aa(false);
                    else set_var_keymouse_aa(true);
                }
                hid::press_multiple((uint8_t *)daisywheel_key[keymouse_aa][dir][i]);
                BUTTON_START[i] = 0x80|(keymouse_aa<<4)|dir;
            }  
        }else
        {
            if(BUTTON_START[i])
            {
                hid::release_multiple((uint8_t *)daisywheel_key[(BUTTON_START[i]&0x10)>>4][BUTTON_START[i]&0xf][i]);
                BUTTON_START[i] = 0;
            }
        }
    }
}
void Thumbstick::report_alphanumeric(ThumbstickPosition pos) {
    #define CUT8 22.5f
    Dir8 dir8 = DIR8_CENTER;
    if (pos.radius > 0.7) {
        // Detect direction 8.
        if      (is_between(pos.angle, -CUT8*1,  CUT8*1)) dir8 = DIR8_UP;
        else if (is_between(pos.angle,  CUT8*1,  CUT8*3)) dir8 = DIR8_UP_RIGHT;
        else if (is_between(pos.angle,  CUT8*3,  CUT8*5)) dir8 = DIR8_RIGHT;
        else if (is_between(pos.angle,  CUT8*5,  CUT8*7)) dir8 = DIR8_DOWN_RIGHT;
        else if (is_between(pos.angle, -CUT8*7, -CUT8*5)) dir8 = DIR8_DOWN_LEFT;
        else if (is_between(pos.angle, -CUT8*5, -CUT8*3)) dir8 = DIR8_LEFT;
        else if (is_between(pos.angle, -CUT8*3, -CUT8*1)) dir8 = DIR8_UP_LEFT;
        else if (fabs(pos.angle) >= CUT8*7)               dir8 = DIR8_DOWN;
    } 
    // Report daisy keyboard.
    report_daisywheel(dir8);
}
void Thumbstick::report() {
    joystick_position_t *pos=&Board::GetInstance().joystick_->position[thumbstick.index];
    Config_t *nvm_data = &Board::GetInstance().nvm_->nvm_data;
    // Get correct deadzone.
    float deadzone = thumbstick.deadzone_override ? thumbstick.deadzone : nvm_data->deadzone_values[ nvm_data->deadzone];
    deadzone *= thumbstick.saturation;
    // Calculate trigonometry.
    float radius =pos->radius*thumbstick.saturation;
    radius=constrain(radius, 0, 1);
    if (radius < deadzone) {
        radius = 0;
    } else {
        radius = ramp_inv( ramp_low(radius, deadzone), thumbstick.antideadzone);
    }
    float rag = radians(pos->angle);
    float x = sinf(rag) * radius;
    float y = cosf(rag) * radius;
    ThumbstickPosition position = {x, y, pos->angle, radius};
    // Report.
    if (thumbstick.mode == THUMBSTICK_MODE_4DIR) {
        if (thumbstick.distance_mode == THUMBSTICK_DISTANCE_AXIAL) {
            report_4dir_axial(position);
        }
        if (thumbstick.distance_mode == THUMBSTICK_DISTANCE_RADIAL) {
            report_4dir_radial(position);
        }
    }
    else if (thumbstick.mode == THUMBSTICK_MODE_8DIR) {
        report_8dir(position);
    }
    else if (thumbstick.mode == THUMBSTICK_MODE_ALPHANUMERIC) {
        report_alphanumeric(position);
    }
}

void Thumbstick::reset() {
    if (thumbstick.mode == THUMBSTICK_MODE_4DIR) {
        Button::reset(&thumbstick.left);
        Button::reset(&thumbstick.right);
        Button::reset(&thumbstick.up);
        Button::reset(&thumbstick.down);
        Button::reset(&thumbstick.push);
        Button::reset(&thumbstick.inner);
        Button::reset(&thumbstick.outer);
    }
}

