/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and youre
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // define the Arduino pins used to connect with the LCD pins RS, EN, D4 to D7
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//initialize the library by associating any needed LCD interface pinwith the arduino pin number it is connected to

#include <NewPing.h> // Include  library used for measuring the distance using HC-SR 06 sonar sensor

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#define RELAYPIN 8 // Arduino pin tied to Relaypin of the Relay Circuit
#define EXTRELAYPIN 13 // Arduino pin tied to vin pin of the External Relay Circuit
#define BUZZER 9 // Arduino pin tied to +ve terminal of the Buzzer
float val;
#include <EEPROM.h>
int addr = 0;
int addr2 = 1;
int flag;
byte readval;

#define buttonPin 10
int buttonState = 0;         // variable for reading the pushbutton status
float TankHeight, MaxWaterLevel, EmptySpace, SonarReading, ActualReading,  Temp;
int percentage;
int SpmpSensorPin = A0;    // set A0 as the Spump water sensor pin
int SpmpsensorValue = 0;  // variable to store the value coming from the sensor

// Creating Charaters for Bar Graph and Reverse Mode Icon
byte Level0[8] = {
        0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b11111,
 0b11111
};
byte Level1[8] = {
        0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b11111,
 0b11111,
 0b11111,
 0b11111
};
byte Level2[8] = {
        0b00000,
 0b00000,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111
};
byte Level3[8] = {
        0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111
};
byte NoLevel[8] = {
        0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
}; 

void setup() {
  Serial.begin(9600);
  lcd.createChar(0, Level0);
  lcd.createChar(1, Level1);
  lcd.createChar(2, Level2);
  lcd.createChar(3, Level3);
  lcd.createChar(4, NoLevel);
  Serial.println(flag);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  pinMode(RELAYPIN,OUTPUT);// Relay pin as output pin
  pinMode(EXTRELAYPIN,OUTPUT);// External Relay pin as output pin
  digitalWrite(RELAYPIN,LOW); //Turn off the relay
  digitalWrite(EXTRELAYPIN,HIGH); //Turn off the external relay(External Relay I used was turning on while giving LOW signal, Check your one while doing the program)
  pinMode(BUZZER,OUTPUT);// Buzzer pin as output pin
  digitalWrite(BUZZER,LOW);  //Turn off the Buzzer
  lcd.print("Automatic  Water");
  lcd.setCursor(0,1);
  lcd.print("*Pumping System*");
  delay(2000);
  lcd.clear();
   lcd.print("Long Press Button to Change Tank Height");
  delay(1000);
  for (int positionCounter = 0; positionCounter < 23; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }
delay(1000);
lcd.clear();
lcd.print("Tank must be Empty!");
 delay(1000);
   for (int positionCounter = 0; positionCounter < 3; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }
 delay(1000); 
 lcd.clear();
 
   
   lcd.print("Change Tnk Height");
   
for (int i=0; i<=5; i++)
{
  lcd.setCursor(0,1);
  lcd.print("  in:");
  lcd.print(5-i);
  lcd.print(" Seconds");
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) 
  {
  TankHeight =sonar.ping_cm();
  EEPROM.write(addr, TankHeight);
  }
delay(1000);
}
 
  TankHeight= EEPROM.read(addr);
  lcd.clear();
  lcd.print("Tnk Height Set:");
  lcd.setCursor(0,1);
  lcd.print("    ");
  lcd.print(TankHeight);
  lcd.print("cm    ");
  delay(2000);
  lcd.clear();
  MaxWaterLevel=0.85*TankHeight;
  EmptySpace=TankHeight-MaxWaterLevel;
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(50); // Wait 100ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  SonarReading=sonar.ping_cm();
  SpmpsensorValue=analogRead(SpmpSensorPin);
  Serial.println(SpmpsensorValue);
  Temp= SonarReading-EmptySpace;
  ActualReading= MaxWaterLevel-Temp;
  percentage=(ActualReading/MaxWaterLevel*100);
  lcd.setCursor(8,0);
  lcd.print("Lev:");
  lcd.print(percentage);
  lcd.print("%    ");
  lcd.setCursor(0, 1);
  lcd.print("LOW");
  lcd.setCursor(11, 1);
  lcd.print("HIGH ");
  if(SpmpsensorValue>=100)
  {
  if(percentage<=20)
  {
  lcd.setCursor(0,0);
  lcd.print("PMP ON  ");
  digitalWrite(RELAYPIN,HIGH);
  digitalWrite(EXTRELAYPIN,LOW);
  flag=1;
  EEPROM.write(addr2, flag);
  flag= EEPROM.read(addr2);
  ZeroPercentage();
  }
  else if(percentage>20 && percentage<=100)
  {
  flag= EEPROM.read(addr2);
  if(percentage>20 && percentage<=100 && flag ==1)
  {
    digitalWrite(RELAYPIN,HIGH);
    digitalWrite(EXTRELAYPIN,LOW);
    lcd.setCursor(0,0);
    lcd.print("PMP ON  ");
    if(percentage>20 && percentage<=25)
      {
        TwentyFivePercentage();
      }
      else if(percentage>25 && percentage<=50)
      {
        FiftyPercentage();
      }
          
    else if(percentage>50 && percentage<=75)
      {
         SeventyFivePercentage();   
      }
    else if(percentage>75 && percentage<=100)
      {
          HundredPercentage();
      }
    }
   else if(percentage>20 && percentage<=100 && flag ==0)
  {
    digitalWrite(RELAYPIN,LOW);
    digitalWrite(EXTRELAYPIN,HIGH);
    lcd.setCursor(0,0);
    lcd.print("PMP OFF ");
    if(percentage>20 && percentage<=25)
      {
        
        TwentyFivePercentage();
    
      }
      else if(percentage>25 && percentage<=50)
      {
        FiftyPercentage();
      }
      
      
    else if(percentage>50 && percentage<=75)
      {
  
        SeventyFivePercentage();
      }
    else if(percentage>75 && percentage<=100)
      {
        HundredPercentage();
      
      }
    }
   }
  
  else if(percentage>100)
  {
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("PMP OFF ");
  lcd.setCursor(8,0);
  lcd.print("Lev:");
  lcd.print("100");
  lcd.print("%    ");
  digitalWrite(RELAYPIN,LOW);
  digitalWrite(EXTRELAYPIN,HIGH);
  flag=0;
  EEPROM.write(addr2, flag);
  flag= EEPROM.read(addr2);
  HundredPercentage();
    }
  }
 else if(SpmpsensorValue<=100)
{
  flag= EEPROM.read(addr2);
  if(flag==1)
  { lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*SumTank Empty*");
    lcd.setCursor(0,1);
    lcd.print("*Pump kept OFF*");
    digitalWrite(BUZZER,HIGH);
    digitalWrite(RELAYPIN, LOW);
    digitalWrite(EXTRELAYPIN, HIGH);
    delay(100);
    digitalWrite(BUZZER,LOW);
    delay(100);
  }
  else if(flag==0)
  {
  lcd.setCursor(0,0);
  lcd.print("STnk MT ");
    
      if(percentage<=20)
      {
        ZeroPercentage();
      }  
      else if(percentage>20 && percentage<=25)
      {
        TwentyFivePercentage();
      }
      else if(percentage>25 && percentage<=50)
      {
        FiftyPercentage();
      }
      
      
    else if(percentage>50 && percentage<=75)
      {
         SeventyFivePercentage();   
      }
    else if(percentage>75 && percentage<=100)
      {
          HundredPercentage();
      }
    else if(percentage>100)
      {
         HundredPercentage();
      }
  }
}
}
void ZeroPercentage()
{
  lcd.setCursor(3, 1); 
  lcd.write(byte(4));
  lcd.setCursor(4, 1); 
  lcd.write(byte(4));
  lcd.setCursor(5, 1); 
  lcd.write(byte(4));
  lcd.setCursor(6, 1); 
  lcd.write(byte(4));
  lcd.setCursor(7, 1);
  lcd.write(byte(4));
  lcd.setCursor(8, 1);
  lcd.write(byte(4));
  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(4));
}
void TwentyFivePercentage()
{
  lcd.setCursor(3, 1); 
  lcd.write(byte(0));
  lcd.setCursor(4, 1); 
  lcd.write(byte(0));
  lcd.setCursor(5, 1); 
  lcd.write(byte(4));
  lcd.setCursor(6, 1); 
  lcd.write(byte(4));
  lcd.setCursor(7, 1);
  lcd.write(byte(4));
  lcd.setCursor(8, 1);
  lcd.write(byte(4));
  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(4));
}
void FiftyPercentage()
{
  lcd.setCursor(3, 1); 
  lcd.write(byte(0));
  lcd.setCursor(4, 1); 
  lcd.write(byte(0));
  lcd.setCursor(5, 1); 
  lcd.write(byte(1));
  lcd.setCursor(6, 1); 
  lcd.write(byte(1));
  lcd.setCursor(7, 1);
  lcd.write(byte(4));
  lcd.setCursor(8, 1);
  lcd.write(byte(4));
  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(4)); 
}
void SeventyFivePercentage()
{
  lcd.setCursor(3, 1); 
  lcd.write(byte(0));
  lcd.setCursor(4, 1); 
  lcd.write(byte(0));
  lcd.setCursor(5, 1); 
  lcd.write(byte(1));
  lcd.setCursor(6, 1); 
  lcd.write(byte(1));
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  lcd.setCursor(8, 1);
  lcd.write(byte(2));
  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(4));
  
}
void HundredPercentage()
{
  lcd.setCursor(3, 1); 
  lcd.write(byte(0));
  lcd.setCursor(4, 1); 
  lcd.write(byte(0));
  lcd.setCursor(5, 1); 
  lcd.write(byte(1));
  lcd.setCursor(6, 1);
  lcd.write(byte(1));
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  lcd.setCursor(8, 1);
  lcd.write(byte(2));
  lcd.setCursor(9, 1);
  lcd.write(byte(3));
  lcd.setCursor(10, 1);
  lcd.write(byte(3));
}


