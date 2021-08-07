# LCD_Clock2021<br>
LCD clock with PIC 18F26K22<br>
<br>
<b>Features:</b><ul>
	<li>Alarm clock</li>
	<li>Calendar</li>
	<li>Temperature, Humidity</li>
</ul><br>
<b>Parts:</b><ul>
    <li>LCD display: 2.8inch 320x240, 64K color, ILI9341 LCD driver<br>
      including Touch panel: XT2046</li>
    <li>PIC: 18F26K22</li>
    <li>RTC: EPSON RX-8025NB</li>
    <li>Sensor: AM2320</li>
    <li>GPS unit: GYSFDMAXB module with MT3339 (MediaTek)</li>
    <li>Photo Diode: NJL7502L</li>
    <li>Rotary encoder: ALPS EC12</li>
    <li>Super capacity: 1F 5.5V</li>
    <li>Original board is designed</li>
    </ul><br>
<b>Usage:</b><br>
    When GPS unit available, power on and wait. That's it.<br>
    3 display mode available by tapping calendar on the screen.<br>
    Alarm time can be set always by rotary switch. On/Off is set easily by slide switch.<br>
    Current program is support temperature logging every 1 minunite.<br>
    <br>
<b>Detail description:</b><br>
    High accuracy clock is supported by RX-8025NB.<br>
    Easy time or alarm time setting by rotary swith.<br>
    Touch panel is also used to set date/time.<br>
    When you put GPS unit, not time/date adjustment is needed. PIC pickes up date/time from GPS.<br>
    SD card interface on LCD unit can be used for data logging like temp/humidity.<br>
    To backup RTC without power supply, you can put super capacity.<br>
    Photo diode supports LCD brightness automatic adjustment.<br>
