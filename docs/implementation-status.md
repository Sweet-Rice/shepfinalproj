# Implementation status and build handoff

[PFT Sun Run's Unreal project](../unreal/README.md) is in this repository. The game uses the team's separately shared curated walkthrough, `/Game/Maps/PFTWalkthrough`. This handoff tracks [issue #17](https://github.com/Sweet-Rice/shepfinalproj/issues/17).

| Feature | Verified | Next check |
| --- | --- | --- |
| Wheel and countdown | The Unreal headless test checks movement lock during the wheel, movement unlock at handoff, and a damage-free head start. The [browser prototype](../prototypes/room-wheel/README.md) remains available for browser-only testing. | Verify on-screen controls, wheel animation, and HUD readability in a rendered playthrough. |
| Commons spawn and classroom routes | The automated Unreal playtest walked from the grounded Commons spawn to both modeled classroom doors and won with full health. The clean routes took 22.5 seconds to 1225 and 23.6 seconds to 1221 at normal walking speed. | Time unfamiliar players and confirm visible signs and landmarks in the rendered build. See the [route worksheet](commons-route.md). |
| Sun spread, shade, and damage | The headless test checks that players ahead of the front stay safe, players in reach take damage, shade protects without pausing the round, and damage resumes on exit. | Check how clearly the visual cues convey the radial front and the marked shade area. |
| Door checks and round reset | Both rooms are tested as targets. A wrong door reports its room number, looking away rejects the check, a late correct-door win precedes lethal damage, and loss, same-room retry, and spin again are covered. | Run the [manual round cases](qa/round-cases.md) with keyboard and mouse. |
| Packaged Windows build | The standalone Win64 Development package opened directly to Sun Run at 1280×720 on the build machine. All 15 gameplay checks and both classroom walks passed using the packaged executable. | Test the ZIP on a teammate's Windows machine using the [player quickstart](player-quickstart.md). |
