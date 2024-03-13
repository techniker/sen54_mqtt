# ESP8266 SEN54 MQTT Publisher

This project is a simple ESP8266 (Wemos D1 Mini) program designed to read environmental data from a Sensirion SEN54 sensor and publish it to specified MQTT endpoints. It's ideal for integrating with home automation systems or environmental monitoring platforms.

## Features

- Reads data from Sensirion SEN54 sensor.
- Publishes data to MQTT endpoints.
- Configurable reading interval with offset to avoid conflicts in multi-sensor setups.
- LED feedback during sensor data transmission.

## Prerequisites

- ESP8266 Wemos D1 Mini or compatible ESP8266 module
- Sensirion SEN54 sensor (IKEA Vindstyrka)
- WiFi network
- MQTT Broker (Mosquitto)

## Hardware Setup

1. Connect the Sensirion SEN54 (use test-pads on Vindstyrka PCB under display flat-flex ribbon cable) sensor to the ESP8266 via the I2C interface PINs: (D1/20/GPIO5=SCL;D2/19/GPIO4=SDA).

## Software Setup

### Configuration

Before uploading the code to your ESP8266, you need to configure the following:

- **WiFi Settings:** Enter your WiFi SSID and password in the provided variables.
- **MQTT Settings:** Specify your MQTT broker's address, port, user, and password.

### Dependencies

This project requires the following Arduino libraries:

- `ESP8266WiFi`
- `PubSubClient`
- `Wire`
- `SensirionI2CSen5x`

### Compilation and Upload

Using the Arduino IDE or PlatformIO, compile the sketch and upload it to your ESP8266 device.

## Usage

Once powered, the ESP8266 connects to the configured WiFi network and begins reading data from the SEN54 sensor at specified intervals. The sensor data is published to the MQTT topics according to the set configuration.

## MQTT Topics

The program publishes data to the following MQTT topics:

- `/vindstyrka/temp` - Ambient Temperature
- `/vindstyrka/humidity` - Ambient Humidity
- `/vindstyrka/pm1p0` - PM1.0 Mass Concentration
- `/vindstyrka/pm2p5` - PM2.5 Mass Concentration
- `/vindstyrka/pm4p0` - PM4.0 Mass Concentration
- `/vindstyrka/pm10p0` - PM10 Mass Concentration
- `/vindstyrka/vocIndex` - VOC Index
- `/vindstyrka/noxIndex` - NOx Index (no values)

## Contributions

Contributions are welcome! If you have improvements or bug fixes, please feel free to submit a pull request or open an issue.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

