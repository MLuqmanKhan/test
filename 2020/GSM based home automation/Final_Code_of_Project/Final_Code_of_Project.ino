/***** RemoteXY select connection mode and include library 
 **********Including the recommanded libraries***********/
  
#define REMOTEXY_MODE__ESP32CORE_WIFI_CLOUD
#include "DHT.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <RemoteXY.h>

/*****control humidity and temperature variables******/

int x;
int y;
String incomingData;   // for storing incoming serial data
String message = "";   // A String for storing the message    // Initialized a pin for relay module
bool running_RELAY=false;

/******************************************************************
************Things Board interface using MQTT protocol************* 
*******************************************************************/

#define WIFI_AP "FAB LAB"
#define WIFI_PASSWORD "Proj3ct123"

#define TOKEN "uN9YNlKLBz8hu8HxqIUq"
char thingsboardServer[] = "demo.thingsboard.io";

/*******Define DHT11 pin*******/

#define DHTPIN 4
#define DHTTYPE DHT11
#define RXD2 16
#define TXD2 17

/******************************************************************
******************RemoteXY connection settings********************* 
*******************************************************************/
 
#define REMOTEXY_WIFI_SSID "FAB LAB"
#define REMOTEXY_WIFI_PASSWORD "Proj3ct123"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "be3defc50356ba6276c8f3d18a0eec07"

WiFiClient espClient;

/****Initialize DHT sensor*****/

DHT dht(DHTPIN, DHTTYPE);

PubSubClient client(espClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

/*****RemoteXY configurate*****/  

#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,4,0,0,0,71,0,10,13,1,
  2,0,4,28,22,11,2,26,31,31,
  79,78,0,79,70,70,0,2,0,36,
  28,22,11,2,26,31,31,79,78,0,
  79,70,70,0,2,0,36,54,22,11,
  2,26,31,31,79,78,0,79,70,70,
  0,2,0,5,54,22,11,2,26,31,
  31,79,78,0,79,70,70,0 };
  
/******this structure defines all the variables and******
 ******events of your control interface******************/
 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  uint8_t switch_2; // =1 if switch ON and =0 if OFF 
  uint8_t switch_4; // =1 if switch ON and =0 if OFF 
  uint8_t switch_3; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_1 22
#define PIN_SWITCH_2 21
#define PIN_SWITCH_4 18
#define PIN_SWITCH_3 19

void setup()
{
     RemoteXY_Init (); 

  pinMode (PIN_SWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  pinMode (PIN_SWITCH_4, OUTPUT);
  pinMode (PIN_SWITCH_3, OUTPUT);
   
   // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
  
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  Serial.println("Serial begin");

  /*******************************************************
   ********Initially making the relay pins High because of
   ****************output of relay module*****************
   *******************************************************/
   
  pinMode(18, OUTPUT);   // Setting erlay pin as output pin
  digitalWrite(18,HIGH);  // Making relay pin initailly low
  pinMode(19, OUTPUT);   // Setting erlay pin as output pin
  digitalWrite(19, HIGH);  // Making relay pin initailly low
  pinMode(21, OUTPUT);   // Setting erlay pin as output pin
  digitalWrite(21, HIGH);  // Making relay pin initailly low
  pinMode(22, OUTPUT);   // Setting erlay pin as output pin
  digitalWrite(22, HIGH);  // Making relay pin initailly low
  Serial.println("Serial Monitor");
  
  // set SMS mode to text mode
  Serial2.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to tp show the output on serial out
  Serial2.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);
}

void loop()
{
  /*********************************************************
   **********Controling devices using android app***********
   *********************************************************/

RemoteXY_Handler ();

  if(running_RELAY==false){
  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==1)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_2, (RemoteXY.switch_2==1)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_4, (RemoteXY.switch_4==1)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_3, (RemoteXY.switch_3==1)?LOW:HIGH);
  }
  if ( !client.connected() ) {
  reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
  getAndSendTemperatureAndHumidityData();
  lastSend = millis();
  }

   client.loop();
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 
  receive_message();   //Function for receiving sms

/*************************************************************
 *******************RELAY 1**********************************
 ***********************************************************/
  // if received command is to turn on relay
  if(incomingData.indexOf("Relay1on")>=0)
  {
    digitalWrite(22, LOW);
    message = "Relay 1 is turned ON";
    running_RELAY=true;
    send_message(message);    // Send a sms back to confirm that the relay is turned on
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("Relay1off")>=0)
  {
    digitalWrite(22, HIGH);
    message = "Relay 1 is turned OFF";
    running_RELAY=false;
    send_message(message);    // Send a sms back to confirm that the relay is turned off
  }    
  
/*************************************************************
 *******************RELAY 2**********************************
 ***********************************************************/
   if(incomingData.indexOf("Relay2on")>=0)
  {
    digitalWrite(21, LOW);
    message = "Relay 2 is turned ON";
    running_RELAY=true;
    send_message(message);    // Send a sms back to confirm that the relay is turned on
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("Relay2off")>=0)
  {
    digitalWrite(21, HIGH);
    running_RELAY=false;
    message = "Relay 2 is turned OFF";
    send_message(message);    // Send a sms back to confirm that the relay is turned off
  } 
  
/*************************************************************
 *******************RELAY 3**********************************
 ***********************************************************/       
   if(incomingData.indexOf("Relay3on")>=0)
  {
    digitalWrite(19, LOW);
    message = "Relay 3 is turned ON";
        running_RELAY=true;
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("Relay3off")>=0)
  {
    digitalWrite(19, HIGH);
    running_RELAY=false;
    message = "Relay 3 is turned OFF";
    // Send a sms back to confirm that the relay is turned off
    send_message(message);
  }      
  
/*************************************************************
 *******************RELAY 4**********************************
 ***********************************************************/  
   if(incomingData.indexOf("Relay4on")>=0)
  {
    digitalWrite(18, LOW);
    message = "Relay 4 is turned ON";
        running_RELAY=true;
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("Relay4off")>=0)
  {
    digitalWrite(18, HIGH);
    running_RELAY=false;
    message = "Relay 4 is turned OFF";
    // Send a sms back to confirm that the relay is turned off
    send_message(message);
  }
   /*************************************************************
 *******************ALL RELAYS**********************************
 ***********************************************************/  
   if(incomingData.indexOf("Relayallon")>=0)
  {
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    digitalWrite(21, LOW);
    digitalWrite(22, LOW);
    message = "All Relays are turned ON";
        running_RELAY=true;
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("Relayalloff")>=0)
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(21, HIGH);
    digitalWrite(22, HIGH);
    running_RELAY=false;
    message = "All Relays are turned OFF";
    // Send a sms back to confirm that the relay is turned off
    send_message(message);
  }
  /***********************************************************
   ******Conditon if humidity or temperature is exceeded****** 
   *****************from the limited values*******************/
   
if(x>=60)
{
  int state= digitalRead(22);
  if(state==0){
  digitalWrite(22,HIGH);
  message = "Humidity is exceeded from the limited value so the Relay 1 is turned OFF";
  send_message(message);    // Send a sms back to confirm that the relay is turned off
  }
}
if(y>=40)
{
  int state= digitalRead(21);
  if(state==1){
  digitalWrite(21,LOW);
  message = "Temperature is exceeded from the limited value so the Relay 2 is turned ON";
  send_message(message);    // Send a sms back to confirm that the relay is turned off
  }
}
}

/************************************************************
 **************Recieve message by GSM module***************
 ************************************************************/  

void receive_message()
{
  if (Serial2.available() > 0)
  {
    incomingData = Serial2.readString(); // Get the data from the serial port.
    Serial.print(incomingData); 
    delay(10); 
  }
}

/************************************************************
 ****************Send message by GSM module******************
 ************************************************************/  

void send_message(String message)
{
  Serial2.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  Serial2.println("AT+CMGS=\"+923474232708\""); // Replace it with your mobile number
  delay(100);
  Serial2.println(message);   // The SMS text you want to send
  delay(100);
  Serial2.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  Serial2.println();
  delay(1000);  
}
/*********************************************************************
 ***Getting data from DHT11 sensor and sending to thingsboad server***
 *********************************************************************/  
 
void getAndSendTemperatureAndHumidityData()
{
Serial.println("Collecting temperature data.");

// Reading temperature or humidity takes about 250 milliseconds!
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();

// Check if any reads failed and exit early (to try again).
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" *C ");

String temperature = String(t);
String humidity = String(h);

x=h;
y=t;

// Just debug messages
Serial.print( "Sending temperature and humidity : [" );
Serial.print( temperature ); Serial.print( "," );
Serial.print( humidity );
Serial.print( "] -> " );

// Prepare a JSON payload string
String payload = "{";
payload += "\"temperature\":"; payload += temperature; payload += ",";
payload += "\"humidity\":"; payload += humidity;
payload += "}";

// Send payload
char attributes[100];
payload.toCharArray( attributes, 100 );
client.publish( "v1/devices/me/telemetry", attributes );
Serial.println( attributes );

}

/************************************************************
 ******************Initializing the WIFI*********************
 ************************************************************/  

void InitWiFi()
{
Serial.println("Connecting to AP ...");
// attempt to connect to WiFi network

WiFi.begin(WIFI_AP, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Connected to AP");
}
void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
status = WiFi.status();
if ( status != WL_CONNECTED) {
WiFi.begin(WIFI_AP, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Connected to AP");
}
Serial.print("Connecting to Thingsboard node ...");
// Attempt to connect (clientId, username, password)
if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
Serial.println( "[DONE]" );
} else {
Serial.print( "[FAILED] [ rc = " );
Serial.print( client.state() );
Serial.println( " : retrying in 5 seconds]" );
// Wait 5 seconds before retrying
delay( 5000 );
}
}
}
