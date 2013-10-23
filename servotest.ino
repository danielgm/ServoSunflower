
#include <Servo.h> 

const int PAN = A1;
const int TILT = A2;
const int LDR = A5;

const int TILT_MIN = 60;
const int TILT_MAX = 180;
const int TILT_STEPS = 8;
const int PAN_MIN = 0;
const int PAN_MAX = 180;
const int PAN_STEPS = 9;

Servo panServo, tiltServo;

bool isSearching;
int index, highestReading, highestReadingIndex;

void setup() {
  Serial.begin(9600);
  
  pinMode(1, OUTPUT);
  panServo.attach(14); // Analog pin 0.
	tiltServo.attach(15); // Analog pin 1.

	isSearching = true;
	index = 0;
	highestReading = 0;
	highestReadingIndex = -1;

	tiltServo.write(90);
	panServo.write(0);
} 

void loop() { 
	if (isSearching) {
		goToPosition(index);

		int reading = analogRead(A5);
		if (reading > highestReading) {
			highestReading = reading;
			highestReadingIndex = index;

			int pan = floor(index / TILT_STEPS);
			int tilt = index % TILT_STEPS;
			if (pan % 2 == 1) {
				tilt = TILT_STEPS - tilt - 1;
			}
			Serial.println("New high found at position " + String(index));
			Serial.println("Value: " + String(reading));
			Serial.println("Pan: " + String(map(pan, 0, PAN_STEPS, PAN_MIN, PAN_MAX)));
			Serial.println("Tilt: " + String(map(tilt, 0, TILT_STEPS, TILT_MIN, TILT_MAX)));
		}

		index++;
		if (index >= PAN_STEPS * TILT_STEPS) {
			isSearching = false;

			goToPosition(highestReadingIndex);
			delay(5000);
			index = 0;
			highestReading = 0;
			highestReadingIndex = -1;
			isSearching = true;
		}
	}
} 

void goToPosition(int i) {
	// Boustrophedonic search pattern.
	int pan = floor(i / TILT_STEPS);
	int tilt = i % TILT_STEPS;
	if (pan % 2 == 1) {
		tilt = TILT_STEPS - tilt - 1;
	}

	panServo.write(constrain(map(pan, 0, PAN_STEPS, PAN_MIN, PAN_MAX), PAN_MIN, PAN_MAX));
	tiltServo.write(constrain(map(tilt, 0, TILT_STEPS, TILT_MIN, TILT_MAX), TILT_MIN, TILT_MAX));
	delay(100);
}

