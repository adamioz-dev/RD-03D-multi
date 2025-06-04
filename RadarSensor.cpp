#include "RadarSensor.h"

// constructor
RadarSensor::RadarSensor(HardwareSerial *serial, uint8_t rx_pin, uint8_t tx_pin, bool multi_target_mode)
{
  radarSerial = serial;
  rx = rx_pin;
  tx = tx_pin;
  if (multi_target_mode == true)
  {
    multi_target = true;
  }
  else
  {
    multi_target = false;
  }
}

void RadarSensor::begin(unsigned long baud) {
  
  uint8_t mode_commands[2][12] = {// Command for Single-Target Detection 
                                  {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x80, 0x00, 0x04, 0x03, 0x02, 0x01},
                                  // Command for Multi-Target Detection
                                  {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x90, 0x00, 0x04, 0x03, 0x02, 0x01}};
  radarSerial->begin(baud, SERIAL_8N1, rx, tx);

  // set operation mode
  if (multi_target)
  {
    radarSerial->write(mode_commands[1], sizeof(mode_commands[1]));
  }
  else
  {
    radarSerial->write(mode_commands[0], sizeof(mode_commands[0]));
  }
  delay(200);
  radarSerial->flush();
}

// Parser state-machine for UART data
bool RadarSensor::update() {
  static uint8_t buffer[30]; 
  static size_t index = 0;
  static enum {HEAD_BYTE_0, HEAD_BYTE_1, HEAD_BYTE_2, HEAD_BYTE_3, RECEIVE_DATA_FRAME} state = HEAD_BYTE_0;
  bool data_updated = false;

  while (radarSerial->available()) {
    byte byteIn = radarSerial->read();

    switch(state) {
      case HEAD_BYTE_0:
        if(byteIn == 0xAA) state = HEAD_BYTE_1;
        break;
      case HEAD_BYTE_1:
        if(byteIn == 0xFF) state = HEAD_BYTE_2;
        else state = HEAD_BYTE_0;
        break;
      case HEAD_BYTE_2:
        if(byteIn == 0x03) state = HEAD_BYTE_3;
        else state = HEAD_BYTE_0;
        break;
      case HEAD_BYTE_3:
        if(byteIn == 0x00) {
          index = 0;
          state = RECEIVE_DATA_FRAME;
        } else state = HEAD_BYTE_0;
        break;
      case RECEIVE_DATA_FRAME:
        buffer[index++] = byteIn;
        if(index >= 26) { // 24 bytes data + 2 tail bytes
          if(buffer[24] == 0x55 && buffer[25] == 0xCC) {
            data_updated = parseData(buffer, 24);
          }
          state = HEAD_BYTE_0;
          index = 0;
        }
        break;
    }
  }
  return data_updated;
}

bool RadarSensor::parseData(const uint8_t *buf, size_t len) {
  if(len != 24)
    return false;

  // Parse 8 bytes for each target
  int raw_x[3] = {              
                  (buf[0] | (buf[1] << 8)),
                  (buf[8] | (buf[9] << 8)), 
                  (buf[16] | (buf[17] << 8))
                };
  int raw_y[3] = {              
                  (buf[2] | (buf[3] << 8)),
                  (buf[10] | (buf[11] << 8)),
                  (buf[18] | (buf[19] << 8))
                };
  int raw_speed[3] = {          
                  (buf[4] | (buf[5] << 8)), 
                  (buf[12] | (buf[13] << 8)),
                  (buf[20] | (buf[21] << 8))
                  };
  int raw_pixel_dist[3] = {     
                  (buf[6] | (buf[7] << 8)),
                  (buf[14] | (buf[15] << 8)),
                  (buf[22] | (buf[23] << 8))
                };

  /* Process all targets */
  for (int i = 0; i < 3; i++)
  {
    targets[i].detected = !(raw_x[i] == 0 && raw_y[i] == 0 && raw_speed[i] == 0 && raw_pixel_dist[i] == 0);

    // correctly parse signed values
    targets[i].x = ((raw_x[i] & 0x8000) ? 1 : -1) * (raw_x[i] & 0x7FFF);
    targets[i].y = ((raw_y[i] & 0x8000) ? 1 : -1) * (raw_y[i] & 0x7FFF);
    targets[i].speed = ((raw_speed[i] & 0x8000) ? 1 : -1) * (raw_speed[i] & 0x7FFF);

    if (targets[i].detected) {
      targets[i].distance = sqrt(targets[i].x * targets[i].x + targets[i].y * targets[i].y);
      
      // angle calculation (convert radians to degrees, then flip)
      float angleRad = atan2(targets[i].y, targets[i].x) - (PI / 2);
      float angleDeg = angleRad * (180.0 / PI);
      targets[i].angle = -angleDeg; // align angle with x measurement positive / negative sign
    } else {
      /* Set distance and angle to 0 when target not detected */
      targets[i].distance = 0.0;
      targets[i].angle = 0.0;
    }
  }
  return true;
}

RadarTarget RadarSensor::getTarget(uint8_t index) {
  /* Validate input */
  if(index>2)
  {
    return targets[0];
  }
  return targets[index];
}