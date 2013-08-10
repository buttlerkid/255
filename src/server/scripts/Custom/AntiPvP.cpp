#include "ScriptPCH.h"
#include "CustomUtil.h"
#include "Language.h"
#include <vector>

class AntiPvP_OnPvPKill : public PlayerScript
{
public:
	AntiPvP_OnPvPKill() : PlayerScript("AntiPvP_OnPvPKill") {}

	void OnPVPKill(Player *pKiller, Player *pVictim)
	{
		int32 AntiPvPAIDS[] = {
			268,	// Azshara Leveling zone
			4320,	// Ampitheater of Angish
			4430,	// Temple of Storms
			-1		// No where
		};

		if( pKiller->GetGUID() == pVictim->GetGUID() )
			return;

		for( int32 *ptr = AntiPvPAIDS; *ptr != -1; ptr++ )
		{
			if( *ptr != pKiller->GetAreaId() )
				continue;

			const GameTele *tele = sObjectMgr->GetGameTele("gur1");
			if( tele )
			{
				std::stringstream msg;
				msg << PlayerChatLink(pKiller)
					<< " was teleported to Gurubashi Arena for PvPing in a No-PvP zone!";
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg.str().c_str(), 0);

				pKiller->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);

				pVictim->ResurrectPlayer(1.0f);
				pVictim->SpawnCorpseBones();
				pVictim->SaveToDB();
			}
		}		
	}
};

void AddSC_AntiPvP()
{
	new AntiPvP_OnPvPKill;
}