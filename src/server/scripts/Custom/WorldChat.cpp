#include "ScriptPCH.h"
#include "CustomUtil.h"
#include "Chat.h"
#include "SocialMgr.h"

struct _chatInfo
{
	std::string lastMessage;
	time_t lastMessageTime;
	uint32 messageRepeats;
};

std::map<uint64, _chatInfo> chatInfo;

class cs_world_chat : public CommandScript
{
	public:
		cs_world_chat() : CommandScript("cs_world_chat"){}

	ChatCommand * GetCommands() const
	{
		static ChatCommand WorldChatCommandTable[] = 
		{
			{"chat",	SEC_PLAYER,		true,		&HandleWorldChatCommand,	"", NULL},
			{"dchat",	SEC_DONOR,		false,		&HandleDonorChatCommand,	"", NULL},
			{NULL,		0,				false,		NULL,						"", NULL}
		};

		return WorldChatCommandTable;
	}

	static bool SendChat(ChatHandler *handler, const char *args, bool donorChat)
	{
		Player *player;
		std::string temp = args;
		time_t t = time(NULL);
		
		if( !handler->GetSession() || !handler->GetSession()->GetPlayer() ||
			!(player = handler->GetSession()->GetPlayer()) || !player->CanSpeak() ||
			!args || temp.empty() || temp.find_first_not_of(' ') == std::string::npos )
			return false;

		std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

		double seconds = difftime(t, chatInfo[player->GetGUID()].lastMessageTime);
		if( chatInfo[player->GetGUID()].lastMessage.compare(temp) == 0 && seconds < 15 )
		{
			handler->PSendSysMessage("Please wait %.f seconds before repeating yourself.", 15.0-seconds);
			handler->SetSentErrorMessage(true);
			return false;
		}
		else if( chatInfo[player->GetGUID()].lastMessage.compare(temp) == 0 )
			chatInfo[player->GetGUID()].messageRepeats++;
		else
			chatInfo[player->GetGUID()].messageRepeats = 0;

		if( chatInfo[player->GetGUID()].messageRepeats > 3 )
		{
			handler->PSendSysMessage("Please quit repeating yourself.");
			handler->SetSentErrorMessage(true);
			return false;
		}

		chatInfo[player->GetGUID()].lastMessage = temp;
		chatInfo[player->GetGUID()].lastMessageTime = t;

		std::stringstream msg;
		msg << "|cff00ff00[" << (donorChat ? "Donor" : "Global") << "]|r ";

		switch(handler->GetSession()->GetSecurity())
		{
		default:
			break;
		case SEC_MODERATOR:
		case SEC_GAMEMASTER:
		case SEC_ADMINISTRATOR:
			msg << "|cff00ffff[GM]|r ";
			break;
		case SEC_DEVELOPER:
			msg << "|cffff8000[Dev]|r ";
			break;
		case SEC_OWNER:
			msg << "|cfffa0000[Owner]|r ";
			break;
		}

		msg << PlayerChatLink(player)
			<< " |cfffaeb00"
			<< args
			<< "|r";

		SessionMap sessions = sWorld->GetAllSessions();
		for( SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr )
		{
			if( !itr->second || (donorChat && itr->second->GetSecurity() < SEC_DONOR))
				continue;

			Player *plr = itr->second->GetPlayer();
			if( !plr || (player->GetSession()->GetSecurity() <= SEC_DONOR && plr->GetSocial()->HasIgnore(player->GetGUIDLow())) )
				continue;

			sWorld->SendServerMessage(SERVER_MSG_STRING, msg.str().c_str(), plr);
		}

		return true;
	}

	static bool HandleWorldChatCommand(ChatHandler *handler, const char *args)
	{
		return SendChat(handler, args, false);
	}

	static bool HandleDonorChatCommand(ChatHandler *handler, const char *args)
	{
		return SendChat(handler, args, true);
	}
};

void AddSC_cs_world_chat()
{
	new cs_world_chat();
}