#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servoEntrada;
Servo servoSalida;

const int echoEntrada = 9;
const int echoSalida = 11;
const int trigEntrada = 6;
const int trigSalida = 10;
const int pinServoEntrada = 5;
const int pinServoSalida = 3;
const int pinFoto = A0;
int duracionEntrada;
int duracionSalida;
int distanciaEntrada;
int distanciaSalida;
int intensidad;

// variables para la pantalla lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);
int contador = 4;
int prevContador = contador;
byte admir[] = {
  B00100,
  B00000,
  B00000,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};
byte aConTilde[] = {
  B00011,
  B00000,
  B01110,
  B00001,
  B01111,
  B10001,
  B01111,
  B00000
};

const int posicionInicial = 1;
const int posicionFinal = 90;
const float duracion = 750;
const float intervalo = 75;

void setup() {
  // inicializa la comunicacion con el otro arduino
  Serial.begin(9600);

  // inicializa la pantalla lcd
  lcd.init();
  // enciende la luz
  lcd.backlight();
  lcd.createChar(0, admir);
  lcd.createChar(1, aConTilde);
  // muestra el mensaje de bienvenida y el contador en cero
  mostrarBienvenida();

  // inicializa los servos
  servoEntrada.attach(pinServoEntrada);
  servoSalida.attach(pinServoSalida);
  servoEntrada.write(posicionInicial);
  servoSalida.write(posicionInicial);

  // inicializa los pines
  pinMode(echoEntrada, INPUT);
  pinMode(echoSalida, INPUT);
  pinMode(trigEntrada, OUTPUT);
  pinMode(trigSalida, OUTPUT);
  pinMode(pinFoto, INPUT);

  delay(500);
}

void loop() {
  // leer y activar entrada
  digitalWrite(trigEntrada, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigEntrada, LOW);
  duracionEntrada = pulseIn(echoEntrada, HIGH);
  distanciaEntrada = duracionEntrada / 58.2;

  // Además hacer la validación de que el contador no
  // esté en el máximo ya
  if (distanciaEntrada <= 10) {
    intensidad = analogRead(pinFoto);

    int a = 1;
    if (contador == 0) {
      parqueoLleno();
    } else {
      // reemplazar este `if (true)` por `intensidad == LOW`
      if (a == 2) {
        lcd.clear();
        mostrarPedirPin();
        bool esCorrecto = false;
        while (!esCorrecto) {
          char caracterRecibido;
          if (Serial.available() == 0) {
            while (Serial.available() == 0) {
              delay(250);
            }
            caracterRecibido = Serial.read();
            if (caracterRecibido == '1') {
              esCorrecto = true;
              lcd.clear();
              lcd.setCursor(2, 0);
              lcd.print("Pin correcto");
              activarEntrada();
            } else {
              lcd.clear();
              lcd.setCursor(1, 0);
              lcd.print("Pin incorrecto");
              lcd.setCursor(0, 1);
              lcd.print("Intente de nuevo");
              delay(1000);
            }
          } else {
            caracterRecibido = Serial.read();
            if (caracterRecibido == '1') {
              esCorrecto = true;
              lcd.clear();
              lcd.setCursor(2, 0);
              lcd.print("Pin correcto");
              activarEntrada();
            } else {
              lcd.clear();
              lcd.setCursor(1, 0);
              lcd.print("Pin incorrecto");
              lcd.setCursor(0, 1);
              lcd.print("Intente de nuevo");
              delay(1000);
            }
          }
        }
      } else {
        activarEntrada();
      }
    }
  }

  // leer y activar salida
  digitalWrite(trigSalida, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigSalida, LOW);
  duracionSalida = pulseIn(echoSalida, HIGH);
  distanciaSalida = duracionSalida / 58.2;


  if (distanciaSalida <= 10) {
    if (contador <= 3) {
      activarSalida();
    }
  }
}

void mostrarContador() {
  if (contador == 0) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("El parqueo est");
    lcd.setCursor(10, 0);
    lcd.write(1);
    lcd.setCursor(0, 1);
    lcd.print("lleno");
  } else if (prevContador == 0 && contador == 1) {
    mostrarBienvenida();
  } else {
    lcd.setCursor(14, 1);
    lcd.print(contador);
  }
}

void mostrarBienvenida() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.write(0);
  lcd.setCursor(3, 0);
  lcd.print("Bienvenido!");
  lcd.setCursor(1, 1);
  lcd.print("Disponibles:");
  lcd.setCursor(14, 1);
  lcd.print(contador);
}

void mostrarPedirPin() {
  lcd.setCursor(1, 0);
  lcd.print("Ingrese el pin");
  lcd.setCursor(1, 1);
  lcd.print("para continuar");
}

void activarEntrada() {
  contador--;
  int posicionActual = posicionInicial;
  int pasitos = duracion / intervalo;
  int incremento = (posicionFinal - posicionInicial) / pasitos;
  for (int i = 0; i < pasitos; i++) {
    posicionActual += incremento;
    servoEntrada.write(posicionActual);
    delay(intervalo);
  }
  delay(1200);
  for (int i = 0; i < pasitos; i++) {
    posicionActual -= incremento;
    servoEntrada.write(posicionActual);
    delay(intervalo);
  }
  if (contador == 0) {
    parqueoLleno();
  } else {
    mostrarBienvenida();
  }
}

void parqueoLleno() {
  lcd.clear();
  lcd.print(" Parqueo lleno");
}

void activarSalida() {
  contador++;
  int posicionActual = posicionInicial;
  int pasitos = duracion / intervalo;
  int incremento = (posicionFinal - posicionInicial) / pasitos;
  for (int i = 0; i < pasitos; i++) {
    posicionActual += incremento;
    servoSalida.write(posicionActual);
    delay(intervalo);
  }
  delay(1200);
  for (int i = 0; i < pasitos; i++) {
    posicionActual -= incremento;
    servoSalida.write(posicionActual);
    delay(intervalo);
  }
  mostrarBienvenida();
}
