#pragma once

#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <memory>
#include <vector>
#include "../audio/Audio.hpp"
#include "../data/Data.hpp"
#include "../software/Software.hpp"

class Software;


class Application : public std::enable_shared_from_this<Application> {
public:
    Application();
    ~Application();


    bool Initialize();
    int Run(int argc, char *argv[]);
    std::shared_ptr<Data> GetData() const;
    std::shared_ptr<Audio> GetAudio() const;

private:
    // Data class
    std::shared_ptr<Data> _data;
    std::shared_ptr<Audio> _audio;
    std::unique_ptr<Software> _software;

protected:
};
