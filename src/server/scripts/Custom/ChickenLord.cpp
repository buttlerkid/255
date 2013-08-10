#include "ScriptPCH.h"

enum Spells
{
    // List of spells.
    // Not required to define them in this way, but will make it easier to maintain in case spellId change
    SPELL_BUFF                                  = 72523,
    SPELL_BERSERK                               = 41924,
};

class Cicken_Lord : public CreatureScript
{
    public:

        Cicken_Lord()
            : CreatureScript("Cicken_Lord")
        {
        }

        struct Cicken_LordAI : public ScriptedAI
        {
            // *** HANDLED FUNCTION ***
            //This is the constructor, called only once when the Creature is first created
            Cicken_LordAI(Creature* c) : ScriptedAI(c) {}

            // *** CUSTOM VARIABLES ****
            //These variables are for use only by this individual script.
            //Nothing else will ever call them but us.

            uint32 m_uiRebuffTimer;                                 // Timer for rebuffing

            // *** HANDLED FUNCTION ***
            //This is called after spawn and whenever the core decides we need to evade
            void Reset()
            {
                me->RestoreFaction();
            }

            // *** HANDLED FUNCTION ***
            // Enter Combat called once per combat
            void EnterCombat(Unit* who)
            {
                //Say some stuff
                DoPlaySoundToSet(me,6820);
            }

            // *** HANDLED FUNCTION ***
            //Update AI is called Every single map update (roughly once every 50ms if a player is within the grid)
            void UpdateAI(const uint32 uiDiff)
            {
                //Out of combat timers
                if (!me->getVictim())
                {
                    //Rebuff timer
                    if (m_uiRebuffTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_BUFF);
                        m_uiRebuffTimer = 900000;                  //Rebuff agian in 15 minutes
                    }
                    else
                        m_uiRebuffTimer -= uiDiff;
                }

			
                //Return since we have no target
                if (!UpdateVictim())
                    return;

                if (HealthBelowPct(25))
                    {
                    DoPlaySoundToSet(me,15936);
                    DoCast(me, SPELL_BERSERK);
                    }

				  DoMeleeAttackIfReady();
                }
            };
         
        CreatureAI* GetAI(Creature* creature) const
        {
            return new Cicken_LordAI(creature);
        }
};

//This is the actual function called only once durring InitScripts()
//It must define all handled functions that are to be run in this script
void AddSC_Cicken_Lord()
{
    new Cicken_Lord();
}