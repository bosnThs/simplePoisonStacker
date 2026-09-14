#include "UI.h"

void UI::Register() {
	LoadSettings("Settings");

    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }
    SKSEMenuFramework::SetSection("SimplePoisonStacker");
    SKSEMenuFramework::AddSectionItem("Settings", Settings::Render);
	SKSEMenuFramework::AddEvent(UI::Settings::OnEvent, 0);
	SKSEMenuFramework::AddSectionItem("HUD", HUD::Render);
}

void UI::LoadSettings(const char* a_section) {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(Configuration::Settings::iniFilePath.c_str());
	if (a_section != "defaults")
	{
		Configuration::Settings::bDisplayNotification = ini.GetBoolValue("Settings", "bDisplayNotification", true);
		Configuration::Settings::fAlchemyMod = static_cast<float>(ini.GetDoubleValue("Settings", "fAlchemyMod", 0.4));
	}

	Configuration::Settings::fRightWidgetXpos = static_cast<float>(ini.GetDoubleValue(a_section, "fRightWidgetXpos", 10.0));
	Configuration::Settings::fRightWidgetYpos = static_cast<float>(ini.GetDoubleValue(a_section, "fRightWidgetYpos", 10.0));
	Configuration::Settings::fRightWidgetRotation = static_cast<float>(ini.GetDoubleValue(a_section, "fRightWidgetRotation", 0.0));
	Configuration::Settings::fRightWidgetXscale = static_cast<float>(ini.GetDoubleValue(a_section, "fRightWidgetXscale", 75.0));
	Configuration::Settings::fRightWidgetYscale = static_cast<float>(ini.GetDoubleValue(a_section, "fRightWidgetYscale", 75.0));
    Configuration::Settings::bRightWidgetBarGrowth = ini.GetBoolValue(a_section, "bRightWidgetBarGrowth", false);

	Configuration::Settings::fLeftWidgetXpos = static_cast<float>(ini.GetDoubleValue(a_section, "fLeftWidgetXpos", 50.0));
	Configuration::Settings::fLeftWidgetYpos = static_cast<float>(ini.GetDoubleValue(a_section, "fLeftWidgetYpos", 96.3));
	Configuration::Settings::fLeftWidgetRotation = static_cast<float>(ini.GetDoubleValue(a_section, "fLeftWidgetRotation", 0.0));
	Configuration::Settings::fLeftWidgetXscale = static_cast<float>(ini.GetDoubleValue(a_section, "fLeftWidgetXscale", 75.0));
	Configuration::Settings::fLeftWidgetYscale = static_cast<float>(ini.GetDoubleValue(a_section, "fLeftWidgetYscale", 75.0));
	Configuration::Settings::bLeftWidgetBarGrowth = ini.GetBoolValue(a_section, "bLeftWidgetBarGrowth", false);
	Configuration::Settings::iWidgetBarColor = static_cast<std::uint32_t>(ini.GetDoubleValue(a_section, "iWidgetBarColor", 0x9966ff));
	Configuration::Settings::iWidgetBorderColor = static_cast<std::uint32_t>(ini.GetDoubleValue(a_section, "iWidgetBorderColor", 0xDCDCDC));
	Configuration::Settings::iWidgetBGColor = static_cast<std::uint32_t>(ini.GetDoubleValue(a_section, "iWidgetBGColor", 0x0A0A0A));
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

	ini.SetDoubleValue("Settings", "iWidgetBarColor", static_cast<double>(Configuration::Settings::iWidgetBarColor));
	ini.SetDoubleValue("Settings", "iWidgetBorderColor", static_cast<double>(Configuration::Settings::iWidgetBorderColor));
	ini.SetDoubleValue("Settings", "iWidgetBGColor", static_cast<double>(Configuration::Settings::iWidgetBGColor));
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

void UI::setWidgetColor(const char* label, int& color) {
	float col[3] = {
		((color >> 16) & 0xFF) / 255.0f,
		((color >> 8) & 0xFF) / 255.0f,
		(color & 0xFF) / 255.0f
	};
	if (ImGuiMCP::ColorEdit3(label, col)) {
		color = (static_cast<int>(col[0] * 255.0f) << 16) |
				(static_cast<int>(col[1] * 255.0f) << 8) |
				static_cast<int>(col[2] * 255.0f);
	}
}

void UI::HUD::Render() {
	ImGuiMCP::Text("HUD Settings");
	ImGuiMCP::SliderFloat("Right Widget X Position", &Configuration::Settings::fRightWidgetXpos, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::SliderFloat("Right Widget Y Position", &Configuration::Settings::fRightWidgetYpos, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::SliderFloat("Right Widget Rotation", &Configuration::Settings::fRightWidgetRotation, 0.0f, 360.0f, "%.2f");
	ImGuiMCP::SliderFloat("Right Widget X Scale", &Configuration::Settings::fRightWidgetXscale, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::SliderFloat("Right Widget Y Scale", &Configuration::Settings::fRightWidgetYscale, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::Checkbox("Invert Right Widget Bar Growth", &Configuration::Settings::bRightWidgetBarGrowth);
	ImGuiMCP::SliderFloat("Left Widget X Position", &Configuration::Settings::fLeftWidgetXpos, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::SliderFloat("Left Widget Y Position", &Configuration::Settings::fLeftWidgetYpos, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::SliderFloat("Left Widget Rotation", &Configuration::Settings::fLeftWidgetRotation, 0.0f, 360.0f, "%.2f");
	ImGuiMCP::SliderFloat("Left Widget X Scale", &Configuration::Settings::fLeftWidgetXscale, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::SliderFloat("Left Widget Y Scale", &Configuration::Settings::fLeftWidgetYscale, 0.0f, 100.0f, "%.2f");
	ImGuiMCP::Checkbox("Invert Left Widget Bar Growth", &Configuration::Settings::bLeftWidgetBarGrowth);
	
	setWidgetColor("Widget Bar Color", Configuration::Settings::iWidgetBarColor);
	setWidgetColor("Widget Border Color", Configuration::Settings::iWidgetBorderColor);
	setWidgetColor("Widget Background Color", Configuration::Settings::iWidgetBGColor);

	if(ImGuiMCP::Button("Defaults"))
	{
		UI::LoadSettings("defaults");
	}
}
