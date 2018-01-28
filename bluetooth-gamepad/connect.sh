#!/bin/bash

# bluetoothctl connect 00:FF:01:00:11:54

echo -e "connect 00:FF:01:00:11:56 \nquit" | bluetoothctl
