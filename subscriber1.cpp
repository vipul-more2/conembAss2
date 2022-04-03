#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <unistd.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <json/json.h>
#include <json-c/json.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <wiringPi.h>
#include <iostream>

#define ADDRESS "tcp://192.168.0.227:1883"
#define CLIENTID "rpi4"
#define AUTHMETHOD "vipul"
#define AUTHTOKEN "groot"
#define TOPIC "ee513/CPUTemp"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

using namespace std;

void tempCheck(double temp){
	
	printf("Temperature Check\n");
	
	if(temp > 20){

	  printf("The Temperature %f is too high!\n",temp);
	  if(wiringPiSetup() == -1){

		printf("Exit");

	  }

	  pinMode(0,OUTPUT);
	  digitalWrite(0,1);
	usleep(10000);
	  digitalWrite(0,0);
	usleep(10000);

	}
}

void json_parse(){
}

/*
void json_parse(json_object *obj){

	printf("JSON parse\n");

	enum json_type type;
	json_object_object_foreach(obj, key, val){
	printf("JSON parse for loop\n");
	//printf("Value:%f\n",val);
	type = json_object_get_type(val);
	//printf("Type Value:%s\n",type);
	switch(type){
	  case json_type_double: printf("type: json_type_double, ");
	  printf("Temperature: %f\n", json_object_get_double(val));
	
	  tempCheck(json_object_get_double(val));

          break;
	}
	}

}

*/
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
//	json_object * obj = json_tokener_parse((char*) message->payload);
//	json_parse(obj);
/*string mesg(payloadptr);
//mesg.append(payloadptr);
cout<<mesg<<endl;
string text ="{\"1\":{\"name\":\"MIKE\",\"surname\":\"TAYLOR\"},\"2\":{\"name\":\"TOM\",\"surname\":\"JERRY\"}}";
//string text = mesg;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();

  Json::Value root;
  string errors;

  bool parsingSuccessful = reader->parse(text.c_str(), text.c_str() + text.size(), &root, &errors);
  delete reader;

  if ( !parsingSuccessful )
  {
    cout << text << endl;
    cout << errors << endl;
  }

  for( Json::Value::const_iterator outer = root.begin() ; outer != root.end() ; outer++ )
  {
    for( Json::Value::const_iterator inner = (*outer).begin() ; inner!= (*outer).end() ; inner++ )
    {
      cout << inner.key() << ": " << *inner << endl;
cout<< "12";
    }
  }
	printf("End of JSON parsing");

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
*/
const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json)

Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
 
    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
 
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl

}
void connlost(void *context, char *cause) {
	printf("\nConnection lost\n");
	printf(" cause: %s\n", cause);
}
int main(int argc, char *argv[]) {
	MQTTClient client;
	MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;
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
	do {
		ch = getchar();
	} while (ch != 'Q' && ch != 'q');
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;

}

