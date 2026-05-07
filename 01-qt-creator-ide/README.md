# Module 01 — Introduction to Qt & Qt Creator

> Your starting point. Understand what Qt is, why automotive teams pick it, get it installed, and learn your way around the IDE.

| Phase | Level | Time | Qt modules |
| :---- | :---- | :--- | :--------- |
| Phase 1 — Qt Fundamentals | Beginner | 1.5 hours | Qt Core · Qt Widgets |

---

## Table of Contents

1. [What is Qt?](#1-what-is-qt)
2. [Qt in Automotive](#2-qt-in-automotive)
3. [Installation](#3-installation)
4. [Qt Creator IDE Tour](#4-qt-creator-ide-tour)
5. [Anatomy of a Qt Project](#5-anatomy-of-a-qt-project)
6. [Official Documentation Map](#6-official-documentation-map)
7. [Common Errors & Fixes](#7-common-errors--fixes)

---

## 1. What is Qt?

**Qt** (pronounced *"cute"*) is a cross-platform C++ framework for building applications with graphical user interfaces. Write your code once, and the same source compiles and runs on Windows, Linux, macOS, embedded Linux, Android, iOS, and even microcontrollers.

A few things make Qt stand out:

- **One codebase, many targets** — the same C++ runs on a desktop, a car's infotainment screen, and a phone.
- **Native look and feel** — your app uses each platform's native widgets where appropriate.
- **Extensive module library** — networking, multimedia, charts, 3D graphics, serial ports, Bluetooth, SQL — all built in.
- **Mature tooling** — Qt Creator IDE, visual UI designer, built-in debugger and profiler.
- **Active commercial backing** — maintained by The Qt Company with both open-source and commercial licenses.

Qt was first released in 1995 and has been used in everything from KDE Linux desktop to satellite control systems. Today it's one of the most widely deployed C++ UI frameworks in the world.

---

## 2. Qt in Automotive

Qt has become the **de facto standard** for in-vehicle software UIs. When you sit in a modern car and interact with the digital instrument cluster, the infotainment system, or the heads-up display, there's a very good chance Qt is rendering what you see.

### Who uses Qt in their vehicles?

- **Mercedes-Benz** — MBUX infotainment system
- **BMW** — iDrive, digital cluster
- **Volkswagen Group** — Cariad software platform across VW, Audi, Porsche
- **Tesla** — early Model S/X infotainment
- **Lucid Motors** — Lucid Air dashboard
- **Rivian, Polestar, Hyundai, Honda, Peugeot** — various dashboard and IVI systems
- **John Deere, Caterpillar** — heavy-equipment HMIs

### Why automotive picks Qt

| Need                                          | How Qt addresses it                                          |
| :-------------------------------------------- | :----------------------------------------------------------- |
| Single codebase for cluster + IVI + HUD       | One Qt project targets multiple displays                     |
| 60 fps animations on embedded hardware        | Hardware-accelerated rendering via OpenGL / Vulkan           |
| Long product lifecycle (10+ years)            | LTS releases, commercial support, stable API                 |
| Functional safety certification               | Qt Safe Renderer is ISO 26262 ASIL-B certified               |
| Multi-language (i18n) support                 | Built-in translation system, RTL languages, font handling    |
| Boot-time constraints                         | Qt for MCUs and Qt Quick Compiler reduce startup latency     |

### Where Qt fits in a typical car

A modern vehicle has multiple screens, and Qt is commonly used for all of them:

- **Digital Instrument Cluster** — speedometer, RPM, warning lights (replaces analogue gauges)
- **In-Vehicle Infotainment (IVI)** — navigation, media, phone, climate
- **Head-Up Display (HUD)** — projected speed, navigation arrows
- **Rear-Seat Entertainment** — passenger screens
- **Heavy-equipment dashboards** — tractors, excavators, trucks

This training program is focused on building an **instrument-cluster-style HMI** — exactly the kind of UI you'd find in front of a driver.

### Qt for Automotive offerings

The Qt Company packages a few automotive-specific products:

- **Qt for Automotive** — the commercial package with Qt Safe Renderer, automotive grade Linux integration, IVI reference applications.
- **Qt Auto** — newer initiative bringing Qt closer to AAOS (Android Automotive OS) and other automotive platforms.
- **Qt for MCUs** — for low-power, low-memory clusters running on microcontrollers (no Linux).

Throughout this training we use plain **open-source Qt 6** running on a normal desktop — no automotive-specific SDK needed. The HMI we build looks and behaves like a real cluster.

---

## 3. Installation

We use **Qt 6.5 LTS or newer** with the open-source license. Total install size is around 10–12 GB.

### Step 1 — Create a Qt account

Go to <https://www.qt.io/download-qt-installer> and create a free account. The open-source download requires a sign-in but doesn't cost anything. Confirm your email before proceeding.

### Step 2 — Download the online installer

The same page provides a small installer for your OS (Windows / Linux / macOS). Download and run it.

### Step 3 — Choose components

When the installer asks what to install, **tick at minimum**:

- ✅ **Qt 6.5** *(or the latest LTS shown)*
  - ✅ **MSVC 2019 64-bit** *(Windows — preferred)* OR **MinGW 11.2 64-bit** *(Windows — alternative)*
  - ✅ **GCC 64-bit** *(Linux)*
  - ✅ **macOS** *(Mac)*
  - ✅ **Qt Charts** *(needed for charting modules)*
  - ✅ **Qt Debug Information Files** *(helps with debugging — optional but recommended)*
- ✅ **Qt Creator** *(the IDE — usually pre-selected)*
- ✅ **CMake** and **Ninja** *(build tools — usually pre-selected)*

You can leave everything else at defaults. Coffee break — install takes 20–40 minutes depending on your internet speed.

### Step 4 — Verify the install

Open **Qt Creator**. You should see the Welcome screen.

- Go to **Edit → Preferences → Kits**
- You should see at least one kit with a green ✅ checkmark
- If you see a red ❌, click the kit and read the warning — usually a missing compiler

### Step 5 — Build the smoke-test project

Open `01-qt-creator-ide/HelloAutoHMI/HelloAutoHMI.pro` from this repo in Qt Creator. Pick your kit when prompted. Press **Ctrl+R** (Build & Run).

If a window titled *"Hello AutoHMI"* appears with a button on it, your toolchain is working. ✅

For OS-specific notes and troubleshooting, see [`SETUP.md`](../SETUP.md) at the repo root.

---

## 4. Qt Creator IDE Tour

Qt Creator is the IDE that ships with Qt. Unlike adding Qt support to a generic editor, here it's first-class — the editor understands Qt classes, the designer is built in, and the debugger knows how to display Qt types.

The five things to learn on day one:

### Welcome mode

The landing page when Qt Creator opens. Quick access to recent projects, examples (browse Qt's hundreds of example apps directly), and tutorials. A great place to explore working code.

### Edit mode

The main code editor. Highlights:

- **Auto-completion** for every Qt class and method
- **F2** — jump to definition of whatever's under the cursor
- **F4** — switch between header `.h` and source `.cpp` of the same class
- **Ctrl+K** — locator: type a filename, line number, class name, or symbol to jump anywhere
- **Right side** — outline of the current file's classes and methods

### Design mode

The visual UI editor. When you open a `.ui` file, Qt Creator switches to this mode. Drag widgets from the left panel onto the form, set their properties on the right panel. Saves to XML which qmake compiles into C++ at build time.

### Build & Run buttons (bottom-left corner)

- **Hammer icon (Ctrl+B)** — build the project, don't run
- **Green play (Ctrl+R)** — build and run
- **Bug + green play (F5)** — build and run under the debugger
- The dropdown above lets you switch between **Debug** and **Release** builds, and switch kits

### Issues / Application Output panes (bottom)

- **Issues** — compile errors and warnings, click to jump to the line
- **Application Output** — anything your app prints with `qDebug()` shows up here
- **Compile Output** — the raw compiler messages
- **Debugger Console** — for typing GDB/CDB commands during a debug session

That's 90% of what you need. Everything else — version control panels, profiler, test runner — you'll discover as you need it.

---

## 5. Anatomy of a Qt Project

Open any of the starter projects in this repo and you'll see a handful of files. Here's what each one does.

### `<ProjectName>.pro` — the project file

This is the **manifest**. qmake reads it to figure out what to compile and how. Typical contents:

```pro
QT       += core gui widgets   # which Qt modules this app uses
CONFIG   += c++17              # use C++17
TARGET    = HelloAutoHMI       # name of the output executable
TEMPLATE  = app                # we're building an application

SOURCES  += main.cpp mainwindow.cpp
HEADERS  += mainwindow.h
FORMS    += mainwindow.ui
```

When you add a new `.cpp` or `.h` file to your project, you list it here. Right-clicking *Add New…* in Qt Creator handles this for you automatically.

> 📘 **Reference:** [qmake Manual](https://doc.qt.io/qt-6/qmake-manual.html) · [Qt build system overview](https://doc.qt.io/qt-6/build-system-overview.html)

### `main.cpp` — the entry point

Every Qt app starts here. Tiny on purpose:

```cpp
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);   // initializes Qt
    MainWindow w;                   // creates the main window
    w.show();                       // shows it
    return app.exec();              // enters the event loop
}
```

`QApplication` is the Qt runtime. `app.exec()` runs the event loop that handles mouse clicks, key presses, timers, and signals — control returns from `exec()` only when the app quits.

### `mainwindow.h` — the main window class declaration

Where you declare your window's C++ interface — its methods, signals, slots, and member variables. Marked with `Q_OBJECT` so Qt's meta-object compiler (`moc`) generates the signal/slot machinery at build time.

### `mainwindow.cpp` — the main window class implementation

Where you implement the methods declared in the header. Sets up child widgets, connects signals to slots, handles user interaction.

### `mainwindow.ui` — the visual form (optional)

XML file describing the window's visual layout. Edited in Qt Creator's Design mode (drag and drop widgets). At build time, qmake's `uic` tool converts it to a C++ header (`ui_mainwindow.h`) that your `mainwindow.cpp` includes.

You don't have to use a `.ui` file — many of the modules in this repo build their UI in pure C++ for clarity. For complex layouts, the visual designer saves time.

### Build output

When you build, Qt Creator creates a `build-<projectname>-<kit>-Debug/` folder next to your project. The compiled executable lives inside. Delete this folder anytime to force a clean rebuild — it'll be recreated.

> 📘 **Reference:** [Qt application structure](https://doc.qt.io/qt-6/qtwidgets-tutorials-notepad-example.html) · [Object model & QObject](https://doc.qt.io/qt-6/object.html)

---

## 6. Official Documentation Map

The Qt documentation is excellent but vast. Here are the pages that actually matter when you're starting out — bookmark these.

### Start here

| Resource                                | What it gives you                                                |
| :-------------------------------------- | :--------------------------------------------------------------- |
| [Qt 6 documentation root](https://doc.qt.io/qt-6/) | Top-level index of everything — module list, class reference     |
| [Getting Started with Qt](https://doc.qt.io/qt-6/gettingstarted.html) | Official onboarding — read this once cover-to-cover  |
| [Qt Creator Manual](https://doc.qt.io/qtcreator/) | The IDE's full manual — searchable, well-indexed                  |
| [Qt Examples and Tutorials](https://doc.qt.io/qt-6/qtexamplesandtutorials.html) | Hundreds of working sample apps          |

### Core concepts

| Resource                                | When to read it                                                  |
| :-------------------------------------- | :--------------------------------------------------------------- |
| [Object Model](https://doc.qt.io/qt-6/object.html)             | Understanding parent-child ownership, QObject basics |
| [Signals & Slots](https://doc.qt.io/qt-6/signalsandslots.html) | The mechanism that ties Qt apps together (Module 03) |
| [Qt Properties](https://doc.qt.io/qt-6/properties.html)        | `Q_PROPERTY`, used heavily in custom widgets         |
| [Event System](https://doc.qt.io/qt-6/eventsandfilters.html)   | How Qt delivers mouse/keyboard/paint events          |

### Widgets reference

| Resource                                | What it gives you                                                |
| :-------------------------------------- | :--------------------------------------------------------------- |
| [Qt Widgets index](https://doc.qt.io/qt-6/qtwidgets-index.html) | Overview of the widgets module                  |
| [Widget Classes](https://doc.qt.io/qt-6/widget-classes.html)    | List of every standard widget                   |
| [Layout Management](https://doc.qt.io/qt-6/layout.html)         | How to compose UIs with box & grid layouts      |
| [Style Sheets Reference](https://doc.qt.io/qt-6/stylesheet-reference.html) | All QSS selectors and properties     |

### Build system

| Resource                                | What it gives you                                                |
| :-------------------------------------- | :--------------------------------------------------------------- |
| [qmake Manual](https://doc.qt.io/qt-6/qmake-manual.html)        | Everything about `.pro` files                   |
| [CMake with Qt](https://doc.qt.io/qt-6/cmake-manual.html)       | Modern alternative to qmake                     |

### Automotive resources

| Resource                                | What it gives you                                                |
| :-------------------------------------- | :--------------------------------------------------------------- |
| [Qt for Automotive](https://www.qt.io/product/automotive)       | Commercial automotive offering overview         |
| [Qt Auto](https://www.qt.io/qt-auto)                            | Newer initiative for next-gen automotive platforms |
| [Qt Wiki — Qt for Automotive](https://wiki.qt.io/Qt_for_Automotive) | Community wiki, links to demos and projects |
| [Qt Safe Renderer](https://doc.qt.io/QtSafeRenderer/)           | Safety-certified rendering for ISO 26262        |

### Community

| Resource                                | What it's good for                                               |
| :-------------------------------------- | :--------------------------------------------------------------- |
| [Qt Forum](https://forum.qt.io/)        | Active community Q&A — most beginner questions are already answered here |
| [Qt Wiki](https://wiki.qt.io/)          | Tips, tricks, less-formal articles                               |
| [KDAB Blog](https://www.kdab.com/blog/) | Qt consultancy with deep technical posts                         |

---

## 7. Common Errors & Fixes

Things that bite every Qt newcomer. If you hit one of these, don't panic.

### "qmake: command not found" *(terminal)*

Your shell doesn't know where Qt installed `qmake`. Fix by adding Qt's bin folder to your `PATH`:

- **Windows:** add `C:\Qt\6.5.x\msvc2019_64\bin` (adjust to your version) to System Environment Variables → PATH
- **Linux/Mac:** `export PATH="$HOME/Qt/6.5.x/gcc_64/bin:$PATH"` in your `.bashrc` or `.zshrc`

You don't need this if you only build via Qt Creator — only when running `qmake` from a terminal.

### "No suitable kit found" *(Qt Creator)*

Qt Creator hasn't connected your installed Qt version to a compiler. Fix:

1. **Edit → Preferences → Kits → Qt Versions** — add your installed Qt manually (point to `qmake.exe`)
2. **Edit → Preferences → Kits → Compilers** — make sure your compiler is detected
3. **Edit → Preferences → Kits → Kits** — create a new kit pairing them

Auto-detection usually works; manual fixup is needed only after weird installs.

### Linker error "cannot find -lQt6Charts"

You forgot to install **Qt Charts** during the installer step. Re-run the Qt Maintenance Tool, find your Qt 6 version, tick **Qt Charts**, install. Then in Qt Creator, *Build → Rebuild All*.

### Compile error "'Q_OBJECT' macro requires moc"

Either:
- The `moc` (meta-object compiler) didn't run on your header — fix by **right-clicking the project → Run qmake**, then *Build → Rebuild*
- Your class declares `Q_OBJECT` but the header isn't listed under `HEADERS +=` in the `.pro` file

### App builds but window appears blank or crashes immediately *(Linux)*

Missing system libraries that Qt needs at runtime. Install them:

```bash
sudo apt install libxcb-xinerama0 libxcb-icccm4 libxcb-image0 \
                 libxcb-keysyms1 libxcb-render-util0 libxcb-shape0
```

### Slow builds on Windows

Antivirus scanning every file Qt touches kills build speed. Add `C:\Qt\` and your projects folder to your antivirus exclusion list. Builds typically get 3–5× faster.

### "Project ERROR: Unknown module(s) in QT: charts"

Same as the Qt Charts linker error above — Qt Charts wasn't installed. Rerun the installer and tick it.

### Build works, but Designer doesn't show your custom widget

Custom widgets need to be **promoted** in Designer. Right-click a placeholder QWidget in the form → *Promote to…* → enter your class name and header file. Qt will use your class at runtime, even though Designer shows the placeholder.

---

## What's next

Once you've got Qt installed and can build & run `HelloAutoHMI`, you're ready for **[Module 02 — Qt Widgets & Layouts](../02-qt-widgets-layouts/)** *(coming soon)* where you'll start composing real automotive dashboard UIs.

---

← [Back to syllabus](../README.md)  ·  [Next module →](../02-qt-widgets-layouts/) *(coming soon)*
