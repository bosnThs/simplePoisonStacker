#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace logs = SKSE::log;

namespace stl
{
	//using namespace SKSE::stl;
	
	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}
}

#ifdef ENABLE_SKYRIM_SE
#	define OFFSET(se, ae) ae
#else
#	define OFFSET(se, ae) se
#endif