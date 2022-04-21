

////////////////////////////////////////////////////////////
/////sub para cuando parpadea
void subParpadeo()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

//////////////////////////////////////////////////////////////////
// void subAnimacionPant()
// {
//   Obj_Pantalla.displayAnimate();
// }

//////////////////////////////////////////////////////////////////
void subAnimacionDig()
{
  if (f_cambia_Dig)
    cambiar_Digitos();
}

////////////////////////////////////////////////////////////
// avanza jugada y verifica
void avanza_Jugada()
{
  mi_tono(600,50);
avanzaJug:
  jug_Actual++;
  num_txt_Actual = 1;
  if (jug_Actual > ARRAY_SIZE(n_Jugada))// si supera la ultima vuelve a la 0
  {
    jug_Actual = 0;
  }
  if (f_most_Jug[jug_Actual] == false)// si esta no mostrar vuelve a sumar
  {
    goto avanzaJug;
  }
  n_jug_Actual = n_Jugada[jug_Actual];
  Serial.println(".");
  Serial.printf("Avanza Jugada a %s \n", n_jug_Actual.c_str());
  f_cambiar_jugada = true;
  f_anima_dig = true;
}
