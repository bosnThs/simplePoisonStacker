#include <chrono>
#include "poisonMeterMenu.h"


poisonMeterMenu::poisonMeterMenu()
{
	auto scaleformManager = RE::BSScaleformManager::GetSingleton();

	inputContext = Context::kNone;
	depthPriority = 0;

	menuFlags.set(RE::UI_MENU_FLAGS::kAlwaysOpen);
	menuFlags.set(RE::UI_MENU_FLAGS::kRequiresUpdate);
	menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);

	if (uiMovie) {
		uiMovie->SetMouseCursorCount(0);  // disable input
	}

	scaleformManager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
		a_def->SetState(RE::GFxState::StateType::kLog,
			RE::make_gptr<Logger>().get());
		});
}


void poisonMeterMenu::Register()
{
	auto ui = RE::UI::GetSingleton();
	if (ui) {
		ui->Register(MENU_NAME, Creator);
		logs::info("Menu Registered.");
	}
}


void poisonMeterMenu::Show()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
	}
}


void poisonMeterMenu::Hide()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}
}


struct detail
{
	static int getPoisonCharges(RE::InventoryEntryData* a_EntryData)
	{
		if (a_EntryData && a_EntryData->IsPoisoned()) {
			for (auto& xList : *a_EntryData->extraLists) {
				auto xID = xList->GetByType<RE::ExtraPoison>();
				if (xID && xID->poison) {
					return xID->count;
				}
			}
		}
		return 0;
	}

	static double get_poisonCharges_pct(RE::PlayerCharacter* a_player, bool a_leftHand)
	{
		return ((double)getPoisonCharges(a_player->GetEquippedEntryData(a_leftHand)) / Configuration::Settings::getMaxCharges(a_player)) * 100.0;
	}
};

void poisonMeterMenu::setFavoritesMenuOpen(bool a_bool)
{
	isOnFavorites = a_bool;
}
bool poisonMeterMenu::getFavoritesMenuOpen()
{
	return isOnFavorites;
}


// apply location, rotations and scale settings to menu.
void poisonMeterMenu::drawMeter(RE::GPtr<RE::IMenu> poisonMeter, bool a_leftHand, std::uint32_t a_currentTime)
{
	if (!poisonMeter || !poisonMeter->uiMovie)
		return;

	//if (Settings::GetSingleton()->bReposition)
	//	Settings::GetSingleton()->Load();

	auto def = poisonMeter->uiMovie->GetMovieDef();
	float xPos, yPos, rot, xScale, yScale;
	int barColor = Configuration::Settings::iWidgetBarColor * 4294967040;
	int borderColor =Configuration::Settings::iWidgetBorderColor * 4294967040;
	int bgColor = Configuration::Settings::iWidgetBGColor * 4294967040;
	bool barGrowth;
	if (!a_leftHand)
	{ 
		xPos = Configuration::Settings::fRightWidgetXpos;
		yPos = Configuration::Settings::fRightWidgetYpos;
		rot = Configuration::Settings::fRightWidgetRotation;
		xScale = Configuration::Settings::fRightWidgetXscale;
		yScale = Configuration::Settings::fRightWidgetYscale;
		barGrowth = Configuration::Settings::bRightWidgetBarGrowth;
	}
	else
	{
		xPos = Configuration::Settings::fLeftWidgetXpos;
		yPos = Configuration::Settings::fLeftWidgetYpos;
		rot = Configuration::Settings::fLeftWidgetRotation;
		xScale = Configuration::Settings::fLeftWidgetXscale;
		yScale = Configuration::Settings::fLeftWidgetYscale;
		barGrowth = Configuration::Settings::bLeftWidgetBarGrowth;
	}

	bool fadeOut = false;
	auto player = RE::PlayerCharacter::GetSingleton();
	auto fillPct = detail::get_poisonCharges_pct(player, a_leftHand);
	if (!player->AsActorState()->IsWeaponDrawn() || fillPct == 0)
		fadeOut = true;

	const RE::GFxValue widget_xpos = (xPos / 100.0) * def->GetWidth();
	const RE::GFxValue widget_ypos = (yPos / 100.0) * def->GetHeight();
	const RE::GFxValue widget_rotation = rot;
	const RE::GFxValue widget_xscale = xScale * 100;
	const RE::GFxValue widget_yscale = yScale * 100;
	const RE::GFxValue widget_barColor = barColor;
	const RE::GFxValue widget_borderColor = borderColor;
	const RE::GFxValue widget_bgColor = bgColor;
	const RE::GFxValue widget_barGrowth = barGrowth;
	const RE::GFxValue widget_leftHand = a_leftHand;
	const RE::GFxValue widget_fadeOut = fadeOut;
	const RE::GFxValue widget_currentTime = a_currentTime;
	RE::GFxValue posArray[11]{ widget_xpos, widget_ypos, widget_rotation, widget_xscale, widget_yscale, widget_barColor, widget_borderColor, widget_bgColor, widget_leftHand, widget_fadeOut, widget_currentTime };
	poisonMeter->uiMovie->Invoke("main.setPosition", nullptr, posArray, 11);


	const RE::GFxValue widget_fillPct = fillPct;
	RE::GFxValue currentChargesAmount[4]{ widget_fillPct , widget_leftHand , widget_currentTime, widget_barGrowth };

	if (!fadeOut)
		poisonMeter->uiMovie->Invoke("main.setMeterPercent", nullptr, currentChargesAmount, 4);
}

void poisonMeterMenu::Update(std::uint32_t a_currentTime)
{
	RE::GPtr<RE::IMenu> poisonMeter = RE::UI::GetSingleton()->GetMenu(poisonMeterMenu::MENU_NAME);
	if (!poisonMeter || !poisonMeter->uiMovie)
		return;

	if (!want_visible)
	{
		SetMenuVisibilityMode(MenuVisibilityMode::kHidden);
		poisonMeter->uiMovie->Invoke("main.doHide", nullptr, nullptr, 0);
		return;
	}

	SetMenuVisibilityMode(MenuVisibilityMode::kVisible);
	poisonMeter->uiMovie->Invoke("main.doShow", nullptr, nullptr, 0);

	drawMeter(poisonMeter, false, a_currentTime);
	drawMeter(poisonMeter, true, a_currentTime);
}

// Every time a new frame of the menu is rendered call the update function.
void poisonMeterMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime)
{
	poisonMeterMenu::Update(a_currentTime);
	RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
}

RE::UI_MESSAGE_RESULTS poisonMeterMenu::ProcessMessage(RE::UIMessage& a_message)
{
	using Type = RE::UI_MESSAGE_TYPE;

	switch (*a_message.type) {
	case Type::kShow:
		OnOpen();
		return RE::IMenu::ProcessMessage(a_message);
	case Type::kHide:
		OnClose();
		return RE::IMenu::ProcessMessage(a_message);
	default:
		return RE::IMenu::ProcessMessage(a_message);
	}
}

bool poisonMeterMenu::IsOpen() const
{
	return _bIsOpen;
}

void poisonMeterMenu::OnOpen()
{
	// The advance movie process will handle showing the meter when appropriate
	poisonMeterMenu::SetMenuVisibilityMode(poisonMeterMenu::MenuVisibilityMode::kHidden);
	_bIsOpen = true;
}

void poisonMeterMenu::OnClose()
{
	want_visible = false;
	_bIsOpen = false;
}


RE::GPtr<poisonMeterMenu> poisonMeterMenu::GetpoisonMeterMenu()
{
	auto ui = RE::UI::GetSingleton();
	return ui ? ui->GetMenu<poisonMeterMenu>(poisonMeterMenu::MENU_NAME) : nullptr;
}

void poisonMeterMenu::SetMenuVisibilityMode(MenuVisibilityMode a_mode)
{
	auto menu = GetpoisonMeterMenu();
	if (menu) {
		auto _view = menu->uiMovie;

		if (_view) {
			switch (a_mode) {
			case MenuVisibilityMode::kHidden:
				if (menu->_menuVisibilityMode == MenuVisibilityMode::kVisible) {
					_view->SetVisible(false);
					menu->_menuVisibilityMode = a_mode;
				}
				break;

			case MenuVisibilityMode::kVisible:
				if (menu->_menuVisibilityMode == MenuVisibilityMode::kHidden && want_visible) {
					_view->SetVisible(true);
					menu->_menuVisibilityMode = a_mode;
				}
				break;
			}
		}
	}
}