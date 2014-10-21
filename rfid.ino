#include <Adafruit_NFCShield_I2C.h> //RFID shield

#define IRQ   (10) // Digital I/O for RFID shield
#define RESET (3)  // Not connected by default on the NFC Shield
Adafruit_NFCShield_I2C nfc(IRQ, RESET);

void initRfidReading(void){
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0x00);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
}

String readRfidCard(void){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    
    if (uidLength == 4)
    {
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
       uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
      // Start with block 4 (the first block of sector 1) since sector 0
      // contains the manufacturer data and it's probably better just
      // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
    
      if (success)
      {
        uint8_t data[16];
    
        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
    
        if (success)
        {
          String rfidLog = logRfidData(data, 16);
          return rfidLog;
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
          return "";
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
        return "";
      }
    }
    
  }else{return "";}
}


String logRfidData(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  String rfidId;

  for (szPos=0; szPos < numBytes; szPos++) 
  {
    if (data[szPos] <= 0x1F)
      rfidId += F("");
    else
      rfidId +=(char)data[szPos];
  }
  return rfidId;
}
