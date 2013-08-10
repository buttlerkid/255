# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Custom/CustomUtil.cpp
  Custom/SetupCustom.cpp
  Custom/DuelReset.cpp
  Custom/KillStreak.cpp
  Custom/TeleporterNPC.cpp
  Custom/WorldChat.cpp
  Custom/DonorCommands.cpp
  Custom/Transmogrification.cpp
  Custom/ProfessionsNPC.cpp
  Custom/EnchanterNPC.cpp
  Custom/BufferNPC.cpp
  Custom/BountyHunter.cpp
  Custom/VP_OnSave.cpp
  Custom/WebsiteDatabase.h
  Custom/WebsiteDatabase.cpp
  Custom/CharCustomize.cpp
  Custom/BossMaximus.cpp
  Custom/ChickenLord.cpp
  Custom/VIPBuffCommand.cpp
  Custom/AntiPvP.cpp
)

message("  -> Prepared: Custom")
