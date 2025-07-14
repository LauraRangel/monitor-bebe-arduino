# 👶 Monitor de Bebés IoT con Arduino y Remotexy

Un sistema completo de monitoreo para bebés controlado remotamente vía WiFi con múltiples sensores, audio automático, iluminación LED y interfaz móvil.

## 🌟 Características

- 📱 **Control remoto** vía app móvil RemoteXY
- 🌡️ **Monitoreo ambiental** (temperatura, humedad)
- 👁️ **Detección de movimiento** con sensor PIR
- 🎤 **Detección de llanto** con micrófono calibrado
- 🎵 **Audio automático** con canciones relajantes
- 💡 **Iluminación LED** con efectos por modo
- 🔄 **5 modos de operación** personalizables
- 📊 **Visualización en tiempo real** de todos los parámetros

## 📸 Vista Previa

### Panel de Control RemoteXY
```
┌─────────────────────────────────┐
│ 👶 Monitor de bebé              │
├─────────────────────────────────┤
│ 🌡️22.0°C 💧50% 🔊Normal        │
├─────────────────────────────────┤
│ [APAGADO] [DORMIR] [CAMBIO]     │
│ [MANUAL] [ALERTA]               │
├─────────────────────────────────┤
│ [▶️] [⏮️] [⏭️] 🔊[████░░░]      │
│ 🔴[██] 🟢[░░] 🔵[░░]            │
└─────────────────────────────────┘
```

## 🛠️ Componentes Necesarios

### Hardware Principal
- **Arduino Uno R3**
- **ESP8266 WiFi Module** (NodeMCU o similar)
- **DFPlayer Mini** + MicroSD + Parlante
- **12x LEDs WS2812** (NeoPixel Strip)

### Sensores
- **DHT22** - Temperatura y humedad
- **PIR HC-SR501** - Detección de movimiento
- **Micrófono electret** - Detección de sonido

### Componentes Adicionales
- **LED 5mm** (indicador de movimiento)
- **Resistencias** 220Ω (2x)
- **Protoboard** y cables jumper
- **Fuente 5V 2A**

## 🔌 Diagrama de Conexiones

```
ESP8266:
├── TX → Arduino Pin 10
└── RX → Arduino Pin 11

DFPlayer Mini:
├── TX → Arduino Pin 9
├── RX → Arduino Pin 8
├── SPK+ → Parlante +
└── SPK- → Parlante -

Sensores:
├── DHT22 DATA → Pin 7
├── PIR OUT → Pin 12
└── Micrófono OUT → Pin A0

LEDs:
├── WS2812 DATA → Pin 6
├── LED Movimiento → Pin 2
└── LED Status → Pin 13 (integrado)

Alimentación:
├── Todos VCC → 5V
└── Todos GND → GND
```

## 📦 Instalación

### 1. Librerías Requeridas

Instalar en Arduino IDE:

```bash
# Librerías principales
- RemoteXY (3.1.13+)
- DFRobotDFPlayerMini
- FastLED
- DHT sensor library

# Librerías incluidas con Arduino IDE
- SoftwareSerial
```

### 2. Configuración del Módulo ESP8266 (CRÍTICO)

⚠️ **IMPORTANTE**: El ESP8266 debe configurarse como Access Point antes del primer uso.

#### Paso 1: Código de Configuración AT

Subir este código al Arduino para configurar el ESP8266:

```cpp
#include <SoftwareSerial.h>
SoftwareSerial ESP(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);
  ESP.begin(9600);
  
  Serial.println("=== CONFIGURADOR ESP8266 ===");
  Serial.println("Comandos AT automáticos iniciando...");
  
  delay(2000);
  
  // Reset del módulo
  Serial.println("1. Reseteando ESP8266...");
  ESP.println("AT+RST");
  delay(5000);
  
  // Configurar como Access Point únicamente
  Serial.println("2. Configurando modo Access Point...");
  ESP.println("AT+CWMODE_DEF=2");
  delay(1000);
  
  // Crear Access Point con credenciales del proyecto
  Serial.println("3. Creando Access Point RemoteXY...");
  ESP.println("AT+CWSAP_DEF=\"RemoteXY\",\"12345678\",5,3");
  delay(3000);
  
  // Habilitar múltiples conexiones
  Serial.println("4. Habilitando múltiples conexiones...");
  ESP.println("AT+CIPMUX=1");
  delay(1000);
  
  // Iniciar servidor TCP
  Serial.println("5. Iniciando servidor TCP...");
  ESP.println("AT+CIPSERVER=1,6377");
  delay(1000);
  
  // Verificar configuración
  Serial.println("6. Verificando configuración...");
  ESP.println("AT+CIFSR");
  delay(2000);
  
  Serial.println("=== CONFIGURACIÓN COMPLETADA ===");
  Serial.println("El ESP8266 está listo para el proyecto");
}

void loop() {
  // Relay para verificación manual si es necesario
  if (ESP.available()) {
    Serial.write(ESP.read());
  }
  if (Serial.available()) {
    ESP.write(Serial.read());
  }
}
```

#### Paso 2: Verificación de Configuración

En el Serial Monitor (115200 baud) deberías ver:

```
=== CONFIGURADOR ESP8266 ===
1. Reseteando ESP8266...
OK

2. Configurando modo Access Point...
AT+CWMODE_DEF=2
OK

3. Creando Access Point RemoteXY...
AT+CWSAP_DEF="RemoteXY","12345678",5,3
OK

4. Habilitando múltiples conexiones...
AT+CIPMUX=1
OK

5. Iniciando servidor TCP...
AT+CIPSERVER=1,6377
OK

6. Verificando configuración...
AT+CIFSR
+CIFSR:APIP,"192.168.4.1"
+CIFSR:APMAC,"xx:xx:xx:xx:xx:xx"
OK

=== CONFIGURACIÓN COMPLETADA ===
```

#### Paso 3: Verificar Red WiFi

En tu dispositivo móvil:
1. Buscar redes WiFi disponibles
2. Debe aparecer **"RemoteXY"**
3. **NO conectarse aún**, solo verificar que existe

### 3. Configuración de Firmware Antiguo

Si tu ESP8266 tiene firmware muy antiguo (AT version 1.1.0.0), algunos comandos pueden variar:

#### Comandos Alternativos:
```cpp
// Si AT+CWMODE_DEF no funciona, usar:
ESP.println("AT+CWMODE=2");

// Si AT+CWSAP_DEF no funciona, usar:
ESP.println("AT+CWSAP=\"RemoteXY\",\"12345678\",5,3");

// Para firmware muy antiguo:
ESP.println("AT+CIOBAUD=9600");  // En lugar de UART commands
```

#### Solución de Problemas AT:

| Problema | Solución |
|----------|----------|
| `AT` no responde | Verificar conexiones TX/RX, probar otros baudios (74880, 115200) |
| `ERROR` en comandos | Usar comandos alternativos sin `_DEF` |
| Configuración se pierde | El firmware no soporta guardado permanente |
| `+CIFSR` no muestra IP | Repetir secuencia de comandos |

### 4. Configuración Permanente

⚠️ **NOTA**: Una vez configurado correctamente, el ESP8266 mantendrá la configuración. Si se pierde, repetir el Paso 1.

### 5. Configuración RemoteXY

Después de configurar el ESP8266:

1. Red WiFi creada: **"RemoteXY"**
2. Password: **"12345678"**
3. IP del Arduino: **"192.168.4.1"**
4. Puerto: **6377**
5. Password de acceso: **"210424"**

### 6. Preparar MicroSD para DFPlayer

Formatear MicroSD en FAT32 y agregar archivos:
```
001.mp3 - Canción energética
002.mp3 - Canción calmante  
003.mp3 - Sonidos de naturaleza
004.mp3 - Melodía suave
```

## 🚀 Uso del Sistema

### Modos de Operación

| Modo | Descripción | Audio | LEDs | Volumen |
|------|-------------|-------|------|---------|
| 🔴 **APAGADO** | Sistema en standby | Silencio | Apagados | - |
| 😴 **DORMIR** | Canciones relajantes | Canción 1 | Rojo suave | Alto (25/30) |
| 👶 **CAMBIO** | Calmar al bebé | Volumen medio | Naranja | Medio (15/30) |
| 🎛️ **MANUAL** | Control total | Usuario decide | Control RGB | Variable |
| 🚨 **ALERTA** | Modo emergencia | Silencio | Blanco | - |

### Controles en Modo Manual

- **▶️ Play/Stop**: Reproducir/pausar audio
- **⏭️ Next/⏮️ Prev**: Cambiar canciones
- **🔊 Volumen**: Control deslizante -100 a +100
- **🌈 RGB**: Control de color de LEDs

### Monitoreo en Tiempo Real

- **🌡️ Temperatura**: Display en °C
- **💧 Humedad**: Display en %
- **👁️ Movimiento**: LED indicador ON/OFF
- **📊 Llanto**: Barra 0-100% calibrada

## ⚙️ Configuración Avanzada

### Calibración del Micrófono

El sistema incluye auto-calibración, pero puedes ajustar manualmente:

```cpp
// En leerSensores()
static int nivelBase = 305;  // Ajustar según tu ambiente

// En actualizarDisplays()  
RemoteXY.cry = map(nivelSonido, 0, 336, 0, 100);  // Ajustar sensibilidad
```

### Personalizar Volúmenes

```cpp
// En configurarModo()
case 1: currentVolume = 25;  // DORMIR (0-30)
case 2: currentVolume = 15;  // CAMBIO (0-30)
```

### Personalizar Colores LED

```cpp
// En configurarModo()
fill_solid(leds, NUM_LEDS, CRGB(255, 80, 0));  // Naranja rojizo
```

## 📱 App RemoteXY

### Instalación
- **Android**: [Google Play Store](https://play.google.com/store/apps/details?id=com.shemanin.remotexy)
- **iOS**: [App Store](https://apps.apple.com/app/remotexy/id1447060284)

### Conexión
1. Conectar dispositivo a red **"RemoteXY"**
2. Abrir app RemoteXY
3. Debería conectar automáticamente
4. Si no conecta: Agregar manual IP `192.168.4.1:6377`

## 🔧 Solución de Problemas

### ESP8266 no configurado
- ⚠️ **CRÍTICO**: Ejecutar configuración AT del Paso 2
- ✅ Verificar que aparece red "RemoteXY" 
- ✅ Usar comandos alternativos para firmware antiguo

### App no conecta
- ✅ Verificar conexión a red "RemoteXY"
- ✅ Resetear Arduino
- ✅ Configurar IP manual: 192.168.4.1

### Micrófono no responde
- ✅ Verificar conexión Pin A0
- ✅ Ajustar valor `nivelBase` en código
- ✅ Verificar alimentación 5V

### DFPlayer sin sonido
- ✅ Verificar MicroSD con archivos MP3
- ✅ Verificar conexiones pines 8,9
- ✅ Verificar parlante conectado

### LEDs no funcionan
- ✅ Verificar alimentación 5V suficiente
- ✅ Verificar conexión Pin 6
- ✅ Verificar tipo de LED (WS2812/NeoPixel)

## 📊 Estadísticas del Código

Para configuración manual del ESP8266:

```bash
# Comandos básicos
AT                              # Verificar conexión
AT+GMR                          # Ver versión firmware
AT+RST                          # Reset módulo

# Configuración WiFi
AT+CWMODE_DEF=2                 # Solo Access Point (permanente)
AT+CWSAP_DEF="RemoteXY","12345678",5,3  # Crear AP (permanente)

# Configuración servidor
AT+CIPMUX=1                     # Múltiples conexiones
AT+CIPSERVER=1,6377             # Servidor TCP puerto 6377
AT+CIFSR                        # Ver IP asignada

# Para firmware antiguo (AT 1.1.0.0)
AT+CWMODE=2                     # Modo AP (temporal)
AT+CWSAP="RemoteXY","12345678",5,3      # Crear AP (temporal)
AT+CIOBAUD=9600                 # Configurar baudios
```

- **Tamaño**: ~21KB (66% de Arduino Uno)
- **RAM**: ~1.1KB (54% disponible)  
- **Pines utilizados**: 8 de 14 disponibles
- **Velocidad**: Loop cada 10ms + sensores cada 500ms

## 🤝 Contribuir

1. Fork del repositorio
2. Crear rama para nueva característica
3. Commit de cambios
4. Push a la rama
5. Crear Pull Request


## 👨‍💻 Autor
- GitHub: [@LauraRangel](https://github.com/LauraRangel)

## 🙏 Agradecimientos

- **RemoteXY** - Por la excelente librería de control remoto
- **DFRobot** - Por la librería DFPlayer Mini
- **FastLED** - Por los efectos de LEDs
- **Adafruit** - Por las librerías de sensores

## 📈 Versiones

### v1.0.0 (Actual)
- ✅ Sistema completo funcional
- ✅ 5 modos de operación
- ✅ Control remoto WiFi
- ✅ Monitoreo multi-sensor
- ✅ Audio automático
- ✅ LEDs con efectos

### Próximas versiones
- 🔄 Notificaciones push
- 🔄 Almacenamiento de datos
- 🔄 Control por voz
- 🔄 Modo nocturno automático

---

⭐ **¡Si te gusta este proyecto, dale una estrella!** ⭐
