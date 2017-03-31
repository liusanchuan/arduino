#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
uint8_t id;
uint8_t getFingerprintEnroll();
uint8_t getFingerprintID();
SoftwareSerial mySerial(52, 53);
// SoftwareSerial Serial1(47,46);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#include <Keypad.h>
#include <MCUFRIEND_kbv.h>
#include <UTFTGLUE.h>
UTFTGLUE myGLCD(0x9327, A2, A1, A3, A4, A0);
extern uint8_t SmallFont[];
#if !defined(SmallFont)
extern uint8_t SmallFont[];    //.kbv GLUE defines as GFXFont ref
#endif
const byte ROWS = 4; // 4 Rows
const byte COLS = 3; // 3 Columns
char keys[ROWS][COLS] = { // 定義 Keypad 的按鍵
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {28, 27, 26, 25};
byte colPins[COLS] = {24, 23, 22};
Keypad keypad=Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int ii=0;
bool TF=false;//用于判断是否清除的变量
bool TH=false;//用于判断是否是充值操作
bool TE=true;//用于判断在刷卡情况下指纹是否采集成功
bool TL=false;//用于判断是否刷指纹了
uint8_t zid=19;//注册时的id号
uint8_t cid=0;
double jine;
String cmoney,Name;
void key(int a[])
{
  int j=0;
  char key=keypad.getKey();
  while(1){
    key=keypad.getKey();
    if(key!=NO_KEY){
    if(key=='#')
    {
      for(j=0;j<=30;j++)
      {
        key=keypad.getKey();
        if(key=='#')//调用清平子程序//
        {
          TF=true;
          LCD(4);
          delay(1000);
          LCD(3);
          return;
        }
        delay(10);
      }
      return;
    }
    else
      if(ii>=7)//调用提示数字过大子程序//
      {
        LCD(2);
        delay(1000);
        LCD(3);
        return;
      }
      else
        {
        if(key!='*')
        {
          a[ii]=int(key)-48;
         String cc=(String)key;
          if(ii==0)
            LCD(16);
          dayin(3,cc);//调用显示子程序//
          ii++;
        }
        else
        {
          a[ii]=-1;
          dayin(3,".");
          ii++;
        }
        }
    }
  }
}
void LCD(int i)
{
  switch(i)
  {
  case 1://初始化屏幕
    {
        myGLCD.clrScr();
        myGLCD.setColor(255, 255, 255);
        myGLCD.fillRect(0, 0, 399, 399);
        myGLCD.setColor(0, 0, 255);
        myGLCD.fillRect(10, 20, 389, 60);
        myGLCD.setColor(0, 250, 0);
        myGLCD.setBackColor(0, 0, 255);
        myGLCD.setTextSize(2.8);
        myGLCD.print("welcome to use our product", 40, 30);
        myGLCD.setColor(255, 0, 0);
        myGLCD.fillRect(10, 180, 389, 220);
        myGLCD.setColor(64, 64, 64);
        myGLCD.fillRect(10, 60, 389, 180);
        myGLCD.setColor(0, 250, 0);
        myGLCD.setBackColor(64, 64, 64);
        myGLCD.setTextSize(3);
        myGLCD.print("money:", 20, 70);
        myGLCD.setColor(0, 250, 0);
        myGLCD.setBackColor(64, 64, 64);
        myGLCD.setTextSize(3);
        myGLCD.print("remain:", 15, 150);
        myGLCD.setColor(0, 250, 0);
        myGLCD.setBackColor(64, 64, 64);
        myGLCD.setTextSize(3);
        myGLCD.print("name:", 30, 110);
        break;
    }
  case 2://字数超过7时提示语
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(250, 0, 0);
            myGLCD.setTextSize(2.8);
            myGLCD.print("The number is to big", 80, 190 );
            break;
    }
  case 3://刷中间屏幕
    {
        myGLCD.setColor(255, 0, 0);
      myGLCD.fillRect(10, 180, 389, 220);
      myGLCD.setColor(64, 64, 64);
      myGLCD.fillRect(135, 60, 389, 180);
      break;
    }
  case 4://清除提示语
     {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(250, 0, 0);
            myGLCD.setTextSize(3);
            myGLCD.print("clear!", 170 , 190 );
            break;
     }
  case 5://提示刷卡操作
    {
      myGLCD.setColor(0, 250, 0);
      myGLCD.setBackColor(250, 0, 0);
      myGLCD.setTextSize(2.8);
      myGLCD.print("please post the cord", 80, 190 );
      break;
    }
  case 6://提示指纹输入错误
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(250, 0, 0);
            myGLCD.setTextSize(2.8);
            myGLCD.print("The finger is rong!!", 80, 190 );
            break;
    }
  case 7://提示指纹输入正确
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(250, 0, 0);
            myGLCD.setTextSize(2.8);
            myGLCD.print("The finger is right!", 80, 190 );
            break;
    }
  case 8://刷下边条幅
    {
            myGLCD.setColor(255, 0, 0);
            myGLCD.fillRect(10, 180, 389, 220);
            break;
    }
  case 9://注册时提示放入指纹
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(250, 0, 0);
            myGLCD.setTextSize(2.8);
            myGLCD.print("please put your finger", 80, 190 );
            break;
    }
  case 10://注册时提示图像已采集
    {
            myGLCD.setColor(255, 0, 0);
            myGLCD.fillRect(10, 180, 389, 220);
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(64, 64, 64);
            myGLCD.setTextSize(2);
            myGLCD.print("Image converted", 120, 190);
            break;
    }
  case 11://注册时提示再次放入指纹
    {
            myGLCD.setColor(255, 0, 0);
            myGLCD.fillRect(10, 180, 389, 220);
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(250, 0, 0);
            myGLCD.setTextSize(2.8);
            myGLCD.print("Rmove your finger", 80, 190 );
            break;
    }
  case 13://注册时提示指纹以保存
    {
      myGLCD.setColor(255, 0, 0);
      myGLCD.fillRect(10, 180, 389, 220);
      myGLCD.setColor(0, 250, 0);
      myGLCD.setBackColor(250, 0, 0);
      myGLCD.setTextSize(2.8);
      myGLCD.print("Stored!", 180, 190 );
      break;
    }
  case 14://打印ID号时的定位符
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(64, 64, 64);
            myGLCD.setTextSize(3);
            myGLCD.print("", 150, 110);
            break;
    }
  case 15://打印余额时的定位符
    {
             myGLCD.setColor(0, 250, 0);
             myGLCD.setBackColor(64, 64, 64);
             myGLCD.setTextSize(3);
             myGLCD.print("", 150, 150);
             break;
    }
  case 16://打印输入金额时的定位符
    {
             myGLCD.print(" ", 120, 70);
             break;
    }
  case 17://提示再次放入指纹，注册时
    {
            myGLCD.setColor(255, 0, 0);
      myGLCD.fillRect(10, 180, 389, 220);
          myGLCD.setColor(0, 250, 0);
          myGLCD.setBackColor(250, 0, 0);
      myGLCD.setTextSize(2.8);
      myGLCD.print("please put finger again", 80, 190 );
      break;
    }
  }
}
void dayin(int ww,String yy)
{
  switch (ww)
  {
  case 1://打印ID号
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(64, 64, 64);
            myGLCD.setTextSize(3);
            myGLCD.print(yy, 150, 110);
            break;
    }
  case 2://打印余额
    {
           myGLCD.setColor(0, 250, 0);
           myGLCD.setBackColor(64, 64, 64);
           myGLCD.setTextSize(3);
           myGLCD.print(yy, 150, 150);
           break;
    }
  case 3://打印输入金额数字和小数点两种情况
    {
            myGLCD.setColor(0, 250, 0);
            myGLCD.setBackColor(64, 64, 64);
            myGLCD.setTextSize(3);
            myGLCD.print(yy, 120, 70);
            break;
    }
  }
}
uint8_t getFingerprintEnroll()
{
  int p=-1;
  LCD(9);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
         case FINGERPRINT_OK:
       break;
     case FINGERPRINT_NOFINGER:
             break;
         case FINGERPRINT_PACKETRECIEVEERR:
             break;
         case FINGERPRINT_IMAGEFAIL:
             break;
         default:
             break;
    }
  }
    p = finger.image2Tz(1);
    switch (p)
  {
    case FINGERPRINT_OK:
        break;
    case FINGERPRINT_IMAGEMESS:
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        return p;
    case FINGERPRINT_FEATUREFAIL:
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        return p;
    default:
        return p;
  }
  LCD(10);
  delay(2000);
  LCD(11);
  delay(3000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
  {
        p = finger.getImage();
  }
    p = -1;
  LCD(17);
    while (p != FINGERPRINT_OK)
  {
        p = finger.getImage();
        switch (p)
    {
         case FINGERPRINT_OK:
             break;
         case FINGERPRINT_NOFINGER:
             break;
         case FINGERPRINT_PACKETRECIEVEERR:
             break;
         case FINGERPRINT_IMAGEFAIL:
             break;
         default:
             break;
    }
  }
  LCD(10);
  delay(2000);
    p = finger.image2Tz(2);
    switch (p)
  {
     case FINGERPRINT_OK:
         break;
     case FINGERPRINT_IMAGEMESS:
         return p;
     case FINGERPRINT_PACKETRECIEVEERR:
         return p;
     case FINGERPRINT_FEATUREFAIL:
         return p;
     case FINGERPRINT_INVALIDIMAGE:
         return p;
     default:
         return p;
  }
    p = finger.createModel();
    if (p == FINGERPRINT_OK);
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    return p;
  }
  else
  {
    return p;
  }
    p = finger.storeModel(zid);
    Serial.println(zid);
    if (p == FINGERPRINT_OK)
  {
    LCD(13);
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
        return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
      return p;
  }
  else
  {
        return p;
  }
}
void wifi1()//WiFi连接子程序
{
  // Serial1.listen();
  Serial1.println("AT+RST");
  delay(500);
    Serial1.println("AT+CWMODE=3");
    delay(500);
    Serial1.println("AT+CWJAP=\"AAA\",\"1234567890\"");
    delay(500);
    for(int i=1;i<10;i++)
  {
        Serial1.println("AT+CIPSTART=\"TCP\",\"192.168.191.1\",8080");
        delay(1000);
  }
}
void wifizhucesend()//WiFi发送数据子程序
{
  // Serial1.listen();
  String zhuc="001#"+(String)zid+"1000%";
  Serial1.print("AT+CIPSEND=");
  Serial1.println(zhuc.length());
  Serial1.println(zhuc);
  delay(1000);
}
void congzhisend()
{
  // Serial1.listen();
  String congz="002#"+(String)cid+"1000%";
  Serial1.print("AT+CIPSEND=");
  Serial1.println(congz.length());
  Serial1.println(congz);
  delay(1000);
}
void wifisend(){
  // Serial1.listen();
 String xiaof="%("+(String)cid+")("+(String)jine+")%";
 Serial1.print("AT+CIPSEND=");
 Serial1.println(xiaof.length());
 Serial1.println(xiaof);
  delay(1000);
}
void setup()
{
    while (!Serial);
    delay(500);
    Serial.begin(9600);//定义波特
    finger.begin(9600);
    Serial1.begin(9600);
    wifi1();
    // mySerial.listen();
    if(finger.verifyPassword())
    Serial.println("right");
    else
    {
      Serial.println("wrong");
      while(1);
    }
    randomSeed(analogRead(5));   //.kbv Due does not like A0
    pinMode(A0, OUTPUT);       //.kbv mcufriend have RD on A0
    digitalWrite(A0, HIGH);
    myGLCD.InitLCD();         // Setup the LCD
    myGLCD.setFont(BigFont);

    //用于初始化显示屏
    int buf[398];
    int x, x2;
    int y, y2;
    int r;
    LCD(1);
}
void loop()
{
  int a[6]={0,0,0,0,0,0};
  ii=0;
 
	if(keypad.getKey()=='#')
       {
       // Serial1.listen();
	   Serial.println("Serial1.listen");
       // while(!Serial1.available());
       delay(1000);
       wifi3();
       LCD(14);
	   Serial.println(Name);
	   Serial.println(cmoney);
       myGLCD.print(Name,150,110);
       LCD(15);
       myGLCD.print(cmoney,150,150);
       delay(2000);
       }
   // L1: LCD(3);
}
double chartoint(int t[])
{
  double sum=0;
  int c=0;
  double xiaoshu=0;
  for(int j=0;t[j]!=-1&&j<ii;j++)
    c=j;
  for (int j=0;j<=c;j++)
  {
    for (int d=0;d<c-j;d++)
    {
      t[j]=t[j]*10;
    }
    sum=sum+t[j];
    }
    for(int j=c+2;j<ii;j++)
  {
        xiaoshu=t[j];
        for(int d=0;d<j-c-1;d++)
    {
      xiaoshu=xiaoshu/10;
    }
    sum = sum + xiaoshu;
    }
    return (sum);
}
uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
     case FINGERPRINT_OK:
         break;
     case FINGERPRINT_NOFINGER:
         return p;
     case FINGERPRINT_PACKETRECIEVEERR:
         return p;
     case FINGERPRINT_IMAGEFAIL:
         return p;
     default:
         return p;
  }
  p = finger.image2Tz();
  switch (p)
  {
     case FINGERPRINT_OK:
         break;
     case FINGERPRINT_IMAGEMESS:
         return p;
     case FINGERPRINT_PACKETRECIEVEERR:
         return p;
     case FINGERPRINT_FEATUREFAIL:
         return p;
     case FINGERPRINT_INVALIDIMAGE:
         return p;
     default:
         return p;
  }
    p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK);
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    return p;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    LCD(6);
//        tone(41, 10000, 2500);// 播放音频  接arduino9号端口，thispitch为播放频率，10为维持时间
    delay(4000);
//    TH = false;这地方需要处理
    TE = false;
    LCD(3);
   return p;
  }
  else
  {
        return p;
  }
    if (p == FINGERPRINT_OK)
  {
        LCD(7);//在这添加输出用户姓名与余额
//    tone(41, 10000, 500);
    delay(4000);// 播放音频  接arduino9号端口，thispitch为播放频率，10为维持时间
    //TE = true;这地方需要修改
    LCD(8);
  }
  cid=finger.fingerID;
  if(TH)
  {
    TH=false;
    congzhisend();
  }
else
{
  wifisend();
  TL=true;
}
}
void wifi3()//WiFi接收数据子程序
{
  char ch;
  cmoney="";
  Name="";
  // Serial1.listen();
  Serial.println("Serial1.listen in wifi3");
  while(1)
  {
	 
    ch=Serial1.read();
	if((int)ch>33&&(int)ch<126){
		Serial.print(ch);Serial.print(' ');
	}
	bool return_flag=false;
    if(ch=='@')
    {
		while(!return_flag){
		Serial.println("enter in ...");
		ch=Serial1.read();
			if((int)ch>33&&(int)ch<126){
				if(ch!='@'){
					Serial.println("i`m return back!:"+ch);
					return_flag=true;				
				}else{
						  ch=Serial1.read();
						  while(ch!=',')
						  {
							  if((int)ch>33&&(int)ch<126)Name+=ch;
							ch=char(Serial1.read());
						  }
						  Serial.println("name is:"+Name);
							if(ch==',')
							{
							  ch=Serial1.read();
							  while(ch!='%'){
								if((int)ch>33&&(int)ch<126)cmoney+=ch;
								ch=char(Serial1.read());
							  };
							  Serial.println("money is :"+cmoney);
							  return_flag=true;
							  return;
							}
				}
			}
		}	
	}
  }
}

