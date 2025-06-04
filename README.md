# RD-03D-multi
My unofficial Arduino library for RD-03D mmWave Radar for multiple target detection and tracking, made to use with the ESP32.

The files are based on the files provided by www.electroniclinic.com for single target tracking (see the article here: https://www.electroniclinic.com/rd-03d-mmwave-radar-multi-human-tracking-with-distance-speed-positioning)

**Usage** (see example.cpp):
1) Create the radar object:
   -  RadarSensor radar(&HWSERIAL,RADAR_RX_PIN,RADAR_TX_PIN, MODE);
       - HWSERIAL -> the used HW serial example: Serial1
       - RADAR_RX_PIN -> the previously selected HW serial RX pin connected to the radar
       - RADAR_TX_PIN -> the previously selected HW serial TX pin connected to the radar
       - MODE -> true/false (true for multiple target detection, false for single target detection)
2)  In setup():
    -  radar.begin(RADAR_BAUD_RATE);
        -  RADAR_BAUD_RATE -> the module comunicates at 256000bps
3)  In loop():
    -  RadarTarget target[3]; // create targets
    -  // call update and check state (retuns true if data is received successfully)
    -  if ( radar.update() )  {
       - // get data for each of the 3 available targets (0 to 2)
       - target[0] = radar.getTarget(0);    
       - target[1] = radar.getTarget(1);    
       - target[2] = radar.getTarget(2);   

       - /* Run your code here */
   - }
    
