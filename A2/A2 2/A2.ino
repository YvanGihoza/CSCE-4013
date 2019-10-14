#include <Servo.h>
#include <string.h>
#include <WiFi101.h>
#include <PubSubClient.h>
//the liquid crytal library
#include <LiquidCrystal.h>

//username
#define USERNAME "ygihoza"
#define PUBLISH_TOPIC "light/" USERNAME
// Update these with values suitable for your network.
#define LCD "uark/csce5013/ygihoza/lcd"
#define ANGLES "uark/csce5013/ygihoza/angles"
#define PHRASE "uark/csce5013/ygihoza/phrase"
//servomotor declarations
#define servoPin 9

Servo servo;
//Global variables for storing the message and angles
char *lcdMessage;
int angles[12];

char ssid[] = "ATT4f5xy9K";
char password[] = "96jjm6p77vbi";
int status = WL_IDLE_STATUS;
const char* mqtt_server = "thor.csce.uark.edu";

//the pins used the the liquid crystal
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

WiFiClient wifiClient;
PubSubClient client(wifiClient);
long lastMsg = 0;
char msg[50];
int value = 0;
long lastCheckConnected = -1000;


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //servo to pin defined
  servo.attach(servoPin);
  //initialize the LCd
  lcd.begin(16, 2);
  //lcd.print("BROKEN");
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (status != WL_CONNECTED) {
      status = WiFi.begin(ssid, password);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  //check if the topic is LCD
  int checkLCD = strcmp(LCD,topic); 
  //check if the topic is angles
  int checkANGLES = strcmp(ANGLES,topic);
  lcd.setCursor(0, 0);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //if the topic is lcd
    if(checkLCD == 0)
    {
      //copy the payload to the lcd global variable
      lcdMessage = (char*)payload[i];
      lcd.print((char)payload[i]);
      delay(500);
      //lcd.autoscroll();
      //lcd.print((char)lcdMessage[i]);
      //lcd.clear();
      //lcd.print("adjust");
    }
     
    //if the the topic is angles
    else if(checkANGLES == 0)
    {
      char * check = (char*)payload[i];
      char * token = strtok(check,",");
      int angle = atoi(token);
      //Serial.print(angle);
      //Serial.println(atoi((char*)check));
      //angles[i] = atoi(strtok (((char*)payload[i]),","));
      //Serial.println(angle);
      servo.write(angle);
    }
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(USERNAME)) {
      Serial.println("connected");
      // Once connected, subscribe to input channel
      //subscribe to the channels lcd, angles and phrase
      client.subscribe(LCD);
      client.subscribe(ANGLES);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  //lcd.setCursor(0,0);
  //lcd.setCursor(16, 1);
  //lcd.autoscroll();
  //lcd.noAutoscroll();
  if(millis() - lastCheckConnected >= 1000){
    if(!client.connected()) {
      reconnect();
    }
    client.loop();
    lastCheckConnected = millis();
  }
  value = analogRead(A0);
  snprintf(msg,10,"%03d",value);
  client.publish(PUBLISH_TOPIC,msg);
  
  // turn off automatic scrolling
  //lcd.noAutoscroll();

  // clear screen for the next loop:
  delay(500);
  lcd.clear();
}
