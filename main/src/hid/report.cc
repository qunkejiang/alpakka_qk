
// KeyboardReport hid_get_keyboard_report() {
//     // Keys.
//     uint8_t keys[6] = {0};
//     uint8_t keys_available = 6;
//     for(int i=0; i<=115; i++) {
//         if (state_matrix[i] >= 1) {
//             keys[keys_available - 1] = (uint8_t)i;
//             keys_available--;
//             if (keys_available == 0) {
//                 break;
//             }
//         }
//     }
//     // Modifiers.
//     uint8_t modifiers = 0;
//     for(int i=0; i<8; i++) {
//         // Any value bigger than 1 consolidates to 1 (with !!).
//         modifiers += !!state_matrix[MODIFIER_INDEX + i] << i;
//     }
//     // Create report.
//     KeyboardReport report = {modifiers};
//     memcpy(report.keycode, keys, 6);
//     return report;
// }
// MouseReport hid_get_mouse_report() {
//     // Create button bitmask.
//     int8_t buttons = 0;
//     for(int i=0; i<5; i++) {
//         buttons += state_matrix[MOUSE_INDEX + i] << i;
//     }
//     uint8_t scroll = state_matrix[MOUSE_SCROLL_UP] - state_matrix[MOUSE_SCROLL_DOWN];
//     // Create report.
//     MouseReport report = {buttons, mouse_x, mouse_y, scroll, 0};
//     return report;
// }
// GamepadReport hid_get_gamepad_report() {
//     // Sorted so the most common assigned buttons are lower and easier to
//     // identify in-game.
//     int32_t buttons = (
//         // Any value bigger than 1 consolidates to 1 (with !!).
//         ((!!state_matrix[GAMEPAD_A])      <<  0) +
//         ((!!state_matrix[GAMEPAD_B])      <<  1) +
//         ((!!state_matrix[GAMEPAD_X])      <<  2) +
//         ((!!state_matrix[GAMEPAD_Y])      <<  3) +
//         ((!!state_matrix[GAMEPAD_L1])     <<  4) +
//         ((!!state_matrix[GAMEPAD_R1])     <<  5) +
//         ((!!state_matrix[GAMEPAD_L3])     <<  6) +
//         ((!!state_matrix[GAMEPAD_R3])     <<  7) +
//         ((!!state_matrix[GAMEPAD_LEFT])   <<  8) +
//         ((!!state_matrix[GAMEPAD_RIGHT])  <<  9) +
//         ((!!state_matrix[GAMEPAD_UP])     << 10) +
//         ((!!state_matrix[GAMEPAD_DOWN])   << 11) +
//         ((!!state_matrix[GAMEPAD_SELECT]) << 12) +
//         ((!!state_matrix[GAMEPAD_START])  << 13) +
//         ((!!state_matrix[GAMEPAD_HOME])   << 14)
//     );
//     // Adjust range from [-1,1] to [-32767,32767].
//     int16_t lx_report = hid_axis(gamepad_axis[LX], GAMEPAD_AXIS_LX, GAMEPAD_AXIS_LX_NEG) * BIT_15;
//     int16_t ly_report = hid_axis(gamepad_axis[LY], GAMEPAD_AXIS_LY, GAMEPAD_AXIS_LY_NEG) * BIT_15;
//     int16_t rx_report = hid_axis(gamepad_axis[RX], GAMEPAD_AXIS_RX, GAMEPAD_AXIS_RX_NEG) * BIT_15;
//     int16_t ry_report = hid_axis(gamepad_axis[RY], GAMEPAD_AXIS_RY, GAMEPAD_AXIS_RY_NEG) * BIT_15;
//     // HID triggers must be also defined as unsigned in the USB descriptor, and has to be manually
//     // value-shifted from signed to unsigned here, otherwise Windows is having erratic behavior and
//     // inconsistencies between games (not sure if a bug in Windows' DirectInput or TinyUSB).
//     int16_t lz_report = ((hid_axis(gamepad_axis[LZ], GAMEPAD_AXIS_LZ, 0) * 2) - 1) * BIT_15;
//     int16_t rz_report = ((hid_axis(gamepad_axis[RZ], GAMEPAD_AXIS_RZ, 0) * 2) - 1) * BIT_15;
//     GamepadReport report = {
//         lx_report,
//         ly_report,
//         rx_report,
//         ry_report,
//         lz_report,
//         rz_report,
//         buttons,
//     };
//     return report;
// }
// XInputReport hid_get_xinput_report() {
//     int8_t buttons_0 = 0;
//     int8_t buttons_1 = 0;
//     // Button bitmask.
//     // Any value bigger than 1 consolidates to 1 (with !!).
//     for(int i=0; i<8; i++) {
//         buttons_0 += (!!state_matrix[GAMEPAD_INDEX + i]) << i;
//     }
//     for(int i=0; i<8; i++) {
//         buttons_1 += (!!state_matrix[GAMEPAD_INDEX + i + 8]) << i;
//     }
//     // Adjust range from [-1,1] to [-32767,32767].
//     int16_t lx_report = hid_axis(gamepad_axis[LX], GAMEPAD_AXIS_LX, GAMEPAD_AXIS_LX_NEG) * BIT_15;
//     int16_t ly_report = hid_axis(gamepad_axis[LY], GAMEPAD_AXIS_LY, GAMEPAD_AXIS_LY_NEG) * BIT_15;
//     int16_t rx_report = hid_axis(gamepad_axis[RX], GAMEPAD_AXIS_RX, GAMEPAD_AXIS_RX_NEG) * BIT_15;
//     int16_t ry_report = hid_axis(gamepad_axis[RY], GAMEPAD_AXIS_RY, GAMEPAD_AXIS_RY_NEG) * BIT_15;
//     // Adjust range from [0,1] to [0,255].
//     uint16_t lz_report = hid_axis(gamepad_axis[LZ], GAMEPAD_AXIS_LZ, 0) * BIT_8;
//     uint16_t rz_report = hid_axis(gamepad_axis[RZ], GAMEPAD_AXIS_RZ, 0) * BIT_8;
//     XInputReport report = {
//         .report_id   = 0,
//         .report_size = XINPUT_REPORT_SIZE,
//         .buttons_0   = buttons_0,
//         .buttons_1   = buttons_1,
//         .lz          = lz_report,
//         .rz          = rz_report,
//         .lx          = lx_report,
//         .ly          = -ly_report,
//         .rx          = rx_report,
//         .ry          = -ry_report,
//         .reserved    = {0, 0, 0, 0, 0, 0}
//     };
//     return report;
// }