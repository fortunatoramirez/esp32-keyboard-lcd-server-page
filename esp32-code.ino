#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <SocketIoClient.h>


/******************/
const char*     ssid      = "MYNETNAME_2.4"; 
const char*     password  = "*******";
const char*     server    = "172.168.1.127"; //Enter server adress
const uint16_t  port      = 5001; // Enter server port
uint64_t        _millis   = 0;
uint64_t        now       = 0;
/******************/

#define ROWS  4
#define COLS  4
#define ONBOARD_LED  2

char keyMap[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

char key;

uint8_t rowPins[ROWS] = {13, 12, 14, 27}; // GIOP13, GIOP12, GIOP14, GIOP27
uint8_t colPins[COLS] = {26, 25, 33, 32}; // GIOP26, GIOP25, GIOP33, GIOP32
uint8_t LCD_CursorPosition = 0;

Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C I2C_LCD(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
/******************/
SocketIoClient socketIO;
String    mensaje;
/******************/

void setup(){
  Serial.begin(115200);
  // Initialize The I2C LCD
  I2C_LCD.begin();
  // Turn ON The Backlight
  I2C_LCD.backlight();
  // Clear The Display
  I2C_LCD.clear();

  /******************/
  connectWiFi_STA();
  socketIO.begin(server, port);
  socketIO.on("DESDE_SERVER_COMANDO",procesar_comando_recibido);
  //socketIO.on("DESDE_SERVER_SENAL",procesar_senal_recibida);
  pinMode(ONBOARD_LED,OUTPUT);
  /******************/
}
 
void loop(){
  now = millis();
  key = keypad.getKey();
  
  if (key) {
    Serial.print(key);
    I2C_LCD.setCursor(LCD_CursorPosition++, 0);
    if(LCD_CursorPosition > 16 || key == 'C')
    {
      I2C_LCD.clear();
      LCD_CursorPosition = 0;
    }
    else
    {
      I2C_LCD.print(key);
      /******************/
      mensaje = "\""+String(key)+"\"";
      socketIO.emit("DESDE_ESP32_TECLA",mensaje.c_str());
      /******************/
    }
  }

  if(now%500==0)
  {
    mensaje = "\""+String(analogRead(34))+"\"";
    socketIO.emit("DESDE_ESP32_SENAL",mensaje.c_str());
  }
  socketIO.loop();
}


void connectWiFi_STA()
{
   delay(10);
   Serial.println("");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     delay(100);  
     Serial.print('.'); 
   }
   Serial.println("");
   Serial.print("Iniciado STA:\t");
   Serial.println(ssid);
   Serial.print("IP address:\t");
   Serial.println(WiFi.localIP());
}

void procesar_comando_recibido(const char * payload, size_t length) {
  Serial.printf("Mensaje recibido: %s\n", payload);
  String paystring = String(payload);

  I2C_LCD.setCursor(0, 1);
  I2C_LCD.print("                ");
  I2C_LCD.setCursor(0, 1);
  I2C_LCD.print(paystring);

  if(paystring == "ON")
  {
    digitalWrite(ONBOARD_LED,HIGH);
  }
  else if(paystring == "OFF")
  {
    digitalWrite(ONBOARD_LED,LOW);
  }
}

/*
void procesar_senal_recibida(const char * payload, size_t length) {
  Serial.printf("Senal recibida: %s\n", payload);
  String paystring = String(payload);
  I2C_LCD.setCursor(0, 1);
  I2C_LCD.print("    ");
  I2C_LCD.setCursor(0, 1);
  I2C_LCD.print(paystring);
}
*/


