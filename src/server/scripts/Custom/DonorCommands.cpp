/*
* Made By ???
* Error Fixed By Rochet2
* Released By Ghostcrawler
* Edited By Sinistah
*/

/* Insert Into World Database
INSERT INTO `command` VALUES ('donor mall', '1', '');
INSERT INTO `command` VALUES ('donor changerace', '1', '');
INSERT INTO `command` VALUES ('donor changefaction', '1', '');
INSERT INTO `command` VALUES ('donor maxskills', '1', '');
INSERT INTO `command` VALUES ('donor customize', '1', '');
INSERT INTO `command` VALUES ('donor tele', '1', '');
INSERT INTO `command` VALUES ('donor morph', '1', '');
INSERT INTO `command` VALUES ('donor demorph', '1', '')
*/

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"
#include "Language.h"

class donorcommands : public CommandScript
{
public:
    donorcommands() : CommandScript("donorcommands") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand donorCommandTable[] =

        {
            { "mall",	    SEC_PLAYER,     true, &HandledonorMallCommand,         "", NULL },
            { "changerace",    SEC_PLAYER,  false, &HandleChangeRaceCommand,             "", NULL },
			{ "changefaction",	SEC_PLAYER,  false, &HandleChangeFactionCommand,		"", NULL },
			{ "maxskills",	SEC_PLAYER,  false, &HandleMaxSkillsCommand,		"", NULL },
			{ "customize",	SEC_PLAYER,  false, &HandleCustomizeCommand,		"", NULL },
			{ "tele",           SEC_PLAYER,  false, &HandleTeleCommand,		"", NULL },
			{ "morph",           SEC_PLAYER,  false, &HandleMorphCommand,		"", NULL },
			{ "demorph",           SEC_PLAYER,  false, &HandleDemorphCommand,		"", NULL },
 
            { NULL,             0,                     false, NULL,                                           "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "donor",	    SEC_PLAYER,   true, NULL,      "",  donorCommandTable},
	       { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }


static bool HandleTeleCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Player* me = handler->GetSession()->GetPlayer();

        // id, or string, or [name] Shift-click form |color|Htele:id|h[name]|h|r
        GameTele const* tele = handler->extractGameTeleFromLink((char*)args);

        if (!tele)
        {
            handler->SendSysMessage(LANG_COMMAND_TELE_NOTFOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (me->isInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        MapEntry const* map = sMapStore.LookupEntry(tele->mapId);
        if (!map || map->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_CANNOT_TELE_TO_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->isInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

		if( me->GetVehicle() )
			me->ExitVehicle();

        me->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
        return true;
		}

static bool HandledonorCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

            me->Say("donor command?", LANG_UNIVERSAL);
            return true;
    }

static bool HandleMorphCommand(ChatHandler* handler, const char* args)
    {
		int32 bannedMorphs[] = {
			15332, // crashes client
			15444, // crashes client
			20242, // Ivisibility
			17116, // crashes client
			-1 // end of list
		};

		uint32 morph = atoi(args);

		for( int32 *ptr = bannedMorphs; *ptr != -1; ptr++ )
		{
			if( morph == *ptr )
			{
				handler->PSendSysMessage("Invalid morph.");
				handler->SetSentErrorMessage(true);
				return false;
			}
		}

        handler->GetSession()->GetPlayer()->SetDisplayId(morph);

        return true;
    }

static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Relog to change race of your character.");
        return true;
    }

static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Relog to change faction of your character.");
        return true;
    }

static bool HandleMaxSkillsCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->UpdateSkillsToMaxSkillsForLevel();
		handler->PSendSysMessage("Your weapon skills are now maximized.");
        return true;
    }

static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Relog to customize your character.");
        return true;
    }

static bool HandledonorMallCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

        if (me->isInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->isInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

		me->TeleportTo(571,	5838.920410f,	676.107361f,	609.886108f,	0.627516f); // MapId, X, Y, Z, O
                return true;
    }

static bool HandleDemorphCommand(ChatHandler* handler, const char* args)
	{
		Player* me = handler->GetSession()->GetPlayer();
		me->DeMorph();
		me->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0);
		return true;
	}
	
};

void AddSC_donorcommands()
{
    new donorcommands();
}
