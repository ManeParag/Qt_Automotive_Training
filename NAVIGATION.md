# 🧭 Navigation

> Quick-jump index to every module in the **Qt Automotive HMI Training**. For the full programme overview, see [`README.md`](./README.md).

---

## 📚 Module Index

| #   | Module                                                          | Phase          | Level        | Time   |
| :-- | :-------------------------------------------------------------- | :------------- | :----------- | :----- |
| 01  | [Qt Creator IDE Navigation](./01-qt-creator-ide/)               | 1 Fundamentals | Beginner     | 1.5 hr |
| 02  | [Qt Widgets & Layouts](./02-qt-widgets-layouts/)                | 1 Fundamentals | Beginner     | 2 hr   |
| 03  | [Signals & Slots](./03-signals-and-slots/)                      | 1 Fundamentals | Beginner     | 2 hr   |
| 04  | [QPainter Custom Drawing](./04-qpainter-custom-drawing/)        | 2 Intermediate | Intermediate | 3 hr   |
| 05  | [QTimer & Real-time Updates](./05-qtimer-realtime-updates/)     | 2 Intermediate | Intermediate | 1.5 hr |
| 06  | [File I/O & Data Logging](./06-file-io-data-logging/)           | 2 Intermediate | Intermediate | 2 hr   |
| 07  | [Qt Stylesheets (QSS)](./07-qt-stylesheets/)                    | 2 Intermediate | Intermediate | 1.5 hr |
| 08  | [QtCharts (Bar, Line, Pie)](./08-qtcharts/)                     | 3 Advanced     | Advanced     | 2.5 hr |
| 09  | [Qt Gauge / Speedometer](./09-qt-gauge-speedometer/)            | 3 Advanced     | Advanced     | 4 hr   |
| 10  | [Final Project — Full HMI Integration](./10-final-project/)     | 4 Project      | Project      | 1 day  |

---
---

## 📊 Progress Tracking

| Resource | Purpose |
| :--- | :--- |
| [Training Plan & Progress Tracker](./tracking/Qt_Automotive_HMI_Training_Plan.xlsx) | Master Excel — daily plan, per-trainee tracker, practice tasks, references, final project |
| [Tracking README](./tracking/README.md) | How to read and update the tracker |

## 🗺️ Recommended Order

### Phase 1 — Qt Fundamentals
Start here. Build the foundation: the IDE, widgets, layouts, and signal/slot mechanism.

➡️ [01 — Qt Creator IDE](./01-qt-creator-ide/) → [02 — Qt Widgets & Layouts](./02-qt-widgets-layouts/) → [03 — Signals & Slots](./03-signals-and-slots/)

### Phase 2 — Intermediate Qt
Move beyond static UIs: custom drawing, real-time timers, file I/O, and theming.

➡️ [04 — QPainter](./04-qpainter-custom-drawing/) → [05 — QTimer](./05-qtimer-realtime-updates/) → [06 — File I/O](./06-file-io-data-logging/) → [07 — Stylesheets](./07-qt-stylesheets/)

### Phase 3 — Advanced Qt
Charting and custom gauge widgets — the visually heavy parts of an HMI.

➡️ [08 — QtCharts](./08-qtcharts/) → [09 — Qt Gauge / Speedometer](./09-qt-gauge-speedometer/)

### Phase 4 — Final Project
Putting it all together.

➡️ [10 — Final Project](./10-final-project/)

---

## 🎯 Find a Topic Fast

| If you want to learn about…                | Go to                                        |
| :----------------------------------------- | :------------------------------------------- |
| Installing Qt and using the IDE            | [Module 01](./01-qt-creator-ide/)            |
| Buttons, sliders, dropdowns, tabs          | [Module 02](./02-qt-widgets-layouts/)        |
| Arranging widgets, responsive layouts      | [Module 02](./02-qt-widgets-layouts/)        |
| Reacting to clicks and events              | [Module 03](./03-signals-and-slots/)         |
| Drawing your own widgets from scratch      | [Module 04](./04-qpainter-custom-drawing/)   |
| Animations & periodic updates              | [Module 05](./05-qtimer-realtime-updates/)   |
| Reading / writing CSV / JSON files         | [Module 06](./06-file-io-data-logging/)      |
| Theming the UI with CSS-like syntax        | [Module 07](./07-qt-stylesheets/)            |
| Charts and live telemetry plots            | [Module 08](./08-qtcharts/)                  |
| Building a speedometer / RPM gauge         | [Module 09](./09-qt-gauge-speedometer/)      |
| Putting it all together into one HMI       | [Module 10](./10-final-project/)             |

---

## 🔧 By Qt Module Used

If you already know what Qt module you need to work with, this maps it back to where it's taught.

| Qt module          | Taught in                                                                                |
| :----------------- | :--------------------------------------------------------------------------------------- |
| **Qt Core**        | All modules (foundational)                                                               |
| **Qt Widgets**     | [02](./02-qt-widgets-layouts/), [04](./04-qpainter-custom-drawing/), [09](./09-qt-gauge-speedometer/) |
| **Qt GUI**         | [04](./04-qpainter-custom-drawing/), [09](./09-qt-gauge-speedometer/)                    |
| **Qt Charts**      | [08](./08-qtcharts/)                                                                     |
| **Qt Animation**   | [05](./05-qtimer-realtime-updates/), [09](./09-qt-gauge-speedometer/)                    |
| **Stylesheets**    | [07](./07-qt-stylesheets/)                                                               |

---

## 📦 Repository Structure

```
Qt_Automotive_Training/
├── README.md                         ← programme overview
├── NAVIGATION.md                     ← this file (quick index)
├── SETUP.md                          ← OS-specific install steps
├── CONTRIBUTING.md                   ← branch / PR / code style
├── LICENSE                           ← MIT
│
├── 01-qt-creator-ide/
│   ├── README.md
│   └── HelloAutoHMI/
│
├── 02-qt-widgets-layouts/
│   └── README.md
│
├── 03-signals-and-slots/
├── 04-qpainter-custom-drawing/
├── 05-qtimer-realtime-updates/
├── 06-file-io-data-logging/
├── 07-qt-stylesheets/
├── 08-qtcharts/
│
├── 09-qt-gauge-speedometer/
│   ├── README.md
│   └── images/
│       ├── speedometer-preview.svg
│       └── speedometer-anatomy.svg
│
└── 10-final-project/
```

---

← [Back to programme overview](./README.md)
