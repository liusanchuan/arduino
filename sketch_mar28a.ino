//引用指纹库函数
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
uint8_t id;
uint8_t getFingerprintEnroll();
SoftwareSerial mySerial(52, 53 );
SoftwareSerial mySerial2(47,46);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// 引用 Keypad library
#include <Keypad.h>

//引用显示屏库函数
#include <MCUFRIEND_kbv.h>
#include <UTFTGLUE.h>
UTFTGLUE myGLCD(0x9327, A2, A1, A3, A4, A0);
extern uint8_t SmallFont[];
#if !defined(SmallFont)
extern uint8_t SmallFont[];    //.kbv GLUE defines as GFXFont ref
#endif

// 3x4 Keypad
const byte ROWS = 4; // 4 Rows
const byte COLS = 3; // 3 Columns
char keys[ROWS][COLS] = { // 定義 Keypad 的按鍵
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// 定義 Keypad 連到 Arduino 的接腳
byte rowPins[ROWS] = {28, 27, 26, 25}; // 連到 Keypad 的 4 個 Rows: Row0, Row1, Row2, Row3
byte colPins[COLS] = {24, 23, 22};   // 連到 Keypad 的 3 個 Columns: Column0, Column1, Column2
// 建立 Keypad 物件
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void wifi1()//WiFi连接子程序
{
  mySerial2.listen();
  mySerial2.println("AT+RST");
  delay(500);
    mySerial2.println("AT+CWMODE=3");
    delay(500);
    mySerial2.println("AT+CWJAP=\"CC\",\"99998888\"");
    delay(500);
    for(int i=1;i<10;i++)
  {
        mySerial2.println("AT+CIPSTART=\"TCP\",\"192.168.155.1\",8080");
        delay(1000);
  }
    while(mySerial2.available())
  {
        mySerial2.read();
  }
}
//setup 函数
void setup() {
  while (!Serial);
  delay(500);
  Serial.begin(115200);//定义波特率
  finger.begin(9600);
  mySerial2.begin(9600);
  //检测指纹传感器是否接入
  mySerial.listen();
  if (finger.verifyPassword()) {
  } else {
    while (1);
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
  mySerial2.listen();
  wifi1();
}

//以下是本程序所用到的全局变量
int anX = 0;
int a[6] = {0, 0, 0, 0, 0, 0};
int i = 0; //存储输入值时数组所用到的角标
int c = 0; //记录小数点位置
int f = 0;
int ggg=0;//记录输出的位数
int www=0;//记录id号；
String bb;//由于显示屏只能显示字符串变量，所以定义该字符串变量用于显示按键的输入值
String cc = ".";//用于在显示屏上打印小数点
double jine;   //用于存储键盘输入的金额
bool TE;//用于采集指纹时限制采集次数
bool TH;
int df;
int pp = 0;
int qq = 0;
int zid=9;
String cmoney="",Name="";
bool congzhi=false;
//以下是loop函数
void loop() {

  // 讀取 Keypad 的輸入
  char key = keypad.getKey();

  //当案件输入值超过取值范围时进行提示，并刷屏
  if (i >= 7) {
    LCD(2);
    delay(1000);
    LCD(3);
    i = 0; c = 0; anX = 0; f = 0;
    for (int n = 0; n < 6; n++) {
      a[n] = 0;
    }
  }
  
 bool TF = true;
  if (key == '#') {
    if (a[0] == 0 && a[1] == 0 && a[2] == 1) {
      while(!getFingerprintEnroll());
      zhucesend();
       zid++;
      i = 0; c = 0; anX = 0; f = 0;
    }
    else if(a[0] == 0 && a[1] == 0 && a[2] == 2){
      congzhi=true;
      for(int gdr=0;gdr<=12;gdr++){
        getFingerprintID();
        delay(5);
      }
      i = 0; c = 0; anX = 0; f = 0;
    }
    else{
      for (int z = 0; z < 100; z++) {
      delay(20);
      char k = keypad.getKey();
      //当按下清除键时的操作
      if (k == '#') {
        TF = false;
        LCD(4);
        delay(1000);
        LCD(3);
        //所有变量初始化
        i = 0; c = 0; anX = 0; f = 0;
        for (int n = 0; n < 6; n++) {
          a[n] = 0;
        }
        return;
      }
    }
   

    //执行刷卡操作
    if (TF == false)return;
    LCD(5);
    delay(1500);

    //执行录入指纹操作
    jine = chartoint();
    TE = false;
    TH = true;
    for (int TM = 1; TM < 10; TM++)
    { mySerial.listen();
      getFingerprintID();
      delay(5);
      if (TE) {
        TM = 9;
      }
    }
    Serial.println(1000);
    mySerial2.listen();
    while(!mySerial2.available());
    Serial.println(1000);
  while(mySerial2.available())
  {
    char ch=char(mySerial2.read());
    if(ch=='(')
    {
      ch=mySerial2.read();
      while(ch!=')')
      {
        Name+=ch;
        ch=char(mySerial2.read());
      }
    }
    if(ch=='*')
    {
      ch=mySerial2.read();
      while(ch!='%')
      {
        cmoney+=ch;
        ch=char(mySerial2.read());
      }
    }
  }
//    //从串口读余额和ID好，并显示到显示屏上
//    if (TH && TE) {
//      char Name[10] = {'0', '0', '0', '0', '0'};
//      int ooo = 0;
//      int ppp = 0;
//      char Money[10] = {'0', '0', '0', '0'};
//      cmoney = cmoney - jine;
//      Serial.print(cmoney);
//      //这地方需要更改
//      if (www >= 100) {
//        ooo = 2;
//        for (int o = 2; o >= 0; o--) {
//          Name[o] = www % 10 + 48;
//          www = www / 10;
//        }
//      }
//      else if (www >= 10) {
//        ooo = 1;
//        for (int o = 1; o >= 0; o--) {
//          Name[o] = www % 10 + 48;
//          www = www / 10;
//        }
//      }
//      else {
//        ooo = 0;
//        Name[0] = www + 48;
//      }
//      int chmoney = cmoney;
//      if (chmoney >= 100) {
//        ppp = 2;
//        for (int o = 2; o >= 0; o--) {
//          Money[o] = chmoney % 10 + 48;
//          chmoney = chmoney / 10;
//        }
//      }
//      else if (chmoney >= 10) {
//        ppp = 1;
//        for (int o = 1; o >= 0; o--) {
//          Money[o] = chmoney % 10 + 48;
//          chmoney = chmoney / 10;
//        }
//      }
//      else {
//        ppp = 0;
//        Money[0] = chmoney + 48;
//      }


      //在屏幕上打印余额与ID号
      LCD(14);
//      for (int o = 0; o <= ooo; o++) {
//        String w = (String)Name[o];
        myGLCD.setColor(0, 250, 0);
        myGLCD.setBackColor(64, 64, 64);
        myGLCD.setTextSize(3);
        myGLCD.print(Name, 150, 110);
//      }
      myGLCD.setColor(0, 250, 0);
      myGLCD.setBackColor(64, 64, 64);
      myGLCD.setTextSize(3);
      myGLCD.print("", 150, 150);
//      for (int o = 0; o <= ppp; o++) {
//        String w = (String)Money[o];
        myGLCD.setColor(0, 250, 0);
        myGLCD.setBackColor(64, 64, 64);
        myGLCD.setTextSize(3);
        myGLCD.print(cmoney, 150, 150);
//      }
      delay(2000);
//    }

    //进入循环，刷屏并变量初始化
    myGLCD.setColor(64, 64, 64);
    myGLCD.fillRect(135, 60, 389, 180);
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(10, 180, 389, 220);
    i = 0; c = 0; anX = 0; f = 0;
    for (int n = 0; n < 6; n++) {
      a[n] = 0;
    }
    }
  }

  //执行按键操作并在屏幕上显示
  if (key != NO_KEY && key != '#') {
    // 假如有按鍵被按下，就印出按鍵對應的字元
    if (key != '*') {
      //键盘按入值打印
      bb = (String)key;
      myGLCD.setColor(0, 250, 0);
      myGLCD.setBackColor(64, 64, 64);
      myGLCD.setTextSize(3);
      if (f == 0)
      { myGLCD.print(" ", 120, 70);
        f++;
      }
      myGLCD.print(bb, 120, 70);
      a[i] = int(key) - 48;
      i++;
      if (anX != 1) {
        c = i;
      }
    } else {
      myGLCD.setColor(0, 250, 0);
      myGLCD.setBackColor(64, 64, 64);
      myGLCD.setTextSize(3);
      myGLCD.print(cc, 120, 70);
      c = i;
      anX = 1;
    }
  }
}
  

//金额char类型向整形转化的子函数
double chartoint() {
  double sum = 0;
  double xiaoshu = 0;
  for (int j = c - 1; j >= 0; j--) {
    for (int d = 0; d < c - j - 1; d++) {
      a[j] = a[j] * 10;
    }
    sum = sum + a[j];
  }
  if (anX == 1) {
    for (int j = c; j < i; j++) {
      xiaoshu = a[j];
      for (int d = 0; d < j - c + 1; d++) {
        xiaoshu = xiaoshu / 10;
      }
      sum = sum + xiaoshu;
    }
  }
  return (sum);
}

//对比指纹的子函数
uint8_t getFingerprintID() {
  //提取指纹图像
  uint8_t p = finger.getImage();
  switch (p) {
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

  // 指纹处理
  p = finger.image2Tz();
  switch (p) {
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

  // 指纹对比程序段
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
     return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    myGLCD.setColor(0, 250, 0);
    myGLCD.setBackColor(250, 0, 0);
    myGLCD.setTextSize(2.8);
    myGLCD.print("The finger is rong!!", 80, 190 );
    tone(41, 10000, 2500);// 播放音频  接arduino9号端口，thispitch为播放频率，10为维持时间
    delay(4000);
    TH = false;
    TE = true;
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(10, 180, 389, 220);
    myGLCD.setColor(64, 64, 64);
    myGLCD.fillRect(135, 60, 389, 180);
    return p;
  } else {
    return p;
  }
  if (p == FINGERPRINT_OK) {
    myGLCD.setColor(0, 250, 0);
    myGLCD.setBackColor(250, 0, 0);
    myGLCD.setTextSize(2.8);
    myGLCD.print("The finger is right!", 80, 190 ); //在这添加输出用户姓名与余额
    tone(41, 10000, 500);
    delay(4000);// 播放音频  接arduino9号端口，thispitch为播放频率，10为维持时间
    TE = true;
    LCD(3);
  }
   www=finger.fingerID;//这地方需要计算所需要传递的位数
   if(congzhi==false){
   ggg=i+pp+4;
   wifisend();
   delay(2000);
   }else{
    congzhi=false;
   congzhisend();
   }
  //指纹比对成功打印所对比指纹id号

}
void wifisend(){
  mySerial2.listen();
 String eeeu="%("+(String)www+")("+(String)jine+")%";
 mySerial2.print("AT+CIPSEND=");
 mySerial2.println(eeeu.length());
 mySerial2.println(eeeu);
  delay(1000);
  while(mySerial2.available())
  mySerial.read();
}
uint8_t getFingerprintEnroll() {

  int p = -1;
  myGLCD.setColor(0, 250, 0);
  myGLCD.setBackColor(250, 0, 0);
  myGLCD.setTextSize(2.8);
  myGLCD.print("please put your finger", 80, 190 );
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
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

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
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
  LCD(8);
  myGLCD.setColor(0, 250, 0);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setTextSize(2);
  myGLCD.print("Image converted", 120, 190);
  delay(2000);
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(10, 180, 389, 220);
  myGLCD.setColor(0, 250, 0);
  myGLCD.setBackColor(250, 0, 0);
  myGLCD.setTextSize(2.8);
  myGLCD.print("Rmove your finger", 80, 190 );
  delay(1000);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(10, 180, 389, 220);
  myGLCD.setColor(0, 250, 0);
  myGLCD.setBackColor(250, 0, 0);
  myGLCD.setTextSize(2.8);
  myGLCD.print("please put finger again", 80, 190 );
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
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
  LCD(8);
//  myGLCD.setColor(64, 64, 64);
//  myGLCD.fillRect(135, 60, 389, 180);
  myGLCD.setColor(0, 250, 0);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setTextSize(2);
  myGLCD.print("Image converted", 120, 190);
  delay(2000);
  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
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

  // OK converted!

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    LCD(13);
    delay(1000);
    LCD(3);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    return p;
  } else {
    return p;
  }
  p = finger.storeModel(zid);
  Serial.println(zid);
  if (p == FINGERPRINT_OK) {
//    myGLCD.setColor(255, 0, 0);
//    myGLCD.fillRect(10, 180, 389, 220);
//    myGLCD.setColor(0, 250, 0);
//    myGLCD.setBackColor(250, 0, 0);
//    myGLCD.setTextSize(2.8);
//    myGLCD.print("Stored!", 180, 190 );
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {

    return p;
  } else if (p == FINGERPRINT_FLASHERR) {

    return p;
  } else {
    return p;
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

void congzhisend() {
  mySerial2.listen();
 String eeee="002#"+(String)www+" 1000%";
 mySerial2.print("AT+CIPSEND=");
 mySerial2.println(eeee.length());
 mySerial2.println(eeee);
  delay(1000);
}
void zhucesend()
{
  mySerial2.listen();
 String eeet="001#"+(String)zid+" 1000%";
 mySerial2.print("AT+CIPSEND=");
 mySerial2.println(eeet.length());
 mySerial2.println(eeet);
  delay(1000);
}



