# Sound Switcher (based on ESP8266 for Home Assistant)

## Configuration

The firmware must be configured before flashing to ESP8266. Rename `src/Firmware/sketches/Production/config-sample.h` to `src/Firmware/sketches/Production/config.h` and change the values like desired.

## Example configuration for Home Assistant

    input_select:
      soundswitcher:
        name: Soundumschalter
        options:
          - 1
          - 2
          - 3
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
