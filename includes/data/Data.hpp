#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

using SectionMap = std::unordered_map<std::string, std::string>;
using ConfigMap = std::unordered_map<std::string, SectionMap>;

class Data {
public:
    Data();
    ~Data();

    bool Initialize();
    bool CreateDefaultConfig();
    void LoadConfig();

private:
    ConfigMap _configMap;
};
