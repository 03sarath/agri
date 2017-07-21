/*
  MySQL Connector/Arduino Example : complex insert

  This example demonstrates how to issue an INSERT query to store data in a
  table using data from variables in our sketch. In this case, we supply the
  values dynamically.

  This sketch simulates storing data from a sensor in a table.

  For this, we will create a special database and table for testing.
  The following are the SQL commands you will need to run in order to setup
  your database for running this sketch.

  CREATE DATABASE vibration_test;
  CREATE TABLE vibration_test.level (
    num integer primary key auto_increment,
    vibrated char(40),
    value float,
    recorded timestamp
  );

  Here we have a table that contains an auto_increment primary key, a text
  field, a field to identify the sensor, the value read, and timestamp of
  the recorded data.

  Note: Since this sketch uses test data, we place the INSERT in the setup()
        method so that it runs only once. Typically, you would have the
        INSERT in the loop() method after your code to read from the sensor.

  INSTRUCTIONS FOR USE

  1) Create the database and table as shown above.
  2) Change the address of the server to the IP address of the MySQL server
  3) Change the user and password to a valid MySQL user and password
  4) Connect a USB cable to your Arduino
  5) Select the correct board and port
  6) Compile and upload the sketch to your Arduino
  7) Once uploaded, open Serial Monitor (use 115200 speed) and observe
  8) After the sketch has run for some time, open a mysql client and issue
     the command: "SELECT * FROM test_arduino.hello_sensor" to see the data
     recorded. Note the field values and how the database handles both the
     auto_increment and timestamp fields for us. You can clear the data with
     "DELETE FROM test_arduino.hello_sensor".

  Note: The MAC address can be anything so long as it is unique on your network.

  Created by: Dr. Charles A. Bell
*/
#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

int EP=16;



IPAddress server_addr(192,168,11,177);  // IP of the MySQL *server* here
char user[] = "sarath";              // MySQL user login username
char password[] = "12345678";        // MySQL user login password

// WiFi card example
char ssid[] = "M G BLOCK NEW";
char pass[] = "";

const IPAddress ipadd(192,168,11,222);     //-------these 3 require-------------- 
const IPAddress ipgat(192,168,11,1);       //--32 more bytes than const uint8_t--
const IPAddress ipsub(255,255,255,0);     //------------------------------------

// Sample query
char INSERT_RFID[] = "INSERT INTO vibration_test.level (RFID) VALUES ('%s')";
char queryrfid[128];
char rfid[10];

WiFiClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
    Serial.begin(115200);
    //vibration sensor*************
     
      pinMode(EP,INPUT);//Set EP input from measurement
      Serial.println("--------------------RFID-------------");

  

// Begin WiFi section
WiFi.begin(ssid, pass);
WiFi.config(ipadd, ipgat, ipsub);
// Wait for connection
while ( WiFi.status() != WL_CONNECTED ) {
delay ( 500 );
Serial.print ( "." );
}
Serial.println ( "" );
Serial.print ( "Connected to " );
Serial.println ( ssid );
Serial.print ( "IP address: " );
Serial.println ( WiFi.localIP() );
// End WiFi section
Serial.println("DB - Connecting...");
while (conn.connect(server_addr, 3306, user, password) != true) {
Serial.print ( "." );
}
 
}


void loop() {

long measurement=TP_init();
  delay(50);
  Serial.print("measurement= ");
  Serial.println(measurement);
 

 // Initiate the query class instance
    MySQL_Cursor *cur_rfid = new MySQL_Cursor(&conn);
    // Save
    dtostrf(measurement, 1, 1, rfid);
    sprintf(queryrfid, INSERT_RFID,rfid);
    // Execute the query
    cur_rfid->execute(queryrfid);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_rfid;
    Serial.println("Data recorded.");
}


  long TP_init()
  {
    delay(10);
    long measurement=pulseIn(EP,HIGH);
    return measurement;
    
    }
