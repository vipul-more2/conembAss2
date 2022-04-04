#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <unistd.h>
#include <string>

#include <json-c/json.h>

#include <wiringPi.h>

#define ADDRESS "tcp://192.168.0.227:1883"
#define CLIENTID "rpi11"
#define AUTHMETHOD "vipul"
#define AUTHTOKEN "groot"
#define TOPIC "ee513/CPUTemp"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

using namespace std;

void tempCheck(double temp){
	
	
	
	if(temp > 42){

	  printf("The Temperature %f is rising, kindly turn on the cooling fan!\n",temp);
	  
	  //checking the value for wiringpi, if it is -1, then the wiring and setup is incomplete and the code would not be executed further.
	  if(wiringPiSetup() == -1){

		printf("Exit");

	  }

	  //code to glow led's using wiringpi
	  pinMode(0,OUTPUT);
	  digitalWrite(0,1);
	  usleep(50000);
	  
	  //code to turn off the led using wiringpi
	  digitalWrite(0,0);
	  usleep(50000);

	}
}


void json_parse(json_object *obj){

	printf("JSON parse\n");

	enum json_type type;
	json_object_object_foreach(obj, key, val){
	
	type = json_object_get_type(val);
	double tmp;
	switch(type){
	  case json_type_double: printf("type: json_type_double, ");
	  printf("Current cpu temperature is : %f\n", json_object_get_double(val));
	
	  tmp = json_object_get_double(val);

          break;
	}
	}

}

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen,
		MQTTClient_message *message) {
	int i;
	char *payloadptr;
	printf("Message arrived\n");
	printf(" topic: %s\n", topicName);
	printf(" message: ");
	payloadptr = (char*) message->payload;
	for (i = 0; i < message->payloadlen; i++) {
		putchar(*payloadptr++);
	}
	putchar('\n');

	printf("Parsing JSON");
	json_object * obj = json_tokener_parse((char*) message->payload);
	json_parse(obj);

	

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void *context, char *cause) {
	printf("\nConnection lost\n");
	printf(" cause: %s\n", cause);
}

int main(int argc, char *argv[]) {
	MQTTClient client;
	MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
	int rc;
	int character;
	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE,
			NULL);
	opts.keepAliveInterval = 20;
	opts.cleansession = 1;
	opts.username = AUTHMETHOD;
	opts.password = AUTHTOKEN;
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
	if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}
	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
			"Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);

	//below code subscribes and wait initially untill the user manually exits the subscribing code
	do {
		character = getchar();
	} while (character != 'Q' && character != 'q');


	//Initiating the disconnect protocol
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}

