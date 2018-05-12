#include <OneWire.h>
#include <DallasTemperature.h> 
#include <LedControl.h>

//Описание и схема метеостанции: https://bigus.ru/meteostantsiya_na_arduino.html
//НЕ ЗАБУДЬТЕ ПОМЕНЯТЬ АДРЕСА ДАТЧИКОВ ТЕМПЕРАТУРЫ  НА СВОИ!


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// здесь прописываются адреса датчиков температуры, у каждого они свои
// определите эти адреса и замените в коде ниже
DeviceAddress Thermometer1 = { 
  0x28, 0xBF, 0xAF, 0x07, 0x00, 0x00, 0x80, 0x6B };  // адрес датчика 28BFAF070000806B 
  
DeviceAddress Thermometer2 = { 
  0x28, 0xFD, 0xD6, 0x03, 0x00, 0x00, 0x80, 0xE0};  // адрес датчика 28FDD603000080E0

LedControl lc=LedControl(6,5,7,1);

  int i;
  int flag_en = 0;                // Флаг признака однократного гашения экрана
  unsigned long delaytime = 250;

  const int pirPin = 8;           // PIR Sensor
  const int ledPin = 4;           // LED подсветка
 
  const int ledBlinkTime = 500;   // Период моргания светодиода
  const unsigned long calibrationTime = 60; // Время калибровки PIR Module, секунды

  int ledState = LOW;             // этой переменной устанавливаем состояние светодиода
  int TempState = LOW;            // этой переменной устанавливаем состояние флага опроса температуры
  unsigned long previousTempMillis = 0;
  unsigned long temperature_interval = 60000; // интервал между запросами температуры (одна минута)


void setup(void) {
  sensors.begin();
   delay(delaytime); 
  sensors.setResolution(Thermometer1, 10);
   delay(delaytime); 
  sensors.setResolution(Thermometer2, 10);
 
   lc.shutdown(0,false);
   delay(delaytime); 
  
  
   lc.setIntensity(0,8);
   delay(delaytime); 
  
   lc.clearDisplay(0);
   delay(delaytime);   

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(pirPin, LOW);
    
   for (i = 0; i < calibrationTime; i++) {
        digitalWrite(ledPin, HIGH); 
        delay (500);
        digitalWrite(ledPin, LOW); 
        delay (500);
       }
    TemperatureCheck ();            // Здесь запросим температуру один раз 

}


void Show7(){                       // Функция вывода на индикатор
  
  lc.shutdown(0,false);          // Отключаем режим экономии индикатора
   delay(delaytime); 
 
  lc.clearDisplay(0);
  delay(delaytime); 
  
  float tempC = sensors.getTempC(Thermometer1);
 
  int x1;
    
   if( tempC < 0 && tempC >= -9) lc.setChar (0,6,'-',false);
   if( tempC < 0 && tempC < -9) lc.setChar (0,7,'-',false);
 
   tempC = abs (tempC);
  x1 = (int)(tempC*10);
   lc.setDigit(0,4,x1%10,false);
   x1 = x1/10;
  lc.setDigit(0,5,x1%10,true);
   x1 = x1/10;
  if( x1 >0 )lc.setDigit(0,6,x1%10,false);
  
   
   tempC = sensors.getTempC(Thermometer2);
     
   int x2;
   if( tempC < 0 && tempC >= -9) lc.setChar (0,2,'-',false);
   if( tempC < 0 && tempC < -9) lc.setChar (0,3,'-',false);
   
   tempC = abs (tempC);
   x2 = (int)(tempC*10);
   lc.setDigit(0,0,x2%10,false);
   x2 = x2/10;
   lc.setDigit(0,1,x2%10,true);
   x2 = x2/10;
   if( x2 >0 )lc.setDigit(0,2,x2%10,false);


 
 }

void TemperatureCheck () {
   
  digitalWrite(ledPin, HIGH);     // Включаем подсветку
  
  sensors.requestTemperatures();  // Запрос температуры
  
  Show7();                        // Вызов функции показа на индикаторе

while (digitalRead(pirPin) == HIGH) { // Надо, короче
delay (100);
}

  digitalWrite(ledPin, LOW);   // Гасим подсветку
  
  lc.clearDisplay(0);          // Очищаем экран
  lc.shutdown(0,true);         // Индикатор в режим экономии энергии
   
}

void loop(void) {
 
 if (digitalRead(pirPin) == HIGH) { // Движение?
    TemperatureCheck ();            // Уходим на запрос и отображение температуры 
  } 

}
