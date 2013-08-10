#include "ScriptPCH.h"
#include "Common.h"
#include "Config.h"
#include "WebsiteDatabase.h"

class WDB_Pinger : public WorldScript
{
private:
	WDB *WebsiteDatabase;
	uint32 timer;

public:
	WDB_Pinger() : WorldScript("WDB_Pinger")
	{
		WebsiteDatabase = new WDB;
	}
	~WDB_Pinger()
	{
		delete WebsiteDatabase;
	}

	void OnUpdate(uint32 diff)
	{
		timer += diff;

		if( timer >= ((uint32)ConfigMgr::GetIntDefault("MaxPingTime", 30) * MINUTE * IN_MILLISECONDS) )
		{
			(*WebsiteDatabase)->KeepAlive();
			timer = 0;
		}
	}
};


class VP_Rewards : public PlayerScript
{
private:
	WDB *WebsiteDatabase;

public:
	VP_Rewards() : PlayerScript("VP_Reward")
	{
		WebsiteDatabase = new WDB;

		if( !WebsiteDatabase->IsOpen() )
		{
			sLog->outError(LOG_FILTER_GENERAL, "VP_Rewards: Database is not open.");
			return;
		}
	}

	~VP_Rewards()
	{
		delete WebsiteDatabase;
	}

	void OnSave(Player *plr)
	{
		if( !plr || !WebsiteDatabase->IsOpen() )
			return;

		uint32 psi = ConfigMgr::GetIntDefault("PlayerSaveInterval", 0);
		float pph = ConfigMgr::GetFloatDefault("Vote.PointsPerHour", 0.f);
		if( psi == 0 || pph == 0.f )
			return;

		PreparedStatement *stmt = (*WebsiteDatabase)->GetPreparedStatement(WEB_ADD_VOTE_POINTS);
		stmt->setFloat(0, ((pph / 60.f / 60.f) * ((float)psi / 1000)));
		stmt->setUInt32(1, plr->GetSession()->GetAccountId());
		(*WebsiteDatabase)->Execute(stmt);
	}
};

void Setup_VP_Reward()
{
	new VP_Rewards;
	new WDB_Pinger;
}
