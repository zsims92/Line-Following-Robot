#include <Servo.h>
#include <avr/io.h>
#include <stdlib.h>

#define STRAIGHT 0;
#define LEFT 1;
#define RIGHT 2;

int leftSensor = 15;
int rightSensor = 16;
int button1 = 4;
int button2 = 2;
int led1 = 6;
int led2 = 5;
Servo leftWheel;
Servo rightWheel;
int stat = 0;


void setup(){
  Serial.begin(9600);

  pinSetup();

  leftWheel.attach(9);
  rightWheel.attach(10);

  leftWheel.write(90);
  rightWheel.write(93);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  int but1 = digitalRead(button1);
  int but2 = digitalRead(button2);

  PORTD |= (but1 << led1) | (but2 << led2);

  if(but1 == 0 && but2 == 0)
    stat = 3;
  else if(but1 == 0 && but2 == 1)
    stat = 2;
  else if(but1 == 1 && but2 == 0)
    stat = 1;
  else
    stat = 0;

  delay(1000);

  while(digitalRead(button1)){
  }

  PORTD &= ~((1 << led1) | (1 << led2));
  delay(1000);
}

void pinSetup(){
  //Enable the led as an output
  DDRD |= (1 << led1) | (1 << led2);

  //Enable the sensors as inputs
  DDRC &= ~((1 << 1) | (1 << 2));

  /*Enable the analog converter*/
  ADMUX &= ~(1 << REFS1);
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


int sens1CurrVal = 0;
int sens2CurrVal = 0;
int leftSpeed = 0;
int rightSpeed = 0;


void straddleLine1(){  
  /*Set up to read from ADC1*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
  ADMUX |= (1 << MUX0);

  ADCSRA |= (1 << ADSC);

  while(ADCSRA & 0b1000000){

  }
  sens1CurrVal = ADC;

  /*Set up to read from ADC2*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
  ADMUX |= (1 << MUX1);

  ADCSRA |= (1 << ADSC);
  while(ADCSRA & 0b1000000){

  }
  sens2CurrVal = ADC;

  leftSpeed = map(sens1CurrVal, 0, 1024, 30, 130);
  rightSpeed = map(sens2CurrVal, 0, 1024, 30, 130);

}

void straddleRight(){
  /*Set up to read from ADC1*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
  ADMUX |= (1 << MUX0);

  ADCSRA |= (1 << ADSC);

  while(ADCSRA & 0b1000000){

  }
  sens1CurrVal = ADC;

  /*Set up to read from ADC2*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
  ADMUX |= (1 << MUX1);

  ADCSRA |= (1 << ADSC);
  while(ADCSRA & 0b1000000){

  }
  sens2CurrVal = ADC;

  leftSpeed = map(sens1CurrVal, 0, 1024, 40, 100);
  rightSpeed = map(sens2CurrVal, 0, 1024, 100, 40);

  if(sens1CurrVal > 800 && sens2CurrVal < 200){
    leftSpeed = 100;
    rightSpeed = 30;
  }

  leftWheel.write(180 - leftSpeed);
  rightWheel.write(rightSpeed);
}

void straddleLeft(){
  /*Set up to read from ADC1*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
  ADMUX |= (1 << MUX0);

  ADCSRA |= (1 << ADSC);

  while(ADCSRA & 0b1000000){

  }
  sens1CurrVal = ADC;

  /*Set up to read from ADC2*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
  ADMUX |= (1 << MUX1);

  ADCSRA |= (1 << ADSC);
  while(ADCSRA & 0b1000000){

  }
  sens2CurrVal = ADC;

  leftSpeed = map(sens1CurrVal, 0, 1024, 33, 100);
  rightSpeed = map(sens2CurrVal, 0, 1024, 100, 36);

  leftWheel.write(180 - leftSpeed);
  rightWheel.write(rightSpeed);
}

void straddleLine2(){  
  /*Set up to read from ADC1*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
  ADMUX |= (1 << MUX0);

  ADCSRA |= (1 << ADSC);

  while(ADCSRA & 0b1000000){

  }
  sens1CurrVal = ADC;

  /*Set up to read from ADC2*/
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
  ADMUX |= (1 << MUX1);

  ADCSRA |= (1 << ADSC);
  while(ADCSRA & 0b1000000){

  }
  sens2CurrVal = ADC;

  leftSpeed = map(sens1CurrVal, 0, 1024, 50, 127);
  rightSpeed = map(sens2CurrVal, 0, 1024, 53, 120);

  if(sens1CurrVal > 700 && sens2CurrVal > 700){
    leftSpeed = 30;
    rightSpeed = 30;
  }

  leftWheel.write(180 - leftSpeed);
  rightWheel.write(rightSpeed);

}

void loop(){
  if(stat == 0)
    straddleLine1();
  else if(stat == 1)
    straddleRight();
  else if(stat == 2)
    straddleLeft();
  else if(stat == 3)
    straddleLine2();
}






