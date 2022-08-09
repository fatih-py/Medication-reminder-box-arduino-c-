//Kullanılacak kütüphaneler kodlara dahil ediliyor
#include <Keypad.h>    //Keypad tuş takımı kütüphanesi
#include <DS3231.h>    //DS3231 Saat modülü kütüphanesi
#include <Wire.h>      //Seri haberleşme kütüphanesi
#include <LiquidCrystal_I2C.h>  //LCD ekran kütüphanesi

LiquidCrystal_I2C lcd(0x27,16,2); //LCD ekran adresi, sütun sayısı, satır sayısı

DS3231 rtc(SDA, SCL); // RTC saat modülü pinleri tanımlandı

const byte ROWS = 4;  //tuş takımı için satır sayısı          
const byte COLS = 3;  //tuş takımı için sütun sayısı          
char keys[ROWS][COLS] = {   //Tuş takımı satır ve sütun tanımlamaları    
  {'1','2','3'},            
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {6, 7, 8, 9};         // Arduino Satır pinleri tanıtıldı
byte colPins[COLS] = {10, 11, 12};         // Arduino Sütun pinleri tanıtıldı

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //Keypad tuş takımı haritsı oluşturuldu

//Ledlerin hangi pinlere tanımlandığı belirtiliyor
int sabah_led=A0;   //sabah ledi A0 pinine
int ogle_led=A1;    //ogle ledi A1 pinine
int aksam_led=A2;   //akşam ledi A2 pinine
int gece_led=A3;    //gece ledi A3 pinine

String Saat;      //Saat isimli string veri türünde bir değişken oluşturuldu
String Alarm_1;   //Alarm_1 isimli string veri türünde bir değişken oluşturuldu
String Alarm_2;   //Alarm_2 isimli string veri türünde bir değişken oluşturuldu
String Alarm_3;   //Alarm_3 isimli string veri türünde bir değişken oluşturuldu
String Alarm_4;   //Alarm_4 isimli string veri türünde bir değişken oluşturuldu

byte SutunSayac=0;  //SutunSayac isimli byte veri türünde değişken ilk değeri 0
byte AlarmSayac=0;  //AlarmSayac isimli byte veri türünde değişken ilk değeri 0
  
int ses_modul=5;   //Ses kayıt modülü kontrol ucu 5 nolu pine tanımlandı

void setup() {

Serial.begin(9600);       //Seri haberleşme başlangıç ayarları yapıldı 9600 baut rate hızı seçildi

//Led pinleri çıkış olarak tanımlanıyor
pinMode (sabah_led, OUTPUT); 
pinMode (ogle_led, OUTPUT);
pinMode (aksam_led, OUTPUT);
pinMode (gece_led, OUTPUT);

lcd.begin();      //Lcd başlangıç ayarları yapılıyor
lcd.backlight();  //Lcd arka plan ışığı yakılıyor

rtc.begin(); // RTC saat modülü başlatıldı
//rtc.setDOW(); /* Gün ayarı yapılır. Program yüklendikten sonra yorum haline getirilir ve program tekrar yüklenir. */
//rtc.setTime(16, 20, 0); /* Saat ayarı yapılır. Program yüklendikten sonra yorum haline getirilir ve program tekrar yüklenir. */
//rtc.setDate(18, 04, 2022); /* Tarih ayarı yapılır. Program yüklendikten sonra yorum haline getirilir ve program tekrar yüklenir. */

}

void loop() {
  
Serial.print(" Haftanin gunu: ");
Serial.println(rtc.getDOWStr());  // Seri ekrana Haftanın gününü yazdırıldı
int Gun=rtc.getDOWStr();
Serial.print(" Tarih:");
Serial.println(rtc.getDateStr()); // Seri ekrana Tarih bilgisi yazdırıldı
int Tarih=rtc.getDateStr();
Serial.print(" Saat: ");
Serial.println(rtc.getTimeStr()); // Seri Ekrana Saat bilgisi yazdırıldı
String Saat=rtc.getTimeStr();
Serial.print(" Sicaklik: ");
Serial.print(rtc.getTemp());      // Seri Ekrana Sıcaklık bilgisi yazdırıldı
float Sicaklik=rtc.getTemp();
Serial.println(" derece");
Serial.println("--------------------------------");

int SaatSayi = Saat.toInt(); // String türündeki Saat değişkenini integer türüne dönüştüren fonksiyon

lcd.setCursor(0,0);                 //Cursor yani imleç 0.sütun 0.satıra getirildi
lcd.print(rtc.getDateStr());        //Ekrana tarih yazdırıldı

lcd.setCursor(12,0);                //Cursor yani imleç 12.sütun 0.satıra getirildi
lcd.print(rtc.getTemp(),0);         //Ekrana sıcaklık yazdırıldı

lcd.setCursor(15,0);                //Cursor yani imleç 15.sütun 0.satıra getirildi
lcd.print("C");                     //Ekrana C yazdırıldı

lcd.setCursor(0,1);                 //Cursor yani imleç 0.sütun 1.satıra getirildi
lcd.print(rtc.getTimeStr());        //Ekrana saat yazdırıldı

lcd.setCursor(9,1);                 //Cursor yani imleç 9.sütun 1.satıra getirildi
lcd.print(rtc.getDOWStr());         //Ekrana gün yazdırıldı

//Herhangi bir alarm vakti geldiğinde yapılacaklar tanımlanır
if (Saat==Alarm_1 || Saat==Alarm_2 || Saat==Alarm_3 || Saat==Alarm_4){   //Eğer Alarm_1 veya Alarm_2 veya Alarm_3 veya Alarm_4 Saate denk ise yani alarm vakti gelmişse
  
  lcd.clear();                  //Ekranı temizle
  char key = keypad.getKey();   //Tuş takımındaki tuşları oku
  while(key!='#'){              //# tuşuna basılana kadar
    key = keypad.getKey();
      
    if (5<=SaatSayi && SaatSayi<12){
      lcd.setCursor(3,0);
      lcd.print("ILAC VAKTI");
      lcd.setCursor(0,1);
      lcd.print("ILACINIZI ALINIZ");
      digitalWrite (sabah_led,HIGH);    //İlgili led yansın
      digitalWrite (ses_modul, HIGH);   //Ses modülü aktif olsun
      digitalWrite (ses_modul, LOW);    //Ses modülü pasif olsun
      delay(1000);                      //1 saniye bekleme
      digitalWrite(sabah_led,LOW);      //ilgili led sonsun
      delay(500);                       //yarım saniye bekleme
    }
    
    if (12<=SaatSayi && SaatSayi<17){
      lcd.setCursor(3,0);
      lcd.print("ILAC VAKTI");
      lcd.setCursor(0,1);
      lcd.print("ILACINIZI ALINIZ");
      digitalWrite (ogle_led,HIGH);     //İlgili led yansın
      digitalWrite (ses_modul, HIGH);   //Ses modülü aktif olsun
      digitalWrite (ses_modul, LOW);    //Ses modülü pasif olsun
      delay(1000);                      //1 saniye bekleme
      digitalWrite(ogle_led,LOW);       //ilgili led sonsun
      delay(500);                       //yarım saniye bekleme
    }

    if (17<=SaatSayi && SaatSayi<22){
      lcd.setCursor(3,0);
      lcd.print("ILAC VAKTI");
      lcd.setCursor(0,1);
      lcd.print("ILACINIZI ALINIZ");
      digitalWrite (aksam_led,HIGH);    //İlgili led yansın
      digitalWrite (ses_modul, HIGH);   //Ses modülü aktif olsun
      digitalWrite (ses_modul, LOW);    //Ses modülü pasif olsun
      delay(1000);                      //1 saniye bekleme
      digitalWrite(aksam_led,LOW);      //ilgili led sonsun
      delay(500);                       //yarım saniye bekleme
    }

    if ((22<=SaatSayi && SaatSayi<24) || (0<=SaatSayi && SaatSayi<5)){
      lcd.setCursor(3,0);
      lcd.print("ILAC VAKTI");
      lcd.setCursor(0,1);
      lcd.print("ILACINIZI ALINIZ");
      digitalWrite (gece_led,HIGH);     //İlgili led yansın
      digitalWrite (ses_modul, HIGH);   //Ses modülü aktif olsun
      digitalWrite (ses_modul, LOW);    //Ses modülü pasif olsun
      delay(1000);                      //1 saniye bekleme
      digitalWrite(gece_led,LOW);       //ilgili led sonsun
      delay(500);                       //yarım saniye bekleme
    }
    
  }
  lcd.clear();                        //Ekranı temizle
}

//Bir alarm kurmak için yapılan işlemler burada belirtilir
char key = keypad.getKey();           //Tuş takımında basılan tuşu oku
if (key=='*') {                       //Tuş takımında * tuşuna basılmışsa
      AlarmSayac ++;                  //Alarm_Sayac değişkenini 1 arttır
      if (AlarmSayac==1){             //AlarmSayac 1 ise
      lcd.clear();                    //Ekranı temizle
      lcd.setCursor(0,0);             //İmleci 0.sütun 0.satıra getir
      lcd.print("1.ALARM GIRIN");     //1.ALARMI GİRİN yaz
      lcd.setCursor(4,1);             //İmleci 4.sütun 0.satıra getir
      Alarm_1="";                     //Alarm_1 değişkenini temizle
      SutunSayac = 0;                 //SutunSayac değişkeni 0 olsun
        
      //1.Alarm için girilen saat bilgileri lcd ekrana yazdırılır
      while(key!='#'){               
        key = keypad.getKey();
        delay(100);
                   
          if(key){
            lcd.print(key);
            Alarm_1 += key; 
            SutunSayac ++;
          }

          if(SutunSayac==2){
              key=':';
              lcd.print(key);
              Alarm_1 += key;
              SutunSayac ++;
          }

          if(SutunSayac==5){
              key=':';
              Alarm_1 += key;
              key='0';
              Alarm_1 += key;
              key='0';
              Alarm_1 += key; 
              key='#';             
          }
       }
          key=0;
          lcd.clear();                  //Alarm saati giridiğinde ekranı temizle
          lcd.setCursor(0,0);           //İmleci 0.sütun 0.satıra getir
          lcd.print("1.ALARM KURULDU"); //1.Alarm kuruldu yaz
          lcd.setCursor(4,1);           //İmleci 4.sütun 1.satıra getir
          lcd.print(Alarm_1);           //Ekrana Alarm_1 saatini yaz
          delay(5000);                  //5 saniye bekle
          lcd.clear();                  //Ekranı temizle
      }

      if (AlarmSayac==2){               //Eğer AlarmSayac değeri 2 ise 2.Alarm için yukarıdaki işlemler tekrar edilir
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("2.ALARM GIRIN");
      lcd.setCursor(4,1);
      Alarm_2="";
      SutunSayac = 0;
        
      while(key!='#'){
        key = keypad.getKey();
        delay(100);
                   
          if(key){
            lcd.print(key);
            Alarm_2 += key; 
            SutunSayac ++;
          }

          if(SutunSayac==2){
              key=':';
              lcd.print(key);
              Alarm_2 += key;
              SutunSayac ++;
          }

          if(SutunSayac==5){
              key=':';
              Alarm_2 += key;
              key='0';
              Alarm_2 += key;
              key='0';
              Alarm_2 += key; 
              key='#';             
          }
       }
          key=0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("2.ALARM KURULDU");
          lcd.setCursor(4,1);
          lcd.print(Alarm_2);
          delay(5000);
          lcd.clear();
      }

      if (AlarmSayac==3){   //Eğer AlarmSayac değeri 3 ise 3.Alarm için yukarıdaki işlemler tekrar edilir
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("3.ALARM GIRIN");
      lcd.setCursor(4,1);
      Alarm_3="";
      SutunSayac = 0;
        
      while(key!='#'){
        key = keypad.getKey();
        delay(100);
                   
          if(key){
            lcd.print(key);
            Alarm_3 += key; 
            SutunSayac ++;
          }

          if(SutunSayac==2){
              key=':';
              lcd.print(key);
              Alarm_3 += key;
              SutunSayac ++;
          }

          if(SutunSayac==5){
              key=':';
              Alarm_3 += key;
              key='0';
              Alarm_3 += key;
              key='0';
              Alarm_3 += key; 
              key='#';             
          }
       }
          key=0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("3.ALARM KURULDU");
          lcd.setCursor(4,1);
          lcd.print(Alarm_3);
          delay(5000);
          lcd.clear();
      }

    if (AlarmSayac==4){   //Eğer AlarmSayac değeri 4 ise 4.Alarm için yukarıdaki işlemler tekrar edilir
      AlarmSayac=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("4.ALARM GIRIN");
      lcd.setCursor(4,1);
      Alarm_4="";
      SutunSayac = 0;
        
      while(key!='#'){
        key = keypad.getKey();
        delay(100);
                   
          if(key){
            lcd.print(key);
            Alarm_4 += key; 
            SutunSayac ++;
          }

          if(SutunSayac==2){
              key=':';
              lcd.print(key);
              Alarm_4 += key;
              SutunSayac ++;
          }

          if(SutunSayac==5){
              key=':';
              Alarm_4 += key;
              key='0';
              Alarm_4 += key;
              key='0';
              Alarm_4 += key; 
              key='#';             
          }
       }
          key=0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("4.ALARM KURULDU");
          lcd.setCursor(4,1);
          lcd.print(Alarm_4);
          delay(5000);
          lcd.clear();
      }
  }
}
