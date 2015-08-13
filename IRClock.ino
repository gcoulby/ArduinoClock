// include the library code:
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <Time.h>

//IR Control
int RECEIVE_PIN = 3;
IRrecv irInput(RECEIVE_PIN);
decode_results results;

int clockSetButton = 2160;
int clockSetButton2 = 112;
int upButton = 2132;
int upButton2 = 84;
int downButton = 2131;
int downButton2 = 83;
int okButton = 2165;
int okButton2 = 117;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);

void setup()
{
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  setTime(15,40,0,13,11,2014);  //Time of Creation!
  printDate();
  printTime();
  irInput.enableIRIn(); // Start the receiver

}

void printDate()
{
  lcd.setCursor(0, 0);
  lcd.print(dayShortStr(weekday()));
  lcd.print(" - ");

  if(day()<10)
  {
    lcd.print(0);
    lcd.print(day());
  }
  else
  {
    lcd.print(day());
  }
  lcd.print("/");
  if(month()<10)
  {
    lcd.print(0);
    lcd.print(month());
  }
  else
  {
    lcd.print(month());
  }
  lcd.print("/");
  lcd.print(year());
}

void printTime()
{
  lcd.setCursor(4, 2);
  //print hours
  if(hour()<10){
    lcd.print(0);
    lcd.print(hour());
  }
  else
  {
    lcd.print(hour());
  }

  //print dots
  lcd.print(":");

  //print minutes
  if(minute()<10)
  {
    lcd.print(0);
    lcd.print(minute());
  }
  else
  {
    lcd.print(minute());
  }

  //print dots
  lcd.print(":");

  //print seconds
  if(second()<10)
  {
    lcd.print(0);
    lcd.print(second());
  }
  else
  {
    lcd.print(second());
  }
}

//Sends switch cases to set to set the time
// time format HH,MM,SS,DD,MM,YYYY)
//Seconds are reset after setting the clock
void setClock()
{
  setTime(set(0),set(1),0,set(2),set(3),set(4));
}

int set(int i)
{
  int set = 0;
  int maxValue = 0;
  int minValue = 0;
  boolean isYear = false;
  char whatToSet[10];

  switch(i)
  {
  case 0:
    set = hour();
    maxValue = 23;
    minValue = 0;
    strcpy(whatToSet, "Hour");
    break;
  case 1:
    set = minute();
    maxValue = 59;
    minValue = 0;
    strcpy(whatToSet, "Minute");
    break;
  case 2:
    set = day();
    maxValue = 31;
    minValue = 1;
    strcpy(whatToSet, "Day");
    break;
  case 3:
    set = month();
    maxValue = 12;
    minValue = 1;
    strcpy(whatToSet, "Month");
    break;
  case 4:
    set = year();
    strcpy(whatToSet, "Year");
    break;
  }

  lcd.clear();
  lcd.setCursor(4,0);
  char build[80];
  strcpy(build, "Set ");
  strcat(build, whatToSet);
  lcd.print(build);

  while(true)
  {
    if(i == 4)
    {
      lcd.setCursor(6,1);
      lcd.print(set);
    }
    else
    {
      lcd.setCursor(7,1);
      if(set<10){
        lcd.print(0);
        lcd.print(set);
      }
      else
      {
        lcd.setCursor(7,1);
        lcd.print(set);
      }
    }
    //Read IR signals      
    if (irInput.decode(&results))
    {
      delay(100);

      if(results.value == upButton || results.value == upButton2)
      {
        if(i<4)
        {
          if(set<maxValue)
          {
            set++;
          }
          else
          {
            set = minValue;
          }
        }
        else
        {
          set++;
        }
      }
      else if(results.value == downButton || results.value == downButton2)
      {
        if(i<4)
        {
          if(set>minValue)
          {
            set--;
          }
          else
          {
            set = maxValue;
          }
        }
        else
        {
          set--;
        }
      }
      else if(results.value == okButton || results.value == okButton2)
      {
        irInput.resume();// Receive the next value
        break;
      }
      irInput.resume();// Receive the next value
    }
  }
  return set;
}


void loop()
{
  if (irInput.decode(&results))
  {
    delay(100);
    Serial.print(results.value);
    if(results.value == clockSetButton || results.value == clockSetButton2)
    {
      setClock();
    }
    irInput.resume();// Receive the next value
  }
  delay(1000);
  adjustTime(0.5);
  printDate();
  printTime();
}