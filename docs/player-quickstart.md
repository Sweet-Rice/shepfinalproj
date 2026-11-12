# PFT Sun Run player quickstart

## 1. Launch the Windows playtest

1. Get the playtest ZIP from your team. In File Explorer, right-click it and choose **Extract All**.
2. Open the extracted folder and double-click **PFTFloor1Viewer.exe**. Keep the other folders alongside it. The game and PFT map are included.
3. If Windows reports a missing Microsoft Visual C++ runtime, run **vc_redist.x64.exe** under **Engine\Extras\Redist\en-us**, then try again.

The standalone build runs on 64-bit Windows. Building a new package requires Unreal and the team's walkthrough assets. [Developer packaging instructions](../unreal/README.md)

## 2. Controls

| Input | Action |
| --- | --- |
| Enter | Spin for a classroom or spin again after the round |
| WASD / Mouse | Move / look |
| Shift / Space | Sprint / jump |
| E | Check the classroom door you are facing |
| Esc | Pause or resume |
| R | Retry the same classroom after the round |
| Alt+F4 | Exit the game |

## 3. Gameplay Loop and Rules

*What If the Sun Was Inside PFT?* is a first-person wayfinding game set in the digital twin of Patrick F. Taylor Hall (PFT), designed for LSU freshmen learning the building layout.

- **Spawn & Objective:** You start just inside the designated Commons exterior door. A wheel assigns a first-floor classroom. Once it settles, the target room number stays visible, movement unlocks, and the head-start countdown begins.
- **Wayfinding:** Follow the room ranges and arrows on the overhead signs, then check the numbered classroom door.
- **Checking Rooms:** Approach a classroom door and use the build's confirmed door-check input.
  - If it is the **wrong door**, its room number is shown. Your objective does not change, and the timer and hazard continue without pause.
  - If it is the **correct target door** and you are still alive when checking it, you win. Simply entering a room is not a confirmed substitute for checking its door.
- **Countdown & Hazard:**
  - During the headstart countdown, **no hazard damage occurs**.
  - When the countdown reaches zero, the indoor sun spreads from the Commons into the corridors. **Reaching zero is not an automatic defeat.**
  - If caught outside shade after zero, your health drops over time.
  - Stepping into **shade/cover** stops incoming hazard damage. Shade does not pause the clock or stop the sun's spread, and health does not regenerate.
- **Outcomes & Retries:**
  - **Victory:** Checking the correct target door while alive wins, even after the countdown reaches zero.
  - **Defeat:** Depleting all health results in defeat.
  - **Same Room Retry:** Return to the Commons spawn with full health and the same target classroom; the countdown and sun spread reset for a fresh attempt.
  - **Spin Again:** Return to room selection with a clean reset. A new countdown begins when the wheel settles, not while it spins. The new selection may be the same classroom again.

## 4. Reporting Issues and Bugs

If you encounter unexpected behavior, visual defects, or navigation blockers, report them using our reusable bug report form:

- **Bug Report Template:** [`../.github/ISSUE_TEMPLATE/bug-report.md`](../.github/ISSUE_TEMPLATE/bug-report.md)

### How to Submit

- **After Merge into `main`:** Navigate to the repository's GitHub Issues tab, select **New Issue**, and choose the **Bug report** template.
- **Before Merge:** Open [`../.github/ISSUE_TEMPLATE/bug-report.md`](../.github/ISSUE_TEMPLATE/bug-report.md), copy the Markdown content below the YAML frontmatter (below the second `---` line), and paste it into a new blank issue on GitHub.
- **Markdown Format:** The template is fillable Markdown text, not an automated web form. Replace the bracketed placeholders with your observed details.

### Public Repository Privacy Rules

This repository is publicly visible. Follow these privacy rules when reporting issues:
- **Never include:** Student names, LSU IDs, personal class schedules, private server or download links, access credentials, or building asset files.
- **Allowed:** Anonymous hardware specs (CPU, GPU, RAM, OS version), display resolution, and relevant bug reproduction details.
- **Media:** Redacted screenshots or video clips showing only in-game behavior are welcome. Send unredacted logs or sensitive files only through approved internal team channels.
- **Testing Safety:** Never disable operating system security features, firewall settings, or antivirus to test a build.

For background on the player quickstart and reporting workflow, see [Issue #28](https://github.com/Sweet-Rice/shepfinalproj/issues/28).
