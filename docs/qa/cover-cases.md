# Cover and Exposure QA Test Matrix

This document defines the formal QA test matrix for hazard front expansion, occlusion cover, and player exposure mechanics under [Issue #26](https://github.com/Sweet-Rice/shepfinalproj/issues/26). It establishes reproducible test fixtures and acceptance criteria based on encounter rules outlined in [Issue #18](https://github.com/Sweet-Rice/shepfinalproj/issues/18) (gameplay baseline), [Issue #31](https://github.com/Sweet-Rice/shepfinalproj/issues/31) (visible hazard alignment and shade protection), and [Issue #25](https://github.com/Sweet-Rice/shepfinalproj/issues/25) (verification baseline).

> **Schematic Notice:** The diagrams and case definitions below are unmeasured conceptual test fixtures. They do not represent Patrick F. Taylor Hall (PFT) building geometry, are not to scale, and assert no real-world coordinates, room numbers, or metre measurements.

![Cover Test Fixtures](cover-fixtures.svg)

---

## Intended Encounter Baseline & Principles

1. **Head Start & Hazard Expansion:** No hazard damage occurs during the configured positive countdown. At zero, the front begins expanding from the Commons through corridors; zero is not an automatic loss. The roughly 60-second corridor-reach target is measured from wheel handoff, not added after the head start, and is neither a player deadline nor an immunity window.
2. **Front-Gated Damage:** Damage occurs only when the local hazard reaches an unshaded player. A clear view of a central sun alone must not inflict damage before front arrival.
3. **Occlusion & Shade:** Intended opaque cover stops exposure damage while surrounding spread continues. The host must establish the tested point's protection under the agreed model, not infer it solely from the sketch.
4. **World-Space Exposure:** Looking away is not cover. Exposure follows world-space hazard/cover conditions; this document does not prescribe a particular trace or volume implementation.
5. **Open Rulings:** Detailed damage-per-second (DPS) curves, door/glass transmission, corner propagation mechanics, boundary inclusion, and alignment tolerances remain open design and engineering decisions.

---

## Test Fixture Schematics

The cases below reference two standardized schematic fixtures depicted above:

### Figure 1: Straight Corridor Fixture
- **`S` (Upstream Origin):** Hazard/sun origin located at the upstream (left) end of a straight corridor segment.
- **`F` (Hazard Front):** Advancing hazard boundary propagating downstream (left to right) toward the corridor exit.
- **`A` (Exposed Player Point):** Downstream player sample position located in the open main corridor path.
- **`W` (Opaque Barrier Slot):** Opaque obstacle protruding from the corridor wall, occluding direct origin sightlines to point `H`. (Substituted by closed doors or glass panels in policy test variants).
- **`H` (Candidate Cover Point):** Position behind barrier `W` along transit path `A -> H -> A`. Labelled strictly as a *candidate* cover point requiring host verification under the active cover model.

### Figure 2: L-Corner Fixture
- **`S` (Upstream Origin):** Hazard origin located at the left end of an upstream horizontal corridor arm.
- **`B` (Corner Bend):** 90-degree corridor junction connecting the horizontal and vertical arms.
- **`P` (Downstream Player Point):** Player sample position located in the downstream vertical corridor arm.
- **`F` (Hazard Front):** Advancing hazard front shown upstream of bend `B`, with forward path turning around `B` toward `P`. The interior corner occludes the direct straight-line ray from `S` to `P`.

---

## Host Setup & Measurement Guidance

- **Environment & Instrumentation:** Record build revision, engine version, map, configuration, and actual diagnostic access. Record the model's front-arrival/coverage, shade/exposure, health change, and update association—not screen brightness. These are semantic measurements, not assertions that variables or console commands with particular names exist.
- **Sample Point & Boundary:** Confirm the player reference (point/capsule), the visual front reference (not bloom), boundary inclusion, and relevant update cadence. Do not invent a one-frame, pixel, or world-distance tolerance.
- **Alive & Positive Control:** Keep health positive throughout comparisons. First demonstrate positive damage at `A` under the tested configuration; zero-health clamping, a zero-damage tuning region, or the head start must not masquerade as protection.
- **Cover Proof:** Damage at `A` alone does not prove that `H` is protected by `W`; `H` might simply be unreached. Record protective classification or the agreed equivalent control proving the wall's effect. If the model encodes cover as excluded hazard occupancy, document an appropriate matched control. Otherwise leave the cover claim `Blocked` on attempted execution.
- **Policy & Execution:** Door/glass decisions are separate from execution results. Record the agreed rule/configuration before evaluating them; if policy or required evidence is missing during an attempt, use `Blocked`, not a guessed pass or a policy-based failure.
- **Recording:** All observations/results/evidence below are `Pending`: no game was run here. After execution, record build, fixture/sample positions, interval/update evidence, observed health/coverage, and deviations separately from Expected. Copy each record for repeat/configuration variants; never overwrite earlier evidence.

---

## Test Cases

### CV-01: Exposed Corridor (Pre-Front and Post-Front)
- **Fixture:** Figure 1 (Points `S`, `F`, `A`).
- **Steps:**
  1. Position player at sample point `A` with direct line-of-sight to origin `S`.
  2. Observe player health during pre-front interval (head start active or front `F` upstream of `A`).
  3. Observe player health as front `F` propagates downstream and passes point `A`.
- **Expected Behavior:**
  - Pre-front: Zero damage ($\Delta\text{HP} = 0$), despite unobstructed line-of-sight to origin `S`.
  - Post-front: Once front `F` reaches `A`, player transitions to exposed; health decreases over time ($\Delta\text{HP} < 0$) while alive.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-02: Opaque Wall Cover
- **Fixture:** Figure 1 (Barrier `W`, candidate cover point `H`, exposed control point `A`).
- **Steps:**
  1. Confirm front `F` has expanded past obstacle `W`, verifying active damage at control point `A`.
  2. Position player at candidate cover point `H` behind opaque barrier `W`.
  3. Establish that protection at `H` is attributable to the intended wall/cover model, not merely a front that has not arrived; capture the coverage/protection evidence or matched control.
  4. Record health and classification over a nonlethal interval at `H`.
- **Expected Behavior:**
  - The intended opaque wall provides protection at `H`, while the exposed control at `A` takes damage and spread continues.
  - Health at the protected point remains unchanged. If cover cannot be distinguished from an unreached/zero-damage point, the attempted cover assertion is `Blocked`, not proved by unchanged health alone.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-03: Enter Shade (Transit Exposed to Covered)
- **Fixture:** Figure 1 (Transit path `A -> H` behind barrier `W`).
- **Steps:**
  1. Confirm positive damage at reached, exposed `A` and establish intended protection at `H` using CV-02's evidence requirements.
  2. Move `A -> H` while health remains positive; record coverage/cover transitions and health at the actual gameplay update boundaries.
  3. Observe a full interval confirmed inside shade, separately from the interval spent crossing its edge.
- **Expected Behavior:**
  - Damage occurs in reached, unshaded `A`; it stops during confirmed fully protected intervals at `H`, while the front continues spreading.
  - Do not call a mixed transition interval a failure without agreed timing/sample rules; boundary alignment is evaluated in CV-09.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-04: Leave Shade (Transit Covered to Exposed)
- **Fixture:** Figure 1 (Transit path `H -> A` exiting from barrier `W`).
- **Steps:**
  1. Establish `H` as intended, observed protection and verify the destination `A` is already reached and damaging.
  2. Move `H -> A` without changing other configuration; keep health positive.
  3. Record the cover transition and a full reached/unshaded observation interval at `A`.
- **Expected Behavior:**
  - Health is unchanged while fully protected at `H`, then decreases once the actual exposed interval at reached `A` begins.
  - Judge onset against the agreed damage cadence/boundary rule, not an invented rate or rounded HUD sample. A destination ahead of the front cannot demonstrate resumed exposure.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-05: Look Away (Camera Orientation vs World-Space Cover)
- **Fixture:** Figure 1, fixed sample point `A` with the local front already arrived for both camera orientations.
- **Steps:**
  1. At reached, unshaded `A`, record positive damage while facing `S`.
  2. Turn the camera away using confirmed look controls; keep the actual player sample position and cover conditions unchanged.
  3. Record exposure and health while facing away. If look input moves the sample point or changes cover, use a controlled variant or record that comparison `Blocked`.
- **Expected Behavior:**
  - Looking away provides no protection: the reached, unshaded player still takes damage.
  - Do not infer a camera bug from unequal rates unless all damage-relevant inputs and the rate model are agreed and matched; stopping damage solely because the sun is off-screen is a violation.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-06: Closed Door Barrier (Policy Decision)
- **Fixture:** Figure 1 (Barrier slot `W` replaced by closed door asset; candidate cover point `H`).
- **Team Policy Ruling:** Unresolved (Pending Team Decision: does a closed interior door provide full cover, attenuation, or zero protection?)
- **Steps:**
  1. Record the agreed door policy and barrier configuration. Do not decide protection from the door's appearance alone.
  2. Repeat the matched barrier/control setup from CV-02 with the closed door at `W`, keeping health positive.
  3. Record actual coverage/protection and health under that policy.
- **Expected Behavior:**
  - A blocking policy means intended protection and no damage; a transmitting policy means damage at a reached, unshaded point.
  - If the team explicitly chooses attenuation, record its expected rule/parameters first and compare matched controls. This is not an added baseline feature. An attempted variant without a ruling or sufficient evidence is `Blocked`.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-07: Glass Barrier (Policy Decision)
- **Fixture:** Figure 1 (Barrier slot `W` replaced by glass panel asset; candidate cover point `H`).
- **Team Policy Ruling:** Unresolved (Pending Team Decision: does architectural glass permit full exposure, attenuate damage, or block exposure?)
- **Steps:**
  1. Record the agreed glass policy and exposure configuration; material transparency and physical collision alone do not establish it.
  2. Repeat CV-02's matched setup/control with glass replacing `W`; maintain positive health.
  3. Record coverage/protection and health, distinguishing protection from a merely unreached point.
- **Expected Behavior:**
  - A transmitting policy permits damage at a reached, unshaded point; a blocking policy provides protection with unchanged health.
  - If attenuation is explicitly agreed, record its expected rule and compare matched controls; do not invent a damage fraction. Missing policy/evidence during execution means `Blocked`.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-08: Front Propagation Around Corner
- **Fixture:** Figure 2 (`S`, bend `B`, downstream `P`, front `F`). Confirm the propagation rule and that `P` is intended to be unshaded once the local front arrives. Direct `S -> P` occlusion alone is not that ruling.
- **Steps:**
  1. Place the player at `P`; record the sample point and intended cover/propagation configuration.
  2. Record front position and health while `F` is upstream of `B`, during the turn, and on arrival at `P`.
  3. Compare authoritative local coverage, cover state, visual progression, and health over survivable intervals.
- **Expected Behavior:**
  - No damage before the local front reaches `P`, including while the front is still on the upstream arm.
  - Under the agreed model, reached/unshaded `P` takes damage even though a direct line to central origin `S` is blocked by the inner wall.
  - Do not accept central-origin occlusion as permanent protection without a cover ruling. If turn behavior, intended shade at `P`, or coverage evidence is unresolved, record the attempted variant `Blocked` rather than assume a model.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending

### CV-09: Visible Boundary vs Damage Alignment
- **Fixture:** Figure 1 near `A`, after zero, with positive health and a confirmed positive-damage, unshaded control. Use the host's defined player sample and visual/authoritative edge.
- **Steps:**
  1. Record the agreed edge reference, sample geometry, damage cadence, inclusion rule, and alignment tolerance. Bloom/glow is not automatically the edge.
  2. Capture matched samples just ahead of and behind the edge; verify that neither observation is explained by shade, the head start, zero health, or a zero-damage rate.
  3. Associate visual evidence with authoritative update/coverage and health records. Evaluate a full relevant interval on each side separately from exact contact/crossing.
- **Expected Behavior:**
  - The ahead-of-front sample stays undamaged; the reached, unshaded trailing sample takes damage under the recorded configuration.
  - Visual arrival and damage onset agree within the **agreed** sample/cadence/tolerance rule. Exact touching is judged only after its inclusion rule is confirmed.
  - Missing temporal association, coverage proof, or a required boundary ruling makes the attempted alignment claim `Blocked`. A nearby glow or a video frame alone is not proof; no result is claimed here.
- **Observed Behavior:** Pending (Host execution required)
- **Result:** Pending
- **Evidence:** Pending
