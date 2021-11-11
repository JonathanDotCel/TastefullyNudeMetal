# TastefullyNude Metal
(Almost bare)

Lets you disable the watchdog on ESP32 core1 (App CPU), and disables the scheduler interrupts.

E.g. Runs a task on core 1 that can take as long as it likes, but without FreeRTOS in unicore mode.

Alternative approach to the BlueRertro's:
https://hackaday.io/project/170365-blueretro/log/189836-esp32-rtos-bare-metal-best-of-both-worlds

I'm sure there are plenty of pitfalls to this approach, but it's lightweight, and decent enough for bit banging.

Note:
Remember to set LWIP's affinity to core0 via menuconfig, then WIFI will behave.

## Troubleshooting

### LWIP Affinity?
`idf.py menuconfig`, `/`, type `task`, `lwip_tcpip_task_affinity`

### There's gibberish on the console!
`idf.py menuconfig`, `/`, type `xtal`, `ESP32_XTAL_FREQ=26`

