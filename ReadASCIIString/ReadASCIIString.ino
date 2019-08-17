#include <EEPROM.h>

int RPM;
int previousRPM = 0;
int Address =1;
int i = 1;

// initializes pickup position
int PickupPosition = 50;

int value;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(500);

  // write zeros to EEPROM
  for(int k = 0; k <= 512; ++k) {
    EEPROM.write(k, 0);
  }
  EEPROM.commit();

  Serial.println(""); 
  Serial.println("Reading values from USB---------. Enter: rpm , advance");
}

void loop() {
  // if there's any serial available, read it:
  // La fonction Serial.available() est toujours utilisée pour connaître combien d'octets restent dans le buffer.
  // Celui-ci est limité à 63 octets et si il n'y pas de Serial.read() ou de Serial.parseInt() pour enlever petit à petit les octets, alors il atteindra son maximum. 

  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    RPM = Serial.parseInt();

    // do it again:
    int ADV = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
      if (RPM < previousRPM && RPM > 0) {
         Serial.println("Error!");
      }
      else {
        previousRPM = RPM;

        //prints and writes directly on eeprom
        Serial.print(RPM);
        Serial.print("rpm = ");
        EEPROM.put(Address, RPM);

        Address = Address + 4;
        
        Serial.print(ADV);
        EEPROM.put(Address, ADV);
        Serial.println("deg.");
        
        Address = Address + 4;
      }
    }
    // Write it down if user type 0,0 or just 0:
    if (RPM == 0) {
      Serial.println("Save.");
      // saves directly on eeprom
      EEPROM.commit();
      // reads values from eeprom
      readEEPROM();

      //reset Address
      Address = 1;
    }
  }
}

void readEEPROM() {
    for(int Address = 1; Address <= 513; Address=Address+4) {
        value = EEPROM.get(Address,RPM);
        delay(0.75);
        Serial.print(Address);
        Serial.print("\t");
        Serial.print(value, DEC);
        Serial.println();
    }
}