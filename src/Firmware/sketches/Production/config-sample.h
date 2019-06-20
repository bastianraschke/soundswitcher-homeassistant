/*
 * Configuration
 */

#define DEBUG_LEVEL                    1

// Generate a random id with:
// $ echo -n "soundswitcher_"; head /dev/urandom | tr -dc A-Z0-9 | head -c 8; echo ""
#define SOUNDSWITCHER_NODE_ID          "soundswitcher_AAAABBBB"

#define WIFI_SSID                      ""
#define WIFI_PASSWORD                  ""

#define MQTT_CLIENTID                  SOUNDSWITCHER_NODE_ID

// Put the host/IPv4 address of your MQTT broker here
#define MQTT_SERVER                    ""

// Use the SHA1 fingerprint of the server certificate (NOT the CA certificate) in the following format:
#define MQTT_SERVER_TLS_FINGERPRINT    "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX"

#define MQTT_PORT                      8883
#define MQTT_USERNAME                  SOUNDSWITCHER_NODE_ID
#define MQTT_PASSWORD                  ""

#define MQTT_CHANNEL_STATE             "/soundswitcher/api/1/id/AAAABBBB/state/"
#define MQTT_CHANNEL_COMMAND           "/soundswitcher/api/1/id/AAAABBBB/command/"

// Uncomment if on the board is an onboard LED
#define PIN_STATUSLED                  LED_BUILTIN

#define PIN_CHANNEL_1                  D1
#define PIN_CHANNEL_2                  D2
#define PIN_CHANNEL_3                  D3
#define PIN_CHANNEL_4                  D4
