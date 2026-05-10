# Module 02 — Qt Widgets & Layouts

> The building blocks of every Qt HMI. Learn the standard widget set, master Qt's layout system, and start composing real automotive dashboard screens.

| Phase | Level | Time | Qt modules |
| --- | --- | --- | --- |
| Phase 1 — Qt Fundamentals | Beginner | 2 hours | Qt Core · Qt Widgets · Qt GUI |

---

## Table of Contents

1. [Why Widgets & Layouts Matter](#1-why-widgets--layouts-matter)
2. [Widgets in Automotive HMIs](#2-widgets-in-automotive-hmis)
3. [The Standard Widget Catalog](#3-the-standard-widget-catalog)
4. [Qt Layout System](#4-qt-layout-system)
5. [Building an Automotive Cluster Mock-up](#5-building-an-automotive-cluster-mock-up)
6. [Custom Widgets — When Standard Isn't Enough](#6-custom-widgets--when-standard-isnt-enough)
7. [Official Documentation Map](#7-official-documentation-map)
8. [Reference Videos](#8-reference-videos)
9. [Common Errors & Fixes](#9-common-errors--fixes)

---

## 1. Why Widgets & Layouts Matter

Everything you see on an automotive screen — a speed readout, a fuel bar, a climate-control button, a navigation panel — is, at its lowest level, a **widget**. And the way those widgets stay correctly positioned across a 7-inch cluster, a 12-inch IVI screen, and a 15-inch passenger display is handled by **layouts**.

Get widgets and layouts right and you've solved 70 % of the visual side of an HMI. Get them wrong and you'll spend the rest of the project fighting pixel coordinates every time the resolution changes.

A widget is any visible element that inherits from `QWidget`. It owns a region of the screen, it can paint itself, and it can receive mouse, keyboard, and touch events. Buttons, labels, sliders, progress bars, lists, and even your top-level window are all widgets.

A layout is **not** a widget — it's a non-visual manager that decides where its child widgets go and how they should resize when the parent grows or shrinks. Layouts are why a Qt app written for 1024×600 still looks correct when the OEM swaps in a 1920×720 panel six months into the project.

---

## 2. Widgets in Automotive HMIs

Open the dashboard of any modern car running Qt and you can usually identify the widget under each visual element:

| Dashboard element | Widget(s) typically used |
| --- | --- |
| Speed digits "120 km/h" | `QLabel` with a styled font |
| Fuel / battery bar | `QProgressBar` (often custom-styled) or custom paint |
| Turn-signal arrows | `QLabel` with an icon, blinked via `QTimer` |
| ADAS warning chime button | `QPushButton` with stylesheet |
| Climate temperature dial | `QDial` or custom widget |
| Media volume slider | `QSlider` |
| Drive-mode selector (Eco / Sport / Snow) | `QButtonGroup` of `QRadioButton`s |
| Trip-info list (Avg speed, Fuel used, Range) | `QListWidget` or `QTableWidget` |
| Navigation panel | container `QWidget` + nested layouts |
| Settings / Preferences dialog | `QDialog` with `QFormLayout` |

The same widget catalog you'd use to write a desktop app powers what the driver sees in front of them. The visual treatment differs — automotive HMIs are heavily styled with QSS or replaced with custom-painted variants — but the underlying `QWidget` hierarchy is the same.

### Why widgets, not Qt Quick, for clusters?

Qt offers two UI technologies: **Qt Widgets** (C++, what this module covers) and **Qt Quick / QML** (declarative, JavaScript-flavoured). For consumer-facing flashy IVI systems, QML wins on animation. But many automotive teams still pick Widgets for:

- **Lower memory footprint** — important on entry-level clusters
- **Deterministic rendering** — easier to certify for ISO 26262 with Qt Safe Renderer
- **All-C++ stack** — no JavaScript engine, simpler debugging
- **Mature tooling** — Designer, stylesheets, decades of examples

A common pattern is **Widgets for the cluster, QML for the IVI** in the same vehicle.

---

## 3. The Standard Widget Catalog

Qt ships ~50 widget classes. You'll use about a dozen of them every day. Group them by purpose:

### Display widgets — read-only

| Widget | Purpose | Auto HMI example |
| --- | --- | --- |
| `QLabel` | text or image display | speed digits, gear indicator "P R N D" |
| `QLCDNumber` | retro 7-segment digits | trip odometer |
| `QProgressBar` | filled bar 0–100 % | fuel level, battery SoC |
| `QFrame` | container with border | grouping warning lights |

### Input widgets — user interaction

| Widget | Purpose | Auto HMI example |
| --- | --- | --- |
| `QPushButton` | clickable button | "Start Engine", climate fan + / − |
| `QToolButton` | icon-only button | media play / pause |
| `QCheckBox` | on / off toggle | enable lane-keep assist |
| `QRadioButton` | one-of-many choice | drive mode selector |
| `QSlider` | drag to set value | volume, brightness |
| `QDial` | rotary knob | temperature setting |
| `QComboBox` | drop-down list | language, units |
| `QLineEdit` | single-line text | Bluetooth pairing PIN |
| `QSpinBox` | numeric up / down | set timer minutes |

### Container widgets — hold other widgets

| Widget | Purpose | Auto HMI example |
| --- | --- | --- |
| `QWidget` | plain container | every panel you build |
| `QGroupBox` | titled border + content | "Driver Assist" group |
| `QTabWidget` | tabbed pages | Phone / Media / Nav tabs |
| `QStackedWidget` | one page visible at a time | switch between cluster views |
| `QScrollArea` | scrollable content | long contact list |

### Item-view widgets — collections of data

| Widget | Purpose | Auto HMI example |
| --- | --- | --- |
| `QListWidget` | flat list of items | call log, recent destinations |
| `QTableWidget` | rows × columns | trip statistics |
| `QTreeWidget` | hierarchical | settings menu |

> 📘 **Reference:** [Widget Classes (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/widget-classes.html) · [Qt Widgets Examples (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/examples-widgets.html)

---

## 4. Qt Layout System

Layouts are how you say *"these widgets go side-by-side"* or *"this one fills whatever space is left."* You almost never set widget positions manually with `move()` and `resize()` in production code — it breaks the moment the screen size changes.

Qt provides four built-in layout managers. Master these and you can build anything.

### `QHBoxLayout` — horizontal row

Arranges children left-to-right (or right-to-left for RTL languages — Qt handles Arabic / Hebrew automatically).

    QWidget *panel = new QWidget;
    QHBoxLayout *row = new QHBoxLayout(panel);
    row->addWidget(new QLabel("Speed:"));
    row->addWidget(new QLabel("120"));
    row->addWidget(new QLabel("km/h"));

**Auto HMI use:** the row of warning icons across the top of a cluster.

### `QVBoxLayout` — vertical column

Same idea, top-to-bottom.

    QVBoxLayout *col = new QVBoxLayout(panel);
    col->addWidget(speedLabel);
    col->addWidget(rpmLabel);
    col->addWidget(gearLabel);

**Auto HMI use:** stacking the speed read-out, RPM, and gear indicator in the centre of a cluster.

### `QGridLayout` — rows × columns

The workhorse. Place widgets at specific row / column positions, optionally spanning multiple cells.

    QGridLayout *grid = new QGridLayout(panel);
    grid->addWidget(speedometer,   0, 0, 2, 1);   // row 0, col 0, span 2 rows
    grid->addWidget(tachometer,    0, 1, 2, 1);   // row 0, col 1, span 2 rows
    grid->addWidget(fuelGauge,     0, 2);
    grid->addWidget(tempGauge,     1, 2);

**Auto HMI use:** the entire cluster layout — two large gauges side-by-side with smaller indicators on the right.

### `QFormLayout` — label + field rows

Two columns: a label on the left, an input widget on the right. Built specifically for settings dialogs.

    QFormLayout *form = new QFormLayout(panel);
    form->addRow("Driver name:",  nameEdit);
    form->addRow("Units:",        unitsCombo);
    form->addRow("Language:",     langCombo);

**Auto HMI use:** the user-profile screen, Bluetooth pairing dialog, settings panes.

### Spacers, stretches, and nesting

Three more concepts and you're done:

- **`addStretch()`** — adds an invisible spring that absorbs leftover space. Use it to push widgets to one edge: `row->addWidget(logo); row->addStretch(); row->addWidget(closeButton);` puts the logo on the left and close on the right.
- **`QSpacerItem`** — a fixed-size gap, useful for breathing room.
- **Nesting** — layouts can contain other layouts (`addLayout()`), which is how you build complex screens. A grid layout for the overall cluster, with vertical layouts inside each cell.

> 📘 **Reference:** [Layout Management (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/layout.html) · [QHBoxLayout](https://doc.qt.io/archives/qt-6.1/qhboxlayout.html) · [QVBoxLayout](https://doc.qt.io/archives/qt-6.1/qvboxlayout.html) · [QGridLayout](https://doc.qt.io/archives/qt-6.1/qgridlayout.html) · [QFormLayout](https://doc.qt.io/archives/qt-6.1/qformlayout.html)

### Size policies — the rule that resolves resize conflicts

When the parent grows, who absorbs the extra pixels? The `QSizePolicy` of each child decides.

| Policy | Meaning |
| --- | --- |
| `Fixed` | never resizes — stays at `sizeHint()` |
| `Minimum` | uses `sizeHint()` as minimum, can grow |
| `Maximum` | uses `sizeHint()` as maximum, can shrink |
| `Preferred` | default — sizeHint preferred, can grow or shrink |
| `Expanding` | grabs as much space as it can get |
| `MinimumExpanding` | minimum is sizeHint, wants more |
| `Ignored` | layout ignores the widget's size hint entirely |

Rule of thumb for a cluster: gauges set to `Expanding`, status icons set to `Fixed`, padding handled by `addStretch()`.

> 📘 **Reference:** [QSizePolicy (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qsizepolicy.html)

---

## 5. Building an Automotive Cluster Mock-up

Putting widgets and layouts together. The starter project in this folder builds a **simplified instrument cluster** with three regions:

    ┌────────────────────────────────────────────────────────┐
    │  ⚠  🔋  💡  🛡                              📶  📞  ⚙  │   ← top bar (QHBoxLayout)
    ├──────────────┬───────────────────┬───────────────────┤
    │              │                   │                   │
    │ Tachometer   │   Speedometer     │   Fuel  Temp      │   ← centre (QGridLayout)
    │ (custom)     │   "120 km/h"      │   ▓▓▓▓░  ▓▓░░░    │
    │              │                   │                   │
    ├──────────────┴───────────────────┴───────────────────┤
    │  Gear: D    Trip: 142 km    Range: 380 km    Eco     │   ← bottom bar (QHBoxLayout)
    └────────────────────────────────────────────────────────┘

The whole window uses a top-level `QVBoxLayout` with three children — top bar, centre grid, bottom bar — and stretch factors so the centre grid takes most of the height.

### Skeleton code

    // mainwindow.cpp
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    {
        auto *central = new QWidget(this);
        setCentralWidget(central);

        auto *root = new QVBoxLayout(central);

        // --- top bar ---
        auto *top = new QHBoxLayout;
        top->addWidget(new QLabel("⚠"));
        top->addWidget(new QLabel("🔋"));
        top->addStretch();                 // pushes the right group to the edge
        top->addWidget(new QLabel("📶"));
        top->addWidget(new QLabel("⚙"));
        root->addLayout(top);

        // --- centre ---
        auto *centre = new QGridLayout;
        centre->addWidget(makeTachometer(),    0, 0);
        centre->addWidget(makeSpeedometer(),   0, 1);
        centre->addWidget(makeFuelGauge(),     0, 2);
        centre->setColumnStretch(0, 2);
        centre->setColumnStretch(1, 3);    // speedo is widest
        centre->setColumnStretch(2, 2);
        root->addLayout(centre, /*stretch*/ 1);

        // --- bottom bar ---
        auto *bottom = new QHBoxLayout;
        bottom->addWidget(new QLabel("Gear: D"));
        bottom->addWidget(new QLabel("Trip: 142 km"));
        bottom->addWidget(new QLabel("Range: 380 km"));
        bottom->addStretch();
        bottom->addWidget(new QLabel("Eco"));
        root->addLayout(bottom);
    }

Open `02-qt-widgets-layouts/ClusterMockup/ClusterMockup.pro` in Qt Creator and press **Ctrl+R** to see it run.

> 📘 **Reference:** [Qt Widgets — Window & Layout Example (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qtwidgets-tutorials-widgets-windowlayout-example.html)

---

## 6. Custom Widgets — When Standard Isn't Enough

You won't ship an automotive cluster using a stock `QProgressBar` for the fuel gauge — it'd look like a desktop app. There are three paths to a custom look, in increasing order of effort:

### Path 1 — Qt Style Sheets (QSS)

CSS-like syntax to restyle any widget. Good for colours, fonts, borders, icons.

    QPushButton {
        background-color: #1a1a1a;
        color: #00d4ff;
        border: 2px solid #00d4ff;
        border-radius: 8px;
        padding: 10px 20px;
        font: bold 16pt "Roboto";
    }
    QPushButton:hover   { background-color: #003040; }
    QPushButton:pressed { background-color: #00d4ff; color: #1a1a1a; }

A surprising amount of automotive HMI styling is done with QSS. Covered in detail in **Module 04 — Styling & QSS**.

### Path 2 — Subclass and override `paintEvent`

For shapes that no standard widget gives you — circular tachometer arcs, sweeping needles, custom warning icons. You inherit from `QWidget`, override `paintEvent(QPaintEvent*)`, and draw with `QPainter`.

    void TachometerWidget::paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        // draw arc, ticks, needle ...
    }

Covered in **Module 06 — Custom Widgets & QPainter**.

### Path 3 — Promote in Qt Designer

When you've written a custom widget, you can use it inside a `.ui` file by **promoting** a placeholder `QWidget`: right-click → *Promote to…* → enter your class name. Designer shows the placeholder; the runtime uses your class.

> 📘 **Reference:** [Qt Style Sheets (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/stylesheet.html) · [QPainter (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qpainter.html) · [Custom Widgets in Designer (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/designer-using-custom-widgets.html)

---

## 7. Official Documentation Map

Bookmark these — every link is the **Qt 6.1** version (the same pages exist under `doc.qt.io/qt-5/...` for Qt 5.15).

### Widgets

| Resource | What it gives you |
| --- | --- |
| [Qt Widgets Index](https://doc.qt.io/archives/qt-6.1/qtwidgets-index.html) | Top-level overview of the module |
| [All Widget Classes](https://doc.qt.io/archives/qt-6.1/widget-classes.html) | One-page list of every `QWidget` subclass |
| [Qt Widgets Examples](https://doc.qt.io/archives/qt-6.1/examples-widgets.html) | Runnable example apps |
| [Widgets Tutorial](https://doc.qt.io/archives/qt-6.1/qtwidgets-tutorials-widgets-toplevel-example.html) | Hands-on walk-through, beginner-friendly |

### Layouts

| Resource | What it gives you |
| --- | --- |
| [Layout Management](https://doc.qt.io/archives/qt-6.1/layout.html) | The canonical reference — read once cover-to-cover |
| [QHBoxLayout](https://doc.qt.io/archives/qt-6.1/qhboxlayout.html) · [QVBoxLayout](https://doc.qt.io/archives/qt-6.1/qvboxlayout.html) | Horizontal and vertical box layouts |
| [QGridLayout](https://doc.qt.io/archives/qt-6.1/qgridlayout.html) | Rows × columns layout |
| [QFormLayout](https://doc.qt.io/archives/qt-6.1/qformlayout.html) | Label + field forms |
| [QStackedLayout](https://doc.qt.io/archives/qt-6.1/qstackedlayout.html) | One-of-many page switcher |
| [QSizePolicy](https://doc.qt.io/archives/qt-6.1/qsizepolicy.html) | How widgets negotiate space |

### Designer & styling

| Resource | What it gives you |
| --- | --- |
| [Qt Designer Manual](https://doc.qt.io/archives/qt-6.1/qtdesigner-manual.html) | Full visual-editor reference |
| [Qt Style Sheets](https://doc.qt.io/archives/qt-6.1/stylesheet.html) | QSS guide & examples |
| [Style Sheet Reference](https://doc.qt.io/archives/qt-6.1/stylesheet-reference.html) | Every selector and property |

### Automotive context

| Resource | What it gives you |
| --- | --- |
| [Qt for Automotive](https://www.qt.io/product/automotive) | Commercial automotive package |
| [Qt Safe Renderer](https://doc.qt.io/QtSafeRenderer/) | ISO 26262 safe rendering |
| [Qt Wiki — Qt for Automotive](https://wiki.qt.io/Qt_for_Automotive) | Community demos and case studies |

---

## 8. Reference Videos

Curated picks. Watch the first two before starting the exercises in this folder.

| Video | Length | Why watch |
| --- | --- | --- |
| [Introduction to Qt Widgets — Hello World (KDAB)](https://www.youtube.com/watch?v=g7yijWiZTmI) | ~10 min | Episode 1 of KDAB's official Qt Widgets series — sets up the mental model |
| [Introduction to Qt Widgets — Layout in Qt Designer (KDAB)](https://www.youtube.com/watch?v=10A_A7NkdRs) | ~15 min | Building a real form with layouts in Designer, step-by-step |
| [Layouts in Qt C++ — Crash Course](https://www.youtube.com/watch?v=DK7Yc7i-tb4) | ~20 min | All four layout managers covered with code |
| [How to use Qt Layouts](https://www.youtube.com/watch?v=h_DVfsD9PKI) | ~15 min | Going from a GUI design sketch to working layout code |
| [GridLayout in Qt Designer (KDAB)](https://www.youtube.com/watch?v=X116W8Uk9Wk) | ~12 min | Deeper dive on grid layouts — relevant to cluster design |
| [Qt Tutorials for Beginners — Layouts (H, V, Grid, Form)](https://www.youtube.com/watch?v=-GtxYBAfPPw) | ~15 min | Concise tour of all four standard layout types |

---

## 9. Common Errors & Fixes

### Widgets overlap or appear in the wrong place

You set `move()` / `resize()` instead of using a layout, or you forgot to call `setLayout()` on the parent. Fix: build a layout, add the widgets to it, set the layout on the parent widget. Never hard-code positions.

### "QLayout: Attempting to add QLayout to QWidget which already has a layout"

You called `setLayout()` twice on the same widget, or you passed the parent into both the widget constructor and `setLayout()`. Fix: pick one. Either `new QHBoxLayout(parent)` *or* `parent->setLayout(layout)` — not both.

### Widget added to layout doesn't appear

Three usual causes:

1. The parent widget was never `show()`-n.
2. The widget was added to the layout *after* its parent was already shown — call `update()` or just add before showing.
3. `QSizePolicy::Fixed` with `sizeHint()` of `(0, 0)` — set a meaningful minimum size or change the policy.

### Layout looks fine in Designer, broken at runtime

Almost always a missing `setLayout()` in your generated code, or you forgot to call `ui->setupUi(this)` in the constructor. Open `ui_*.h` (in your build folder) and confirm the layout is being applied.

### Stylesheet isn't applied

- Stylesheets cascade. Set on the parent and they affect children. If a child has its own stylesheet, it overrides the parent's for that widget.
- Check selector syntax — `QPushButton#startButton` selects by `objectName`, set via `setObjectName("startButton")`.
- If you subclass `QWidget` and override `paintEvent`, stylesheets are ignored unless you call the QStyle paint helpers explicitly.

### App freezes when a button is held down

You're doing work in the event handler that should be on a worker thread. The event loop can't process more events until your slot returns. Quick fix for testing: `QApplication::processEvents()`. Proper fix: move the work to a `QThread` (covered in **Module 07 — Threading**).

### Window opens at tiny size

You forgot to add a layout, so the window has no `sizeHint()`. Add a layout, or call `resize(800, 480)` on the main window before `show()`.

---

## What's next

Once you can lay out the cluster mock-up and tweak its widgets, you're ready for **[Module 03 — Signals & Slots](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/03-signals-and-slots)** *(coming soon)* — the mechanism that wires button clicks, sensor updates, and animation timers to the rest of your HMI.

---

← [Previous module](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/01-qt-creator-ide) · [Back to syllabus](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/README.md) · [Next module →](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/03-signals-and-slots) *(coming soon)*
