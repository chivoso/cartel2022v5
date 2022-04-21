


//////////////////////////////////////////////////////////////////
// hace un escala de sonido
void mi_tono(uint16_t ton, uint32_t dur)
{
  Serial.print("\n-...Piip...-");
  if (f_sonido)
  {
    tone(PIN_BUZZER, ton, dur);
  }
}

//////////////////////////////////////////////////////////////////
// hace un escala de sonido
void son_escala_arriba()
{
  // static int tonoBuzzer = 200; // Estado del Buzzer
  // tonoBuzzer += 80;
  // if (tonoBuzzer > 700)
  //   tonoBuzzer = 200;
  // mi_tono(tonoBuzzer, 50);
}

