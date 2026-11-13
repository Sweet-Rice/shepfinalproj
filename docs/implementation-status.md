# Implementation status and evidence ledger

The [Unreal project and runner](../unreal/README.md) are checked in under `unreal/PFTFloor1Viewer`; the curated assets for `/Game/Maps/PFTWalkthrough` are shared separately.

**Source inspected:** [`57ef6b11ed265c442e54f8c65a9135df003183b0`](https://github.com/Sweet-Rice/shepfinalproj/commit/57ef6b11ed265c442e54f8c65a9135df003183b0), after PR #53. This is a source snapshot, **not a confirmed revision of the distributed ZIP**.

## Evidence classifications

- **Demonstrated:** Inspectable execution or capture establishes the stated behavior in the stated environment. A browser demonstration does not establish Unreal behavior.
- **Reported:** A linked teammate report states that the check passed. It has not been independently exercised in this local environment.
- **Unverified:** The required observation or record is still missing from the checked evidence. This does not mean the feature is absent or broken.

No Unreal editor or packaged game was run for this documentation update. In particular, source code presence is not a rendered gameplay demonstration. The [browser wheel](../prototypes/room-wheel/README.md) is a separate prototype with **placeholder room numbers**, not a verified PFT room roster or an engine integration test.

## Loop component ledger

| Component | Classification | Evidence and scope | Next check |
| --- | --- | --- | --- |
| Destination wheel | Reported | [PR #49](https://github.com/Sweet-Rice/shepfinalproj/pull/49), [#52](https://github.com/Sweet-Rice/shepfinalproj/pull/52): native wheel and packaged launch directly to it. | Approved capture of spin, settled selection, and persistent objective. |
| Movement handoff and head start | Reported | [PR #49](https://github.com/Sweet-Rice/shepfinalproj/pull/49), [#50](https://github.com/Sweet-Rice/shepfinalproj/pull/50): movement locked during the wheel phase, unlocked at handoff; damage-free head start. The documented prototype head start is 25 seconds. | Rendered handoff and readable timer; do not confuse the wheel phase with the navigable head start. |
| Commons spawn and classroom routes | Reported | [PR #50](https://github.com/Sweet-Rice/shepfinalproj/pull/50), [#52](https://github.com/Sweet-Rice/shepfinalproj/pull/52): both automated walking routes won with 100 HP. Times and limits are in the [route worksheet](commons-route.md#existing-engine-reports-not-a-completed-field-survey). | Annotated route reference, spawn/threshold inspection, and unfamiliar-player traversal. Automated walks are not the field audit. |
| Expanding sun and exposure damage | Reported | [PR #51](https://github.com/Sweet-Rice/shepfinalproj/pull/51): radial front, amber floor cue, and repeated automated checks. The [prototype](../unreal/README.md) reaches 32 m at 60 seconds after handoff. | Rendered cue clarity; corridor/occlusion behavior against the intended [design](gameplay-design.md). |
| Shade and exposure transitions | Reported | [PR #49](https://github.com/Sweet-Rice/shepfinalproj/pull/49), [#51](https://github.com/Sweet-Rice/shepfinalproj/pull/51): shade protection and a teal shelter boundary. | Inspect entry/exit feedback and execute the [cover matrix](qa/cover-cases.md); do not mark its pending cases passed from these aggregate reports. |
| Door checks, win and loss | Reported | [PR #49](https://github.com/Sweet-Rice/shepfinalproj/pull/49), [#50](https://github.com/Sweet-Rice/shepfinalproj/pull/50): automated lifecycle/door checks, including wrong-door and late-win behavior. | Rendered prompts and the [manual round cases](qa/round-cases.md), including terminal-state and same-update ordering checks. |
| Retry and spin again | Reported | [PR #49](https://github.com/Sweet-Rice/shepfinalproj/pull/49), [#50](https://github.com/Sweet-Rice/shepfinalproj/pull/50): round reset paths exercised by automation. [Quickstart](player-quickstart.md) documents R / Enter. | Human-visible reset of objective, health, timer, hazard, spawn, and prompts. |
| First-floor signs | Reported | [PR #53](https://github.com/Sweet-Rice/shepfinalproj/pull/53): 30 signs restored, fonts cooked, packaged signs visually checked by the author, and both walks repeated. | Approved sign/door captures showing legibility and useful arrow directions from the actual route. |
| Win64 package | Reported | [PR #52](https://github.com/Sweet-Rice/shepfinalproj/pull/52), [#53](https://github.com/Sweet-Rice/shepfinalproj/pull/53): Development package launched; #52 reports 1280×720 startup, 15 lifecycle checks, both walking runs, and ZIP integrity. | Confirm package custody and tested revisions; then a teammate-machine launch/performance record under [#32](https://github.com/Sweet-Rice/shepfinalproj/issues/32). |
| Human wayfinding and readability | Unverified | No approved gameplay recording or annotated screenshot set was found in the checked repository, PR #49–#53 discussions, or scoped local project/package locations. | [#33](https://github.com/Sweet-Rice/shepfinalproj/issues/33): review actual gameplay with timestamp/screenshot references, observations, and top three fixes. |

The reported **15 lifecycle checks and two walking runs are separate results**. Their aggregate pass report is not a reconstruction of individual test cases, a substitute for the manual matrices, or proof of human playability.

## Current owner and missing handoff

**Latest reporting builder:** @kennynguyen216, author of PR #49–#53. **Current package custodian/distribution owner:** not confirmed. Authorship alone does not establish current binary custody.

A [handoff request has been posted on #17](https://github.com/Sweet-Rice/shepfinalproj/issues/17#issuecomment-6828165525). To finish that record, obtain:

1. Confirmation of who holds the current approved build.
2. The tested gameplay source revision and asset/Perforce revision, exact engine version, map, and package identifier if a package is being shared. The source project's UE 5.8 association is not proof of the tested patch version.
3. An approved short gameplay clip or screenshot set, tied to that revision, plus the working-feature/blocker summary. Cover spawn, wheel/handoff, signs/route, sun/shade, door interaction, and result/retry transitions.

An approved capture can support no-engine review without requiring this machine to download or render the package. If sharing a package, use the team's approved private channel; a checksum is useful for matching files, not an additional acceptance gate. Keep private download links, credentials, asset-server locations, and restricted level assets out of this public repository.

[#32](https://github.com/Sweet-Rice/shepfinalproj/issues/32) also needs the host's hardware/settings and observed startup/performance. A successful launch on the builder's machine does not predict integrated-GPU performance here.

## Remaining issue boundaries

- [#17](https://github.com/Sweet-Rice/shepfinalproj/issues/17): this ledger and the request are updated; confirmed ownership/revisions and approved capture remain outstanding.
- [#19](https://github.com/Sweet-Rice/shepfinalproj/issues/19): complete the [route survey](commons-route.md) through a campus walk or annotated approved reference photos/footage.
- [#20](https://github.com/Sweet-Rice/shepfinalproj/issues/20): candidate landmarks/clues are recorded, but at least one destination still needs route/sign verification. The CSV is not the runtime wheel's selection gate.
- [#33](https://github.com/Sweet-Rice/shepfinalproj/issues/33): actual media is required before a timestamped review can be written. Browser/HUD mockups are not substitute gameplay evidence.

These issues remain open; this update does not claim their unmet acceptance criteria are complete.
