//
// Created by fire1 on 2024-03-20.
//

#ifndef CMDSERIAL_H
#define CMDSERIAL_H

#include <Arduino.h>

#define CMDSERIAL_VERSION "1.0.4"

#ifndef CmdSerial_EnterMsg
#define CmdSerial_EnterMsgTitle F("[SET] Entered  /")
#endif

#ifndef CmdSerial_EnterMsg
#define CmdSerial_EnterMsgValue F("/ value: ")
#endif

#ifndef CmdSerial_StopMsg
#define CmdSerial_StopMsg F("[STOP] ")
#endif

#ifndef CmdSerial_KeywordShow
#define CmdSerial_KeywordShow F("show")
#endif

#ifndef CmdSerial_KeywordStop
#define CmdSerial_KeywordStop F("stop")
#endif

/**
 * @class CmdSerial
 * @brief A simple command providing class for Serial.
 * @details
 *
 *  @author Angel Zaprianov /fire1/
 */
class CmdSerial {
private:
    bool isSet = false;
    Stream *serial;
    String cmdName;
    String cmdData;


    unsigned long lastCmdTime = 0;
    char spr;
    char eol;

public:
    //
    // Default parameters as constants.
    static const char Separator = ' ';
    static const char EndOfLine = '\n';

    /**
     * Constructor of the class with dependencies.
     * @param theSerial Serial stream to be used.
     * @param separator The separator of the command.
     * @param endOfLine The end character of the command line.
     */
    CmdSerial(
            Stream &theSerial = Serial,
            const char separator = CmdSerial::Separator,
            const char endOfLine = CmdSerial::EndOfLine
    ) {
        serial = &theSerial;
        this->spr = separator;
        this->eol = endOfLine;
    }

    /**
     * Listens for Serial stream input and parses it based on methods used in this class.
     *      This method is required be called in the `loop()` function in order this class to work.
     */
    void listen() {
        isSet = false;
        if (serial->available() > 0) {
            cmdName = serial->readStringUntil(spr);
            cmdName.trim();
            cmdData = serial->readStringUntil(eol);
            cmdData.trim();
        }
    }

    /**
     * Shows data for given cycles, should be called in `if` statement.
     * @param name  The command name
     * @param milliSeconds  Time/interval used to display data /return true/
     * @return
     */
    bool show(const String &name, unsigned long milliSeconds = 500) {
        if (name != cmdName && !isSet) return false;

        if (cmdData == CmdSerial_KeywordShow) {
            unsigned long time = millis();
            if (cmdName == name && time - lastCmdTime >= milliSeconds) {
                lastCmdTime = time;
                return true;
            }
        } else if (name == cmdName && cmdData == CmdSerial_KeywordStop) {
            cmdName = "";
            serial->print(CmdSerial_StopMsg);
            serial->println(name);
            lastCmdTime = 0;
        }
        return false;
    }

    String get() {
        return cmdData;
    }

    /**
     * Method to set value in the sketch, should be called in `if` statement /not recommended for use/.
     * @param name
     * @return
     */
    bool set(const String name) {
        if (name != cmdName) return false;
        if (cmdData == CmdSerial_KeywordShow || cmdData == CmdSerial_KeywordStop) return false;
        isSet = true;
        cmdName = "";
        return true;
    }

    /**
     * Method to set data in sketch but with specific type /float/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, float &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData.toFloat();
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }

    /**
     * Method to set data in sketch but with specific type /int/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, int &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData.toInt();
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }

    /**
     * Method to set data in sketch but with specific type /uint8_t/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, uint8_t &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData.toInt();
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }

    /**
     * Method to set data in sketch but with specific type /int8_t/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, int8_t &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData.toInt();
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }

    /**
     * Method to set data in sketch but with specific type /uint16_t/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, uint16_t &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData.toInt();
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }


    /**
     * Method to set data in sketch but with specific type /double/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, double &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData.toDouble();
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }

    /**
     * Method to set data in sketch but with specific type /String/.
     * @param name
     * @param data
     * @param isMessaging
     * @return
     */
    bool set(const String name, String &data, bool isMessaging = true) {
        if (set(name)) {
            data = cmdData;
            if (isMessaging) printMessage(name);
            return true;
        }
        return false;
    }

    /**
     * Shortcut to display message with numbers.
     * @param msg
     * @param value
     */
    void print(String msg, long value) {
        Serial.print(msg);
        Serial.print(spr);
        Serial.print(value);
        Serial.println();
    }

    /**
     * Just to support bigger numbers
     * @param msg
     * @param value
     */
    void print(String msg, unsigned long value) {
        Serial.print(msg);
        Serial.print(spr);
        Serial.print(value);
        Serial.println();
    }
    /**
     * Just to support for float
     * @param msg
     * @param value
     */
    void print(String msg, float value) {
        Serial.print(msg);
        Serial.print(spr);
        Serial.print(value);
        Serial.println();
    }
    /**
     * Additional support of the number
     * @param msg
     * @param value
     */
    void print(String msg, int8_t value) {
        print(msg, (unsigned long) value);
    }
    /**
     * Additional support of the number
     * @param msg
     * @param value
     */
    void print(String msg, uint8_t value) {
        print(msg, (unsigned long) value);
    }
    /**
     * Additional support of the number
     * @param msg
     * @param value
     */
    void print(String msg, int value) {
        print(msg, (long) value);
    }

    /**
     * Additional support of the number
     * @param msg
     * @param value
     */
    void print(String msg, unsigned int value) {
        print(msg, (unsigned long) value);
    }

private:

    /**
     * Shows message as a response to the Set method.
     * @param name
     */
    void printMessage(const String name) {
        serial->print(CmdSerial_EnterMsgTitle);
        serial->print(name);
        serial->print(CmdSerial_EnterMsgValue);
        serial->println(cmdData);
        cmdData = "";
    }
};

#endif //CMDSERIAL_H
