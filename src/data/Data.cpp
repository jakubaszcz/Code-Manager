#include "../../includes/data/Data.hpp"
#include "../../includes/error/Error.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Data::Data() {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Data::~Data() {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


bool Data::Initialize() {

    if (!CreateDefaultConfig()) {
        std::cout << ">> CreateDefaultConfig() failed" << std::endl;
        return false;
    } else {
        // Load every file informations
        LoadConfig();
    }
    return true;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


bool Data::CreateDefaultConfig() {
    // Check if file already exists
    if (std::filesystem::exists("config.cfg"))
        return true;

    // Create config file
    std::ofstream file("config.cfg");

    // Close file to avoid multipl openning
    file.close();
    return true;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::LoadConfig() {
    std::ifstream file("config.cfg");
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open config.cfg" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            _configMap[key] = value;
        }
    }
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


std::unordered_map<std::string, std::string> Data::GetConfigMap() const {
    return _configMap;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::ChangeConfig(const std::string& key, const std::string& value) {
    std::ifstream infile("config.cfg");
    std::vector<std::string> lines;
    bool keyFound = false;

    if (!infile.is_open()) {
        std::cerr << "ERROR: Cannot open config.cfg" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string currentKey = line.substr(0, pos);
            if (currentKey == key) {
                lines.push_back(key + "=" + value);
                keyFound = true;
            } else {
                lines.push_back(line);
            }
        } else {
            lines.push_back(line);
        }
    }
    infile.close();

    if (!keyFound) {
        lines.push_back(key + "=" + value);
    }

    // Réécrit tout le fichier
    std::ofstream outfile("config.cfg", std::ios::trunc);
    for (const auto& l: lines) {
        outfile << l << "\n";
    }

    _configMap[key] = value;
}
