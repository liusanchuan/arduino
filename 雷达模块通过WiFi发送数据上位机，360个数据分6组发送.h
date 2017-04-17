//雷达模块通过WiFi发送数据上位机，360个数据分6组发送
/* 雷达接线
	motor：3
	电机正负极
	rx tx 接串口
	正负电源；
	
		
 */

#include <RPLidar.h>
#include <SoftwareSerial.h>
RPLidar lidar;
SoftwareSerial mySerial(8,9);
int lidar_data[360];

int lidar_data_count=0;
#define RPLIDAR_MOTOR 3
void wifi1()//WiFi连接子程序
{
  mySerial.println("AT+RST");
  delay(500);
    mySerial.println("AT+CWMODE=3");
    delay(500);
    mySerial.println("AT+CWJAP=\"AAA\",\"1234567890\"");
    delay(500);
    for(int i=1;i<10;i++)
  {
        mySerial.println("AT+CIPSTART=\"TCP\",\"192.168.155.1\",8080");
        delay(1000);
  }
}
void wifisend(){
	for(int i=0;i<6;i=i+1){
		String send_data_60="["+(String)i+":";
		for(int j=i*60;j<i*60+60;j++){
			send_data_60+=(String)lidar_data[j]+",";
		}
		send_data_60+="END]";
		mySerial.print("AT+CIPSEND=");
		mySerial.println(send_data_60.length());
		mySerial.println(send_data_60);
		delay(500);
		// while(mySerial.read()>=0){
		 // mySerial.read();
		// }
	}	
}
void Direction(){
	int Sum_4dirc[4];
	for(int i=0;i<4;i++){
		int querter=i*90+350;
		for(int k=0;k<20;k++){
			Sum_4dirc[i]+=lidar_data[querter+k];
		}
	}
	if(Sum_4dirc[0]/20<20){
		if(Sum_4dirc[3]/20>20){
			Serial.println("turn left");
		}else if(Sum_4dirc[2]/20>20){
			Serial.println("turn back");
		}else if(Sum_4dirc[1]/20>20){
			Serial.println("turn right");
		}else{
			Serial.println("stop");
		}
	}
}
void setup() {
	mySerial.begin(9600);
  // put your setup code here, to run once:
  Serial.begin(115200);
  lidar.begin(Serial);
  wifi1();
  pinMode(RPLIDAR_MOTOR,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 if(IS_OK(lidar.waitPoint()))
 {
  float distance=lidar.getCurrentPoint().distance;
  int angle=lidar.getCurrentPoint().angle;
	lidar_data[angle]=distance;
	  lidar_data_count++;
	  // Serial.println(lidar_data_count);
	  if(lidar_data_count>360){
		lidar_data_count=0;
		Direction();
		// wifisend();   
	  }
  
//  
//  Serial.print("distance");
//  Serial.println(distance);
//   Serial.print("angle");
//  Serial.println(angle);
 }
 else
 {
  analogWrite(RPLIDAR_MOTOR,0);
  rplidar_response_device_info_t info;
  if(IS_OK(lidar.getDeviceInfo(info,100)))
  {
    lidar.startScan();
    analogWrite(RPLIDAR_MOTOR,255);
    delay(200);
  }
 }
}
