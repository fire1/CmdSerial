#include <CmdSerial.h>

CmdSerial cmd;
int testInt = 0;
float temperature = 21.5;

void setup() {
    Serial.begin(9600);
    Serial.println("Example started, use `help` to list all commands."_f);
}

void loop() {
    //
    // Listen for income commands.
    cmd.listen();

    //
    // Change int from Serial input: `test 12`
    if (cmd.set("test"_f, testInt, "Set the test integer"_f)) {
        Serial.println("Test set!"_f);
    }

    //
    // Change float: `temp 23.4`
    cmd.set("temp"_f, temperature, "Set temperature"_f);

    //
    // Monitor values: `test show` / `test stop`
    // "..."_f keeps labels in PROGMEM (same idea as F("..."))
    if (cmd.show("test"_f, "This command is just for test..."_f)) {
        cmd.print("Test value:"_f, testInt);
    }

    //
    // Multiple fields on one line: `sensors show`
    if (cmd.show("sensors"_f, "Print several values at once"_f)) {
        cmd.println("temp"_f, temperature, "test"_f, testInt);
    }
}
