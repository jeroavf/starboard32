# Okudagram

Interface LCARS (Star Trek TNG) para o TTGO T-Display, exibindo dados reais do ESP32 como sistemas de uma nave estelar.

![LCARS](https://img.shields.io/badge/LCARS-47-FF9900?style=flat-square&labelColor=000)
![ESP32](https://img.shields.io/badge/ESP32-T--Display-blue?style=flat-square)
![Arduino](https://img.shields.io/badge/Arduino-CLI-00979D?style=flat-square)

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

## Estrutura

```
okudagram/
  okudagram/          Sketch Arduino
    okudagram.ino     Loop principal e maquina de estados
    config.h          Constantes (pinos, timings, layout)
    colors.h          Paleta LCARS em RGB565
    lcars_frame.cpp   Chrome visual compartilhado
    screen_eng.cpp    Tela Engineering
    screen_tac.cpp    Tela Tactical
    screen_ops.cpp    Tela Operations
    screen_sci.cpp    Tela Science
    data_source.cpp   Leitura de telemetria do ESP32
    wifi_setup.cpp    WiFiManager
    ntp_sync.cpp      Sincronizacao NTP + stardate
    buttons.cpp       Navegacao por botoes
  demo/
    index.html        Mockup HTML da interface
```

## Licenca

MIT
