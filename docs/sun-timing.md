# Sun Timing Worksheet

A small CSV worksheet for tuning the head-start countdown and the sun spread ([Issue #21](https://github.com/Sweet-Rice/shepfinalproj/issues/21)). It compares 20, 25 and 30 second head starts on one Commons → first-floor classroom route. At each checkpoint it shows when the player arrives, when the sun front arrives and the gap between them.

**Status: every number here is an estimate.** The route survey ([Issue #19](https://github.com/Sweet-Rice/shepfinalproj/issues/19)) is not done yet. The checkpoint names and distances are placeholders, and the in-game walk speed has not been measured. Replace them as real data comes in. See [Updating the worksheet](#updating-the-worksheet).

## Files

| File | What it holds | Edit by hand? |
| --- | --- | --- |
| [`data/sun-timing-parameters.csv`](../data/sun-timing-parameters.csv) | Walking pace, head-start options, the 60 s spread target, and where each value comes from. | Yes |
| [`data/sun-timing-checkpoints.csv`](../data/sun-timing-checkpoints.csv) | Named route checkpoints with player distance, sun-path distance, pause time, and source. | Yes |
| [`data/sun-timing.csv`](../data/sun-timing.csv) | Computed timings for every spread model × head start × checkpoint. | No, it is generated |
| [`scripts/build_sun_timing.py`](../scripts/build_sun_timing.py) | Rebuilds `sun-timing.csv` from the two input files (standard-library Python). | Only to change the model |

All three CSVs open in Excel, Google Sheets or LibreOffice. To explore, filter `sun-timing.csv` on `spread_model` and `head_start_s`.

## Model

All times are elapsed seconds from **wheel handoff** (`t = 0`), when movement unlocks and the countdown starts ([Phased Timing & Tuning Targets](gameplay-design.md#phased-timing--tuning-targets)).

**Player.** The player leaves the spawn at `t = 0` and walks each leg at `walking_pace`. They pause for `dwell_s` at a checkpoint (to read signs or pick a turn) before leaving it.

```text
player_arrival[k] = sum over legs up to k of (leg length / walking_pace) + sum of dwell_s at earlier checkpoints
```

**Sun.** The sun front is a single front that starts at the Commons sun origin when the countdown hits zero (`t = head_start`). It moves at a constant speed along the corridors that the route follows. `sun_path_m` is the corridor distance from the sun origin to each checkpoint. It is not a straight line, because the front follows corridors and turns.

```text
sun_arrival[k] = head_start + sun_path_m[k] / sun_front_speed
gap_s[k]       = sun_arrival[k] - player_arrival[k]     (positive = player is ahead; negative = player was caught)
```

The front speed comes from the ~60 s target in [the gameplay design](gameplay-design.md#phased-timing--tuning-targets). The checkpoint marked `spread_reference = yes` stands for "the most exposed corridors". Two variants are computed so you can see the trade-off:

| `spread_model` | Front speed | Effect of a longer head start |
| --- | --- | --- |
| `fixed_60s_target` | `reference sun_path_m / (60 - head_start)`, solved again for each head start | The front still reaches the reference at 60 s, so it moves faster and gives back most of the extra head start. |
| `fixed_speed` | `reference sun_path_m / (60 - 25)`, the speed that hits 60 s with the 25 s default | The front's arrival shifts by the head-start difference (reference reached at 55 / 60 / 65 s). |

Everything else in the round, including damage, shade and health, is outside this sheet. "Caught" only means that the front has passed the player. Under the design, a player who is caught takes damage unless they are in shade, and can still win while health remains.

## Current estimates

The placeholder route is ~115 m with 5 s of pauses. At the real-world pace of 1.34 m/s, the player reaches the door at **90.8 s**.

| Model | Head start | Front speed | Front overtakes player | Exposure before door (no shade) |
| --- | --- | --- | --- | --- |
| `fixed_60s_target` | 20 s | 2.75 m/s | ~44 s, ~57 m along route | ~47 s |
| `fixed_60s_target` | 25 s | 3.14 m/s | ~48 s, ~61 m along route | ~43 s |
| `fixed_60s_target` | 30 s | 3.67 m/s | ~50 s, ~65 m along route | ~40 s |
| `fixed_speed` | 20 s | 3.14 m/s | ~39 s, ~49 m along route | ~52 s |
| `fixed_speed` | 30 s | 3.14 m/s | ~56 s, ~73 m along route | ~35 s |

The overtake times come from the same model at 0.01 s steps. They are not in the CSV, which only lists checkpoints.

What this suggests, with all inputs still estimates:

- **At a real walking pace, the head start barely matters for a ~115 m route.** With a fixed 60 s target, going from 20 to 30 s moves the catch point by about 6 s, because the front speeds up to compensate. Every variant catches the player around the main corridor (between CP2 and CP3).
- **For the player to beat the front to the door** (25 s default, `fixed_60s_target`), they would need to average about 1.9 m/s, or the route would need to be about 55 m long (with the same 5 s of pauses). The other options are to give up the 60 s target, or to plan on shade along the route so the player survives the catch.
- **In-game speed may change the picture completely.** Unreal's default walk speed is 6 m/s, about 4.5× real pace. At that speed every head start leaves a large margin. Measure the PFT pawn's actual speed before choosing a head start.

## `source_type` values

Every input row and every computed row has a `source_type`:

| Value | Meaning |
| --- | --- |
| `estimate` | A guess, a published typical value, or a design target. **Everything in this PR is `estimate`.** |
| `real-world measured` | Timed or measured in PFT, for example by the [#19](https://github.com/Sweet-Rice/shepfinalproj/issues/19) route survey. |
| `in-game measured` | Timed or measured in the Unreal build, with the build revision noted in `distance_source` or `source`. |

A computed row is `mixed` when its walking pace and checkpoint distance have different source types. Note what was measured in `travel_time_source`.

## Updating the worksheet

1. **When #19 lands:** rename the checkpoints in `sun-timing-checkpoints.csv` after the surveyed spawn, turns, exposed corridors and room. Fill in measured `player_route_m` and `sun_path_m`, set `placeholder` to `no`, change `source_type` to `real-world measured`, and cite the survey in `distance_source`. Keep exactly one row with `spread_reference = yes`.
2. **When the in-game walk speed is known:** fill in `in_game_walk_speed`. To time the game instead of real life, copy that value into `walking_pace` and set its `source_type` to `in-game measured`.
3. **To try other head starts:** edit `head_start_options` (semicolon-separated, each value below 60).
4. Regenerate and commit all the CSVs together:

   ```bash
   python3 scripts/build_sun_timing.py
   ```

   `python3 scripts/build_sun_timing.py --check` exits non-zero if `sun-timing.csv` no longer matches the inputs.

If a stopwatch time is easier to get than a distance, convert it to metres using the pace it was walked at. Record both in `notes`.
