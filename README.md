# LCD_Clock2021
LCD clock with PIC 18F26K22

## Features:
* Alarm clock
* Calendar
* Temperature, Humidity

## Parts:
* LCD display: 2.8inch 320x240, 64K color, ILI9341 LCD driver<br>
      including Touch panel: XT2046
* PIC: 18F26K22
* RTC: EPSON RX-8025NB
* Sensor: AM2320
* GPS unit: GYSFDMAXB module with MT3339 (MediaTek)
* Photo Diode: NJL7502L
* Rotary encoder: ALPS EC12
* Super capacity: 1F 5.5V
* Original board is designed

## Usage:
    When GPS unit available, power on and wait. That's it.
    3 display mode available by tapping calendar on the screen.
    Alarm time can be set always by rotary switch. On/Off is set easily by slide switch.
    Current program is support temperature logging every 1 minunite.

## Detail description:
    High accuracy clock is supported by RX-8025NB.
    Easy time or alarm time setting by rotary swith.
    Touch panel is also used to set date/time.
    When you put GPS unit, not time/date adjustment is needed. PIC pickes up date/time from GPS.
    SD card interface on LCD unit can be used for data logging like temp/humidity.
    To backup RTC without power supply, you can put super capacity.
    Photo diode supports LCD brightness automatic adjustment.
