# TastefullyNudeMetal
It's not quite Bare Metal, but disables the watchdog on ESP32 core1, and disables the scheduler interrupts.

E.g. Runs a task on core 1 that can take as long as it likes, but without FreeRTOS in unicore mode.

Alternative approach to the BlueRertro's:
https://hackaday.io/project/170365-blueretro/log/189836-esp32-rtos-bare-metal-best-of-both-worlds


## Troubleshooting

### There's gibberish on the console!
`idf.py menuconfig`, `/`,`xtal`, `ESP32_XTAL_FREQ=26`

