#include "Plato/KeyCode.h"

// Load default, fallback keybinds
// FPS CAM
#define KB_FPSCAM_FORWARD         Input::KEY_CODE::W
#define KB_FPSCAM_BACKWARD        Input::KEY_CODE::A
#define KB_FPSCAM_STRAFE_LEFT     Input::KEY_CODE::S
#define KB_FPSCAM_STRAFE_RIGHT    Input::KEY_CODE::D
#define KB_FPSCAM_DOWN            Input::KEY_CODE::Q
#define KB_FPSCAM_UP              Input::KEY_CODE::F
#define KB_FPSCAM_FORWARD         Input::KEY_CODE::W

#define KB_FPSCAM_SPRINT          Input::KEY_CODE::LSHIFT
#define KB_FPSCAM_DUMP_COORDS     Input::KEY_CODE::Z

// EXCL KEYBOARD FPS CAM SPECIFICS
#define KB_FPSCAM_LOOK_LEFT       Input::KEY_CODE::H
#define KB_FPSCAM_LOOK_DOWN       Input::KEY_CODE::J
#define KB_FPSCAM_LOOK_UP         Input::KEY_CODE::K
#define KB_FPSCAM_LOOK_RIGHT      Input::KEY_CODE::L
#define KB_FPSCAM_INCREASE_FOV    Input::KEY_CODE::NUM_1
#define KB_FPSCAM_DECREASE_FOV    Input::KEY_CODE::NUM_2

// APP PLAYER
#define KB_APPPLAYER_QUIT         Input::KEY_CODE::ESCAPE

// BENCHMARK PLAYER
#define KB_BENCHMKPLAYER_SKIP     Input::KEY_CODE::SPACE

// Apply local overrides (this file should be gitignored!)
#include "Keybinds_local.h"

