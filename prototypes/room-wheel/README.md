# Classroom wheel prototype (browser only)

A throwaway HTML/CSS/JS prototype of the round-start **classroom wheel** for the PFT sun-survival game (issue #22). No Unreal, no build step, no network.

> Room numbers are **placeholders** (`1100`, `1200`, … `3142`). They are not verified PFT rooms or routes.

## Try it

Open `index.html` in a browser (double-click it, or `file:///…/prototypes/room-wheel/index.html`). If your browser blocks local scripts, serve the folder instead:

```bash
python3 -m http.server 8722 --directory prototypes/room-wheel
```

then visit <http://localhost:8722>.

| Input | Effect |
| --- | --- |
| Click **Spin** (or click the wheel) | Spin with animation |
| <kbd>Space</kbd> / <kbd>Enter</kbd> | Spin (on the focused Spin button, or anywhere outside a form field) |
| <kbd>Esc</kbd> or **Skip ⏭** while spinning | Jump straight to the already-chosen result |
| **Skip animation** checkbox | No animation at all; on by default if the OS asks for reduced motion |
| <kbd>R</kbd> | New round (reset) |
| `?seed=7` in the URL | Reproducible sequence of picks, for QA |

The side panel lets you toggle **eligible rooms**, choose a **spawn location** (that room is never assigned), and set the **countdown** (20–30 s, default 25). The **Handoff events** log shows exactly what the wheel tells the rest of the game.

Logic tests (needs Node): `node prototypes/room-wheel/wheel-logic.test.js`.

## How it guarantees "what you see is what you get"

1. **Pick first.** `pickIndex()` chooses the room from the eligible list before anything moves.
2. **Animate to the pick.** `targetRotation()` computes a stop angle that is several full turns ahead and lands inside the chosen segment (with a little jitter, never on a border).
3. **One finish path.** Mouse, keyboard, Esc-skip and the skip checkbox all call the same `spin()` → `finishSpin()`. Skipping just snaps to the same precomputed angle.
4. **Check.** `finishSpin()` recomputes the segment under the pointer with `segmentAtRotation()` and reports it as `landedIndex` next to `segmentIndex`; the tests assert they match across 20,000 random cases.
5. The animation timer, not `transitionend`, ends the spin, so a backgrounded tab can't leave the round stuck.

## What the Unreal implementer should reuse

Port the **logic**, not the DOM. Everything worth keeping is in [`wheel-logic.js`](wheel-logic.js):

| Prototype | Unreal suggestion |
| --- | --- |
| `filterEligible(rooms, spawnRoomId)` | In `SetupRound()`: gather `BP_ClassroomGoal`s where `Eligible` is true and the goal doesn't contain the player at spawn. |
| Empty list → "Setup problem", Spin disabled | Stay `Inactive` and surface the error (matches the main README's setup contract). |
| `pickIndex(eligible, rng)` | `RandomIntegerInRange` / a `FRandomStream`. Choose and store `TargetClassroom` **before** the wheel widget animates. |
| `seededRng(seed)` | Use an `FRandomStream` with a seed for repeatable test rounds. |
| `targetRotation(...)` / `segmentAtRotation(...)` | The wheel widget (UMG) receives the chosen index and only animates to it; the widget never chooses. Same angle convention: 0° at the pointer (top), clockwise. |
| Skip / reduced-motion | A "skip" input or accessibility setting snaps the widget to the final angle and fires the same completion. |
| `roomwheel:selected` event `{ roomId, countdownSeconds, … }` | An event dispatcher such as `OnTargetRoomChosen(RoomId)` that the HUD binds to for the persistent objective "Find Room {RoomId} before the sun kills you." |
| `roomwheel:countdownStarted` / countdown loop | Start the pre-sun countdown (proposed 25 s, tunable 20–30 s) **after** the wheel finishes. Use elapsed time, not frame counts. |
| `roomwheel:sunActive` | Countdown end → the game mode enables `UpdateSunEncounter` damage. One owner, one transition. |

Not meant to be reused: the SVG drawing, colours, CSS animation, and the event log panel.

## Files

- `index.html` — page and styles
- `app.js` — wheel drawing, animation, input, countdown, event log
- `wheel-logic.js` — pure selection/angle logic (port this)
- `wheel-logic.test.js` — Node assertions for the logic
