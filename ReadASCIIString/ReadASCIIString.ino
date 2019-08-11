/*
  Reading a serial ASCII-encoded string.

In my main CDI Ignition ESP software, I make use of 1 variable and 2 maps:

int PickupPosition = 0;
int RPMmap[] = {0, 800, 2250, 5000, 0};
int ADVmap[] = {0, 1 ,  28 ,  28  , 0};

- Maps work by couple: 800 goes with 1,  2250 with 28 , 5000,28 etc
- 1st and last couples MUST be 0,0 as there are boundaries

I want the customer to insert values into those maps via USB serial connection.Below is the code I wrote (it compile so far)

- User first enter <PickupPosition> (in setup loop I suppose) ie:  50\r\n 
- then User is supposed to enter some couple of values ( <rpm> <space or comma or semicolomn> <advance>   ie 800,1\r\n
- RPM MUST be in incremential order , if user enter 1000 after 2000 , a error msg is shown (or the code can order them if it's not too complicated )
- The number of couple is free , minimum is 2 couples: 
int RPMmap[] = {0, 800, 5000, 0};
int ADVmap[] = {0, 1 ,  28  , 0};
   maximum is 28 couples (+2 zero couples)

- If the user enter letter S (I used "0" in the code because rpm is declare as int), all couples are saved in eeprom and the program leave the loop and close (if possible)
- If the user enter "E" the code Erase all previously saved values in eeprom (EEPROM.erase)

I also need another function that read the eprom values and fill in the 2 maps like this Pseudo-code:

for i = 0 to 30  {
  EEPROM.get(i, RPMmap(i))
  EEPROM.get(i, ADVmap(i))
  }

The result should then be:  
int RPMmap[] = {0, 800, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int ADVmap[] = {0, 1 ,  28  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


*/
#include <EEPROM.h>

int RPM;
int previousRPM = 0;

int i = 1;

// initializes pickup position
int PickupPosition = 50;

// initializes char array with a size of 150
int RPMmap[150] = {};
int ADVmap[150] = {};

void setup() {
  // initialize serial:
  Serial.begin(9600);
  delay(500);
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
        // Serial.print("RPM:");
        // Serial.println(RPM);
        // Serial.print("ADV:");
        // Serial.println(ADV);
        RPMmap[i] = RPM;
        // Serial.println(RPMmap[i]);
        ADVmap[i] = ADV;
        // Serial.println(ADVmap[i]);

        i = i + 1;
        RPMmap[i] = 0;
        ADVmap[i] = 0;

        for(int j = 0; j <= i; ++j) {
          Serial.println(RPMmap[j]);
        }
      }
    }

    // Write it down if user type 0,0 or just 0:
    if (RPM == 0) {
      Serial.println("Save.");
      EEPROM.put(0,RPM); // (adr, octet)
    }
  }

}