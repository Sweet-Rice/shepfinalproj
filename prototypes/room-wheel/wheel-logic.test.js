// Run: node prototypes/room-wheel/wheel-logic.test.js
"use strict";
const assert = require("assert");
const L = require("./wheel-logic.js");

const rooms = [
  { id: "1100", eligible: true },
  { id: "1200", eligible: false },
  { id: "1263", eligible: true },
  { id: "2150", eligible: true },
];

// Filtering drops ineligible rooms and the spawn room.
assert.deepStrictEqual(
  L.filterEligible(rooms, "1263").map((r) => r.id),
  ["1100", "2150"]
);

// Empty list is a setup error, not a silent pick.
assert.throws(() => L.pickIndex([], Math.random));

// pickIndex never goes out of range, even if rng returns 1.
assert.strictEqual(L.pickIndex([1, 2, 3], () => 0.999999999), 2);
assert.strictEqual(L.pickIndex([1, 2, 3], () => 1), 2);

// The stopping rotation always lands on the chosen segment, for every count,
// index, start angle and jitter we can throw at it.
const rng = L.seededRng(42);
for (let trial = 0; trial < 20000; trial++) {
  const count = 1 + Math.floor(rng() * 16);
  const index = L.pickIndex(new Array(count).fill(0), rng);
  const current = (rng() - 0.5) * 100000;
  const jitter = rng() * 2 - 1;
  const minTurns = Math.floor(rng() * 8);
  const end = L.targetRotation(current, index, count, minTurns, jitter);
  assert.ok(end >= current + minTurns * 360, "spins forward");
  assert.strictEqual(L.segmentAtRotation(end, count), index, `trial ${trial}`);
}

// Same seed, same sequence.
const a = L.seededRng(7), b = L.seededRng(7);
for (let i = 0; i < 10; i++) assert.strictEqual(a(), b());

console.log("wheel-logic: all tests passed");
