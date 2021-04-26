const int SER = 2; //  Serial input en pin 2
const int RCLK = 4; // Output Register Clock en pin 4
const int SRCLK = 5; // Shift Register Clock en pin 5

int opcion;
int num_patrones = 0, patron_actual = 0;
int duracion = 0;
bool opcion_secuencia = 0;

// Mientras tanto, sin memoria dinamica, maximo 5 patrones:
// bool patrones_usuario[5][8][8];

// Con memoria dinámica:
bool ***patrones_usuario;

void reservar_memoria(int n) // n = número patrones que quiere el usuario
{
  // Reservar memoria para n patrones
  patrones_usuario = new bool **[n];
  for (int patron = 0 ; patron < n ; patron++)
  {
    // Reservar memoria para 8 filas
    patrones_usuario[patron] = new bool *[8];
    for (int fila = 0 ; fila < 8 ; fila++)
    {
      // Reservar memoria para 8 columnas
      patrones_usuario[patron][fila] = new bool[8];
    }
  }
}

void liberar_memoria(int n) // liberar memoria :P
{
  for (int patron = 0 ; patron < n ; patron++)
  {
    for (int fila = 0 ; fila < 8 ; fila++)
    {
      delete[] patrones_usuario[patron][fila];
    }
    delete[] patrones_usuario[patron];
  }
}


// fin memoria dinámica

bool patrones_predeterminados[4][8][8] =
{
  {

    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}

  },
  {

    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}

  },
  {

    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,1,0,0,1,0,0},
    {0,0,1,0,0,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}

  },
  {

    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}

  }
};




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
    3. Ingresar y mostrar secuencia de patrones \n \
    4. Usar secuencia predeterminada" );

}


void loop()
{


  if (Serial.available())
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
      liberar_memoria(num_patrones);
      opcion_secuencia = 0;
      patron_actual = 0;
      num_patrones = 1;
      reservar_memoria(num_patrones);
      Serial.println("Digite fila 1 del patron 1");
      imagen(0);
      menu();
      break;

      case 3:
      opcion_secuencia = 0;
      patron_actual = 0;
      //publik(duracion, num_patrones);
      publik();
      menu();
      break;

      case 4:
      opcion_secuencia = 1;
      patron_actual = 0;
      num_patrones = 4;
      duracion = 300;
      menu();
      break;
    }
  }


  presentar_imagen(patron_actual);

  // Verbose - comentar
  /*
  Serial.print("Duracion: ");
  Serial.print(duracion);
  Serial.print(", Numero de Patrones: ");
  Serial.print(num_patrones);
  Serial.print(", Patron actual: ");
  Serial.println(patron_actual);
  */

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
        patrones_usuario[posicion][fila][col] = temp.toInt();
        Serial.print(patrones_usuario[posicion][fila][col]);
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

//void publik(int &duracion, int &num_patrones)
void publik()
{
  liberar_memoria(num_patrones);
  int estado = 0;
  Serial.println("Digite el numero de patrones a secuenciar:");
  while (1)
  {
    if (Serial.available() > 0)
    {
      if (estado == 0 )
      {
        num_patrones = Serial.parseInt();
        Serial.println(num_patrones);
        reservar_memoria(num_patrones);
        estado = 1;
        Serial.println("Digite la duracion (ms) entre visualizaciones:");
      }
      else if ( estado == 1)
      {
        duracion = Serial.parseInt();
        Serial.println(duracion);
        estado = 2;
        Serial.println("Digite a continuacion los patrones:");
        Serial.println("Digite fila 1 del patron 1");
      }
      else if ( estado == 2)
      {
        for ( int posicion = 0 ; posicion < num_patrones ;  posicion++)
        {
          imagen(posicion);
          if(posicion != num_patrones-1)
          {
            Serial.print("Digite fila 1 del patron ");
            Serial.println(posicion+2);
          }
        }
        estado = 3;
      }
      if ( estado == 3)
      {
        //Serial.println("Sali");
        break;

      }
    }
  }
}



void desplazar_registro(bool *fila)
// void desplazar_registro(bool fila[8])  
  
{
  for (int col=7 ; col>=0 ; col--)
  {
    digitalWrite(SER, fila[col]);
    digitalWrite(SRCLK, 0);
    digitalWrite(SRCLK, 1);
  }
}

void presentar_imagen(int patron)
{
  if(num_patrones!=0)
  {
    for (int fila=7 ; fila>=0 ; fila--)
    {
      if (opcion_secuencia == 0){

        desplazar_registro(patrones_usuario[patron][fila]);
      }
      else {
        desplazar_registro(patrones_predeterminados[patron][fila]);
      }
    }

    digitalWrite(RCLK, 0);
    digitalWrite(RCLK, 1);
  }

}





