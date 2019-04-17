#include <QuadDisplay.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BLACK   0x0000
#define BLUE    tft.color565(0, 0, 120)
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA tft.color565(200, 0, 170)
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define release 1
#define w 320
#define h 240
#define qd 12
#define WaitData(){\
    while (Serial.available() <= 0)\
      delay(1);\
  }\


#define networks 5
  String wifi[networks] = { "CarlsHide", "HTC111", "OdinokyMacho", "MEIZUMEIZU", "NewWifi"};
  String passwordWifi[networks] = {"Car555Site", "314159265", "314159265", "ragooragoo", "1111"};
  String host = "snajpers159.000webhostapp.com";
  String link[3] = {"GetVk.php", "GetInst.php", "GetFb.php"};

  String cmdWifi;
  String cmdHost = "AT+CIPSTART=\"TCP\",\"" + host + "\",80";
  String cmdLink;
  String ErrMessage = "I can't connect wifi,\ntry to make this network:\nName: " + wifi[networks - 1] + "\nPassword:" + passwordWifi[networks - 1] + "\n\nor just restart)";

  word subscribers[3] = {0};
  byte socNetwork = 0;
  byte numwifi = 0;

  String makeWifi() {
    String toReturn  =  "AT+CWJAP=\"";
    toReturn        +=  wifi[numwifi];
    toReturn        +=  "\",\"";
    toReturn        +=  passwordWifi[numwifi];
    toReturn        +=  "\"";

    return toReturn;
  }

  String makeGet() {
    String toReturn  =  "GET /";
    toReturn        +=  link[socNetwork];
    toReturn        +=  " HTTP/1.1\r\nHost:";
    toReturn        +=  host;
    toReturn        += "\r\n\r\n";

    return toReturn;
  }

  void setup()
  {
    Serial.begin(115200);
    displayClear(qd);
    tft.reset();
    tft.begin(0x9341);
    tft.setRotation(1);
#if release
    Loading();
#else
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
#endif
    cmdLink = makeGet();
    Serial.println("ATE0");
    Serial.readString();
  }

  void loop() {

    String input;
    int stat = getStatus();
    numwifi = 0;
    //find active wifi network
    while (numwifi < networks && (stat == 5 || stat == -1)) // 5 - not connected; -1 - parsing error
    {
      cmdWifi = makeWifi();
#if release == 0
      tft.println(cmdWifi);
#endif
      Serial.println(cmdWifi);
      WaitData();
      stat = getStatus();
#if release
      Serial.readString();
#else
      tft.println(Serial.readString());
#endif
      if (stat == 5)
        ++numwifi;
      if (numwifi == networks)
        ShowError(ErrMessage);
    }


    stat = 0;
    while (!stat)
    {
      //connect to web-host
      while (stat != 3)
      {
        Serial.println(cmdHost);
        WaitData();
        stat = getStatus();
#if release
        Serial.readString();
#else
        tft.println(Serial.readString());
#endif
      }

      //send request to host
      stat = 0;
      Serial.print("AT+CIPSEND=");  //Request to send data
      Serial.println(cmdLink.length());
      if (Serial.find(">"))
      {
        Serial.print(cmdLink); //send http
        stat = !readResult();
#if release
        if (stat)
          ShowResult();
#endif
        socNetwork = (socNetwork + 1) % 3;
        cmdLink = makeGet();
      }
      else
        Serial.println("AT+CIPCLOSE");
    }

#if release == 0
    delay(3000);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
#endif
  }

  int readResult()
  {
#if release == 0
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
#endif

    String input = Serial.readStringUntil('\n');
    while (!input.startsWith("CLOSED"))
    {
      if (input.startsWith("$"))
      {
        Serial.println("AT+CIPCLOSE");
        input.replace("$", "");

#if release
        Serial.flush();
#else
        tft.println(Serial.readString());
#endif

        if (input.toInt())
        {
          subscribers[socNetwork] = input.toInt();
          return 0;
        }
        else
          return 1;
      }
      input = Serial.readStringUntil('\n');

#if release == 0
      tft.println(input);
#endif
    }
    return 2;
  }

  int getStatus() {
    Serial.println("AT+CIPSTATUS");
    WaitData();
    String input = Serial.readStringUntil('\n');
    while (!input.startsWith("STATUS:"))
    {
      if (input.startsWith("busy"))
      {
        delay(500);
        Serial.println("AT+CIPSTATUS");
        WaitData();
      }
      input = Serial.readStringUntil('\n');
    }
    
#if release == 0
      tft.println(input);
#endif
    input.replace("STATUS:", "");
    return input.toInt();
  }

  void ShowResult()
  {
    switch (socNetwork)
    {
      case 0:
        Vk();
        break;
      case 1:
        Instagram();
        break;
      case 2:
        Facebook();
        break;
    }
    displayInt(qd, subscribers[socNetwork]);
    delay(6000);
  }

  void Vk()
  {
    tft.fillScreen(BLUE);
    tft.fillRoundRect(50, 40, 220, 160, 7, WHITE);
    int i = 40;
    int w1 = (w >> 1) + 15;
    tft.fillTriangle(50, 48, 50, 200, w1 - i, 200, BLUE);
    tft.fillCircle((w >> 1) + 10, (h >> 1) + 8, 79, WHITE);
    tft.fillTriangle(50, 48, 94, 150, w1 - i, 150, WHITE);
    tft.fillTriangle(50 + i, 40, w1 - i, (h >> 1) + 30, w1 - i, 40, BLUE);
    tft.fillRect((w >> 1) - 40, (h >> 1) - 4, 20, 40, WHITE);
    tft.fillCircle((w >> 1) - 32, (h >> 1) - 5, 7, BLUE);
    tft.fillRect(w1 - 10, 40, 10, h - 80,  BLUE);
    tft.fillRoundRect(w1 - 20, 40, 20, h - 80, 8, WHITE);
    tft.fillTriangle(w1, 40, w1, (h >> 1) + 15, w - 50 - i, 40, BLUE);
    tft.fillTriangle(w1, (h >> 1) + 17, w1, h - 40, w - 50 - i, h - 40, BLUE);
    tft.fillRect(w1, (h >> 1) - 8, 30, 44, WHITE);
    tft.fillCircle(w1 + 7, (h >> 1) - 12, 7, BLUE);
    tft.fillCircle(w1 + 8, (h >> 1) + 39, 8, BLUE);
    tft.fillTriangle(w - 50, 48, w1 + i + 5, (h >> 1) + i / 3, w - 50, h - 47, BLUE);
    tft.fillRect(216, (h >> 1) - 10, 20, 45, WHITE);
    tft.fillCircle(263, (h >> 1) + 10, 35, BLUE);
    tft.fillRect(100, h - 40, 150, 40, BLUE);
  }

  void Facebook()
  {
    tft.fillScreen(WHITE);
    int border = (w - h) >> 1;
    tft.fillRoundRect(border, 0, h, h, 15, BLUE);
    int i = 35;
    int h1 = h / 3 - 10;
    tft.fillRect((w >> 1) + 15, h1, i, h - h1, WHITE);
    tft.fillCircle((w >> 1) + 15 + i, h1, i, WHITE);
    tft.fillRoundRect((w >> 1) + 15 + i, h1, 70, 100, 15, BLUE);
    tft.fillRect((w >> 1) + 15 + i, h1 - i, i, i, WHITE);
    tft.fillRect((w >> 1) - 15, (h >> 1) - 10, 100, i, WHITE);
  }

  void Instagram()
  {
    tft.fillScreen(WHITE);
    int border = (w - h) >> 1;
    tft.fillRoundRect(border, 0, h, h, 65, MAGENTA);
    int i = 35;
    tft.fillRoundRect(border + i, i, h - (i << 1), h - (i << 1), 45, WHITE);
    i = 50;
    tft.fillRoundRect(border + i, i, h - (i << 1), h - (i << 1), 35, MAGENTA);
    tft.fillCircle(w >> 1, h >> 1, 45, WHITE);
    tft.fillCircle(w >> 1, h >> 1, 30, MAGENTA);
    tft.fillCircle(205, 75, 10, WHITE);
  }

  void Loading()
  {
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(100, 100);
    tft.print("Loading...");
  }

  void ShowError(String message)
  {
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println(message);
    while (1);
  }

