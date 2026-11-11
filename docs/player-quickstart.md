# Player Quickstart Guide

This guide describes how to run and play a shared build of *What If the Sun Was Inside PFT?* as a playtester or teammate without needing Unreal Engine installed.

**Draft for shared-build testing:** No packaged build, launch command, input binding, or gameplay behavior has been verified here. Ask the build owner to confirm the handoff details below before testing. Bold action names describe intended actions, not confirmed button labels.

## 1. Obtaining and Launching the Build

1. **Obtain the build package:** Request the packaged build archive, revision identifier, and control card directly from the build owner via team channels.
2. **Extract the package:** Unpack the entire ZIP archive to a local folder (avoid OneDrive or cloud-synced folders). Do not run directly from inside the compressed archive.
3. **Launch the game:** Run only the confirmed executable file provided by the build owner. You do not need to install Unreal Engine, open its editor, or modify project source to play a packaged build.

### Build Handoff Status

| Property | Status / Confirmed Value | Notes |
| :--- | :--- | :--- |
| **Build Revision / Commit** | *Pending build confirmation* | Specified by build owner with shared archive |
| **Target Platform** | *Pending build confirmation* | Windows 64-bit standalone package planned |
| **Executable Name** | *Pending build confirmation* | Launch only the binary confirmed by owner |
| **Control Scheme** | *Pending build confirmation* | Keybindings awaiting input mapping confirmation |

## 2. Controls and Actions

Input keybindings have not been finalized or verified against a packaged build. Do not assume standard keyboard/mouse mappings until confirmed on the build control card.

| Intended Action | Action / Label to Look For (Unconfirmed) | Confirmed Key / Input |
| :--- | :--- | :--- |
| Start assignment | Spin Wheel | *Pending build confirmation* |
| Move character | Movement / Locomotion | *Pending build confirmation* |
| Look / Aim camera | Camera Look | *Pending build confirmation* |
| Inspect classroom door | Check Door / Interact | *Pending build confirmation* |
| Restart same room | Same Room Retry | *Pending build confirmation* |
| Roll new room target | Spin Again | *Pending build confirmation* |

## 3. Gameplay Loop and Rules

*What If the Sun Was Inside PFT?* is a first-person wayfinding game set in the digital twin of Patrick F. Taylor Hall (PFT), designed for LSU freshmen learning the building layout.

- **Spawn & Objective:** You start just inside the designated Commons exterior door. A wheel assigns a first-floor classroom. Once it settles, the target room number stays visible, movement unlocks, and the head-start countdown begins.
- **Wayfinding:** Walk toward your assigned classroom using room signs and landmarks.
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
