# Freshman playtest facilitator script

Preparation for [issue #27](https://github.com/Sweet-Rice/shepfinalproj/issues/27): a host runs an approved build while an observer facilitates a **3–5-person** playtest. No session or build execution was performed for this change. The [CSV](../../data/playtest-results.csv) contains five `not_run` planning slots, not observations.

Use the [results guide](results-guide.md) for field definitions, blocker codes, and grouping. The intended loop comes from [issue #18](https://github.com/Sweet-Rice/shepfinalproj/issues/18); build handoff and sharing are tracked in [#17](https://github.com/Sweet-Rice/shepfinalproj/issues/17) and [#32](https://github.com/Sweet-Rice/shepfinalproj/issues/32).

## Before participants arrive

The host and observer complete these checks; they are not claims of checks already performed:

- [ ] Obtain an approved working package, exact revision, and confirmed movement/door-check controls. If missing, stop preparation and request the handoff; do not invent keys or a build revision.
- [ ] Host verifies the Commons spawn, assigned room/HUD, wheel handoff, countdown, zero-triggered expansion, shade feedback, correct-door victory, and retry. Check the first route is walkable. Do not expose the participant to this demonstration before their unaided attempt.
- [ ] Confirm the workstation and input devices work. Never disable OS security, firewall, or antivirus to run the game.
- [ ] Prepare the CSV and a stopwatch. Use a session code and `P01`–`P05`; fill only the 3–5 slots actually used and leave the rest `not_run`.
- [ ] Reset the build to its ready state. The host runs it; the observer times and notes behavior without giving route hints.

Participation is voluntary. Do not collect names, student IDs, contact details, schedules, meeting dates, or an identity lookup key. Recordings are not required. Keep notes anonymous and do not publish private links, credentials, or game/building assets.

## Read aloud before play

> Thanks for helping us test the game. We are testing its signs, routes, and prompts—not your abilities. You can stop or take a break at any time.
>
> You will start in the Commons and receive a classroom assignment from a wheel. Use the game's instructions and signs to find that classroom while staying alive. Once play begins, I cannot give directions or point out the route.
>
> Before we start, let's review the confirmed movement and door-check controls. Do you have any questions about those controls?

Review only controls confirmed by the host. **Do not explain what zero means or how shade works before play or between attempts**: understanding is assessed afterward, before teaching the answers.

## Observe each attempt

1. Fill the participant's existing attempt-1 row with the actual `build_revision` and `room_id`. Start `route_seconds` at wheel handoff, when the objective, movement, and countdown begin together. Exclude the wheel animation.
2. Stay neutral: no verbal navigation hints, pointing, nodding toward the correct route, or corrective reactions. If asked for directions, say: **“Please use what you can see in the game.”**
3. Count `wrong_door_checks` whenever a wrong-door feedback event occurs; repeat checks of one door count separately. Do not count passing a sign or walking past a door. Note visible blockers using the guide's `blocker_codes`; distinguish observations from suspected causes.
4. Stop timing at a valid target-door win while alive (`won`), zero-health defeat (`lost`), or an early stop/pause (`stopped`). Record elapsed seconds and observed checks. If handoff never happened or timing was unavailable, leave time blank and explain; do not invent zero.
5. Intervene immediately for safety or technical problems. Set `assisted=yes` and describe what happened; if intervention ends/interrupts the attempt, record `stopped`. Navigation hints or control clarification during play also require this flag. Exclude assisted times from unaided comparisons; do not resume a paused clock and call it a clean run.
6. If the player chooses another attempt, append a new row with the same session/participant and the next `attempt_number`. Set `start_reason=same_room_retry` or `spin_again`. Keep the same target for same-room retry; spin again may repeat a room. Each attempt starts a fresh clock when control and the fresh countdown begin—exclude menus/respawn. Do not overwrite the previous row or carry its time/check counts forward.

For unassisted observed attempts, record `assisted=no`. Keep all observation fields blank on unused `not_run` rows. The participant can stop rather than retry; do not pressure them to finish.

## Debrief after their final attempt

Ask without supplying the correct answer first:

> What did you understand would happen when the countdown reached zero?
>
> What did you understand shaded or covered areas did during your run?

Record `understood_zero` and `understood_shade` **once on that participant's attempt-1 row**, after this conversation. Leave both fields blank on retry rows.

| Field | Score `yes` when the participant explains… |
| --- | --- |
| `understood_zero` | Zero starts expansion, not automatic defeat; an alive player can still check the correct door and win. |
| `understood_shade` | Shade stops incoming damage while the sun keeps spreading. |

Use `no` for an explicit misconception, `unclear` for a partial/vague answer, and `not_asked` when unassessed. Claims that shade heals or freezes the sun are misconceptions. Do not demand extra technical details. After recording the responses, you may explain the intended rules and thank the participant.

## Wrap up the session

Follow the [results guide](results-guide.md), without collecting identities:

- Include only observed `won`, `lost`, or `stopped` attempts; exclude unused slots.
- Group blockers by distinct `(session_id, participant_id)` per code across retries. Count one person once per blocker, not once per attempt.
- Report the three most common blocker categories with safe observations and explicit ties. If fewer occurred, do not invent more.
- Count observed same-room/spin-again restart rows separately. Summarize understanding once per participant; use assessed responses as the denominator and report `not_asked` separately.
- Compare unaided winning times only for the same build and room. Keep losses/stops, missing timings, and interventions separate. No real metrics are available until an actual test is run.
