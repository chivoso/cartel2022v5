/////////////////////////////////////////
//// inicia_Teclado
void inicia_Teclado()
{
  Obj_Botones.begin();
  Obj_Botones.setPressTime(100);
  Obj_Botones.setDoublePressTime(300);
  Obj_Botones.setLongPressTime(500);
  Obj_Botones.setRepeatTime(1500);
  Obj_Botones.enableDoublePress(false);
  Obj_Botones.enableLongPress(false);
  Obj_Botones.enableRepeat(true);
  Obj_Botones.enableRepeatResult(true);
}

/////////////////////////////////////////
////  segun tecla pulsada
void funcion_Teclado()
{
  if (nomAccTecla[accTecla] == "PRESS") // Pulsacion normal
  {
    switch (tecla)
    {
    case 4: // Pulsa tecla
      avanza_Jugada();
      break;
    case 5: // Pulsa tecla
      mostrar_Ip();
      break;
    }
  }
  else if (nomAccTecla[accTecla] == "REPEAT") // Pulsaión larga
  {
    switch (tecla)
    {
    case 4: // Pulsa tecla
      f_sonido = !f_sonido;
      Serial.print("***SONIDO*: ");
      Serial.println(f_sonido ? "Encendido" : "Apagado");
      imp_Pant_Efecto("  * Sonido " + String(f_sonido ? "Encendido" : "Apagado") + " * ");
      break;
    case 5: // Pulsa tecla
      modo_offline = !modo_offline;
      if (modo_offline)
      {
        imp_Pant_Simple("*Modo AP*");
        Serial.println("*Modo AP*");
      }
      else
      {
        imp_Pant_Simple("*Modo STA*");
        Serial.println("*Modo STA*");
      }
      grabarEprom();
      delay(4000);
      ESP.restart();
      break;
    }
  }
}

/////////////////////////////////////////
////  Actualiza teclado
void botonesPulsados()
{

  MD_UISwitch::keyResult_t k0 = Obj_Botones.read();

  if (k0 != MD_UISwitch::KEY_NULL)
  {
    tecla = Obj_Botones.getKey();
    accTecla = k0;
    Serial.print("[");
    Serial.print(tecla);
    Serial.print("] {");
    Serial.print(nomAccTecla[accTecla]);
    Serial.print("} {");
    Serial.print(accTecla);
    Serial.println("}");
    switch (accTecla)
    {
    // case 1: //DOWN
    //   mi_tono(3000, 50);
    //   break;
    // case 2: //up
    //   mi_tono(2000, 20);
    //   break;
    case 3: // press
      mi_tono(3500, 70);
      break;
    // case 4: //double
    //   mi_tono(3500, 50);
    //   break;
    // case 5: //long
    //   mi_tono(3000, 50);
    //   break;
    case 6: // repeat
      mi_tono(2500, 100);
      break;
    }
    funcion_Teclado();
  }
}
