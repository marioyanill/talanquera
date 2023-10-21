#include <Keypad.h>

const byte filas = 4;
const byte columnas = 4;
int contador = 0;
String codigo = "1234";
String entrada = "";
int alarma = 10;

char mapa[4][4] = {
  { '1',
    '2',
    '3',
    'A' },
  { '4',
    '5',
    '6',
    'B' },
  { '7',
    '8',
    '9',
    'C' },
  { '*',
    '0',
    '#',
    'D' }
};

byte pinFilas[filas] = {
  9,
  8,
  7,
  6
};
byte pinColumnas[columnas] = {
  5,
  4,
  3,
  2
};

Keypad teclado = Keypad(makeKeymap(mapa), pinFilas, pinColumnas, filas, columnas);

char tecla;

void setup() {
  Serial.begin(9600);
  // Si se muestra en la pantalla serial
  // Serial.println("Ingresa tu clave");
  pinMode(alarma, OUTPUT);
  noTone(alarma);
}

void loop() {
  tecla = teclado.getKey();
  if (tecla) {
    if (tecla == 'D') {
      if (codigo == entrada) {
        // SERIAL ONLY
        // Serial.println("\nClave correcta");
        Serial.write('x');
        contador = 0;
      } else {
        // SERIAL ONLY
        // Serial.println("\nClave incorrecta");
        Serial.write('y');
        contador++;
        if (contador >= 2) {
          // SERIAL ONLY
          // Serial.println("Acceso bloqueado");
          Serial.write('z');
          Alarma();
          contador = 0;
        }
      }
      entrada = "";
      // SERIAL ONLY
      // Serial.println("\nIngresa tu clave");
    } else {
      entrada += tecla;
      Serial.print("*");
    }
  }
}

void Alarma() {
  for (int i = 0; i <= 8; i++) {
    for (int i = 550; i <= 800; i += 25) {
      tone(alarma, i, 100);
    }
    for (int i = 800; i >= 550; i -= 25) {
      tone(alarma, i, 100);
    }
  }
}
