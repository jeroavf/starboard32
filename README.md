# Okudagram

LCARS interface (Star Trek TNG) for the TTGO T-Display, showing real ESP32 data as starship systems.

![LCARS](https://img.shields.io/badge/LCARS-47-FF9900?style=flat-square&labelColor=000)
![ESP32](https://img.shields.io/badge/ESP32-T--Display-blue?style=flat-square)
![Arduino](https://img.shields.io/badge/Arduino-CLI-00979D?style=flat-square)

> [Leia em Portugues](#portugues)

## What is an Okudagram?

Okudagram is the name given to the computer screen graphics in Star Trek, created by designer **Michael Okuda**. The **LCARS** (Library Computer Access/Retrieval System) visual language is recognized by its rounded blocks in orange, lavender and blue tones on a black background.

This project recreates that aesthetic on a 1.14" TFT display using real microcontroller data.

## Hardware

- **TTGO T-Display v1** (ESP32 + ST7789 135x240px)
- 2 physical buttons (GPIO 0 and GPIO 35)
- USB-C for power and upload

## Screens

The display cycles through 4 screens via buttons:

| Screen | Star Trek Theme | Real Data |
|--------|----------------|-----------|
| **ENG** | Warp Core, Power Output, Antimatter | Heap memory, CPU freq, PSRAM |
| **TAC** | Shields, Comm Status, Signal Analysis | WiFi RSSI, SSID/IP, waveform |
| **OPS** | Ship Status, Stardate, System Alerts | Uptime, NTP time, alerts |
| **SCI** | Temperature, Magnetic Field, Sensor Sweep | Internal temp, scan animation |

## Controls

| Action | Button |
|--------|--------|
| Next screen | Right (GPIO 35) |
| Previous screen | Left (GPIO 0) |
| WiFi reset | Left held 3s |

## Build & Upload

Requires [Arduino CLI](https://arduino.github.io/arduino-cli/) with libraries:

- `TFT_eSPI` 2.5.43
- `WiFiManager` 2.0.17
- ESP32 board core 3.3.6

```bash
# Compile
arduino-cli compile --fqbn esp32:esp32:esp32 okudagram/

# Upload
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 okudagram/

# Serial monitor
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

## First Boot

1. The ESP32 creates a WiFi AP called **"Okudagram-Config"**
2. Connect to it from your phone or laptop
3. Configure your WiFi network in the captive portal
4. The device restarts, syncs NTP and displays the LCARS interface

## Mockup

The file `demo/index.html` contains a web version of the layout for visual reference. Open it in a browser to see the interface at real size (135x240) and scaled 3x.

## Structure

```
okudagram/
  okudagram/          Arduino sketch
    okudagram.ino     Main loop and state machine
    config.h          Constants (pins, timings, layout)
    colors.h          LCARS palette in RGB565
    lcars_frame.cpp   Shared visual chrome
    screen_eng.cpp    Engineering screen
    screen_tac.cpp    Tactical screen
    screen_ops.cpp    Operations screen
    screen_sci.cpp    Science screen
    data_source.cpp   ESP32 telemetry reading
    wifi_setup.cpp    WiFiManager
    ntp_sync.cpp      NTP sync + stardate
    buttons.cpp       Button navigation
  demo/
    index.html        HTML interface mockup
```

## License

MIT

---

<a name="portugues"></a>

# Okudagram (Portugues)

Interface LCARS (Star Trek TNG) para o TTGO T-Display, exibindo dados reais do ESP32 como sistemas de uma nave estelar.

## O que e um Okudagram?

Okudagram e o nome dado aos graficos e displays das telas de computador em Star Trek, criados pelo designer **Michael Okuda**. O sistema visual **LCARS** (Library Computer Access/Retrieval System) e reconhecido pelos blocos arredondados em tons de laranja, lavanda e azul sobre fundo preto.

Este projeto recria essa estetica num display TFT de 1.14" usando dados reais do microcontrolador.

## Hardware

- **TTGO T-Display v1** (ESP32 + ST7789 135x240px)
- 2 botoes fisicos (GPIO 0 e GPIO 35)
- Conexao USB-C para alimentacao e upload

## Telas

O display alterna entre 4 telas via botoes:

| Tela | Tema Star Trek | Dados Reais |
|------|---------------|-------------|
| **ENG** | Warp Core, Power Output, Antimatter | Heap memory, CPU freq, PSRAM |
| **TAC** | Shields, Comm Status, Signal Analysis | WiFi RSSI, SSID/IP, waveform |
| **OPS** | Ship Status, Stardate, System Alerts | Uptime, hora NTP, alertas |
| **SCI** | Temperature, Magnetic Field, Sensor Sweep | Temp interna, scan animation |

## Controles

| Acao | Botao |
|------|-------|
| Proxima tela | Direito (GPIO 35) |
| Tela anterior | Esquerdo (GPIO 0) |
| Reset WiFi | Esquerdo segurado 3s |

## Build e Upload

Requer [Arduino CLI](https://arduino.github.io/arduino-cli/) com as bibliotecas:

- `TFT_eSPI` 2.5.43
- `WiFiManager` 2.0.17
- ESP32 board core 3.3.6

```bash
# Compilar
arduino-cli compile --fqbn esp32:esp32:esp32 okudagram/

# Upload
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 okudagram/

# Monitor serial
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

## Primeiro Boot

1. O ESP32 cria um AP WiFi chamado **"Okudagram-Config"**
2. Conecte-se a esse AP pelo celular ou notebook
3. Configure sua rede WiFi no portal captivo
4. O dispositivo reinicia, sincroniza o NTP e exibe a interface LCARS

## Mockup

O arquivo `demo/index.html` contem uma versao web do layout para referencia visual. Abra no navegador para ver a interface em tamanho real (135x240) e ampliada 3x.

## Licenca

MIT
