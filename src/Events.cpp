#pragma once
#include "Events.h"
#include "poisonMeterMenu.h"
#include "RE/U/UI.h"
#include "string.h"

MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
{
	static MenuOpenCloseEventHandler singleton;
	return std::addressof(singleton);
}

void MenuOpenCloseEventHandler::Register()
{
	auto ui = RE::UI::GetSingleton();
	ui->AddEventSink<RE::MenuOpenCloseEvent>(GetSingleton());
	logs::info("Registered {}", typeid(RE::MenuOpenCloseEvent).name());
}

RE::BSEventNotifyControl MenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
{

	if (a_event)
		logs::debug("Received RE::MenuOpenCloseEvent for {} with opening {}", a_event->menuName.c_str(), a_event->opening);

	// On HUD menu open/close - open/close the plugin's HUD menu
	if (a_event) {
		//if (a_event->menuName == RE::TweenMenu::MENU_NAME || a_event->menuName == RE::LockpickingMenu::MENU_NAME) {
		//	if (a_event->opening) 
		//		poisonMeterMenu::Hide();
		//	else 
		//		poisonMeterMenu::Show();
		//}
		if (a_event->opening && a_event->menuName != RE::FavoritesMenu::MENU_NAME)
			poisonMeterMenu::want_visible = false;
		else
			poisonMeterMenu::want_visible = true;

		if (a_event->menuName == RE::HUDMenu::MENU_NAME) {
			if (a_event->opening) {
				poisonMeterMenu::Show();
			}
			else {
				poisonMeterMenu::Hide();
			}
		}
		if (a_event->menuName == RE::LoadingMenu::MENU_NAME && !a_event->opening) {
			poisonMeterMenu::Show();
		}

	}


	// Hide the widgets when a menu is open
	auto controlMap = RE::ControlMap::GetSingleton();
	if (controlMap) {
		auto& priorityStack = controlMap->GetRuntimeData().contextPriorityStack;
		if (priorityStack.empty()) {
			poisonMeterMenu::want_visible = false;
		} else if (priorityStack.back() == RE::UserEvents::INPUT_CONTEXT_ID::kGameplay ||
				   priorityStack.back() == RE::UserEvents::INPUT_CONTEXT_ID::kFavorites ||
				   priorityStack.back() == RE::UserEvents::INPUT_CONTEXT_ID::kConsole) 
		{
			poisonMeterMenu::want_visible = true;
		} else {
			poisonMeterMenu::want_visible = false;
		}
		logs::debug("PoisonMeterMenu::want_Visible = {}", poisonMeterMenu::want_visible);
	}

	return RE::BSEventNotifyControl::kContinue;
}
