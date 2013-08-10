#include "ScriptPCH.h"

std::string PlayerChatLink(Player *plr)
{
	if( !plr )
		return "";

	std::string prepend;
	if( plr->GetSession()->GetSecurity() == SEC_DONOR )
		prepend = "|cff43A3ED+|cffFFFFFF";
	else
		prepend = "";

	std::stringstream ss;
	ss	<< "|cffFFFFFF"
		<< "|Hplayer:" << plr->GetName()
		<< "|h[" 
		<< prepend << plr->GetName() << "]|h|r";
	return ss.str();
}