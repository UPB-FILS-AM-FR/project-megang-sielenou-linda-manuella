#include <LCD_I2C.h>
LCD_I2C lcd(0x3F , 16, 2);  // Default address of most PCF8574 modules, change according

#define PUMP 12
#define HUM A2
#define HUM_POWER 7
#define LEVEL A1
#define LEVEL_POWER 10

//-----------------------
                       
int target_hum = 50;   
                       
//-----------------------

int readLevel() {
  digitalWrite(LEVEL_POWER, HIGH);  // Turn the sensor ON
  delay(10);                        // wait 10 milliseconds
  int val = analogRead(LEVEL);      // Read the analog value form sensor
  digitalWrite(LEVEL_POWER, LOW);   // Turn the sensor OFF
  return val;                       // send current reading
}

int readHum() {
  digitalWrite(HUM_POWER, HIGH);  // Turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  int val = analogRead(HUM);      // Read the analog value form sensor
  digitalWrite(HUM_POWER, LOW);   // Turn the sensor OFF
  return val;                     // send current reading
}


void setup() {

  pinMode(PUMP, OUTPUT);  
  digitalWrite(PUMP, LOW);

  //sensor hum
  pinMode(HUM, INPUT);
  pinMode(HUM_POWER, OUTPUT);
  digitalWrite(HUM_POWER, LOW);
  //sensor nvl apa
  pinMode(LEVEL, INPUT);
  pinMode(LEVEL_POWER, OUTPUT);
  digitalWrite(LEVEL_POWER, LOW);

  //turn screen one
  lcd.begin();      // If you are using more I2C devices using the Wire library use lcd.begin(false)
  lcd.backlight();  //back light on
}

void loop() {

  int hum = readHum();
  int level = readLevel();

  if (isnan(hum) || isnan(level)) {  // If either sensor reading is Not a Number
    lcd.clear();
    lcd.setCursor(0, 0);  // Setting the cursor in the desired position.
    lcd.print("Verifie les capteurs");

    lcd.setCursor(0, 1);  // Setting the cursor in the desired position.
    lcd.print("??");

    digitalWrite(PUMP, LOW);
    //digitalWrite(PUMP2, LOW);
     } else {
    char sHum[16];
    sprintf(sHum, "H:%2d%%  ", hum);

    if (level > 50) {
       lcd.clear();
      lcd.setCursor(0, 0);  // Setting the cursor in the desired position.
      lcd.print(sHum);

      lcd.setCursor(0, 0);  // Setting the cursor in the desired position.
      lcd.print("Hum cible:");
      lcd.print(target_hum);

      lcd.setCursor(0, 1);  // Setting the cursor in the desired position.
      lcd.print("Hum act:");
      lcd.print(map(hum, 0, 1023, 100, 0));

      
      if (map(hum, 0, 1023, 100, 0) > target_hum)
       { digitalWrite(PUMP, LOW);

       }
      else
       { digitalWrite(PUMP, HIGH);

       }
    } else {
      lcd.clear();
      lcd.setCursor(0, 1);  // Setting the cursor in the desired position.
      lcd.print("Verifie niv eau");
      digitalWrite(PUMP, LOW);

    }
  }

  delay(300);  // Delay to reduce the update rate of the LCD and sensor reads
}
