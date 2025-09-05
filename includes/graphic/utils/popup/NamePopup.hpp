#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include "IPopup.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class NamePopup : public IPopup {
public:
    explicit NamePopup(QWidget *parent = nullptr);
    const std::string& GetName() const;

protected:
    void Draw() override;
    void DrawStyling() override;

private:
    std::string _name;
};
