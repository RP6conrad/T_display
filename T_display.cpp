#include "T_display.h"
#include "Definitions.h"
#include <LITTLEFS.h>

#ifndef T5_E_PAPER
char time_now[8]="00:00";
char time_now_sec[12];
int sat_sprite_position;
//void Boot_screen(void){};

//void Sleep_screen(int choice){};
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
  sprite.fillRoundRect(offset + posX, posY, 0.5 * batW, 0.25 * batW, 0,color);                 //battery top
  sprite.fillRoundRect(offset + posX - 0.25 * batW, posY + 0.25 * batW, batW, batL,0,color);  //battery body
  if (bat_perc < 67){sprite.fillRoundRect(offset + posX - 0.25 * batW + line, posY + 0.25 * batW + line, segW, segL,0, TFT_BLACK);}
  if (bat_perc < 33){sprite.fillRoundRect(offset + posX - 0.25 * batW + line, posY + 0.25 * batW + line + 1 * (segL + 1), segW, segL,0, TFT_BLACK);}
  if (bat_perc < 5) {sprite.fillRoundRect(offset + posX - 0.25 * batW + line, posY + 0.25 * batW + line + 2 * (segL + 1), segW, segL,0, TFT_BLACK);}

  if (bat_perc < 100) sprite.setCursor(120,114);//was 193
  else sprite.setCursor(104,114);//was 184
  
  sprite.print(RTC_voltage_bat+0.04,1);
  sprite.print("V ");
  sprite.print(int(bat_perc));
  sprite.print("%");
}
void Sats_level(int offset) {
  int posX = 95;//was 176
  int posY = 114;  //-(circelL+2*circelS);
  int satnum = ubxMessage.navPvt.numSV;
  sprite.drawBitmap( posX, posY,ESP_Sat_22, 22, 22,TFT_BLACK,TFT_GREEN);
  sprite.setCursor(posX - 25, posY);//tft.setCursor(posX - (satnum < 10 ? 10 : 19), posY);
  sprite.print(ubxMessage.navPvt.numSV);
}
int Time(int offset) {
  if (!update_time()) {
    sprite.setCursor(offset, 114);
    sprite.print(time_now);
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

void InfoBar(int offset) {
  sprite.unloadFont();
  sprite.loadFont(Aerial20);
  sprite.setTextColor(TFT_GREEN);
  Bat_level_Simon(offset);
  Sats_level(offset);
  //if(ubxMessage.navPvt.numSV>4) M8_M10(offset);
  Time(offset);
  //sprite.pushSprite(0,0);
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
 void Stats_4lines(String Message1, String Message2, String Message3, String Message4, float Value1, float Value2, float Value3, float Value4) {
  #define STAT4_ROW2 130
  #define DIS_WIDTH 240
    sprite.fillSprite(TFT_BLACK);
    sprite.setCursor(0,0);
    sprite.unloadFont();
    sprite.loadFont(Aerial30);
    sprite.setTextColor(TFT_GREEN);
    sprite.print(Message1);sprite.setCursor(STAT4_ROW2,0);sprite.println(Value1,1);
    sprite.drawLine(0, sprite.getCursorY() - 4,DIS_WIDTH, sprite.getCursorY() - 4, TFT_PINK);
    sprite.print(Message2);sprite.setCursor(STAT4_ROW2,sprite.getCursorY());sprite.println(Value2,1);
    sprite.drawLine(0, sprite.getCursorY() - 4,DIS_WIDTH, sprite.getCursorY() - 4, TFT_PINK);
    sprite.print(Message3);sprite.setCursor(STAT4_ROW2,sprite.getCursorY());sprite.println(Value3,1); 
    sprite.drawLine(0, sprite.getCursorY() - 4,DIS_WIDTH, sprite.getCursorY() - 4, TFT_PINK);
    sprite.print(Message4);sprite.setCursor(STAT4_ROW2,sprite.getCursorY());sprite.println(Value4,1); 
    sprite.drawLine(0, sprite.getCursorY() - 4,DIS_WIDTH, sprite.getCursorY() - 4, TFT_PINK); 
    InfoBar(0); 
    sprite.pushSprite(0,0); 
  }  
void Best_5_runs(String Message, GPS_speed M) {
  sprite.unloadFont();
  sprite.fillSprite(TFT_BLACK);
  sprite.setCursor(0, 0);
  for (int i = 0; i < 5; i++) {
    sprite.loadFont(Aerial20);
    sprite.print(Message);
    sprite.unloadFont();
    sprite.loadFont(Aerial30);
    sprite.print(M.avg_speed[9 - i] * calibration_speed, 1);
    sprite.unloadFont();
    sprite.loadFont(Aerial20);
    sprite.print("@");
    sprite.unloadFont();
    sprite.loadFont(Aerial30);
    sprite.printf("%02d:%02d\n",M.time_hour[9 - i],M.time_min[9 - i]);
    sprite.drawLine(0, sprite.getCursorY() - 6, 240, sprite.getCursorY() - 6, TFT_RED);
  }
  sprite.pushSprite(0,0);
}
void Best_5_runs_S(String Message, GPS_time S) {
  sprite.unloadFont();
  sprite.fillSprite(TFT_BLACK);
  sprite.setCursor(0, 0);
  for (int i = 0; i < 5; i++) {
    sprite.loadFont(Aerial20);
    sprite.print(Message);
    sprite.unloadFont();
    sprite.loadFont(Aerial30);
    sprite.print(S.avg_speed[9 - i] * calibration_speed, 1);
    sprite.unloadFont();
    sprite.loadFont(Aerial20);
    sprite.print("@");
    sprite.unloadFont();
    sprite.loadFont(Aerial30);
    sprite.printf("%02d:%02d\n",S.time_hour[9 - i],S.time_min[9 - i]);
    sprite.drawLine(0, sprite.getCursorY() - 6, 240, sprite.getCursorY() - 6, TFT_BLUE);
  }
  sprite.pushSprite(0,0);
}
void Boot_Screen1(void){
  if((!sdOK)&(!LittleFS_OK)){
    tft.init();           // Initialize ST7789 240x135
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    sprite.createSprite(TFT_HEIGHT,TFT_WIDTH);
    sprite.fillScreen(TFT_BLACK);
    pinMode(TFT_BL, OUTPUT);      // TTGO T-Display enable Backlight pin 4
    digitalWrite(TFT_BL, HIGH);   // T-Display turn on Backlight
    sprite.setCursor(0, 0);
    sprite.setTextWrap(false);
    sprite.setTextColor(TFT_YELLOW,TFT_BLACK);
    sprite.unloadFont();
    sprite.loadFont(Noto_Sans_Bold26);
    //sprite.setTextFont(4);
    sprite.println("T-Display ESP-GPS");
    for (int pos=0;pos<240;pos++){
      sprite.drawBitmap( pos, 60,ESP_GPS_logo,48 , 48,TFT_BLACK,TFT_RED);
      sprite.drawLine(pos,60,pos,60+48,TFT_BLACK);
      sprite.pushSprite(0,0);
      }
    }
    else{
      int Mbytes=freeSpace;
      sprite.setCursor(0,0);
      sprite.setTextColor(TFT_YELLOW);
      sprite.fillScreen(TFT_BLACK);
      sprite.println("T-Display ESP-GPS");
      sprite.setTextColor(TFT_GREEN);
      if(LittleFS_OK) sprite.print("Flash free:");
      if(sdOK) sprite.print("SD free:");
      sprite.print(Mbytes);
      sprite.println("Mb   ");
      cursor_y=sprite.getCursorY(); 
      Bat_level_Simon(0);
      sprite.pushSprite(0,0); 
    }   
}
void Off_screen(int choice){
sprite.fillSprite(TFT_BLACK);
sprite.unloadFont();
sprite.loadFont(Noto_Sans_Bold26);
sprite.setCursor(0,0);
sprite.println("ESP-GPS saving");
sprite.println("Saving session");
 float session_time = (millis() - start_logging_millis) / 1000;
sprite.print("Time: ");sprite.print(session_time, 0);sprite.println(" s");
sprite.print("AVG: ");sprite.println(RTC_avg_10s, 2);
sprite.print("Dist: ");sprite.println(Ublox.total_distance / 1000, 0);
sprite.pushSprite(0,0);
}

//****************************************************************************************************************************************  
void Update_screen(int screen){
  /*
  static int Old_Screen;
  if(Old_Screen!=screen)tft.fillScreen(TFT_BLACK);  
  Old_Screen=screen;
  */
  if(screen==WIFI_STATION){
    int Mbytes=freeSpace;
    sprite.fillSprite(TFT_BLACK);
    sprite.setCursor(0, 0);
    sprite.setTextColor(TFT_GREEN);
    if (Mbytes < 5) sprite.setTextColor(TFT_YELLOW, TFT_BLACK);
    if (Mbytes < 2) sprite.setTextColor(TFT_RED, TFT_BLACK);
    sprite.loadFont(Noto_Sans_Bold26);
    if (LittleFS_OK) sprite.print("Flash free:");
    if (sdOK) sprite.print("SD free:");
    sprite.print(Mbytes);
    sprite.println("Mb   ");
    sprite.setTextColor(TFT_GREEN, TFT_BLACK);
    sprite.print("Search for ");
    sprite.print(wifi_search);
    sprite.println(" s    ");
    sprite.print(actual_ssid);
    sprite.println("             ");
    sprite.pushSprite(0,0);
    }
//**************************************************************************************************************************************************       
  if(screen==WIFI_SOFT_AP){
    sprite.fillSprite(TFT_BLACK);
    sprite.print("AP active for: ");
    sprite.print(wifi_search);
    sprite.println("     ");
    sprite.print(soft_ap_ssid);
    sprite.println("       ");
    sprite.pushSprite(0,0);
    }
//**************************************************************************************************************************************************    
  if(screen==WIFI_ON) {
    int Mbytes=freeSpace;
    sprite.fillSprite(TFT_BLACK);
    sprite.setCursor(0, 0);
    sprite.setTextColor(TFT_WHITE);
    sprite.unloadFont();
    sprite.loadFont(Noto_Sans_Bold26);
    if (LittleFS_OK) sprite.print("Flash free:  ");
    if (sdOK) sprite.print("SD free:  ");
    sprite.print(Mbytes);
    sprite.println("Mb   ");
    sprite.println("Connected to Wifi 2.4GHz ");
    sprite.println(actual_ssid);
    sprite.print("IP: ");
    sprite.println(IP_adress);
    sprite.setTextColor(TFT_CYAN);
    InfoBar(0);
    sprite.pushSprite(0,0);

    //if(millis()%6000<3000)digitalWrite(TFT_BL, HIGH);   // T-Display turn on Backlight
    //else digitalWrite(TFT_BL, LOW);   // T-Display turn on Backlight
  } 
//*************************************************************************************************************************************************

  if (screen == GPS_INIT_SCREEN) {
    sprite.fillSprite(TFT_BLACK);
    sprite.unloadFont();
    sprite.loadFont(Noto_Sans_Bold26);
    if (config.ublox_type == 0xFF) {
      sprite.setCursor(0, 0);
      sprite.print("Auto detect gps-type");
    } else if (!ubxMessage.monVER.hwVersion[0]) {
      sprite.setCursor(0, 0);
      sprite.println("Gps initializing");
      sprite.drawBitmap(sat_sprite_position, 58, ESP_Sat_22, 22, 22, TFT_BLACK, TFT_YELLOW);
      sprite.drawLine(sat_sprite_position, 58, sat_sprite_position, 58 + 22, TFT_BLACK);
      sat_sprite_position++;
      if (sat_sprite_position > 240) sat_sprite_position = 0;
      //delay(2);
    } else {
      sprite.setCursor(0, 0);
      sprite.println("Gps initializing...");
      sprite.printf("%s at %dHz", gpsChip(1), config.sample_rate);
      sprite.println("");
      if (ubxMessage.navPvt.numSV < 5) {
        int color_sat = TFT_RED;
        if (ubxMessage.navPvt.numSV > 1) color_sat = TFT_MAGENTA;
        if (ubxMessage.navPvt.numSV > 3) color_sat = TFT_YELLOW;
        sprite.print("Waiting for Sat>=5");
        sprite.drawBitmap(sat_sprite_position, 150, ESP_Sat_22, 22, 22, TFT_BLACK, color_sat);
        // sprite.drawLine(sat_sprite_position,150,sat_sprite_position,150+22,TFT_BLACK);
        sat_sprite_position++;
        if (sat_sprite_position > 536) sat_sprite_position = 0;
        delay(10);
      } else {
        sprite.println("Ready for action !       ");
        sprite.print("Move > then ");
        if ((int)(calibration_speed * 100000) == 194) {
          sprite.print(config.start_logging_speed * 1.94384449, 1);
          sprite.print("kn");
        }
        if ((int)(calibration_speed * 1000000) == 3600) {
          sprite.print(config.start_logging_speed * 3.6, 1);
          sprite.print("km/h");
        }
      }
    }
    InfoBar(0);
    sprite.pushSprite(0,0); 
  }   
//*********************************************************************************************************************************************************************
  if(screen==SPEED) {
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextColor(TFT_PINK);
  sprite.unloadFont();
  sprite.loadFont(Noto_Sans_Bold26);
  sprite.setCursor(0,0);
  sprite.print("Run");
  sprite.unloadFont();
  sprite.loadFont(Aerial30);
  sprite.print(S10.s_max_speed * calibration_speed,1);
  sprite.unloadFont();
  sprite.loadFont(Noto_Sans_Bold26);
  sprite.setCursor(120,0);
  sprite.print("Avg");
  sprite.unloadFont();
  sprite.loadFont(Aerial30);
  sprite.println(S10.avg_5runs * calibration_speed,1);
  sprite.setCursor(0,120);
  sprite.setTextColor(TFT_WHITE);
  sprite.unloadFont();
  sprite.setFreeFont(&FreeSansBold60pt7b);
  //sprite.setTextSize(2);
  //sprite.setFreeFont(&SansSerif_bold_96_nr);
  sprite.print(gps_speed * calibration_speed,1);
  //sprite.setTextSize(2);
  sprite.pushSprite(0,0);
  }
//****************************************************************************************************************************************************************************
  if(screen==STATS1){ 
    #define ROW1 60
    #define ROW2 120
    #define ROW3 180
    sprite.fillSprite(TFT_BLACK);
    sprite.unloadFont();
    sprite.loadFont(Noto_Sans_Bold26);
    sprite.setTextColor(TFT_PINK);
    sprite.setCursor(0,0);
    sprite.print("AVG ");sprite.setCursor(ROW1,0);sprite.print(S10.avg_5runs * calibration_speed,1);
    sprite.setCursor(ROW2,0);
    sprite.print("2s ");sprite.setCursor(ROW3,0);sprite.println(S2.display_max_speed * calibration_speed,1);
    sprite.print("10sF");sprite.setCursor(ROW1,sprite.getCursorY());sprite.print(S10.display_max_speed * calibration_speed,1);
    sprite.setCursor(ROW2,sprite.getCursorY());
    sprite.print("10sS");sprite.setCursor(ROW3,sprite.getCursorY());sprite.println(S10.display_speed[5] * calibration_speed,1);
    sprite.print("Alfa");sprite.setCursor(ROW1,sprite.getCursorY());sprite.print(A500.avg_speed[9] * calibration_speed,1);
    sprite.setCursor(ROW2,sprite.getCursorY());
    sprite.print("NM");sprite.setCursor(ROW3,sprite.getCursorY());sprite.println(M1852.display_max_speed* calibration_speed,1);
    sprite.print("Dis");sprite.setCursor(ROW1,sprite.getCursorY());sprite.print(Ublox.total_distance / 1000.0/1000.0, 1);
    sprite.setCursor(ROW2,sprite.getCursorY());
    sprite.print("1h");sprite.setCursor(ROW3,sprite.getCursorY());sprite.println(S3600.display_max_speed* calibration_speed,1);
    InfoBar(0);
    sprite.pushSprite(0,0);
  }
if (screen == STATS2) {
    Stats_4lines("Dist: ", "1852m: ", "1hour: ", "Alfa: ", Ublox.total_distance / 1000000, M1852.display_max_speed * calibration_speed, S3600.display_max_speed * calibration_speed, A500.avg_speed[9] * calibration_speed);
  }
if (screen == STATS3) {
    Stats_4lines("100m: ", "250m: ", "500m: ", "Alfa: ",M100.display_max_speed * calibration_speed, M250.display_max_speed * calibration_speed,M500.display_max_speed * calibration_speed, A500.avg_speed[9] * calibration_speed);
  }
if(screen==STATS4) {Best_5_runs("500m: ", M500) ;}
if(screen==STATS5) {Best_5_runs_S("Run: ", S10) ;}
if (screen == STATS6) {
    #define STAT_ROW1 60
    #define STAT_ROW2 120
    #define STAT_ROW3 180
    #define DIS_WIDTH 240
    sprite.fillSprite(TFT_BLACK);
    sprite.unloadFont();
    sprite.loadFont(Aerial20);
    sprite.setCursor(0, 0);
    sprite.setTextColor(TFT_WHITE);
    sprite.print("AVG: ");
    sprite.setCursor(STAT_ROW1, 0);
    sprite.println(S10.avg_5runs * calibration_speed, 2);
    sprite.drawLine(0, sprite.getCursorY() - 4,DIS_WIDTH, sprite.getCursorY() - 4, TFT_CYAN);
    sprite.setTextColor(TFT_LIGHTGREY);
    sprite.print("R1: ");
    sprite.setCursor(STAT_ROW1, sprite.getCursorY());
    sprite.println(S10.display_max_speed * calibration_speed, 2);
    sprite.drawLine(0, sprite.getCursorY() - 4, DIS_WIDTH, sprite.getCursorY() - 4, TFT_CYAN);
    sprite.print("R2: ");
    sprite.setCursor(STAT_ROW1, sprite.getCursorY());
    sprite.println(S10.display_speed[8] * calibration_speed, 2);
    sprite.drawLine(0, sprite.getCursorY() - 4, DIS_WIDTH, sprite.getCursorY() - 4, TFT_CYAN);
    sprite.print("R3: ");
    sprite.setCursor(STAT_ROW1, sprite.getCursorY());
    sprite.println(S10.display_speed[7] * calibration_speed, 2);
    sprite.drawLine(0, sprite.getCursorY() - 4, DIS_WIDTH, sprite.getCursorY() - 4, TFT_CYAN);
    sprite.print("R4: ");
    sprite.setCursor(STAT_ROW1, sprite.getCursorY());
    sprite.println(S10.display_speed[6] * calibration_speed, 2);
    sprite.drawLine(0, sprite.getCursorY() - 4, DIS_WIDTH, sprite.getCursorY() - 4, TFT_CYAN);
    sprite.print("R5: ");
    sprite.setCursor(STAT_ROW1, sprite.getCursorY());
    sprite.println(S10.display_speed[5] * calibration_speed, 2);
    sprite.drawLine(0, sprite.getCursorY() - 4, DIS_WIDTH, sprite.getCursorY() - 4, TFT_CYAN);
    sprite.setTextColor(TFT_WHITE);
    sprite.setCursor(STAT_ROW2, 0);
    sprite.print("2sec: ");
    sprite.setCursor(STAT_ROW3, sprite.getCursorY());
    sprite.println(S2.display_max_speed * calibration_speed, 2);
    sprite.setCursor(STAT_ROW2, sprite.getCursorY());
    sprite.print("Dist: ");
    sprite.setCursor(STAT_ROW3, sprite.getCursorY());
    sprite.println(Ublox.total_distance / 1000.0 / 1000.0, 2);
    sprite.setCursor(STAT_ROW2, sprite.getCursorY());
    sprite.print("Alfa: ");
    sprite.setCursor(STAT_ROW3, sprite.getCursorY());
    sprite.println(A500.avg_speed[9] * calibration_speed, 2);
    sprite.setCursor(STAT_ROW2, sprite.getCursorY());
    sprite.print("1h: ");
    sprite.setCursor(STAT_ROW3, sprite.getCursorY());
    sprite.println(S3600.display_max_speed * calibration_speed, 2);
    sprite.setCursor(STAT_ROW2, sprite.getCursorY());
    sprite.print("NM: ");
    sprite.setCursor(STAT_ROW3, sprite.getCursorY());
    sprite.println(M1852.display_max_speed * calibration_speed, 2);
    sprite.setCursor(STAT_ROW2, sprite.getCursorY());
    sprite.print("500m: ");
    sprite.setCursor(STAT_ROW3, sprite.getCursorY());
    sprite.println(M500.display_max_speed * calibration_speed, 2);
    InfoBar(0);
    sprite.pushSprite(0,0); 
  }
}  
#else

#endif