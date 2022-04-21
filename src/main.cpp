
// Carga de librerias

#include <arduino.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>       //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncTCP.h>
#include <LittleFS.h>
#include <Ticker.h>              //https://github.com/esp8266/Arduino/tree/master/libraries/Ticker
#include <EEPROM.h>              //https://github.com/esp8266/Arduino/tree/master/libraries/EEPROM
#include <ESP8266mDNS.h>         //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266mDNS
#include <ArduinoOTA.h>          //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
#include <WiFiUdp.h>             //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiUdp.cpp
#include <ESPAsyncWiFiManager.h> //https://github.com/tzapu/WiFiManager

#include <LedControl.h>        // http://wayoda.github.io/LedControl/
#include <MD_Parola.h>         // https://github.com/MajicDesigns/MD_Parola
#include <MD_MAX72XX.h>        //https://github.com/MajicDesigns/MD_MAX72XX
#include "Parola_Fonts_data.h" //tipografia Bold[]
#include <MD_UISwitch.h>
#include "sprites.h"

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#include "config.h" // variables y conf generales

//// Define objetos ticker
Ticker tck_parpadeo;
Ticker tck_cambio_jugada;
Ticker tck_anim_digitos;
Ticker tck_ver_botones;
Ticker tck_ver_firebase;

//// Crea objeto parola
MD_Parola Obj_Pantalla = MD_Parola(TIPO_PANT, PIN_DATA_PANT, PIN_CLK_PANT, PIN_CS_PANT, CANT_MOD_PANT);
////Setea la fuente

//// crea objeto ledcontrol
LedControl Obj_Digito = LedControl(PIN_DATA_DIG, PIN_CLK_DIG, PIN_CS_DIG, CANT_MOD_DIG);

////Define y crea teclado
MD_UISwitch_Digital Obj_Botones(DIGITAL_SWITCH_PINS, ARRAY_SIZE(DIGITAL_SWITCH_PINS), DIGITAL_SWITCH_ACTIVE);

////Define y crea objetos Firebase
FirebaseData obj_fbdo_FB;
FirebaseAuth obj_auth_FB;
FirebaseConfig obj_config_FB;

////Define objeto DNS server web
DNSServer obj_dns_server;
AsyncWebServer obj_web_server(80);

//// Creamos una instancia de la clase WiFiManager
AsyncWiFiManager wifiManager(&obj_web_server, &obj_dns_server);

#include "sonido.hpp"   //sub de sonidos y melodias
#include "imprimir.hpp" // sub manejo impresion pantalla y digitos
#include "tickers.hpp"  //subrutinas ticker
#include "memoria.hpp"  // subs para manejo de memoria
#include "conectar.hpp" //rutinas con modo de coneccion wifi
#include "servidor.hpp" // subs para manejo del servidor

#include "ota.hpp"      //rutinas OTA
#include "botones.hpp"  //manejo de botones

///////////////////////////////////////////////////////////////
////////// setup - setup - setup  /////////////////////////////
///////////////////////////////////////////////////////////////
void setup()
{
  pinMode(PIN_LED, OUTPUT);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);

  Serial.begin(115200);

  EEPROM.begin(512);
  // grabarEprom();      // solo para cargar la memoria por 1era vez
  leerEprom();

  // Inicializa SPIFFS
  if (!LittleFS.begin())
  {
    Serial.println("Un ERROR ha ocurrido al montar el SPIFFS");
    return;
  }

  inicia_Pantalla();
  inicia_Digitos();
  inicia_Teclado();

  //// Coneccion Wi-Fi
  mi_tono(400, 200);
  delay(1000);
  mi_tono(400, 200);
  delay(1000);
  mi_tono(400, 200);
  delay(1000);
  ////Cambia modo en el arranque segun teclas

  if (!digitalRead(D1))
  {
    delay(500);
    if (!digitalRead(D1))
    {
      mi_tono(400, 200);
      modo_offline = true;
      grabarEprom();
      delay(1000);
    }
  }
  if (!digitalRead(D2))
  {
    delay(500);
    if (!digitalRead(D2))
    {
      mi_tono(400, 200);
      modo_offline = false;
      wifiManager.resetSettings();
      grabarEprom();
    }
  }
  if (modo_offline == true)
    ConectaWiFi_OFFline();
  else
  {
    ConectaWiFi_Manager();
    //////////////////////////////////////////////
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    obj_config_FB.api_key = API_KEY;
    obj_auth_FB.user.email = USER_EMAIL;
    obj_auth_FB.user.password = USER_PASSWORD;
    obj_config_FB.database_url = DATABASE_URL;
    Firebase.reconnectWiFi(true);
    obj_fbdo_FB.setResponseSize(4096);
    obj_config_FB.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    obj_config_FB.max_token_generation_retry = 5;
    Firebase.begin(&obj_config_FB, &obj_auth_FB);
  }

  //// Inicializa OTA
  iniciar_OTA();

  //// carga las....
  respuestas_Servidor();

  //// Arranca el server
  obj_web_server.begin();
  Serial.println("Servidor iniciado.");

  // imp_Pant_Efecto("  ...Arranque OK", 0, 1000);
  mostrar_Ip();
  Serial.println("  ...Arranque OK");

  ////Ticker cambioJugada
  tck_cambio_jugada.attach_ms(tiempo_jugada, avanza_Jugada);
  ////parpadeo led
  tck_parpadeo.attach_ms(parpadeo_led, subParpadeo);
  //// refresco animacion digitos
  tck_anim_digitos.attach_ms(REFRESH_DIG, subAnimacionDig);
  //// refresco botones
  tck_ver_botones.attach_ms(VERIF_PULSA, botonesPulsados);
  //// refresco firebase
  tck_ver_firebase.attach_ms(VERIF_BD, flag_leer_Actualizacion);
}

///////////////////////////////////////////////////////////////
///////////////////  LOOP LOOP LOOP   /////////////////////////
///////////////////////////////////////////////////////////////
void loop()
{
  ArduinoOTA.handle(); // Permite la actualizacion OTA

  //////////////////////////////////////////////////////
  if (modo_offline == true)
    obj_dns_server.processNextRequest(); // portal cautivo

  /////////////////////////////////////////////////////////////
  //// Firebase
  ////////////////////falta verificar si hay muchos errores pausar el firebase//////////////////////
  if (f_leer_actual_fire) // si hay que verificar actualización
  {
    f_leer_actual_fire = false;
    if (Firebase.ready()) // si firebase responde
    {
      if (ver_actual_FB()) // si se puede leer el numero de actualizacion
      {
        tck_ver_firebase.attach_ms(VERIF_BD, flag_leer_Actualizacion); // tiempo normal de verif
        if (actualizacion < actualizacion_FB)
        {
          if (leeFirebase()) // si la lectura es correcta
          {
            pasaFirebase(); // pasa lo leido al cartel
            graba_veri_leidos();
            error_firebase = 1;
            Serial.printf("\n ----Errores= %i", error_firebase);
            tck_ver_firebase.attach_ms(VERIF_BD * error_firebase, flag_leer_Actualizacion); // tiempo normal de verif
          }
          else
          {
            Serial.printf("\n ----Errores= %i", error_firebase++);
            tck_ver_firebase.attach_ms(VERIF_BD * error_firebase, flag_leer_Actualizacion); // tiempo de verif si error
          }
        }
      }
      else // si NO se puede leer el numero de actualizacion
      {
        Serial.printf("\n ----Errores= %i", error_firebase++);
        tck_ver_firebase.attach_ms(VERIF_BD * error_firebase, flag_leer_Actualizacion); // tiempo de verif si error
      }
    }
  }
  if (f_grabar_firebase) // si hay que grabar
  {
    if (Firebase.ready())                                                           // si firebase responde
    {
      if (graba_Firebase()) // si la grabacion es correcta
      {
        f_grabar_firebase = false;
        error_firebase = 1;
        tck_ver_firebase.attach_ms(VERIF_BD * error_firebase, flag_leer_Actualizacion); // tiempo de verif si error
        Serial.printf("\n ----Errores= %i", error_firebase);
      }
      else
      {
        Serial.printf("\n ----Errores= %i", error_firebase++);
      }
    }
  }

  /////////////////////////////////////////////////////////////

  if (f_anima_pant)
  {
    if (Obj_Pantalla.displayAnimate())
    {
      if (f_cambiar_jugada)
      {
        f_cambia_Dig = true;
        f_cambiar_jugada = false;
        // if (random(10) == 1)
        //   Obj_Pantalla.setInvert(true);
      }
      else // f_cambiar_jugada
      {
        if (!f_cambia_Dig)
          Obj_Pantalla.setInvert(false);

        cambia_Texto();

      } // f_cambiar_jugada

    } // Obj_Pantalla
  }   // f_anima_pant
} // loop
