#include <Servo.h>
Servo motor;
int arkamotorin1 = 6;
int arkamotorin2 = 5;
int arkaenable = 3;

int trigPin_on = 2; //mesafe ölçer sensörlerimizin pinleri
int echoPin_on = 4;
int trigPin_arka = 11; 
int echoPin_arka = 12; 
int trigPinsag_on = 7; 
int echoPinsag_on = 8;
int trigPinsag_arka = 9; 
int echoPinsag_arka = 10;  

int counter_on ;
int counter_arka ;
float avg_arka = 0;  //Hesaplanan ortalama değerlerinin değişkenleri
float m_arka = 0;  
float avg_on = 0;
float m_on = 0;  

 int val_on1 ;
 int val_on2 ;
 int val_on3 ;   // mesafe sensörlerinden gelen verilerin 5 er tanesini hafızada tutmak için tanımlanan değişkenler.
 int val_on4 ;
 int val_on5 ;
 int val_arka1 ;
 int val_arka2 ;
 int val_arka3 ;
 int val_arka4 ;
 int val_arka5 ;


long sure1; // mesafe ölçme fonksiyonlarında kullanılan süre tanımlamaları
long sure2; 
long sag_on_uzaklik;
long sag_arka_uzaklik ;
long on_uzaklik;
long arka_uzaklik;
int park_durumu=0;

void setup() {
  motor.attach(13);
  motor.write(85);
  delay(5000);
  Serial.begin(9600);
  pinMode(arkamotorin1, OUTPUT);
  pinMode(arkamotorin2, OUTPUT);
  pinMode(arkaenable, OUTPUT);
  pinMode(trigPinsag_on , OUTPUT);
  pinMode(echoPinsag_on , INPUT);
  pinMode(trigPinsag_arka , OUTPUT);
  pinMode(echoPinsag_arka , INPUT);
  pinMode(trigPin_on , OUTPUT);
  pinMode(echoPin_on , INPUT);
  pinMode(trigPin_arka , OUTPUT);
  pinMode(echoPin_arka , INPUT);
  

}

void loop() {
  park();
}

float mov_on_avg (int counter, float input)
{
  avg_on = 0;  //Hesaplanan ortalama değeri
  m_on = 0;    //Ortalama hesabında payda hesabı ( "0" değerleri dikkate alınmaz)
  
  if (counter_on == 1){  //ilk okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_on1=input;
    avg_on = val_on1;
    
  }
  else if(counter_on == 2){  //2. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_on2=input;
    avg_on = val_on2;
    
  }
  else if(counter_on == 3){  //3. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_on3=input;
    avg_on = val_on3;
    
  }
  else if(counter_on == 4){  //4. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_on4=input;
    avg_on = val_on4;
    
  }
  else if(counter_on == 5){  //5. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_on5=input;
    avg_on = val_on5;
    
  }
  else if (counter_on > 5 ){ // 5 değer kaydedildikten sonra filtre çalışmaya başlar
    
    counter_on = 6;  // counter'ın kontrolsüz büyümemesi için sabitlenir
    
    if (val_on1 == 0){  //1. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_on=m_on+1;
    }
    if (val_on2 == 0){  //2. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_on=m_on+1;
    }
    if (val_on3 == 0){  //3. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_on=m_on+1;
    }
    if (val_on4 == 0){  //4. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
     m_on=m_on+1;
    }
    if (val_on5 == 0){  //5. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_on=m_on+1;
    }
    if (input == 0){  //yeni okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_on=m_on+1;
    }
 
int    d=6-m_on;  //sıfır sayısına göre payda belirlenir
 
    
    if (d==0)  // bütün değerlerin sıfır olması durumudur. 
    {
     avg_on = input; //Çıkışa direk giriş verilir. Filtre işlevsizdir
     counter = 1; //Bütün değerler "0" okunduğu için filtre baştan başlatılır
    }
    else  
    {
      avg_on = (val_on1+val_on2+val_on3+val_on4+val_on5+input)/d; //kaydedilen 5 değer ve yeni okunan değerin ortalaması alınır.
    }   
 
     val_on1 = val_on2; // Burada kaydırma işlemi yapılır.
     val_on2 = val_on3; // Her hesaplamanın sonunda yeni değerler kaydedilir.
     val_on3 = val_on4; // En eski değer kaybolur.
     val_on4 = val_on5; // En son olarak da yeni okunan değer kaydedilir.
     val_on5 = input;
  }
 
  return avg_on; //avg değeri yani ortalama döndürülür.
}
float mov_arka_avg (int counter, float input)
{
  avg_arka = 0;  //Hesaplanan ortalama değeri
  m_arka = 0;    //Ortalama hesabında payda hesabı ( "0" değerleri dikkate alınmaz)
  
  if (counter_arka == 1){  //ilk okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_arka1=input;
    avg_arka = val_arka1;
    
  }
  else if(counter_arka == 2){  //2. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_arka2=input;
    avg_arka = val_arka2;
    
  }
  else if(counter_arka == 3){  //3. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_arka3=input;
    avg_arka = val_arka3;
    
  }
  else if(counter_arka == 4){  //4. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_arka4=input;
    avg_arka = val_arka4;
    
  }
  else if(counter_arka == 5){  //5. okunan değer kaydedilir. Çıkış boş kalmaması için giriş direk verilir.
    val_arka5=input;
    avg_arka = val_arka5;
    
  }
  else if (counter_arka > 5 ){ // 5 değer kaydedildikten sonra filtre çalışmaya başlar
    
    counter = 6;  // counter'ın kontrolsüz büyümemesi için sabitlenir
    
    if (val_arka1 == 0){  //1. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_arka=m_arka+1;
    }
    if (val_arka2 == 0){  //2. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
     m_arka=m_arka+1;
    }
    if (val_arka3 == 0){  //3. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_arka=m_arka+1;
    }
    if (val_arka4 == 0){  //4. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_arka=m_arka+1;
    }
    if (val_arka5 == 0){  //5. okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_arka = m_arka+1;
    }
    if (input == 0){  //yeni okunan değerin sıfır olma durumu kontrol edilir. m değeri 1 arttırılır
      m_arka=m_arka+1;
    }
 
int    d=6-m_arka;  //sıfır sayısına göre payda belirlenir
 
    
    if (d==0)  // bütün değerlerin sıfır olması durumudur. 
    {
     avg_arka = input; //Çıkışa direk giriş verilir. Filtre işlevsizdir
     counter_arka = 1; //Bütün değerler "0" okunduğu için filtre baştan başlatılır
    }
    else  
    {
     avg_arka = (val_arka1+val_arka2+val_arka3+val_arka4+val_arka5+input)/d; //kaydedilen 5 değer ve yeni okunan değerin ortalaması alınır.
    }   
 
     val_arka1 = val_arka2; // Burada kaydırma işlemi yapılır.
     val_arka2 = val_arka3; // Her hesaplamanın sonunda yeni değerler kaydedilir.
     val_arka3 = val_arka4; // En eski değer kaybolur.
     val_arka4 = val_arka5; // En son olarak da yeni okunan değer kaydedilir.
     val_arka5 = input;
  }
 
  return avg_arka; //avg değeri yani ortalama döndürülür.
}


  void park(){
    sag_on();
    sag_arka();
    on();
    arka();
    Serial.println("Park durumu : ");
    Serial.println(park_durumu);
    Serial.println("on");
    Serial.println(sag_on_uzaklik);
    Serial.println("arka");
    Serial.println(sag_arka_uzaklik);
    if(sag_on_uzaklik < 10 && sag_arka_uzaklik < 10 && park_durumu==0) 
    {
      ileri(90);
      park_durumu=1; // park durumu aracımızın ilerleyişini kontrol ederek her if'in içerisine girmemeyi sağlıyor}
    }
    if( sag_on_uzaklik > 10 && sag_arka_uzaklik < 10 && park_durumu==1)
    {
      park_durumu=2;
    }
 
    if((sag_on_uzaklik >10) && (sag_arka_uzaklik > 10) && park_durumu == 2) // aracımız 1. araçtan sonra park edilecek bir alan olduğunu anlıyor.
    {  park_durumu=3;
       analogWrite(arkaenable, 60); 
    }
   
    if((sag_on_uzaklik <10) && (sag_arka_uzaklik > 10) && park_durumu == 3)// 2. aracın yanına yaklaştığını anlıyor  geldiğini anlıyor.
    {      
      park_durumu=4;
    }
    
    if((sag_on_uzaklik < 10) && ( sag_arka_uzaklik <10 ) && park_durumu == 4)//2. aracın yanına geldiğini ve 2 aracın arasında park edilebilecek bir boşluk olduğunu anlıyor
    {
     //delay(150);
     dur();
     park_durumu=5;
     sag();
     Serial.println("Park durumu : ");
     Serial.println(park_durumu);
    }
     if(park_durumu == 5 )// arac bu if kontrolünde arac arkasında ki mesafe kontrol ederek geri geri manevrasını yaparak gelme komutuna giriyor.
    { 
      geri(110);
      park_durumu = 6;
    }
    if(sag_on_uzaklik > 10 && park_durumu == 6){
      dur();
      delay(500);
      geri(70);
      sol();
      park_durumu=7;       
    }
    if(arka_uzaklik < 50 && park_durumu == 7)// manevrayı rahat alabilmesi için ön tekerler sağ tarafa çevriliyor 
        {
          dur();
          ortala();
          park_durumu=8; 
          }
    if(park_durumu == 8) // arkasındaki araca çapmamak için bu komutta ön tekerler sol tarafa çevrilerek ileri gidiyor.
    {
      ileri(100);
      park_durumu=9;     
      sag();
    }if(park_durumu==9)
      {  on();
          arka();   }
    if((on_uzaklik < 15) && park_durumu == 9)// ön mesafeyi ayarlayıp parkını bitirdiğini anlıyor.  Ve parkını bitiriyor.
    { 
      dur();
      ortala();
      park_durumu=10;
    }

  
  }
  
  
  void sag_on(){
  digitalWrite(trigPinsag_on, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPinsag_on, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPinsag_on, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
  sure1 = pulseIn(echoPinsag_on, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  sag_on_uzaklik= (sure1 * 0.034) /2; /* ölçülen sure uzaklığa çevriliyor */  
   if(sag_on_uzaklik > 1000){
    sag_on_uzaklik = 0;
  }
  
 
  delay(30);
  }
  
  void sag_arka(){
  digitalWrite(trigPinsag_arka, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPinsag_arka, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPinsag_arka, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
  sure2 = pulseIn(echoPinsag_arka, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  sag_arka_uzaklik= (sure2 * 0.034)/2; /* ölçülen sure uzaklığa çevriliyor */  
   if(sag_arka_uzaklik > 1000){
    sag_arka_uzaklik = 0;
  }
  //Serial.println("Arka");
  //Serial.println(sag_arka_uzaklik);

  delay(30);
  }
 
  void on(){
  digitalWrite(trigPin_on, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPin_on, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPin_on, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
  sure1 = pulseIn(echoPin_on, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  on_uzaklik= (sure1 * 0.034)/2; /* ölçülen sure uzaklığa çevriliyor */   
  if(on_uzaklik>200)
  on_uzaklik=0;  
  counter_on++;
  if(counter_on==6)
  counter_on==0;
  float avg = mov_on_avg(counter_on,on_uzaklik);
  on_uzaklik= avg;
  Serial.println("on");
  Serial.println(on_uzaklik);
  delay(30);    
  }
  
  void arka(){
  digitalWrite(trigPin_arka, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPin_arka, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPin_arka, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
  sure2 = pulseIn(echoPin_arka, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  arka_uzaklik= (sure2 * 0.034)/2; /* ölçülen sure uzaklığa çevriliyor */  
  if(arka_uzaklik > 200)
    arka_uzaklik = 200;  
  counter_arka++;
  if(counter_arka==6)
  counter_arka==0;
  arka_uzaklik = mov_arka_avg(counter_arka,arka_uzaklik);
  
  Serial.println("arka"); 
  Serial.println(arka_uzaklik);
  delay(30);    
  }

  void geri(int x){
  analogWrite(arkaenable, x); 
  digitalWrite(arkamotorin1, HIGH);
  digitalWrite(arkamotorin2,  LOW);
}
void ileri(int x){
  analogWrite(arkaenable, x); 
  digitalWrite(arkamotorin1, LOW);
  digitalWrite(arkamotorin2,  HIGH);
}

void sag(){
  motor.write(65);
}
void sol(){
  motor.write(105);
}
void ortala(){
  motor.write(85);
}

void dur(){
  digitalWrite(arkamotorin1, LOW);
  digitalWrite(arkamotorin2,  LOW);
}


  
