#define YUN

#ifdef YUN
#include <Bridge.h>
#include <YunClient.h>
#include <PubSubClient.h>
#endif

#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "FastLED.h"

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// Set up nRF24L01 radio on SPI bus plus pins 3 & 4

#ifdef YUN 
RF24 radio(3, 4);
#else
RF24 radio(9, 10);
#endif

// NeoPixel Communication
#define BUFFER_SIZE 32
char packet[BUFFER_SIZE] = {0};

String readSerial = "";

// Role
// The various roles supported by this sketch
typedef enum { role_listener = 1, role_sender } role_e;
const char* role_friendly_name[] = { "invalid", "Listener", "Sender"};

role_e role = role_listener;

#ifdef YUN
YunClient yun;
PubSubClient client("shineupon.me", 1883, callback, yun);
#endif

// How many leds in your strip?
#define NUM_LEDS 9
#define DATA_PIN 2
// Define the array of leds
CRGB leds[NUM_LEDS];

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("MQTT Bitmap:");
 Serial.print(length);
 Serial.print(":");
 role = role_sender;

 for(int i=0; i<length; i++)
  readSerial+=(char) payload[i];
 Serial.println(readSerial);

}


// Callback function header
void setup() {

 Serial.begin(115200);
#ifdef YUN
 if  (!Serial) {
  delay(5000);
 }
#endif

 printf_begin();
 Serial.println("Setup BEGIN");

 radio.begin();
 // enable dynamic payloads
 radio.enableDynamicPayloads();
 // optionally, increase the delay between retries & # of retries
 radio.setRetries(15, 15);
 radio.setAutoAck(false);
 radio.setDataRate(RF24_250KBPS);
 //radio.setChannel(1);
 radio.setPALevel(RF24_PA_MAX);

 radio.openWritingPipe(pipes[0]);
 radio.openReadingPipe(1, pipes[1]);

 radio.startListening();
 radio.printDetails();
 
 #ifdef YUN
 Serial.println("YUN Version");
 Bridge.begin();

 if(client.connect("hackAveiroMQTT")) {
  Serial.println("connected");
  client.subscribe("/hackaveiro/bitmap");
 }
 #endif

 Serial.println("Setup DONE");

 printf("*** CHANGING ROLE -- PRESS 'S' TO SWITCH TO: SENDER\n\r");
#ifndef YUN
FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
#endif
}

void loop() {
#ifdef YUN
    client.loop();


 if(!client.connected()) {
  client.connect("hackAveiroMQTT");
  Serial.println("Reconnected");
  client.subscribe("/hackaveiro/bitmap");
 }

#else
	static uint8_t hue = 0;
    FastLED.showColor(CHSV(hue, 255, 255));
#endif


 	if (role == role_listener)
	{
		if ( radio.available() )
		{
			// Dump the payloads until we've gotten everything
			bool done = false;
			while (!done) 
			{
				size_t len = radio.getDynamicPayloadSize();
				packet[len] = 0;
				// Fetch the payload, and see if this was the last one.
				done = radio.read( &packet, len );

				char cb = packet[0];
				char *payload = (packet+1);
	    		printf("Control Byte [ ");
	    		for( int i = 0; i < 8; i++) {
				    if (cb & 0x80)
				        printf("1");
				    else
				        printf("0");
				    cb <<= 1;
				}

				printf(" ] ");

				// Spew it
				printf("Got payload: %s\n\r",payload);
#ifndef YUN
				hue = (payload[0]-'0')*100+(payload[1]-'0')*10+(payload[2]-'0');
				printf("Hue: %i\n", hue);
#endif
				// Delay just a little bit to let the other unit
				// make the transition to receiver
				delay(20);
			}
		}
	}
	if ( role == role_sender && (readSerial.length() != 0) )
	{

		// Fill packet with readSerial
		unsigned int strSize = readSerial.length();
		char *control_byte = packet;
		char *payload = packet;
		++payload;

		// Initialize Control Byte
		*control_byte = 0x00;

		while(strSize != 0){
			strSize = readSerial.length();
			printf("String Length = %i \n\r", strSize);

			readSerial.substring(0, BUFFER_SIZE-2).toCharArray(payload, BUFFER_SIZE-1);
			readSerial.remove(0, BUFFER_SIZE-2);

			strSize = readSerial.length();
			printf("New String Length = %i \r\n", strSize);

			// More Packets Flag
			if( strSize != 0){
				// There are more packets to send
				(*control_byte) = (*control_byte) | 0x80;
			}else{
				// This is the last packet in this batch
				(*control_byte) = (*control_byte) & 0x7F;
			}

			// Update Packet Number
			(*control_byte) = (*control_byte) + 0x01;

			// First, stop listening so we can talk.
	    	radio.stopListening();
	    	bool ok = radio.write( &packet, strlen(packet) );
	    	if (ok){
	    		char cb = *control_byte;
	    		printf("Control Byte [ ");
	    		for( int i = 0; i < 8; i++) {
				    if (cb & 0x80)
				        printf("1");
				    else
				        printf("0");

				    cb <<= 1;
				}
				printf(" ] ");
				printf("Sent: '%s'\n\r", (byte*)packet+1);
			}
			else
				printf("failed.\n\r");

			// Now, continue listening
	    	radio.startListening();
	    }
	    // readSerial = "";

 	}
 	if ( Serial.available() )
 	{
 		role = role_listener;
 		char c = Serial.read();

 		if( c == '\r'){
			Serial.println();
			role = role_sender;

	 		// if ( c == 'L' && role == role_sender )
		  //   {
		  //   	printf("*** CHANGING ROLE -- PRESS 'S' TO SWITCH TO: SENDER\n\r");
				// // Become the primary transmitter (ping out)
				// role = role_listener;
				// radio.openWritingPipe(pipes[0]);
				// radio.openReadingPipe(1,pipes[1]);

		 	// }else if ( c == 'S' && role == role_listener ){
		 	// 	printf("*** CHANGING ROLE -- PRESS 'L' TO SWITCH TO: LISTENER\n\r");

				// // Become the primary receiver (pong back)
				// role = role_sender;
				// radio.openWritingPipe(pipes[1]);
				// radio.openReadingPipe(1,pipes[0]);
		  //   }

	    }else if( c == '\n' ){

    	}else{
    		readSerial += c;
    		Serial.print(c);
    	}
	}else{
		role = role_listener;
	}
}



