# Player-facing copy

Short on-screen strings for one round of the PFT sun-survival game. Resolves issue #24.

**Status:** draft copy for the team to review. Nothing here is implemented yet. String IDs are proposals for a Unreal String Table (for example `ST_PlayerCopy`), so the HUD and end screen can look text up by key instead of hard-coding it.

## Round flow these strings cover

1. The player spawns at a Commons exterior door.
2. A classroom wheel spins and lands on the target room.
3. A countdown runs (baseline 25 s, tunable 20–30 s).
4. At zero, the sun starts expanding. Exposed players take damage, and shade protects them.
5. Reaching the target room alive wins. This still counts after the countdown hits zero.
6. When health reaches zero, the round is lost. The end screen offers **Retry** (same room) or **Spin Again** (new room).

> The README describes the sun, damage, cover, and win/loss rules. The wheel, countdown, Commons spawn, and retry/spin-again flow come from the team's newer design discussion and are not in the README yet. See [Open questions](#open-questions).

## Writing rules

- **Room number first and loudest.** Every message that names the target puts `{ROOM}` near the start, in its own visual weight. Render `{ROOM}` bold or in the accent color, and at least 1.5× the surrounding text size on the wheel result and end screens. Never shorten or wrap the room number.
- **Readable at a glance.** HUD toasts should be no more than about 40 characters and readable in under 2 s. Banners and end-screen titles should be no more than about 24 characters. Body lines should be no more than about 70 characters.
- **Plain verbs.** Use *Find*, *Get to*, *Get out of the light*, *Retry*, and *Spin Again*. Avoid jargon such as "exposure" or "LOS".
- **Zero is not game over.** Countdown copy says the sun *starts expanding* at zero. It never says "time's up" or "you lose".
- **Retry ≠ Spin Again.** Retry always says **same room** and shows `{ROOM}`. Spin Again always says **new room** and never shows the old number as the goal.
- Sentence case, no exclamation-mark spam (one at most per screen), and no ALL CAPS except the room number when the font supports it.

### Placeholders

| Placeholder | Meaning | Example | Notes |
| --- | --- | --- | --- |
| `{ROOM}` | Target room ID exactly as signed in PFT | `1200` | From `BP_ClassroomGoal.RoomId`. Emphasize it (see above). |
| `{SECONDS}` | Whole seconds left on the countdown | `10` | Integer, no decimals, no leading zero. |
| `{ENTERED_ROOM}` | Room the player walked into that isn't the target | `1263` | Only in wrong-room feedback. Display it smaller than `{ROOM}`. |
| `{TIME}` | Round time from spawn to win/loss | `0:48` | `m:ss`. Optional; omit the line if not tracked. |

Max length is a guideline in characters **after** placeholders are filled with a 4-character room ID.

## 1. Wheel result (room assignment)

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `WHEEL_SPINNING` | Spinning for your room… | – | 28 | While the wheel is animating. |
| `WHEEL_RESULT_TITLE` | Room **{ROOM}** | `{ROOM}` | 12 | Big, centered, the moment the wheel stops. Hold ~1.5 s. |
| `WHEEL_RESULT_SUB` | Get there before the sun does. | – | 34 | Under the title, same moment. |
| `OBJECTIVE_PERSISTENT` | Find Room **{ROOM}** | `{ROOM}` | 18 | HUD corner, from wheel result until round ends. Never hidden. |
| `OBJECTIVE_PERSISTENT_LONG` | Find Room **{ROOM}** before the sun kills you. | `{ROOM}` | 46 | Optional alternative to the short objective (README wording). Use one, not both. |

## 2. Countdown warning

The countdown shows as a number on the HUD. These strings are the label and the escalating warnings.

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `COUNTDOWN_LABEL` | Sun expands in {SECONDS}s | `{SECONDS}` | 26 | HUD, next to the timer, for the whole countdown. |
| `COUNTDOWN_WARN_10` | 10 seconds. Find shade or get to **{ROOM}**. | `{ROOM}` | 44 | Toast at 10 s left. |
| `COUNTDOWN_WARN_5` | Sun expands in 5… | – | 20 | Toast at 5 s; the number then ticks 4, 3, 2, 1. |
| `COUNTDOWN_ZERO_TITLE` | The sun is expanding | – | 22 | Banner at 0, ~2 s. |
| `COUNTDOWN_ZERO_SUB` | You can still make it. Get to Room **{ROOM}**. | `{ROOM}` | 46 | Under the banner at 0. Covers the "still winnable" criterion. |
| `COUNTDOWN_EXPIRED_LABEL` | Sun expanding. Reach **{ROOM}** alive. | `{ROOM}` | 38 | Replaces `COUNTDOWN_LABEL` after zero, until the round ends. |

## 3. Shelter (in shade)

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `SHELTER_ENTER` | In shade. You're safe here. | – | 30 | Toast when the player goes from exposed to covered (after zero only). |
| `SHELTER_HINT` | Safe for now. **{ROOM}** is still the goal. | `{ROOM}` | 42 | If the player stays in shade ~5 s, so they don't stop moving. |
| `SHELTER_ICON_TOOLTIP` | Shaded | – | 8 | Label for a persistent cover/exposure indicator, if the HUD has one. |

## 4. Damage (exposed)

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `DAMAGE_EXPOSED` | You're in the sunlight! Find shade. | – | 36 | Toast on first damage tick after leaving cover. Throttle to once per exposure. |
| `DAMAGE_CLOSE` | Too close to the sun! | – | 22 | When in the near-damage band (higher damage rate). |
| `DAMAGE_LOW_HEALTH` | Low health. Get to shade or **{ROOM}**. | `{ROOM}` | 38 | Once when health first drops below ~25%. |
| `EXPOSURE_ICON_TOOLTIP` | Exposed | – | 8 | Opposite state of `SHELTER_ICON_TOOLTIP`. |

## 5. Wrong-room feedback

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `WRONG_ROOM` | That's {ENTERED_ROOM}. You need **{ROOM}**. | `{ENTERED_ROOM}`, `{ROOM}` | 36 | Toast when the player enters an eligible non-target classroom. Throttle per room. |
| `WRONG_ROOM_NO_ID` | Wrong room. You need **{ROOM}**. | `{ROOM}` | 30 | Fallback when the entered room has no known ID. |

## 6. Victory

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `WIN_TITLE` | You made it! | – | 14 | End-screen headline on `Won`. |
| `WIN_SUB` | You reached Room **{ROOM}** alive. | `{ROOM}` | 34 | Under the headline. |
| `WIN_SUB_AFTER_ZERO` | Room **{ROOM}**, with the sun right behind you. | `{ROOM}` | 46 | Instead of `WIN_SUB` if the win came after the countdown hit zero. |
| `WIN_TIME` | Time: {TIME} | `{TIME}` | 12 | Optional stat line. |

## 7. Loss

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `LOSS_TITLE` | The sun got you | – | 16 | End-screen headline on `Lost`. |
| `LOSS_SUB` | You didn't reach Room **{ROOM}**. | `{ROOM}` | 32 | Under the headline. Repeats the room so Retry makes sense. |
| `LOSS_TIP` | Tip: stay behind walls and move between shade. | – | 48 | Optional third line. |

## 8. Same-room retry

Use this for the **same target room** with a fresh round: full health, new countdown, same `{ROOM}`.

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `BTN_RETRY` | Retry Room **{ROOM}** | `{ROOM}` | 18 | End-screen button (win and loss). |
| `BTN_RETRY_HINT` | Same room. Fresh start. | – | 24 | Small text under/next to the button, or tooltip. |
| `RETRY_START` | Same room: **{ROOM}**. Go! | `{ROOM}` | 26 | Brief banner when the retried round begins (replaces the wheel). |

## 9. Spin again

Use this for a **new target room**: the wheel spins again, with full health and a new countdown.

| ID | Text | Placeholders | Max len | When it shows |
| --- | --- | --- | --- | --- |
| `BTN_SPIN_AGAIN` | Spin Again | – | 12 | End-screen button (win and loss). |
| `BTN_SPIN_AGAIN_HINT` | New room. New spin. | – | 20 | Small text under/next to the button, or tooltip. |
| `SPIN_AGAIN_START` | Spinning for a new room… | – | 26 | Replaces `WHEEL_SPINNING` when the wheel is triggered by Spin Again. Then the normal `WHEEL_RESULT_*` strings show. |

**Why these can't be confused:** Retry always names the room it will reuse, and Spin Again always says *new*. On the end screen, put the two buttons side by side with Retry on the left, so the room number on the left button is the first thing a player sees.

## Example: one round, top to bottom

```text
Spinning for your room…
Room 1200 — Get there before the sun does.
[HUD] Find Room 1200 · Sun expands in 25s
10 seconds. Find shade or get to 1200.
The sun is expanding — You can still make it. Get to Room 1200.
That's 1263. You need 1200.
You're in the sunlight! Find shade.
In shade. You're safe here.
You made it! — Room 1200, with the sun right behind you.
[Retry Room 1200]  [Spin Again]
```

## Acceptance criteria check

- **Room number easy to spot:** `{ROOM}` is in every assignment, objective, wrong-room, result, and retry string, with emphasis rules above. The wheel title is just `Room {ROOM}`.
- **Zero starts expansion, still winnable:** `COUNTDOWN_LABEL` says the sun *expands* at zero. `COUNTDOWN_ZERO_SUB` and `COUNTDOWN_EXPIRED_LABEL` say to reach the room alive, and `WIN_SUB_AFTER_ZERO` rewards it.
- **Retry vs. spin again distinct:** Retry says *same room* and shows `{ROOM}`. Spin Again says *new room* and shows no room.

## Open questions

1. **README sync.** The README doesn't yet describe the wheel, the countdown, the Commons spawn, or the retry/spin-again end screen. Should someone add those to the design baseline so this copy matches the source of truth?
2. **Room ID format.** Are rooms shown as signed in PFT (`1200`) or with a prefix (`PFT 1200`)? Are there any letters or suffixes (`1263A`)? This affects max lengths.
3. **Shade/damage toasts before zero.** Before expansion there's no damage, so shelter/damage strings are assumed to show only after zero. Confirm.
4. **Can Spin Again land on the same room?** If yes, should the wheel avoid the previous room, or should the copy acknowledge it ("Same room again!")?
5. **Is Retry offered after a win?** It's proposed on both end screens. Drop it from the win screen if replaying a won room has no point.
6. **Countdown display.** Should the timer show whole seconds only (assumed), and should the warnings be at 10 s and 5 s if the countdown is tuned to 20 s?
7. **Tone.** Is the playful/urgent voice right ("The sun got you"), or does the team want something drier for the class demo?
8. **Localization.** Assumed English-only. If not, keep all strings in a String Table and avoid building sentences by concatenation.
