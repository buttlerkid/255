/*
* Made By Ghostcrawler336 From www.ac-web.org
*/
/* ScriptData
SDName: Custom_Boss
SD%Complete: %60
EndScriptData */    
	 
#include "ScriptPCH.h"
     
enum Yells
{
    SAY_AGGRO                   = -1603278,
    SAY_ENRAGE                  = -1189002,
    SAY_KILL                    = -1189003
};

enum Emotes
{
    EMOTE_ENRAGE                = -1189004
};

enum Spells
{
    SPELL_RUSHINGCHARGE         = 8260,
    SPELL_BARKSKIN              =22812,
	SPELL_MANGLE                =48566,
	SPELL_RAKE                  =1822,
	SPELL_MOONKIN               =62795,
    SPELL_FRENZY                = 8269,
	SPELL_CAT_FORM              =768
};
     
class Custom_Boss : public CreatureScript
{
public:
   Custom_Boss() : CreatureScript("Custom_Boss") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Custom_BossAI(creature);
    }

    struct Custom_BossAI : public ScriptedAI
    {
        Custom_BossAI(Creature* creature) : ScriptedAI(creature) {}

        bool Enrage;

        uint32 BARKSKIN_Timer;
        uint32 MANGLE_Timer;
		uint32 RAKE_Timer;
		
		void Reset()
        {
            Enrage = false;
           BARKSKIN_Timer = 25000;
           MANGLE_Timer = 3000;
		   RAKE_Timer = 6000;
			
 }
 
 
		        void EnterCombat(Unit* /*who*/)
        {
           DoScriptText(SAY_AGGRO, me);
            DoCast(me, SPELL_RUSHINGCHARGE);
			DoCast(me, SPELL_CAT_FORM);
        }

         void KilledUnit(Unit* /*victim*/)
         {
		     DoScriptText(SAY_AGGRO, me);
             DoScriptText(SAY_KILL, me);
         }
		 
		         void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //If we are <25% hp goes Enraged
            if (!Enrage && !HealthAbovePct(25) && !me->IsNonMeleeSpellCasted(false))
            {
                DoScriptText(EMOTE_ENRAGE, me);
                DoCast(me, SPELL_FRENZY);
                Enrage = true;
            }
			
		//If we are <60% hp goes MoonKin
            if (!HealthAbovePct(60) && !me->IsNonMeleeSpellCasted(false))
            {
                DoCast(me, SPELL_MOONKIN);
            }

            //BARKSKIN_Timer
            if (BARKSKIN_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_BARKSKIN);
                BARKSKIN_Timer = 9000;
            }
            else BARKSKIN_Timer -= diff;

            // MANGLE_Timer
            if (MANGLE_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MANGLE);
                MANGLE_Timer = 3000;
            }
            else MANGLE_Timer -= diff;
			
		      DoMeleeAttackIfReady();
			  
			  
			  
			  // RAKE_Timer
            if (RAKE_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_RAKE);
                RAKE_Timer = 4000;
            }
            else RAKE_Timer -= diff;

            DoMeleeAttackIfReady();

			
        }
    };
};

void AddSC_Custom_Boss()
{
    new Custom_Boss();
}