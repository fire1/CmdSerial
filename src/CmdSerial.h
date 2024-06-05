// Created by fire1 on 2024-03-20.
//

#ifndef CMDSERIAL_H
#define CMDSERIAL_H

#include <Arduino.h>


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

#ifndef CmdSerial_KeywordHelp
#define CmdSerial_KeywordHelp F("help")
#endif

#ifndef CmdSerial_ShowTime
#define CmdSerial_ShowTime 500
#endif


/**
 * @class CmdSerial
 *  @brief A simple command providing class for Serial.
 *  @details
 *  @author Angel Zaprianov /fire1/
*/
class CmdSerial {
  private:
    bool isSet = false;
    bool isList = false;
    bool isHelp = false;
    Stream *serial;
    String cmdName;
    String cmdData;
    String printer;

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
        const char endOfLine = CmdSerial::EndOfLine) {
        serial = &theSerial;
        this->spr = separator;
        this->eol = endOfLine;
    }

    /**
     * Listens for Serial stream input and parses it based on methods used in this class.
     *  This method is required be called in the `loop()` function in order this class to work.
     */
    void listen() {
        //
        // Clear the command after help block is displayed.
        if (isHelp) {
            cmdData = "";
            cmdName = "";
        }

        //
        // Reset stateds
        isSet = false;
        isList = false;
        isHelp = false;


        if (serial->available() > 0) {

            //
            // Reads command name
            cmdName = serial->readStringUntil(spr);
            cmdName.trim();

            //
            // Detect help term
            if (cmdName == CmdSerial_KeywordHelp) {
                isList = true;
                serial->println(F("List of available commands:"));
                serial->println(F("---------------------------"));
                return;
            }

            //
            // Reads input data/keyword
            cmdData = serial->readStringUntil(eol);
            cmdData.trim();

            //
            // Enable help block from the keyword
            if (cmdData == CmdSerial_KeywordHelp) {
                isHelp = true;
            }
        }
    }

    /**
      * Shows data for given cycles, should be called in `if` statement.
      * @param name  The command name
      * @param milliSeconds  Time/interval used to display data /return true/
      * @param help The command help
      * @return bool
      */
    bool _show(const String name, unsigned long milliSeconds = CmdSerial_ShowTime, String help = "") {

        if (isList)
            printHelp(name, help, F("\t [show, stop] \t - "));
        if (name != cmdName && !isSet)
            return false;

        //
        // The set block
        if (cmdData == CmdSerial_KeywordShow) {
            unsigned long time = millis();
            if (cmdName == name && time - lastCmdTime >= milliSeconds) {
                lastCmdTime = time;
                return true;
            }
            //
            // Display show block
        } else if (name == cmdName && cmdData == CmdSerial_KeywordStop) {
            cmdName = "";
            serial->print(CmdSerial_StopMsg);
            serial->println(name);
            lastCmdTime = 0;

            //
            // Display help block
        } else if (name == cmdName && cmdData == CmdSerial_KeywordHelp) {
            //
            // Generating the title for help block
            String title = F("`");
            title += name;
            title += F("`\t (show) \t - ");

            serial->print(title);

            if (help.length() > 0)
                serial->println(help);
            else
                serial->print(F("Sorry, the command <help> block is empty..."));
            //
            // Display all help blocks
        }

        return false;
    }

    /**
      * Shows data with help block
    * @param name The command name
    * @param help The command help
    * @return bool
    */
    bool show(const String name) {
        return _show(name, CmdSerial_ShowTime, "");
    }

    /**
      * Shows data with help block
      * @param name The command name
      * @param help The command help
      * @return bool
      */
    bool show(const String name, String help) {
        return _show(name, CmdSerial_ShowTime, help);
    }

    /**
     * Shows data with time interval /milliSeconds/
     *  @param name The command name
     *  @param milliSeconds Time between displaing the data.
     *  @return bool
     */
    bool show(const String name, unsigned long milliSeconds) {
        return _show(name, milliSeconds, "");
    }

    String get() {
        return cmdData;
    }

    /**
     * Method to set value in the sketch, should be called in `if` statement /not recommended for use/.
     *  @param name
     *  @return
    */
    bool _set(const String name, const String help = "") {

        if (isList || isHelp && name == cmdName) {
            printHelp(name, help, F("\t (set) <value> \t - "));
            return false;
        }

        if (name != cmdName || isHelp)
            return false;

        if (cmdData == CmdSerial_KeywordShow || cmdData == CmdSerial_KeywordStop)
            return false;

        Serial.println(cmdName);
        Serial.println(cmdData);
        isSet = true;
        cmdName = "";
        return true;
    }

    /**
     * An call from single tag only
     */
    bool set(const String name) {

        if (_set(name, "")) {
            cmdData = "";
            printMessage(name);
            return true;
        }
        return false;
    }
    /**
     *  Method to set data in sketch but with specific type /float/.
     *  @param name
     *  @param data
     *  @param help
     *  @param isMessaging
     *  @return
     */
    bool set(const String name, float &data, const String help = "", bool isMessaging = true) {
        if (_set(name, help)) {
            data = cmdData.toFloat();
            if (isMessaging)
                printMessage(name);
            return true;
        }
        return false;
    }

    /**
     *  Method to set data in sketch but with specific type /int/.
     *  @param name
     *  @param data
     *  @param help
     *  @param isMessaging
     *  @return
     */
    bool set(const String name, int &data, const String help = "", bool isMessaging = true) {

        if (_set(name, help)) {
            data = cmdData.toInt();
            if (isMessaging)
                printMessage(name);
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
    bool set(const String name, uint8_t &data, const String help = "", bool isMessaging = true) {
        if (_set(name, help)) {
            data = cmdData.toInt();
            if (isMessaging)
                printMessage(name);
            return true;
        }
        return false;
    }

    /**
     *  Method to set data in sketch but with specific type /int8_t/.
     *  @param name
     *  @param data
     *  @param help
     *  @param isMessaging
     *  @return
    */
    bool set(const String name, int8_t &data, const String help = "", bool isMessaging = true) {
        if (_set(name, help)) {
            data = cmdData.toInt();
            if (isMessaging)
                printMessage(name);
            return true;
        }
        return false;
    }

    /**
     *  Method to set data in sketch but with specific type /uint16_t/.
     *  @param name
     *  @param data
     *  @param help
     *  @param isMessaging
     *  @return
     */
    bool set(const String name, uint16_t &data, const String help = "", bool isMessaging = true) {
        if (_set(name, help)) {
            data = cmdData.toInt();
            if (isMessaging)
                printMessage(name);
            return true;
        }
        return false;
    }


    /**
     *  Method to set data in sketch but with specific type /double/.
     *  @param name
     *  @param data
     *  @param help
     *  @param isMessaging
     *  @return
     */
    bool set(const String name, double &data, const String help = "", bool isMessaging = true) {
        if (_set(name, help)) {
            data = cmdData.toDouble();
            if (isMessaging)
                printMessage(name);
            return true;
        }
        return false;
    }

    /**
     *  Method to set data in sketch but with specific type /String/.
     *  @param name
     *  @param data
     *  @param help
     *  @param isMessaging
     *  @return
     */
    bool set(const String name, String &data, const String help = "", bool isMessaging = true) {
        if (_set(name, help)) {
            data = cmdData;
            if (isMessaging)
                printMessage(name);
            return true;
        }
        return false;
    }

    /**
     *  The common print function
     *  @param msg
     *  @param value
     */
    void print(String msg, String value) {
        Serial.print(msg);
        Serial.print(spr);
        Serial.print(value);
        Serial.println();
    }

    /**
     *  Shortcut to display message with numbers.
     *  @param msg
     *  @param value
     */
    void print(String msg, long value) {
        printer = String(value);
        print(msg, printer);
        printer = "";
    }

    /**
    * Just to support bigger numbers
    * @param msg
    * @param value
    */
    void print(String msg, unsigned long value) {
        printer = String(value);
        print(msg, printer);
        printer = "";
    }

    /**
    * Just to support for float
    * @param msg
    * @param value
    */
    void print(String msg, float value) {
        printer = String(value);
        print(msg, printer);
        printer = "";
    }

    /**
      * Additional support of the number
      * @param msg
      * @param value
      */
    void print(String msg, int8_t value) {
        print(msg, (unsigned long)value);
    }

    /**
     * Additional support of the number
     * @param msg
     * @param value
     */
    void print(String msg, uint8_t value) {
        print(msg, (unsigned long)value);
    }

    /**
      * Additional support of the number
      * @param msg
      * @param value
    */
    void print(String msg, int value) {
        print(msg, (long)value);
    }

    /**
        Additional support of the number
        @param msg
        @param value
    */
    void print(String msg, unsigned int value) {
        print(msg, (unsigned long)value);
    }

  private:
    /**
     * Shows message as a response to the Set method.
     *  @param name
     */
    void printMessage(const String name) {
        serial->print(CmdSerial_EnterMsgTitle);
        serial->print(name);
        serial->print(CmdSerial_EnterMsgValue);
        serial->println(cmdData);
        cmdData = "";
    }

    /**
     * Generates help block for show keyword
     * @param name Command name
     * @param help Command help
     */
    void printHelp(const String name, const String help, const String type) {

        serial->print(F("`"));
        serial->print(name);
        serial->print(F("`"));


        serial->print(type);

        if (help.length() > 0) {
            serial->println(help);
        } else
            serial->println(F("<empty>"));
    }
};

#endif //CMDSERIAL_H
