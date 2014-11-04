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

void setup()
{
  Serial.begin(9600);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  pinMode(A0,INPUT);
}
 
void loop()
{
// Read the voltage on each analog pin then scale down to 0-255 and inverting the value for common anode
  //currentColorValueRed = (255 - map( analogRead(rgedPotPin), 0, 1024, 0, 255 ) );
  //currentColorValueBlue = (255 - map( analogRead(bluePotPin), 0, 1024, 0, 255 ) );
  //currentColorValueGreen = (255 - map( analogRead(greenPotPin), 0, 1024, 0, 255 ) );
 
 int a0Val = analogRead(A0);
 
 delay(100);
 Serial.println(a0Val);
 
 if(a0Val<970) {
   analogWrite(redPin, 255);
   analogWrite(bluePin, 255);
   analogWrite(greenPin, 0);
   
 } else {
   analogWrite(redPin, 0);
   analogWrite(bluePin, 255);
   analogWrite(greenPin, 255);
 }
 
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
