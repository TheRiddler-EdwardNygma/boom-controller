# boom-controller
A boomer controller made by esp8266

This is an esp8266 project,uploaded by 2024.04.07  17:12

the made-up of the project:
1.main body---esp8266
2.a 360° SG90 servo
(actually the 180° SG90 servo is better to control,because the 180° servo can use the founctions in <Servo.h> to 
define the degree simply,the 360° servo do not have this founction,youcanonly control it using PWM )
3.an oled display
4.a button




the project has a few simple functions:


1.WiFi controlable
 As we know the esp8266 has a characteristic to link WiFi
so we can use some simple functions in <WiFi.h> to control it to link WiFi
and it also have a characteristic to link information-saved WiFi automantically,but this function
is not made up by me,actually i don not know it at first and i have attempted to control the action of linking or quit linking when it
disconnect with WiFi,obviously i was failed

2.all the information will be posted on the oled display
use the founctions in oledstore to control the oled display the informations
  1.the main page :BOOM CONTROLLER + (if wifi is connected)
  2.the tip of WiFi connected/disconnected
  3.the tip of hold fire/ready to shoot/fire


3.the multiple functions of a single button

the button has different functions through different sence
on the main page,short click to enter the second page for ready to shoot statemant
on the seconed page (ready/hold),short click to change the servo up or down circularly (represented the loop of ready to shoot/hold fire),
  and on the hold fire page,long click to return to the mainpage,and on the ready to shoot page ,long click to enter the third page--fire.
on the third page,long click always reference to fire,short click to return to the second page--hold fire


4.the indicate led light
when you are in the main page or the second page of"hold fire",the light is not bright
when you enter the "ready to shoot" statement,the led light will tiny brighted
when you enter the "fire" statement,the light will spark foe twice quickly like a gun which is shooting

5.The simplicity of the code
I have boxed all the codes in their specific functions,the setup function and loop function is really simplified.


here are the codes:


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

