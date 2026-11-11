# Implementation status and build handoff

The browser wheel is in GitHub. A separate local Unreal walkthrough provides evidence that part of the building can be loaded and traversed. The team still needs a revision and evidence from its actual gameplay build. This handoff tracks [issue #17](https://github.com/Sweet-Rice/shepfinalproj/issues/17).

| Feature | Evidence | Next check |
| --- | --- | --- |
| Room wheel and countdown | Demonstrated in the [standalone browser prototype](../prototypes/room-wheel/README.md). The [integration PR](https://github.com/Sweet-Rice/shepfinalproj/pull/43) records browser checks for keyboard Skip, repeat spins, and one expansion event at 25 seconds. The [repository CI run](https://github.com/Sweet-Rice/shepfinalproj/actions/runs/43114532782) passed the wheel logic tests. | In Unreal, check that wheel handoff starts movement and the timer together and the objective shows the same room. |
| Building and first-floor movement | Demonstrated separately in the local `PFTFloor1Viewer` project, map `/Game/Maps/PFTWalkthrough`, with Unreal 5.8.4. The November 5 local check reported a grounded Camber atrium spawn and a passing upper-floor guard. | Check the agreed Commons spawn and walk all the way to a classroom in the team's gameplay checkout. |
| Commons spawn, classroom, and wrong door | [Candidate leads](commons-route.md) are available from the building guide and local geometry metadata. The route and interaction remain unverified. | Confirm the entrance, step-free path, readable sign, and door interaction in a running build. |
| Sun spread, shade, and damage | The [gameplay design](gameplay-design.md) defines intended behavior. The browser countdown event is only a preview of the handoff. | Record a gameplay clip from wheel handoff through expansion, exposed damage, and shade. |
| Win, loss, and retry | [Manual cases](qa/round-cases.md) are ready to run. Gameplay results are pending. | Record a correct-door win, zero-health loss, and same-room retry. |
| Packaged Windows build | No package or public release was visible in this repository when this handoff was prepared. | Have the build owner share a permitted package or host a run, and confirm the controls in the [player quickstart](player-quickstart.md). |

## Evidence to provide

The team gameplay build owner has yet to be identified in the issue or repository. The owner should provide the following in [issue #17](https://github.com/Sweet-Rice/shepfinalproj/issues/17) or the team's approved handoff channel.

- **Build owner and revision.** Give a contact handle, the exact Perforce changelist or Git SHA, the Unreal version, and the map or packaged build ID.
- **What runs today.** Confirm which pieces in the table were exercised and which are still missing or blocked. Include a short clip or screenshots when permitted.
- **How someone else can check it.** Share confirmed controls and a way to run or observe the build. Keep private asset locations and credentials in the team's approved channels.
