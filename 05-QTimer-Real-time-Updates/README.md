# Module 05 — QTimer & Real-time Updates

> The heartbeat of every live HMI. Learn how `QTimer` drives animations, polls sensors, and refreshes the cluster at 30 fps — and when you need a worker thread instead.

| Phase | Level | Time | Qt modules |
| --- | --- | --- | --- |
| Phase 2 — Qt Fundamentals | Intermediate | 3 hours | Qt Core · Qt Widgets |

---

## Table of Contents

1. [Why QTimer Matters](#1-why-qtimer-matters)
2. [QTimer in Automotive HMIs](#2-qtimer-in-automotive-hmis)
3. [The Event Loop — How QTimer Actually Works](#3-the-event-loop--how-qtimer-actually-works)
4. [Single-shot vs Repeating Timers](#4-single-shot-vs-repeating-timers)
5. [Connecting a Timer to a Slot](#5-connecting-a-timer-to-a-slot)
6. [Timing Precision and Multiple Timers](#6-timing-precision-and-multiple-timers)
7. [When QTimer Isn't Enough — Threading Basics](#7-when-qtimer-isnt-enough--threading-basics)
8. [QThread + `moveToThread` Pattern](#8-qthread--movetothread-pattern)
9. [Official Documentation Map](#9-official-documentation-map)
10. [Reference Videos](#10-reference-videos)
11. [Common Errors & Fixes](#11-common-errors--fixes)

---

## 1. Why QTimer Matters

An HMI is not a static drawing. The speed changes, the RPM needle sweeps, the turn-signal arrow blinks, the fuel bar slowly drops, the warning icon pulses. Every one of these is a value that has to be re-read, re-calculated, or re-drawn *over time*.

You could try to handle this with raw `sleep()` calls in a loop — but `sleep()` blocks everything. The button stops responding, the screen freezes, the user sees a hang. Qt's answer is **`QTimer`**: a small object that asks the event loop to call your slot at a fixed interval, without blocking anything else.

`QTimer` is how you say:

- *"Every 100 ms, read the speed sensor and update the speedometer."*
- *"Every 500 ms, toggle the turn-signal arrow visibility."*
- *"Once, after 3 seconds, hide this welcome message."*
- *"Every 16 ms (≈60 fps), advance the gauge needle animation by one step."*

Combine `QTimer` with the `update()` call you learned in Module 04 and you've built an animated HMI. Combine it with the signals & slots from Module 03 and you've built a data pipeline. This module is small in code volume but enormous in payoff.

---

## 2. QTimer in Automotive HMIs

Almost every dynamic behaviour in a cluster is a `QTimer` tick somewhere:

| HMI behaviour | Timer interval | What runs each tick |
| --- | --- | --- |
| Speedometer animation | 16 ms (≈60 fps) | Advance needle angle toward target, `update()` |
| RPM gauge refresh | 50 ms (20 Hz) | Read latest CAN value, `setRpm()` |
| Turn-signal blink | 500 ms | Toggle arrow visibility |
| Warning icon pulse | 50 ms | Change alpha 0 → 255 → 0 in a sine wave |
| Fuel level poll | 1000 ms | Read tank level, update fuel bar |
| Trip-time counter | 1000 ms | Increment elapsed time, refresh label |
| Welcome-screen auto-dismiss | 3000 ms (single-shot) | Hide splash, show cluster |
| Service-reminder check | 60000 ms | Check odometer vs service threshold |

Notice the spread of intervals — from 16 ms for smooth animation down to once per minute for background checks. `QTimer` handles all of them with the same API.

In a real cluster you'll have **5–15 timers running simultaneously**. They don't interfere with each other; Qt's event loop multiplexes them onto a single thread.

---

## 3. The Event Loop — How QTimer Actually Works

Before you use `QTimer`, you need to understand what it depends on: the **event loop**.

Every Qt GUI app starts an event loop the moment you call `app.exec()` in `main()`. The event loop is a function that repeatedly:

1. Checks the OS for input events (mouse, keyboard, touch).
2. Checks for queued signal/slot calls.
3. Checks for **expired timers**.
4. Calls the matching handler for whichever it found.
5. Loops back.

When you `start()` a `QTimer`, Qt registers it with the event loop. On every loop iteration, the loop checks "has enough time passed since this timer was started?" If yes, it emits the timer's `timeout()` signal — which calls every slot you connected.

Two consequences fall out of this:

- **A timer needs a running event loop.** If you block the main thread (with a long calculation, a `sleep()`, a network call), the event loop can't iterate, and your timer doesn't fire. The HMI freezes.
- **Timer precision depends on event-loop health.** A heavily loaded GUI thread will deliver a 100 ms timer at 110 ms, 120 ms, sometimes more. For animation and sensor polling that's fine. For sub-millisecond timing it isn't — you'd need a real-time OS feature, not `QTimer`.

> 📘 **Reference:** [Event System (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/eventsandfilters.html) · [QCoreApplication::exec (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qcoreapplication.html#exec)

---

## 4. Single-shot vs Repeating Timers

`QTimer` does two distinct jobs.

### Repeating — fires forever until stopped

The default. Used for animation loops and sensor polling.

    QTimer *clockTimer = new QTimer(this);
    clockTimer->setInterval(1000);        // 1 second
    connect(clockTimer, &QTimer::timeout, this, &Dashboard::updateClock);
    clockTimer->start();

    // Later, when leaving the screen:
    clockTimer->stop();

Always parent the timer (`new QTimer(this)`) so it's destroyed when its owner is — otherwise it keeps firing after you'd expect.

### Single-shot — fires once after a delay

Used for timeouts, splash screens, delayed actions. No need to construct a `QTimer` at all — use the static helper:

    // Hide the welcome message after 3 seconds
    QTimer::singleShot(3000, this, &Dashboard::hideWelcome);

    // Or with a lambda
    QTimer::singleShot(500, this, [this]() {
        statusLabel->setText("Ready");
    });

`singleShot` creates and destroys the timer for you. Cleaner than allocating a `QTimer` you'll only use once.

> 📘 **Reference:** [QTimer (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qtimer.html) · [QTimer::singleShot (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qtimer.html#singleShot)

---

## 5. Connecting a Timer to a Slot

A timer is just a signal source. The pattern is identical to anything else you've connected:

    // In Dashboard's constructor
    m_timer = new QTimer(this);
    m_timer->setInterval(50);              // 20 Hz refresh

    connect(m_timer, &QTimer::timeout,
            this,    &Dashboard::refreshGauges);

    m_timer->start();

The slot itself is an ordinary member function. For a cluster it usually reads the latest values and pushes them into the widgets:

    void Dashboard::refreshGauges() {
        const int kmh = m_sensor->latestSpeed();
        const int rpm = m_sensor->latestRpm();

        speedometer->setSpeed(kmh);    // calls update() internally — Module 04
        tachometer->setRpm(rpm);
        speedLabel->setText(QString::number(kmh) + " km/h");
    }

That's a full animated cluster in under 10 lines of glue code. The timer fires 20 times a second, your slot reads sensor values and pushes them into widgets, the widgets repaint themselves.

### Animating a needle toward a target

For smooth animation rather than instant snapping, store a *target* speed and let the timer ease the displayed value toward it:

    void Speedometer::onTimerTick() {
        const int delta = m_target - m_current;
        if (delta == 0) return;
        m_current += (delta > 0) ? 1 : -1;   // step by 1 km/h per tick
        update();
    }

At 16 ms intervals this gives a smooth sweep. For real animation curves Qt offers `QPropertyAnimation` — covered in a later module — but a hand-rolled timer is often all you need.

---

## 6. Timing Precision and Multiple Timers

Two practical points worth knowing before you build a real cluster.

### Precision

`QTimer` has three precision modes, settable via `setTimerType()`:

| Mode | Accuracy | Use for |
| --- | --- | --- |
| `Qt::CoarseTimer` (default) | ±5 % | UI animation, sensor polling — what you want 95 % of the time |
| `Qt::PreciseTimer` | ±1 ms where the OS allows | Sub-second timing-critical work |
| `Qt::VeryCoarseTimer` | full seconds | Long delays where exact timing doesn't matter |

The default `CoarseTimer` lets Qt batch timers that fire near the same time, saving battery and CPU. Only bump to `PreciseTimer` if you actually see jitter affecting the visuals.

### Multiple timers

You can have many timers running at once. They share the GUI thread's event loop, so a slow slot on one timer can delay another. Keep slot bodies short — read values, update widgets, return.

    // Bad — heavy work blocks every other timer
    void Dashboard::refreshGauges() {
        QImage img(2000, 2000, QImage::Format_RGB32);  // big allocation
        renderHeavyChart(img);                          // takes 200 ms
        // ... other gauges now late
    }

If a slot has heavy work to do, that's the signal to move it off the GUI thread — which is §7.

---

## 7. When QTimer Isn't Enough — Threading Basics

`QTimer` works because the event loop is iterating. If your slot takes 30 ms to run and your timer fires every 50 ms, you've used 60 % of the event loop's time and everything else (button responses, painting, other timers) gets squeezed. Push slot work much past that and the HMI feels sluggish.

Three situations push you off the GUI thread entirely:

- **Blocking I/O** — reading a real CAN bus, opening a serial port, fetching a tile from a navigation server. These can block for tens or hundreds of milliseconds.
- **Heavy computation** — image processing, FFTs on audio data, large file parsing.
- **Periodic work that doesn't need the GUI thread** — a sensor sampler running at 1 kHz, a logger writing CAN frames to disk.

The answer is a **worker thread**: a second OS thread running its own event loop, with its own QTimer if needed. The worker emits signals; the GUI thread's slots react. You've already seen this pattern in Module 03's connection-type table — now we'll wire it up.

### The one-line summary

In Qt 5/6 you do **not** subclass `QThread`. You write a worker class that inherits `QObject`, then **move** it onto a `QThread`. This is the modern, recommended pattern.

---

## 8. QThread + `moveToThread` Pattern

The worker reads a sensor 20 times per second on its own thread and emits a signal. The GUI thread's slot receives the signal — automatically queued across the thread boundary — and updates the widget.

### The worker (header)

    // sensorworker.h
    class SensorWorker : public QObject {
        Q_OBJECT
    public:
        explicit SensorWorker(QObject *parent = nullptr);

    public slots:
        void start();        // begin sampling
        void stop();         // stop sampling

    signals:
        void speedSampled(int kmh);

    private slots:
        void doSample();     // called by the worker's own QTimer

    private:
        QTimer *m_timer = nullptr;
    };

### The worker (implementation)

    // sensorworker.cpp
    SensorWorker::SensorWorker(QObject *parent) : QObject(parent) {}

    void SensorWorker::start() {
        m_timer = new QTimer(this);
        m_timer->setInterval(50);                   // 20 Hz
        connect(m_timer, &QTimer::timeout,
                this,   &SensorWorker::doSample);
        m_timer->start();
    }

    void SensorWorker::stop() {
        if (m_timer) m_timer->stop();
    }

    void SensorWorker::doSample() {
        // Read from real CAN / serial / shared memory here.
        // For now, simulated:
        const int kmh = readSpeedFromSensor();
        emit speedSampled(kmh);
    }

### Wiring it up on the GUI thread

    // dashboard.cpp constructor
    auto *thread = new QThread(this);
    auto *worker = new SensorWorker;                // no parent — we move it
    worker->moveToThread(thread);

    // When the thread starts, ask the worker to begin sampling
    connect(thread, &QThread::started,
            worker, &SensorWorker::start);

    // The worker's signal — fires on the worker thread.
    // AutoConnection automatically becomes QueuedConnection across threads,
    // so onSpeedSampled runs on the GUI thread safely.
    connect(worker, &SensorWorker::speedSampled,
            this,   &Dashboard::onSpeedSampled);

    // Clean shutdown when the dashboard is destroyed
    connect(this, &Dashboard::destroyed, worker, &SensorWorker::stop);
    connect(this, &Dashboard::destroyed, thread, &QThread::quit);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();

### The GUI-thread slot

    void Dashboard::onSpeedSampled(int kmh) {
        speedometer->setSpeed(kmh);   // safe — we're on the GUI thread
    }

### Two rules to remember

1. **Never touch widgets from a worker thread.** Widget calls are GUI-thread only. The worker emits signals; the slot on the GUI thread is what touches the widget.
2. **`moveToThread` moves the object, not its children.** Anything the worker creates *inside* a slot that runs on the worker thread (like the `QTimer` in `start()` above) will be created on that thread and behave correctly. Anything created in the constructor before `moveToThread` was called lives on whichever thread the constructor ran on.

That's the entire threading pattern. Most automotive HMI workers are 50–150 lines of code.

> 📘 **Reference:** [QThread (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qthread.html) · [Threads and QObjects (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/threads-qobject.html) · [You're doing it wrong — Qt blog on QThread subclassing](https://www.qt.io/blog/2010/06/17/youre-doing-it-wrong)

---

## 9. Official Documentation Map

Every link is the **Qt 6.1** version (same pages exist under `doc.qt.io/qt-5/...` for Qt 5.15).

### Timers

| Resource | What it gives you |
| --- | --- |
| [QTimer](https://doc.qt.io/archives/qt-6.1/qtimer.html) | Master reference — all timer functions |
| [Timers Overview](https://doc.qt.io/archives/qt-6.1/timers.html) | When to use QTimer vs QBasicTimer vs startTimer |
| [Qt::TimerType](https://doc.qt.io/archives/qt-6.1/qt.html#TimerType-enum) | Coarse vs Precise vs VeryCoarse explained |

### Event loop

| Resource | What it gives you |
| --- | --- |
| [Event System](https://doc.qt.io/archives/qt-6.1/eventsandfilters.html) | How Qt dispatches events and signals |
| [QEventLoop](https://doc.qt.io/archives/qt-6.1/qeventloop.html) | Nested loops, local event loops |

### Threading

| Resource | What it gives you |
| --- | --- |
| [QThread](https://doc.qt.io/archives/qt-6.1/qthread.html) | Thread lifecycle, signals, slots |
| [Threads and QObjects](https://doc.qt.io/archives/qt-6.1/threads-qobject.html) | Thread affinity, cross-thread signal/slot |
| [Thread Support Overview](https://doc.qt.io/archives/qt-6.1/thread-basics.html) | High-level view of concurrent options in Qt |

### Worked examples

| Resource | What it gives you |
| --- | --- |
| [Analog Clock Example](https://doc.qt.io/archives/qt-6.1/qtwidgets-widgets-analogclock-example.html) | QTimer + QPainter together — the canonical demo |
| [Producer / Consumer with Threads](https://doc.qt.io/archives/qt-6.1/qtcore-threads-producerconsumer-example.html) | Cross-thread data flow |

---

## 10. Reference Videos

| Video | Length | Why watch |
| --- | --- | --- |
| [Qt QTimer Tutorial — Basics](https://www.youtube.com/watch?v=H-Cs2-jjyA8) | ~12 min | First timer, single-shot vs repeating, simple slot |
| [Animating Widgets with QTimer in Qt C++](https://www.youtube.com/watch?v=HVL4Yz0oqYM) | ~18 min | Driving a custom-painted widget — connects directly to Module 04 |
| [Qt Event Loop Explained](https://www.youtube.com/watch?v=DI5BLP4SH3o) | ~15 min | What's actually happening under `app.exec()` |
| [QThread the Right Way — KDAB](https://www.youtube.com/watch?v=fQyqIO9P1ek) | ~25 min | Official KDAB explanation of `moveToThread` vs subclassing |
| [Multithreading in Qt — Worker Pattern](https://www.youtube.com/watch?v=eRZgenIxX_g) | ~20 min | Full worker setup, signals across threads |

---

## 11. Common Errors & Fixes

Things that bite every Qt newcomer when working with timers and threads.

### Timer never fires

Walk down this checklist:

1. Did you call `start()` on the timer? Constructing one isn't enough.
2. Did you connect `timeout` to a slot before calling `start()`? Connecting after is fine too, but a typo in the signal name (with the old string syntax) silently fails — use the new pointer-to-member syntax.
3. Is `QApplication::exec()` running? Without an event loop, no timer fires.
4. Is the main thread blocked by a long-running function? Add a `qDebug()` near the end of your blocking code to confirm it returns.

### Timer fires once, then never again

You used `QTimer::singleShot` instead of constructing a `QTimer` and calling `start()`. Or you called `stop()` somewhere you didn't intend. **Fix:** use a `QTimer` member variable with `start()` for repeating behaviour.

### Slot runs much later than the interval

The GUI thread is busy. Either:

- A different slot is doing heavy work — profile and offload it (see §7–8).
- You have dozens of timers all firing at once — consolidate where possible.
- You're using `Qt::CoarseTimer` (the default) and need `Qt::PreciseTimer`.

### App freezes for a few seconds, then catches up

Classic event-loop starvation. Some slot is blocking the GUI thread. **Fix:** find the slow slot (`QElapsedTimer` around suspect code), move its work to a worker thread or break it into chunks scheduled via `QTimer::singleShot(0, ...)`.

### `QObject::startTimer: Timers can only be used with threads started with QThread`

You tried to use a timer on a thread that has no event loop — usually a raw `std::thread` or a `QThread` you subclassed and forgot to call `exec()` inside `run()`. **Fix:** use the `moveToThread` pattern from §8 — a default `QThread` runs its own event loop automatically.

### `QObject: Cannot create children for a parent that is in a different thread`

You tried to create a child `QObject` (e.g. another `QTimer`) on the GUI thread but parent it to an object that lives on a worker thread. **Fix:** create child objects from a slot that runs *on the worker thread* — like inside `SensorWorker::start()` above, never inside the constructor that ran on the GUI thread.

### Crash when widget closes — "QWidget: Must construct a QApplication before a QWidget"

You're touching a widget from a worker thread, or the worker is still running and emitting signals to a destroyed widget. **Fix:**

- Never touch widgets from a worker. The worker emits; the GUI slot updates the widget.
- Stop the worker thread before destroying the widget — call `thread->quit(); thread->wait();` in the destructor.

### `QTimer::start: Timers cannot be started from another thread`

You tried to `start()` a timer from a thread different from the one the timer lives on. **Fix:** start the timer from a slot that runs on the same thread the timer is parented to — typically the worker's own `start()` slot, invoked via signal.

### Cross-thread signal works but the GUI doesn't update

You connected with `Qt::DirectConnection` and the slot ran on the worker thread, which means widget calls were ignored or crashed silently. **Fix:** leave the connection type as default (`Qt::AutoConnection`) — Qt picks `QueuedConnection` automatically across threads.

### Custom type in a queued signal: "Cannot queue arguments of type 'MyType'"

Queued connections need the argument type registered with Qt's meta-object system. **Fix:** at startup, before any cross-thread connect:

    qRegisterMetaType<MyType>("MyType");

### Build error: `'QTimer' was not declared in this scope`

Missing include. Add `#include <QTimer>` (and `#include <QThread>` if you're threading). Each class is its own header.

---

## What's next

You can now drive animations, refresh data at fixed rates, and offload slow work to background threads. Next you'll learn to **persist** that data — log trip statistics, save user preferences, replay recorded CAN sessions — in **[Module 06 — File I/O & Data Logging](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/06-file-io-and-data-logging)** *(coming soon)*.

A worked sample project — an animated speedometer driven by a worker-thread sensor simulator — will live in a subfolder next to this README.

---

← [Previous module](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/04-qpainter-custom-drawing) · [Back to syllabus](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/README.md) · [Next module →](https://github.com/ManeParag/Qt_Automotive_Training/blob/main/06-file-io-and-data-logging) *(coming soon)*
