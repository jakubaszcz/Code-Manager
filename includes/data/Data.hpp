#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Data {
public:

    enum class ConfigType {
        Command,
        Application
    };

    Data();
    ~Data();

    bool Initialize();
    bool CreateDefaultConfig();
    void LoadConfig();

    void ChangeConfig(const std::string&, const std::string&);

    void AddCommand();
    void RemoveCommand(const std::string&);

    void AddApplication();
    void RemoveApplication(const std::string&);

    std::unordered_map<std::string, std::string> GetConfigMap() const;
    std::unordered_map<ConfigType, std::string> GetCustomConfigMap() const;

private:
    std::unordered_map<std::string, std::string> _configMap;
    std::unordered_map<ConfigType, std::string> _customConfigMap;
};
