#include "RadarSensor.h"


#define RADAR_RX_PIN 16
#define RADAR_TX_PIN 17
#define RADAR_BAUD_RATE 256000

#define MULTI_MODE 1

#if (MULTI_MODE == 1)
bool multi_mode = true;
#else
bool multi_mode = false;
#endif

#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE 
#define DEBUG_PRINT(x)    Serial.print(x)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTLN(x) 
#endif 

enum
{
  TARGET_0 = 0,
  TARGET_1,
  TARGET_2
};

/* Create radar object */
RadarSensor radar(&Serial1,RADAR_RX_PIN,RADAR_TX_PIN, multi_mode);

void setup() {
  Serial.begin(115200);
  radar.begin(RADAR_BAUD_RATE);
  DEBUG_PRINTLN("Radar Sensor Started");
}

void loop() {
  RadarTarget tgt[3];

  if(radar.update()) {
    tgt[TARGET_0] = radar.getTarget(TARGET_0);
    tgt[TARGET_1] = radar.getTarget(TARGET_1);
    tgt[TARGET_2] = radar.getTarget(TARGET_2);
    #ifdef DEBUG_ENABLE
    if ((tgt[TARGET_0].detected) || (tgt[TARGET_1].detected) || (tgt[TARGET_2].detected))
    {
      if ((tgt[TARGET_0].detected)){
        DEBUG_PRINT("X0 (mm): "); DEBUG_PRINT(tgt[TARGET_0].x);
        DEBUG_PRINT("\t Y0 (mm): "); DEBUG_PRINT(tgt[TARGET_0].y);
        DEBUG_PRINT("\t D0 (mm): "); DEBUG_PRINT(tgt[TARGET_0].distance);
        DEBUG_PRINT("\t A0 (deg): "); DEBUG_PRINT(tgt[TARGET_0].angle);
        DEBUG_PRINT("\t S0 (cms): "); DEBUG_PRINTLN(tgt[TARGET_0].speed);
      }
      else
      {
        DEBUG_PRINTLN();
      }
      #if (MULTI_MODE == 1)
      if ((tgt[TARGET_1].detected)){
        DEBUG_PRINT("X1 (mm): "); DEBUG_PRINT(tgt[TARGET_1].x);
        DEBUG_PRINT("\t Y1 (mm): "); DEBUG_PRINT(tgt[TARGET_1].y);
        DEBUG_PRINT("\t D1 (mm): "); DEBUG_PRINT(tgt[TARGET_1].distance);
        DEBUG_PRINT("\t A1 (deg): "); DEBUG_PRINT(tgt[TARGET_1].angle);
        DEBUG_PRINT("\t S1 (cms): "); DEBUG_PRINTLN(tgt[TARGET_1].speed);
      }
      else
      {
        DEBUG_PRINTLN();
      }
      if ((tgt[TARGET_2].detected)){
        DEBUG_PRINT("X2 (mm): "); DEBUG_PRINT(tgt[TARGET_2].x);
        DEBUG_PRINT("\t Y2 (mm): "); DEBUG_PRINT(tgt[TARGET_2].y);
        DEBUG_PRINT("\t D2 (mm): "); DEBUG_PRINT(tgt[TARGET_2].distance);
        DEBUG_PRINT("\t A2 (deg): "); DEBUG_PRINT(tgt[TARGET_2].angle);
        DEBUG_PRINT("\t S2 (cms): "); DEBUG_PRINTLN(tgt[TARGET_2].speed);
      }
      else
      {
        DEBUG_PRINTLN();
      }
      #endif
    }
    else
    {
      DEBUG_PRINTLN();
      DEBUG_PRINTLN();
      DEBUG_PRINTLN();
    }
    DEBUG_PRINTLN("-------------------------");
    #endif
    
    /* Run your code here when the sensor comunicates */  

    delay(100);  
    }
    else
    {
      DEBUG_PRINT("No data\n");
      delay(1000);
    }
}