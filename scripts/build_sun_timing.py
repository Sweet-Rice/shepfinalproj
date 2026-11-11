"""Rebuild data/sun-timing.csv from the parameter and checkpoint CSVs.

Usage:
    python3 scripts/build_sun_timing.py          # rewrite data/sun-timing.csv
    python3 scripts/build_sun_timing.py --check  # fail if the CSV is out of date

Standard library only. See docs/sun-timing.md for the model.
"""

import csv
import io
import sys
from pathlib import Path

DATA = Path(__file__).resolve().parent.parent / "data"
PARAMETERS = DATA / "sun-timing-parameters.csv"
CHECKPOINTS = DATA / "sun-timing-checkpoints.csv"
OUTPUT = DATA / "sun-timing.csv"

COLUMNS = [
    "spread_model",
    "head_start_s",
    "sun_front_speed_mps",
    "checkpoint_id",
    "checkpoint_name",
    "player_route_m",
    "sun_path_m",
    "player_arrival_s",
    "sun_arrival_s",
    "gap_s",
    "status",
    "walking_pace_mps",
    "source_type",
    "travel_time_source",
]


def read_csv(path):
    with path.open(newline="", encoding="utf-8") as handle:
        return list(csv.DictReader(handle))


def fmt(value):
    return f"{value:.1f}"


def build():
    params = {row["parameter"]: row for row in read_csv(PARAMETERS)}
    checkpoints = read_csv(CHECKPOINTS)

    pace = float(params["walking_pace"]["value"])
    pace_source = params["walking_pace"]["source_type"]
    target = float(params["spread_target_total"]["value"])
    calibration = float(params["spread_speed_calibration_head_start"]["value"])
    head_starts = [float(v) for v in params["head_start_options"]["value"].split(";")]

    references = [cp for cp in checkpoints if cp["spread_reference"] == "yes"]
    if len(references) != 1:
        sys.exit("Exactly one checkpoint must have spread_reference=yes")
    reference_m = float(references[0]["sun_path_m"])

    # Player arrival: walk each leg at `pace`, pausing `dwell_s` at each checkpoint
    # before leaving it.
    arrivals = []
    elapsed = 0.0
    previous_m = 0.0
    for cp in checkpoints:
        route_m = float(cp["player_route_m"])
        if route_m < previous_m:
            sys.exit(f"{cp['checkpoint_id']}: player_route_m must not decrease")
        elapsed += (route_m - previous_m) / pace
        arrivals.append(elapsed)
        elapsed += float(cp["dwell_s"])
        previous_m = route_m

    models = {
        # Front speed is re-solved per head start so the reference is always hit at the target.
        "fixed_60s_target": lambda h: reference_m / (target - h),
        # One front speed (solved for the calibration head start); the target moves with the head start.
        "fixed_speed": lambda h: reference_m / (target - calibration),
    }

    rows = []
    for model, speed_for in models.items():
        for head_start in head_starts:
            speed = speed_for(head_start)
            for cp, player_t in zip(checkpoints, arrivals):
                sun_t = head_start + float(cp["sun_path_m"]) / speed
                gap = sun_t - player_t
                source_types = {pace_source, cp["source_type"]}
                rows.append({
                    "spread_model": model,
                    "head_start_s": f"{head_start:g}",
                    "sun_front_speed_mps": f"{speed:.2f}",
                    "checkpoint_id": cp["checkpoint_id"],
                    "checkpoint_name": cp["checkpoint_name"],
                    "player_route_m": cp["player_route_m"],
                    "sun_path_m": cp["sun_path_m"],
                    "player_arrival_s": fmt(player_t),
                    "sun_arrival_s": fmt(sun_t),
                    "gap_s": fmt(gap),
                    "status": "ahead" if gap > 0 else "caught",
                    "walking_pace_mps": f"{pace:g}",
                    "source_type": source_types.pop() if len(source_types) == 1 else "mixed",
                    "travel_time_source": f"{cp['distance_source']} / walking_pace parameter",
                })

    buffer = io.StringIO()
    writer = csv.DictWriter(buffer, fieldnames=COLUMNS, lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return buffer.getvalue()


def main():
    text = build()
    if "--check" in sys.argv[1:]:
        current = OUTPUT.read_text(encoding="utf-8") if OUTPUT.exists() else ""
        if current != text:
            sys.exit(f"{OUTPUT} is out of date; run python3 scripts/build_sun_timing.py")
        print(f"{OUTPUT.name} is up to date")
        return
    OUTPUT.write_text(text, encoding="utf-8")
    print(f"Wrote {OUTPUT}")


if __name__ == "__main__":
    main()
