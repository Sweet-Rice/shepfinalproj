# PFT Sun Run in Unreal

Use **Windows**, **Unreal Engine 5.8**, and a Windows C++ toolchain compatible with that engine. The project uses the module name `PFTFloor1Viewer` because the curated map references that name.

## Build, test, and play

From the repository root in PowerShell:

```powershell
.\unreal\Run-SunRun.ps1 -Mode Build
.\unreal\Run-SunRun.ps1 -Mode Test -AssetsFrom 'C:\Path\To\PFTFloor1Viewer'
.\unreal\Run-SunRun.ps1 -Mode Play
.\unreal\Run-SunRun.ps1 -Mode Package -PackageDirectory 'C:\Builds\PFTSunRun'
```

- **Build** compiles the editor target from source. Use `-EngineRoot` if Unreal is installed outside the default Epic Games location.
- **Test** launches Unreal headlessly and checks the Commons spawn, shade clearance, wheel handoff, exposure, door checks, and round resets. It also walks the character from the Commons to each target using Unreal movement and checks that both runs win. It returns a nonzero exit code if a check fails and prints the log location.
- **Play** compiles and opens the game in a standalone window. Press **Enter** to spin, **WASD** to move, **Shift** to sprint, **Space** to jump, **E** to check a door, and **Esc** to pause or resume. After the round, press **R** to retry or **Enter** to spin again.
- **Package** cooks the walkthrough and stages a standalone Win64 Development build under `<PackageDirectory>\Windows`. It uses the attached walkthrough assets. See the [player quickstart](../docs/player-quickstart.md) for the executable and controls.

`-AssetsFrom` takes the directory of the team's curated walkthrough project. It creates local directory junctions to the walkthrough's `Content` and `Plugins` folders if those directories are absent. Supply the path once per checkout, or place your own copies of those folders under `unreal/PFTFloor1Viewer`. Get the assets through the team's sharing channel. The asset directories and generated files are ignored by Git.

The standalone launcher selects `PFTSunRunMode` explicitly because the shared map's World Settings select the original walkthrough mode. To use Play In Editor for level edits, use a local copy of the map and select `PFTSunRunMode` under **World Settings → GameMode Override**.

## Gameplay and verification

The wheel selects room 1225 or 1221. The current sun front expands radially from the Commons after 25 seconds and reaches 32 meters at 60 seconds. The amber floor line marks the nearby sun front. The teal floor outline marks the shade zone, where exposure damage stops. See the [build handoff](../docs/implementation-status.md) and [manual round cases](../docs/qa/round-cases.md) for current verification.
