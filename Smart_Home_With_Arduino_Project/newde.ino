#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

/*Start read password data*/
#define Password_Length 7      // add password length +1 
char Data[Password_Length]; 
char Master[Password_Length] = "123456"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x20, 16, 2); //a0/a1/a2==0 so the address is 0x20  
/*End read password data*/
/*Start variables*/
Servo servoMotor;            //initialize a servo object for the connected servo
int angelRotate = 0;        // initialize the angle
int motorPin = 10;       
int ledPin = 11;         
int tempPin = A1;
int fanPin = 12;
int temp1;
int temp2;
int fanSpeed;
/*End Variables*/

void setup()
{
  lcd.init();                  //Initialize the lcd 
  lcd.backlight();            //turn on backlight.
  servoMotor.attach(10);      // attach the signal pin of servo to pin10 of arduino
  pinMode(ledPin, OUTPUT);     // Declare the LED as an output
  pinMode (tempPin , INPUT);
  pinMode(fanPin, OUTPUT);
}

void loop()
{
 lcd.setCursor(1,0);
 lcd.print("Enter Password");
 customKey = customKeypad.getKey();
 if (customKey)
  {
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print('*'); 
    data_count++; 
  }
  if(customKey == '#') //clear the data if entered #
  {
    lcd.clear();
    clearData();
  }
  if(data_count == Password_Length-1)
  {
    lcd.clear();
    if(strcmp(Data, Master)  == 0)//corect password
    {
      lcd.setCursor(4,0); 
      lcd.print("Correct");
      delay(600);
 
      open_Servo_Motor(); //open the door
      delay(600);
      open_leds();
      delay(200);
     
      while(!customKeypad.getKey())
      {
        open_Temp_Sensor_Fan();
      }
      close_all_home();
      delay(1000);
    }
    else   //Incorect password
    {
      lcd.setCursor(4,0); 
      lcd.print("Incorrect");
      delay(1000);
    }
    clearData();  
  }
}
///////////functions
void clearData()
{
  lcd.clear();
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}
void open_Servo_Motor()
{
  lcd.clear();
  lcd.setCursor(1,0); 
  lcd.print("Door Openning");

  for(angelRotate = 180; angelRotate>=1; angelRotate-=5)     // command to move from 180 degrees to 0 degrees 
  {          
   analogWrite(motorPin, angelRotate);                   
   servoMotor.write(angelRotate);              //command to rotate the servo to the specified angle
   delay(5);                       
  } 
  return;
}
void open_leds()
{
  lcd.clear();
  lcd.setCursor(2,0); 
  lcd.print("Leds Openning");
  digitalWrite(ledPin, HIGH); // Turn the LED on
  return;
}
void open_Temp_Sensor_Fan()
{
   temp1 = analogRead(tempPin);  // read analog volt from sensor 
   temp1 *= 0.48828125;
   
   if(temp2 != temp1)
   {
      temp2 = temp1;
      lcd.clear();
      lcd.setCursor(1,0); 
      lcd.print("TEMP= ");
      lcd.setCursor(7,0); 
      lcd.print(temp2); // display temperature value
      lcd.print("*C");
      if(temp2 > 38)
        fanSpeed = 20;
      else
        fanSpeed = 5;     
      digitalWrite(fanPin, fanSpeed);
      lcd.setCursor(0,1); 
      lcd.print("Fan Speed = ");
      lcd.setCursor(13,1); 
      lcd.print(fanSpeed);
   }
   return ;
}
void close_all_home()
{
  lcd.clear();
  lcd.setCursor(1,0); 
  lcd.print("Home Closeing");
  digitalWrite(ledPin, LOW); 
  fanSpeed = 0;
  digitalWrite(fanPin, fanSpeed);
  temp2 = 0;
  close_Servo_Motor();
  return ;
}
void close_Servo_Motor() ///error function
{
  lcd.setCursor(1,1); 
  lcd.print("Door Closeing");
  for(angelRotate = 0; angelRotate<=180; angelRotate+=5)     // command to move from 0 degrees to 180 degrees 
  {          
   analogWrite(motorPin, angelRotate);                   
   servoMotor.write(angelRotate);              //command to rotate the servo to the specified angle
   delay(5);                       
  } 
  return;
}
