// Copyright 2021 Manna Harbour
// https://github.com/manna-harbour/miryoku


// ---- owner customizations (appended) ----

// MOUSE layer (MOUSE_VI verbatim) with right-thumb LCLK/RCLK swapped:
// inner K35 = left click, middle K36 = right click, outer K37 = middle click
#define MIRYOKU_LAYER_MOUSE \
  U_BOOT, &u_to_U_TAP, &u_to_U_EXTRA, &u_to_U_BASE, U_NA, U_RDO, U_PST, U_CPY, \
  U_CUT, U_UND, &kp LGUI, &kp LALT, &kp LCTRL, &kp LSHFT, U_NA, U_MS_L, \
  U_MS_D, U_MS_U, U_MS_R, U_NU, U_NA, &kp RALT, &u_to_U_SYM, \
  &u_to_U_MOUSE, U_NA, U_WH_L, U_WH_D, U_WH_U, U_WH_R, U_NU, U_NP, U_NP, \
  U_NA, U_NA, U_NA, U_BTN1, U_BTN2, U_BTN3, U_NP, U_NP

// NAV layer (NAV_VI verbatim) with empty left-thumb K32/K33/K34 filled with
// mouse buttons: K32 = left click, K33 = right click, K34 = middle click
#define MIRYOKU_LAYER_NAV \
  U_BOOT, &u_to_U_TAP, &u_to_U_EXTRA, &u_to_U_BASE, U_NA, U_RDO, U_PST, U_CPY, \
  U_CUT, U_UND, &kp LGUI, &kp LALT, &kp LCTRL, &kp LSHFT, U_NA, &kp LEFT, \
  &kp DOWN, &kp UP, &kp RIGHT, &u_caps_word, U_NA, &kp RALT, &u_to_U_NUM, \
  &u_to_U_NAV, U_NA, &kp HOME, &kp PG_DN, &kp PG_UP, &kp END, &kp INS, \
  U_NP, U_NP, U_BTN1, U_BTN2, U_BTN3, &kp RET, &kp BSPC, &kp DEL, U_NP, U_NP
