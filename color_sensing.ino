#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 2, en = 3, d4 = 4, d5 = 8, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo myservo_1;
Servo myservo_2;

int ldr1 = 0;
int ldr2 = 0;
int ldr3 = 0;
int St_ldr1 = 0;
int St_ldr2 = 0;
int St_ldr3 = 0;

int rIn = 255;
int gIn = 255;
int bIn = 255;
int maximum = 0;
int inside = 1;

int red = 9;
int green = 10;
int blue = 11;
int i = 1;

int cal = 0;
char color1;
char color2;
char detected_color;

int proximity = A1;
int posq;

int em = 7;

int X;
/////------setup------//////
void setup() {
  Serial.begin(9600);
  myservo_1.attach(5);
  myservo_1.write(40);
  myservo_2.attach(6);
  myservo_2.write(160);
  delay(100);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(em, OUTPUT); 

  pinMode(A2, INPUT);
  lcd.begin(16, 2);

}

/////-------auto-calibration------//////

void AutoCalibrate() {
  while (inside == 1) {
    analogWrite(red, rIn);
    delay(150);
    ldr1 = analogRead(A2);
    analogWrite(red, 0);

    analogWrite(green, gIn);
    delay(150);
    ldr2 = analogRead(A2);
    analogWrite(green, 0);

    analogWrite(blue, bIn);
    delay(150);
    ldr3 = analogRead(A2);
    analogWrite(blue, 0);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("R:");
    lcd.print(ldr1);

    lcd.print(" G:");
    lcd.print(ldr2);

    lcd.setCursor(0, 1);
    lcd.print("B:");
    lcd.print(ldr3);
    lcd.display();
    delay(100);

    if (inside == 1) {
      if (ldr1 > ldr2 && ldr1 > ldr3) {
        maximum = ldr1;
      }
      else if (ldr2 > ldr3) {
        maximum = ldr2;
      }
      else {
        maximum = ldr3;
      }

      if (maximum - ldr1 > 20) {
        rIn = rIn - 5;
      }
      if (maximum - ldr2 > 20) {
        gIn = gIn - 5;
      }
      if (maximum - ldr3 > 20) {
        bIn = bIn - 5;
      }
      //maximum = max(max(ldr1,ldr2),ldr3)); 

      Serial.println(rIn);
      Serial.println(gIn);
      Serial.println(bIn);
      Serial.print(ldr1);
      Serial.print("\t");
      Serial.print(ldr2);
      Serial.print("\t");
      Serial.print(ldr3);
      Serial.print("\n");
      if (maximum - ldr1 < 20 && maximum - ldr2 < 20 && maximum - ldr3 < 20) {
        inside = 0;
        Serial.println("Calibration_Completed");
      }
    }
  }
}

/////---------manual-calibration---------////////

void ManualCalibrate() {
  while (inside == 1) {
    read_ldr();
    if (inside == 1) {
      if (ldr1 > ldr2 && ldr1 > ldr3) {
        maximum = ldr1;
      }
      else if (ldr2 > ldr3) {
        maximum = ldr2;
      }
      else {
        maximum = ldr3;
      }
      Serial.println(rIn); //ldr1
      Serial.println(gIn); //ldr2
      Serial.println(bIn); //ldr3
      if (maximum - ldr1 < 60 && maximum - ldr2 < 60 && maximum - ldr3 < 60) {
        inside = 0;
        Serial.println("Calibration_Completed");
      }
    }
  }
  St_ldr1 = ldr1;
  St_ldr2 = ldr2;
  St_ldr3 = ldr3;
}
/////-------readldr----------/////
void read_ldr() {
  digitalWrite(red, HIGH);
  delay(150);
  ldr1 = analogRead(A2);
  digitalWrite(red, 0);

  digitalWrite(green, HIGH);
  delay(150);
  ldr2 = analogRead(A2);
  digitalWrite(green, 0);

  digitalWrite(blue, HIGH);
  delay(150);
  ldr3 = analogRead(A2);
  digitalWrite(blue, 0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:");
  lcd.print(ldr1);

  lcd.print(" G:");
  lcd.print(ldr2);

  lcd.setCursor(0, 1);
  lcd.print("B:");
  lcd.print(ldr3);
  lcd.display();
  delay(100);
}

/////---------reach_color--------------//////
void reach_color(char color) {
  int pos = 40;
  int inside = 1;
  int in = 0;

  for (pos = 40; pos <= 160; pos += 3) { 
    myservo_1.write(pos);             
    delay(700);                       
    read_ldr();
    Serial.println(pos);
    Serial.print(ldr1);
    Serial.print("\t");
    Serial.print(ldr2);
    Serial.print("\t");
    Serial.println(ldr3);
    int a = min(min(ldr1, ldr2), ldr3);
    int distance = 15;


    if (a == ldr1 && (ldr2 - St_ldr2) > distance && (ldr3 - St_ldr3) > distance) {
      Serial.println("Red");
      if (color == 'r') {
        Serial.println(color);
        in += 1;
        Serial.println(in);
        if (in == 2) {
          break;
        }
      }
    }

    if (a == ldr2 && (ldr1 - St_ldr1) > distance && ldr3 - St_ldr3 > distance) {
      Serial.println("Green");
      if (color == 'g') {
        //Serial.println(detected_color);
        Serial.println(color);
        in += 1;
        Serial.println(in);
        if (in == 2) {
          break;
        }
        
      }
    }

    if (a == ldr3 && ldr1 - St_ldr1 > distance && ldr2 - St_ldr2 > distance) {
      Serial.println("Blue");
      if (color == 'b') {
        //Serial.println(detected_color);
        Serial.println(color);
        in += 1;
        if (in == 2) {
          break;
        }
        Serial.println(in);
      }
    }
  }
  return in;
}

/////-----------loop---------/////
void loop() {

  //1 ----> Calibration

  Serial.println("Use_0:Manual_Calibration_Or_1:Auto Calibration");
  while (Serial.available() == 0) {}

  while (Serial.available() > 0) {
    cal = Serial.parseInt();
    Serial.println(cal);
    if (cal == 1) {
      Serial.println("Auto_Cal_Started");
      AutoCalibrate();
      Serial.println("Auto_Cal_End");
    }
    else if (cal == 0) {
      Serial.println("Manual_Cal_Started");
      ManualCalibrate();
      Serial.println("Manual_Cal_End");
    }
  }

  //2 ----> Colour

  Serial.println("Specify_input_color_r,g,b");
  while (1) {
    color1 = Serial.read();
    if (isAlpha(color1))
    {
      break;
    }
  }

  reach_color(color1);
  Serial.println("Wanted_Input_Color");
  for (int posp = 160; posp >= 0; posp -= 5) { 
    if (analogRead(proximity)<800){
      myservo_2.write(posp);              
      delay(15);                       
    }
    else {
      myservo_2.write(posp); 
      delay(15);
      break;
    }
  }
  delay(2000);
  digitalWrite(em,HIGH);
  myservo_2.write(160);
  delay(2000);

  Serial.println("Specify_drop_color_r,g,b");
  while (1) {
    color1 = Serial.read();
    if (isAlpha(color1))
    {
      break;
    }
  }
  
  reach_color(color1);
  Serial.println("Wanted_Output_Color");
  
  for (int posp = 160; posp >= 0; posp -= 5) { 
    posq = posp;
    if (analogRead(proximity)<800){
      myservo_2.write(posp);              
      delay(15);                       
    }
    else {
      myservo_2.write(posp); 
      Serial.println("Obstruction");
      delay(15);
      break;
    }
  }
 
  if (posq == 0)
  {
    digitalWrite(em, LOW);
  }
  delay(1000);
  myservo_2.write(160);

  Serial.println("Mission_Accomplished");
  delay(1000);
}
