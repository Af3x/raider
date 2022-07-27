#pragma once

#include "ue4.h"

namespace Game
{
    void Start()
    {
        GetPlayerController()->SwitchLevel(L"Athena_Terrain?game=/Game/Athena/Athena_GameMode.Athena_GameMode_C");
        bTraveled = true;
    }

    void OnReadyToStartMatch()
    {
        //Set global variables
        Globals::bDBNO = false;
        Globals::bFriendlyFire = true;
        Globals::bLargeTeamGame = false;
        Globals::bRespawnPlayers = true;
        Globals::MaxHealth = 100;
        Globals::MaxShield = 100;
        Globals::bSoloGame = true;
        Globals::bLateGame = true;
        bool ACEnabled = false;
        Globals::DebugRespawn = false;
		
        auto GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);
        auto GameMode = reinterpret_cast<AAthena_GameMode_C*>(GetWorld()->AuthorityGameMode);
        static auto SoloPlaylist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");
        static auto DuoPlaylist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultDuo.Playlist_DefaultDuo");
        static auto PlaygroundList = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_Playground.Playlist_Playground");
        auto InProgress = UObject::FindObject<UKismetStringLibrary>("Default__KismetStringLibrary")->STATIC_Conv_StringToName(L"InProgress");
        ((UFortGameInstance*)GetWorld()->OwningGameInstance)->AthenaCodeOfConductURL = L"https://www.youtube.com/watch?v=dQw4w9WgXcQ";

        GameState->bGameModeWillSkipAircraft = true;
        GameState->AircraftStartTime = 9999.9f;
        GameState->WarmupCountdownEndTime = 99999.9f;

        GameState->GamePhase = EAthenaGamePhase::Warmup;
        GameState->OnRep_GamePhase(EAthenaGamePhase::None);

        GameMode->bDisableGCOnServerDuringMatch = true;
        GameMode->bAllowSpectateAfterDeath = true;
        GameMode->bEnableReplicationGraph = true;

        GameMode->MatchState = InProgress;
        GameMode->K2_OnSetMatchState(InProgress);

        auto Playlist = PlaygroundList;

        if (Playlist)
        {
            Playlist->bNoDBNO = !Globals::bDBNO;
            Playlist->bIsLargeTeamGame = Globals::bLargeTeamGame;
            if (Globals::bRespawnPlayers)
            {
                Playlist->RespawnLocation = EAthenaRespawnLocation::Air;
                Playlist->RespawnType = EAthenaRespawnType::InfiniteRespawn;
                
            }
            if (Globals::bFriendlyFire)
            {
                Playlist->FriendlyFireType = EFriendlyFireType::On;
                GameMode->FriendlyFireType = EFriendlyFireType::On;
            }
            if (Globals::bLateGame)
            {
                BusLocation = GetRandomBattleBusLocation();
            }
				

            GameState->CurrentPlaylistData = Playlist;
            GameState->OnRep_CurrentPlaylistData();
        }

        

        GameMode->StartPlay();

        GameState->bReplicatedHasBegunPlay = true;
        GameState->OnRep_ReplicatedHasBegunPlay();

        GameMode->StartMatch();
        GameMode->bAlwaysDBNO = Globals::bDBNO;
        GameMode->MinRespawnDelay = 5.0f;
    }
}
