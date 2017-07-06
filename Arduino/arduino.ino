#include "Adafruit_Sensor.h"
#include "DHT.h"
#define DHTPIN 7     // pin donde se encuentra conectado el DHT11
//defino el tipo de sensor ya que la libreria sirve para varios sensores DHT
#define DHTTYPE DHT11
#include <SoftwareSerial.h>
 // arduino Rx (pin 2) ---- serie Tx
 // arduino Tx (pin 3) ---- serie Rx
SoftwareSerial serie(3,2); 
//creo el objeto dht
DHT dht(DHTPIN, DHTTYPE); 


int speakerPin = 9;

int length = 15; // el numero de las notas
char notes[] = "eeecegcgfabaagegafgecdb "; 
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;


// La salida digital del sensor debe ir conectada al pin 5


int LecturaSensor=0; // variable para almacenar el valor del sensor
int Bloquear =0; // Variable que permite bloquear el estado actual del pin de salida
int pinLed=13;//pin para led 
int pinRelay=4;// pin led a cambiar por relay

#define UMBRAL       80           // poner el valor que se desee ahora fijado en 80%
#define RETARDO      1000         // 1000 mseg = 1 seg
const byte pinBigSound=A1;    // pin analogo para el sensor de sonido
int valorSonido=0;  // valor leido por el sensor de sonido

const byte pinCorazon=A2;


String red="SOa-IoT-N750";
String pass="ioteshumo";

/*
String red="Rock";
String pass="callejeros";
*/
/*String red="mutualuci";
String pass="Cortadores1275";
*/
void setup()
{
 Serial.begin(9600);  // monitor serial del arduino
 serie.begin(9600); // baud rate del ESP8266
 dht.begin();
 pinMode(13,OUTPUT);
 pinMode(pinRelay,OUTPUT);
 digitalWrite(13,LOW);
 pinMode(speakerPin, OUTPUT);
 String con="AT+CWJAP=\"";
 con+=red;
 con+="\",\"";
 con+=pass;
 con+="\"\r\n";

 Serial.println(con);
 sendData("AT+CWMODE=3\r\n",1000); // configurar para multiples conexiones
 sendData(con,4000);
 sendData("AT+CIPMUX=1\r\n",1000); // configurar para multiples conexiones
 sendData("AT+CIPSERVER=1,80\r\n",1000);         // servidor en el puerto 80
 sendData("AT+CIFSR\r\n",2000);    // obtener dirección IP
}
void loop()
{
 float t = dht.readTemperature(); //leo temperatura
 float h = dht.readHumidity();  //leo humedad
 valorSonido = map(analogRead(pinBigSound), 0, 1023, 0, 100);
 int valorPulso=analogRead(pinCorazon)/10;
// Serial.println(valorPulso);
  
 if(serie.available())   // revisar si hay mensaje del serie
 {
   if(serie.find("+IPD,")) // revisar si el servidor recibio datos
   {
       delay(500); // esperar que lleguen los datos hacia el buffer
       int conexionID = serie.read()-48; // obtener el ID de la conexión para poder responder
       serie.find("comando="); // bucar el texto "comando="
       int cmd = (serie.read()-48); // Obtener el estado del pin a mostrar
          
       //PRIMER ACTUADOR ENCENDIDO DE LED   
       if(cmd==1 || cmd==0) // si el comando es prender la luz
         digitalWrite(13, cmd); // Cambiar estado del pin
       while(serie.available()){
           char c = serie.read();
           Serial.print(c);
       }
       
       ///SEGUNDO LED QUE SERA UN RELAY  SEGUNDO ACTUADOR
       if(cmd==2){ //enciendo segundo led o relay
         digitalWrite(4,LOW);
       }
       if(cmd==3){ //enciendo segundo led o relay
         digitalWrite(4,HIGH);
       }
       ///TERCER ACTUADOR MUSICA
       if(cmd==5){/// enciendo musica
            for (int i = 0; i < length; i++) {
                if (notes[i] == ' ') {
                    delay(beats[i] * tempo); 
                  } else {
                    playNote(notes[i], beats[i] * tempo);
                  }
              
                  // pausa entre notas
                  delay(tempo / 2); 
                }    
            }
            
      
     //MANDO DE RESPUESTA A CLIENTE MEDICIONES DE SENSORES
       
 //responder y cerrar la conexión      
            
            if(cmd==4){ 
              
            // String webpage = "HTTP/1.1 200 OK\r\n";
             String webpage ="{\"temp\":\"";
            // webpage+="Access-Control-Allow-Origin: *\r\n";
             //webpage+="Content-Type: text/plain\r\n";
             //webpage+="Content-Result:";
             webpage+=t;
             webpage+="\",\"humedad\":\"";
             webpage+=h;
             webpage+="\",\"sonido\":\"";
             webpage+=valorSonido;
             webpage+="\",\"pulso\":\"";
             webpage+=valorPulso;
             webpage+="\"}";
             webpage+="\r\n";
             /*webpage+="Hola\r\n";
             webpage+=t;
             //webpage+="\"}";
             webpage+=h;
             webpage+="</h1>";
             if (cmd==1) webpage += "<h2>LED_13 = encendido</h2>";
             else { webpage += "<h2>LED_13 = apagado!</h2>";}
      */ 
       // comando para enviar a cliente
             String comandoWebpage = "AT+CIPSEND=";
             comandoWebpage+=conexionID;
             comandoWebpage+=",";
             comandoWebpage+=webpage.length();
             comandoWebpage+="\r\n";
             sendData(comandoWebpage,1000);
             sendData(webpage,1000);
       
       // comando para terminar conexión
             String comandoCerrar = "AT+CIPCLOSE=";
             comandoCerrar+=conexionID;
             comandoCerrar+="\r\n";
             sendData(comandoCerrar,3000);
             
           }
      }
      
  }
 
    
}
/*
Enviar comando al serie y verificar la respuesta del módulo, todo esto dentro del tiempo timeout
*/
void sendData(String comando, const int timeout)
{
     long int time = millis(); // medir el tiempo actual para verificar timeout
     
     serie.print(comando); // enviar el comando al serie
     
     while( (time+timeout) > millis()) //mientras no haya timeout
     {
       while(serie.available()) //mientras haya datos por leer
       { 
       // Leer los datos disponibles
           char c = serie.read(); // leer el siguiente caracter
           Serial.print(c);
       }
     } 
 return;
}




void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // toca el tono correspondiente al nombre de la nota
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

