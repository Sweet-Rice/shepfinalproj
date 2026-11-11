# Commons to classroom route worksheet

Start by checking the Commons exterior doors and room **1225**. LSU's first-floor plan marks the Commons and identifies 1225 as a classroom. The registrar also lists 1225 and 1221. The [candidate roster](../data/classroom-candidates.csv) gives the route tester two backups. [LSU first-floor building guide](https://www.lsu.edu/eng/images/pft_floorplan_guide2_webupdated2021.pdf#page=2) · [LSU registrar classroom list](https://lsu.glb.lsu.edu/registrar/student-services/our-controlled-rooms.php#patrick-f-taylor-pft)

## What to inspect in the twin

The local first-floor walkthrough export includes two instances of a Commons glass door mesh near the `SM_TheCommonsFloorPieceA2` floor piece. These centers are approximate mesh bounds in Unreal centimeters. Use them to find the door group in the twin. Pick a clear standing point on the indoor side only after checking where the doors lead and testing floor and capsule collision.

| Model lead | Approximate XY in cm | What to confirm |
| --- | --- | --- |
| Commons glass door mesh A | `(15124, -7594)` | Whether it opens outdoors and where a player can stand clear of the doorway |
| Commons glass door mesh B | `(15513, -7477)` | Same check if the first door is obstructed or unsuitable |
| 1225 floor piece | `(13036, -7890)` | Find the actual corridor-facing door and signed room number. The mesh center is inside the modeled room. |
| 1221 floor piece | `(12143, -8097)` | Verify signage and whether it makes a useful nearby wrong-door check for the 1225 route |
| 1360 floor piece | `(16000, -4453)` | Check as an alternate teaching-room route. The building guide identifies it as the System Integration Classroom. |

These leads come from the separate walkthrough's static mesh labels and bounds. A floor-piece label can help locate a room. The field check establishes the door location, current signage, and whether the route is usable.

## Route check for the first playtest

1. Start just inside a verified Commons exterior door, facing into the Commons. Confirm the starting point is step-free and the wheel won't obscure the first landmark.
2. Walk the public first-floor corridors to the signed 1225 door. Record each turn and the landmark a newcomer can see there. If the route requires stairs, a closed shortcut, or a jump in the twin, record the blocker and test an alternate route.
3. Record a reachable point outside the correct door for the room check. Find a different signed door along the same route and record its number for the wrong-door case.
4. Note a recess or covered area where the level designer could place shade. Confirm shelter behavior in the game after the hazard is implemented.
5. Measure walking and sprint times from the spawn to the door check in the game. Use those measurements to tune the 20–30 second head start and the roughly 60-second spread target.

| Field to fill after verification | Result |
| --- | --- |
| Confirmed exterior door and indoor spawn | Pending |
| Room sign and reachable door check | Pending |
| Step-free route and visible landmarks | Pending |
| Nearby wrong door and candidate shade | Pending |
| Walk time and sprint time in the game | Pending |

Keep [the route audit](https://github.com/Sweet-Rice/shepfinalproj/issues/19) open until the route is checked. The [room roster task](https://github.com/Sweet-Rice/shepfinalproj/issues/20) can then promote verified routes to wheel eligibility.
