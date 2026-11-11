# Round Lifecycle Test Cases

Reproducible manual test cases for one round of the PFT sun game: wheel handoff → head start → sun expansion → win or loss → retry / spin again.

**Status:** Written without an Unreal build. Every result below is **Pending** until a teammate runs a real build and fills in the results table. Do not mark a case Pass from reading Blueprints or source alone (see [README → Verification checklist](../../README.md#verification-checklist)).

## Sources

| Source | What it establishes |
| --- | --- |
| [Original README gameplay baseline](https://github.com/Sweet-Rice/shepfinalproj/blob/ab83623db70f3ae0870bad88106a19f9a9a29a84/README.md#gameplay-baseline), rule 9 | *"Within an encounter update, classroom success is checked before damage."* This historical permalink preserves the ordering source even if the README is reorganized. |
| [Issue #25](https://github.com/Sweet-Rice/shepfinalproj/issues/25) | Requires reproducible lifecycle cases, a right-classroom check in the same update as potentially lethal damage, and execution results left Pending until a build is tested. |
| [README verification checklist](../../README.md#verification-checklist) | A living player's valid target-door check wins before potentially lethal damage in the same update. |
| [Gameplay design](../gameplay-design.md) | Commons exterior-door spawn, wheel handoff, 25 s countdown (20–30 s tuning), no damage during head start, late win after zero, retry same room / spin again. |
| [Issue #29](https://github.com/Sweet-Rice/shepfinalproj/issues/29) | Movement and countdown start together when the wheel settles; same-room retry resets time and health; spin again chooses a room for a new round. |
| [Issue #31](https://github.com/Sweet-Rice/shepfinalproj/issues/31) | The sun begins spreading at countdown zero; shade protects; an alive player can win after zero. |

When the gameplay design changes, re-check the affected cases before running them.

## Tunable values

Use the build's actual values and record them in each result's **Notes**. Expected results are written against these symbols, not hard-coded numbers.

| Symbol | Meaning | Current target | Status |
| --- | --- | --- | --- |
| `T_head` | Initial countdown / head start after wheel handoff | 25 s default | Tunable **20–30 s**. Test the default, then repeat TC-02 and TC-03 at 20 s and 30 s. |
| `T_spread` | Time from handoff until most exposed corridors are reached | ~60 s | Pacing target only, not a pass/fail timer. |
| `MaxHealth` | Starting health | Not decided | Record the build value. |
| Damage rate | Health lost per second while exposed | Not decided | Record the build value; needed for TC-08. |

## How to record results

- **Build revision:** exact gameplay commit/package revision, engine version, map/mode, and `Editor (PIE)` or `Packaged`.
- **Tester:** public-safe tester token or GitHub handle; no participant names, schedules, private download links, or credentials.
- **Result:** `Pending` means not executed; `Pass` requires all recorded criteria to be observed; `Fail` means an observed contract violation; `Blocked` means an attempted variant lacks a required capability, agreed interaction, or evidence. There is no "Plausible pass" status.
- **Notes:** run/variant ID, actual configuration, room, observed behavior, and evidence reference. For TC-08 include the paired runs and authoritative update IDs. Do not substitute assumptions for observations.
- Copy the result row for each configuration and rerun; record the default, 20 s, and 30 s variants separately where required. Unexecuted variants stay `Pending`. If a build cannot expose a required setting, record that variant `Blocked` after confirming the limitation; one passing variant does not cover the others.
- Record core outcomes separately from subchecks that could not be exercised. For example, a win observed in shade does not prove exposed post-win damage suppression; a locked loss screen does not prove that a later door request was rejected.

Record **observed** behavior in Notes; do not rewrite the Expected column to match what happened. If a case exposes a spec gap, add it to [Open questions](#open-questions) instead of inventing a rule.

## Common preconditions

Unless a case says otherwise:

- **P1.** The gameplay map/mode from the team's Unreal checkout is loaded (not the original digital-twin default mode).
- **P2.** At least one eligible first-floor classroom is verified reachable from the inside-Commons exterior-door spawn. A known non-target door is additionally needed for TC-04; it need not be eligible for the wheel.
- **P3.** A verified shade/cover point is required only for cases that use it, notably TC-12 and a shade-assisted route in TC-06. Its absence does not block unrelated handoff/head-start cases.
- **P4.** HUD shows health, countdown, and objective. Exact health, the unrounded authoritative countdown, gameplay state, and update evidence are required for assertions that depend on them. A rounded HUD `0` is not by itself proof of the zero transition. Confirm actual diagnostic access; do not assume a debug command exists.
- **P5.** Start each case from a fresh round using host-confirmed launch/reset controls unless it explicitly continues a previous case. Keep required observations alive/nonlethal until the outcome under test.

Throughout these cases, **classroom check** means the explicit target/wrong-door action from the current brief, using host-confirmed input and detection rules recorded in Notes. Do not silently substitute automatic volume entry. If that action is unavailable or unresolved, record affected execution variants `Blocked` and reference [Q1](#open-questions).

---

## Test cases

### TC-01 — Wheel handoff starts movement and countdown together

| | |
| --- | --- |
| **Preconditions** | P1–P5. Player at the Commons exterior-door spawn, wheel visible and not yet settled. |
| **Steps** | 1. Start a round. 2. While the wheel spins, use the confirmed forward/strafe locomotion inputs and watch the countdown. Record camera-look behavior separately; it is not a lock requirement. 3. Wait for settle. 4. Compare the landed room with the HUD. 5. Test locomotion again and record the activation evidence. |
| **Expected** | Locomotion and the countdown do not run before settle. At handoff, locomotion unlocks and the countdown starts from `T_head` together. The persistent objective matches the single chosen room; health is `MaxHealth`, and no duplicate widget appears. Camera rotation before settle is not a defect unless the team separately specifies it. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-02 — No damage during the head start

| | |
| --- | --- |
| **Preconditions** | P1–P5. Wheel has settled (TC-01). |
| **Steps** | 1. Record health at handoff. 2. Stay outside the target at an exposed point, including a variant at the Commons sun origin. 3. Observe health and damage feedback only while the actual countdown is **strictly positive**; capture the final positive-countdown sample. 4. Run the default, 20 s, and 30 s variants separately and record their configuration/evidence. Use TC-03 for the zero boundary. |
| **Expected** | Health remains `MaxHealth`, the round remains active, and no damage feedback plays while countdown `> 0`. An update at zero or later is outside this assertion: if the front has already reached an unshaded player there, damage may legitimately begin. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-03 — Sun begins expanding at countdown zero

| | |
| --- | --- |
| **Preconditions** | P1–P5. Wheel has settled. |
| **Steps** | 1. Choose a verified exposed point ahead of the front and a survivable observation interval. 2. Record the last positive countdown, the zero transition, front position, and exact health before/after arrival. 3. Also check the Commons origin variant from TC-02. 4. Record default, 20 s, and 30 s variants separately. |
| **Expected** | Expansion starts at zero, not during the positive countdown. Damage begins only once the front reaches an unshaded player. This may be the first expansion update at the Commons origin; a point ahead of the front remains undamaged until arrival. Zero alone is not an automatic loss, and the roughly 60 s spread target is not an immunity window. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-04 — Wrong room does not win

| | |
| --- | --- |
| **Preconditions** | P1–P5. Target known; a different reachable door has a verified actual room number. It need not be an eligible wheel target. Use a fixture/interval that leaves health positive throughout both observations. |
| **Steps** | 1. Before zero, use the confirmed classroom-check action on the non-target door. 2. Record the actual-room feedback, objective, countdown, and state. 3. Repeat after zero with positive health before and after the observation; exposure may continue only where the front has reached the unshaded player. |
| **Expected** | The checked door's actual room ID is shown; the assigned objective is unchanged and no win occurs. While alive, the run remains active. A wrong-door check neither pauses time/spread/damage nor adds a separate penalty. Missing wrong-door feedback is not an optional pass. A legitimate lethal exposure is not evidence that the wrong-door check itself failed; rerun with a survivable fixture. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-05 — Right room during the head start wins

| | |
| --- | --- |
| **Preconditions** | P1–P5. Assigned room is reachable within `T_head` (use the closest eligible room, or raise `T_head` and note it). |
| **Steps** | 1. Reach the target and perform the confirmed door check while countdown is positive. 2. Record the win and health. 3. Observe through the former zero/front-arrival interval where the fixture permits. Record whether post-win exposure or a normal damage/update callback was actually exercised; otherwise leave that suppression subcheck `Blocked`, not inferred from standing in shade. |
| **Expected** | Exactly one transition to `Won` with full health. Later callbacks/exposure, when exercised, cannot lower health or reverse the outcome. Record terminal countdown/sun presentation without inventing a required animation policy; the win screen appears once. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-06 — Late win after zero while alive

| | |
| --- | --- |
| **Preconditions** | P1–P5. Assigned room is reachable on a route the front reaches after zero. |
| **Steps** | 1. Let zero occur and survive until the front has arrived on the chosen route. 2. Take a recorded nonlethal exposure interval, using verified shade if needed. 3. Check the assigned door with health `> 0`. 4. Record health/outcome and any later exposure or normally delivered callbacks; identify unexercised subchecks rather than inferring protection from shade alone. |
| **Expected** | The valid door check produces `Won` despite countdown zero. Health/outcome do not change from subsequent damage/update callbacks when exercised. Record sun visuals separately without a pass/fail requirement: freezing, continued animation, or disappearance is a pending presentation decision. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-07 — Zero health loses

| | |
| --- | --- |
| **Preconditions** | P1–P5. |
| **Steps** | 1. After zero and front arrival, stand outside the target in verified exposure. Before the lethal portion, capture a nonlethal baseline interval for TC-09: fixed position, damage/cover configuration, elapsed time, entry/exit health, and front state. 2. Continue until health reaches zero. 3. Attempt ordinary move/check inputs and observe terminal behavior. If controls still allow a target-door check, exercise it; if locked, record that subcheck as unexercised rather than requiring post-loss walking. |
| **Expected** | Health clamps to zero and one transition to `Lost` occurs, with one loss presentation. Later input/update activity cannot resurrect the round or apply meaningful extra damage. Locked controls are allowed. A post-loss target-check rejection is claimed only if the check actually reached the normal gameplay path; otherwise record that probe `Blocked` separately. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-08 — Right room in the same update as lethal damage

This is the same-update ordering case required by [issue #25](https://github.com/Sweet-Rice/shepfinalproj/issues/25) and the [original README rule 9](https://github.com/Sweet-Rice/shepfinalproj/blob/ab83623db70f3ae0870bad88106a19f9a9a29a84/README.md#gameplay-baseline). Human-timed doorway movement, reduced FPS, or repeated lucky walks cannot establish this result.

| | |
| --- | --- |
| **Required capability** | A host-confirmed development fixture can hold relevant gameplay clocks, restore the same damage-relevant state, queue the real door-check input/request, and advance one controlled authoritative round update with a recorded interval. It records ordered check/state/damage evidence. These capabilities are requirements to confirm, **not existing features or commands asserted here**. Missing capability or timing proof means `Blocked` on attempted execution; no build run here means `Pending`. Implementing engine hooks is outside this documentation change. |
| **Fixture** | Active round at zero or later; front already reaches an unshaded player at a valid target-door interaction position. Starting health `H > 0`; production damage for the controlled interval would be `D >= H`. Restore the same target, transforms, exposure/cover, front/countdown, interval, and damage settings for both runs, plus any relevant timer/random state. No earlier check/damage may resolve between fixture restore and the controlled update. |
| **Steps — valid check** | 1. Record fixture/configuration and freeze before the round update. 2. Queue a real valid target-door check through the confirmed gameplay input path; do not assign `Won` or force a validation result. 3. Advance one update `U`, including the build's actual interaction/update phases. 4. Capture positive health at entry, valid check association with `U`, transition order, applied damage, and exit state/health. |
| **Steps — paired control** | 5. Restore the equivalent fixture and fixed interval, omitting **only** the check request. Do not move the player away from the door or alter exposure. 6. Advance the control update through the same production path and capture its computed pre-clamp damage, health, and outcome. 7. Observe normally delivered subsequent callbacks and record whether they change either terminal result or duplicate presentation. Do not claim callback guards were exercised if the build generated no such callbacks. |
| **Expected** | Valid-check run: player starts alive, the validated target check is associated with `U`, exactly one `Won` occurs before damage, and exit health equals `H`. The winning path skips damage after success; do not invent a computed/applied damage observation. Paired no-check control: production damage proves `D >= H`, health clamps to zero, and exactly one `Lost` occurs. Any subsequently exercised callbacks must not flip outcomes or duplicate end screens. |
| **Evidence required for Pass** | Link both run IDs to the restored fixture and record authoritative update/event IDs, entry state/health, checked/target room IDs and validation, interval and exposure/damage inputs, control damage `D`, applied damage, exit health/state, and terminal transition count. Explain how the build maps its interaction phases to the authoritative round update; a rendered-frame number alone is insufficient. Core ordering passes only when **both** runs prove the shared fixture and ordering; missing proof is `Blocked`, an observed violation is `Fail`. Keep unexercised later-callback checks distinct. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-09 — Retry same room resets time and health

| | |
| --- | --- |
| **Preconditions** | Finish TC-07 (round ended `Lost`). Note the assigned room ID. |
| **Steps** | 1. From TC-07, choose the confirmed **Retry Same Room** action. 2. Inspect spawn, health, countdown, front, target, and UI. 3. Verify no damage during the new strictly-positive head start. 4. After front arrival, repeat TC-07's recorded nonlethal baseline under matched position, exposure, elapsed interval, and damage settings. If equivalence cannot be established, mark only that rate-comparison variant `Blocked`, not a doubled-damage defect. 5. While still alive, check the assigned door. |
| **Expected** | Same target, Commons spawn, `MaxHealth`, fresh countdown starting with control, reset front, and one HUD instance. No new head-start damage. Matched exposure intervals produce the same normal damage behavior, not a duplicate update loop; do not compare unlike distances/front states. The valid door check still wins. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-10 — Spin again starts a clean new round

| | |
| --- | --- |
| **Preconditions** | Finish TC-07 (round ended `Lost`). |
| **Steps** | 1. From a confirmed `Lost`, choose **Spin Again**. 2. Verify locomotion/countdown remain stopped while the wheel spins. 3. At settle compare the landed room with the HUD and verify the fresh handoff. 4. Run the head-start and matched-exposure variants from TC-09, then check the target while alive. 5. Before each repetition, recreate the `Lost` precondition through TC-07; do not assume a win screen offers Spin Again. Record each run/room separately. |
| **Expected** | Wheel shows again; movement stays locked and the countdown does not run while it spins. At settle: movement unlocks and the countdown starts from `T_head` together; HUD shows the new room (it may be the same room — a new spin is not guaranteed to differ). Same clean reset as TC-09: spawn, `MaxHealth`, sun extent, one HUD, normal damage rate. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-11 — New round after a win

| | |
| --- | --- |
| **Preconditions** | Finish TC-05 or TC-06 (round ended `Won`). |
| **Steps** | 1. From TC-05/TC-06, record the actual offered new-round action. If none exists, record `Blocked` and Q3 rather than inventing a control. 2. Follow the corresponding same-room or wheel-handoff reset path. Use an equivalent recorded pre-win baseline for rate comparison; if none exists (for example after TC-05), mark that subcheck `Blocked` rather than comparing arbitrary exposure. 3. In the new active round, stay exposed outside the target until health reaches zero and record the new loss. |
| **Expected** | Clean reset follows the offered action. The previous `Won` does not carry over: the new round can transition once to `Lost` at zero health. Observe that transition rather than assuming a reset display proves it. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-12 — Shade stops damage but not the sun

| | |
| --- | --- |
| **Preconditions** | P1–P5 with a verified shade point and enough health to survive the entry/exit exposure. Detailed geometry/door/glass policies belong to [issue #26](https://github.com/Sweet-Rice/shepfinalproj/issues/26), not assumptions here. |
| **Steps** | 1. After zero and front arrival, record damage in exposure. 2. Enter verified shade and observe a recorded interval while alive. 3. Exit to the same reached, unshaded area and record resumed damage before health reaches zero. If no survivable setup is available, record `Blocked`. |
| **Expected** | Shade stops damage while time/round and spread continue; leaving shade resumes damage only where the front has reached the player. The player remains alive and the run active during this comparison. A legitimate death is not by itself a shade failure. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

---

## Summary

| Case | Area | Result |
| --- | --- | --- |
| TC-01 | Wheel handoff | Pending |
| TC-02 | No damage during head start | Pending |
| TC-03 | Expansion at zero | Pending |
| TC-04 | Wrong room | Pending |
| TC-05 | Win during head start | Pending |
| TC-06 | Late win (after zero, alive) | Pending |
| TC-07 | Zero-health loss | Pending |
| TC-08 | Right room vs lethal damage, same update | Pending |
| TC-09 | Retry same room | Pending |
| TC-10 | Spin again | Pending |
| TC-11 | New round after a win | Pending |
| TC-12 | Shade stops damage (lifecycle) | Pending |

## Open questions

These are spec gaps found while writing the cases. They are **not** decided here; the expected results above avoid depending on a particular answer where possible.

1. **Q1 — Door-check mechanism.** Confirm the actual input, range, and validation/detection path for checking the assigned or wrong door. The current brief calls for a check; an older containment-only proposal is not an interchangeable pass condition. Cases needing an unavailable or unresolved interaction are `Blocked` on attempted execution.
2. **Q2 — Countdown display after zero.** Does the HUD countdown hide, show 0, or switch to another indicator once the sun starts? Affects what the tester reads in TC-03 and TC-06.
3. **Q3 — Restart options after a win.** Retry Same Room / Spin Again are defined after a loss. What does the win screen offer, and does a "next round" after a win spin the wheel?
4. **Q4 — What the sun does after a terminal outcome.** Damage must stop, but should the sun keep expanding visually, freeze, or disappear?
5. **Q5 — Same-update capability.** Confirm whether the host can prepare/restore the equivalent fixture, queue the real check, step one authoritative update, and capture the paired traces specified in TC-08. No hook/console command is assumed to exist; without that proof the attempted case is `Blocked`, not a guessed pass.
6. **Q6 — Can Retry pick up a changed `T_head`?** If a designer changes the countdown between attempts, should Retry Same Room use the new value or the one from the original round?
7. **Q7 — Spin repetition.** Repetition alone is not a failure: a new independent spin may return the previous room. Any later non-repeat policy must be agreed and recorded rather than silently added to these tests.
