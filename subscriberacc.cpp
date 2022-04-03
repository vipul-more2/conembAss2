#include "stdio.h"

#include "stdlib.h"

#include <string>

#include "MQTTClient.h"

#include <json-c/json.h>

#include <wiringPi.h>

#include <unistd.h>


#define ADDRESS     "tcp://192.168.0.227:1883"

#define CLIENTID    "rpi4"

#define AUTHMETHOD  "vipul"

#define AUTHTOKEN   "groot"

#define TOPIC       "ee513/CPUTemp"

#define PAYLOAD     "Hello World!"

#define QOS         1

#define TIMEOUT     10000L



using namespace std;


void json_parse(json_object *jobj) { //parsing json data

printf("Parser Called 1");

 enum json_type type;

 json_object_object_foreach(jobj, key, val) {

 type = json_object_get_type(val);

 double temperature;

printf("Parser Called 2");

 switch (type) {

   case json_type_double: printf("type: json_type_double, \n");

   printf("Temperature value: %fn \n", json_object_get_double(val));

   temperature = json_object_get_double(val);

   break;

 }

 

 if (temperature >= 35){//triggers alarm using gpio0
     double temp=35.0;
     temp=temperature-temp;

     printf("The temperature exceeds the limit of 35 by %f\n", temp);

	if (wiringPiSetup() == -1){

		printf("Exit");

	}

	pinMode (0,OUTPUT);

	digitalWrite(0, 1);

    usleep(10000);

	digitalWrite(0, 0);

    usleep(10000);

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
 char  str[] = "{ \"Temperature\" : 40.00 }";

    json_object *jobj = json_tokener_parse(str);

printf("function calling");
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

if (wiringPiSetup()<0)
{
printf("setup fail");
exit(1);
}

for (int i=0;i<5;++i)
{
pinMode (0,OUTPUT);
digitalWrite(0,1);
usleep(10000);
digitalWrite(0,0);
usleep(10000);
}

    return rc;

}



