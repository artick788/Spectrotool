#pragma once

#include "../Defs.hpp"

namespace Spectrotool {

    struct ParserMessage {
        std::string severity;
        std::string message;
        std::string address;
        std::string sheet;
    };

    class ParserMessages {
    public:
        ParserMessages() = default;

        ~ParserMessages() = default;

        const std::vector<ParserMessage>& getMessages() const { return m_Messages; }

        auto begin() { return m_Messages.begin(); }

        auto end() { return m_Messages.end(); }

        void addInfo(const std::string& message, const std::string &address = "", const std::string& sheetName = "") {
            ParserMessage msg;
            msg.severity = "INFO";
            msg.message = message;
            msg.address = address;
            msg.sheet = sheetName;
            m_Messages.push_back(msg);
        }

        void addWarning(const std::string& message, const std::string &address = "", const std::string& sheetName = "") {
            ParserMessage msg;
            msg.severity = "WARNING";
            msg.message = message;
            msg.address = address;
            msg.sheet = sheetName;
            m_Messages.push_back(msg);
        }

        void addError(const std::string& message, const std::string& address = "", const std::string& sheetName = "") {
            ParserMessage msg;
            msg.severity = "ERROR";
            msg.message = message;
            msg.address = address;
            msg.sheet = sheetName;
            m_Messages.push_back(msg);
        }

    private:
        std::vector<ParserMessage> m_Messages;


    };
}