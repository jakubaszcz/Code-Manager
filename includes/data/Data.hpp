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
    Data();
    ~Data();

    bool Initialize();
    bool CreateDefaultConfig();
    void LoadConfig();

    void ChangeConfig(const std::string&, const std::string&);
    void AddCommand();
    void RemoveCommand(const std::string&);

    std::unordered_map<std::string, std::string> GetConfigMap() const;

private:
    std::unordered_map<std::string, std::string> _configMap;
};
