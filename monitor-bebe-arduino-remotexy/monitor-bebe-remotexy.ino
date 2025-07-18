/*
 * MONITOR DE BEBÉS - V.1.0.0
 * Sin Serial Monitor - DFPlayer en Hardware Serial
 * 
 * CONEXIONES:
 * ESP8266 TX → Arduino Pin 10
 * ESP8266 RX → Arduino Pin 11
 * DFPlayer TX → Arduino Pin 0 (RX)
 * DFPlayer RX → Arduino Pin 1 (TX)
 * DHT22 → Pin 7
 * PIR → Pin 12
 * Micrófono → Pin A0
 * LEDs WS2812 → Pin 6
 */

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

#define REMOTEXY_MODE__ESP8266_SOFTSERIAL_POINT
#include <SoftwareSerial.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 10
#define REMOTEXY_SERIAL_TX 11
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_ACCESS_PASSWORD "210424"

#include <RemoteXY.h>

//////////////////////////////////////////////
//        Librerías y definiciones          //
//////////////////////////////////////////////

// Sensores
#include <dht.h>
#define DHT_PIN 7
#define PIR_PIN 12
#define MIC_PIN A0
dht DHT;

// DFPlayer - Hardware Serial
#include <DFRobotDFPlayerMini.h>
DFRobotDFPlayerMini myDFPlayer;
bool isPlaying = false;
int currentSong = 1;
int currentVolume = 15;

// LEDs WS2812
#include <FastLED.h>
#define LED_PIN 6
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];

// LED indicador de movimiento
#define LED_MOVIMIENTO 2

// RemoteXY GUI configuration (TU NUEVA CONFIGURACIÓN)
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 599 bytes
  { 255,12,0,23,0,80,2,19,0,0,0,77,111,110,105,116,111,114,98,101,
  98,101,0,20,1,106,200,1,1,39,0,130,6,17,95,37,8,16,129,3,
  3,109,10,0,203,240,159,145,182,32,77,111,110,105,116,111,114,32,100,101,
  32,98,101,98,195,169,32,240,159,145,182,0,72,17,27,15,15,12,179,183,
  6,28,0,0,0,0,0,0,200,66,0,0,0,0,129,12,21,23,5,64,
  192,72,117,109,101,100,97,100,0,72,73,27,16,16,4,179,181,2,28,0,
  0,0,0,0,0,200,66,0,0,0,0,129,68,20,30,5,64,2,84,101,
  109,112,101,114,97,116,117,114,97,0,67,17,41,11,8,94,6,30,67,73,
  42,14,8,78,64,30,2,129,30,42,6,7,64,6,37,0,129,88,42,8,
  7,64,2,194,176,67,0,130,1,55,104,45,8,41,129,16,61,80,8,64,
  15,77,111,100,111,115,32,100,101,108,32,99,117,97,114,116,111,32,240,159,
  155,143,239,184,143,0,1,7,73,14,14,0,8,31,0,129,5,89,17,4,
  64,24,65,112,97,103,97,100,111,0,1,27,73,14,14,0,162,31,0,129,
  28,90,13,4,64,162,68,111,114,109,105,114,0,1,49,73,14,14,0,92,
  31,0,129,48,90,15,4,64,77,67,97,109,98,105,111,0,1,69,73,13,
  13,0,27,31,0,129,69,90,14,4,64,27,77,97,110,117,97,108,0,1,
  88,74,12,12,0,1,31,0,129,89,90,12,4,64,1,65,108,101,114,116,
  97,0,130,6,123,48,74,14,180,4,13,140,14,43,32,26,26,10,32,140,
  13,13,48,6,26,31,80,76,65,89,0,1,83,84,79,80,0,1,32,155,
  13,13,0,133,31,78,69,88,84,32,62,62,0,1,32,170,13,13,0,62,
  31,60,60,32,80,82,69,86,0,129,10,138,21,5,64,25,86,111,108,117,
  109,101,110,0,129,11,125,42,9,64,187,83,111,117,110,100,32,240,159,148,
  138,0,67,11,185,37,8,100,6,30,11,129,88,28,16,12,64,17,240,159,
  140,161,239,184,143,0,129,7,30,13,10,64,17,240,159,146,167,0,130,56,
  123,47,74,14,96,129,69,126,27,9,64,215,76,117,122,240,159,146,161,0,
  6,60,144,40,40,202,186,70,20,109,10,10,18,30,37,8,73,61,112,34,
  5,109,133,9,5,26,0,0,0,0,0,0,200,66,0,0,0,0,135,0,
  0,72,66,0,0,140,66,93,0,0,140,66,0,0,180,66,36,0,0,180,
  66,0,0,190,66,129,8,103,34,6,64,1,77,111,118,105,109,105,101,110,
  116,111,0,129,63,102,29,7,64,6,66,97,98,121,32,99,114,121,0 };
  
struct {
  uint8_t mode_off;
  uint8_t mode_sleep;
  uint8_t mode_change;
  uint8_t mode_manual;
  uint8_t mode_emergency;
  int8_t volumen;
  uint8_t play_stop;
  uint8_t next;
  uint8_t preview;
  uint8_t rgb_01_r;
  uint8_t rgb_01_g;
  uint8_t rgb_01_b;
  int8_t humedad;
  float temperatura;
  int8_t hum_str;
  float temp_str;
  char song[11];
  uint8_t movimiento;    // 0=No movimiento, 1=Movimiento detectado
  int8_t cry;            // 0-100 nivel de llanto/sonido
  uint8_t connect_flag;
} RemoteXY;   
#pragma pack(pop)

//////////////////////////////////////////////
//           Variables globales             //
//////////////////////////////////////////////

// Estado del sistema
byte modoActual = 0;
float temperatura = 22.0;
float humedad = 50.0;
int nivelSonido = 0;
bool movimientoDetectado = false;

// Control de botones
bool lastModeOff = false, lastModeSleep = false, lastModeChange = false;
bool lastModeManual = false, lastModeEmergency = false;
bool lastPlayStop = false, lastNext = false, lastPreview = false;

// Timers
unsigned long ultimaLecturaSensores = 0;
unsigned long ultimoParpadeoMovimiento = 0;
unsigned long ultimoCambioModo = 0;

// LED de estado del sistema (pin 13 integrado)
#define STATUS_LED 13

void setup() {
  //////////////////////////////////////////////
  //           Inicialización                 //
  //////////////////////////////////////////////
  
  // Inicializar RemoteXY
  RemoteXY_Init();
  
  // Configurar pines de sensores
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_MOVIMIENTO, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  // Inicializar DFPlayer con Hardware Serial
  Serial.begin(9600);
  delay(1000);
  
  if (myDFPlayer.begin(Serial)) {
    myDFPlayer.volume(currentVolume);
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    // Parpadear LED de estado para indicar DFPlayer OK
    for(int i=0; i<3; i++) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  } else {
    // Parpadear rápido si hay error en DFPlayer
    for(int i=0; i<10; i++) {
      digitalWrite(STATUS_LED, HIGH);
      delay(50);
      digitalWrite(STATUS_LED, LOW);
      delay(50);
    }
  }
  
  // Inicializar LEDs WS2812
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  FastLED.clear();
  FastLED.show();
  
  // Secuencia de inicio de LEDs
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(50);
  }
  FastLED.clear();
  FastLED.show();
  
  // Inicializar valores RemoteXY
  strcpy(RemoteXY.song, "Listo");
  RemoteXY.temperatura = temperatura;
  RemoteXY.humedad = (int8_t)humedad;
  RemoteXY.movimiento = 0;
  RemoteXY.cry = 0.0;
  
  // LED de estado final - sistema listo
  digitalWrite(STATUS_LED, HIGH);
  delay(1000);
  digitalWrite(STATUS_LED, LOW);
}

void loop() {
  RemoteXY_Handler();
  
  // Leer sensores cada 500ms
  if (millis() - ultimaLecturaSensores >= 500) {
    leerSensores();
    actualizarDisplays();
    ultimaLecturaSensores = millis();
  }
  
  // Procesar controles
  procesarBotones();
  procesarControlesAudio();
  procesarControlesLEDs();
  
  // Efectos visuales
  efectoMovimiento();
  indicadorEstado();
  
  delay(10);
}

//////////////////////////////////////////////
//           Funciones de sensores          //
//////////////////////////////////////////////

void leerSensores() {
  // Leer DHT22
  int chk = DHT.read22(DHT_PIN);
  if (chk == DHTLIB_OK) {
    temperatura = DHT.temperature;
    humedad = DHT.humidity;
  }
  
static int lecturasSonido[3] = {0};  // Menos muestras = más reactivo
static int indiceLectura = 0;
static int nivelBase = 305;  // Nivel de silencio (se auto-ajusta)
static unsigned long ultimaCalib = 0;

// Leer valor actual
int valorActual = analogRead(MIC_PIN);
lecturasSonido[indiceLectura] = valorActual;
indiceLectura = (indiceLectura + 1) % 3;

// Encontrar el máximo de las últimas 3 lecturas
int maximo = 0;
for(int i=0; i<3; i++) {
  if(lecturasSonido[i] > maximo) {
    maximo = lecturasSonido[i];
  }
}

// Auto-calibración del nivel base cada 30 segundos
if(millis() - ultimaCalib > 30000) {
  int suma = 0;
  for(int i=0; i<3; i++) {
    suma += lecturasSonido[i];
  }
  int promedio = suma / 3;
  
  // Solo actualizar si el ambiente está silencioso
  if(abs(promedio - nivelBase) < 50) {
    nivelBase = promedio;
  }
  ultimaCalib = millis();
}

// Calcular diferencia respecto al silencio
int diferencia = abs(maximo - nivelBase);

// Aplicar ganancia y limitar
nivelSonido = diferencia;
if(nivelSonido > 1023) nivelSonido = 1023;
  
  // Leer PIR
  movimientoDetectado = digitalRead(PIR_PIN);
}

void actualizarDisplays() {
  // Actualizar temperatura y humedad
  RemoteXY.temperatura = temperatura;
  RemoteXY.humedad = (int8_t)humedad;
  RemoteXY.temp_str = temperatura;
  RemoteXY.hum_str = (int8_t)humedad;
  
  // Actualizar indicador de movimiento
  RemoteXY.movimiento = movimientoDetectado ? 1 : 0;
  
  // Actualizar barra de llanto (0-100)
  // Mapear sonido de 0-1023 a 0-100
  RemoteXY.cry = (int8_t)map(nivelSonido, 0, 336, 0, 100);;  // Rango reducido para máxima sensibilidad
  if (RemoteXY.cry > 100) RemoteXY.cry = 100;
  if (RemoteXY.cry < 0) RemoteXY.cry = 0;

  // Suavizar para evitar saltos bruscos
  static int8_t cryAnterior = 0;
  RemoteXY.cry = (int8_t)((RemoteXY.cry * 0.7) + (cryAnterior * 0.3));
  cryAnterior = RemoteXY.cry;
}

//////////////////////////////////////////////
//         Funciones de control             //
//////////////////////////////////////////////

void procesarBotones() {
  // MODO APAGADO
  if (RemoteXY.mode_off && !lastModeOff) {
    modoActual = 0;
    strcpy(RemoteXY.song, "Apagado");
    ultimoCambioModo = millis();
  }
  lastModeOff = RemoteXY.mode_off;
  
  // MODO DORMIR
  if (RemoteXY.mode_sleep && !lastModeSleep) {
    modoActual = 1;
    strcpy(RemoteXY.song, "Dormir");
    ultimoCambioModo = millis();
  }
  lastModeSleep = RemoteXY.mode_sleep;
  
  // MODO CAMBIO
  if (RemoteXY.mode_change && !lastModeChange) {
    modoActual = 2;
    strcpy(RemoteXY.song, "Cambio");
    ultimoCambioModo = millis();
  }
  lastModeChange = RemoteXY.mode_change;
  
  // MODO MANUAL
  if (RemoteXY.mode_manual && !lastModeManual) {
    modoActual = 3;
    strcpy(RemoteXY.song, "Manual");
    ultimoCambioModo = millis();
  }
  lastModeManual = RemoteXY.mode_manual;
  
  // MODO ALERTA
  if (RemoteXY.mode_emergency && !lastModeEmergency) {
    modoActual = 4;
    strcpy(RemoteXY.song, "Alerta");
    ultimoCambioModo = millis();
  }
  lastModeEmergency = RemoteXY.mode_emergency;
  
  // Configurar modo después de cambio
  if (millis() - ultimoCambioModo >= 100 && ultimoCambioModo > 0) {
    configurarModo();
    ultimoCambioModo = 0;
  }
}

void procesarControlesAudio() {
  if (modoActual == 3) { // Solo en modo MANUAL
    
    // Control Play/Stop
    if (RemoteXY.play_stop != lastPlayStop) {
      if (RemoteXY.play_stop) {
        if (!isPlaying) {
          myDFPlayer.start();
          isPlaying = true;
        }
      } else {
        if (isPlaying) {
          myDFPlayer.pause();
          isPlaying = false;
        }
      }
      lastPlayStop = RemoteXY.play_stop;
    }
    
    // Control Next
    if (RemoteXY.next && !lastNext) {
      currentSong++;
      if (currentSong > 4) currentSong = 1;
      myDFPlayer.play(currentSong);
      isPlaying = true;
      strcpy(RemoteXY.song, ("Song " + String(currentSong)).c_str());
    }
    lastNext = RemoteXY.next;
    
    // Control Previous
    if (RemoteXY.preview && !lastPreview) {
      currentSong--;
      if (currentSong < 1) currentSong = 4;
      myDFPlayer.play(currentSong);
      isPlaying = true;
      strcpy(RemoteXY.song, ("Song " + String(currentSong)).c_str());
    }
    lastPreview = RemoteXY.preview;
    
    // Control Volumen
    int vol = map(RemoteXY.volumen, -100, 100, 0, 30);
    if (vol != currentVolume) {
      currentVolume = vol;
      myDFPlayer.volume(currentVolume);
    }
    
  } else {
    // Actualizar controles en modos automáticos
    RemoteXY.volumen = map(currentVolume, 0, 30, -100, 100);
    RemoteXY.play_stop = isPlaying ? 1 : 0;
    lastNext = RemoteXY.next;
    lastPreview = RemoteXY.preview;
    lastPlayStop = RemoteXY.play_stop;
  }
}

void procesarControlesLEDs() {
  if (modoActual == 3) { // Solo en modo MANUAL
    static uint8_t lastR = 255, lastG = 255, lastB = 255;
    
    if (RemoteXY.rgb_01_r != lastR || RemoteXY.rgb_01_g != lastG || RemoteXY.rgb_01_b != lastB) {
      fill_solid(leds, NUM_LEDS, CRGB(RemoteXY.rgb_01_r, RemoteXY.rgb_01_g, RemoteXY.rgb_01_b));
      FastLED.show();
      
      lastR = RemoteXY.rgb_01_r;
      lastG = RemoteXY.rgb_01_g;
      lastB = RemoteXY.rgb_01_b;
    }
  }
}

//////////////////////////////////////////////
//        Configuración por modo            //
//////////////////////////////////////////////

void configurarModo() {
  switch(modoActual) {
    case 0: // APAGADO
      myDFPlayer.pause();
      isPlaying = false;
      FastLED.clear();
      FastLED.show();
      break;
      
    case 1: // DORMIR
      currentSong = 1; // Canción relajante
      currentVolume = 30; // Volumen medio-alto
      myDFPlayer.volume(currentVolume);
      myDFPlayer.play(currentSong);
      isPlaying = true;
      
      // LEDs rojo suave
      FastLED.setBrightness(30);
      fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
      FastLED.show();
      break;
      
    case 2: // CAMBIO
      currentVolume = 15; // Volumen medio
      myDFPlayer.volume(currentVolume);
      
      // LEDs amarillo
      FastLED.setBrightness(100);
      fill_solid(leds, NUM_LEDS, CRGB(255, 50, 0));
      FastLED.show();
      break;
      
    case 3: // MANUAL
      // No hacer nada, el usuario controla
      FastLED.setBrightness(100);
      break;
      
    case 4: // ALERTA
      myDFPlayer.pause();
      isPlaying = false;
      
      // LEDs blanco suave (no parpadeo)
      FastLED.setBrightness(150);  // ← Brillo reducido
      fill_solid(leds, NUM_LEDS, CRGB::White);
      FastLED.show();
      break;
  }
}

//////////////////////////////////////////////
//          Efectos visuales                //
//////////////////////////////////////////////

void efectoMovimiento() {
  // LED físico de movimiento
  if (movimientoDetectado) {
    if (millis() - ultimoParpadeoMovimiento >= 200) {
      digitalWrite(LED_MOVIMIENTO, !digitalRead(LED_MOVIMIENTO));
      ultimoParpadeoMovimiento = millis();
    }
  } else {
    digitalWrite(LED_MOVIMIENTO, LOW);
  }
}

void indicadorEstado() {
  static unsigned long ultimoParpadeo = 0;
  static bool estadoLED = false;
  
  // Parpadeo según modo
  unsigned long intervalo = 2000; // Por defecto
  
  switch(modoActual) {
    case 0: intervalo = 3000; break; // APAGADO - lento
    case 1: intervalo = 2000; break; // DORMIR - normal
    case 2: intervalo = 1000; break; // CAMBIO - rápido
    case 3: intervalo = 500; break;  // MANUAL - muy rápido
    case 4: intervalo = 100; break;  // ALERTA - muy muy rápido
  }
  
  if (millis() - ultimoParpadeo >= intervalo) {
    estadoLED = !estadoLED;
    digitalWrite(STATUS_LED, estadoLED);
    ultimoParpadeo = millis();
  }
  
// Respuesta automática a llanto alto
  if (nivelSonido > 336 && modoActual != 3 && modoActual != 0) {
    // Activar respuesta automática
    if (modoActual == 1) { // En modo dormir
      // Subir volumen temporalmente
      myDFPlayer.volume(30);
    } else if (modoActual == 2) { // En modo cambio
      // Reproducir canción calmante
      myDFPlayer.play(2);
      isPlaying = true;
    }
  }
}
