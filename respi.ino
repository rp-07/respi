// NodeMCU-Ping-IO.ino
// For M&TSI 2021

#include "config.h"

// set up the 'colors' and 'distances' feed
AdafruitIO_Feed *distances = io.feed("distance");
AdafruitIO_Feed *colors = io.feed("colors");

// these correspond to the pins on your NodeMCU
#define trigPin 14    //D5 in Node MCU
#define echoPin 12    //D6 in Node MCU
const int greenPin = 5; //D1 in Node MCU
const int redPin = 4;   //D2 in Node MCU
const int bluePin = 0;  //D3 in Node MCU

// now, we will send this value to the cloud to tell it which color we want to show
// blue = 0, green = 1, and red = 2
int color;

void setup() {
  // declare pins as inputs and outputs
  pinMode (trigPin, OUTPUT );
  pinMode (echoPin, INPUT );
  pinMode (redPin, OUTPUT);
  pinMode (greenPin, OUTPUT);
  pinMode (bluePin, OUTPUT);
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  long duration , distance;
  digitalWrite (trigPin , LOW ); // start trig at 0
  delayMicroseconds (2);
  digitalWrite (trigPin , HIGH ); // the rising edge of trig pulse
  delayMicroseconds (10); // decides duration of trig pulse
  digitalWrite (trigPin , LOW ); // falling edge of the trig pulse
  // NOTE: echo pin reads HIGH till it receives the reflected signal
  duration = pulseIn (echoPin , HIGH ); // reading the duration for which echoPin was HIGH gives
  // the time the sensor receives a reflected signal at the echo pin
  distance = (duration / 2) / 29.1; // calculate the distance of the reflecting surface in cm
  
  // this section lights up LED based on distance
  // case 1: LED is red 
  if (distance<12 && distance>=0){
    color = 2;
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
  // case 2: LED is green 
  if (distance<40 && distance>=12){
    color = 1;
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  }
  // case 3: LED is blue
  if (distance>=40){
    color = 0;
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    Serial.println("Out of Range"); // print when distance is not in either range
  }
  Serial.println("Distance is:    ");
  Serial.println(distance);
  colors->save(color);
  delay(2000);
  distances->save(distance);

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(2000);

}
