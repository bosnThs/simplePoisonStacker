#include <shared_mutex>

int poisonCharges = 0;

struct currentPoison
{
	static RE::AlchemyItem* thunk(RE::InventoryEntryData* poisonTargetWeapon, std::uint32_t*, std::uint32_t*, std::uint32_t*, std::uint32_t*, std::uint32_t*, RE::AlchemyItem* newPoison)
	{
		poisonCharges = 0;
		auto currentPoison = getCurrentPoison(poisonTargetWeapon);
		if (currentPoison) {
			if (currentPoison->poison->formID == newPoison->formID) {
				poisonCharges = currentPoison->count;
				return nullptr;
			} else
				return currentPoison->poison;
		}
		return nullptr;
	}
	static inline REL::Relocation<decltype(thunk)> func;

	static RE::ExtraPoison* getCurrentPoison(RE::InventoryEntryData* weapon)
	{
		for (auto& xList : *weapon->extraLists) {
			auto xID = xList->GetByType<RE::ExtraPoison>();
			if (xID) {
				return xID;
			}
		}
		return nullptr;
	}
};

struct poisonChargesMult
{
	static void thunk(RE::InventoryEntryData* poisonTargetWeapon, RE::AlchemyItem* newPoison, int charges)
	{
		auto poison = currentPoison::getCurrentPoison(poisonTargetWeapon);
		if (poison){
			poison->count = charges + poisonCharges;
			return;
		}
		func(poisonTargetWeapon, newPoison, charges);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};


void Init()
{
	REL::Relocation<std::uintptr_t> targetA{ RELOCATION_ID(39406, 40481) };
	stl::write_thunk_call<currentPoison>(targetA.address() + 0x89);

	REL::Relocation<std::uintptr_t> targetB{ RELOCATION_ID(39407, 40482) };
	stl::write_thunk_call<poisonChargesMult>(targetB.address() + REL ::Relocate(0x106, 0xF0));
}

void InitializeLog()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		util::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= fmt::format("{}.log"sv, Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
	const auto level = spdlog::level::trace;
#else
	const auto level = spdlog::level::info;
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(level);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%l] %v"s);
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {};
#endif

	InitializeLog();

	logger::info("Loaded plugin");

	SKSE::Init(a_skse);

	Init();

	return true;
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) constinit auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName(Plugin::NAME.data());
	v.PluginVersion(Plugin::VERSION);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse();
	return v;
}();

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}
