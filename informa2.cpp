const int SER = 2; //  Serial input en pin 2
const int RCLK = 4; // Output Register Clock en pin 4
const int SRCLK = 5; // Shift Register Clock en pin 5


bool matrices[5][8][8];


void setup()
{
  Serial.begin(9600);
  // Inicialización de pines de salida
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);

  menu();

}

void menu()
{
  Serial.println("Digite una opcion: \n \
    1. Verifiacion \n \
    2. Ingresar y mostrar un patron \n \
    3. Ingresar y mostrar secuencia de patrones" );
}


void loop()
{
  int opcion;
  int num_patrones = 0, patron_actual = 0;
  int duracion = 0;

  if (Serial.available() > 0 )
  {
    opcion = Serial.parseInt();
    switch(opcion)
    {
      case 1:
      verificacion(1);
      delay(500);
      verificacion(0);
      menu();
      break;
      case 2:
      Serial.println("Digite fila 1 del patron 1");
      imagen(0);
      menu();
      num_patrones = 1;
      break;
      case 3:
      publik(duracion, num_patrones);
      menu();
      break;
    }
  }

  presentar_imagen(patron_actual);
  delay(duracion);
  patron_actual++;
  if ( patron_actual == num_patrones)
    patron_actual = 0;

}



void verificacion(bool sw) // comprobación del funcionamiento de los leds
{
  digitalWrite(RCLK, 0); // 
  for (int i=0 ; i<64 ; i++)
  {
    digitalWrite(SRCLK, 0);
    digitalWrite(SER, sw);
    digitalWrite(SRCLK, 1);
  }
  digitalWrite(RCLK, 1);
}

void imagen(int posicion) // Esto era antes captura_matriz, pero bueno...
{
  String entrada, temp;
  //Serial.print("Digite fila 1 del patron ");
  //Serial.println(posicion+1);
  int fila = 0;
  while (1)
  {
    if (Serial.available() > 0 )
    {
      entrada = Serial.readString();
      Serial.println(entrada);
      for (int col = 0 ; col<8 ; col++)
      {
        temp = entrada.substring(col,col+1);
        matrices[posicion][fila][col] = temp.toInt();
        Serial.print(matrices[posicion][fila][col]);
      }
      Serial.print("\n");
      fila++;
      if (fila == 8)
      {
        break;
      }

      Serial.print("Digite fila ");
      Serial.print(fila+1);
      Serial.print(" del patron ");
      Serial.println(posicion+1);
    }
  }
}

void publik(int &duracion, int &num_patrones)
{
  int estado = 0;
  Serial.println("Digite el numero de patrones a secuenciar:");
  while (1)
  {
    if (Serial.available() > 0 )
    {
      if (estado == 0 )
      {
        num_patrones = Serial.parseInt();
        Serial.println(num_patrones);
        estado = 1;
        Serial.println("Digite la duracion (ms) entre visualizaciones:");
      }
      else if ( estado == 1)
      {
        duracion = Serial.parseInt();
        Serial.println(duracion);
        estado = 2;
        Serial.println("Digite a continuacion los patrones:");
        Serial.print("Digite fila 1 del patron 1");
      }
      else if ( estado == 2)
      {
        for ( int posicion = 0 ; posicion < num_patrones ;  posicion++)
        {
          imagen(posicion);
          Serial.print("Digite fila 1 del patron ");
          Serial.println(posicion+1);
        }
        estado = 3;
      }
      else if ( estado == 3)
        break;
    }
  }
}



void desplazar_registro(bool *registro)
{
  for (int i=7 ; i>=0 ; i--)
  {
    digitalWrite(SER, registro[i]);
    digitalWrite(SRCLK, 0);
    digitalWrite(SRCLK, 1);
  }
}

void presentar_imagen(int posicion)
{
  for (int fila=7 ; fila>=0 ; fila--)
  {
    desplazar_registro(matrices[posicion][fila]);
  }
  digitalWrite(RCLK, 0);
  digitalWrite(RCLK, 1);

}




