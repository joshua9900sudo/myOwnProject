
//const char ADDR[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
//const char DATA[] = {39,41,43,45,47,49,51,53};


#define EEPROM_IO0 14
#define EEPROM_IO7 21
#define EEPROM_A0 22
#define EEPROM_A14 52
#define WRITE_EN 69
#define OUTPUT_EN 67
#define CHIP_EN 65

/*
 * Output the address bits and outputEnable signal using shift registers.
 */

void setAddress(int address, bool outputEnable) {
  //#define EEPROM_A0 22
  //#define EEPROM_A14 52
  for (int i = EEPROM_A0; i <= EEPROM_A14; i += 2) {
    pinMode(pin, OUTPUT);
  }

  for (int i = EEPROM_A0; i <= EEPROM_A14; i += 2) {
    //I was trying to designate the pins for the address.
    //My plan is to convert the int address to binary and binary to string
    //and use 'for (;;) {digitalWrite(i, int(address_binary[j]))}
    //22.08.13
  }
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 2) {
    pinMode(pin, INPUT);
  }
  //setAddress(address, /*outputEnable*/ true);
  digitalWrite(OUTPUT_EN, LOW);
  digitalWrite(WRITE_EN, LOW);
  digitalWrite(CHIP_EN, LOW);
  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 2) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) { 
  setAddress(address);
  digitalWrite(OUTPUT_EN, HIGH);
  digitalWrite(WRITE_EN, LOW);
  digitalWrite(CHIP_EN, LOW);
  /////////////////////////////////////////////////////
  for (int pin = EEPROM_IO0; pin <= EEPROM_IO21; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_IO0; pin <= EEPROM_IO21; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  digitalWrite(WRITE_EN, LOW);
  digitalWrite(WRITE_EN, LOW);
  digitalWrite(WRITE_EN, HIGH);
  delayMicroseconds(1);
  //there's no MAX write time, but MIN time so I write the same command triple times for just a little delay.
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


// 4-bit hex decoder for common anode 7-segment display
//byte data[] = { 0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84, 0x88, 0xe0, 0xb1, 0xc2, 0xb0, 0xb8 };

// 4-bit hex decoder for common cathode 7-segment display
 byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };


void setup() {
  // put your setup code here, to run once:
  
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUTPUT_EN, HIGH);
  digitalWrite(CHIP_EN, HIGH);

  pinMode(WRITE_EN, OUTPUT);
  pinMode(OUTPUT_EN, OUTPUT);
  pinMode(CHIP_EN, OUTPUT);   
  Serial.begin(57600);

  // Erase entire EEPROM
  Serial.print("Erasing EEPROM");
  //for the 28c16, this had 2k words(bytes), 28c256 has 32k words = 32768.
  for (int address = 0; address <= 32767; address += 1) {
    writeEEPROM(address, 0xea);
//////////////////////////////////////////////////////////////
    if (address % 8192 == 0) { /*256k / 8192 = 32 */
      Serial.print(".");
    }
  }
  Serial.println(" done");


  // Program data bytes
  Serial.print("Programming EEPROM");
  for (int address = 0; address < sizeof(data); address += 1) {
    writeEEPROM(address, data[address]);
    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");


  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}
