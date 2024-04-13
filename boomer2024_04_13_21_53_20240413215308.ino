#include <Servo.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer;

#define FPSerial Serial

#define OLED_RESET -1 
#define SDA_PIN 12
#define SCL_PIN 14

const char* ssid="Redmi Note 12 Turbo"; 
const char* password="666666666";

WiFiServer server(80);

int val; WiFiClient client;

Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RESET); 
int servoController=4; 
int buttonAdapter=5; 
int ledController=15;

int buttonState=0; 
int buttonState2=0;

boolean up=false; 
boolean WiFiChangeDetector=false; 
boolean WiFiState=false; 
boolean WiFiStateBefore=false;
boolean IsTheFirstTimeToConnect=true;

void ServoHigh (int times)
{ 
  Serial.println("LED ON"); 
  analogWrite(ledController, 5); 
  myDFPlayer.play(2);
  delay(1300);
  for(int i=0;i<times;i++)
  {
     digitalWrite(servoController, HIGH); 
     delayMicroseconds(2400); 
     digitalWrite(servoController, LOW); 
     delayMicroseconds(0); delay(17); 
  }
  myDFPlayer.play(5);
  delay(1500);

} 


void fire()
{ myDFPlayer.play(1);
  delay(1500);
  analogWrite(ledController, 0); 
  delay(100); 
  analogWrite(ledController, 255); 
  delay(100); 
  analogWrite(ledController, 5); 
  delay(100); 
  analogWrite(ledController, 255); 
  delay(300); 
  analogWrite(ledController, 5); 
  myDFPlayer.play(7);
  delay(1300);
  Tip(4); 
  }

void ServoLow(int times)
{ 
  Serial.println("LED OFF"); 
  analogWrite(ledController, 0); 
  myDFPlayer.play(3);
  delay(1300);
  for(int i=0;i<times;i++)
  { 
    digitalWrite(servoController, HIGH); 
    delayMicroseconds(600); 
    digitalWrite(servoController, LOW); 
    delayMicroseconds(400); delay(19);

  } 
  myDFPlayer.play(6);
  delay(1000);
}

void WiFiConnect() 
{ 
  WiFi.begin(ssid,password); 
  if(WiFi.isConnected()==false) 
  { 
    for(int i=0;i<15;i++) 
    { 
      if(WiFi.isConnected()==true) 
      { 
        Serial.println("\n"); 
        Serial.println("WIFI Connected"); 
        Serial.println(WiFi.localIP()); 
        server.begin();
        Tip(1);
        delay(3000);
        Tip(0);
        if(IsTheFirstTimeToConnect==true)
        {
          myDFPlayer.play(4);
          IsTheFirstTimeToConnect=false;
        }
        WiFiState=true;
        break;
  }
  Serial.print("... ");
  delay(1000);
}
  if(WiFiState==false) 
  {
    Tip(2);
    delay(1000);
    Tip(0);
  }
  WiFiStateBefore=WiFiState;
  server.begin();
  WiFiClient client=server.available();
}

}

void OLED_Init() 
{ 
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(WHITE); 
  oled.clearDisplay();

}

void OLED_ShowString(uint8_t x, uint8_t y, uint8_t font_size, String string)
 { 
  oled.setTextSize(font_size);
  oled.setCursor(x, y);
  oled.println(string);
  oled.display();
 }


void Tip(int i) 
{ 
  if(i==0) 
  { 
    if(WiFi.isConnected()==true) 
    { 
      oled.clearDisplay(); 
      OLED_ShowString(4, 7, 3, " BOOM"); 
      OLED_ShowString(100,13,1,"WIFI"); 
      OLED_ShowString(105,22,1,"ON"); 
      OLED_ShowString(4, 50, 2, "CONTROLLER"); 
    } 
    else 
    { 
      oled.clearDisplay(); 
      OLED_ShowString(4, 7, 3, " BOOM"); 
      OLED_ShowString(100,13,1,"WIFI"); 
      OLED_ShowString(103,22,1,"OFF"); 
      OLED_ShowString(4, 50, 2, "CONTROLLER"); 
    }

  }  
  else if(i==1)
  { 
     oled.clearDisplay();
     OLED_ShowString(4, 7, 3, " WIFI"); 
     OLED_ShowString(8, 50, 2, "CONNECTED"); 
  }

  else if(i==2)
  {
    oled.clearDisplay();
    OLED_ShowString(4, 7, 3, " WIFI"); 
    OLED_ShowString(2, 50, 2, "DISCONNECTED"); 
  }

  else if(i==3)
  { 
    oled.clearDisplay(); 
    OLED_ShowString(4, 25, 3, " FIRE");

  } 
  else if(i==4)
  { 
    FireState(up);
  }

}

void FireState(boolean Fire) 
{ 
  if(Fire==true) 
  { 
    oled.clearDisplay(); 
    OLED_ShowString(4, 17, 3, " Ready"); 
    OLED_ShowString(4, 42, 2, " to shoot"); 
  } 
  else 
  { 
    oled.clearDisplay(); 
    OLED_ShowString(8, 17, 3, " Hold"); 
    OLED_ShowString(8, 45, 3, " Fire");
  } 
}

void WiFiTotalConrtoller() 
{ 
  WiFiState=WiFi.isConnected(); 
  if(WiFiState!=WiFiStateBefore) 
  { 
    WiFiChangeDetector=true; 
  }

  if(WiFiChangeDetector==true) 
  { 
    if(WiFiState==true)
    { 
      Tip(1); 
      Serial.println(WiFi.localIP()); 
      server.begin(); 
      WiFiClient client=server.available();
      myDFPlayer.play(4);
      delay(2000);
      Tip(0);
    }

    if(WiFiState==false)
    {
      Tip(2);
      myDFPlayer.play(8);
      delay(2000);
      Tip(0);
    }
    WiFiStateBefore=WiFiState;
    WiFiChangeDetector=false;
  }

  client = server.available();
  if(client)
  { 
    Serial.println("An device is connected to the server."); 
    while(!client.available())
    { 
      delay(1);
    }

    String req=client.readStringUntil('\r');
    Serial.println(req);
    if(req.indexOf("/gpio/0")!=-1) 
    { 
      val=0; 
    }

    else if(req.indexOf("/gpio/1")!=-1) 
    { 
      val=1; 
    }


    if(up==false)
    {
      if(val==1)
      {
        up=true;
        Tip(4);
        ServoHigh(6);
        Serial.println("Ready to shoot");
      }
      else if(val==0)
      {
        oled.clearDisplay();
        Tip(0);
      }

    }
    else if(up==true)
    {
      if(val==1)
      {
        up=false;
        Tip(4);
        ServoLow(6);
        Serial.println("Hold Fire");
      }
      else if(val==0)
      {
        oled.clearDisplay();
        Tip(3);
        fire();
      }

    }
    client.flush();
  } 
}

void ButtonTotalController() 
{ 
  buttonState = digitalRead(buttonAdapter);

  if(up==true) 
  {
    if (buttonState == HIGH) 
    {
      delay(200);
      buttonState = digitalRead(buttonAdapter);

      if(buttonState==HIGH)
      {
        oled.clearDisplay();
        Tip(3);
        fire();
      }
      else
      { up=false;
        Tip(4);
        ServoLow(6);
        Serial.println("Hold fire");

      }
      while(digitalRead(buttonAdapter)==HIGH){}
      delay(50);
    }

  } 
  else 
  {
    if(buttonState == HIGH) 
    {
      delay(200);
      buttonState = digitalRead(buttonAdapter);
      if(buttonState==HIGH)
      {
        oled.clearDisplay();
        Tip(0);
        myDFPlayer.play(9);
        delay(1800);
      }

      else if(up==false)
      {
        up=true;
        Tip(4);
        ServoHigh(6);
        Serial.println("Ready to shoot");
      }
      else
      { 
        up=false;
        Tip(4);        
        ServoLow(6);
        Serial.println("Hold fire");

      }
      while(digitalRead(buttonAdapter)==HIGH){}
      delay(50);
    }
  } 
} 
  


void setup() 
{ 
  Serial.begin(9600); 
  myDFPlayer.begin(FPSerial);
  myDFPlayer.volume(200);
  pinMode(servoController, OUTPUT); 
  pinMode(buttonAdapter, INPUT); 
  pinMode(ledController, OUTPUT); 
  Wire.begin(SDA_PIN, SCL_PIN); 
  OLED_Init(); 
  Tip(0);
  WiFiConnect(); 
}

void loop() {

WiFiTotalConrtoller();

ButtonTotalController();

}