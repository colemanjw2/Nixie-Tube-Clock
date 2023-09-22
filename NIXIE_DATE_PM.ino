#include <Wire.h>
#include <RTClib.h>
#include <RealTimeClockDS1307.h>
#include <ShiftRegister74HC595.h>
#include <DFRobot_DS1307.h>


RTC_DS1307 rtc;
DFRobot_DS1307 DS1307;


int count = 0;
int countminute = 0;
int counthour = 0;
int secondtime;
int minutetime;
int hourtime;
int day;
int month;
int year;

int hourbuttonInput = 12;
int minutebuttoninput = 13;
int hourbuttonState = 0;
int minutebuttonstate = 0;
int datebutton = 15;
int datebuttonstate = 0;

int ledseparators = 11;
int PMindicator = 14;

//Set shift register
ShiftRegister74HC595<1> sr1(2, 3, 4);
ShiftRegister74HC595<1> sr2(5, 6, 7);
ShiftRegister74HC595<1> sr(8, 9, 10);


void setup() {
  // put your setup code here, to run once:
  pinMode(hourbuttonInput, INPUT);
  pinMode(minutebuttoninput, INPUT);
  pinMode(ledseparators, OUTPUT);
  pinMode(PMindicator, OUTPUT);
  pinMode(datebutton, INPUT);

  //Clock stuff
  int hourtimecount = 0;
  int minutetimecount = 0;
  int secondtimecount = 0;

  while (!Serial)
    ;  // for Leonardo/Micro/Zero

  Serial.begin(57600);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
}


void loop() {

  hourbuttonState = digitalRead(hourbuttonInput);
  minutebuttonstate = digitalRead(minutebuttoninput);
  datebuttonstate = digitalRead(datebutton);

  digitalWrite(minutebuttoninput, HIGH);
  digitalWrite(hourbuttonInput, HIGH);

  DateTime now = rtc.now();

  if (minutebuttonstate == LOW) {
    //Cathode poisoning prevention
    if (now.minute() == 10) {
      if (now.second() == 0) {
        AntiPoison();
      }
    }
    if (now.minute() == 20) {
      if (now.second() == 0) {
        AntiPoison();
      }
    }
    if (now.minute() == 30) {
      if (now.second() == 0) {
        AntiPoison();
      }
    }
    if (now.minute() == 40) {
      if (now.second() == 0) {
        AntiPoison();
      }
    }
    if (now.minute() == 50) {
      if (now.second() == 0) {
        AntiPoison();
      }
    }
    if (now.minute() == 0) {
      if (now.second() == 0) {
        AntiPoison();
      }
    }
  }



  Serial.println();
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.print("-");
  Serial.println(now.day());
  Serial.print("-");
  Serial.println(now.month());



  //button conditions for increasing hour and minute

  if (hourbuttonState == HIGH) {
    delay(200);
    int newValue = now.hour() + 1;

    if (newValue > 23) {
      newValue = 0;
    }

    rtc.adjust(DateTime(now.year(), now.month(), now.day(), newValue, now.minute(), 0));
  }


  if (minutebuttonstate == HIGH) {
    delay(200);
    int newValue = now.minute() + 1;
    if (newValue > 59) {
      newValue = 0;
    }
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), newValue, 0));
  }

  // LED parameters

  if ((secondtime % 2) == 0) {
    digitalWrite(ledseparators, HIGH);
  }

  if ((secondtime % 2) != 0) {
    digitalWrite(ledseparators, LOW);
  }

  if (now.hour() >= 12) {
    digitalWrite(PMindicator, HIGH);
  }

  if (now.hour() < 12) {
    digitalWrite(PMindicator, LOW);
  }


  /// date button parameters
  if (datebuttonstate == HIGH) {

    int day = now.day();
    int month = now.month();
    int year = now.year();

    ///left digit day
    counthour = (day / 10) % 10;

    //right digit day
    hourtime = day % 10;

    /// left digit month
    countminute = (month / 10) % 10;

    /// right digit month
    minutetime = month % 10;

    ///left digit years
    count = (year / 10) % 10;

    /// right digit year
    secondtime = year % 10;

    runhours();
    runminutes();
    runseconds();
  }


  else {  //////////////Seconds//////////////

    for (secondtime = now.second(); secondtime >= 10; secondtime = secondtime - 10) {
      int secondtimecount = 1 + secondtimecount;
      ++count;
    }
    runseconds();

    count = 0;

    ////////////////////////////////////////////Hours////////////////////////////////////

    for (hourtime = now.hour(); hourtime >= 10; hourtime = hourtime - 10) {
      if (hourtime >= 13) {
        counthour = 0;
        hourtime = hourtime - 2;
      }

      else {
        int hourtimecount = 1 + hourtimecount;
        ++counthour;
      }
    }

    if (now.hour() == 0) {
      counthour = 1;
      hourtime = 2;
    }


    runhours();

    counthour = 0;



    ////////////////////////////////////////////Minutes////////////////////////////////////

    for (minutetime = now.minute(); minutetime >= 10; minutetime = minutetime - 10) {
      int minutetimecount = 1 + minutetimecount;
      ++countminute;
    }

    runminutes();
    countminute = 0;
  }
}




///////////////////////////seconds///////////////////////////////
/////////////R//////////////////////
void TurnOnR1() {
  sr.set(0, HIGH);
  sr.set(1, LOW);
  sr.set(2, LOW);
  sr.set(3, LOW);
}
void TurnOnR0() {
  sr.set(0, LOW);
  sr.set(1, LOW);
  sr.set(2, LOW);
  sr.set(3, LOW);
}
void TurnOnR9() {
  sr.set(0, HIGH);
  sr.set(1, LOW);
  sr.set(2, LOW);
  sr.set(3, HIGH);
}
void TurnOnR8() {
  sr.set(0, LOW);
  sr.set(1, LOW);
  sr.set(2, LOW);
  sr.set(3, HIGH);
}
void TurnOnR7() {
  sr.set(0, HIGH);
  sr.set(1, HIGH);
  sr.set(2, HIGH);
  sr.set(3, LOW);
}
void TurnOnR6() {
  sr.set(0, LOW);
  sr.set(1, HIGH);
  sr.set(2, HIGH);
  sr.set(3, LOW);
}
void TurnOnR5() {
  sr.set(0, HIGH);
  sr.set(1, LOW);
  sr.set(2, HIGH);
  sr.set(3, LOW);
}
void TurnOnR4() {
  sr.set(0, LOW);
  sr.set(1, LOW);
  sr.set(2, HIGH);
  sr.set(3, LOW);
}
void TurnOnR3() {
  sr.set(0, HIGH);
  sr.set(1, HIGH);
  sr.set(2, LOW);
  sr.set(3, LOW);
}
void TurnOnR2() {
  sr.set(0, LOW);
  sr.set(1, HIGH);
  sr.set(2, LOW);
  sr.set(3, LOW);
}

///////////////////////L////////////////////////////
void TurnOnL1() {
  sr.set(4, HIGH);
  sr.set(5, LOW);
  sr.set(6, LOW);
  sr.set(7, LOW);
}
void TurnOnL0() {
  sr.set(4, LOW);
  sr.set(5, LOW);
  sr.set(6, LOW);
  sr.set(7, LOW);
}
void TurnOnL9() {
  sr.set(4, HIGH);
  sr.set(5, LOW);
  sr.set(6, LOW);
  sr.set(7, HIGH);
}
void TurnOnL8() {
  sr.set(4, LOW);
  sr.set(5, LOW);
  sr.set(6, LOW);
  sr.set(7, HIGH);
}
void TurnOnL7() {
  sr.set(4, HIGH);
  sr.set(5, HIGH);
  sr.set(6, HIGH);
  sr.set(7, LOW);
}
void TurnOnL6() {
  sr.set(4, LOW);
  sr.set(5, HIGH);
  sr.set(6, HIGH);
  sr.set(7, LOW);
}
void TurnOnL5() {
  sr.set(4, HIGH);
  sr.set(5, LOW);
  sr.set(6, HIGH);
  sr.set(7, LOW);
}
void TurnOnL4() {
  sr.set(4, LOW);
  sr.set(5, LOW);
  sr.set(6, HIGH);
  sr.set(7, LOW);
}
void TurnOnL3() {
  sr.set(4, HIGH);
  sr.set(5, HIGH);
  sr.set(6, LOW);
  sr.set(7, LOW);
}
void TurnOnL2() {
  sr.set(4, LOW);
  sr.set(5, HIGH);
  sr.set(6, LOW);
  sr.set(7, LOW);
}

//////////////////////////////Minutes//////////////////////////
void TurnOnMR1() {
  sr2.set(0, HIGH);
  sr2.set(1, LOW);
  sr2.set(2, LOW);
  sr2.set(3, LOW);
}
void TurnOnMR0() {
  sr2.set(0, LOW);
  sr2.set(1, LOW);
  sr2.set(2, LOW);
  sr2.set(3, LOW);
}
void TurnOnMR9() {
  sr2.set(0, HIGH);
  sr2.set(1, LOW);
  sr2.set(2, LOW);
  sr2.set(3, HIGH);
}
void TurnOnMR8() {
  sr2.set(0, LOW);
  sr2.set(1, LOW);
  sr2.set(2, LOW);
  sr2.set(3, HIGH);
}
void TurnOnMR7() {
  sr2.set(0, HIGH);
  sr2.set(1, HIGH);
  sr2.set(2, HIGH);
  sr2.set(3, LOW);
}
void TurnOnMR6() {
  sr2.set(0, LOW);
  sr2.set(1, HIGH);
  sr2.set(2, HIGH);
  sr2.set(3, LOW);
}
void TurnOnMR5() {
  sr2.set(0, HIGH);
  sr2.set(1, LOW);
  sr2.set(2, HIGH);
  sr2.set(3, LOW);
}
void TurnOnMR4() {
  sr2.set(0, LOW);
  sr2.set(1, LOW);
  sr2.set(2, HIGH);
  sr2.set(3, LOW);
}
void TurnOnMR3() {
  sr2.set(0, HIGH);
  sr2.set(1, HIGH);
  sr2.set(2, LOW);
  sr2.set(3, LOW);
}
void TurnOnMR2() {
  sr2.set(0, LOW);
  sr2.set(1, HIGH);
  sr2.set(2, LOW);
  sr2.set(3, LOW);
}

///////////////////////L////////////////////////////
void TurnOnML1() {
  sr2.set(4, HIGH);
  sr2.set(5, LOW);
  sr2.set(6, LOW);
  sr2.set(7, LOW);
}
void TurnOnML0() {
  sr2.set(4, LOW);
  sr2.set(5, LOW);
  sr2.set(6, LOW);
  sr2.set(7, LOW);
}
void TurnOnML9() {
  sr2.set(4, HIGH);
  sr2.set(5, LOW);
  sr2.set(6, LOW);
  sr2.set(7, HIGH);
}
void TurnOnML8() {
  sr2.set(4, LOW);
  sr2.set(5, LOW);
  sr2.set(6, LOW);
  sr2.set(7, HIGH);
}
void TurnOnML7() {
  sr2.set(4, HIGH);
  sr2.set(5, HIGH);
  sr2.set(6, HIGH);
  sr2.set(7, LOW);
}
void TurnOnML6() {
  sr2.set(4, LOW);
  sr2.set(5, HIGH);
  sr2.set(6, HIGH);
  sr2.set(7, LOW);
}
void TurnOnML5() {
  sr2.set(4, HIGH);
  sr2.set(5, LOW);
  sr2.set(6, HIGH);
  sr2.set(7, LOW);
}
void TurnOnML4() {
  sr2.set(4, LOW);
  sr2.set(5, LOW);
  sr2.set(6, HIGH);
  sr2.set(7, LOW);
}
void TurnOnML3() {
  sr2.set(4, HIGH);
  sr2.set(5, HIGH);
  sr2.set(6, LOW);
  sr2.set(7, LOW);
}
void TurnOnML2() {
  sr2.set(4, LOW);
  sr2.set(5, HIGH);
  sr2.set(6, LOW);
  sr2.set(7, LOW);
}

//////////////////////////////Hours//////////////////////////
void TurnOnHR1() {
  sr1.set(0, HIGH);
  sr1.set(1, LOW);
  sr1.set(2, LOW);
  sr1.set(3, LOW);
}
void TurnOnHR0() {
  sr1.set(0, LOW);
  sr1.set(1, LOW);
  sr1.set(2, LOW);
  sr1.set(3, LOW);
}
void TurnOnHR9() {
  sr1.set(0, HIGH);
  sr1.set(1, LOW);
  sr1.set(2, LOW);
  sr1.set(3, HIGH);
}
void TurnOnHR8() {
  sr1.set(0, LOW);
  sr1.set(1, LOW);
  sr1.set(2, LOW);
  sr1.set(3, HIGH);
}
void TurnOnHR7() {
  sr1.set(0, HIGH);
  sr1.set(1, HIGH);
  sr1.set(2, HIGH);
  sr1.set(3, LOW);
}
void TurnOnHR6() {
  sr1.set(0, LOW);
  sr1.set(1, HIGH);
  sr1.set(2, HIGH);
  sr1.set(3, LOW);
}
void TurnOnHR5() {
  sr1.set(0, HIGH);
  sr1.set(1, LOW);
  sr1.set(2, HIGH);
  sr1.set(3, LOW);
}
void TurnOnHR4() {
  sr1.set(0, LOW);
  sr1.set(1, LOW);
  sr1.set(2, HIGH);
  sr1.set(3, LOW);
}
void TurnOnHR3() {
  sr1.set(0, HIGH);
  sr1.set(1, HIGH);
  sr1.set(2, LOW);
  sr1.set(3, LOW);
}
void TurnOnHR2() {
  sr1.set(0, LOW);
  sr1.set(1, HIGH);
  sr1.set(2, LOW);
  sr1.set(3, LOW);
}

///////////////////////R////////////////////////////
void TurnOnHL1() {
  sr1.set(4, HIGH);
  sr1.set(5, LOW);
  sr1.set(6, LOW);
  sr1.set(7, LOW);
}
void TurnOnHL0() {
  sr1.set(4, LOW);
  sr1.set(5, LOW);
  sr1.set(6, LOW);
  sr1.set(7, LOW);
}
void TurnOnHL9() {
  sr1.set(4, HIGH);
  sr1.set(5, LOW);
  sr1.set(6, LOW);
  sr1.set(7, HIGH);
}
void TurnOnHL8() {
  sr1.set(4, LOW);
  sr1.set(5, LOW);
  sr1.set(6, LOW);
  sr1.set(7, HIGH);
}
void TurnOnHL7() {
  sr1.set(4, HIGH);
  sr1.set(5, HIGH);
  sr1.set(6, HIGH);
  sr1.set(7, LOW);
}
void TurnOnHL6() {
  sr1.set(4, LOW);
  sr1.set(5, HIGH);
  sr1.set(6, HIGH);
  sr1.set(7, LOW);
}
void TurnOnHL5() {
  sr1.set(4, HIGH);
  sr1.set(5, LOW);
  sr1.set(6, HIGH);
  sr1.set(7, LOW);
}
void TurnOnHL4() {
  sr1.set(4, LOW);
  sr1.set(5, LOW);
  sr1.set(6, HIGH);
  sr1.set(7, LOW);
}
void TurnOnHL3() {
  sr1.set(4, HIGH);
  sr1.set(5, HIGH);
  sr1.set(6, LOW);
  sr1.set(7, LOW);
}
void TurnOnHL2() {
  sr1.set(4, LOW);
  sr1.set(5, HIGH);
  sr1.set(6, LOW);
  sr1.set(7, LOW);
}
/////////////////Run second///////////////////
void runseconds() {
  if (count == 0) {
    TurnOnL0();
  }
  if (count == 1) {
    TurnOnL1();
  }
  if (count == 2) {
    TurnOnL2();
  }
  if (count == 3) {
    TurnOnL3();
  }
  if (count == 4) {
    TurnOnL4();
  }
  if (count == 5) {
    TurnOnL5();
  }

  //////////Move by second, R////////////////

  if (secondtime == 0) {
    TurnOnR0();
  }
  if (secondtime == 1) {
    TurnOnR1();
  }
  if (secondtime == 2) {
    TurnOnR2();
  }
  if (secondtime == 3) {
    TurnOnR3();
  }
  if (secondtime == 4) {
    TurnOnR4();
  }
  if (secondtime == 5) {
    TurnOnR5();
  }
  if (secondtime == 6) {
    TurnOnR6();
  }
  if (secondtime == 7) {
    TurnOnR7();
  }
  if (secondtime == 8) {
    TurnOnR8();
  }
  if (secondtime == 9) {
    TurnOnR9();
  }
}

/////////////////////runhours////////////////////////
void runhours() {
  if (counthour == 0) {
    TurnOnHL0();
  }
  if (counthour == 1) {
    TurnOnHL1();
  }
  if (counthour == 2) {
    TurnOnHL2();
  }

  //////////Move by hour, R////////////////

  if (hourtime == 0) {
    TurnOnHR0();
  }
  if (hourtime == 1) {
    TurnOnHR1();
  }
  if (hourtime == 2) {
    TurnOnHR2();
  }
  if (hourtime == 3) {
    TurnOnHR3();
  }
  if (hourtime == 4) {
    TurnOnHR4();
  }
  if (hourtime == 5) {
    TurnOnHR5();
  }
  if (hourtime == 6) {
    TurnOnHR6();
  }
  if (hourtime == 7) {
    TurnOnHR7();
  }
  if (hourtime == 8) {
    TurnOnHR8();
  }
  if (hourtime == 9) {
    TurnOnHR9();
  }
}

/////////////////////////////Minutes///////////////////////////
void runminutes() {
  if (countminute == 0) {
    TurnOnML0();
  }
  if (countminute == 1) {
    TurnOnML1();
  }
  if (countminute == 2) {
    TurnOnML2();
  }
  if (countminute == 3) {
    TurnOnML3();
  }
  if (countminute == 4) {
    TurnOnML4();
  }
  if (countminute == 5) {
    TurnOnML5();
  }
  if (countminute == 6) {
    TurnOnML6();
  }
  if (countminute == 7) {
    TurnOnML7();
  }
  if (countminute == 8) {
    TurnOnML8();
  }
  if (countminute == 9) {
    TurnOnML9();
  }

  //////////Move by minute, R////////////////

  if (minutetime == 0) {
    TurnOnMR0();
  }
  if (minutetime == 1) {
    TurnOnMR1();
  }
  if (minutetime == 2) {
    TurnOnMR2();
  }
  if (minutetime == 3) {
    TurnOnMR3();
  }
  if (minutetime == 4) {
    TurnOnMR4();
  }
  if (minutetime == 5) {
    TurnOnMR5();
  }
  if (minutetime == 6) {
    TurnOnMR6();
  }
  if (minutetime == 7) {
    TurnOnMR7();
  }
  if (minutetime == 8) {
    TurnOnMR8();
  }
  if (minutetime == 9) {
    TurnOnMR9();
  }
}

void AntiPoison() {
  TurnOnR0();
  TurnOnL0();
  TurnOnMR0();
  TurnOnML0();
  TurnOnHL0();
  TurnOnHR0();
  delay(200);
  TurnOnR1();
  TurnOnL1();
  TurnOnMR1();
  TurnOnML1();
  TurnOnHR1();
  TurnOnHL1();
  delay(200);
  TurnOnR2();
  TurnOnL2();
  TurnOnMR2();
  TurnOnML2();
  TurnOnHR2();
  TurnOnHL2();
  delay(200);
  TurnOnR3();
  TurnOnL3();
  TurnOnMR3();
  TurnOnML3();
  TurnOnHR3();
  TurnOnHL3();
  delay(200);
  TurnOnR4();
  TurnOnL4();
  TurnOnMR4();
  TurnOnML4();
  TurnOnHR4();
  TurnOnHL4();
  delay(200);
  TurnOnR5();
  TurnOnL5();
  TurnOnMR5();
  TurnOnML5();
  TurnOnHR5();
  TurnOnHL5();
  delay(200);
  TurnOnR6();
  TurnOnL6();
  TurnOnMR6();
  TurnOnML6();
  TurnOnHR6();
  TurnOnHL6();
  delay(200);
  TurnOnR7();
  TurnOnL7();
  TurnOnMR7();
  TurnOnML7();
  TurnOnHR7();
  TurnOnHL7();
  delay(200);
  TurnOnR8();
  TurnOnL8();
  TurnOnMR8();
  TurnOnML8();
  TurnOnHR8();
  TurnOnHL8();
  delay(200);
  TurnOnR9();
  TurnOnL9();
  TurnOnMR9();
  TurnOnML9();
  TurnOnHR9();
  TurnOnHL9();
  delay(200);
}
