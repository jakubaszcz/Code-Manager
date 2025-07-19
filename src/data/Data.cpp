#include "../../includes/data/Data.hpp"
#include "../../includes/error/Error.hpp"

Data::Data() {
}

Data::~Data() {
}

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


bool Data::CreateDefaultConfig() {
	// Check if file already exists
	if (std::filesystem::exists("config.cfg")) return true;

	// Create config file
	std::ofstream file("config.cfg");

	// Close file to avoid multipl openning
	file.close();
	return true;
}

void Data::LoadConfig() {

    std::ifstream file("config.cfg");
    std::string line, currentSection;

	// Looping on every file's line
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        auto pos = line.find('=');
        if (pos != std::string::npos && !currentSection.empty()) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);


            _configMap[currentSection][key] = value;
        }
    }
}
