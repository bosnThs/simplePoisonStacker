#include "UI.h"

void UI::Register() {
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }
    SKSEMenuFramework::SetSection("SimplePoisonStacker");
    SKSEMenuFramework::AddSectionItem("Settings", Settings::Render);
	SKSEMenuFramework::AddSectionItem("HUD", Settings::Render);
    SKSEMenuFramework::AddEvent(UI::Settings::OnEvent, 0);
}

void UI::LoadSettings() {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(Configuration::Settings::iniFilePath.c_str());
    Configuration::Settings::bDisplayNotification = ini.GetBoolValue("Settings", "bDisplayNotification", true);
    Configuration::Settings::fAlchemyMod = static_cast<float>(ini.GetDoubleValue("Settings", "fAlchemyMod", 0.4));

	Configuration::Settings::fRightWidgetXpos = static_cast<float>(ini.GetDoubleValue("Settings", "fRightWidgetXpos", 10.0));
	Configuration::Settings::fRightWidgetYpos = static_cast<float>(ini.GetDoubleValue("Settings", "fRightWidgetYpos", 10.0));
	Configuration::Settings::fRightWidgetRotation = static_cast<float>(ini.GetDoubleValue("Settings", "fRightWidgetRotation", 0.0));
	Configuration::Settings::fRightWidgetXscale = static_cast<float>(ini.GetDoubleValue("Settings", "fRightWidgetXscale", 75.0));
	Configuration::Settings::fRightWidgetYscale = static_cast<float>(ini.GetDoubleValue("Settings", "fRightWidgetYscale", 75.0));
    Configuration::Settings::bRightWidgetBarGrowth = ini.GetBoolValue("Settings", "bRightWidgetBarGrowth", false);

	Configuration::Settings::fLeftWidgetXpos = static_cast<float>(ini.GetDoubleValue("Settings", "fLeftWidgetXpos", 50.0));
	Configuration::Settings::fLeftWidgetYpos = static_cast<float>(ini.GetDoubleValue("Settings", "fLeftWidgetYpos", 96.3));
	Configuration::Settings::fLeftWidgetRotation = static_cast<float>(ini.GetDoubleValue("Settings", "fLeftWidgetRotation", 0.0));
	Configuration::Settings::fLeftWidgetXscale = static_cast<float>(ini.GetDoubleValue("Settings", "fLeftWidgetXscale", 75.0));
	Configuration::Settings::fLeftWidgetYscale = static_cast<float>(ini.GetDoubleValue("Settings", "fLeftWidgetYscale", 75.0));
	Configuration::Settings::bLeftWidgetBarGrowth = ini.GetBoolValue("Settings", "bLeftWidgetBarGrowth", false);

	Configuration::Settings::iWidgetBarColor = static_cast<std::uint32_t>(ini.GetDoubleValue("Settings", "iWidgetBarColor", 0x9966ff));
	Configuration::Settings::iWidgetBorderColor = static_cast<std::uint32_t>(ini.GetDoubleValue("Settings", "iWidgetBorderColor", 0xDCDCDC));
	Configuration::Settings::iWidgetBGColor = static_cast<std::uint32_t>(ini.GetDoubleValue("Settings", "iWidgetBGColor", 0x0A0A0A));
}

void UI::LoadDefaults() {
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(Configuration::Settings::iniFilePath.c_str());
	Configuration::Settings::bDisplayNotification = ini.GetBoolValue("defaults", "bDisplayNotification", true);
	Configuration::Settings::fAlchemyMod = static_cast<float>(ini.GetDoubleValue("defaults", "fAlchemyMod", 0.4));

	Configuration::Settings::fRightWidgetXpos = static_cast<float>(ini.GetDoubleValue("defaults", "fRightWidgetXpos", 10.0));
	Configuration::Settings::fRightWidgetYpos = static_cast<float>(ini.GetDoubleValue("defaults", "fRightWidgetYpos", 10.0));
	Configuration::Settings::fRightWidgetRotation = static_cast<float>(ini.GetDoubleValue("defaults", "fRightWidgetRotation", 0.0));
	Configuration::Settings::fRightWidgetXscale = static_cast<float>(ini.GetDoubleValue("defaults", "fRightWidgetXscale", 75.0));
	Configuration::Settings::fRightWidgetYscale = static_cast<float>(ini.GetDoubleValue("defaults", "fRightWidgetYscale", 75.0));
	Configuration::Settings::bRightWidgetBarGrowth = ini.GetBoolValue("defaults", "bRightWidgetBarGrowth", false);

	Configuration::Settings::fLeftWidgetXpos = static_cast<float>(ini.GetDoubleValue("defaults", "fLeftWidgetXpos", 50.0));
	Configuration::Settings::fLeftWidgetYpos = static_cast<float>(ini.GetDoubleValue("defaults", "fLeftWidgetYpos", 96.3));
	Configuration::Settings::fLeftWidgetRotation = static_cast<float>(ini.GetDoubleValue("defaults", "fLeftWidgetRotation", 0.0));
	Configuration::Settings::fLeftWidgetXscale = static_cast<float>(ini.GetDoubleValue("defaults", "fLeftWidgetXscale", 75.0));
	Configuration::Settings::fLeftWidgetYscale = static_cast<float>(ini.GetDoubleValue("defaults", "fLeftWidgetYscale", 75.0));
	Configuration::Settings::bLeftWidgetBarGrowth = ini.GetBoolValue("defaults", "bLeftWidgetBarGrowth", false);

	//Configuration::Settings::iWidgetBarColor = static_cast<float>(ini.GetDoubleValue("defaults", "iWidgetBarColor", 10053375.0));
	//Configuration::Settings::iWidgetBorderColor = static_cast<float>(ini.GetDoubleValue("defaults", "iWidgetBorderColor", 14540252.0));
	//Configuration::Settings::iWidgetBGColor = static_cast<float>(ini.GetDoubleValue("defaults", "iWidgetBGColor", 657930.0));
}

void UI::SaveSettings() {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(Configuration::Settings::iniFilePath.c_str());
    ini.SetBoolValue("Settings", "bDisplayNotification", Configuration::Settings::bDisplayNotification);
    ini.SetDoubleValue("Settings", "fAlchemyMod", static_cast<double>(Configuration::Settings::fAlchemyMod));

	ini.SetDoubleValue("Settings", "fRightWidgetXpos", static_cast<double>(Configuration::Settings::fRightWidgetXpos));
	ini.SetDoubleValue("Settings", "fRightWidgetYpos", static_cast<double>(Configuration::Settings::fRightWidgetYpos));
	ini.SetDoubleValue("Settings", "fRightWidgetRotation", static_cast<double>(Configuration::Settings::fRightWidgetRotation));
	ini.SetDoubleValue("Settings", "fRightWidgetXscale", static_cast<double>(Configuration::Settings::fRightWidgetXscale));
	ini.SetDoubleValue("Settings", "fRightWidgetYscale", static_cast<double>(Configuration::Settings::fRightWidgetYscale));
	ini.SetBoolValue("Settings", "bRightWidgetBarGrowth", Configuration::Settings::bRightWidgetBarGrowth);

	ini.SetDoubleValue("Settings", "fLeftWidgetXpos", static_cast<double>(Configuration::Settings::fLeftWidgetXpos));
	ini.SetDoubleValue("Settings", "fLeftWidgetYpos", static_cast<double>(Configuration::Settings::fLeftWidgetYpos));
	ini.SetDoubleValue("Settings", "fLeftWidgetRotation", static_cast<double>(Configuration::Settings::fLeftWidgetRotation));
	ini.SetDoubleValue("Settings", "fLeftWidgetXscale", static_cast<double>(Configuration::Settings::fLeftWidgetXscale));
	ini.SetDoubleValue("Settings", "fLeftWidgetYscale", static_cast<double>(Configuration::Settings::fLeftWidgetYscale));
	ini.SetBoolValue("Settings", "bLeftWidgetBarGrowth", Configuration::Settings::bLeftWidgetBarGrowth);

	//ini.SetDoubleValue("Settings", "iWidgetBarColor", static_cast<double>(Configuration::Settings::iWidgetBarColor));
	//ini.SetDoubleValue("Settings", "iWidgetBorderColor", static_cast<double>(Configuration::Settings::iWidgetBorderColor));
	//ini.SetDoubleValue("Settings", "iWidgetBGColor", static_cast<double>(Configuration::Settings::iWidgetBGColor));
    ini.SaveFile(Configuration::Settings::iniFilePath.c_str());
}

void UI::Settings::Render() {
    ImGuiMCP::Text("Max Poison Charges = Alchemy Skill * Modifier");
	ImGuiMCP::SliderFloat("Modifier", &Configuration::Settings::fAlchemyMod, 0.0f, 1.0f, "%.2f");
	ImGuiMCP::Checkbox("Display Notifications", &Configuration::Settings::bDisplayNotification);
}

void __stdcall UI::Settings::OnEvent(SKSEMenuFramework::Model::EventType eventType) {
    if (SKSEMenuFramework::Model::EventType::kCloseMenu == eventType) {
        UI::SaveSettings();
    }
}

void UI::HUD::Render() {
	ImGuiMCP::Text("HUD Settings");
	ImGuiMCP::Text("Right Widget X Position: %.2f", Configuration::Settings::fRightWidgetXpos);
	ImGuiMCP::Text("Right Widget Y Position: %.2f", Configuration::Settings::fRightWidgetYpos);
	ImGuiMCP::Text("Right Widget Rotation: %.2f", Configuration::Settings::fRightWidgetRotation);
	ImGuiMCP::Text("Right Widget X Scale: %.2f", Configuration::Settings::fRightWidgetXscale);
	ImGuiMCP::Text("Right Widget Y Scale: %.2f", Configuration::Settings::fRightWidgetYscale);
	ImGuiMCP::Text("Invert Right Widget Bar Growth: %s", Configuration::Settings::bRightWidgetBarGrowth ? "True" : "False");
	ImGuiMCP::Text("Left Widget X Position: %.2f", Configuration::Settings::fLeftWidgetXpos);
	ImGuiMCP::Text("Left Widget Y Position: %.2f", Configuration::Settings::fLeftWidgetYpos);
	ImGuiMCP::Text("Left Widget Rotation: %.2f", Configuration::Settings::fLeftWidgetRotation);
	ImGuiMCP::Text("Left Widget X Scale: %.2f", Configuration::Settings::fLeftWidgetXscale);
	ImGuiMCP::Text("Left Widget Y Scale: %.2f", Configuration::Settings::fLeftWidgetYscale);
	ImGuiMCP::Text("Invert Left Widget Bar Growth: %s", Configuration::Settings::bLeftWidgetBarGrowth ? "True" : "False");
	ImGuiMCP::Text("Bar Color: %d", static_cast<int>(Configuration::Settings::iWidgetBarColor));
	ImGuiMCP::Text("Border Color: %d", static_cast<int>(Configuration::Settings::iWidgetBorderColor));
	ImGuiMCP::Text("Background Color: %d", static_cast<int>(Configuration::Settings::iWidgetBGColor));
}

void __stdcall UI::HUD::OnEvent(SKSEMenuFramework::Model::EventType eventType) {
	if (SKSEMenuFramework::Model::EventType::kCloseMenu == eventType) {
		UI::SaveSettings();
	}
}
