/*
 * Room wheel — pure selection logic (no DOM).
 *
 * This is the part the Unreal implementer should port. Everything here is
 * deterministic given an RNG function, so it can be tested in Node:
 *   node prototypes/room-wheel/wheel-logic.test.js
 *
 * Angle convention: degrees, 0 = top (where the pointer is), increasing
 * clockwise. Segment i covers [i * size, (i + 1) * size).
 */
(function (root, factory) {
  if (typeof module === "object" && module.exports) {
    module.exports = factory();
  } else {
    root.RoomWheelLogic = factory();
  }
})(typeof self !== "undefined" ? self : this, function () {
  "use strict";

  /** Keep only rooms flagged eligible and not the room the player spawns in. */
  function filterEligible(rooms, spawnRoomId) {
    return rooms.filter(function (r) {
      return r.eligible && r.id !== spawnRoomId;
    });
  }

  /**
   * Choose the result FIRST. The animation is only ever told where to stop;
   * it never decides the room. Returns an index into `eligible`.
   */
  function pickIndex(eligible, rng) {
    if (!eligible.length) throw new Error("No eligible rooms to choose from");
    var i = Math.floor(rng() * eligible.length);
    return Math.min(i, eligible.length - 1); // guard rng() === 1
  }

  function mod(a, n) {
    return ((a % n) + n) % n;
  }

  /** Which segment sits under the top pointer for a given wheel rotation. */
  function segmentAtRotation(rotationDeg, count) {
    var size = 360 / count;
    // Wheel rotated clockwise by R: the pointer sees wheel angle -R.
    return Math.floor(mod(-rotationDeg, 360) / size) % count;
  }

  /**
   * Final rotation that lands segment `index` under the pointer after at least
   * `minTurns` full turns forward from `currentDeg`. `jitter` in [-1, 1] moves
   * the stop point off-centre (kept within 70% of the segment so it never
   * lands on a border).
   */
  function targetRotation(currentDeg, index, count, minTurns, jitter) {
    var size = 360 / count;
    var wheelAngle = (index + 0.5) * size + (jitter || 0) * size * 0.35;
    var desired = mod(-wheelAngle, 360);
    var delta = mod(desired - mod(currentDeg, 360), 360);
    return currentDeg + minTurns * 360 + delta;
  }

  /** Small seedable PRNG (mulberry32) so a ?seed= URL replays the same spins. */
  function seededRng(seed) {
    var a = seed >>> 0;
    return function () {
      a = (a + 0x6d2b79f5) >>> 0;
      var t = a;
      t = Math.imul(t ^ (t >>> 15), t | 1);
      t ^= t + Math.imul(t ^ (t >>> 7), t | 61);
      return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
    };
  }

  function clamp(v, lo, hi) {
    return Math.max(lo, Math.min(hi, v));
  }

  return {
    filterEligible: filterEligible,
    pickIndex: pickIndex,
    segmentAtRotation: segmentAtRotation,
    targetRotation: targetRotation,
    seededRng: seededRng,
    clamp: clamp,
  };
});
