# Implementation status and build handoff

[PFT Sun Run's Unreal project](../unreal/README.md) is in this repository. The game uses the team's separately shared curated walkthrough, `/Game/Maps/PFTWalkthrough`. This handoff tracks [issue #17](https://github.com/Sweet-Rice/shepfinalproj/issues/17).

| Feature | Verified | Next check |
| --- | --- | --- |
| Wheel and countdown | The Unreal headless test checks movement lock during the wheel, movement unlock at handoff, and a damage-free head start. The [browser prototype](../prototypes/room-wheel/README.md) remains available for browser-only testing. | Verify on-screen controls, wheel animation, and HUD readability in a rendered playthrough. |
| Commons spawn and classroom routes | Unreal collision checks found a grounded spawn and continuous capsule clearance to the modeled approaches for 1225 and 1221. | Walk both routes in the rendered build and confirm visible signs and landmarks. See the [route worksheet](commons-route.md). |
| Sun spread, shade, and damage | The headless test checks that players ahead of the front stay safe, players in reach take damage, shade protects without pausing the round, and damage resumes on exit. | Check how clearly the visual cues convey the radial front and the marked shade area. |
| Door checks and round reset | Both rooms are tested as targets. A wrong door reports its room number, looking away rejects the check, a late correct-door win precedes lethal damage, and loss, same-room retry, and spin again are covered. | Run the [manual round cases](qa/round-cases.md) with keyboard and mouse. |
| Packaged Windows build | The [Unreal runner](../unreal/README.md) builds and launches the editor target locally. | Package a Windows build and verify it on a teammate's machine using the [player quickstart](player-quickstart.md). |
