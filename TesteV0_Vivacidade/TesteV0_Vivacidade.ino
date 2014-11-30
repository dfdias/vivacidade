// Init the Pins used for PWM
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

// Init the Pins used for 10K pots
const int redPotPin = 0;
const int greenPotPin = 1;
const int bluePotPin = 2;

// Init our Vars
int currentColorValueRed;
int currentColorValueGreen;
int currentColorValueBlue;

int avg[100];
int last_avg = 0;
boolean ledState = false;
int r_avg;

void setup()
{
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  ledState = false;

  pinMode(A0, INPUT);

  int len = sizeof(avg)/sizeof(int);
  for(int i = 0; i<len; i++) {
    avg[i] = analogRead(A0);
  }

  r_avg = running_average(A0);
}

int running_average(int port) {
  int len = sizeof(avg)/sizeof(int);
  last_avg = (last_avg+1) % len;

  avg[last_avg] = analogRead(port);

  int avg_t = 0;
  for(int i = 0; i<len; i++) {
    avg_t += avg[i]/len;
  }
  return avg_t;
}

void loop()
{
  // Read the voltage on each analog pin then scale down to 0-255 and inverting the value for common anode
  //currentColorValueRed = (255 - map( analogRead(rgedPotPin), 0, 1024, 0, 255 ) );
  //currentColorValueBlue = (255 - map( analogRead(bluePotPin), 0, 1024, 0, 255 ) );
  //currentColorValueGreen = (255 - map( analogRead(greenPotPin), 0, 1024, 0, 255 ) );

  //int a0Val = analogRead(A0);
  int a0 = 0;
  for(int i = 0; i<5; i++)
  {
    a0 += analogRead(A0);
  }

  a0 = a0/5;

  int dif = (a0 - r_avg);
  float percent = (float) dif/r_avg;

  Serial.print(a0);
  Serial.print(" - " + String(r_avg));
  Serial.println(" - " + String(percent));

  if(percent > 0.1) {
    analogWrite(redPin, 255);
    analogWrite(bluePin, 0);
    analogWrite(greenPin, 0);

    ledState = true;
  } 
  else {
    analogWrite(redPin, 0);
    analogWrite(bluePin, 255);
    analogWrite(greenPin, 255);

    ledState = false;

    //r_avg = running_average(A0);
  }

  r_avg = running_average(A0);

  /*
 if(a0Val<900) {
   analogWrite(redPin, 255);
   analogWrite(bluePin, 255);
   analogWrite(greenPin, 0);
   
   } else {
   analogWrite(redPin, 0);
   analogWrite(bluePin, 255);
   analogWrite(greenPin, 255);
   }
   */

  // Write the color to each pin using PWM and the value gathered above
  //  analogWrite(redPin, 255);
  //  analogWrite(bluePin, 255);
  //  analogWrite(greenPin, 0);

  //RGB Fade Test
  //  int i;
  //  for(i=0;i<255;i++) {
  //    analogWrite(redPin, i);
  //    delay(10);
  //    int j;
  //    for(j=0;j<255;j++) {
  //      analogWrite(bluePin, j);
  //      delay(10);
  //      int k;
  //      for(k=0;k<255;k++) {
  //        analogWrite(greenPin, k);
  //        delay(10);
  //      }
  //    }
  //  }

}


