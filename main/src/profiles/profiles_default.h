#pragma once


#include "webusb.h"
#include "button.h"
#include "thumbstick.h"
#include "nvm.h"
#include "ctrl.h"
#include "profile.h"


void config_profile_default_home(CtrlProfile *profile);
void config_profile_default_fps_fusion(CtrlProfile *profile);
void config_profile_default_racing(CtrlProfile *profile);
void config_profile_default_console(CtrlProfile *profile);
void config_profile_default_desktop(CtrlProfile *profile);
void config_profile_default_fps_wasd(CtrlProfile *profile);
void config_profile_default_flight(CtrlProfile *profile);
void config_profile_default_console_legacy(CtrlProfile *profile);
void config_profile_default_rts(CtrlProfile *profile);
void config_profile_default_custom(CtrlProfile *profile);

