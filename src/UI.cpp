#include "UI.h"

void UI::Register() {
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }
    SKSEMenuFramework::SetSection("SimplePoisonStacker");
    SKSEMenuFramework::AddSectionItem("Settings", Settings::Render);
    SKSEMenuFramework::AddEvent(UI::Settings::OnEvent, 0);
}

void UI::LoadSettings() {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(Configuration::Settings::iniFilePath.c_str());
    Configuration::Settings::iMaxPoisonCharges = ini.GetLongValue("Settings", "iMaxPoisonCharges", 99);
    Configuration::Settings::bDisplayNotification = ini.GetBoolValue("Settings", "bDisplayNotification", true);
}

void UI::SaveSettings() {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(Configuration::Settings::iniFilePath.c_str());
    ini.SetLongValue("Settings", "iMaxPoisonCharges", Configuration::Settings::iMaxPoisonCharges);
    ini.SetBoolValue("Settings", "bDisplayNotification", Configuration::Settings::bDisplayNotification);
    ini.SaveFile(Configuration::Settings::iniFilePath.c_str());
}

void UI::Settings::Render() {
    ImGuiMCP::Text("Max Poison Charges:");
    ImGuiMCP::SliderInt("", &Configuration::Settings::iMaxPoisonCharges, 0, 99);
	ImGuiMCP::Checkbox("Display Notifications", &Configuration::Settings::bDisplayNotification);
}

void __stdcall UI::Settings::OnEvent(SKSEMenuFramework::Model::EventType eventType) {
    if (SKSEMenuFramework::Model::EventType::kCloseMenu == eventType) {
        UI::SaveSettings();
    }
}
