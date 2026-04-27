# CLAUDE.md

Owner-specific notes for the `BANANASJIM/miryoku_zmk` fork. The fork tracks
upstream `manna-harbour/miryoku_zmk` (a build framework that wraps ZMK with the
Miryoku 8-layer keymap). Two custom build paths exist on this fork; do not
confuse them.

## Build paths

| Workflow | Outboard | Source branch | Hardware | Status |
| --- | --- | --- | --- | --- |
| `build-example-cygnus.yml` | `outboards/shields/charybdis` | `BANANASJIM/zmk-for-charybdis@Cygnus` | Plain Charybdis (no trackball) | Original |
| `build-example-cygnus-nano35.yml` | `outboards/shields/charybdis_nano35` | `BANANASJIM/zmk-for-charybdis@Cygnus-Nano35` | Charybdis + PMW3610 trackball + ZMK Studio physical layout | Active |

Both paths produce a firmware with the user's standard Miryoku layout
(`MIRYOKU_ALPHAS_QWERTY` + `MIRYOKU_NAV_VI` + `MIRYOKU_KLUDGE_MOUSEKEYSPR` +
`MIRYOKU_CLIPBOARD_WIN`) and the Cygnus 36-key mapping
(`miryoku/mapping/36/cygnus.h`). Do **not** modify the original `cygnus`
build path or `outboards/shields/charybdis` when working on the Nano35
variant — keep them isolated.

## Cygnus-Nano35 build chain

1. `build-example-cygnus-nano35.yml` calls `main.yml` with shield
   `charybdis_nano35_left` / `charybdis_nano35_right`.
2. `main.yml` strips `_left`/`_right` → `keyboard_base = charybdis_nano35`,
   then sources `outboards/shields/charybdis_nano35`.
3. The outboard file pins ZMK to `zmkfirmware/zmk@v0.3` (PMW3610 driver
   needs ≥ v0.3) and pulls `DoctorWangWang/zmk-pmw3610-driver@main` as a
   Zephyr module via `outboard_modules`.
4. The shield directory is symlinked from
   `BANANASJIM/zmk-for-charybdis@Cygnus-Nano35:config/boards/shields/charybdis_nano35`.
5. `config/charybdis_nano35.keymap` (in this repo) provides the Miryoku
   include chain that ZMK picks up for the shield.

## ZMK Studio

Enabled over **BLE** (Studio's `ZMK_STUDIO_TRANSPORT_BLE` defaults `y` in
v0.3 whenever `ZMK_BLE` is on). The `studio-rpc-usb-uart` snippet is **not**
needed and `main.yml` does not support snippets anyway. Adding USB-cable
Studio later would require a custom workflow that bypasses `main.yml`.

## Locked-in design decisions (do not regress)

- **EXT_POWER stays disabled** in `charybdis_nano35_right.conf`. The
  Bastardkb sensor PCB has no power-gating MOSFET (verified against
  `Bastardkb/charybdis-pmw-3360-sensor-pcb` KiCad BOM); enabling
  `CONFIG_ZMK_EXT_POWER=y` without a matching ext_power GPIO node leaks
  ~3.8 mA in deep sleep for zero behavior change.
- **Trackball cannot wake from deep sleep.** `DoctorWangWang/zmk-pmw3610-driver@main`
  has zero `pm_device` / `wakeup` code (verified end-to-end in `src/pmw3610.c`).
  Adding `wakeup-source;` to the trackball@0 node is silently ignored.
  Only key matrix wakes the SoC (kscan has `wakeup-source;`).
- **BT device name `Cygnus-Nano35` is exactly 13 chars** and must stay
  `< 16` (Zephyr `BUILD_ASSERT(DEVICE_NAME_LEN < CONFIG_BT_DEVICE_NAME_MAX)`
  in `subsys/bluetooth/host/hci_core.c:4112`). Do not lengthen without also
  bumping `CONFIG_BT_DEVICE_NAME_MAX`.

## Testing the firmware

CI build passing is necessary but not sufficient. After flashing, verify on
the physical keyboard:

1. Trackball produces cursor movement (this is the highest-risk path —
   `zmk,input-listener` → legacy `CONFIG_ZMK_MOUSE` HID is a fragile glue
   in v0.3). If broken, swap the driver fork (`badjeff/zmk-pmw3610-driver`
   or `petejohanson/zmk-pointing` are alternatives).
2. Trackball X/Y direction. `CONFIG_PMW3610_ORIENTATION_90=y` and
   `CONFIG_PMW3610_INVERT_X=y` are inherited from upstream Vzhao Nano35-MOD
   without physical verification on this build.
3. Any-key wake from idle (any matrix key triggers wake; trackball does not).

## Repo layout

- `miryoku/` — upstream Miryoku core (Manna Harbour). Don't edit unless
  upstreaming a fix.
- `miryoku/mapping/36/cygnus.h` — owner-defined 36-key physical mapping.
  This was the entry point for adapting Miryoku to the Cygnus thumb layout
  and is the file most likely to need future tweaks.
- `miryoku/miryoku_babel/miryoku_layer_alternatives.h` — owner has tweaks
  here (mostly nav/sym alternatives). Search for diffs vs upstream before
  editing.
- `config/<shield>.keymap` — three lines: include custom_config + mapping
  + miryoku.dtsi. Adding a new shield variant means adding one of these.
- `.github/workflows/outboards/shields/<shield>` — sources external shield
  repo. Use existing `charybdis_nano35` as template for new variants.
