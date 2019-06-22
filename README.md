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

* **Arduino IDE:** 1.8.5
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

    input_select:
      soundswitcher:
        name: Soundswitcher
        options:
          - 1
          - 2
          - 3
          - 4
        initial: 1
        icon: mdi:speaker

    automation:
      - alias: "Get soundswitcher state"
        trigger:
          platform: mqtt
          topic: "/soundswitcher/api/1/id/AAAABBBB/state/"
        action:
          service: input_select.select_option
          data_template:
            entity_id: input_select.soundswitcher
            option: "{{ trigger.payload }}"

      - alias: "Set soundswitcher state"
        trigger:
          platform: state
          entity_id: input_select.soundswitcher
        action:
          service: mqtt.publish
          data:
            topic: "/soundswitcher/api/1/id/AAAABBBB/command/"
            payload_template: "{{ states('input_select.soundswitcher') }}"

## Further information

The project is [fully documentated](https://sicherheitskritisch.de/2019/06/smart-mqtt-cinch-audio-channel-switcher-for-home-assistant-integration-en/) on my blog [Sicherheitskritisch](https://sicherheitskritisch.de).
