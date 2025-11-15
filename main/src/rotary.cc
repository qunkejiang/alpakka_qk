#include "rotary.h"
#include "esp_log.h"
#include "board.h"
#include <cstring>



// #define TAG "Rotary"


void Rotary::set_mode(uint8_t value) {
    self.mode = value;
}

void Rotary::report() {
    int8_t diff = Board::GetInstance().pcnt_->diff;
    uint8_t *actions;
    if (diff<0)
    {
        actions = self.actions[self.mode][ROTARY_DOWN];
        diff = -diff;
    }else
    {
        actions = self.actions[self.mode][ROTARY_UP];
    }
    for(int8_t i=0; i<diff; i++)
    {
        hid::press_multiple(actions);
        hid::release_multiple_later(actions, 10);
    }
}


void Rotary::config_mode(
    uint8_t mode,
    Actions actions_up,
    Actions actions_down
){
    memcpy(self.actions[mode][ROTARY_UP], actions_up, ACTIONS_LEN);
    memcpy(self.actions[mode][ROTARY_DOWN], actions_down, ACTIONS_LEN);
}


