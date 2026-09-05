<h1>
  <img src="https://github.com/user-attachments/assets/28d73a41-1c02-4343-8365-3a19d0771d48" width="50" height="50" align="absmiddle" />
  &nbsp;Core
</h1>

**Core** is a community made Fortnite gameserver focused on recreating original behavior as accurately as possible across our current supported Fortnite builds. We prioritize fixing reported issues and completing functionality for supported versions over expanding version support. When work begins on a new Fortnite version, we aim to make it fully functional before moving on to another version.

Core runs inside Fortnite to provide server-side systems to load maps, apply playlists, accept players, run matches, and support gameplay. This repository does NOT include Fortnite game files, a launcher, or backend services.

> Core is an independent community project. It is not affiliated with, endorsed by, or supported by Epic Games.

**Please refer to the [Contributing](#contributing) section before posting an issue, Thank you!**

[Join the Core Discord](https://discord.gg/auzEaKs7AS) · [Report an issue](https://github.com/PongooDev/Core/issues)

## 📥[Download](https://github.com/PongooDev/Core/releases)📥

## Version support

| Fortnite version | Status |
| --- | --- |
| 1.7.2–8.00 | Full support ✅|
| 8.01+ | Unsupported ⚠️|

- **Full support** means that a version is actively supported. We prioritize fixes and missing functionality for fully supported versions before adding support for other versions.
- **Unsupported** means the build may start and have some gameplay features, but it may be unfinished, untested, or have game-breaking bugs.
- **Save the World is experimental and unfinished.** Expect missing or incomplete mission, inventory, quest, AI, and progression behavior.

### When reporting a problem refer to the [Contributing](#contributing) section.

## Contents

- [Building](#building)
- [Server config options](#server-config-options)
- [Playlist selection](#playlist-selection)
- [Commands](#commands)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [Credits](#credits)

## Building

### Requirements
- Windows 10 or Windows 11
- Visual Studio 2022 or 2026
- Desktop development with C++ workload
- MSVC v143 C++ x64/x86 build tools
- Windows 10 SDK

### Build steps
1. Clone the repository:

   ```
   git clone https://github.com/PongooDev/Core

   cd Core
   ```

2. Open [`Core.sln`](Core.sln) in Visual Studio
3. Select **Release** and **x64** in the solution toolbar.
4. Build it

## Server config options

Core reads configuration from the Fortnite process command line during startup. Boolean options are enabled by including their name as a command-line flag.

| Option | Default | Purpose |
| --- | ---: | --- |
| `Playlist=<id-or-name>` | `2` | Selects an Athena playlist by numeric ID or internal object name. |
| `MapToLoad=<map>` | automatic | Overrides Core's default map selection. |
| `-bIsProd` | off | Enables production behavior and disables Core's custom development commands. |
| `-bEnableClientConsole` | off | Allows Core's console window when running in client mode. |
| `-bIsClient` | off | Runs Core's client initialization path instead of the gameserver path. |
| `-bSetClientLogVerbosity` | off | Applies Core's log-verbosity setup in client mode. |
| `-bUseGameSessions` | off | Uses the game-session validation and assignment path. |
| `-bSkipSessionValidation` | off | Skips session validation when game sessions are enabled. |
| `-bListenServer` | off | Uses Core's listen-server path instead of dedicated-server setup. |
| `-bDevSameTeam` | off | Places players on the same development team. |

Example:

```
Playlist=Playlist_DefaultSolo -bUseGameSessions
```

Core logs the resolved configuration at startup. Check the console or `Server_log.txt` to confirm that the intended values were received.

> These are developer focused options. Some combinations are version-specific or experimental.

## Playlist selection

`Playlist=` accepts either:

- a numeric playlist ID, such as `Playlist=2`; or
- an internal playlist name, such as `Playlist=Playlist_DefaultSolo`.

Playlist names and availability differ by Fortnite build. If a playlist cannot be found, double check the internal name in the game assets or in an object dump.

## Commands

When `-bIsProd` isn't enabled, Core extends Fortnite's `ServerCheat` path with development and testing commands.

Core supports positional arguments and named parameters:

```
cheat GiveItem WID_Assault_Auto_Athena_R_Ore_T03 1
cheat GiveItem?ItemDefinitionName=WID_Assault_Auto_Athena_R_Ore_T03?Count=1
```

Current command groups include:

| Category | Commands |
| --- | --- |
| Items and ammo | `GiveItem`, `ForceGiveItem`, `SpawnPickup`, `SetLoadedAmmo`, `GiveAmmo`, `ClearEquippedItem`, `GetWeaponStats`, `ServerExecuteInventoryItem`, `DumpInventory` |
| Player state | `SetHealth`, `SetMaxHealth`, `SetShield`, `SetMaxShield`, `SetKillScore`, `TeleportToLocation`, `DumpCurrentLocation` |
| Pawns and bots | `DumpAllPawns`, `PossessPawnByIndex`, `PossessPawnByName`, `DespawnAllBots` |
| World and actors | `SpawnActor`, `DumpActorsWithClass`, `DumpAircrafts`, `DestroyTarget`, `StartEvent`, `ActivateRifts`, `DumpEncounters`, `StartEncounter` |
| QuickBars | `SpawnQuickBars`, `DestroyQuickBars`, `DumpQuickBars` |
| Testing and fun | `LootRain`, `SetGameSpeed`, `TeleportAllToMe`, `SwapPlaces`, `LaunchPawn`, `SetScale`, `ScalePawn`, `Goto`, `DestroyBuildings`, `EmoteAll`, `EmoteAllSpecific`, `EmotePlayerByName`, `TogglePersonalVehicle` |
| AFortAthenaMutator_Heist | `SetSpawnExitCraftTime`, `DumpExitCraftTimer`, `HeistPhaseStep`, `DumpGameState`, `UpdateGamePhaseStep`, `SpawnExitCraft` |
| Quests | `ListQuests`, `ProgressQuest` |

Run `cheat Help` in game for all commands, syntax, and descriptions.

## Troubleshooting

### The project does not build

- Confirm **Release | x64** is selected.
- Build from [`Core.sln`](Core.sln)
- Look at the actual compiler errors near the end of the output, the project can have many warnings.

### A feature works on one build but not another

Core functions differently on every version. Reference the next section about making an issue report.

## Contributing

Contributions are welcome, whether it is code or an issue report.

### Issue reports

Include all of the following in a report if possible:

1. Fortnite version and CL
2. Playlist
3. Number of players
4. Reproduction steps
5. Screenshots
6. Server log
7. Core commit
8. Anything else that may help

**Issues that are overly broad, contain only a crash log without context, or provide too little information to accurately show or reproduce the problem may be closed until the required details are supplied.**

### Code

Before submitting code:

1. Keep unrelated fixes in separate pull requests
2. Keep your code clean and readable. You may create helper functions if needed for better readability.
3. Test your code and include the versions you tested it on.
4. Necessary or helpful logging is encouraged however remove excessive or temporary logging from the final change.
5. Avoid broad version gates unless the compatibility boundary is understood and supported by testing or other evidence.

**Pull requests may be declined when they are inaccurate, untested, overly broad, unrelated to the reported issue, or harmful to other supported versions.**

## Credits

- **PongooDev** — project creator and primary maintainer
- **Ploosh** — crash reporter and finder work
- **Milxnor** — finder work
- **MrWallio** — This README and general Core contributions
- **Podge** — The Core Logo.
- [Core contributors](https://github.com/PongooDev/Core/graphs/contributors)

For more help, join the [Core Discord](https://discord.gg/auzEaKs7AS).

---
If you use Core in another project, please credit the Core project.
