

//////////////////////////////////////////////////////////////////////////////////
// envia al server los archivos sueltos
void enviarImagenes()
{
  obj_web_server.on("/grillo.png", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(LittleFS, "/grillo.png", "image/png"); });
  obj_web_server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(LittleFS, "/favicon.ico", "image/ico"); });
  obj_web_server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(LittleFS, "/estilos.css", "text/css"); });
  obj_web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(LittleFS, "/index.html", "text/css"); });
}

///////////////////////////////////////////////////////////////////////
// lee los numeros de las variables numeros y mostNum
String getnumero(u_int8_t ren)
{
  u_int8_t seleccion = pag_Jugada * 6 + ren - 1;
  // Serial.printf("getnumero numero[%u]: %u\n", seleccion, numeros[seleccion]);
  String strNum = String(mi_n_salido[seleccion].numero);
  strNum = "000" + strNum;
  strNum.remove(0, strNum.length() - 4);
  if (mi_n_salido[seleccion].f_mostrar == 0)
    strNum = "----";
  return strNum;
}

///////////////////////////////////////////////////////////////////////
// lee los numeros de las variables f_most_Jug
String selectJug(u_int8_t a)
{
  String strJug = "";
  if (f_most_Jug[a - 1] == true)
    strJug = "checked";
  else
    strJug = "unchecked";
  return strJug;
}
///////////////////////////////////////////////////////////////////////
// remplaza en el html los datos de la pagina Numeros
String proceDatosHtml(const String &var)
{
  // Serial.printf("proceDatosHtml var: %s\n", var.c_str());
  if (var == "NUM1")
    return getnumero(1);
  else if (var == "NUM2")
    return getnumero(2);
  else if (var == "NUM3")
    return getnumero(3);
  else if (var == "NUM4")
    return getnumero(4);
  else if (var == "NUM5")
    return getnumero(5);
  else if (var == "NUM6")
    return getnumero(6);
  else if (var == "NUM7")
    return getnumero(7);
  else if (var == "NUM8")
    return getnumero(8);
  else if (var == "NUM9")
    return getnumero(9);
  else if (var == "NUM10")
    return getnumero(10);
  else if (var == "NUM11")
    return getnumero(11);
  else if (var == "NUM12")
    return getnumero(12);
  else if (var == "NUM13")
    return getnumero(13);
  else if (var == "NUM14")
    return getnumero(14);
  else if (var == "NUM15")
    return getnumero(15);
  else if (var == "NUM16")
    return getnumero(16);
  else if (var == "NUM17")
    return getnumero(17);
  else if (var == "NUM18")
    return getnumero(18);
  else if (var == "NUM19")
    return getnumero(19);
  else if (var == "NUM20")
    return getnumero(20);
  else if (var == "NUM21")
    return getnumero(21);
  else if (var == "NUM22")
    return getnumero(22);
  else if (var == "NUM23")
    return getnumero(23);
  else if (var == "NUM24")
    return getnumero(24);
  else if (var == "NUM25")
    return getnumero(25);
  else if (var == "NUM26")
    return getnumero(26);
  else if (var == "NUM27")
    return getnumero(27);
  else if (var == "NUM27")
    return getnumero(27);
  else if (var == "NUM28")
    return getnumero(28);
  else if (var == "NUM29")
    return getnumero(29);
  else if (var == "NUM30")
    return getnumero(30);
  else if (var == "TIEMPOJUG")
    return String(tiempo_jugada / 1000);
  else if (var == "VELOCIDAD")
    return String(vel_texto);
  else if (var == "BRITEXTO")
    return String(brillo_textos);
  else if (var == "ACTUALIZA")
    return String(actualizacion);
  else if (var == "CH_PREV")
    return selectJug(1);
  else if (var == "CH_PRIM")
    return selectJug(2);
  else if (var == "CH_MATU")
    return selectJug(3);
  else if (var == "CH_VESP")
    return selectJug(4);
  else if (var == "CH_NOCT")
    return selectJug(5);
  else
    return "";
}

///////////////////////////////////////////////////////////////
// toma los numeros de la pagina numeros y los carga en la variable
void cargaDatosNuevos(String dato, String valor)
{
  // Serial.printf("cargaDatoNuevos: %s -> %s\n", dato.c_str(), valor.c_str());
  if (dato.startsWith("num"))
  {
    dato.remove(0, 3);
    u_int8_t seleccion = pag_Jugada * 6 + dato.toInt() - 1;
    // Serial.printf("Num[seleccion][%u]: %u \n", seleccion, numeros[seleccion]);
    if (valor.endsWith("-"))
      mi_n_salido[seleccion].f_mostrar = 0;
    else
      mi_n_salido[seleccion].f_mostrar = 1;
    mi_n_salido[seleccion].numero = valor.toInt();
    // Serial.printf("Num2[seleccion][%u]: %u \n", seleccion, numeros[seleccion]);
  }
  else if (dato == "tiempojug")
    tiempo_jugada = valor.toInt() * 1000;
  else if (dato == "velocidad")
    vel_texto = valor.toInt();
  else if (dato == "britexto")
    brillo_textos = valor.toInt();
  else if (dato == "hch_prev")
    if (valor == "true")
      f_most_Jug[0] = 1;
    else
      f_most_Jug[0] = 0;
  else if (dato == "hch_prim")
    if (valor == "true")
      f_most_Jug[1] = 1;
    else
      f_most_Jug[1] = 0;
  else if (dato == "hch_matu")
    if (valor == "true")
      f_most_Jug[2] = 1;
    else
      f_most_Jug[2] = 0;
  else if (dato == "hch_vesp")
    if (valor == "true")
      f_most_Jug[3] = 1;
    else
      f_most_Jug[3] = 0;
  else if (dato == "hch_noct")
  {
    if (valor == "true")
      f_most_Jug[4] = 1;
    else
      f_most_Jug[4] = 0;
  }
}

///////////////////////////////////////////////////////////////
// todas las respuestas a las llamadas del servidor
void respuestas_Servidor()
{
  // enviando imagenes y otros
  enviarImagenes();

  obj_web_server.onNotFound([](AsyncWebServerRequest *request) { // Serial.println("pasando por el onNotFound");
    request->send(LittleFS, "/index.html", String(), false, proceDatosHtml);
  });

  obj_web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { // Serial.println("pasando por el on(/)GET");
    request->send(LittleFS, "/index.html", String(), false, proceDatosHtml);
  });

  obj_web_server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) { // Serial.println("pasando por el on(/)POST");
    int params = request->params();
    for (uint8_t i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isPost())
      { // Serial.printf("POSTbtn[%s]: %s\n", p->name().c_str(), p->value().c_str());
        cargaDatosNuevos(String(p->name().c_str()), String(p->value().c_str()));
      }
    }
    grabarEprom();
    request->send(LittleFS, "/index.html", String(), false, proceDatosHtml);
  });
}