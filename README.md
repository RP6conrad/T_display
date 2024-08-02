# ESP-GPS logger with LCD-display

GPS logger for Speedsurfing, based on ESP32 & Ublox  hardware.  This is a open source project for a gps-speedlogger with a ESP32 µ-controller. This logger is approved for uploading to [http://gps-speedsurfing.com](http://gps-speedsurfing.com)
![ESP GPS banner 1024x500](https://user-images.githubusercontent.com/58887243/228194449-03b4aee7-f6ac-44cf-936f-beec46ba28fb.jpg)


#### Hardware :
![T_display_GPS](https://github.com/user-attachments/assets/107f00fc-0653-4c78-b557-dcd038ae628a)
- LCD display 240*135 with build in ESP32 and 16 MB flash (lilygo ttgo T-display).
- SD-card holder + SD card is optional, but with 16MB Flash, 13MB can be used as file storage. With .gpy@5Hz files, you can log >20 h.
- A gps module ublox M8n / M9 / M10 is connected over serial. 
- A 1000 - 2000 mAh protected lipo (102560 = 25 width, 60 length and 10 thickness).
- Reed switch for switch ON / OFF (off = deepsleep)
- Inductive charging coil
- Watersealed housing, electrical box or GoPro housing
#### Software
- GPS is configured for sending ubx navpvt (+navdop + navsat) messages. 
- Log rate can be 1,2,4,10 or 20 Hz (20Hz only with ublox M9). 
- Every message is logged to the flash memory (optional SD card)
- Logfile type configurable : .ubx, .sbp, .gpy, .gpx, .txt. 
- gpy is the new compact open source format, developed for speedsurfing and is accepted @ gp3s.
- For speedsurfing, next values are measured and visible on the e-paper : 
	- Max speed average over 2s.
	- Max speed average over 10s.
	- 5*10s average from 5 runs.
	- Average speed over 100m, 200m, 500m, 1852 m (nautical mile)
	- Highest average over 30 min / 60 min. t
  - Alfa gate & exit live : after a jibe you see the actual "lost heigth" and "predicted lost heigth" (should be < 50m) !
	- Total distance from the session.


#### Interface
SW is written in Arduino IDE (C++). For easy access to the files on the sd, a ftp-server over wifi is integrated. Also OTA (over the air update) of the sw is possible. SW 5.58 : Thanks to the contribution of tritondm, the webserver is now extended and all files on the sd can be downloaded over this webserver. Also the complete configuration can be changed over the webserver !! No need for ftp anymore, but this is still possible. SW 5.75 : Autodetection Ublox M8,M9,M10 is supported. First time @boot, the type of ublox and baudrate is detected and stored in eeprom memory.
#### Arduino / library versions : 
- Arduino IDE 2.02
- Partition.csv for 16 MB flash !!!
- ESP32 version 1.06, but next file need a change (only needed if you compile from the source files): 
- Change next file for compiling with Arduino IDE 2.02 (SD(esp32) to SD) C:\Users\andre\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.6\libraries\SD\library.properties
- ArduinoJson version 6.17.2
##### Building manual / first flashing :
https://docs.google.com/document/d/1EAuqvbSOyW8XMN_1-I5Rsge2eNxJeE1X47yJEWSanZo/edit?usp=sharing
![T_display_wiring_small](https://github.com/user-attachments/assets/bd4a129b-077c-40a8-a71f-e7b644c1f0c1)
##### Quick User guide
https://docs.google.com/document/d/1KfDua0H2_-J25e4L3CR8Znd8aIM6bbTPa6EjC0b7B3U/edit?usp=sharing
##### User manual :
https://docs.google.com/document/d/1j86kj3bNMID3sHCRT7QTYyrL7IHeQnGPec1LajsDfB4/edit?usp=sharing

