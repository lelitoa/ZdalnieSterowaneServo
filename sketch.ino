#include <Servo.h>
#include <IRremote.h>
#include <LiquidCrystal.h>

//servo
Servo myservo;
int pos = 0;
int posBefore = 0;

bool ServoStatus = 0;

//ir-remote
#define PIN_RECEIVER 2   // Signal Pin of IR receiver

IRrecv receiver(PIN_RECEIVER);

//lcd
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Witaj!  wcisnij");
  lcd.setCursor(0, 1);
  lcd.print("dowolny przycisk");
  receiver.enableIRIn(); // Start the receiver
  myservo.attach(3);
  myservo.write(pos);
}

void loop() {
  if (receiver.decode()) {
    translateIR();
    receiver.resume();  // Receive the next value
  }
}

void lcdPrint(char* button)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(button);
  lcd.setCursor(10, 0);
  lcd.print("kod");
  SetDisplayPosition(receiver.decodedIRData.command, 0);
  lcd.print(receiver.decodedIRData.command);
}

void SetDisplayPosition(int value, bool columnPos)
{
  if (value < 10)
  {
    lcd.setCursor(15, columnPos);
  }
  else if (value >= 10 && value < 100)
  {
    lcd.setCursor(14, columnPos);
  }
  else if (value > 99)
  {
    lcd.setCursor(13, columnPos);
  }
}

void lcdPrintEmpty()
{
  lcd.setCursor(0, 1);
  lcd.print("Brak akcji");
}

void lcdPrintStartMiddleStop(int valuePos, int DisplayPos)
{
  if (ServoStatus == 1)
  {
    posBefore = pos;
    pos = valuePos;
    myservo.write(pos);
    lcd.setCursor(0, 1);
    lcd.print("Pozycja servo");
    lcd.setCursor(DisplayPos, 1);
    lcd.print(pos);
  }
  else
    lcdPrintEmpty();
}

void lcdPrintPlusMinus(bool value)
{
  if (ServoStatus == 1)
  {
    if (value == 1)
    {
      if (pos < 180)
      {
        posBefore = pos;
        pos++;
      }
    }
    else if (value == 0)
    {
      if (pos > 0)
      {
        posBefore = pos;
        pos--;
      }
    }
    myservo.write(pos);
    lcd.setCursor(0, 1);
    lcd.print("Pozycja servo");
    SetDisplayPosition(pos, 1);
    lcd.print(pos);
  }
  else
    lcdPrintEmpty();
}

String num = "";
String tmp = "";
void lcdPrintNumbers(String value)
{
  if (ServoStatus == 1)
  {
    if (num.length() >= 0 && num.length() < 3)
    {
      tmp = tmp + value;

      if (tmp.toInt() <= 180)
      {
        num = num + value;

        pos = num.toInt();

        myservo.write(pos);
        lcd.setCursor(0, 1);
        lcd.print("Pozycja servo");
        SetDisplayPosition(pos, 1);
        lcd.print(pos);
      }
      else
      {
        tmp = tmp.substring(0,2);
        lcd.setCursor(0, 1);
        lcd.print("Poza zakresem!");
      }
    }
    else if (num.length() == 3)
    {
      lcd.setCursor(0, 1);
      lcd.print("Poza zakresem!");
    }    
  }
  else
    lcdPrintEmpty();
}

void translateIR()
{
  switch (receiver.decodedIRData.command) {
    case 162:
      lcdPrint("POWER");
      lcd.setCursor(0, 1);
      if (ServoStatus == 0)
      {
        ServoStatus = 1;
        lcd.print("Servo wlaczone");
      }
      else if (ServoStatus == 1)
      {
        ServoStatus = 0;
        lcd.print("Servo wylaczone");
      }
      break;
    case 226:
      lcdPrint("MENU");
      lcdPrintEmpty();
      break;
    case 34:
      lcdPrint("TEST");

      if (ServoStatus == 1)
      {
        posBefore = pos;
        pos = random(0,180);

        myservo.write(pos);
        lcd.setCursor(0, 1);
        lcd.print("Pozycja servo");
        SetDisplayPosition(pos, 1);
        lcd.print(pos);
      }
      else
        lcdPrintEmpty();
      break;
    case 2:
      lcdPrint("PLUS");
      lcdPrintPlusMinus(1);
      break;
    case 194:
      lcdPrint("BACK");
      
      if (ServoStatus == 1)
      {
        if (tmp.length() == 1 && num.length() == 1)
        {
          num = "";
          tmp = "";
          pos = 0;
        }
        else if (tmp.length() == 2 && num.length() == 2)
        {
          num = num.substring(0,1);
          tmp = tmp.substring(0,1);
          pos = num.toInt();
        }
        else if (tmp.length() == 3 && num.length() == 3)
        {
          num = num.substring(0,2);
          tmp = tmp.substring(0,2);
          pos = num.toInt();
        }
        else
        {
          pos = posBefore;
        }

        myservo.write(pos);
        lcd.setCursor(0, 1);
        lcd.print("Pozycja servo");
        SetDisplayPosition(pos, 1);
        lcd.print(pos);
      }
      else
        lcdPrintEmpty();
      break;
    case 224:
      lcdPrint("PREV");
      lcdPrintStartMiddleStop(0, 15);
      break;
    case 168:
      lcdPrint("PLAY");
      lcdPrintStartMiddleStop(90, 14);
      break;
    case 144:
      lcdPrint("NEXT");
      lcdPrintStartMiddleStop(180, 13);
      break;
    case 104:
      lcdPrint("num: 0");
      lcdPrintNumbers("0");
      break;
    case 152:
      lcdPrint("MINUS");
      lcdPrintPlusMinus(0);
      break;
    case 176:
      lcdPrint("Cancel");
      lcdPrintStartMiddleStop(0, 15);
      num = "";
      tmp = "";
      break;
    case 48:
      lcdPrint("num: 1");
      lcdPrintNumbers("1");
      break;
    case 24:
      lcdPrint("num: 2");
      lcdPrintNumbers("2");
      break;
    case 122:
      lcdPrint("num: 3");
      lcdPrintNumbers("3");
      break;
    case 16:
      lcdPrint("num: 4");
      lcdPrintNumbers("4");
      break;
    case 56:
      lcdPrint("num: 5");
      lcdPrintNumbers("5");
      break;
    case 90:
      lcdPrint("num: 6");
      lcdPrintNumbers("6");
      break;
    case 66:
      lcdPrint("num: 7");
      lcdPrintNumbers("7");
      break;
    case 74:
      lcdPrint("num: 8");
      lcdPrintNumbers("8");
      break;
    case 82:
      lcdPrint("num: 9");
      lcdPrintNumbers("9");
      break;
    default:
      lcd.clear();
      lcd.print(receiver.decodedIRData.command);
  }
}
