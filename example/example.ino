#include "CmdSerial.h"

CmdSerial cmd;
int testInt = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Example started...");
}



void loop() {
  //
  // Listen for income commands.
  cmd.listen();


  //
  // Example how to change int from Serial input.
  if (cmd.set(F("test"),testInt) ) {
    //
    // The if is true when value is set.
    Serial.println(F("Test set!"));
  }

  //
  // Example how to check what is the value of testInt.
  if (cmd.show(F("test"))) {
    cmd.print(F("Test value:"), testInt);
  }


}
