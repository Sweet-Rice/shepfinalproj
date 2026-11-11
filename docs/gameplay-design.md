# Gameplay Design: Commons Wheel & Expanding Sun

## Overview & Learning Goal

*PFT Sun Run* is a first-person wayfinding game designed to help freshmen learn navigation across Patrick F. Taylor Hall (PFT) at LSU. The player begins in the Commons, receives an eligible first-floor classroom via a selection wheel, and uses room signs and landmarks to find it before the expanding indoor sun depletes their health.

Authoritative source precedence: the local design brief (*PFT Sun Run Game Design Brief.docx*) establishes the primary gameplay loop; GitHub [Issue #18](https://github.com/Sweet-Rice/shepfinalproj/issues/18) establishes the initial timing baseline; and Issues [#25](https://github.com/Sweet-Rice/shepfinalproj/issues/25), [#29](https://github.com/Sweet-Rice/shepfinalproj/issues/29), [#30](https://github.com/Sweet-Rice/shepfinalproj/issues/30), and [#31](https://github.com/Sweet-Rice/shepfinalproj/issues/31) clarify timing, spawn, and verification rules.

**Status:** Intended design and initial tuning targets, not demonstrated gameplay. No Unreal build was run for this document. Only verified, reachable first-floor rooms should be eligible; no room numbers or surveyed coordinates are established here.

## Core Round Lifecycle & Intended Rules

1. **Spawn**: The player spawns grounded just inside a Commons exterior door leading outside. Routes require standard walking pace without jumping.
2. **Wheel Selection & Handoff**: The room wheel selects one eligible first-floor classroom. When the wheel settles, handoff occurs: player movement unlocks, the initial countdown begins, and the assigned room number appears persistently on the HUD.
3. **Head Start Phase**: The initial countdown provides a safe exploration window. **Strictly no hazard damage occurs during the head start.**
4. **Hazard Expansion**: When the countdown reaches zero, the sun begins expanding outward from the Commons through corridors. Once the front reaches an unshaded player, damage accumulates over time. A player can be caught and damaged **before the roughly 60-second spread target**, but never during the head start.
5. **Shade & Shelter**: Stepping into shade immediately stops damage accumulation while the sun continues spreading through exposed areas.
6. **Classroom Navigation & Feedback**: Players navigate via hall signs and landmarks. Checking a wrong door displays its actual room number and leaves the assigned objective unchanged. The run, countdown, and hazard continue; the wrong-door check is not a pause or a separate damage penalty.
7. **Victory & Same-Update Ordering**: Checking the assigned classroom door while alive triggers an immediate win (`Won`), even after the countdown reaches zero. Evaluate a valid classroom check before hazard damage in the same update: if the player is still alive when that update begins, a potentially lethal hit in that update does not override the win. An already-lost run cannot win afterward.
8. **Defeat & Round Reset**: Depleting health to zero transitions to `Lost`. Terminal outcomes cannot be reversed by later callbacks. After a loss, the player may select:
   - **Retry Same Room**: Keep the assigned classroom and reset the Commons spawn, full health, countdown, hazard extent, and HUD. Begin a fresh head start when control returns; no new room spin is required.
   - **Spin Again**: Return to the wheel for a new assignment and the same clean reset. Movement and the new countdown begin at wheel handoff, not while it is spinning. A new spin is not guaranteed to select a different room.

## Phased Timing & Tuning Targets

The round uses a phased progression measured from wheel handoff. The ~60-second value represents a pacing target for hazard reach across exposed corridors, not an immunity window or an additional timer added after zero.

| Phase / Milestone | Target Timing (from Wheel Handoff) | Intended Gameplay Rules & Behavior |
| --- | --- | --- |
| **Wheel Handoff** | `t = 0 s` | Wheel settles; movement unlocks; countdown begins; persistent room HUD active. |
| **Head Start Phase** | `25 s` default (`20–30 s` tuning range) | Safe navigation window; reading signs; **zero hazard damage** permitted. |
| **Hazard Ingress Start** | At countdown `0`; `t = 25 s` with the default | The front starts spreading from the Commons. Damage begins only when the hazard reaches an unshaded player, not everywhere at once. |
| **Exposed Corridor Reach** | `~60 s` total (`~35 s` after a default `25 s` head start) | Aim for most exposed corridors to be reached. This is a pacing target, not an automatic defeat or a second 60-second countdown. |
| **Late Win Window** | After countdown `0`, for as long as health remains positive | Shade stops damage, not time or expansion; the player can still check the correct classroom door and win. |

## First Integration Scope

Implementation proceeds in three verified milestones without premature complexity:

1. **Mechanical Prototype**: A simple sandbox map to verify wheel handoff, countdown timer, hazard damage application, and shade protection.
2. **First PFT Route**: Place the grounded Commons exterior-door spawn and one verified walking route to a real first-floor classroom in PFT, including at least one verified shade/cover point.
3. **Route Expansion**: Introduce two additional verified first-floor classrooms for playtesting and route variation.

## Unresolved Compatibility & Architecture Decisions

The following items are pending team adjudication and must not be assumed as settled baseline:

- **Damage Model Compatibility**: Decide whether the earlier distance-based damage rate is retained, adapted, or replaced once the spreading front reaches the player. Reaching the front is the damage gate; an unobstructed view of a fixed sun alone must not cause head-start damage or damage ahead of that front.
- **Cover & Occlusion Geometry**: Shade must stop damage while expansion continues. Decide how the front follows corridors and turns, how shade is detected, and whether glass, open/closed doors, or small props block exposure. Do not assume the earlier single trace to a central sun correctly models an expanding front.
- **Classroom Arrival Detection**: The brief's player action is to check the correct door. Confirm the actual input, interaction range, and whether door queries or existing room volumes support that check. Automatic victory on mere room entry is not an agreed replacement.
- **Tuning Constants**: Damage rate, maximum health, and spread speed require team confirmation and route testing. Do not treat the timing targets above as measured building or in-game results.

## Reference Links

- [Issue #18: [No Unreal] Document the Commons wheel and expanding sun design](https://github.com/Sweet-Rice/shepfinalproj/issues/18)
- [Issue #25: [No Unreal] Write reproducible test cases for the round lifecycle](https://github.com/Sweet-Rice/shepfinalproj/issues/25)
- [Issue #29: [Unreal] Wire the room wheel into the round and countdown](https://github.com/Sweet-Rice/shepfinalproj/issues/29)
- [Issue #30: [Unreal] Place and verify the Commons spawn and first room in PFT](https://github.com/Sweet-Rice/shepfinalproj/issues/30)
- [Issue #31: [Unreal] Integrate the expanding sun and cover on the first PFT route](https://github.com/Sweet-Rice/shepfinalproj/issues/31)
- [Team Project Overview](../README.md)
