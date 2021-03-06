#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//gps data been published as Float32 to the topic gpsArray
#include <ros.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/Empty.h>

ros::NodeHandle  nh;

////ROS global vars
std_msgs::Pose gpsval;
ros::Publisher gps_chatter("gpsArray", &gpsval);
//ROS global vars


static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  nh.initNode();  
  nh.advertise(gps_chatter);
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
   nh.spinOnce(); 
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    gpsval.data.x = gps.location.lat();    
    gpsval.data.y = gps.location.lng();
    gps_chatter.publish(&gpsval);   
    //gps.location.lng()
  }
 
}
