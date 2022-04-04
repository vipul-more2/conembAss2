#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <json-c/json.h>
#include <wiringPi.h>
#include <unistd.h>
#include <string>


#define ADDRESS     "tcp://192.168.0.227:1883"
#define CLIENTID    "rpi8"
#define AUTHMETHOD  "vipul"
#define AUTHTOKEN   "groot"
#define TOPIC       "ee513/CPUTemp"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L


using namespace std;


void json_parse(json_object *jobj) {

	printf("JSON parse\n");
	int count = 0;
	float temperature,time,sensorpitch,sensorroll;
	enum json_type type;
	json_object_object_foreach(jobj, key, val)
	{
		type = json_object_get_type(val);
		count++;
		switch (count) {
		case 1:
			temperature = json_object_get_double(val);
			break;

		case 2:
		    time = json_object_get_double(val);
			break;

		case 3:
			sensorpitch = json_object_get_double(val);
			break;

		case 4:
			sensorroll = json_object_get_double(val);
			break;
		}

//	   sensorsafetycheck(sensorpitch,sensorroll);


	}
	sensorsafetycheck(sensorpitch,sensorroll);
	

}


 void sensorsafetycheck(float sensorpitch, float sensorroll)
{
int alarmtrigger = 0;
    if(sensorpitch==25 && sensorroll == -15)
	{
	printf("Object A near please move away");
	alarmtrigger=1;
	}
	if (sensorpitch==45 && sensorroll == -35)
	{
	alarmtrigger=1;
	printf("Object B near, please move away");
	}
	if (sensorpitch==50 && sensorroll == 25)
	{
	printf("Object C near, please move away");
		alarmtrigger=1;
	}

	if (sensorpitch!=10)
	{
		if (sensorroll <30)
		{
		    printf("Don't leave the boundary, shift your location");


			if (wiringPiSetup() == -1){

			printf("Exit");

			}

			pinMode (2,OUTPUT);

			digitalWrite(2, 1);

			usleep(50000);

			digitalWrite(2, 0);

    		usleep(50000);
		}
	}


}





volatile MQTTClient_deliveryToken deliveredtoken;


void delivered(void *context, MQTTClient_deliveryToken dt) {

    printf("Message with token value %d delivery confirmed\n", dt);

    deliveredtoken = dt;

}



int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

    int i;

    char* payloadptr;

    printf("Message arrived\n");

    printf("     topic: %s\n", topicName);

    printf("   message: ");

    payloadptr = (char*) message->payload;

    for(i=0; i<message->payloadlen; i++) {

        putchar(*payloadptr++);

    }

    putchar('\n');

    json_object * jobj = json_tokener_parse((char*) message->payload);

    json_parse(jobj);

    MQTTClient_freeMessage(&message);

    MQTTClient_free(topicName);

    return 1;

}



void connlost(void *context, char *cause) {

    printf("\nConnection lost\n");

    printf("     cause: %s\n", cause);

}



int main(int argc, char* argv[]) {

    MQTTClient client;

    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;

    int rc;

    int ch;



    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);

    opts.keepAliveInterval = 20;

    opts.cleansession = 0;

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



    do {

        ch = getchar();

    } while(ch!='Q' && ch != 'q');

    MQTTClient_disconnect(client, 10000);

    MQTTClient_destroy(&client);

    return rc;

}



