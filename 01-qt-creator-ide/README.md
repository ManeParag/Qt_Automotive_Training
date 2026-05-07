# Module 01 — Qt Creator IDE Navigation

> Tour the IDE, build your first Qt app, learn the kit/build/run cycle.

| Phase | Level | Time | Qt modules |
| :---- | :---- | :--- | :--------- |
| Phase 1 — Qt Fundamentals | Beginner | 1.5 hours | Qt Core · Qt Widgets |

---

## Concepts you'll cover

- Qt Creator interface — Welcome, Edit, Debug, Projects, Help modes
- Projects pane: the file tree, right-click *Add New…* / *Add Existing…*
- Kits — what they are, why a project needs one, switching between MSVC/MinGW/GCC
- The .pro file — Qt's project description (we'll use qmake throughout)
- Build, Run, Debug shortcuts: `Ctrl+B`, `Ctrl+R`, `F5`
- Designer mode: drag-drop widgets onto a `.ui` form
- The Issues / Application Output / Compile Output panes

## Theory

Qt Creator is the IDE Qt ships with. Unlike VS Code or CLion you don't bolt the Qt
support on — it's first-class. When you open a `.pro` file, Qt Creator parses it,
configures the active **Kit** (compiler + Qt version + debugger), and gives you a
project tree, a code editor with auto-completion for Qt classes, a visual UI
designer, and an integrated debugger.

A typical Qt project tree looks like this:

```
HelloAutoHMI.pro      ← qmake project file (the "manifest")
main.cpp              ← entry point: creates QApplication, shows main window
mainwindow.h          ← declares your main window class
mainwindow.cpp        ← implements it
mainwindow.ui         ← visual form (XML, edited in Designer)
```

`main.cpp` is small on purpose — it just bootstraps `QApplication` and the main
window. All your real UI work lives in `mainwindow.cpp` (and the `.ui` form).


## Your turn

Open `HelloAutoHMI/HelloAutoHMI.pro` in Qt Creator and complete the steps below.

1. Build and run the starter project as-is. You should see a window titled
   *Hello AutoHMI* with a single **Start Engine** button.
2. Change the window title to **AutoHMI — <your name>**.
3. Add a `QStatusBar` message via `statusBar()->showMessage(...)` that says
   *"Engine off"* on startup.
4. When the **Start Engine** button is clicked, change the status-bar message
   to *"Engine running"* and the button text to **Stop Engine**. Clicking
   again toggles it back.


## Submission checklist

- [ ] Code builds clean with no warnings
- [ ] App runs without crashing
- [ ] All steps under *Your turn* above implemented
- [ ] Screenshot saved as `screenshot.png` in `exercises/`
- [ ] PR opened from branch `module-01/<your-name>`

## Further reading

- [Qt Creator Manual](https://doc.qt.io/qtcreator/)
- [Getting started with Qt](https://doc.qt.io/qt-6/gettingstarted.html)
- [qmake Manual](https://doc.qt.io/qt-6/qmake-manual.html)

---

← [Back to syllabus](../README.md)  ·  [Next module →](../02-qt-widgets-layouts)
