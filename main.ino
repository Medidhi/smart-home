#include<ESP8266WiFi.h>
#include <stdio.h>
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "IoT1"
#define WLAN_PASS       "drowpass"
#define light           "off"


// Add tokens to collect data
// UPDATE THES TOKENS
#define fan1 "5801198a7625420483f89bff"
#define fan2 "5801198e76254203a8326270"
#define light1 "5801198c7625420483f89c0f"
#define light2 "580119987625420483f89c4e"

#define token1  "UT7l53ABguMVGjreMsW22GZxfHD6wy"


//global variables
int i=0;
int i1=0;
int i2=0;
int i3=0;

WiFiServer server( 80); 


WiFiClient client;
void ubiSave_value(String value, String idvariable, String token) {
  
  // if you get a connection, report back via serial:
  //  WiFiClient client;
  int num=0;
  String var = "{\"value\": " + String(value)+"}";
  num = var.length();
  if (client.connect("things.ubidots.com", 80)) {

    Serial.println("connected ubidots");
    client.println("POST /api/v1.6/variables/"+idvariable+"/values HTTP/1.1");
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+String(num));
    client.println("X-Auth-Token: "+String(token));
    client.println("Host: things.ubidots.com\n");
    client.print(var);

  }
  else {
    Serial.println("ubidots connection failed");
  }

  if (!client.connected()) {
    Serial.println("NotConnected");
    Serial.println("disconnecting ubidots.");
    client.stop();
    for(;;);
  }

  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}





void setup() {

  Serial.begin(115200);

  pinMode( 5, OUTPUT); // D1 in nodemcu    FAN 1
  pinMode( 4, OUTPUT); // D2 in nodemcu    LIGHT 1
  pinMode( 0, OUTPUT); // D3 in nodemcu    FAN 2 
  pinMode( 16, OUTPUT); // D0 in nodemcu   LIGHT 2
  pinMode( 2,INPUT);  //  D4 FEED BACK
  pinMode( 14,INPUT);  //  D5 FEED BACK
  pinMode( 12,INPUT);  // D6 FEED BACK
  pinMode( 13,INPUT);  // D7 FEED BACK

  digitalWrite( 5, LOW); 
  digitalWrite( 4, LOW); 
  digitalWrite( 0, LOW); 
  digitalWrite( 16, LOW); // Connect to WiFi network 
  /*
  ------------------------------------------------------------------------------
        PIN WITH CORRESPONDING FEED BACK
  ------------------------------------------------------------------------------
  DEVICE        OUTPUT PIN        FEED BACK PIN 

  FAN 1       D1(5)             D4(2)                
  LIGHT 1     D2(4)             D5(14)
  FAN 2       D3(0)             D6(12)
  LIGHT 2     D0(16)            D7(13)
  */

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  server.begin();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

}


void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available(); 

  if (! client) { return; }
  // Wait until the client sends some data
  Serial.println(" new client"); 

  while(! client.available())
  { delay( 1); } 
  //ubiSave_value(String("1"),te,token1);
  // Read the first line of the request 

  String request = client.readStringUntil('\ r');
  Serial.println( request);
  client.flush(); // Match the request
  //Serial.print("light 1 off");
  if (request.indexOf("fan1") > 0)
  {

    Serial.println( "\n\n\n ");
    Serial.println(i);
    Serial.println(" \n\n\n");
    if(i==0) {
      i=1;
    } else {
      i=0;
    }

    if(i!=0) {
      digitalWrite( 5, HIGH);
      Serial.println(" FAN 1 on");
    } else {
      digitalWrite( 5, LOW);
      Serial.println("FAN 1 off");
    }

  } 


  if (request.indexOf("light1") > 0)
  {
    Serial.println( "\n\n\n ");
    Serial.println(i);
    Serial.println(" \n\n\n");

    if(i1==0) {
      i1=1;
    } else {
      i1=0;
    }

    if(i1!=0) {
      digitalWrite( 4, HIGH);
      Serial.println(" LIGHT 1 on");
    } else {
      digitalWrite( 4, LOW);
      Serial.println("LIGHT 1 off");
    }

  } 

  if (request.indexOf("fan2") > 0)
  {
    Serial.println( "\n\n\n ");
    Serial.println(i);
    Serial.println(" \n\n\n");

    if(i2==0) {
      i2=1;
    } else {
      i2=0;
    }

    if(i2!=0) {
      digitalWrite( 0, HIGH);
      Serial.println(" FAN 2 on");
    } else {
      digitalWrite( 0, LOW);
      Serial.println("FAN 2 off");
    }

  } 



  if (request.indexOf("light2") > 0)
  {
    Serial.println( "\n\n\n ");
    Serial.println(i3);
    Serial.println(" \n\n\n");
    if(i3==0) {
      i3=1;
    } else {
      i3=0;
    }

    if(i3!=0) {
      digitalWrite( 16, HIGH);
      Serial.println(" LIGHT 2 on");
    } else {
      digitalWrite( 16, LOW);
      Serial.println("LIGHT 2 off");
    }
  } 

  if(request.indexOf("feed_back") <= 0)
  {
    client.println(" HTTP/ 1.1 200 OK");
    client.println(" Content-Type: text/ html");
    client.println(""); // do not forget this one 
    client.println(" <!DOCTYPE HTML>"); 
    client.println(" <html>");
    client.println(" <head>");
    client.println(" </head>"); 
    client.println(" <h4> <center> remote control  </center> </h4>");
    client.println(" <br> <br>");
    client.println(" <br> <br>");
    client.println(" <center>");
    client.println("STL FAN"); 
    
    if (digitalRead( 5)) {
      client.println(" <a href =\"/ fan1\"\"><button style =\"background-color:green\">STL FAN  </button> </a>");
    }
    else {
      client.println(" <a href =\"/ fan1\"\"><button style =\"background-color:red\">STL FAN  </button> </a> <br/>");
    }

    client.println(" </center>");
    client.println(" <br> <br>");
    client.println(" <center>");
    client.println("STL LIGHT");

    if (digitalRead( 4)) {
      client.println(" <a href =\"/ light1\"\"><button style =\"background-color:green\">STL LIGHT  </button> </a>");
    }
    else {
      client.println(" <a href =\"/ light1\"\"><button style =\"background-color:red\">STL LIGHT </button> </a> <br/>");
    }

    client.println(" </center>"); 
    client.println(" <br> <br>"); 
    client.println(" <center>");
    client.println("VAN FAN");



    if (digitalRead( 0)) {
      client.println(" <a href =\"/ fan2\"\"><button style =\"background-color:green\">VAN FAN  </button> </a>");
    }
    else {
      client.println(" <a href =\"/ fan2\"\"><button style =\"background-color:red\">VAN FAN  </button> </a> <br/>");
    }

    client.println(" </center>");
    client.println(" <br> <br>");
    client.println(" <center>");
    client.println("VAN LIGHT"); 


    if (digitalRead( 16)) {
      client.println(" <a href =\"/ light2\"\"><button style =\"background-color:green\">VAN LIGHT  </button> </a>");
    }
    else {
      client.println(" <a href =\"/ light2\"\"><button style =\"background-color:red\">VAN LIGHT  </button> </a> <br/>");
    }

    client.println(" </center>");
    client.println(" <br> <br>");
    client.println(" <center>");
    client.println(" <a href =\"/ feed_back\"\"><button style =\"background-color:orange\">   CUTTRNT STATUS   </button> </a> <br/>");
    client.println(" </center>");
    client.println(" <br> <br>");
    client.println(" <center>");
    client.println(" current remote status ");
    client.println(" <table border =\" 5\">");
    client.println(" <tr>"); 

    if (digitalRead( 5))// read digital pin 5 
      {client.print(" <td> FAN 1 is ON </td>"); } 
    else
      { client.print(" <td> FAN 1 is OFF </td>"); }



    if (digitalRead( 4))// read digital pin 4
      { client.print(" <td> LIGHT 1 is ON </td>"); }
    else 
      { client.print(" <td> LIGHT 1 is OFF </td>"); } 

    client.println(" </tr>"); 
    client.println(" <tr>"); 

    if (digitalRead( 0))// read digital pin 0 
      { client.print(" <td> FAN 2 is ON </td>"); }
    else 
      { client.print(" <td> FAN 2 is OFF </td>"); } 

    if (digitalRead( 16))// read digital pin 13
      { client.print(" <td> LIGHT 2 is ON </td>"); } 
    else 
      { client.print(" <td> LIGHT 2 is OFF </td>"); } 

    client.println(" </tr>");
    client.println(" </table>");
    client.println(" </center>");
    client.println(" </html>");




    /*

    if (digitalRead( 2)>0)// read digital pin 2 
    {
    ubiSave_value("1",fan1,token1);
    }
    else
    {

    ubiSave_value("0",fan1,token1);

    }

    if (digitalRead( 14)>0)// read digital pin 2 
    {

    ubiSave_value("1",light1,token1);

    }
    else
    {

    ubiSave_value("0",light1,token1);
    }
    if (digitalRead( 12)>0)// read digital pin 2 
    {

    ubiSave_value("1",fan2,token1);
    }
    else
    {

    ubiSave_value("0",fan2,token1);
    }
    if (digitalRead( 13)>0)// read digital pin 2 
    {

    ubiSave_value("1",light2,token1);
    }
    else
    {

    ubiSave_value("0",light1,token1);
    }

    */

    delay( 1);
    Serial.println(" Client disonnected"); 
    Serial.println(""); 

  }
  else
  {
    client.println(" HTTP/ 1.1 200 OK");
    client.println(" Content-Type: text/ html");
    client.println(""); // do not forget this one 
    client.println(" <!DOCTYPE HTML>"); 
    client.println(" <html>");
    client.println(" <head>");
    client.println("<center>");
    client.println("CURRENTT STATUS OF THE DEVICE ");
    client.println("</center>");
    client.println(" <br><br> ");
    client.println("<center>");
    client.println(" current remote status ");
    client.println(" <table border =\" 5\">");
    client.println(" <tr>"); 





    if (digitalRead( 2)>0)// read digital pin 2 
    {client.print(" <td> FAN 1 is ON </td>"); } 
    else
    { client.print(" <td> FAN 1 is OFF </td>"); }



    if (digitalRead( 14)>0)// read digital pin 14
    { client.print(" <td> LIGHT 1 is ON </td>"); }
    else 
    { client.print(" <td> LIGHT 1 is OFF </td>"); } 

    client.println(" </tr>"); 
    client.println(" <tr>"); 

    if (digitalRead( 12)>0)// read digital pin 0 
    { client.print(" <td> FAN 2 is ON </td>"); }
    else 
    { client.print(" <td> FAN 2 is OFF </td>"); } 

    if (digitalRead( 13)>0)// read digital pin 13
    { client.print(" <td> LIGHT 2 is ON </td>"); } 
    else 
    { client.print(" <td> LIGHT 2 is OFF </td>"); } 
    client.println(" </tr>");
    client.println(" </table>");
    client.println(" </center>");
    client.println(" <center>");
    client.println(" <a href =\"/ hi\"\"><button style =\"background-color:red\">  back   </button> </a> <br/>");
    client.println(" </center>");
    client.println(" </head>");  
    client.println(" </html>");

    /*
    if (digitalRead( 2)>0)// read digital pin 2 
    {
    ubiSave_value("1",fan1,token1);
    }
    else
    {

    ubiSave_value("0",fan1,token1);

    }

    if (digitalRead( 14)>0)// read digital pin 2 
    {

    ubiSave_value("1",light1,token1);

    }
    else
    {

    ubiSave_value("0",light1,token1);
    }
    if (digitalRead( 12)>0)// read digital pin 2 
    {

    ubiSave_value("1",fan2,token1);
    }
    else
    {

    ubiSave_value("0",fan2,token1);
    }
    if (digitalRead( 13)>0)// read digital pin 2 
    {

    ubiSave_value("1",light2,token1);
    }
    else
    {

    ubiSave_value("0",light1,token1);
    }
    */


    delay( 1);
    Serial.println(" Client disonnected"); 
    Serial.println(""); 
  }
} // code ends here




























