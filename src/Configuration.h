#pragma once
namespace Configuration {
    namespace Settings {
		inline bool bDisplayNotification = true;
		inline std::string iniFilePath = "Data/SKSE/Plugins/simplePoisonStacker.ini";
		inline float fAlchemyMod = 0.4f;

		inline float fRightWidgetXpos = 10.0f;
		inline float fRightWidgetYpos = 10.0f;
		inline float fRightWidgetRotation = 0.0f;
		inline float fRightWidgetXscale = 75.0f;
		inline float fRightWidgetYscale = 75.0f;
		inline float fLeftWidgetXpos = 50.0f;
		inline float fLeftWidgetYpos = 96.3f;
		inline float fLeftWidgetRotation = 0.0f;
		inline float fLeftWidgetXscale = 75.0f;
		inline float fLeftWidgetYscale = 75.0f;
		inline bool bRightWidgetBarGrowth = false;
		inline bool bLeftWidgetBarGrowth = false;

		inline float iWidgetBarColor = 10053375;
		inline float iWidgetBorderColor = 14474460;
		inline float iWidgetBGColor = 657930;

		inline int getMaxCharges(RE::Actor* a_actor)
		{
			return a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kAlchemy) * fAlchemyMod;
		}
    }
};