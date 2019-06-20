#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h"

#define FIRMWARE_VERSION  "1.0.0"

WiFiClientSecure secureWifiClient = WiFiClientSecure();
PubSubClient mqttClient = PubSubClient(secureWifiClient, MQTT_SERVER_TLS_FINGERPRINT);

int currentChannel = 0;

/*
 * Setup
 */

void setup() {
    Serial.begin(115200);
    delay(250);

    char buffer[64] = {0};
    sprintf(buffer, "setup(): The node '%s' was powered up.", MQTT_CLIENTID);
    Serial.println();
    Serial.println(buffer);

    #ifdef PIN_STATUSLED
        pinMode(PIN_STATUSLED, OUTPUT);
    #endif

    setupWifi();
    setupSwitches();
    setupMQTT();
}

void setupWifi() {
    Serial.printf("setupWifi(): Connecting to to Wi-Fi access point '%s'...\n", WIFI_SSID);

    // Do not store Wi-Fi config in SDK flash area
    WiFi.persistent(false);

    // Disable auto Wi-Fi access point mode
    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        // Blink 2 times when connecting
        blinkStatusLED(2);

        delay(500);
        Serial.println(F("setupWifi(): Connecting..."));
    }

    Serial.print(F("setupWifi(): Connected to Wi-Fi access point. Obtained IP address: "));
    Serial.println(WiFi.localIP());
}

void blinkStatusLED(const int times) {
    #ifdef PIN_STATUSLED
        for (int i = 0; i < times; i++) {
            // Enable LED
            digitalWrite(PIN_STATUSLED, LOW);
            delay(100);

            // Disable LED
            digitalWrite(PIN_STATUSLED, HIGH);
            delay(100);
        }
    #endif
}

void setupSwitches() {
    Serial.println("setupSwitches(): Setup switches...");

    pinMode(PIN_CHANNEL_1, OUTPUT);
    pinMode(PIN_CHANNEL_2, OUTPUT);
    pinMode(PIN_CHANNEL_3, OUTPUT);
    pinMode(PIN_CHANNEL_4, OUTPUT);

    // Initially switch channel
    currentChannel = 1;
    switchChannel(currentChannel);
}

void setupMQTT() {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(onMessageReceivedCallback);
}

void onMessageReceivedCallback(char* topic, byte* payload, unsigned int length) {
    if (!topic || !payload) {
        Serial.println("onMessageReceivedCallback(): Invalid argument (nullpointer) given!");
    } else {
        char payloadMessage[length + 1];

        for (int i = 0; i < length; i++) {
            payloadMessage[i] = (char) payload[i];
        }

        payloadMessage[length] = '\0';

        Serial.printf("onMessageReceivedCallback(): Received message on channel '%s': %s\n", topic, payloadMessage);

        if (updateValuesAccordingMessage(payloadMessage)) {
            switchChannel(currentChannel);
            publishState();
        } else {
            Serial.println("onMessageReceivedCallback(): The payload could not be parsed!");
        }
    }
}

bool updateValuesAccordingMessage(char* payload) {
    bool wasSuccessfulParsed = true;

    const int channelNumberFromPayload = atoi(payload);

    if (channelNumberFromPayload >= 1 && channelNumberFromPayload <= 4) {
        currentChannel = channelNumberFromPayload;
    } else {
        wasSuccessfulParsed = false;
    }

    return wasSuccessfulParsed;
}

void switchChannel(const int channelNumber) {
    switch (channelNumber) {
        case 1:
            digitalWrite(PIN_CHANNEL_1, HIGH);
            digitalWrite(PIN_CHANNEL_2, LOW);
            digitalWrite(PIN_CHANNEL_3, LOW);
            digitalWrite(PIN_CHANNEL_4, LOW);
            break;
        case 2:
            digitalWrite(PIN_CHANNEL_1, LOW);
            digitalWrite(PIN_CHANNEL_2, HIGH);
            digitalWrite(PIN_CHANNEL_3, LOW);
            digitalWrite(PIN_CHANNEL_4, LOW);
            break;
        case 3:
            digitalWrite(PIN_CHANNEL_1, LOW);
            digitalWrite(PIN_CHANNEL_2, LOW);
            digitalWrite(PIN_CHANNEL_3, HIGH);
            digitalWrite(PIN_CHANNEL_4, LOW);
            break;
        case 4:
            digitalWrite(PIN_CHANNEL_1, LOW);
            digitalWrite(PIN_CHANNEL_2, LOW);
            digitalWrite(PIN_CHANNEL_3, LOW);
            digitalWrite(PIN_CHANNEL_4, HIGH);
            break;
    }
}

void publishState() {
    Serial.printf("publishState(): Publish message on channel '%s': %d\n", MQTT_CHANNEL_STATE, currentChannel);
    mqttClient.publish(MQTT_CHANNEL_STATE, String(currentChannel).c_str(), true);
}

void loop() {
    connectMQTT();
    mqttClient.loop();
}

void connectMQTT() {
    if (mqttClient.connected() == true) {
        return ;
    }

    Serial.printf("connectMQTT(): Connecting to MQTT broker '%s:%i'...\n", MQTT_SERVER, MQTT_PORT);

    while (mqttClient.connected() == false) {
        Serial.println("connectMQTT(): Connecting...");

        if (mqttClient.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD) == true) {
            Serial.println("connectMQTT(): Connected to MQTT broker.");

            // (Re)subscribe on topics
            mqttClient.subscribe(MQTT_CHANNEL_COMMAND);

            // Initially publish current state
            publishState();
        } else {
            Serial.printf("connectMQTT(): Connection failed with error code %i. Try again...\n", mqttClient.state());
            blinkStatusLED(3);
            delay(500);
        }
    }
}
