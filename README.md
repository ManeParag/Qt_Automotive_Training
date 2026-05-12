# Qt Automotive HMI Training

> A hands-on, project-driven training program that takes you from your first `Hello, Qt` window to a fully working **automotive HMI dashboard** — speedometer, RPM gauge, fuel & temperature progress bars, scrolling telemetry chart, CSV data logging, dark automotive theme.

![Qt](https://img.shields.io/badge/Qt-6.x-41CD52?logo=qt) ![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus) ![License](https://img.shields.io/badge/license-MIT-blue) ![Duration](https://img.shields.io/badge/duration-2%20weeks-orange)

📖 **[Module Navigation →](./NAVIGATION.md)** — quick links to every module's README

---

## Table of Contents

- [What You'll Build](#what-youll-build)
- [Who This Is For](#who-this-is-for)
- [Competencies](#competencies)
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

| Component                        | Module(s) used                             |
| :------------------------------- | :----------------------------------------- |
| Window + dock layout             | 02 (Widgets & Layouts)                     |
| Speedometer + RPM gauge          | 04 (QPainter), 09 (Qt Gauge / Speedometer) |
| Fuel & Temperature progress bars | 02, 07 (Stylesheets)                       |
| Live updates @ 10 Hz             | 03 (Signals & Slots), 05 (QTimer)          |
| Scrolling speed line chart       | 08 (QtCharts)                              |
| Trip distance bar chart          | 08 (QtCharts)                              |
| CSV telemetry logging + replay   | 06 (File I/O)                              |
| Dark automotive theme            | 07 (Qt Stylesheets)                        |

A reference architecture and submission rubric live in [`10-final-project/`](./10-final-project/).

---

## Who This Is For

- **C++ developers new to Qt** who want a structured path into Qt 6
- **Automotive / embedded engineers** exploring Qt for instrument-cluster and IVI work
- **Students or interns** onboarding to a Qt-based product team
- **Self-learners** who prefer building over reading

You don't need any Qt experience to start. You **do** need basic C++ (classes, pointers, references) and to be comfortable in a terminal.

---

## Competencies

This section spells out what you should walk in with, and what you'll walk out with. Use it as a self-check before starting and a credential when you've finished.

### 🎯 Required Going In *(must have)*

| Competency                          | What it means in practice                                              |
| :---------------------------------- | :--------------------------------------------------------------------- |
| **C++ fundamentals**                | Classes, inheritance, pointers vs. references, headers vs. source, `const`, basic templates |
| **Object-oriented thinking**        | Comfortable modelling problems as classes with responsibilities and collaborators |
| **Build & toolchain basics**        | Understand what a compiler, linker, and build folder are; comfortable with one of MSVC / GCC / Clang |
| **Command-line literacy**           | Navigate folders, run executables, set environment variables           |
| **Git basics**                      | `clone`, `add`, `commit`, `push`, `pull`, working on a branch          |

### ⚡ Helpful but Optional

| Competency                          | Why it helps                                                           |
| :---------------------------------- | :--------------------------------------------------------------------- |
| **Prior GUI framework experience**  | MFC, WinForms, GTK, JavaFX — any prior GUI work makes Qt's event loop and widget tree concepts faster to absorb |
| **Modern C++17 features**           | Lambdas, `auto`, range-for, `std::function` — used heavily in signal/slot connections |
| **Basic trigonometry**              | Used in gauge needle rotation and tick-mark placement (Module 04 & 09) |
| **Automotive domain awareness**     | Understanding what a cluster, IVI, HUD, ADAS warning is — helps contextualise the design choices |
| **CMake or qmake exposure**         | Speeds up the build-system parts; covered from scratch in Module 01    |

### 🚀 Competencies You'll Develop

By completing the program you will be able to:

#### **Qt framework mastery**
- Navigate Qt Creator, configure kits, debug, profile, and use the visual UI designer
- Compose user interfaces from the standard widget catalog using box and grid layouts
- Wire components together with the signal/slot mechanism (Qt's event-driven backbone)
- Build modular, reusable Qt widget classes with `Q_PROPERTY` and proper meta-object integration
- Apply Qt Stylesheets (QSS) to skin applications without changing C++ code

#### **Custom 2D graphics**
- Subclass `QWidget` and override `paintEvent` to draw custom visuals from scratch
- Use `QPainter` with antialiasing, gradients, transformations (translate / rotate / scale)
- Implement animated indicators (gauge needles, sweeping arcs) with `QPropertyAnimation`
- Coordinate timer-driven repaints at 60 fps without flicker or excess CPU

#### **Data & I/O**
- Read and write CSV, JSON, and plain-text files using `QFile` and `QTextStream`
- Log telemetry to disk and replay it offline
- Visualise time-series and aggregate data with QtCharts (line, bar, pie, scatter)

#### **Automotive HMI engineering**
- Recognise the standard parts of an instrument cluster (speedometer, tachometer, fuel/temp bars, warning icons, drive-mode badge)
- Architect a cluster as cooperating widgets that subscribe to a shared data source
- Handle 10 Hz sensor updates with smoothing and redundancy filtering
- Design themable widgets that an OEM team can re-skin for multiple vehicle models
- Apply unit switching (km/h ↔ mph) and basic internationalisation patterns

#### **Production discipline**
- Structure a multi-module Qt project so individual widgets are testable in isolation
- Read and use the official Qt 6 documentation effectively
- Diagnose common Qt pitfalls (missing `Q_OBJECT`, leaking transformations, stale `update()`, layout-fights)

When you finish the program, these competencies map directly to the day-one expectations of a junior Qt HMI engineer at an automotive Tier-1 or OEM.

---

## Prerequisites

| Requirement     | Minimum                          | Recommended         |
| :-------------- | :------------------------------- | :------------------ |
| C++ knowledge   | Classes, references              | C++17 features      |
| OS              | Win 10 / Ubuntu 20.04 / macOS 11 | latest LTS          |
| RAM             | 8 GB                             | 16 GB               |
| Disk            | 15 GB free                       | 30 GB free          |
| Compiler        | MSVC 2019 / GCC 9 / Clang 12     | latest              |
| Qt version      | Qt 5.15 LTS                      | **Qt 6.1+**         |

Full install steps are in [`SETUP.md`](./SETUP.md).

---

## Setup

```bash
# 1. Clone this repo
git clone https://github.com/ManeParag/Qt_Automotive_Training.git
cd Qt_Automotive_Training

# 2. Install Qt 6 — see SETUP.md for OS-specific steps

# 3. Open any module's .pro file in Qt Creator and hit Ctrl+R to build & run
```

That's it. Each module is a standalone, runnable Qt project. No global dependencies, no external SDKs.

---

## Learning Path

The course is organized into **4 phases across 2 weeks**, with 9 teaching modules building progressively into the final integration project.

### 🟦 Phase 1 — Qt Fundamentals *(Week 1)*

Build the foundation: the IDE, widgets, and the signal/slot mechanism that ties Qt apps together.

| #   | Module                                                          | Level    | Time   |
| :-- | :-------------------------------------------------------------- | :------- | :----- |
| 01  | [Qt Creator IDE Navigation](./01-qt-creator-ide/)               | Beginner | 1.5 hr |
| 02  | [Qt Widgets & Layouts](./02-qt-widgets-layouts/)                | Beginner | 2 hr   |
| 03  | [Signals & Slots](./03-signals-and-slots/)                      | Beginner | 2 hr   |

### 🟩 Phase 2 — Intermediate Qt *(Week 1 → Week 2)*

Move beyond static UIs: custom drawing, real-time timers, file I/O, and theming.

| #   | Module                                                          | Level        | Time   |
| :-- | :-------------------------------------------------------------- | :----------- | :----- |
| 04  | [QPainter Custom Drawing](./04-qpainter-custom-drawing/)        | Intermediate | 3 hr   |
| 05  | [QTimer & Real-time Updates](./05-qtimer-realtime-updates/)     | Intermediate | 1.5 hr |
| 06  | [File I/O & Data Logging](./06-file-io-data-logging/)           | Intermediate | 2 hr   |
| 07  | [Qt Stylesheets (QSS)](./07-qt-stylesheets/)                    | Intermediate | 1.5 hr |

### 🟧 Phase 3 — Advanced Qt *(Week 2)*

Advanced visualization: charting and custom gauge widgets — the visually heavy parts of an HMI.

| #   | Module                                                          | Level    | Time   |
| :-- | :-------------------------------------------------------------- | :------- | :----- |
| 08  | [QtCharts (Bar, Line, Pie)](./08-qtcharts/)                     | Advanced | 2.5 hr |
| 09  | [Qt Gauge / Speedometer](./09-qt-gauge-speedometer/)            | Advanced | 4 hr   |

### 🟥 Phase 4 — Final Project *(Week 2)*

| #   | Module                                                          | Level   | Time     |
| :-- | :-------------------------------------------------------------- | :------ | :------- |
| 10  | [Full HMI Integration](./10-final-project/)                     | Project | Full day |

---

## How To Use This Repo

Each module folder follows the same structure:

```
NN-module-name/
├── README.md          ← Theory, concepts, exercise, submission checklist
├── images/            ← Diagrams and SVG assets referenced from README.md
├── ProjectName/       ← Runnable Qt 6 starter project (where applicable)
│   ├── ProjectName.pro
│   ├── main.cpp
│   ├── mainwindow.h
│   ├── mainwindow.cpp
│   └── mainwindow.ui
└── exercises/
    └── README.md      ← The "your turn" task you submit
```

**Suggested workflow per module:**

1. Read the module `README.md` start to finish (~10–15 min)
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

Full rubric, must-haves, and submission instructions in [`10-final-project/README.md`](./10-final-project/).

---

## Contributing

This repo is designed for trainees to fork and submit work. See [`CONTRIBUTING.md`](./CONTRIBUTING.md) for the branch naming, PR template, and code style.

---

## License

MIT — see [`LICENSE`](./LICENSE). Use this material freely for your own training programs; attribution appreciated but not required.

---

**Maintainer:** [@ManeParag](https://github.com/ManeParag) · *Built with ❤️ and Qt 6*
