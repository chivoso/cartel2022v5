

///////////////////////////////////////
// inicia pantalla
void inicia_Pantalla()
{
  Obj_Pantalla.begin();
  Obj_Pantalla.displayClear();
  Obj_Pantalla.displaySuspend(false);
  Obj_Pantalla.setIntensity(brillo_textos); // Setea brillo (0-15)
  Obj_Pantalla.setTextAlignment(PA_CENTER); // Texto centrado
  // textEffect_t  scrollEffect = PA_SCROLL_LEFT;
  // Obj_Pantalla.setTextEffect(scrollEffect, scrollEffect);
  // Obj_Pantalla.setSpeed(velocidad);
}

//////////////////////////////////////////////////
// Inicia digitos
void inicia_Digitos()
{
  for (u_int8_t x = 0; x < CANT_MOD_DIG; x++)
  {                                                // Inicia digitos
    Obj_Digito.shutdown(x, false);                 // MAX7219 es activado (Digitos)
    Obj_Digito.setIntensity(x, brillo_textos + 2); // Setea brillo (0-15)
    Obj_Digito.clearDisplay(x);                    // Limpia digitos
  }
}

////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
uint8_t utf8Ascii(uint8_t ascii)
// Convert a single Character from UTF8 to Extended ASCII according to ISO 8859-1,
// also called ISO Latin-1. Codes 128-159 contain the Microsoft Windows Latin-1
// extended characters:
// - codes 0..127 are identical in ASCII and UTF-8
// - codes 160..191 in ISO-8859-1 and Windows-1252 are two-byte characters in UTF-8
//                 + 0xC2 then second byte identical to the extended ASCII code.
// - codes 192..255 in ISO-8859-1 and Windows-1252 are two-byte characters in UTF-8
//                 + 0xC3 then second byte differs only in the first two bits to extended ASCII code.
// - codes 128..159 in Windows-1252 are different, but usually only the €-symbol will be needed from this range.
//                 + The euro symbol is 0x80 in Windows-1252, 0xa4 in ISO-8859-15, and 0xe2 0x82 0xac in UTF-8.
//
// Modified from original code at http://playground.arduino.cc/Main/Utf8ascii
// Extended ASCII encoding should match the characters at http://www.ascii-code.com/
//
// Return "0" if a byte has to be ignored.
{
  static uint8_t cPrev;
  uint8_t c = '\0';
  if (ascii < 0x7f) // Standard ASCII-set 0..0x7F, no conversion
  {
    cPrev = '\0';
    c = ascii;
  }
  else
  {
    switch (cPrev) // Conversion depending on preceding UTF8-character
    {
    case 0xC2:
      c = ascii;
      break;
    case 0xC3:
      c = ascii | 0xC0;
      break;
    case 0x82:
      if (ascii == 0xAC)
        c = 0x80; // Euro symbol special case
    }
    cPrev = ascii; // save last char
  }
  return (c);
}

///////////////////////////////////////////////////////////////
void utf8Ascii(char *s)
// In place conversion UTF-8 string to Extended ASCII
// The extended ASCII string is always shorter.
{
  uint8_t c = 0;
  char *cp = s;
  while (*s != '\0')
  {
    c = utf8Ascii(*s++);
    if (c != '\0')
      *cp++ = c;
  }
  *cp = '\0'; // terminate the new string
}

///////////////////////////////////////////
// impresion sencilla
void imp_Pant_Simple(String texto)
{
  // Serial.print("Simple: ");
  // Serial.println(texto);
  char *texto2 = (char *)"12345678901234567890123456789012345678901234567890";
  texto.toCharArray(texto2, texto.length() + 1);
  utf8Ascii(texto2);
  Obj_Pantalla.print(texto);
  f_anima_pant = false;
}

///////////////////////////////////////////
// impresion scroll izquierdo
void imp_Pant_Efecto(String texto = "VACIO", u_int8_t efecto = 0, u_int16_t pausa = 4000)
{
  // MD_MAX72XX::fontType_t *fuente = ExtASCII;
  MD_MAX72XX::fontType_t *fuente = boldFont;
  Obj_Pantalla.setFont(fuente);

  // Serial.print("Scroll: ");
  // Serial.println(texto);
  char *texto2 = (char *)"12345678901234567890123456789012345678901234567890";
  texto.toCharArray(texto2, texto.length() + 1);
  utf8Ascii(texto2);

  if (efecto == 0)
  {
    Obj_Pantalla.displayText(texto2, PA_CENTER, 20, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    Obj_Pantalla.displayReset();
    f_anima_pant = true;
  }
  else
  {
    //// set up all the new animation parameters manually
    u_int8_t eIn = random(ARRAY_SIZE(catalog));
    u_int8_t eOut = random(ARRAY_SIZE(catalog));
    Obj_Pantalla.setTextBuffer(texto2);
    Obj_Pantalla.setTextAlignment(PA_CENTER);

    uint16_t vel_ent = map(vel_texto, 1, 10, catalog[eIn].speed_min, catalog[eIn].speed_max);// mapea velocidades min y max de acurdo a la web
    uint16_t vel_sal = map(vel_texto, 1, 10, catalog[eOut].speed_min, catalog[eOut].speed_max);// mapea velocidades min y max de acurdo a la web
    Obj_Pantalla.setSpeedInOut(vel_ent, vel_sal);
    Obj_Pantalla.setPause(pausa);
    Obj_Pantalla.setTextEffect(catalog[eIn].effect, catalog[eOut].effect);
#if EN_SPRITE
    if (catalog[eIn].effect == PA_SPRITE || catalog[eOut].effect == PA_SPRITE)
    {
      Obj_Pantalla.setSpeed(catalog[eIn].effect == PA_SPRITE ? vel_ent : vel_sal);
      Obj_Pantalla.setTextEffect(PA_SPRITE, PA_SPRITE);
      uint8_t curFXin = random(ARRAY_SIZE(sprite));
      uint8_t curFXout = random(ARRAY_SIZE(sprite));
      Obj_Pantalla.setSpriteData(sprite[curFXin].data, sprite[curFXin].width, sprite[curFXin].frames,  // entry sprite
                                 sprite[curFXout].data, sprite[curFXout].width, sprite[curFXout].frames); // exit sprite
    }
#endif
    Obj_Pantalla.displayReset();
    // Serial.print(catalog[eIn].psz);
    // Serial.print("\t");
    // Serial.print(vel_ent);
    // Serial.print("\t");
    // Serial.print(vel_sal);
    // Serial.print("\t");
    // Serial.println(catalog[eOut].psz);
    f_anima_pant = true;
  }
}

///////////////////////////////////////////
// Imprime la direccion IP
void mostrar_Ip()
{

  if (modo_offline)
  {
    Serial.print("   *** Buscar WiFi: ");
    Serial.println(ssidAP);
    imp_Pant_Efecto("   *** Buscar WiFi= " + String(ssidAP));
  }
  else
  {
    Serial.print(" *** Direc. IP= ");
    Serial.println(WiFi.localIP());
    imp_Pant_Efecto("   *** Dirección IP= " + WiFi.localIP().toString() + "");
  }
}

///////////////////////////////////////////
// Imprime un numero de 4 cifras en los digitos
void imp_Num_dig(u_int16_t numero, u_int8_t loteria, boolean mostrar)
{ // Imprime numeros
  /*   valor= entero 0000 a 9999, renglon= entero  loteria de 1 a 6, mostrar= 0 pone guiones, 1  muestra valor */
  // Serial.print(valor);
  // Serial.print(" - ");
  // Serial.print(renglon);
  // Serial.print(" - ");
  // Serial.println(mostrar);
  u_int8_t panel = 0;  // panel 0 o 1 de cada modulo (c/u 4 digitos)
  u_int8_t modulo = 0; // modulos del 0 a 2 (c/u 8 digitos)
  u_int8_t digito[4];  // separa los valores de cada digito
  digito[3] = numero % 10;
  numero = numero / 10;
  digito[2] = numero % 10;
  numero = numero / 10;
  digito[1] = numero % 10;
  numero = numero / 10;
  digito[0] = numero;
  switch (loteria)
  {
  case 0:
    modulo = 0;
    panel = 0;
    break; // Provincia 1
  case 1:
    modulo = 0;
    panel = 1;
    break; // Ciudad  2
  case 2:
    modulo = 1;
    panel = 0;
    break; // Loteria 3
  case 3:
    modulo = 1;
    panel = 1;
    break; // Loteria 4
  case 4:
    modulo = 2;
    panel = 0;
    break; // Loteria 5
  case 5:
    modulo = 2;
    panel = 1;
    break; // Loteria 6
  default:
    Serial.println("Error loteria...");
    break;
  }

  if (mostrar == true)
    for (u_int8_t v = 0; v <= 3; v++)
      Obj_Digito.setDigit(modulo, 3 - v + panel * 4, digito[v], false);
  else
    for (u_int8_t v = 0; v <= 3; v++)
      Obj_Digito.setChar(modulo, 3 - v + panel * 4, '-', false);
}

///////////////////////////////////////////
// imprime los numeros de una jugada
void imp_num_loterias(u_int8_t jug)
{
  for (u_int8_t lot = 0; lot < ARRAY_SIZE(n_Loteria); lot++)
  {
    u_int8_t indice = lot + jug * ARRAY_SIZE(n_Loteria);
    imp_Num_dig(mi_n_salido[indice].numero, lot, mi_n_salido[indice].f_mostrar);
  }
}

///////////////////////////////////////////
// pasar al texto siguiente
void cambia_Texto()
{
  static String texto;
  texto = n_Jugada[jug_Actual];
  if (random(20) == 1)
  {
    u_int8_t textoVarios = random(6 + 1);
    if (textoVarios == 0)
      texto = "Bruja-" + String(random(99)) + String(random(99)) + "";
    if (textoVarios == 1)
      texto = "Dato-" + String(random(99)) + String(random(99)) + "";
    if (textoVarios == 2)
      texto = "Fija-" + String(random(99)) + String(random(99)) + "";
    if (textoVarios == 3)
      texto = "Suerte-" + String(random(99)) + String(random(99)) + "";
    if (textoVarios == 4)
      texto = "Jugá y Ganá";
    if (textoVarios == 5)
      texto = "Ganá Acá";
    if (textoVarios == 6) // los sueños
    {
      u_int8_t num = random(ARRAY_SIZE(mi_sueños));
      texto = "";
      if (mi_sueños[num].numero < 10) // agrega un 0 si es solo 1 digito
        texto = "0";
      texto += String(mi_sueños[num].numero) + "-" + mi_sueños[num].significado;
    }
  }
  imp_Pant_Efecto("-" + texto + "-", 1);
}

//////////////////////////////////////////
// enciende un segmento determinado
void imp_Segm_dig(u_int8_t renglon, u_int8_t digito, u_int8_t segmento)
{
  u_int8_t seg[] = {
      B00000000, // todo apagado
      B01000000, // seg A
      B00100000, // seg B
      B00010000,
      B00001000,
      B00000100,
      B00000010,
      B00000001};
  u_int8_t panel = 0;
  u_int8_t modulo = 0;
  switch (renglon)
  {
  case 0:
    modulo = 0;
    panel = 0;
    break; // Provincia 1
  case 1:
    modulo = 0;
    panel = 1;
    break; // Ciudad  2
  case 2:
    modulo = 1;
    panel = 0;
    break; // Loteria 3
  case 3:
    modulo = 1;
    panel = 1;
    break; // Loteria 4
  case 4:
    modulo = 2;
    panel = 0;
    break; // Loteria 5
  case 5:
    modulo = 2;
    panel = 1;
    break; // Loteria 6
  }
  Obj_Digito.setRow(modulo, digito + panel * 4, seg[segmento]);
  // Serial.print("modulo: ");
  // Serial.print(modulo);
  // Serial.print(" /panel: ");
  // Serial.print(panel);
  // Serial.print(" /seg: ");
  // Serial.println(seg[segmento]);
}
////////////////////////////////
// Hace un giro en un digito
void imp_anima_dig()
{
  u_int8_t maxcuadro = 12;
  static u_int8_t cuadro = 0;
  cuadro++;
  if (cuadro > maxcuadro)
  {
    cuadro = 0;
    f_fin_Anim_Dig = true;
  }
  u_int8_t segmento = cuadro;
  if (segmento > 6)
    segmento = segmento - 6;
  for (u_int8_t renglon = 0; renglon < 6; renglon++)
    for (u_int8_t digito = 0; digito < 4; digito++)
      if (renglon == 0 || renglon == 2 || renglon == 4)
        imp_Segm_dig(renglon, digito, segmento);
      else
        imp_Segm_dig(renglon, digito, 7 - segmento);
}

//////////////////////////////////////////////
void cambiar_Digitos()
{
  imp_anima_dig();    // animacion cambio de digitos
  if (f_fin_Anim_Dig) // hasta que terminan de pasar los textos
  {
    imp_num_loterias(jug_Actual); // impresion de los nuevo numeros
    f_cambia_Dig = false;
    f_fin_Anim_Dig = false;
  }
}
