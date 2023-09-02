# MQTT Sound Switcher (based on ESP8266 for Home Assistant)

## Features

- Sound channel switching firmware for ESP8266 (e.g. Wemos D1 Mini)
- Easy integration to Home Assistant
- Encrypted MQTT communication with TLS 1.1

## Project application

Here you see the firmware used on a Wemos D1 Mini with the audio input channel switcher:

<img alt="MQTT audio input channel switcher" src="https://github.com/bastianraschke/soundswitcher-homeassistant/blob/master/projectcover.jpg" width="650">

## Configuration

The firmware must be configured before flashing to ESP8266. Rename `src/Firmware/sketches/Production/config-sample.h` to `src/Firmware/sketches/Production/config.h` and change the values like desired.

### Flash settings

* **Arduino IDE:** 2.1.1
* **Platform:** esp8266 2.4.2
* **Board:** LOLIN (WEMOS) D1 R2 & mini
* **Flash Size**: 4M (1M SPIFFS)
* **Debug Port**: Disabled
* **Debug Level**: None
* **IwIP Variant**: v2 Lower Memory
* **VTables**: Flash
* **CPU Frequency**: 80 MHz
* **Erase Flash**: Only Sketch

## Example configuration for Home Assistant

Tested with Home Assistant 2023.8.2.

The example block must be added to the `mqtt` block of your configuration.

    - select:
        name: "Soundswitcher"
        state_topic: "/soundswitcher/api/1/id/AAAABBBB/state/"
        command_topic: "/soundswitcher/api/1/id/AAAABBBB/command/"
        options:
          - "1"
          - "2"
          - "3"
          - "4"
        icon: mdi:speaker

## Further information

The project is [fully documentated](https://sicherheitskritisch.de/2019/06/smart-mqtt-cinch-audio-channel-switcher-for-home-assistant-integration-en/) on my blog [Sicherheitskritisch](https://sicherheitskritisch.de).
