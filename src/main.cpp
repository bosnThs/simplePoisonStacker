#pragma once

#include <SimpleIni.h>

//std::uint32_t poisonCharges = 0;
std::uint32_t maxPoisonCharges = 0;
bool          bDisplayNotification = false;
RE::AlchemyItem* newPoison;


RE::BGSEquipSlot* leftHandSlot;
int handSlot = 0;

static void loadIni()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(L"Data\\SKSE\\Plugins\\simplePoisonStacker.ini");

	maxPoisonCharges = (int)ini.GetDoubleValue("settings", "iMaxPoisonCharges", 99);
	bDisplayNotification = ini.GetBoolValue("settings", "bDisplayNotification", false);
}

struct currentPoison
{
	static RE::AlchemyItem* thunk(RE::InventoryEntryData* poisonTargetWeapon)
	{
		//poisonCharges = 0;
		auto player = RE::PlayerCharacter::GetSingleton();
		if (player->GetEquippedEntryData(true) && player->GetEquippedEntryData(false) == poisonTargetWeapon && handSlot == 1)
		{
			poisonTargetWeapon = player->GetEquippedEntryData(true);
		}

		auto currentPoison = getCurrentPoison(poisonTargetWeapon);
		if (newPoison && currentPoison && currentPoison->poison) {
			if (currentPoison->poison == newPoison && currentPoison->count < maxPoisonCharges) {
				//poisonCharges = currentPoison->count;
				return nullptr;
			}
			else
				return currentPoison->poison;
		}
		return nullptr;
	}
	static inline REL::Relocation<decltype(thunk)> func;

	static RE::ExtraPoison* getCurrentPoison(RE::InventoryEntryData* weapon)
	{
		if (weapon->IsPoisoned()) {
			for (auto& xList : *weapon->extraLists) {
				auto xID = xList->GetByType<RE::ExtraPoison>();
				if (xID && xID->poison) {
					return xID;
				}
			}
		}
		return nullptr;
	}
};


struct finalizePoison
{
	static char thunk(RE::Actor* a_actor, RE::AlchemyItem* a_poison)
	{
		newPoison = a_poison;
		return func(a_actor, a_poison);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

struct poisonChargesMult
{
	static void thunk(RE::InventoryEntryData* poisonTargetWeapon, RE::AlchemyItem* newPoison, int charges)
	{
		//auto owner = poisonTargetWeapon->GetOwner();
		auto player = RE::PlayerCharacter::GetSingleton();
		if (player->GetEquippedEntryData(true) && player->GetEquippedEntryData(false) == poisonTargetWeapon && handSlot == 1)
		{
			poisonTargetWeapon = player->GetEquippedEntryData(true);
		}

		auto poison = currentPoison::getCurrentPoison(poisonTargetWeapon);
		if (poison) {
			if (charges + poison->count > maxPoisonCharges)
				poison->count = maxPoisonCharges;
			else
				poison->count += charges;
			return;
		}
		func(poisonTargetWeapon, newPoison, charges);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

struct PoisonTarget
{
	static void thunk(RE::Actor* a_actor, RE::InventoryEntryData* a_poisonedWeapon, std::uint32_t* a_arg2, std::uint32_t* a_arg3)
	{
		bool hasPoison = false;
		if (a_poisonedWeapon && currentPoison::getCurrentPoison(a_poisonedWeapon))
			hasPoison = true;

		func(a_actor, a_poisonedWeapon, a_arg2, a_arg3);

		if (a_actor->IsPlayerRef() && hasPoison && bDisplayNotification && a_poisonedWeapon) {
			auto poison = currentPoison::getCurrentPoison(a_poisonedWeapon);
			if (!poison) {
				auto object = a_poisonedWeapon->object;
				if (!object->IsWeapon())
					return;

				RE::DebugNotification(std::string(a_poisonedWeapon->GetDisplayName()).append(" is no longer poisoned.").c_str());
			}

		}
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

struct PoisonWeapon
{
	static char thunk(std::int64_t* a1, RE::Actor* a_actor, std::int64_t* a3, RE::BGSEquipSlot* a_equipSlot, char a5)
	{
		auto a_form = (RE::TESForm*)*a3;
		handSlot = 0;
		if (a_equipSlot && a_equipSlot == leftHandSlot && a_form && a_form->GetFormType() == RE::FormType::AlchemyItem)
			handSlot = 1;

		return func(a1, a_actor, a3, a_equipSlot, a5);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};



static void Init()
{
	loadIni();

	REL::Relocation<std::uintptr_t> targetD{ RELOCATION_ID(36976, 38001) };
	stl::write_thunk_call<finalizePoison>(targetD.address() + REL::Relocate(0x7A8, 0x733));

	REL::Relocation<std::uintptr_t> targetA{ RELOCATION_ID(39406, 40481) };
	stl::write_thunk_call<currentPoison>(targetA.address() + 0x89);

	REL::Relocation<std::uintptr_t> targetB{ RELOCATION_ID(39407, 40482) };
	stl::write_thunk_call<poisonChargesMult>(targetB.address() + REL::Relocate(0x106, 0xF0));

	REL::Relocation<std::uintptr_t> targetC{ RELOCATION_ID(37673, 38627) };
	stl::write_thunk_call<PoisonTarget>(targetC.address() + REL::Relocate(0x185, 0x194));


	if (REL::Module::IsAE()) {
		constexpr std::array locationsA{
			std::make_pair<std::uint64_t, std::size_t>(40570, 0xf2),
			std::make_pair<std::uint64_t, std::size_t>(51149, 0x76),
			std::make_pair<std::uint64_t, std::size_t>(51543, 0x235),
			std::make_pair<std::uint64_t, std::size_t>(51548, 0xc2),  //8ba080
			std::make_pair<std::uint64_t, std::size_t>(51870, 0x85),  //8ba080
		};
		for (const auto& [id, offset] : locationsA) {
			REL::Relocation<std::uintptr_t> target(REL::ID(id), offset);
			stl::write_thunk_call<PoisonWeapon>(target.address());
		}
	}
	else {
		constexpr std::array locationsA{
			std::make_pair<std::uint64_t, std::size_t>(39491, 0xf2),
			std::make_pair<std::uint64_t, std::size_t>(50649, 0x236),
			std::make_pair<std::uint64_t, std::size_t>(50654, 0xc4),
			std::make_pair<std::uint64_t, std::size_t>(50991, 0x8f),
		};
		for (const auto& [id, offset] : locationsA) {
			REL::Relocation<std::uintptr_t> target(REL::ID(id), offset);
			stl::write_thunk_call<PoisonWeapon>(target.address());
		}
	}


	auto g_message = SKSE::GetMessagingInterface();
	g_message->RegisterListener([](SKSE::MessagingInterface::Message* msg) -> void
		{
			if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
				RE::BSInputDeviceManager* inputEventDispatcher = RE::BSInputDeviceManager::GetSingleton();
				if (inputEventDispatcher) {
					auto dataHandler = RE::TESDataHandler::GetSingleton();
					leftHandSlot = dataHandler->LookupForm<RE::BGSEquipSlot>(0x13f43, "Skyrim.esm");
				}
			}
		});
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	Init();

	return true;
}
