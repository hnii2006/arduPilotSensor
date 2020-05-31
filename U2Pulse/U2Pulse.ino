/**  QA evaluation program 2019/12~
 *  
 *  servoQAtest.ino 
 * 
 *  RC servo by timer1
 *  h.nii 2019/4/10
 *  
 *  2ch servo control program. 
 *  Clock frequency is 16MHz for this program.
 *  
 *  "servo.h" library allow to output 0 degree to 180 degree, 
 *   convert to 544uS ~ 2400uS (center:1472uS)
 *  for the real value: 544uS: OCR1B = 68, 2400uS: OCR1B = 300.
 *  
 *  VIVIPARTS "servox2" has protection about 15 degree at the edge on the FW.
 *  This code has 20 degree protection at the edge.
 *  
 */

#define LED 13
#define SERVOPORT 10 //don't change!
#define SERVOPORT2 9 //don't change!
#define TRIGOUT 8
#define ECHOIN 7
#define SW1 3

int Mode;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  Serial.println("SW test");
  pinMode(LED,OUTPUT);
  pinMode(SERVOPORT,OUTPUT);
  pinMode(SERVOPORT2,OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(TRIGOUT,OUTPUT);
  pinMode(ECHOIN, INPUT_PULLUP);
  TCCR1A &= ~((1<<WGM10)|(1<<COM1B0)|(1<<COM1A0)); 
  TCCR1A |= (1<<WGM11) |(1<<COM1B1) | (1<<COM1A1);
  TCCR1B |= (1<<WGM12) |(1<<WGM13);
  ICR1 = 20000/4; //20mS
  OCR1A = 1500/4; //1.5mS
  OCR1B = 1500/4; //1.5mS
//  pinMode(A0,OUTPUT); //for debug LED
//  digitalWrite(A0,HIGH);
  delay(1000);
  Mode = 0;
}

void servo(unsigned int angle) {
  if(angle>180) angle = 180;
  angle =  94 + angle;
  OCR1B = angle;
}
void servo2(unsigned int angle) {
  //if(angle>180) angle = 180;
  angle =  94 + angle;
  OCR1A = angle;
}

#define NEXT_POS_DELAY 50
#define TIMEOUT 10
int Timer;
int swTest() {
  int i, val;
  val = 0;
  i = digitalRead(SW1);
  if(i==0) {
    if(Timer>TIMEOUT) {
      Mode++;
      if(Mode>3) Mode = 1;
      val = -1;
      Serial.print("Mode:");
      Serial.println(Mode);
    }
    Timer=0;
  } else {
    if(Timer<(TIMEOUT+5)) {
      Serial.print("Timer:");
      Serial.println(Timer);
      Timer++;
    }
  }  
  return val;
}
void sweep() {
  int i,j;
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  for(i=10; i<2000; i+=10) {
    OCR1A = 6*i/4;
    delay(NEXT_POS_DELAY);
    j=swTest();
    if(j<0) return;
  }
}
int getTime() {
  int duration;
  delay(20);
  digitalWrite(TRIGOUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGOUT, LOW);
  digitalWrite(LED,HIGH);
  duration = pulseIn(ECHOIN, HIGH);
  digitalWrite(LED, LOW);
  duration = duration /59;
  return duration;
}
void loop() {
  int i;
  delay(10);
  swTest();
  switch(Mode) {
    case 0:
      sweep();
      break;
    case 1:
      OCR1A = 500/4*6;
      digitalWrite(LED,LOW);
      break;
    case 2:
      digitalWrite(LED,HIGH);
      OCR1A = 2000/4*6;
      break;  
    case 3:
      i=getTime();
      Serial.print(i);
      Serial.println("cm");
      OCR1A = i*10/4;
    default:
      break;
  }
}
