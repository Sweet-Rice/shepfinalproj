# Playtest results guide

Use with the [facilitator script](facilitator-script.md) and [results CSV](../../data/playtest-results.csv) for [issue #27](https://github.com/Sweet-Rice/shepfinalproj/issues/27).

**Preparation only:** No actual playtest, build execution, or participant evidence was produced for this kit. The CSV has five planned slots, not five observed participants. All observation cells are blank and every initial `outcome` is `not_run`.

## Use the sheet

1. Use 3–5 actual participants once a teammate can host an approved working build. Leave unused slots `not_run`; never interpret them as zero seconds, zero wrong doors, or failed understanding.
2. Give the session a batch code such as `S01`; use `P01`–`P05` only within that session. Do not keep a names-to-codes key or track identities across sessions.
3. Fill each participant's existing attempt-1 row with actual observations. For another attempt, append a row with the same session/participant, an incremented attempt number, and the appropriate `start_reason`. Do not overwrite a prior attempt.
4. Record understanding once, on attempt 1, **after all of that person's attempts and a neutral debrief**. Leave those fields blank on appended attempts.
5. Open the CSV in a spreadsheet, or edit it as comma-delimited text. Export as CSV; when editing manually, quote fields containing commas/newlines and double any embedded quotation marks. A pipe separates blocker codes within one cell, not CSV columns.

Do not record names, student IDs, email addresses, schedules, meeting dates, private links, credentials, or building assets. Notes should describe observed behavior; label suspected causes as inference. Participation is voluntary and can stop at any point. Do not require recordings.

## Data dictionary

The row key is `(session_id, participant_id, attempt_number)`. Keep these 14 columns in order:

| Column | Meaning and allowed values |
| --- | --- |
| `session_id` | Anonymous session batch, e.g. `S01`; not a timestamp or personal identifier. |
| `participant_id` | Session-local token `P01`–`P05`; reuse it for that person's retries. |
| `build_revision` | Exact host-confirmed build revision/package tag. Blank in unrun slots. |
| `attempt_number` | Starts at `1`; increase by one for every new attempt. |
| `start_reason` | `initial`, `same_room_retry`, or `spin_again`. |
| `room_id` | Target shown in the actual build; do not invent a classroom number. |
| `outcome` | `not_run`, `won`, `lost`, or `stopped`. |
| `route_seconds` | Observed elapsed seconds from handoff to outcome/stop; leave blank if no timing was obtained. |
| `wrong_door_checks` | Observed count of wrong-door feedback events, including repeat checks of one door. |
| `assisted` | `yes` or `no` for observed attempts; blank for unrun slots. Explain assistance in `notes`. |
| `understood_zero` | `yes`, `no`, `unclear`, or `not_asked`; debrief response recorded only on attempt 1. |
| `understood_shade` | Same response codes; record only on attempt 1. |
| `blocker_codes` | Unique codes from the taxonomy below, separated by `|` with no spaces. |
| `notes` | Brief anonymous observations/interventions; no personal details or private material. |

Blank observation cells on `not_run` rows mean **not observed**. On a finished observed attempt, an empty `blocker_codes` cell means no blocker was observed. Enter numeric `0` for wrong-door checks only when zero checks was actually observed; do not fill missing measurements with zero.

## Timing, attempts, and assistance

- **Start:** Wheel settles and movement plus the head-start countdown begin. Exclude the wheel animation. On same-room retry, start a new clock when control and the fresh countdown begin; the sun must not begin expanding before zero.
- **Stop:** Valid target-door check while alive (`won`), health reaches zero (`lost`), or the attempt ends early (`stopped`). A voluntary stop, technical interruption, or safety intervention may end an attempt. If it never reached handoff, leave `route_seconds` blank and explain why.
- **Wrong doors:** Count actual incorrect-door feedback, not walking past signs or merely entering another room. Checking the same wrong door twice counts twice.
- **Restarts:** Each observed `same_room_retry` or `spin_again` row represents one restart, not another participant. Each attempt gets its own clock and wrong-door count.
- **Assistance:** Before the run, explain only the confirmed controls. During it, navigation hints, control clarification, or technical/safety intervention require `assisted=yes` and a note. Do not describe assisted timing as unassisted performance.
- **Comparison:** Compare unassisted winning times only within the same build revision and target room. Report loss/stopped durations separately; never combine them into winning route times. State timing source and any estimate/uncertainty in notes.

## Understanding: score after play, not before

Ask the neutral questions in the script before explaining the answers. Do not teach zero or shade during the initial briefing.

| Field | What counts as `yes` |
| --- | --- |
| `understood_zero` | Explains that zero starts expansion rather than automatic defeat, and that an alive player can still win by checking the correct door. |
| `understood_shade` | Explains that shade stops incoming damage while the sun keeps spreading. Do not promise healing or a paused timer. |

Use `no` for an explicit misconception, `unclear` for a partial/vague answer, and `not_asked` if it was not assessed. An explicit claim that shade heals or freezes the spread is a misconception. Do not require the player to volunteer extra implementation details to receive `yes`.

## Blocker taxonomy

Use these codes only. For multiple observations, for example, use `sign_readability|door_feedback`; do not enter semicolon-separated or new category names.

| Code | Observation to group |
| --- | --- |
| `sign_readability` | Room signs were hard to read or notice. |
| `route_landmarks` | Player could not orient using the available route/landmarks. |
| `door_feedback` | Door checking or its room-number feedback was unclear. |
| `objective_visibility` | Assigned-room text was missed, clipped, or unclear. |
| `zero_meaning` | Observed confusion about what countdown zero means. |
| `shade_readability` | Safe shade versus exposed hazard was hard to distinguish. |
| `controls` | Difficulty using confirmed controls. |
| `performance` | Stutter, lag, or hitches interrupted play. |
| `build_failure` | Crash, fall-through, or another build failure blocked the attempt. |
| `other` | Another blocker; explain the observation in `notes`. |

These codes describe observations, not proven technical causes. Do not label a game bug solely because a player took a wrong turn.

## Group the most common blockers

A manual tally or spreadsheet is sufficient; no scripts or editor are required.

1. **Filter first:** Include only observed `won`, `lost`, or `stopped` rows. Exclude `not_run`, blank rows, and unknown outcomes. Report each session/build separately.
2. **Split codes:** Split each observed `blocker_codes` cell on `|`. For each `(session_id, participant_id)`, take the union of codes across all attempts.
3. **Count people, not retries:** Make a grid with one row per session/participant and one column per blocker code. Mark whether each code appeared at least once; sum the columns. Repeated occurrences on the same person's retries count once per code.
4. **Rank:** Sort those counts descending and report the three highest categories with anonymous supporting observations. Include ties at the cutoff explicitly; do not describe other observed codes as absent. If fewer than three categories occurred, report only those observed.
5. **Restarts:** Separately count observed rows whose `start_reason` is `same_room_retry` or `spin_again`, grouped by session/participant. Report each kind and the total; exclude `initial` and `not_run` rows.
6. **Understanding:** Use only observed attempt-1 rows, once per session/participant. Report `yes / assessed`, where assessed includes `yes`, `no`, and `unclear`; show `not_asked` separately. Blank retry responses are not `no`. If none were assessed, report `not assessed`, not a percentage or division by zero.
7. **Route outcomes:** Report unassisted winning times grouped by build/room, wrong-door counts, and the number of observed attempts. Keep missing measurements blank, and explain assistance or timing uncertainty.

When combining sessions, keep the full `(session_id, participant_id)` key. A reused `P01` is not the same record across sessions. Describe combined counts as anonymous participant-sessions, not verified unique people across sessions; no identity tracking is collected.

## Report after an actual test

Use a brief summary: build/session code; observed participant count and unused slots; top blocker categories/counts plus safe evidence; same-room/spin-again restart counts; zero/shade comprehension counts and unasked counts; unassisted winning route times by room; and limitations/interventions. Keep observations separate from proposed fixes. Do not report results before testing.
