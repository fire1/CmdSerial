// Created by fire1 on 2024-03-20.
//

#ifndef CMDSERIAL_H
#define CMDSERIAL_H

#include <Arduino.h>


#ifndef CmdSerial_EnterMsgTitle
#define CmdSerial_EnterMsgTitle F("[SET] Entered  /")
#endif

#ifndef CmdSerial_EnterMsgValue
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
 * String literal `"hello"_f` for labels passed to print/set/show.
 *
 * On C++14+ (GCC character-pack UDL) the text is stored in PROGMEM on AVR,
 * same idea as F("hello"). Arduino AVR defaults to gnu++11, where that UDL
 * is unavailable, so the fallback keeps a normal const char* string instead.
 */
namespace CmdSerialLiterals {

#if __cplusplus >= 201402L

template <typename T, T... Chars>
struct FlashString {
    static const char data[sizeof...(Chars) + 1] PROGMEM;

    constexpr operator const __FlashStringHelper *() const {
        return reinterpret_cast<const __FlashStringHelper *>(data);
    }
};

template <typename T, T... Chars>
const char FlashString<T, Chars...>::data[sizeof...(Chars) + 1] PROGMEM = {Chars..., '\0'};

template <typename T, T... Chars>
constexpr FlashString<T, Chars...> operator""_f() {
    return {};
}

#else

struct FlashString {
    const char *data;
    constexpr explicit FlashString(const char *s) : data(s) {}
    constexpr operator const char *() const { return data; }
};

constexpr FlashString operator""_f(const char *s, size_t) {
    return FlashString{s};
}

#endif

} // namespace CmdSerialLiterals

using CmdSerialLiterals::operator""_f;


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
    bool _show(const String &name, unsigned long milliSeconds = CmdSerial_ShowTime, const String &help = "") {

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
            serial->print(F("`"));
            serial->print(name);
            serial->print(F("`\t (show) \t - "));

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
    template<typename Name>
    bool show(const Name &name) {
        return _show(String(name), CmdSerial_ShowTime, "");
    }

    /**
     * Shows data with time interval /milliSeconds/
     *  @param name The command name
     *  @param milliSeconds Time between displaing the data.
     *  @return bool
     */
    template<typename Name>
    bool show(const Name &name, unsigned long milliSeconds) {
        return _show(String(name), milliSeconds, "");
    }

    /**
      * Shows data with help block
      * @param name The command name
      * @param help The command help
      * @return bool
      */
    template<typename Name>
    bool show(const Name &name, const char *help) {
        return _show(String(name), CmdSerial_ShowTime, String(help));
    }

    template<typename Name>
    bool show(const Name &name, const String &help) {
        return _show(String(name), CmdSerial_ShowTime, help);
    }

    template<typename Name>
    bool show(const Name &name, const __FlashStringHelper *help) {
        return _show(String(name), CmdSerial_ShowTime, String(help));
    }

    /**
     * Shows data with help text and custom interval.
     */
    template<typename Name>
    bool show(const Name &name, const char *help, unsigned long milliSeconds) {
        return _show(String(name), milliSeconds, String(help));
    }

    template<typename Name>
    bool show(const Name &name, const String &help, unsigned long milliSeconds) {
        return _show(String(name), milliSeconds, help);
    }

    template<typename Name>
    bool show(const Name &name, const __FlashStringHelper *help, unsigned long milliSeconds) {
        return _show(String(name), milliSeconds, String(help));
    }

    String get() {
        return cmdData;
    }

    /**
     * Method to set value in the sketch, should be called in `if` statement /not recommended for use/.
     *  @param name
     *  @return
    */
    bool _set(const String &name, const String &help = "") {

        if (isList || (isHelp && name == cmdName)) {
            printHelp(name, help, F("\t (set) <value> \t - "));
            return false;
        }

        if (name != cmdName || isHelp)
            return false;

        if (cmdData == CmdSerial_KeywordShow || cmdData == CmdSerial_KeywordStop)
            return false;

        isSet = true;
        cmdName = "";
        return true;
    }

    /**
     * An call from single tag only
     */
    template<typename Name>
    bool set(const Name &name) {
        String n(name);
        if (_set(n, "")) {
            printMessage(n);
            return true;
        }
        return false;
    }

    /**
     * Set a sketch variable from Serial input. Works with numeric types and String.
     *  @param name Command name
     *  @param data Variable to update
     *  @param help Optional help text
     *  @param isMessaging Print confirmation after set (default true)
     *  @return true when the value was applied
     */
    template<typename Name, typename T>
    bool set(const Name &name, T &data, bool isMessaging = true) {
        return set(name, data, "", isMessaging);
    }

    template<typename Name, typename T, typename Help>
    bool set(const Name &name, T &data, const Help &help, bool isMessaging = true) {
        String n(name);
        if (_set(n, String(help))) {
            fromCmd(cmdData, data);
            if (isMessaging)
                printMessage(n);
            return true;
        }
        return false;
    }

    /**
     * Print a label and a value on one line: `label <sep> value`
     * Accepts any type Stream::print supports (numbers, String, F(), `"..."_f`, const char*).
     */
    template<typename Label, typename Value>
    void print(const Label &label, const Value &value) {
        serial->print(label);
        serial->print(spr);
        serial->print(value);
        serial->println();
    }

    /**
     * Print several values on one line, separated by the command separator.
     * Example: cmd.println("temp"_f, t, "hum"_f, h);
     */
    template<typename T>
    void println(const T &value) {
        serial->println(value);
    }

    template<typename T, typename... Args>
    void println(const T &first, const Args &... rest) {
        serial->print(first);
        serial->print(spr);
        println(rest...);
    }

  private:
    // Conversion helpers for set<T>()
    static void fromCmd(const String &s, String &out) { out = s; }
    static void fromCmd(const String &s, float &out) { out = s.toFloat(); }
    static void fromCmd(const String &s, double &out) { out = s.toDouble(); }
    static void fromCmd(const String &s, long &out) { out = s.toInt(); }
    static void fromCmd(const String &s, unsigned long &out) { out = strtoul(s.c_str(), nullptr, 10); }

    template<typename T>
    static void fromCmd(const String &s, T &out) {
        out = static_cast<T>(s.toInt());
    }

    /**
     * Shows message as a response to the Set method.
     *  @param name
     */
    void printMessage(const String &name) {
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
    void printHelp(const String &name, const String &help, const __FlashStringHelper *type) {

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
