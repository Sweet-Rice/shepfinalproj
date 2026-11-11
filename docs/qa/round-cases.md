# Round Lifecycle Test Cases

Reproducible manual test cases for one round of the PFT sun game: wheel handoff → head start → sun expansion → win or loss → retry / spin again.

**Status:** Written without an Unreal build. Every result below is **Pending** until a teammate runs a real build and fills in the results table. Do not mark a case Pass from reading Blueprints or source alone (see [README → Verification checklist](../../README.md#verification-checklist)).

## Sources

| Source | What it establishes |
| --- | --- |
| [README → Gameplay baseline](../../README.md#gameplay-baseline), rule 9 | *"Within an encounter update, classroom success is checked before damage."* Reaching safety and taking a potentially lethal hit in the same update resolves as a win. |
| [README → Room detection and update ordering](../../README.md#room-detection-and-update-ordering) | Containment is evaluated before damage; `Won`/`Lost` are terminal until an explicit new round. |
| [README → Verification checklist](../../README.md#verification-checklist) | Row *"Enter target with health low enough for the next hit to be lethal → Victory takes precedence over that update's damage."* |
| [Issue #18](https://github.com/Sweet-Rice/shepfinalproj/issues/18) / [PR #34](https://github.com/Sweet-Rice/shepfinalproj/pull/34) (`docs/gameplay-design.md`, not yet merged) | Commons exterior-door spawn, wheel handoff, 25 s countdown (20–30 s tuning), no damage during head start, late win after zero, retry same room / spin again. |
| [Issue #29](https://github.com/Sweet-Rice/shepfinalproj/issues/29) | Movement and countdown start together when the wheel settles; same-room retry resets time and health; spin again chooses a room for a new round. |
| [Issue #31](https://github.com/Sweet-Rice/shepfinalproj/issues/31) | The sun begins spreading at countdown zero; shade protects; an alive player can win after zero. |

If PR #34 changes before merging, re-check the cases marked with a source of #18/#34.

## Tunable values

Use the build's actual values and record them in each result's **Notes**. Expected results are written against these symbols, not hard-coded numbers.

| Symbol | Meaning | Current target | Status |
| --- | --- | --- | --- |
| `T_head` | Initial countdown / head start after wheel handoff | 25 s default | Tunable **20–30 s**. Test the default, then repeat TC-02 and TC-03 at 20 s and 30 s. |
| `T_spread` | Time from handoff until most exposed corridors are reached | ~60 s | Pacing target only, not a pass/fail timer. |
| `MaxHealth` | Starting health | Not decided | Record the build value. |
| Damage rate | Health lost per second while exposed | Not decided | Record the build value; needed for TC-08. |

## How to record results

- **Build revision:** git commit SHA of the gameplay checkout (or package name/date for a packaged build), plus `Editor (PIE)` or `Packaged`.
- **Tester:** name or GitHub handle.
- **Result:** `Pass`, `Fail`, `Blocked` (cannot run yet, say why), or `Pending`.
- **Notes:** tunable values used, room ID, frame rate if relevant, and a link to a clip or screenshot for any Fail.

Record **observed** behavior in Notes; do not rewrite the Expected column to match what happened. If a case exposes a spec gap, add it to [Open questions](#open-questions) instead of inventing a rule.

## Common preconditions

Unless a case says otherwise:

- **P1.** The gameplay map/mode from the team's Unreal checkout is loaded (not the original digital-twin default mode).
- **P2.** At least two eligible classrooms are configured and verified reachable from the Commons spawn.
- **P3.** At least one verified shade/cover point exists on the route used.
- **P4.** HUD shows health, countdown, and objective. A way to read exact health and `RoundState` (debug HUD, print string, or console) is available — several cases need it.
- **P5.** Start each case from a fresh round (launch PIE, or use the explicit new-round action) unless the case continues from another one.

---

## Test cases

### TC-01 — Wheel handoff starts movement and countdown together

| | |
| --- | --- |
| **Preconditions** | P1–P5. Player at the Commons exterior-door spawn, wheel visible and not yet settled. |
| **Steps** | 1. Start a round. 2. While the wheel is still spinning, try to move (forward, strafe, look) and watch the countdown. 3. Wait for the wheel to settle. 4. Note the room the wheel landed on and the HUD objective. 5. Immediately try to move again. |
| **Expected** | During step 2 the player cannot move and the countdown has not started (shows `T_head` or is hidden). When the wheel settles, movement unlocks and the countdown starts from `T_head` in the same moment. The persistent objective shows the same room ID the wheel landed on, exactly once (no duplicate widget). Health is `MaxHealth`. The wheel picks a room only once per round. The assigned room is not the spawn location. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-02 — No damage during the head start

| | |
| --- | --- |
| **Preconditions** | P1–P5. Wheel has settled (TC-01). |
| **Steps** | 1. Record health at handoff. 2. For the whole head start, stay in the most exposed position you can find — including standing in the Commons where the sun will originate, in open line of sight of any sun visual, and facing it. 3. Record health at countdown 1 s and at 0. 4. Repeat with `T_head` = 20 s and 30 s. |
| **Expected** | Health is unchanged at `MaxHealth` for the entire head start at every tested `T_head`. `RoundState` stays `Active`. No damage feedback (flash, sound, HUD) plays. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-03 — Sun begins expanding at countdown zero

| | |
| --- | --- |
| **Preconditions** | P1–P5. Wheel has settled. |
| **Steps** | 1. Move to an exposed spot a short distance from the Commons, on the sun's expected path. 2. Watch the countdown reach 0. 3. Note when the sun's front starts moving and when it reaches you. 4. Record health just before and just after the front reaches you. 5. Repeat at `T_head` = 20 s and 30 s. |
| **Expected** | The front starts spreading from the Commons when the countdown hits 0 (handoff + `T_head`), not before. Damage starts only when the visible front reaches the unshaded player, not everywhere at the moment of zero. Once reached, health falls over time while exposed. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-04 — Wrong room does not win

| | |
| --- | --- |
| **Preconditions** | P1–P5. Wheel has settled; note the assigned room ID. A different eligible room is reachable. |
| **Steps** | 1. During the head start, go to a classroom that is **not** the assigned room. 2. Do the classroom check (enter the volume, or use the door-check interaction — see [Q1](#open-questions)). 3. Watch the HUD, countdown and `RoundState`. 4. Repeat once after countdown zero, while being damaged. |
| **Expected** | No win. `RoundState` stays `Active`. The objective still shows the original assigned room. If the build shows wrong-door feedback, it shows that door's actual room number. The countdown, sun and damage keep going (no pause, no extra penalty). |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-05 — Right room during the head start wins

| | |
| --- | --- |
| **Preconditions** | P1–P5. Assigned room is reachable within `T_head` (use the closest eligible room, or raise `T_head` and note it). |
| **Steps** | 1. Go straight to the assigned room. 2. Do the classroom check before countdown zero. 3. Wait 10 s and watch the HUD and state. |
| **Expected** | Exactly one transition to `Won`, with full health. The countdown stops (or no longer matters) and the sun never damages the player afterward. The win screen appears once. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-06 — Late win after zero while alive

| | |
| --- | --- |
| **Preconditions** | P1–P5. Assigned room is reachable on a route the front reaches after zero. |
| **Steps** | 1. Wait in the Commons until the countdown reaches 0. 2. Walk toward the assigned room, taking some exposed damage on the way (stay above zero; use shade if needed). 3. Do the classroom check with health > 0. 4. Stay where you are for 10 s. |
| **Expected** | `Won`, even though the countdown hit 0. Health stays at whatever it was when the win happened; it does not keep falling. The sun keeps expanding visually or freezes (either is fine — see [Q4](#open-questions)), but it causes no damage after `Won`. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-07 — Zero health loses

| | |
| --- | --- |
| **Preconditions** | P1–P5. |
| **Steps** | 1. After countdown zero, stand in exposed light outside every classroom and stay there. 2. Watch health until it reaches 0. 3. Then walk into (or check) the assigned room and wait 10 s. |
| **Expected** | Health reaches exactly 0, never negative. Exactly one transition to `Lost`; the loss screen shows once with the retry / spin-again options. Movement and damage stop (or have no effect). Reaching the assigned room afterward does **not** change the outcome to `Won`. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-08 — Right room in the same update as lethal damage

This is the edge case from [README rule 9](../../README.md#gameplay-baseline) and the verification checklist row *"Enter target with health low enough for the next hit to be lethal"*. Within one encounter update, the classroom check runs **before** damage, so a player who is still alive at the start of that update wins, even if that update's damage would take them to zero.

| | |
| --- | --- |
| **Preconditions** | P1–P5. Assigned room known. The front reaches the doorway/approach of that room while it is still exposed. The tester can set health directly (debug command or editor-exposed variable) or can read damage per second precisely. |
| **Steps** | 1. Get to the area just outside the assigned room after the front has reached it, standing in exposure. 2. Set health to a value that is smaller than one update's damage (for example below `damage rate × frame time`), or reduce frame rate (e.g. `t.MaxFPS 5`) so one update deals a large chunk. 3. In the same moment, step into the room / do the classroom check. 4. Repeat at least 5 times; record how many attempts actually hit the same update (a debug log line per update with health, containment and state makes this checkable). 5. Control run: do the same thing but stay one step outside the room. |
| **Expected** | Every attempt where the classroom check and the lethal damage land in the same update ends `Won`, not `Lost`, and health does not drop to 0 from that update. The control run ends `Lost` with health 0. There is never a `Won`→`Lost` or `Lost`→`Won` flip, and only one end screen shows. |
| **How to tell it hit** | Without a per-update log, this case can only be called *Plausible pass*. Note in the results which method was used. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-09 — Retry same room resets time and health

| | |
| --- | --- |
| **Preconditions** | Finish TC-07 (round ended `Lost`). Note the assigned room ID. |
| **Steps** | 1. Choose **Retry Same Room**. 2. Check spawn, health, countdown, sun extent, objective and `RoundState`. 3. Stay exposed through the new head start. 4. After the new zero, take one second of damage and compare the drop with the previous round. 5. Do the classroom check in the assigned room. |
| **Expected** | Same room ID as before; no wheel spin required. Player is back at the Commons spawn with `MaxHealth`. The countdown starts again from `T_head` when control returns, and the sun is reset to its starting extent. One objective widget, one health display (no duplicates). No damage during the new head start. Damage per second matches the earlier round (not doubled — no leftover update loop). The win in step 5 works normally. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-10 — Spin again starts a clean new round

| | |
| --- | --- |
| **Preconditions** | Finish TC-07 (round ended `Lost`). |
| **Steps** | 1. Choose **Spin Again**. 2. While the wheel spins, try to move and watch the countdown. 3. When it settles, compare the new room with the HUD objective. 4. Repeat steps 3–5 of TC-09. 5. Repeat the whole case 3+ times and record the rooms chosen. |
| **Expected** | Wheel shows again; movement stays locked and the countdown does not run while it spins. At settle: movement unlocks and the countdown starts from `T_head` together; HUD shows the new room (it may be the same room — a new spin is not guaranteed to differ). Same clean reset as TC-09: spawn, `MaxHealth`, sun extent, one HUD, normal damage rate. |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-11 — New round after a win

| | |
| --- | --- |
| **Preconditions** | Finish TC-05 or TC-06 (round ended `Won`). |
| **Steps** | 1. Use whatever restart/new-round action the win screen offers. 2. Check the same things as TC-09 step 2–4. |
| **Expected** | Clean reset as in TC-09/TC-10. The previous `Won` does not carry over; the new round can be lost. If the win screen offers no restart action, record **Blocked** and reference [Q3](#open-questions). |

| Build revision | Tester | Result | Notes |
| --- | --- | --- | --- |
| Pending | Pending | Pending | Pending |

### TC-12 — Shade stops damage but not the sun

| | |
| --- | --- |
| **Preconditions** | P1–P5, including P3. Detailed cover geometry is tested separately in `docs/qa/cover-cases.md` ([issue #26](https://github.com/Sweet-Rice/shepfinalproj/issues/26)); this case only checks the lifecycle side. |
| **Steps** | 1. After zero, get caught by the front and take some damage. 2. Step into verified shade and wait 10 s. 3. Step back out. |
| **Expected** | Health stops falling as soon as you are in shade, while the countdown/round keeps running and the sun keeps spreading. Damage resumes when you step out. `RoundState` stays `Active` throughout. |

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

1. **Q1 — What counts as reaching the classroom?** The README's baseline is containment (player actor location inside the room volume, win on entry). The design in PR #34 says the player *checks the correct door*, and says automatic win on entry is not an agreed replacement. TC-04, TC-05, TC-06 and TC-08 need to know which action to perform.
2. **Q2 — Countdown display after zero.** Does the HUD countdown hide, show 0, or switch to another indicator once the sun starts? Affects what the tester reads in TC-03 and TC-06.
3. **Q3 — Restart options after a win.** Retry Same Room / Spin Again are defined after a loss. What does the win screen offer, and does a "next round" after a win spin the wheel?
4. **Q4 — What the sun does after a terminal outcome.** Damage must stop, but should the sun keep expanding visually, freeze, or disappear?
5. **Q5 — Same-update test hook.** TC-08 is only reliable with a way to set health and a per-update log (health, containment, state). Can the implementer expose one in development builds?
6. **Q6 — Can Retry pick up a changed `T_head`?** If a designer changes the countdown between attempts, should Retry Same Room use the new value or the one from the original round?
7. **Q7 — Spin repetition.** A new spin may return the same room. Is that acceptable for playtests, or should Spin Again avoid the previous room?
