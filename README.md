# shepfinalproj — What If the Sun Was Inside PFT?

Final project for 4330.

But is it TRULY the final project?

## What we are making

*PFT Sun Run* is a first-person wayfinding and survival game set inside the digital twin of **Patrick F. Taylor Hall (PFT)** at LSU, designed to help freshmen learn navigation across the building.

The player spawns in the Commons, spins a wheel to receive an assigned first-floor classroom, and gets an initial countdown head start to navigate using hallway signs and architectural landmarks. When the timer reaches zero, a dangerous indoor sun expands from the Commons through the corridors. Checking the assigned classroom door while alive wins the round; losing all health loses. Stepping into shade stops damage accumulation while the sun continues spreading, and checking wrong doors reveals their actual room numbers to assist navigation.

The hall is an existing Unreal project, not a model we need to rebuild. Our work makes the environment playable and adds the Commons spawn, room wheel, countdown, corridor hazard, shelter, navigation cues, HUD, and round lifecycle.

### Status and design reference

- **Authoritative gameplay design:** Intended gameplay rules, phased timing, and loop architecture are documented in **[docs/gameplay-design.md](docs/gameplay-design.md)** (tracked under [Issue #18](https://github.com/Sweet-Rice/shepfinalproj/issues/18)). Refer to that document for design specifications.
- **Repository contents:** Design and playtest documentation, plus a [standalone browser room wheel](prototypes/room-wheel/README.md). The playable encounter, Unreal source, and building assets are tracked through the team's build handoff.
- **Build and implementation status:** Current build behavior and in-editor features remain unverified until supported by hands-on testing and shareable evidence (revision tracking, gameplay clips, screenshots) under [Issue #17](https://github.com/Sweet-Rice/shepfinalproj/issues/17), not inferences from closed tickets.
- **Unverified architecture and Blueprint names:** All component and class names mentioned in this README (such as `BP_SunEscapeGameMode`, `BP_IndoorSun`, `BP_ClassroomGoal`, `WBP_SunEscapeHUD`) are proposed conceptual abstractions, not confirmed assets in the digital twin project.
- **Verified digital twin baseline:** Archive metadata and configuration verified from `PFTDigitalTwin.zip` are preserved below in [Existing digital twin](#existing-digital-twin).

## Core gameplay rules and invariants

The authoritative gameplay loop is specified in [docs/gameplay-design.md](docs/gameplay-design.md). Key invariants and lifecycle rules include:

1. **Commons Spawn & Grounded Navigation:** The player spawns grounded just inside a Commons exterior door leading outside. Routes are designed for standard walking pace without jumping ([#30](https://github.com/Sweet-Rice/shepfinalproj/issues/30)).
2. **Wheel Selection & Handoff:** The room wheel selects one eligible first-floor classroom. When the wheel settles, movement unlocks, the initial countdown starts, and the assigned room remains persistent on the HUD ([#29](https://github.com/Sweet-Rice/shepfinalproj/issues/29)).
3. **Head Start Protection:** The initial countdown allows safe navigation and sign-reading. **No hazard damage occurs during the head start.** Initial timing and tuning targets are in the design reference.
4. **Hazard Expansion:** At countdown zero, the sun begins expanding from the Commons through corridors (targeting ~60 seconds total from wheel handoff to reach most exposed corridors; [#31](https://github.com/Sweet-Rice/shepfinalproj/issues/31)). The ~60-second value is a pacing benchmark, not an automatic defeat timer or an immunity window.
5. **Shade & Shelter:** Stepping into shade immediately stops damage accumulation while the sun continues spreading.
6. **Wayfinding Feedback:** Checking a wrong door displays its actual room number and leaves the objective unchanged. The countdown and hazard continue; checking a wrong door does not pause the run.
7. **Late Victory:** Checking the correct classroom door while alive wins the round immediately (`Won`), even after the countdown has reached zero.
8. **Same-Update Success Ordering:** A valid target-door check by a living player resolves before potentially lethal hazard damage in that update ([#25](https://github.com/Sweet-Rice/shepfinalproj/issues/25)). An already-lost run cannot win afterward.
9. **Single Damage Owner & Time-Based Application:** Apply damage using elapsed time and one gameplay owner; the proposed owner is the game mode. Clamp health to `[0, MaxHealth]`.
10. **Terminal Transitions & Round Reset:** `Won` and `Lost` (at zero health) are terminal states with no resurrection. On defeat, the player may choose **Retry** (resets spawn, health, timer, hazard spread, and HUD with the same assigned classroom) or **Spin Again** (starts a fresh round with a new room wheel selection).

## Existing digital twin

The supplied `PFTDigitalTwin.zip` was inspected without extracting the full project or launching Unreal.

| Item | Verified finding |
| --- | --- |
| Project descriptor | `PFTDigitalTwin/PFTDigitalTwin.uproject` |
| Engine association | `5.8` |
| Included editor build | Unreal **5.8.2**, according to `Binaries/Win64/PFTDigitalTwinEditor.target` |
| Project type | Native C++ module `PFTDigitalTwin`, with Blueprint/content assets |
| Startup and default map | `/Game/Level/PatrickFTaylorHall.PatrickFTaylorHall` |
| Configured game mode | `/Game/Blueprints/UnrealCore/BP_PFTGameModeBase.BP_PFTGameModeBase_C` |
| Configured game instance | `/Script/PFTDigitalTwin.PFTGameInstance` |
| Archive size | Approximately 6.94 GB compressed / 8.53 GB uncompressed, using decimal GB |
| File count | 115,506 files, including many external actor/object packages |
| Custom plugins included | `DigitalTwinTools`, `DMAEEditorToolsPlugin`, and content plugins |

Other maps include `MEP`, `L_MeshEdit`, and `RobotVehicleTest`. The archive also contains tour, room-search, area-trigger, UI, and food-delivery-related source files. Their presence does **not** prove that movement, room detection, collision, or existing missions meet this game's needs.

The building's geometry, classroom labels, collision, lighting, performance, and original gameplay have **not yet been verified in the editor**. Successful compilation and compatibility with a local engine installation also remain unverified.

### Asset handling

- Keep the original archive unchanged as a baseline.
- Extract the **whole project**, including `Content`, `Config`, `Source`, and `Plugins`. Do not copy only the hall's `.umap`: much of its content is stored in separate external actor/object packages.
- Do not rename or manually reorganize imported building assets. Unreal references can break; use editor-supported operations when a move is necessary.
- Do not commit the ZIP, engine installation, credentials, generated caches, or build output.
- This repository is public. Confirm permission to redistribute the digital twin and its third-party content **before** publishing assets.
- Decide the shared project/asset distribution method before multiple agents begin implementation. If assets will be versioned here, agree on Git LFS and Unreal asset handling first; this README does not establish that policy.

## Local setup

1. Obtain the supplied archive from the team's agreed location. It is not included in this repository.
2. Use **Unreal Engine 5.8.2** if available, matching the included build. Confirm the team's exact version before converting or saving the project in another engine version; changing `EngineAssociation` alone does not make assets or plugins compatible.
3. Prepare a Windows C++ toolchain and Windows SDK supported by that engine. Although our game logic can be Blueprint-first, the underlying project and custom plugins contain C++.
4. Extract outside OneDrive or other sync folders, using a short path such as:

   ```text
   C:\UnrealProjects\PFTDigitalTwin\PFTDigitalTwin.uproject
   ```

   The ZIP already contains a `PFTDigitalTwin` folder, so choose the extraction destination accordingly.

5. Leave room for the archive, extracted assets, engine, shader/derived-data caches, and build output. The 8.53 GB extracted size is not the total development footprint.
6. Open `PFTDigitalTwin.uproject`. If Unreal reports incompatible or missing modules, use the matching engine/toolchain to regenerate project files and build the editor target. Diagnose actual errors; do not remove plugins or native dependencies merely to bypass startup failures.
7. Open the configured `PatrickFTaylorHall` map, let required compilation finish, and test the existing experience in Play In Editor.
8. Record the working engine version, build steps, and any confirmed setup fixes in this README.

Playtesters using a shared package can follow the [player quickstart](docs/player-quickstart.md) and use the [bug report template](.github/ISSUE_TEMPLATE/bug-report.md).

**First editor check:** locate a usable starting area and classroom, verify walking-scale dimensions, floors, walls, doors, stairs, and a safe player spawn. A visually correct twin is not automatically a playable level.

## Architecture and integration guidance

Implementation should follow Blueprint-first gameplay built on top of the existing C++ project. The names below are **unverified conceptual proposals** to establish clear separation of responsibilities, not existing assets:

| Proposed component | Proposed responsibility |
| --- | --- |
| `BP_SunEscapeGameMode` *(proposed)* | Own round state (`Inactive`, `Active`, `Won`, `Lost`), assigned room, countdown timer, hazard expansion trigger, health, and terminal transitions. |
| Existing player pawn or `BP_SunEscapeCharacter` *(proposed)* | Player locomotion, collision, camera, and exposure sample point. No independent damage timer. |
| `BP_IndoorSun` *(proposed)* | Visual hazard front in Commons and corridors; exposure/hazard origin. |
| `BP_ClassroomGoal` / Door Actor *(proposed)* | Room identification, eligibility, wrong-door feedback trigger, and target arrival detection. |
| `WBP_SunEscapeHUD` *(proposed)* | Persistent target room number, countdown timer, health display, wrong-room notifications, and win/loss presentation. Reads state; does not resolve outcomes. |

### Architectural invariants

- **One active update source, one damage owner, one terminal transition:** The game mode alone manages round transitions and applies damage. Do not create competing damage loops in pawns, hazard volumes, or UI widgets.
- **Terminal outcomes within a run:** The proposed states are `Inactive`, `Active`, `Won`, and `Lost`; late overlap or trace callbacks cannot reverse a result. Only an explicit retry or spin-again action starts a fresh lifecycle.
- **Decoupled presentation:** The HUD reflects game state; it never adjudicates win/loss conditions or timers.

### Phased integration sequence

To manage scope and allow incremental verification, integration proceeds in three milestones (see [docs/gameplay-design.md](docs/gameplay-design.md)):

1. **Mechanical Prototype:** Sandbox map validating the wheel handoff, countdown, damage application, and shade relief.
2. **First PFT Route:** Grounded Commons exterior-door spawn and one audited walking route to a verified first-floor classroom with cover in PFT ([#30](https://github.com/Sweet-Rice/shepfinalproj/issues/30), [#31](https://github.com/Sweet-Rice/shepfinalproj/issues/31)).
3. **Route Expansion:** Two additional verified first-floor classrooms added to the wheel for playtesting.

## Verification checklist

Verification requires evidence from in-editor or packaged gameplay tests (tracked under [#17](https://github.com/Sweet-Rice/shepfinalproj/issues/17) and [#25](https://github.com/Sweet-Rice/shepfinalproj/issues/25)). Do not mark items complete without recorded test results.

Use the [round lifecycle cases](docs/qa/round-cases.md) for reproducible steps and a results log.

| Scenario | Expected result | Status |
| --- | --- | --- |
| Wheel handoff | At the Commons spawn, the wheel settles on one eligible room; the objective, movement, and configured countdown start together exactly once. | Unverified |
| Head start navigation | Player navigates during initial countdown; **zero hazard damage** occurs while countdown > 0. | Unverified |
| Hazard expansion at zero | At countdown 0, sun expands from Commons through corridors; unshaded player caught by front takes damage over time. | Unverified |
| Corridor reach pacing | Expanding hazard reaches most exposed corridors at ~60 s total from wheel handoff. | Unverified |
| Shade protection | Stepping into covered/shaded area stops damage accumulation while hazard continues spreading. | Unverified |
| Inspect wrong door | Interacting with or checking a non-target door displays its actual room number on HUD without failing or ending the round. | Unverified |
| Inspect target door (late win) | Checking the correct classroom door while alive transitions to `Won` immediately, even after countdown zero. | Unverified |
| Same-update target check & damage | A living player's valid target-door check wins before potentially lethal damage in that update. Mere room entry is not an agreed replacement for checking the door. | Unverified |
| Deplete health to zero | Health clamped to 0; transition to `Lost`; encounter updates and damage stop. | Unverified |
| Same-room retry | Defeat menu "Retry" cleanly resets player to Commons spawn, restores full health, resets timer and hazard, and keeps the same target classroom. | Unverified |
| Spin again | Defeat menu "Spin Again" starts a fresh round, resetting state and spinning the wheel for a room assignment. | Unverified |
| Packaged build validation | Packaged Windows executable loads map, plugins, assets, UI, and executes the lifecycle identically to PIE. | Unverified |

Use the [freshman playtest kit](docs/playtesting/facilitator-script.md) and [results sheet](data/playtest-results.csv) once a teammate can host a verified build. The starter rows are planning slots.

## Handoff rules for group members and their agents

1. **Inspect before editing.** This repository currently documents an external project archive. Verify files and check out actual assets before assuming source paths or assets exist.
2. **Preserve the twin.** Do not delete original missions, alter the global game instance, strip plugin dependencies, or resave the hall as incidental cleanup.
3. **Agree on contracts before splitting work.** Maintain the lifecycle, state ownership, and interface contracts documented in [docs/gameplay-design.md](docs/gameplay-design.md).
4. **Keep one owner per binary asset.** Coordinate changes to maps, Blueprints, widgets, and other `.uasset` files; Git cannot merge simultaneous binary edits.
5. **Separate independent work packages.** Reasonable split: wheel/round lifecycle, corridor hazard/shade mechanics, room/door identification, and HUD.
6. **Do not duplicate existing systems by assumption.** Inspect existing twin components (tour, room-search, area-triggers, pawn) before creating redundant implementations.
7. **Keep changes scoped.** No networking, procedural generation, or real stellar physics unless explicitly authorized.
8. **Report verifiable evidence.** Handoffs must cite tested build revisions, attach gameplay clips or screenshots, and record exercised scenarios under [#17](https://github.com/Sweet-Rice/shepfinalproj/issues/17).
9. **Keep documentation aligned.** Update documentation when design decisions are agreed upon or setup instructions change.

## Open decisions

The following questions remain open for team decision (see [docs/gameplay-design.md](docs/gameplay-design.md) and [Issue #18](https://github.com/Sweet-Rice/shepfinalproj/issues/18)):

- **Earlier damage model:** Decide whether to retain, adapt, or replace distance-based rates once the expanding front reaches the player. A clear line to the sun alone must not bypass the head start or damage players ahead of the front.
- **Cover and occlusion geometry:** Shade must stop damage while spread continues. Confirm how the front follows corridors and turns, how shade is detected, and which doors, glass, and small props provide cover; do not assume a single trace to the sun models the front.
- **Classroom arrival detection:** The intended player action is checking the correct door. Confirm input, range, and how door queries or existing volumes support that action; automatic victory on overlap is not yet agreed.
- **Tuning and pacing constants:** Finalizing damage per second, player walk speed, hazard spread rate, and the roster of eligible first-floor classrooms based on route walk times.
- **Project asset distribution & versioning:** Establishing Git LFS, Perforce, or external package hosting for binary Unreal assets.
