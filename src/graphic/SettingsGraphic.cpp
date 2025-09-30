#include "../../includes/graphic/SettingsGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


SettingsGraphic::SettingsGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(std::move(application)) {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void SettingsGraphic::Draw(QVBoxLayout *layout) {}