# Module 03 — Signals & Slots

> The wiring layer of every Qt app. Learn how Qt's signal–slot mechanism lets a button click, a sensor update, or a CAN-bus message trigger exactly the right code — without the sender knowing or caring who's listening.

| Phase | Level | Time | Qt modules |
| --- | --- | --- | --- |
| Phase 1 — Qt Fundamentals | Beginner | 2 hours | Qt Core · Qt Widgets |

---

## Table of Contents

1. [Why Signals & Slots Matter](#1-why-signals--slots-matter)
2. [Signals & Slots in Automotive HMIs](#2-signals--slots-in-automotive-hmis)
3. [The Mechanism — QObject, MOC, and `connect()`](#3-the-mechanism--qobject-moc-and-connect)
4. [Connection Syntax — String vs Pointer-to-Member](#4-connection-syntax--string-vs-pointer-to-member)
5. [Defining Your Own Signals and Slots](#5-defining-your-own-signals-and-slots)
6. [Connection Types — Direct, Queued, Auto](#6-connection-types--direct-queued-auto)
7. [Lambdas as Slots](#7-lambdas-as-slots)
8. [Disconnecting and Signal Blocking](#8-disconnecting-and-signal-blocking)
9. [Official Documentation Map](#9-official-documentation-map)
10. [Reference Videos](#10-reference-videos)
11. [Common Errors & Fixes](#11-common-errors--fixes)

---

## 1. Why Signals & Slots Matter

A car's dashboard is full of things that *happen*: the driver presses a button, the speed sensor ticks every 50 ms, the CAN bus delivers a new RPM frame, the parking-brake state flips. Each of these events needs to update one or more widgets on screen — and often the same event needs to update several at once.

The naive way to wire this up is *callbacks*: the sender keeps a list of function pointers, calls each one when something happens. It works, but it's fragile — the sender has to know the exact signature of every receiver, you crash when a receiver is destroyed before the sender, and threading turns it into a nightmare.

Qt's **signals & slots** is a type-safe, loosely-coupled callback system built into the language (via the **MOC** preprocessor). A class emits a **signal** when something noteworthy happens. Any number of other objects can have **slots** — ordinary member functions — connected to that signal. When the signal fires, every connected slot runs. The sender doesn't know who's listening; the receiver doesn't know who's sending. Add a thread boundary and Qt automatically queues the call for you.

Get signals & slots right and your HMI code becomes a graph of small, independently testable objects. Get them wrong and you'll spend a week debugging why your speed label doesn't update.

---

## 2. Signals & Slots in Automotive HMIs

Almost every visible behaviour in an automotive HMI is a signal–slot connection somewhere:

| Event in the car | Signal emitted by | Slot reacts on |
| --- | --- | --- |
| Driver taps "Start Engine" | `QPushButton::clicked` | `VehicleController::startEngine()` |
| Speed sensor updates @ 20 Hz | `SpeedSensor::speedChanged(int)` | `QLabel::setText`, `Speedometer::setValue` |
| Fuel level drops below 10 % | `FuelModel::lowFuelWarning()` | `WarningPanel::showLowFuelIcon()` |
| Turn signal stalk pulled | `StalkInput::leftSignalActivated` | `QTimer::start` for blink animation |
| CAN frame `0x180` arrives | `CanBus::frameReceived(QCanBusFrame)` | `RpmModel::onFrame()` |
| Drive mode changed to Sport | `DriveModeSelector::modeChanged(Mode)` | Cluster restyle, suspension API, sound profile |
| Bluetooth phone connects | `PhoneService::connected(QString name)` | `MediaPanel::showSource`, `ContactList::reload` |
| Climate fan + button held | `QPushButton::pressed` + `QTimer::timeout` | `ClimateModel::increaseFan()` repeatedly |

Notice the pattern: one event can fan out to many widgets, and one widget often listens to several signals. This is exactly the "many-to-many" wiring problem signals & slots was designed for.

The structural payoff: the **model** classes (`SpeedSensor`, `FuelModel`, `CanBus`) only `emit` signals — they don't touch any widget. The **view** classes (labels, gauges, panels) only have slots — they don't poll any sensor. The two halves are wired together in one place at startup. You can swap the simulator for a real CAN bus without changing a single line of UI code.

---

## 3. The Mechanism — QObject, MOC, and `connect()`

Three things make signals & slots work:

1. **Inherit from `QObject`.** Every class that wants to send or receive signals must inherit `QObject` (or a subclass — `QWidget` already does).
2. **Add the `Q_OBJECT` macro.** First line inside the class body. This is what tells the build system "run MOC on me."
3. **Call `connect()`** at runtime to wire a signal to a slot.

The **MOC** (Meta-Object Compiler) is a Qt-specific preprocessor that runs before your C++ compiler. It scans for `Q_OBJECT` and generates a `moc_*.cpp` file containing the introspection tables that make `connect()` work. CMake and qmake run MOC automatically — you almost never invoke it by hand, but knowing it exists explains a lot of "weird build errors" (see §11).

A minimal sender / receiver pair:

    // speedsensor.h
    class SpeedSensor : public QObject {
        Q_OBJECT
    public:
        explicit SpeedSensor(QObject *parent = nullptr);
    signals:
        void speedChanged(int kmh);   // declaration only — MOC implements it
    };

    // dashboard.h
    class Dashboard : public QWidget {
        Q_OBJECT
    public:
        explicit Dashboard(QWidget *parent = nullptr);
    public slots:
        void onSpeedChanged(int kmh);
    };

    // main.cpp — wiring
    SpeedSensor sensor;
    Dashboard   dash;
    QObject::connect(&sensor, &SpeedSensor::speedChanged,
                     &dash,   &Dashboard::onSpeedChanged);

`signals:` and `slots:` are MOC keywords — they expand to nothing for the compiler but tell MOC how to generate the meta-object. You **never** write the body of a signal; you just `emit speedChanged(120);` and Qt's generated code calls every connected slot for you.

> 📘 **Reference:** [Signals & Slots (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/signalsandslots.html) · [QObject (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qobject.html) · [The Meta-Object System (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/metaobjects.html)

---

## 4. Connection Syntax — String vs Pointer-to-Member

Qt offers two `connect()` syntaxes. Both still work in Qt 6, but **always prefer the new one.**

### Old (Qt 4 style — string-based)

    connect(button, SIGNAL(clicked()),
            this,   SLOT(onStartClicked()));

Strings are checked at runtime. A typo (`cliked()`) compiles fine and silently fails at runtime — your button does nothing and there's no error.

### New (Qt 5/6 style — pointer-to-member)

    connect(button, &QPushButton::clicked,
            this,   &Dashboard::onStartClicked);

The compiler verifies that the signal and slot exist and that their signatures are compatible. Typos are caught at build time. Refactoring tools rename them correctly.

Use the new syntax everywhere unless you're maintaining a Qt 4 codebase.

> 📘 **Reference:** [Differences between String-Based and Functor-Based Connections (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/signalsandslots-syntaxes.html)

---

## 5. Defining Your Own Signals and Slots

For automotive work, most of the signals you'll connect are ones *you* defined on your model classes. The pattern is small and identical every time.

    // fuelmodel.h
    class FuelModel : public QObject {
        Q_OBJECT
    public:
        explicit FuelModel(QObject *parent = nullptr);

        int level() const { return m_level; }

    public slots:
        void setLevel(int percent);

    signals:
        void levelChanged(int percent);
        void lowFuelWarning();          // fires once when crossing 10 %

    private:
        int m_level = 100;
    };

    // fuelmodel.cpp
    void FuelModel::setLevel(int percent) {
        if (percent == m_level) return;       // gate redundant updates
        const bool crossedLow = (m_level >= 10 && percent < 10);
        m_level = percent;
        emit levelChanged(percent);
        if (crossedLow) emit lowFuelWarning();
    }

Three things worth noting:

- **Gate redundant updates.** Compare against the previous value before emitting. A sensor that updates 50 times per second with the same number will redraw your gauge 50 times for nothing.
- **`emit` is just a marker.** It expands to nothing — `emit foo();` and `foo();` compile to identical code. It's there so a human reading the source knows "this call leaves the object and goes to subscribers."
- **Slots are normal member functions.** You can call `fuelModel.setLevel(40)` directly from code. The `public slots:` label just additionally exposes it to `connect()`.

---

## 6. Connection Types — Direct, Queued, Auto

When a signal is emitted, *when* does the slot actually run? That's the **connection type** — an optional fifth argument to `connect()`.

| Type | Behaviour |
| --- | --- |
| `Qt::DirectConnection` | Slot is called immediately, in the emitter's thread. Like a normal function call. |
| `Qt::QueuedConnection` | Event is posted to the receiver's thread event loop. Slot runs later, on the receiver's thread. |
| `Qt::BlockingQueuedConnection` | Like `Queued`, but the emitting thread waits until the slot finishes. Never use within the same thread — deadlocks. |
| `Qt::AutoConnection` | **Default.** Direct if sender and receiver share a thread, Queued otherwise. |

For 90 % of HMI code the default `AutoConnection` is exactly right. The two times you'll override it:

- **`DirectConnection` when you need the slot to run *before* the next line of the emitter.** Rare.
- **`QueuedConnection` to break recursion** — slot A emits signal B which triggers slot A again. Queueing the second call lets the stack unwind first.

A CAN-bus reader running on a worker thread:

    // CanWorker lives on a QThread; Dashboard lives on the GUI thread.
    // AutoConnection picks QueuedConnection here automatically.
    connect(canWorker, &CanWorker::rpmFrame,
            dashboard, &Dashboard::onRpm);

The slot will safely run on the GUI thread, so it can touch widgets without a crash.

> 📘 **Reference:** [Qt::ConnectionType (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qt.html#ConnectionType) · [Threads and QObjects (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/threads-qobject.html)

---

## 7. Lambdas as Slots

Since Qt 5, the third argument to `connect()` can be a lambda — no need to declare a `public slots:` function just to handle a click.

    connect(startButton, &QPushButton::clicked, this, [this]() {
        statusLabel->setText("Engine running");
        engine->start();
    });

Two non-negotiable rules:

1. **Always pass a context object as the third argument** (`this` above). It tells Qt which object's lifetime governs the connection — when `this` is destroyed, the connection is auto-disconnected. Without a context, a lambda captured by reference can outlive its captures and crash.
2. **Capture by value, not by reference**, for anything that might go out of scope.

Bad — no context, captures by reference:

    connect(button, &QPushButton::clicked, [&]() {  // ⚠ no context, refs
        someLocal.doSomething();
    });

Good:

    connect(button, &QPushButton::clicked, this, [this, copy = someLocal]() {
        copy.doSomething();
    });

> 📘 **Reference:** [Signals & Slots — Using Lambdas (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/signalsandslots.html#using-qt-with-3rd-party-signals-and-slots)

---

## 8. Disconnecting and Signal Blocking

You rarely need to disconnect manually — when either the sender or the receiver is destroyed, Qt cleans up automatically. The cases where you do:

    // Disconnect one specific connection
    disconnect(sensor, &SpeedSensor::speedChanged,
               dash,   &Dashboard::onSpeedChanged);

    // Disconnect everything from sensor's speedChanged signal
    disconnect(sensor, &SpeedSensor::speedChanged, nullptr, nullptr);

A more common need is **temporarily silencing** a widget while you set its value programmatically — otherwise its `valueChanged` signal fires, your slot runs, and you get a feedback loop:

    slider->blockSignals(true);
    slider->setValue(newVolume);
    slider->blockSignals(false);

Even cleaner, with RAII:

    QSignalBlocker blocker(slider);
    slider->setValue(newVolume);   // unblocked automatically when blocker goes out of scope

> 📘 **Reference:** [QObject::disconnect (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qobject.html#disconnect) · [QSignalBlocker (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qsignalblocker.html)

---

## 9. Official Documentation Map

Bookmark these — every link is the **Qt 6.1** version (same pages exist under `doc.qt.io/qt-5/...` for Qt 5.15).

### Core mechanism

| Resource | What it gives you |
| --- | --- |
| [Signals & Slots](https://doc.qt.io/archives/qt-6.1/signalsandslots.html) | The canonical reference — read once cover-to-cover |
| [QObject](https://doc.qt.io/archives/qt-6.1/qobject.html) | Base class for every signal/slot-capable type |
| [The Meta-Object System](https://doc.qt.io/archives/qt-6.1/metaobjects.html) | What MOC does and why `Q_OBJECT` exists |
| [Connection Syntaxes](https://doc.qt.io/archives/qt-6.1/signalsandslots-syntaxes.html) | Old string vs new pointer-to-member |

### Threading

| Resource | What it gives you |
| --- | --- |
| [Threads and QObjects](https://doc.qt.io/archives/qt-6.1/threads-qobject.html) | Thread affinity, connection types explained |
| [Qt::ConnectionType](https://doc.qt.io/archives/qt-6.1/qt.html#ConnectionType) | Reference for the five connection-type enums |

### Utilities

| Resource | What it gives you |
| --- | --- |
| [QSignalBlocker](https://doc.qt.io/archives/qt-6.1/qsignalblocker.html) | RAII guard for temporary signal silencing |
| [QMetaObject::Connection](https://doc.qt.io/archives/qt-6.1/qmetaobject-connection.html) | Handle returned by `connect()` for manual disconnect |

---

## 10. Reference Videos

Curated picks. Watch the first two before opening the sample project in this folder.

| Video | Length | Why watch |
| --- | --- | --- |
| [Qt Signals & Slots — KDAB Introduction](https://www.youtube.com/watch?v=LSwzcWj4SQM) | ~12 min | Official KDAB walk-through of the mechanism |
| [Qt Tutorial — Signals and Slots](https://www.youtube.com/watch?v=g0Oa0qAQ4Tg) | ~15 min | Beginner-friendly first connect, with a button + label |
| [Signals & Slots in Qt — Crash Course](https://www.youtube.com/watch?v=jZkpfwGOhT0) | ~18 min | Both syntaxes, custom signals, lambdas |
| [Qt Threads — Signals Across Threads](https://www.youtube.com/watch?v=eRZgenIxX_g) | ~20 min | When and why to think about connection types |
| [Custom Signals & Slots Step-by-Step](https://www.youtube.com/watch?v=h_DVfsD9PKI) | ~15 min | Define your own signals on a model class |

---

## 11. Common Errors & Fixes

The things that bite every Qt newcomer when wiring signals and slots.

### `undefined reference to vtable for MyClass`

Classic. You added `Q_OBJECT` to a class but the build system didn't run MOC on it. **Fix:**

- Make sure the header is listed in your build files (`HEADERS +=` in qmake, or in the `target_sources` / `add_executable` argument list in CMake — Qt 6 CMake auto-discovers them when `set(CMAKE_AUTOMOC ON)` is set).
- Re-run `qmake` / `cmake` and **fully rebuild** — a partial build won't regenerate MOC output.
- Confirm the class inherits `QObject` (directly or transitively).

### `QObject::connect: No such signal MyClass::someSignal()`

Runtime warning when using the old string-based syntax — Qt couldn't find the signal at lookup time. **Fix:** switch to the new pointer-to-member syntax (`&MyClass::someSignal`). The compiler will then tell you exactly what's wrong.

### Signal emitted but slot never runs

Walk down this checklist:

1. Is the slot declared under `public slots:` (or is it a normal member function — which also works with the new syntax)?
2. Does the sender's class have `Q_OBJECT`? Does the receiver's?
3. Is the connect call actually being reached at startup? Put a `qDebug()` next to it.
4. Are the signal and slot parameter types an exact match? `int` and `qint32` are usually fine, but `QString` vs `const char*` will fail silently.
5. Is the receiver still alive when the signal fires? Use a context object in lambda connects.

### Slot runs twice (or N times) per emit

You called `connect()` multiple times. Qt allows duplicate connections by design — each one runs the slot. **Fix:** either guard with `Qt::UniqueConnection` as the fifth `connect()` argument, or move the `connect()` out of any function that gets called more than once (constructors are usually safe; `showEvent`, `paintEvent`, slot bodies are not).

    connect(sender, &Sender::sig, this, &Receiver::slot, Qt::UniqueConnection);

### App crashes when a signal fires

Almost always a dangling pointer — the receiver was deleted but the connection wasn't. **Fix:**

- For raw lambdas, always pass a context object as the third `connect()` argument.
- For QObjects, give them a parent so Qt cleans them up in order.
- If you stored a pointer that might dangle, wrap it in `QPointer<T>` — it auto-nulls on destruction.

### `Q_OBJECT` macro added but build fails with weird errors

You added `Q_OBJECT` to a class defined inside a `.cpp` file (not a header). MOC by default only scans headers. **Fix:** move the class to its own `.h` file, or `#include "myfile.moc"` at the bottom of the `.cpp` and ensure `CMAKE_AUTOMOC` / `qmake`'s `MOC_DIR` is set up.

### Cross-thread signal works but slot touches widgets and crashes

You connected with `Qt::DirectConnection` from a worker thread to a GUI slot — the slot then runs on the worker thread and Qt forbids GUI access off the main thread. **Fix:** use `Qt::AutoConnection` (the default) or explicitly `Qt::QueuedConnection`. Don't override to `Direct` unless you've thought through threading.

### Setting a value triggers an infinite loop

Your slot calls `setValue()` on a widget, which emits `valueChanged`, which calls your slot, ... **Fix:** wrap the programmatic set in a `QSignalBlocker`, or check inside the slot whether the new value actually differs from the current one before reacting.

    void Panel::onModelChanged(int v) {
        QSignalBlocker b(slider);
        slider->setValue(v);
    }

### Build error: `'signals' was not declared in this scope`

You used `signals:` inside a class that doesn't inherit `QObject`, or you forgot `#include <QObject>`. Signals/slots only exist on `QObject` subclasses.

### Custom type in signal: `QObject::connect: Cannot queue arguments of type 'MyType'`

Queued connections need to be able to copy the argument across threads. **Fix:** register the type at startup:

    qRegisterMetaType<MyType>("MyType");

Add a copy constructor and a default constructor to `MyType` while you're at it.

---

## What's next

Once signals & slots feel natural, you can wire any event in the car to any reaction on the screen. Up next is **[Module 04 — Styling & QSS](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/04-styling-and-qss)** *(coming soon)* — turning the default grey Qt look into a polished automotive interface using Qt Style Sheets.

A worked sample project and exercise solutions will live in subfolders next to this README — refer to them after you've absorbed the concepts above.

---

← [Previous module](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/02-qt-widgets-and-layouts) · [Back to syllabus](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/README.md) · [Next module →](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/04-styling-and-qss) *(coming soon)*
