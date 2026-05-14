# 📊 Training Tracker — Qt Automotive HMI

> Single source of truth for the **Qt Automotive HMI Training** programme. Tracks the daily plan, per-trainee progress, practice tasks, learning material, and final-project groups all in one file.

This folder lives inside the main training repo so that the **modules** (the teaching content) and the **tracker** (who's done what) stay version-controlled together. Whenever someone clones the repo, they see both.

---

## What's in this folder

| File | Purpose |
| :--- | :--- |
| **`Qt_Automotive_HMI_Training_Plan.xlsx`** | The master Excel file — 6 sheets, described below |
| **`README.md`** | This file — explains how to read and update the tracker |

---

## The 6 sheets, explained

The Excel file has **six related sheets**. Read them in this order to understand the programme.

### 1. 🗂️ `Dashboard`

**The high-level view.** Open this first.

- One row per trainee
- Columns: # · Name · Department · Phase 1 % · Phase 2 % · Phase 3 % · Overall % · Status
- The **Status** column shows ✅ Completed / 🔄 In Progress / ⏳ Not Started
- Phase percentages are **auto-calculated** from the detailed tracker
- "Total Trainees", "Departments", "Total Topics" appear at the top as summary numbers

> **Audience:** managers, stakeholders, and anyone who wants a quick "where are we?" without reading the detail.

### 2. 📋 `TraineeProgressTracker`

**The detailed per-topic tracker.** Updated weekly.

- One row per trainee
- One column per **topic** (10 topics + 4 final-project stages = 14 status columns)
- Topics are grouped by phase:
  - **Phase 1 — Foundation** *(Week 1)*: Qt Creator IDE Navigation · Qt Widgets & Layouts · Signals & Slots (C++)
  - **Phase 2 — Intermediate Qt** *(Week 1)*: QPainter Custom Drawing · QTimer & Real-time Updates · File I/O & Data Logging · Qt StyleSheets (QSS)
  - **Phase 3 — Advanced Qt** *(Week 2)*: QtCharts (Bar, Line, Pie) · Custom Gauge / Speedometer · Full HMI Integration
  - **Final Project** *(Week 2)*: Project Planning & Design · Implementation · Testing & Review · Final Demo / Presentation

Each cell is one of:
- ✅ Completed — topic done, work reviewed
- 🔄 In Progress — actively being worked on this week
- ⏳ Not Started — not yet attempted

> **Audience:** the mentor (you) and each trainee for their own row.

### 3. 📅 `Qt_Training_Plan`

**The 10-day curriculum.** Doesn't change — read-only reference.

Columns: Phase · Day · Module · Qt Module · Level · Subtopics / Concepts · Hrs · Primary Resource · Alternate Resource

- Maps each topic to which day it should be taught
- Difficulty level (BEGINNER / INTERMEDIATE / ADVANCED)
- Time estimate per topic
- Primary resource = the corresponding folder in this Git repo
- Alternate resource = YouTube / Qt docs / blog link

> **Audience:** trainees planning their week; mentor confirming what should be taught when.

### 4. 🛠️ `Qt_Practice_Tasks`

**Hands-on exercises per topic.** What the trainee actually builds.

Columns: Sr.No · Day · Module · Task Title · Task Description · Reference

- Each row is one practical exercise tied to a specific module
- Trainees submit their solution as a PR on their personal branch
- Mentor reviews the PR before marking the topic ✅ Completed in the tracker

> **Audience:** trainees as their daily to-do list; mentor as the rubric for reviewing.

### 5. 📚 `Qt_LearningMaterial`

**Reference library.** Curated links — Qt docs, tutorials, videos, books.

Columns: Resource Name · Type · Link

Types include Documentation, Video Tutorial, Course, Book, Blog.

> **Audience:** anyone who's stuck and needs an external explainer.

### 6. 🚗 `Qt_FinalProject`

**Final project group assignments.** One row per group of 3–4 trainees.

Columns: Sr.No · Group · Members · Variant Focus · Description · Peer Review · Peer Status · Final Status · GitHub Repo

Four pre-defined variants groups can pick from:

- **Group 1 — ICE Vehicle Dashboard** — petrol/diesel car, RPM gauge, fuel + temperature, gear indicator
- **Group 2 — EV Dashboard** — battery SoC, regen indicator, range, motor temp
- **Group 3 — Hybrid Dashboard** — combined ICE + EV, power-flow visualisation
- **Group 4 — Performance / Sport** — lap timer, 0–100 tracker, sport-mode styling

> **Audience:** trainees during the final project phase; mentor during demo review.

---

## How to read the dashboard at a glance

| What you see | What it means | What to do |
| :--- | :--- | :--- |
| Trainee at **100% overall, ✅ Completed** | Done with the programme | Nothing — celebrate 🎉 |
| Trainee at **50–80%, 🔄 In Progress** | On track | Continue weekly check-ins |
| Trainee at **< 50% by mid-Week 2** | Falling behind | 1-on-1 conversation — find blockers |
| Trainee at **0% after Week 1** | Hasn't started | Urgent — talk to them today |

---

## Update process

### Who updates what

| Sheet | Updated by | When |
| :--- | :--- | :--- |
| `Dashboard` | **Auto-calculated** (don't edit cells) | Updates when `TraineeProgressTracker` changes |
| `TraineeProgressTracker` | **Mentor** (you) | Weekly, after Friday stand-up |
| `Qt_Training_Plan` | **Mentor** | Once at programme start, then locked |
| `Qt_Practice_Tasks` | **Mentor** | Once at programme start, may evolve |
| `Qt_LearningMaterial` | **Anyone** via PR | Whenever a useful link is found |
| `Qt_FinalProject` | **Mentor** | After group selection meeting in Week 2 |

### How often

- **Weekly** — Friday EOD, mentor updates `TraineeProgressTracker` based on the week's reviews
- **End of each phase** — verify the `Dashboard` percentages look right
- **Programme end** — archive the file with the batch name (e.g. `Batch_1_Qt_Automotive_HMI_Training_Plan_FINAL.xlsx`)

### Step-by-step weekly update

1. Open `Qt_Automotive_HMI_Training_Plan.xlsx` in Excel
2. Go to `TraineeProgressTracker` sheet
3. For each trainee who finished a topic this week, change ⏳ → ✅ (or 🔄 if they're partway)
4. Check `Dashboard` — percentages should have updated automatically
5. Save the file
6. Commit and push to Git:

```powershell
git add tracking/Qt_Automotive_HMI_Training_Plan.xlsx
git commit -m "Update progress tracker — Week N"
git push origin main
```

That's it. Five minutes per week.

---

## Why we keep this in Git (not Confluence / OneDrive)

| Benefit | Why it matters |
| :--- | :--- |
| **One repo for everything** | Modules, code, and tracker live together — no broken links between systems |
| **Version history** | `git log` shows exactly when each trainee finished each topic |
| **Auditable** | Every change has an author, a timestamp, and a commit message |
| **Offline-capable** | Works without internet (unlike Confluence) |
| **No extra tooling** | No Confluence licence, no shared-drive permission requests |
| **Survives mentor changes** | If someone takes over the programme, the entire history is in the repo |

**Trade-offs to know about:**

- Excel files don't `git diff` nicely — you see "binary file changed" rather than which cell changed. That's fine for tracking; if you need cell-level history, use the Excel file's built-in versioning by saving with a date suffix periodically.
- Only one person can edit at a time. For a single-mentor programme this is a non-issue.

---

## Common questions

### Can I just look without downloading?

GitHub shows a **preview of `.xlsx` files** if you click the filename in the web UI — readable, though formulas and colour formatting may look slightly different from the desktop Excel.

### What if I accidentally break the dashboard formulas?

Revert the file from Git:

```powershell
git checkout HEAD -- tracking/Qt_Automotive_HMI_Training_Plan.xlsx
```

This restores the last committed version.

### Can trainees update their own rows?

Yes — open a PR on a personal branch:

```powershell
git checkout -b update-my-progress/<your-name>
# edit the file
git add tracking/Qt_Automotive_HMI_Training_Plan.xlsx
git commit -m "Mark Module 02 complete"
git push origin update-my-progress/<your-name>
```

…then open a PR for the mentor to merge. This is optional — for small teams, mentor-owned updates are simpler.

### What's the next step after a batch finishes?

1. Rename the file to `Batch_1_FINAL.xlsx` and copy it to an `archive/` subfolder
2. Create a fresh copy for Batch 2 starting from the same `Qt_Training_Plan` and `Qt_Practice_Tasks` sheets
3. Reset all status cells in `TraineeProgressTracker` to ⏳ Not Started
4. Commit both with a message like `"Archive Batch 1, start Batch 2"`

---

← [Back to programme overview](../README.md) · [Module navigation](../NAVIGATION.md)
