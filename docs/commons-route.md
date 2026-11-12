# Commons to classroom route worksheet

Audit **one route from the Commons to 1225 first**. Room 1221 is a nearby wrong-door candidate; 1360 is an optional later destination. Complete the survey through a campus walk **or annotated, approved teammate photos/footage** covering the route. A floor plan alone is not a completed survey.

## What the public sources establish

- The [official LSU tour map](https://www.lsu.edu/eng/images/selfguidedtourmp3sandimages/tour-final-version-1.jpg) places classrooms 1225 and 1221 together near the Student Leadership Incubator (1280) and Commons (1278). It places the System Integration Classroom (1360) beside the Human Factors Lab (1354).
- The [LSU Registrar](https://www.lsu.edu/registrar/student-services/our-controlled-rooms.php) lists 1225 and 1221 as General Purpose Classrooms. Room 1360 is a map-identified specialized classroom, not a listed Registrar GPC.
- The [self-guided tour](https://www.lsu.edu/eng/about/pftselfguidedtour.php) describes the Commons, Panera, and nearby Student Leadership Incubator. The [building guide](https://www.lsu.edu/eng/images/pft_floorplan_guide2_webupdated2021.pdf#page=2) provides another first-floor reference.

These sources support **candidate landmarks and room identities**, not visible door plaques, step-free thresholds, current access, or a verified turn sequence. The tour map has no compass key; do not invent compass directions from its layout.

## Roster contract

The [candidate roster](../data/classroom-candidates.csv) contains stable room IDs, display numbers, floor, source, route status, eligibility, a `nearest_landmark`, and a `landmark_clue`.

- `nearest_landmark` is a map-supported candidate reference, not a measured closest feature or a promise of visibility from the player's route.
- `landmark_clue` is a self-contained landmark association, not turn-by-turn instructions. All three clues still need a route/sign check.
- All candidates remain `route_status=needs_route_and_sign_check` and `wheel_eligible=false`. Verify each candidate's route and signage before promoting that candidate; one verified destination is enough for the initial roster milestone.
- This CSV is a **field-audit list, not a runtime importer or enforced selection gate**. The [current Unreal prototype](../unreal/README.md) already selects 1225 or 1221. The flags do not remove those prototype targets; reported automated reachability and field-audit readiness are different checks.

## Starting leads in the twin

These approximate walkthrough mesh bounds are search leads, **not verified door interaction coordinates or spawn points**. A room-floor center can be inside the room. Do not infer precise walking routes from these centers.

| Model lead | Approximate XY in Unreal cm | What to confirm |
| --- | --- | --- |
| Commons glass door mesh A | `(15124, -7594)` | Opens outdoors; clear indoor standing point |
| Commons glass door mesh B | `(15513, -7477)` | Alternate if A is obstructed or unsuitable |
| 1225 floor piece | `(13036, -7890)` | Corridor-facing door and signed room plaque |
| 1221 floor piece | `(12143, -8097)` | Signed nearby wrong-door candidate for 1225 |
| 1360 floor piece | `(16000, -4453)` | Corridor door beside Human Factors Lab 1354 |

## Existing engine reports, not a completed field survey

[PR #50](https://github.com/Sweet-Rice/shepfinalproj/pull/50) reports 15 automated checks and both walking runs winning with 100 HP:

| Destination | Reported automated walk time |
| --- | --- |
| 1225 | 22.5 seconds |
| 1221 | 23.6 seconds |

Both reported walks finish before the prototype's 25-second head start; they do not establish how a human navigates under sun pressure. [PR #53](https://github.com/Sweet-Rice/shepfinalproj/pull/53) reports restoring 30 signs, checking rendered signs, and repeating the walks in the package. These are author-reported results, not approved route footage annotated here. See the [build evidence ledger](implementation-status.md) for source revisions, scope, and the outstanding handoff.

## Route audit still needed

| Check | Candidate lead | Status / evidence needed |
| --- | --- | --- |
| Commons exterior door and indoor start | Door mesh A or B above | Pending: approved reference showing the outdoor opening, threshold, and clear indoor standing area |
| Commons-to-1225 turns and landmarks | Incubator / Commons classroom cluster | Pending: a continuous route record with major turns and visible landmarks annotated |
| 1225 identity and reachable door | Official map / Registrar | Pending: readable corridor plaque and door approach reference |
| Step-free route | Intended walking route | Pending: references covering thresholds, level changes, and any access barriers; no jumping required |
| Nearby wrong door | 1221 candidate | Pending: readable plaque and its relationship to the route; in-game rejection remains a separate gameplay check |
| Candidate shade/cover | Architectural recess or alcove along the route | Pending: marked reference; rendered shade behavior must be checked separately in the build |
| Human walking time | Same starting point and target door | Pending: observed walking time, without substituting the automated run |

For each observation, record the approved source and timestamp or photo label. Attach an original route sketch or an approved annotated reference that identifies the start, destination, turns, landmarks, and wrong door. Exclude identifiable bystanders and private information.

No approved route-reference set was found in the checked repository, PR discussions, or local project locations. [#19](https://github.com/Sweet-Rice/shepfinalproj/issues/19) remains open for that survey; [#20](https://github.com/Sweet-Rice/shepfinalproj/issues/20) still needs at least one verified destination before its roster acceptance criteria are met. Package/media handoff is tracked in [#17](https://github.com/Sweet-Rice/shepfinalproj/issues/17); reviewing actual gameplay readability is separate work under [#33](https://github.com/Sweet-Rice/shepfinalproj/issues/33).
