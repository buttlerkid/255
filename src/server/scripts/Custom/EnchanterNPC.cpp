// Release by RuleZ


#include "ScriptPCH.h"

class npc_exchanger : public CreatureScript
{
public:
    npc_exchanger() : CreatureScript("npc_exchanger") { }

    bool OnGossipHello(Player *player, Creature *_creature)
    {
        if (player->isInCombat())
        {
            player->CLOSE_GOSSIP_MENU();
            _creature->MonsterWhisper("You are in combat!", player->GetGUID());
            return true;
        }
        else
        {
            player->ADD_GOSSIP_ITEM( 4, "20 Battle Tokens --> 2000 Honor"                     , GOSSIP_SENDER_MAIN, 3); //Change the ID below
            player->ADD_GOSSIP_ITEM( 4, "50 Battle Tokens --> 7000 Honor"                     , GOSSIP_SENDER_MAIN, 4); //Change the ID below
            player->ADD_GOSSIP_ITEM( 4, "100 Battle Tokens --> 16000 Honor"                     , GOSSIP_SENDER_MAIN, 5); //Change the stats below
            player->ADD_GOSSIP_ITEM( 6, "20 Halaa Battle Tokens --> 3500 Honor"                     , GOSSIP_SENDER_MAIN, 6); //Change the stats below
            player->ADD_GOSSIP_ITEM( 6, "50 Halaa Battle Tokens --> 9000 Honor"                     , GOSSIP_SENDER_MAIN, 7);
			player->ADD_GOSSIP_ITEM( 6, "100 Halaa Battle Tokens --> 20000 Honor"                     , GOSSIP_SENDER_MAIN, 8);
        }

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());         
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 uiAction)
    {
        if (sender == GOSSIP_SENDER_MAIN)
        {
            player->PlayerTalkClass->ClearMenus();
            switch(uiAction)
            {
case 3: //Token-Honor
                if (player->HasItemCount( 23247, 20, false ))
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->DestroyItemCount(23247, 20, true, false);
                    player->ModifyHonorPoints(+2000);
                    _creature->MonsterWhisper("You exchanged succesfully!", player->GetGUID());
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    _creature->MonsterWhisper("I am sorry, but you haven't got the x amount of ...", player->GetGUID());
                    return false;
                }
                break;

case 4: //Token-Honor
                if (player->HasItemCount( 23247, 50, false ))
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->DestroyItemCount(23247, 50, true, false);
                    player->ModifyHonorPoints(+7000);
                    _creature->MonsterWhisper("You exchanged succesfully!", player->GetGUID());
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    _creature->MonsterWhisper("I am sorry, but you haven't got the x amount of ...", player->GetGUID());
                    return false;
                }
                break;

case 5: //Token-Honor
                if (player->HasItemCount( 23247, 100, false ))
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->DestroyItemCount(23247, 100, true, false);
                    player->ModifyHonorPoints(+16000);
                    _creature->MonsterWhisper("You exchanged succesfully!", player->GetGUID());
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    _creature->MonsterWhisper("I am sorry, but you haven't got the x amount of ...", player->GetGUID());
                    return false;
                }
                break;
case 6: //Token-Honor
                if (player->HasItemCount( 26045, 20, false ))
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->DestroyItemCount(26045, 20, true, false);
                    player->ModifyHonorPoints(+3500);
                    _creature->MonsterWhisper("You exchanged succesfully!", player->GetGUID());
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    _creature->MonsterWhisper("I am sorry, but you haven't got the x amount of ...", player->GetGUID());
                    return false;
                }
                break;
case 7: //Token-Honor
                if (player->HasItemCount( 26045, 50, false ))
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->DestroyItemCount(26045, 50, true, false);
                    player->ModifyHonorPoints(+9000);
                    _creature->MonsterWhisper("You exchanged succesfully!", player->GetGUID());
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    _creature->MonsterWhisper("I am sorry, but you haven't got the x amount of ...", player->GetGUID());
                    return false;
                }
                break;
case 8: //Token-Honor
                if (player->HasItemCount( 26045, 100, false ))
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->DestroyItemCount(26045, 100, true, false);
                    player->ModifyHonorPoints(+20000);
                    _creature->MonsterWhisper("You exchanged succesfully!", player->GetGUID());
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    _creature->MonsterWhisper("I am sorry, but you haven't got the x amount of ...", player->GetGUID());
                    return false;
                }
                break;
            } 
        } 
        return true;
    } 
};

void AddSC_npc_exchanger()
{
    new npc_exchanger();
}