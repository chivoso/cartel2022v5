/////////////////////////////////////////////////////////
void ConectaFirebase()
{
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  obj_config_FB.api_key = API_KEY;
  /* Assign the user sign in credentials */
  obj_auth_FB.user.email = USER_EMAIL;
  obj_auth_FB.user.password = USER_PASSWORD;
  /* Assign the RTDB URL */
  obj_config_FB.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  obj_fbdo_FB.setResponseSize(4096);
  /* Assign the callback function for the long running token generation task */
  obj_config_FB.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  /** Assign the maximum retry of token generation */
  obj_config_FB.max_token_generation_retry = 5;
  /* Initialize the library with the Firebase authen and config */
  Firebase.begin(&obj_config_FB, &obj_auth_FB);
}

///////////////////////////////////////////////////////
void ConectaWiFi_Manager()
{
  Serial.println(".");
  Serial.println("Esperando coneccion Wifi Manager.");
  Serial.println(ssidWM);
  imp_Pant_Simple(ssidWM);

  //// para desactivar salida serial
  wifiManager.setDebugOutput(false);
  //// Descomentar para resetear configuración
  // wifiManager.resetSettings();
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn, dns1, dns2);
  // wifiManager.setConnectTimeout(30);

  ////tries to connect to last known settings
  ////if it does not connect it starts an access point with the specified name
  ////here  "AutoConnectAP" with password "password"
  ////and goes into a blocking loop awaiting configuration
  // WiFi.enableInsecureWEP(true); //para el WEP
  if (!wifiManager.autoConnect(ssidWM, passWM))
  {
    Serial.println("Falla de coneccion, reseteando y volviendo a probar...");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  ////if you get here you have connected to the WiFi
  Serial.println("Conectado :)");
  //mostrar_Ip();
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void ConectaWiFi_OFFline()
{
  Serial.println("Conectando con portal cautivo...");
  WiFi.softAP(ssidAP, passAP);
  obj_dns_server.start(53, "*", WiFi.softAPIP());
  // obj_web_server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); //only when requested from AP
  //// more handlers
  Serial.println("");
  Serial.print("Iniciado AP:\t");
  Serial.println(ssidAP);
  Serial.print("Direccion IP:\t");
  Serial.println(WiFi.softAPIP());
}

////////////////////////////////////////////////////////////////////////////////////////
void ConectaWiFi_AP(boolean useStaticIP = false)
{
  Serial.println("");
  WiFi.mode(WIFI_AP);
  Serial.println("Iniciando coneccion.");
  // WiFi.softAPConfig(ipAP, gatewayAP, subnetAP);
  while (!WiFi.softAP(ssidAP, passAP))
  {
    Serial.println(".");
    delay(100);
  }

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  // obj_dns_server.start(DNS_PORT, "*", ipAP);

  Serial.println("");
  Serial.print("Iniciado AP:\t");
  Serial.println(ssidAP);
  Serial.print("Direccion IP:\t");
  Serial.println(WiFi.softAPIP());
  WiFi.localIP() = WiFi.softAPIP();
  Serial.print("Direccion IP:\t");
  Serial.println(WiFi.localIP());
}

//////////////////////////////////////////////////////////////////////////////////////
void ConectaWiFi_STA(boolean useStaticIP = true)
{
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidSTA, passSTA);
  if (useStaticIP)
    WiFi.config(_ip, _gw, _sn);
  Serial.println("Iniciando coneccion.");
  int ciclos = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    ciclos++;
    delay(100);
    Serial.print('.');
    if (ciclos > 100)
    {
      ConectaWiFi_AP();
    }
  }
  Serial.println("");
  Serial.print("Iniciado STA:\t");
  Serial.println(ssidSTA);
  Serial.print("Direccion IP:\t");
  Serial.println(WiFi.localIP());
  Serial.println("Conectado :)");
}
