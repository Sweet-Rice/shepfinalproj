# Unreal playtest results

Tested November 12, 2026 on the curated `/Game/Maps/PFTWalkthrough` map. The shared Wayfinding Windows build contains the source from [57ef6b1](https://github.com/Sweet-Rice/shepfinalproj/commit/57ef6b11ed265c442e54f8c65a9135df003183b0). The timing tests below used a newly compiled Win64 Development executable with the head-start override from this change and the same cooked map and content.

## Wheel, route, and hazard

| Head start | Room 1225 | Room 1221 | Lifecycle |
| --- | --- | --- | --- |
| 20 seconds | 22.5 s, 100 HP | 23.6 s, 100 HP | Pass, headless |
| 25 seconds (default) | 22.4 s, 100 HP | 23.4 s, 100 HP | Pass, rendered |
| 30 seconds | 22.5 s, 100 HP | 23.6 s, 100 HP | Pass, headless |

- The wheel locks walking and jumping, ignores a second spin request while spinning, and preserves the drawn room through handoff. Movement unlocks at the start of the countdown. The HUD keeps the drawn room visible during the round.
- The grounded Commons spawn is at `(15124, -7300, 100)` cm, about 3 m inside the modeled glass exterior door beside Panera. Retry returned to that point. Both walking routes passed without jumping or encountering a collision blocker. At the other classroom door, the game reported its room number and kept the round active. These observations describe the game map.
- The front starts at zero on the countdown and reaches 32 m from the spawn 60 s after handoff. At the default timing, the modeled front reaches the two tested classroom approaches at about 55.5–57.3 s. The regression checks zero damage during the head start, damage on arrival at an exposed point, no damage ahead of the front, continued spread while sheltered, and resumed damage upon leaving shade. Shelter is the marked floor zone near the drinking fountains. The route to it passed the same floor and capsule checks.
- The correct door won even in a post-countdown update that would have depleted the last HP. Remaining exposed reached 0 HP and lost. Same-room retry reset health and time. Spin again reset the round and opened a new draw.

## Windows handoff

- Tested on Windows 11 with a Ryzen 7 5800HS, RTX 3060 Laptop GPU selected by Unreal, and 16 GB RAM. Win64 Development, DirectX 11, 1280 × 720 windowed, packaged walkthrough graphics settings.
- The rendered verification run logged 3.47 s for Unreal engine initialization. The CSV profiler recorded a 16.5 ms median and 22.5 ms 95th-percentile frame time across 480 sampled frames of the first classroom walk. These are measurements from this machine.
- See the [player quickstart](../player-quickstart.md) for download handoff and controls. The Windows ZIP, editable project, and annotated gameplay recording are shared through the private team release. The recording uses scripted positioning and timer jumps for the hazard sequence, followed by a walking route.
