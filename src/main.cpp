/*
 *  Project 22-ntp - main.cpp
 *      Se conecta con el servidor de hora
 *      y muestra dia de la semana y hora completa
 *      para la zona horaria que se declara.
 * 
 *      Además, a través de la biblioteca <time.h> de Unix
 *      se puede entregar la fecha completa en el formato
 *      deseado a través de 'strftime' basado en 'epoch time'
 *
 *      NOTA IMPORTANTE:
 *          Se pueden cambiar definiciones a traves de platformio.ini
 */

/*
 *  Definiciones incluidas en platformio.ini
 * 
 *      MY_SSID     Nombre de la red WiFi
 *      MY_PASS     Password de la red
 *      TIME_ZONE   Zona de tiempo donde reside
 *      NTP_SERVER  Nombre del servidor de NTP
 *      SERIAL_BAUD Baud rate del port serie
 */

/*
 *  Includes
 */

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>        // para acceder al manejo de WiFi para ESP8266
#endif

#include <NTPClient.h>          // para acceder a los servidores de tiempo
#include <WiFiUdp.h>            // para permitir transaccionar mediante UDP por WiFi
#include <time.h>               // para acceder a rutinas de conversion y presentacion de tiempo y fecha


#define TZ(tz)      (tz*3600)               //  Macro para convertir de zona a segundos de diferencia

//  Nombre de los dias de la semana
const char *daysOfTheWeek[] =
{
    "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"
};

/*
 *  Creacion de objetos
 */

WiFiUDP ntpUDP;                                                 //  Objeto para transaccionar en UDP
NTPClient timeClient(ntpUDP, NTP_SERVER, TZ(TIME_ZONE) );       //  Creacion del objeto de tipo NTPClient

void
setup(void)
{
    Serial.begin(SERIAL_BAUD);              //  Habiita el uso de la impresion serie

    WiFi.begin(MY_SSID, MY_PASS);           //  Conexiona WiFi: se le pasa ssid y clave
    while (WiFi.status() != WL_CONNECTED)   //  Se espera conexion a WiFi
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("______________");
    Serial.print( "Baud Rate = " );
    Serial.println( SERIAL_BAUD );
    Serial.print( "Conectado a ");
    Serial.println( MY_SSID );
    Serial.print( "MAC = ");
    Serial.println( WiFi.macAddress() );
    Serial.print( "local IP = " );
    Serial.println( WiFi.localIP() );
    Serial.println("______________");

    timeClient.begin();                     //  Inicializar el codigo del cliente
}

void
loop(void)
{
    time_t epoch_time;
    char buf[80];

    timeClient.update();                    //  Pedir nueva fecha y hora al servidor NTP

    Serial.print( daysOfTheWeek[timeClient.getDay()] );
    Serial.print( ": ");
    Serial.println(timeClient.getFormattedTime());

    Serial.print( "Epoch = " );
    epoch_time = timeClient.getEpochTime();
    Serial.println( epoch_time );
    strftime(buf, sizeof(buf),"%A: %d %B %Y %H:%M:%S", localtime(&epoch_time));
    Serial.println(buf); 

    Serial.println();

    delay(1000);
}
