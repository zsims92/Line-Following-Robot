#include <Servo.h>

#define STRAIGHT 0;
#define LEFT 1;
#define RIGHT 2;

int leftSensor = 15;
int rightSensor = 16;
int button = 3;
int led = 7;
Servo leftWheel;
Servo rightWheel;

void setup(){
	Serial.begin(9600);

	pinSetup();

	leftWheel.attach(9);
	rightWheel.attach(10);
	
	PORTD |= (1 << PD7);
	calibrateSensors();
	PORTD &= ~(1 << PD7);
	
	delay(1000);
	
	timerSetUp();
}

int sen1max = 1024;
int sen1min = 0;
int sen2max = 1024;
int sen2min = 0;

void calibrateSensors(){
	int numCalibrate = 5;
}

void pinSetup(){
	//Enable the led as an output
	DDRD = (1 << PD7);
	
	//Enable the button as an input with the pullup resistor
	DDRD &= ~(1 << PD3);
	PORTD |= (1 << PD3);
	
	//Enable the sensors as inputs
	DDRC &= ~((1 << PC1) | (1 << PC2));
	
	/*Enable the analog converter*/
	ADMUX &= ~(1 << REFS1);
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void timerSetUp(){
	//Disable the interrupts
	cli();

	//Set up the timer for CTC mode and interrupts
	TCCR1A = 0;						/*Clear the timer*/
	TCCR1B = 0;
	OCR1A = 99;						/*This will give us a time of 10ms per interrupts*/
	TCCR1B |= (1 << WGM12);			/*Turn on CTC mode*/
	TCCR1B |= (1 << CS10);			/*This will set the prescaler as 1*/
	TIMSK1 |= (1 << OCIE1A);		/*Enable interrupts*/
	
	//Re-Enable the interrupts
	sei();
}

ISR(TIMER1_COMPA_vect){
	void sensorRead();
}

int sens1CurrVal = 0;
int sens2CurrVal = 0;

void sensorRead(){
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
}

int hexValToWrite = 0;
int leftSpeed = 0;
int rightSpeed = 0;

void updateMotors(){
	leftWheel.write(leftSpeed);
	rightWheel.write(rightSpeed);
	hexWrite();
}

void updateSpeeds(){
	//Depending on the reading I get from the sensors, I will need the change the speed of the wheels

}

void loop(){
	updateMotors();
	updateSpeeds();
}