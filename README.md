# Qt Automotive HMI Training

> A hands-on, project-driven training program that takes you from your first `Hello, Qt` window to a fully working **automotive HMI dashboard** — speedometer, RPM gauge, fuel & temperature progress bars, scrolling telemetry chart, CSV data logging, dark automotive theme.

![Qt](https://img.shields.io/badge/Qt-6.x-41CD52?logo=qt) ![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus) ![License](https://img.shields.io/badge/license-MIT-blue) ![Duration](https://img.shields.io/badge/duration-2%20weeks-orange)

---

## Table of Contents

- [What You'll Build](#what-youll-build)
- [Who This Is For](#who-this-is-for)
- [Prerequisites](#prerequisites)
- [Setup](#setup)
- [Learning Path](#learning-path)
- [How To Use This Repo](#how-to-use-this-repo)
- [Final Project](#final-project)
- [Contributing](#contributing)
- [License](#license)

---

## What You'll Build

By the end of the program you will have built — from scratch — a real-time Qt 6 automotive HMI that displays simulated vehicle telemetry. The dashboard combines every concept covered in the modules:

| Component                       | Module(s) used        |
| :------------------------------ | :-------------------- |
| Window + dock layout            | 02 (Widgets & Layouts) |
| Speedometer + RPM gauge         | 04 (QPainter), 09 (Custom Gauge) |
| Fuel & Temperature progress bars | 02, 07 (Stylesheets) |
| Live updates @ 10 Hz            | 03 (Signals & Slots), 05 (QTimer) |
| Scrolling speed line chart      | 08 (QtCharts) |
| Trip distance bar chart         | 08 (QtCharts) |
| CSV telemetry logging + replay  | 06 (File I/O) |
| Dark automotive theme           | 07 (Qt Stylesheets) |

A reference architecture and submission rubric live in [`final-project/`](./final-project/).

---

## Who This Is For

- C++ developers new to Qt who want a structured path into Qt 6
- Automotive / embedded engineers exploring Qt for HMI work
- Students or interns onboarding to a Qt-based product team
- Self-learners who prefer building over reading

You don't need any Qt experience to start. You **do** need basic C++ (classes, pointers, references) and to be comfortable in a terminal.

---

## Prerequisites

| Requirement     | Minimum             | Recommended         |
| :-------------- | :------------------ | :------------------ |
| C++ knowledge   | Classes, references | C++17 features      |
| OS              | Win 10 / Ubuntu 20.04 / macOS 11 | latest LTS |
| RAM             | 8 GB                | 16 GB               |
| Disk            | 15 GB free          | 30 GB free          |
| Compiler        | MSVC 2019 / GCC 9 / Clang 12 | latest |

Full install steps are in [`SETUP.md`](./SETUP.md).

---

## Setup

```bash
# 1. Clone this repo
git clone https://github.com/ManeParag/Qt-Automotive-HMI-Training.git
cd Qt-Automotive-HMI-Training

# 2. Install Qt 6 — see SETUP.md for OS-specific steps

# 3. Open any module's .pro file in Qt Creator and hit Ctrl+R to build & run
```

That's it. Each module is a standalone, runnable Qt project. No global dependencies, no external SDKs.

---

## Learning Path

The course is organized into **4 phases across 2 weeks**, with 10 modules building progressively into the final project.

### 🟦 Phase 1 — Qt Fundamentals *(Week 1)*

Build the foundation: the IDE, widgets, and the signal/slot mechanism that ties Qt apps together.

| #   | Module                                                                          | Level    | Time   |
| :-- | :------------------------------------------------------------------------------ | :------- | :----- |
| 01  | [Qt Creator IDE Navigation](./01-qt-creator-ide/)                               | Beginner | 1.5 hr |
| 02  | [Qt Widgets & Layouts](./02-qt-widgets-layouts/)                                | Beginner | 3 hr   |
| 03  | [Signals & Slots](./03-signals-and-slots/)                                      | Beginner | 2 hr   |

### 🟩 Phase 2 — Intermediate Qt *(Week 1 → Week 2)*

Move beyond static UIs: custom drawing, real-time timers, file I/O, and theming.

| #   | Module                                                                          | Level        | Time   |
| :-- | :------------------------------------------------------------------------------ | :----------- | :----- |
| 04  | [QPainter Custom Drawing](./04-qpainter-custom-drawing/)                        | Intermediate | 2 hr   |
| 05  | [QTimer & Real-time Updates](./05-qtimer-realtime-updates/)                     | Intermediate | 1.5 hr |
| 06  | [File I/O & Data Logging](./06-file-io-data-logging/)                           | Intermediate | 2 hr   |
| 07  | [Qt Stylesheets (QSS)](./07-qt-stylesheets/)                                    | Intermediate | 1.5 hr |

### 🟧 Phase 3 — Advanced Qt *(Week 2)*

Advanced visualization: charting, custom gauge widgets, the visually heavy parts of an HMI.

| #   | Module                                                                          | Level    | Time   |
| :-- | :------------------------------------------------------------------------------ | :------- | :----- |
| 08  | [QtCharts (Bar, Line, Pie)](./08-qtcharts/)                                     | Advanced | 2.5 hr |
| 09  | [Custom Gauge / Speedometer](./09-custom-gauge-speedometer/)                    | Advanced | 2.5 hr |

### 🟥 Final Project *(Week 2)*

| #   | Module                                                                          | Level   | Time     |
| :-- | :------------------------------------------------------------------------------ | :------ | :------- |
| 🚗  | [Full HMI Integration](./final-project/)                                        | Project | Full day |

---

## How To Use This Repo

Each module folder follows the same structure:

```
NN-module-name/
├── README.md          ← Theory, concepts, exercise, submission checklist
├── ProjectName/       ← Runnable Qt 6 starter project
│   ├── ProjectName.pro
│   ├── main.cpp
│   ├── mainwindow.h
│   ├── mainwindow.cpp
│   └── mainwindow.ui  (where applicable)
└── exercises/
    └── README.md      ← The "your turn" task you submit
```

**Suggested workflow per module:**

1. Read the module `README.md` start to finish (~10 min)
2. Open the starter project in Qt Creator, build & run it as-is
3. Read the source — every starter is heavily commented
4. Do the exercise in `exercises/` (~60–90 min)
5. Commit your solution on a branch named `module-NN/<your-name>`
6. Open a Pull Request against `main` for review

**For trainers / instructors:** the [Excel tracker](https://github.com/ManeParag) (separate repo) gives you the per-trainee progress view that pairs with this repo.

---

## Final Project

The final week culminates in a **3–4 person group project** building the full HMI. Three variant tracks let groups specialize:

- 🔥 **ICE Vehicle Dashboard** — RPM-prominent, fuel + engine-temp + oil-pressure indicators, gear position
- ⚡ **EV Dashboard** — battery SoC, regen indicator, range-remaining, motor-temp gauge instead of RPM
- 🔋 **Hybrid Dashboard** — power-flow visualization, mode selector, separate fuel + battery bars

Full rubric, must-haves, and submission instructions in [`final-project/README.md`](./final-project/README.md).

---

## Contributing

This repo is designed for trainees to fork and submit work. See [`CONTRIBUTING.md`](./CONTRIBUTING.md) for the branch naming, PR template, and code style.

---

## License

MIT — see [`LICENSE`](./LICENSE). Use this material freely for your own training programs; attribution appreciated but not required.

---

**Maintainer:** [@ManeParag](https://github.com/ManeParag) · *Built with ❤️ and Qt 6*
