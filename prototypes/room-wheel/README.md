# Classroom wheel prototype

Browser-only reference for [issue #22](https://github.com/Sweet-Rice/shepfinalproj/issues/22). No Unreal installation, server, package installation, or build step is required.

## Try it

Open `index.html` directly in a modern browser. Keep `index.html`, `styles.css`, and `wheel.js` together; all assets are local and the prototype works offline.

**Every room is a placeholder.** IDs `demo-101` through `demo-106` and displayed numbers 101–106 are sample data, not surveyed or eligible PFT classrooms.

- Click **Spin for room** (or **Spin again**) to select a room and animate the wheel.
- The fixed pointer at the top identifies the chosen sector. The text result must match it.
- During a spin, **Reveal selection now** skips to that spin's already-selected outcome.
- Check **Skip spin animation** before spinning to settle immediately. System reduced-motion preferences also bypass animation.
- Use **Tab / Shift+Tab** to navigate, **Enter / Space** to activate buttons, and **Space** to toggle the checkbox. Keyboard focus is visibly outlined.
- Each spin is independent; consecutive spins may select the same room.

## Selection and handoff contract

`wheel.js` owns the sample room list, wheel geometry, selection, and lifecycle. HTML and CSS provide presentation; they must not choose a second outcome or independently rotate the wheel.

A room is chosen once at the beginning of a spin. Animation, manual reveal, and reduced-motion completion all settle on that same room. New spins are disabled until completion, and stale animation callbacks must not issue another result.

The six sectors start clockwise from 12 o'clock. Sector `i` has center `(i + 0.5) * 60` degrees. The final wheel rotation modulo 360 is `360 - center`, placing that center beneath the pointer. Additional full rotations affect presentation only.

Completion dispatches one bubbling `room-selected` event on `#app`, after updating the result. Initial load does not dispatch it. Example listener:

```javascript
document.getElementById('app').addEventListener('room-selected', (event) => {
  const { roomId, label } = event.detail;
  console.log(roomId, label); // e.g. demo-101, 101
});
```

For inspection, `#app` exposes `data-state="idle|spinning|settled"` and the completed `data-selected-room`. The wheel's SVG rotation is applied to `#wheel-rotor`; the pointer and hub remain stationary.

## Reuse in Unreal

Reuse the behavior, not browser-specific DOM code:

1. Replace sample data with the team's verified, eligible first-floor classroom records.
2. Select exactly one target before presenting its animation; drive both the visual landing and displayed room number from that selection.
3. Route normal completion and skip through the same once-only handoff.
4. At handoff, commit the target, show its persistent room number, unlock player movement, and start the head-start countdown together.

Keep the game's authoritative state outside the wheel widget. This page is only a selection/presentation prototype: it does **not** connect to Unreal, start a game countdown, move a player, implement damage or sun expansion, or load the PFT twin. Its event is a local example of the integration boundary, not a working Unreal bridge.
