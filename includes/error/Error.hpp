#pragma once

#include <exception>
#include <string>
#include <sstream>

class Error : public std::exception {
    private:
        std::string fullMessage;
    public:
        Error(const std::string& msg, const std::string& file, int line) {
            std::ostringstream oss;
                oss << "[Erreur] " << msg << " (" << file << ":" << line << ")";
            fullMessage = oss.str();
        }

        const char* what() const noexcept override {
            return fullMessage.c_str();
        }
};

#define THROW_ERROR(msg) throw Error((msg), __FILE__, __LINE__)