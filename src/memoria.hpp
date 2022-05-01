//////////////////////////////////////////////////////////////////////
// si hay cambios para subir a firebase
boolean f_si_firebase()
{
  byte cambios = 0;
  Serial.print("\n ---------------------Funcion: f_si_firebase()");
  for (u_int8_t t = 0; t < 5; t++) // verifica si cambiaron
  {
    if (f_most_Jug_PGN[t] != f_most_Jug[t])
      cambios++;
    for (u_int8_t l = 0; l < 6; l++)
    {
      if (mi_n_salido_PGN[l + t * 6].numero != mi_n_salido[l + t * 6].numero)
        cambios++;
      if (mi_n_salido_PGN[l + t * 6].f_mostrar != mi_n_salido[l + t * 6].f_mostrar)
        cambios++;
    }
  }
  for (u_int8_t t = 0; t < 5; t++) // carga en el temporal para la proxima
  {
    f_most_Jug_PGN[t] = f_most_Jug[t];
    for (u_int8_t l = 0; l < 6; l++)
    {
      mi_n_salido_PGN[l + t * 6].numero = mi_n_salido[l + t * 6].numero;
      mi_n_salido_PGN[l + t * 6].f_mostrar = mi_n_salido[l + t * 6].f_mostrar;
    }
  }
  return cambios>0;
}
//////////////////////////////////////////////////////////////////////
// Imprime el contenido la memoria
void imprimeEprom()
{
  Serial.println(".");
  Serial.print("\n ---------------------Funcion: imprimeEprom()");
  Serial.println(".");
  Serial.print("--sonido: ");
  Serial.print(f_sonido ? "ON" : "OFF");
  Serial.print("\t--modo_offline: ");
  Serial.print(modo_offline ? "ON" : "OFF");
  Serial.print("\t--tiempo_jugada: ");
  Serial.print(tiempo_jugada);
  Serial.print("\t--brillo_textos: ");
  Serial.print(brillo_textos);
  Serial.print("\t--vel_texto: ");
  Serial.print(vel_texto);
  Serial.println(".");

  for (u_int8_t t = 0; t < 5; t++)
  {
    Serial.print(n_Jugada[t]);
    Serial.print(" = ");
    Serial.print(f_most_Jug[t]);
    Serial.print("\t");
    for (u_int8_t l = 0; l < 6; l++)
    {
      Serial.print(mi_n_salido[l + t * 6].numero);
      Serial.print(" : ");
      Serial.print(mi_n_salido[l + t * 6].f_mostrar);
      Serial.print(" - \t");
    }
    Serial.println(".");
  }
}

//////////////////////////////////////////////////////////////////////
// Lee los numeros en firebase
boolean leeFirebase()
{
  uint8_t errores = 0;
  Serial.print("\n ---------------------Funcion: leeFirebase()");
  actualizacion_FB = Firebase.RTDB.getDouble(&obj_fbdo_FB, path + "Actualización FB/") ? obj_fbdo_FB.doubleData() : errores++;
  subParpadeo();
  Serial.printf("\n actualizacion_FB: %i \t - actualizacion: %i", actualizacion_FB, actualizacion);
  for (u_int8_t t = 0; t < 5; t++)
  {
    String jug = path + (t + 1) + "-" + n_Jugada[t];
    Serial.printf("\n%s: %i", String(n_Jugada[t]).c_str(), Firebase.RTDB.getBool(&obj_fbdo_FB, jug + "/Mostrar", &f_most_Jug_FB[t]) ? f_most_Jug_FB[t] : errores++);
    subParpadeo();
    for (u_int8_t l = 0; l < 6; l++)
    {
      String lot = jug + "/" + (l + 1) + "-" + n_Loteria[l] + "/";
      Serial.printf("\tN- %i", Firebase.RTDB.getInt(&obj_fbdo_FB, lot + "Numero/", &mi_n_salido_FB[l + t * 6].numero) ? mi_n_salido_FB[l + t * 6].numero : errores++);
      subParpadeo();
      Serial.printf(" : %i", Firebase.RTDB.getBool(&obj_fbdo_FB, lot + "Mostrar/", &mi_n_salido_FB[l + t * 6].f_mostrar) ? mi_n_salido_FB[l + t * 6].f_mostrar : errores++);
      subParpadeo();
    }
  }
  Serial.printf("\nSuma de errores de lectura: %i.\n", errores);
  return (errores == 0);
}

//////////////////////////////////////////////////////////////////////// pasa los numeros temporales de firebase al cartel
void pasaFirebase()
{
  Serial.print("\n ---------------Funcion: pasaFirebase()");
  actualizacion = actualizacion_FB;
  for (u_int8_t t = 0; t < 5; t++)
  {
    f_most_Jug[t] = f_most_Jug_FB[t];

    for (u_int8_t l = 0; l < 6; l++)
    {
      mi_n_salido[l + t * 6].numero = mi_n_salido_FB[l + t * 6].numero;
      mi_n_salido[l + t * 6].f_mostrar = mi_n_salido_FB[l + t * 6].f_mostrar;
    }
  }
}

//////////////////////////////////////////////////////////////////////
// cambia la banera para leer en firebase
void flag_leer_Actualizacion()
{
  f_leer_actual_fire = true;
}

//////////////////////////////////////////////////////////////////////
// Verifica el numero de actualizacion FB
boolean ver_actual_FB()
{
  uint8_t errores = 0;
  Serial.print("\n ---------Funcion: ver_actual_FB()");
  subParpadeo();
  actualizacion_FB = Firebase.RTDB.getDouble(&obj_fbdo_FB, path + "Actualización FB/") ? obj_fbdo_FB.doubleData() : errores++;
  subParpadeo();
  Serial.printf("\n actualizacion_FB: %i \t - actualizacion: %i", actualizacion_FB, actualizacion);
  Serial.printf("\n Suma de errores de actualizacion: %i.", errores);
  return (errores == 0);
}

//////////////////////////////////////////////////////////////////////
// Graba verif leidos
void graba_veri_leidos()
{
  uint8_t errores = 0;
  Serial.printf("\n Leido por: %s= %i", AGENCIA.c_str(), Firebase.RTDB.setDoubleAsync(&obj_fbdo_FB, path + "Leido por " + AGENCIA, actualizacion) ? actualizacion : errores++);
  // Serial.printf("\nHora de leido: %s\n", Firebase.RTDB.setTimestampAsync(&obj_fbdo_FB, path + "Hora de leido: ") ? "ok" : obj_fbdo_FB.errorReason().c_str());
  // if (obj_fbdo_FB.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
  // {
  //   // In setTimestampAsync, the following timestamp will be 0 because the response payload was ignored for all async functions.
  //   // Timestamp saved in millisecond, get its seconds from int value
  //   Serial.print("TIMESTAMP (Seconds): ");
  //   time_t hora=obj_fbdo_FB.to<uint64_t>();
  //   Serial.println(time(&hora));
  // }
}
//////////////////////////////////////////////////////////////////////
// Graba los numeros en firebase
boolean graba_Firebase()
{
  uint8_t errores = 0;
  Serial.print("\n ------------Funcion: graba_Firebase()");
  actualizacion_FB = Firebase.RTDB.getDouble(&obj_fbdo_FB, path + "Actualización FB/") ? obj_fbdo_FB.doubleData() : 0; // lee indice actualizacion
  Serial.printf("\n actualizacion: %i", actualizacion_FB++);
  actualizacion = actualizacion_FB;
  Serial.printf("\t Actualizacion_FB: %i", Firebase.RTDB.setDoubleAsync(&obj_fbdo_FB, path + "Actualización FB/", actualizacion) ? actualizacion : errores++);
  Serial.printf("\t Actualizado por: %i", Firebase.RTDB.setStringAsync(&obj_fbdo_FB, path + "Actualizado/", AGENCIA) ? actualizacion : errores++);
  for (u_int8_t t = 0; t < 5; t++)
  {
    String jug = path + (t + 1) + "-" + n_Jugada[t];
    Serial.printf("\n%s: %i", String(n_Jugada[t]).c_str(), Firebase.RTDB.setBoolAsync(&obj_fbdo_FB, jug + "/Mostrar", f_most_Jug[t]) ? f_most_Jug[t] : errores++);
    subParpadeo();
    for (u_int8_t l = 0; l < 6; l++)
    {
      String lot = jug + "/" + (l + 1) + "-" + n_Loteria[l] + "/";
      Serial.printf("\tN- %i", Firebase.RTDB.setIntAsync(&obj_fbdo_FB, lot + "Numero/", mi_n_salido[l + t * 6].numero) ? mi_n_salido[l + t * 6].numero : errores++);
      subParpadeo();
      Serial.printf(" : %i", Firebase.RTDB.setBoolAsync(&obj_fbdo_FB, lot + "Mostrar/", mi_n_salido[l + t * 6].f_mostrar) ? mi_n_salido[l + t * 6].f_mostrar : errores++);
      subParpadeo();
    }
  }
  Serial.printf("\nSuma de errores de grabado: %i.\n", errores);
  return (errores == 0);
}

//////////////////////////////////////////////////////////////////////
// Graba la configuracion inicial en la memoria
void grabarEprom()
{
  Serial.print("Grabando memoria .");
  eeDir = 0;

  EEPROM.put(eeDir, f_sonido);
  eeDir += sizeof(f_sonido);
  Serial.print(".");
  delay(1);

  EEPROM.put(eeDir, modo_offline);
  eeDir += sizeof(modo_offline);
  Serial.print(".");
  delay(1);

  EEPROM.put(eeDir, tiempo_jugada);
  eeDir += sizeof(tiempo_jugada);
  Serial.print(".");
  delay(1);

  EEPROM.put(eeDir, vel_texto);
  eeDir += sizeof(vel_texto);
  Serial.print(".");
  delay(1);

  EEPROM.put(eeDir, brillo_textos);
  eeDir += sizeof(brillo_textos);
  Serial.print(".");
  delay(1);

  for (u_int8_t t = 0; t < 5; t++)
  {
    EEPROM.put(eeDir, f_most_Jug[t]);
    eeDir += sizeof(f_most_Jug[t]);
    Serial.print(".");
    delay(1);
  }
  for (u_int8_t t = 0; t < 30; t++)
  {
    EEPROM.put(eeDir, mi_n_salido);
    eeDir += sizeof(mi_n_salido);
    Serial.print(".");
    delay(1);
  }

  EEPROM.commit();
  Serial.print(".");
  Obj_Pantalla.setIntensity(brillo_textos);
  for (u_int8_t x = 0; x < CANT_MOD_DIG; x++)
    Obj_Digito.setIntensity(x, brillo_textos + 5); // Setea brillo (0-15)
  Serial.println("Memoria Grabada");
  imprimeEprom();
  if (f_si_firebase())
  {
    f_grabar_firebase = true;
    error_firebase = 1;
  }
}

//////////////////////////////////////////////////////////////////////
// Lee la configuracion inicial de la memoria
void leerEprom()
{
  Serial.print("Leyendo memoria .");
  eeDir = 0;

  EEPROM.get(eeDir, f_sonido);
  eeDir += sizeof(f_sonido);
  Serial.print(".");
  delay(1);

  EEPROM.get(eeDir, modo_offline);
  eeDir += sizeof(modo_offline);
  Serial.print(".");
  delay(1);

  EEPROM.get(eeDir, tiempo_jugada);
  eeDir += sizeof(tiempo_jugada);
  Serial.print(".");
  delay(1);

  EEPROM.get(eeDir, vel_texto);
  eeDir += sizeof(vel_texto);
  Serial.print(".");
  delay(1);

  EEPROM.get(eeDir, brillo_textos);
  eeDir += sizeof(brillo_textos);
  Serial.print(".");
  delay(1);

  for (u_int8_t t = 0; t < 5; t++)
  {
    EEPROM.get(eeDir, f_most_Jug[t]);
    eeDir += sizeof(f_most_Jug[t]);
    Serial.print(".");
    delay(1);
  }

  for (u_int8_t t = 0; t < 30; t++)
  {
    EEPROM.get(eeDir, mi_n_salido);
    Serial.print(".");
    delay(1);
    eeDir += sizeof(mi_n_salido);
    Serial.print(".");
    delay(1);
  }

  Serial.print(".");
  Serial.println("Memoria Leida");
  // f_sonido=false;////////////////////////////////////
}
