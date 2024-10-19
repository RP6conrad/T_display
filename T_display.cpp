#include "T_display.h"
#include "Definitions.h"
#include <LITTLEFS.h>

#ifndef T5_E_PAPER
char time_now[8]="00:00";
char time_now_sec[12];
void Boot_screen(void){};

void Sleep_screen(int choice){};
void Bat_level_Simon(int offset) {
  float bat_perc = 100 * (1 - (VOLTAGE_100 - RTC_voltage_bat) / (VOLTAGE_100 - VOLTAGE_0));
  if (bat_perc < 0) bat_perc = 0;
  if (bat_perc > 100) bat_perc = 100;

  int batW = 10;
  int batL = 25;
  int posX = TFT_HEIGHT - batW;//was -10
  int posY = TFT_WIDTH - batL;
  int line = 2;
  int seg = 3;
  int segW = batW - 2 * line;
  int segL = (batL - 0.25 * batW - 2 * line - (seg - 1)) / seg;
  int color = TFT_GREEN;
  if (bat_perc < 33){color=TFT_YELLOW;}
  if (bat_perc < 5) {color=TFT_RED;}
  tft.fillRoundRect(offset + posX, posY, 0.5 * batW, 0.25 * batW, 0,color);                 //battery top
  tft.fillRoundRect(offset + posX - 0.25 * batW, posY + 0.25 * batW, batW, batL,0,color);  //battery body
  if (bat_perc < 67){tft.fillRoundRect(offset + posX - 0.25 * batW + line, posY + 0.25 * batW + line, segW, segL,0, TFT_BLACK);}
  if (bat_perc < 33){tft.fillRoundRect(offset + posX - 0.25 * batW + line, posY + 0.25 * batW + line + 1 * (segL + 1), segW, segL,0, TFT_BLACK);}
  if (bat_perc < 5) {tft.fillRoundRect(offset + posX - 0.25 * batW + line, posY + 0.25 * batW + line + 2 * (segL + 1), segW, segL,0, TFT_BLACK);}
 
  //Serial.printf("info bar cursor pos: %d, display height: %d\n", INFO_BAR_ROW,displayHeight);
  //display.setFont(&FreeSansBold9pt7b);
  //display.setCursor(displayWidth-8,(INFO_BAR_ROW-ROW_9PT));
  //display.print("-");
  if (bat_perc < 100) tft.setCursor(120,114);//was 193
  else tft.setCursor(104,114);//was 184
  tft.setTextFont(4);
  tft.print(RTC_voltage_bat+0.04,1);
  tft.print("V ");
  tft.print(int(bat_perc));
  tft.print("%");
}
void Sats_level(int offset) {

  int posX = 95;//was 176
  int posY = 114;  //-(circelL+2*circelS);
  int satnum = ubxMessage.navPvt.numSV;
  tft.drawBitmap( posX, posY,ESP_Sat_22, 22, 22,TFT_BLACK,TFT_GREEN);
  tft.setCursor(posX - 25, posY);//tft.setCursor(posX - (satnum < 10 ? 10 : 19), posY);
  tft.print(ubxMessage.navPvt.numSV);
}
int Time(int offset) {
  if (!update_time()) {
    tft.setCursor(offset, 114);
    tft.print(time_now);
  }
  return 0;
}
int update_time() {
  int ret = 0;
  if (!NTP_time_set) {
    if (!Gps_time_set) {
      if (Set_GPS_Time(config.timezone)) Gps_time_set = 1;
    }
  }
  if ((!Gps_time_set && !NTP_time_set) || !getLocalTime(&tmstruct)) return 1;
  sprintf(time_now, "%02d:%02d", tmstruct.tm_hour, tmstruct.tm_min);
  sprintf(time_now_sec, "%02d:%02d:%02d", tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
  return ret;
}
void InfoBarRtc(int offset) {
  Bat_level_Simon(offset);
  DateTimeRtc(offset);
}
int TimeRtc(int offset) {
  tft.setCursor(0, 134);
  tft.printf("%d:%d", RTC_hour, RTC_min);
  return 0;
}
int DateTimeRtc(int offset) {
  tft.setCursor(0,25);
  tft.printf("%02d:%02d %02d-%02d-%02d", RTC_hour, RTC_min, RTC_day, RTC_month, RTC_year);
  return 0;
}
void InfoBar(int offset) {
  Bat_level_Simon(offset);
  Sats_level(offset);
  //if(ubxMessage.navPvt.numSV>4) M8_M10(offset);
  Time(offset);
}
void M8_M10(int offset) {
  tft.setCursor(60 , 114);
  tft.print(gpsChip(0));
}
const char* gpsChip(int longname) {
  switch (config.ublox_type) {
    case M8_9600BD:
      return longname ? "M8 9.6Kbd" : "M8";
      break;
    case M8_38400BD:
      return longname ? "M8 38.4Kbd" : "M8";
      break;
    case M9_9600BD:
      return longname ? "M9 9.6Kbd" : "M9";
      break;
    case M9_38400BD:
      return longname ? "M9 38.4Kbd" : "M9";
      break;
    case M10_9600BD:
      return longname ? "M10 9.6Kbd" : "M10";
      break;
    case M10_38400BD:
      return longname ? "M10 38.4Kbd" : "M10";
      break;
    default:
      return "unknown";
      break;
  }
}
void Boot_Screen1(void){
  if((!sdOK)&(!LittleFS_OK)){
    tft.init();           // Initialize ST7789 240x135
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    img.createSprite(TFT_WIDTH, TFT_HEIGHT);
    img.fillScreen(TFT_BLACK);
    pinMode(TFT_BL, OUTPUT);      // TTGO T-Display enable Backlight pin 4
    digitalWrite(TFT_BL, HIGH);   // T-Display turn on Backlight
    tft.setCursor(0, 0);
    tft.setTextWrap(false);
    tft.setTextColor(TFT_YELLOW,TFT_BLACK);
    tft.setTextFont(4);
    tft.println("T-Display ESP-GPS");
    for (int pos=0;pos<240;pos++){
      tft.drawBitmap( pos, 60,ESP_GPS_logo,48 , 48,TFT_BLACK,TFT_RED);
      tft.drawLine(pos,60,pos,60+48,TFT_BLACK);
      }
    }
    else{
      int Mbytes=freeSpace;
      tft.setCursor(0,0);
      tft.setTextColor(TFT_GREEN,TFT_BLACK);
      tft.setTextFont(4);
      if(LittleFS_OK) tft.print("Flash free:");
      if(sdOK) tft.print("SD free:");
      tft.print(Mbytes);
      tft.println("Mb   ");
      cursor_y=tft.getCursorY(); 
      Bat_level_Simon(0);
    }   
}
void Off_screen(int choice){
tft.fillScreen(TFT_BLACK);
tft.setCursor(0,0);
tft.println("ESP-GPS saving");
tft.println("Saving session");
 float session_time = (millis() - start_logging_millis) / 1000;
tft.print("Time: ");tft.print(session_time, 0);tft.println(" s");
tft.print("AVG: ");tft.println(RTC_avg_10s, 2);
tft.print("Dist: ");tft.println(Ublox.total_distance / 1000, 0);
}

//******************************************************************************************************************************************
void Fill_Screen(int color){
  tft.fillScreen(color);
  }
//****************************************************************************************************************************************  
void Update_screen(int screen){
  static int Old_Screen;
  if(Old_Screen!=screen)tft.fillScreen(TFT_BLACK);  
  Old_Screen=screen;
  if(screen==WIFI_STATION){
    int Mbytes=freeSpace;
    tft.setCursor(0,0);
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    if(Mbytes<5)tft.setTextColor(TFT_YELLOW,TFT_BLACK);
    if(Mbytes<2)tft.setTextColor(TFT_RED,TFT_BLACK);
    tft.setTextFont(4);
    if(LittleFS_OK) tft.print("Flash free:");
    if(sdOK) tft.print("SD free:");
    tft.print(Mbytes);
    tft.println("Mb   ");
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.print("Search for ");tft.print(wifi_search);tft.println(" s    ");
    tft.print(actual_ssid);
    tft.println("             ");
    }
//**************************************************************************************************************************************************       
  if(screen==WIFI_SOFT_AP){
    tft.print("AP active for: ");
    tft.print(wifi_search);tft.println("     ");
    tft.print(soft_ap_ssid);tft.println("       ");
    }
//**************************************************************************************************************************************************    
  if(screen==WIFI_ON) {
    int Mbytes=freeSpace;
    tft.setCursor(0,0);
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.setTextFont(4);
    if(LittleFS_OK) tft.print("Flash free:");
    if(sdOK) tft.print("SD free:");
    tft.print(Mbytes);
    tft.println("Mb   ");
    //cursor_y=tft.getCursorY(); 
    //tft.setCursor(0, cursor_y, 4);
    tft.println("Connected to     ");
    tft.print(actual_ssid);
    tft.println("      ");
    tft.print("IP:");
    tft.println(IP_adress);
    InfoBar(0);
    //if(millis()%6000<3000)digitalWrite(TFT_BL, HIGH);   // T-Display turn on Backlight
    //else digitalWrite(TFT_BL, LOW);   // T-Display turn on Backlight
  } 
//*************************************************************************************************************************************************
if(screen==GPS_INIT_SCREEN){     
  if(config.ublox_type==0xFF){
      tft.fillScreen(TFT_BLACK);  
      tft.setCursor(0,0);
      tft.print("Auto detect gps-type");
    }
  else if (!ubxMessage.monVER.hwVersion[0]) {
      tft.setCursor(0,0);
      tft.fillScreen(TFT_BLACK);  
      tft.println("Gps initializing");
      for (int pos=0;pos<240;pos++){
        tft.drawBitmap( pos, 58,ESP_Sat_22,22 ,22,TFT_BLACK,TFT_YELLOW);
        tft.drawLine(pos,58,pos,58+22,TFT_BLACK);
        delay(2);
        }
    }
  else{
      tft.setCursor(0,0);tft.println("");
      tft.printf("%s at %dHz", gpsChip(1), config.sample_rate);
      tft.println("");
      if(ubxMessage.navPvt.numSV<5){
        int color=TFT_RED;
        if(ubxMessage.navPvt.numSV>1)color=TFT_MAGENTA;
        if(ubxMessage.navPvt.numSV>3)color=TFT_YELLOW;
        tft.print("Waiting for Sat>=5");
        for (int pos=0;pos<240;pos++){
          tft.drawBitmap( pos, 78,ESP_Sat_22,22 ,22,TFT_BLACK,color);
          tft.drawLine(pos,78,pos,78+22,TFT_BLACK);
          delay(10);
          }
        }
      else{
        tft.println("Ready for action !       ");
        tft.print("Move > then ");
        if ((int)(calibration_speed * 100000) == 194) {
          tft.print(config.start_logging_speed * 1.94384449,1);
          tft.print("kn");
        }
        if ((int)(calibration_speed * 1000000) == 3600) {
          tft.print(config.start_logging_speed * 3.6,1);
          tft.print("km/h");
        }
      }
    InfoBar(0);  
    }
  } 
//*********************************************************************************************************************************************************************
  if(screen==SPEED) {
  tft.setCursor(0,0);
   tft.print("R");
   tft.setTextFont(6);
   tft.print(S10.s_max_speed * calibration_speed,1);
   tft.setTextFont(4);
   tft.setCursor(120,0);
   tft.print("A");
   tft.setTextFont(6);
   tft.println(S10.avg_5runs * calibration_speed,1);tft.print("      ");
   tft.setCursor(0,38);
   tft.setTextColor(TFT_WHITE,TFT_BLACK);
   tft.setTextFont(8);
   tft.print(gps_speed * calibration_speed);tft.print("   ");
   tft.setTextColor(TFT_GREEN,TFT_BLACK);
   tft.setTextFont(4);
  }
//****************************************************************************************************************************************************************************
  if(screen==STATS1){ 
    tft.setCursor(0,0);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.print("AVG ");tft.print(S10.avg_5runs * calibration_speed,1);
    tft.setCursor(120,0);
    tft.print("2s  ");tft.println(S2.display_max_speed * calibration_speed,1);
    cursor_y=tft.getCursorY(); 
    tft.print("10sF ");tft.print(S10.display_max_speed * calibration_speed,1);
    tft.setCursor(120,cursor_y);
    tft.print("10sS ");tft.println(S10.display_speed[5] * calibration_speed,1);
    cursor_y=tft.getCursorY(); 
    tft.print("Alfa ");tft.print(A500.avg_speed[9] * calibration_speed,1);
    tft.setCursor(120,cursor_y);
    tft.print("NM  ");tft.println(M1852.display_max_speed* calibration_speed,1);
    cursor_y=tft.getCursorY(); 
    tft.print("Dis ");tft.print(Ublox.total_distance / 1000.0/1000.0, 1);
    tft.setCursor(120,cursor_y);
    tft.print("1h  ");tft.println(S3600.display_max_speed* calibration_speed,1);
    InfoBar(0);
  }
}

#else

#endif