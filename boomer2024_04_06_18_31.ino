#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#define OLED_RESET -1
#define SDA_PIN 12   
#define SCL_PIN 14 

const char* ssid="Redmi Note 12 Turbo";
const char* password="666666666";

WiFiServer server(80);

int val;
WiFiClient client;

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

void ServoHigh (int times){
     Serial.println("LED ON");
     analogWrite(ledController, 5);
  for(int i=0;i<times;i++){
      digitalWrite(servoController, HIGH);
      delayMicroseconds(2400);
      digitalWrite(servoController, LOW);
      delayMicroseconds(0);
      delay(17);
 
  }

}
void fire(){
        analogWrite(ledController, 0);
      delay(100);
      analogWrite(ledController, 255);
      delay(100);
      analogWrite(ledController, 5);
      delay(100);
      analogWrite(ledController, 255);
      delay(300);
      analogWrite(ledController, 5);
      Tip(4);
}

void ServoLow(int times){
      Serial.println("LED OFF");
      analogWrite(ledController, 0);
   for(int i=0;i<times;i++){
      digitalWrite(servoController, HIGH);
      delayMicroseconds(600);
      digitalWrite(servoController, LOW);
      delayMicroseconds(400);
      delay(19);

  }
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
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);     //"SSD1306_SWITCHCAPVCC"表示显示器为OLED ,"0x3C"为OLED屏幕默认通信地址
  oled.setTextColor(WHITE);//开像素点发光
  oled.clearDisplay();//清屏
  
}

void OLED_ShowString(uint8_t x, uint8_t y, uint8_t font_size, String string)
{
  oled.setTextSize(font_size);    //设置字体尺寸 (>=1)
  oled.setCursor(x, y);           //设置显示坐标
  oled.println(string);           //显示内容
  oled.display();                 //开启显示
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
  else if(i==1){
    oled.clearDisplay();
    OLED_ShowString(4, 7, 3, " WIFI");
    OLED_ShowString(8, 50, 2, "CONNECTED");
  }

  else if(i==2){
  
  }

  else if(i==3){
    oled.clearDisplay();
    OLED_ShowString(4, 25, 3, " FIRE");

  }
  else if(i==4){
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
  else {
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
    if(WiFiState==true){
      Tip(1);
      Serial.println(WiFi.localIP());
      server.begin();
      WiFiClient client=server.available();
      

      delay(2000);
      Tip(0);
    }

    if(WiFiState==false){
      Tip(2);
      delay(2000);
      Tip(0);

    }

    WiFiStateBefore=WiFiState;
    WiFiChangeDetector=false;

  }



  client = server.available();  
  if(client){
    Serial.println("An device is connected to the server.");
    while(!client.available()){
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


    if(val==1)
    {
        if(up==false)
      {
        ServoHigh(6);
        Serial.println("Ready to shoot");
        up=true;
        Tip(4);

      }
      else
      {
        ServoLow(6);
        Serial.println("Hold fire");
        up=false;
        Tip(4);
      }

    }
    else if(val==0){
      oled.clearDisplay();
      Tip(0);
    }
    client.flush();

  }
}

void ButtonTotalController()
{
  buttonState = digitalRead(buttonAdapter);

if(up==true)
{

if (buttonState == HIGH) {

    delay(200);
    buttonState = digitalRead(buttonAdapter);

    if(buttonState==HIGH){
      oled.clearDisplay();
      Tip(3);
      fire();
    }
    else
    {
      ServoLow(6);
      Serial.println("Hold fire");
      up=false;
      Tip(4);
    }



    while(digitalRead(buttonAdapter)==HIGH){}
    delay(50);

  }



}
else
{

if(buttonState == HIGH) {

    delay(200);
    buttonState = digitalRead(buttonAdapter);

    if(buttonState==HIGH){
      oled.clearDisplay();
      Tip(0);
    }

    else if(up==false)
    {
      ServoHigh(6);
      Serial.println("Ready to shoot");
      up=true;
      Tip(4);

    }
    else
    {
      ServoLow(6);
      Serial.println("Hold fire");
      up=false;
      Tip(4);
    }



    while(digitalRead(buttonAdapter)==HIGH){}
    delay(50);

}
}
}
void setup() {
  Serial.begin(9600);
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
