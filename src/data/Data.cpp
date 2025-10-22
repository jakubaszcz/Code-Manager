#include "../../includes/data/Data.hpp"
#include "../../includes/error/Error.hpp"
#include <set>

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Data::Data() {
    _customConfigMap.clear();
    _configMap.clear();

    _customConfigMap = {
        {ConfigType::Command, "cc#"},
        {ConfigType::Application, "ca#"},
    };
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

    // Close file to avoid multiple openning
    file.close();
    return true;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::LoadConfig() {

    // Load config file
    std::ifstream file("config.cfg");
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open config.cfg" << std::endl;
        return;
    }

    // Clear for every load
    _configMap.clear();


    // Fetch the data
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


std::unordered_map<Data::ConfigType, std::string> Data::GetCustomConfigMap() const {
    return _customConfigMap;
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

    std::ofstream outfile("config.cfg", std::ios::trunc);
    for (const auto& l: lines) {
        outfile << l << "\n";
    }

    _configMap[key] = value;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::AddCommand(const std::string& data) {
    const auto& cfg = GetConfigMap();

    std::set<int> indices;
    for (const auto& [key, value] : cfg) {
        if (key.rfind(_customConfigMap[ConfigType::Command], 0) == 0) {
            try {
                int idx = std::stoi(key.substr(3));
                indices.insert(idx);
            } catch (const std::exception& e) {
                std::cerr << "Invalid key: " << key << " (" << e.what() << ")\n";
            }
        }
    }

    int nextIndex = 1;
    while (indices.count(nextIndex)) {
        ++nextIndex;
    }

    ChangeConfig(_customConfigMap[ConfigType::Command] + std::to_string(nextIndex), data);
}



// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::RemoveCommand(const std::string& key) {
    std::ifstream infile("config.cfg");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(infile, line)) {
        if (line.find(key) == std::string::npos) {
            lines.push_back(line);
        }
    }
    infile.close();

    std::ofstream outfile("config.cfg", std::ios::trunc);
    for (const auto& l : lines) {
        outfile << l << "\n";
    }

    _configMap.erase(key);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::AddApplication() {
    const auto& cfg = GetConfigMap();

    std::set<int> indices;
    for (const auto& [key, value] : cfg) {
        if (key.rfind(_customConfigMap[ConfigType::Application], 0) == 0) {
            try {
                int idx = std::stoi(key.substr(3));
                indices.insert(idx);
            } catch (const std::exception& e) {
                std::cerr << "Invalid key: " << key << " (" << e.what() << ")\n";
            }
        }
    }

    int nextIndex = 1;
    while (indices.count(nextIndex)) {
        ++nextIndex;
    }

    ChangeConfig(_customConfigMap[ConfigType::Application] + std::to_string(nextIndex), "");
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Data::RemoveApplication(const std::string& key) {
    std::ifstream infile("config.cfg");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(infile, line)) {
        if (line.find(key) == std::string::npos) {
            lines.push_back(line);
        }
    }
    infile.close();

    std::ofstream outfile("config.cfg", std::ios::trunc);
    for (const auto& l : lines) {
        outfile << l << "\n";
    }

    _configMap.erase(key);
}
