# Bluetooth Gamepad Controller

This is a module for connecting a bluetooth gamepad controller with the Dragonboard using BlueZ (libbluetooth)

## What I need to use this module

You will need to make sure you have the BlueZ developer headers

`sudo apt install libbluetooth-dev`

## Keycodes mapping

`hd js0`

- Returns two 8 byte packets
- First 4 in each are not used
- Joystick analog values are not accurate either
- dpad have one set with leading 0's and one with direction values
- Last 4 byte button map:

- `A` - 0x0100
- `B` - 0x0101
- `X` - 0x0103
- `Y` - 0x0104
- `L1` - 0x0106
- `R1` - 0x0107
- `L2` - 0x0108
- `R2` - 0x0109
- `Select` - 0x010A
- `Start` - 0x010B
- `Left Trigger` - 0x010D
- `Right Trigger` - 0x010E
- `Up dpad` - 0x0180_0205
- `Right dpad` - 0xff7f_0204
- `Down dpad` - 0xff7f_0205
- `Left dpad` - 0x0180_0204

--------

hci0 - ACL data RX - btmon

a1 07 _0_ _1_ _2_ _3_ _4_ _5_ _6_ 00

0 - left trig L/R (L 00 / R ff)
1 - left trig U/D (U 00 / D ff)
2 - right trig L/R (L 00 / R ff)
3 - right trig U/D (U 00 / D ff)
4 - 88 idle
  - 00 dpad up
  - 02 dpad right
  - 04 dpad down
  - 06 dpad left
5/6 - buttons
  - A - x0100
  - B - x0200
  - X - x0800
  - Y - x1000
  - L1 - x4000
  - R1 - x8000
  - L2 - x0001
  - R2 - x0002
  - Select - x0004
  - Start - x0008
  - Left Trig - x0020
  - Right Trig - x0040
