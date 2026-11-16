# MVP gameplay recording review

Review for [#33](https://github.com/Sweet-Rice/shepfinalproj/issues/33), using the user-supplied `mvpdemo.mp4`. This is a **visual review of recorded presentation**, not an interactive game test or a freshman usability study.

## Recording and provenance

- Duration: approximately **46.63 seconds**; source image size: **1918 × 1078**.
- SHA-256: `5b168b725aaaf2dbb9467bb1857389fd2298e4c73f6b395b62b89be712a06346`.
- Timecodes below are positions in this file, not elapsed game time. Review used decoded native-resolution frames across the recording, with finer checks around the hint and timer transitions. Readability judgments are limited to this capture; audio was not assessed.
- **Exact game/build revision: not supplied.** The wheel has five labeled segments and the displayed objective is 1240, but this does not identify a commit, package, or asset revision. Do not equate this file with the separately described recording in the [Unreal playtest report](unreal-playtest.md).
- The original media and extracted frames remain outside the public repository. This review does not redistribute the twin or screenshots.

## Three prioritized presentation fixes

These are recommendations grounded in visible UI, **not claims that a player was confused or that gameplay logic failed**. The order is for the next presentation pass, not a measured usability severity ranking.

### 1. Resolve the contradictory objective on victory

**Observed — 00:37–00:41:** The result card says `Room 1240 found`, with `R Retry room 1240` and `Enter Spin again`. Meanwhile the upper-left HUD still says `Find room 1240` and displays `Sun reaches here in 37s` above 100 HP. Thus the completed objective and the active-looking navigation/hazard HUD are presented together.

**Recommended change:** On victory, replace the active objective/timer with an unmistakable completed state, or hide them behind a clear victory presentation. Give the result headline stronger visual hierarchy while retaining the room number and the distinct retry/spin choices. The existing [player-copy proposals](../ui/player-copy.md#6-victory) provide wording options; they are not evidence of implementation.

**Needs a live check:** Confirm that an unfamiliar player recognizes completion and can distinguish same-room retry from another draw. The frozen-looking HUD does **not** prove that the simulation continues after victory, nor does showing the options prove either reset path works.

### 2. Make the head-start-to-hazard transition explicit

**Observed — 00:26.5–00:27.0:** The small status line changes from `Sun expands in 1s` to `Sun reaches here in 78s`, in the same position and visual treatment. The objective and health bar remain visible. The second value has a different meaning, but the sampled transition primarily communicates that through the changed sentence.

**Recommended change:** Add an explicit textual phase cue such as `Sun spreading — reach room 1240 alive`, with the local arrival estimate labeled separately. Keep the useful distinction between global head start and local danger; do not represent zero as defeat or add an automatic loss timer. Do not rely only on a color change. See the [countdown copy proposals](../ui/player-copy.md#2-countdown-warning).

**Needs a live check:** Ask a first-time player what zero means and whether the arrival estimate is a new safe countdown. That misunderstanding is a risk to test, **not an observed response**. This review does not establish the estimate's accuracy or whether an unsampled transient effect exists.

### 3. Keep the route hint available at navigation decisions

**Observed — 00:02–00:05.0:** The secondary HUD panel says `Follow the overhead signs for 1232-1274.` and `Stay on the first floor.` At **00:05.5** the panel is absent, before the subsequent Commons turns and stair-area approach at **00:08–00:16**. The persistent target remains, but the initial range/landmark instruction is no longer on screen.

**Recommended change:** Keep this short landmark/range hint available longer, or provide a clearly discoverable way to recall it. Preserve sign-based learning rather than adding a full route or turn-by-turn answer. A reviewer cannot infer from these frames whether another recall mechanism already exists.

**Needs a live check:** Confirm that a newcomer can retrieve the clue at a junction without facilitator guidance and can read the relevant overhead sign in motion. Camera turns alone are **not proof of getting lost**, and distant signs in a compressed capture are not sufficient to declare their text universally unreadable.

## Positive evidence and additional observations

- **00:00–00:02:** The visible wheel is followed by the room-1240 objective. This documents the presentation sequence, not a test of selection randomness or input locking.
- **00:02 and throughout navigation:** The target and numeric HP have a dark backing against the bright scene. The room number stays visible during the route.
- **00:20–00:21:** A contextual message reads `Get close and face a classroom door.` This is useful guidance; it is not a demonstrated wrong-room identification or proof of which input was pressed.
- **00:35–00:36:** The approached door has a visible 1240 label matching the HUD objective. This is evidence about the modeled door in this recording, not a physical campus survey or verification of every room.
- **00:37:** The recording shows a success result after the head-start transition while displaying 100 HP. It supports the visible late-success flow; it does not exercise simultaneous lethal damage ordering.
- **00:42–00:44:** A `Paused` / `Esc Resume` card appears, then the result card returns. The images alone do not prove which simulation clocks pause.

## Coverage limits and follow-up testing

The clip does not demonstrate exposed HP loss, the advancing front reaching the player, entering/leaving shade, or a loss screen. It therefore cannot establish sun/shade cue readability, damage/cover correctness, or loss presentation. It also does not show an executed retry, a new spin after the result, or successful wrong-door identification.

Use the existing [round lifecycle cases](round-cases.md) and [cover matrix](cover-cases.md) for those checks; their pending execution results are not converted to passes by this review. Validate other classrooms, keyboard/controller accessibility, smaller displays, audio cues, and unfamiliar-player comprehension separately. A compressed recording is not a rendering-performance benchmark.

## Sharing note

Around **00:45**, an app-switcher overlay briefly exposes unrelated desktop previews. Trim or redact that portion before wider sharing; no unrelated window contents are transcribed here. Keep the original recording in the team's approved location.
