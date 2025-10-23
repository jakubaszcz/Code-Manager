#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include "IPopup.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class NamePopup : public IPopup {
public:
    explicit NamePopup(QWidget *parent, const std::shared_ptr<Application>& application);
    const std::string& GetName() const;

protected:
    void Draw() override;
    void DrawStyling() override;

private:
    std::string _name;
};
