#include "ScriptPCH.h"
#include "Language.h"

class System_OnPvPKill : public PlayerScript
{
public:
	System_OnPvPKill() : PlayerScript("System_OnPvPKill") {}

	struct SystemInfo
	{
		uint32 KillStreak;
		uint32 LastGUIDKill;
	};

	static std::map<uint32, SystemInfo> KillingStreak;

	void OnPvPKill(Player *pKiller, Player *pVictim)
	{
		uint32 kGUID = pKiller->GetGUID(); 
		uint32 vGUID = pVictim->GetGUID();

		if( kGUID == vGUID )
			return;

		if( KillingStreak[kGUID].LastGUIDKill == vGUID )
			return;

		KillingStreak[kGUID].KillStreak++;
		KillingStreak[vGUID].KillStreak = 0;
		KillingStreak[kGUID].LastGUIDKill = vGUID;
		KillingStreak[vGUID].LastGUIDKill = 0;

		switch( KillingStreak[kGUID].KillStreak )
		{
		case 5:
		case 10:
		case 20:
		case 50:
		case 75:
		case 100:
			char msg[500];
			sprintf(msg, "[PvP System]: %s killed %s and is on a %u kill streak.", pKiller->GetName().c_str(), pVictim->GetName().c_str(), KillingStreak[kGUID].KillStreak);
			sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
			break;
		}
	}
};

void AddSC_KillStreakSystem()
{
	new System_OnPvPKill;
}