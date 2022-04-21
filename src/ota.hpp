void iniciar_OTA()
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname(ssidWM);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "archivo de programa...";
    }
    else
    { 
      type = "sistema de archivos...";
      LittleFS.end();
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Iniciando actualizacion de " + type); });

  ArduinoOTA.onEnd([]()
                   { Serial.println("\nFinalizado Correctamente"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progreso: %u%%\r", (progress / (total / 100))); });

  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Fallo Autenticacion");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Fallo inicio");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Fallo coneccion");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Fallo recepcion");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("Fallo finalizacion");
    } });

  ArduinoOTA.begin();
  Serial.println("");
  Serial.println("OTA iniciado");
}
