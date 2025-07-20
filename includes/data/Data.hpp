#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

class Data {
public:
    Data();
    ~Data();

    bool Initialize();
    bool CreateDefaultConfig();
    void LoadConfig();

    std::unordered_map<std::string, std::string> GetConfigMap() const;

private:
    std::unordered_map<std::string, std::string> _configMap;
};
