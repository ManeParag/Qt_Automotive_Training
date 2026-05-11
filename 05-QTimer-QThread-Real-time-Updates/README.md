# Module 05 — QTimer & Real-time Updates

> The heartbeat of every live HMI. Learn how `QTimer` drives animations, polls sensors, and refreshes the cluster at 30 fps — and when you need a worker thread instead.

| Phase | Level | Time | Qt modules |
| --- | --- | --- | --- |
<<<<<<< HEAD:05-QTimer-QThread-Real-time-Updates/README.md
| Phase 2 — Intermediate Qt | Intermediate | 3 hours | Qt Core · Qt Widgets |
=======
| Phase 2 — Qt Fundamentals | Intermediate | 3 hours | Qt Core · Qt Widgets |
>>>>>>> 99de9a4d1f407164e3ee3217441f082964694323:05-QTimer-Real-time-Updates/README.md

---

## Table of Contents

1. [Why QTimer Matters](#1-why-qtimer-matters)
2. [QTimer in Automotive HMIs](#2-qtimer-in-automotive-hmis)
3. [The Event Loop — How QTimer Actually Works](#3-the-event-loop--how-qtimer-actually-works)
4. [Single-shot vs Repeating Timers](#4-single-shot-vs-repeating-timers)
5. [Connecting a Timer to a Slot](#5-connecting-a-timer-to-a-slot)
6. [Timing Precision and Multiple Timers](#6-timing-precision-and-multiple-timers)
7. [When QTimer Isn't Enough — The Case for Threads](#7-when-qtimer-isnt-enough--the-case-for-threads)
8. [Threads and Multithreading — The Basics](#8-threads-and-multithreading--the-basics)
9. [QThread + `moveToThread` — A Minimal Example](#9-qthread--movetothread--a-minimal-example)
10. [Worked Example — Sensor Worker for Real-time Data](#10-worked-example--sensor-worker-for-real-time-data)
11. [Official Documentation Map](#11-official-documentation-map)
12. [Reference Videos](#12-reference-videos)
13. [Common Errors & Fixes](#13-common-errors--fixes)

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

## 7. When QTimer Isn't Enough — The Case for Threads

`QTimer` works because the event loop is iterating. If your slot takes 30 ms to run and your timer fires every 50 ms, you've used 60 % of the event loop's time and everything else (button responses, painting, other timers) gets squeezed. Push slot work much past that and the HMI feels sluggish.

Three situations push you off the GUI thread entirely:

- **Blocking I/O** — reading a real CAN bus, opening a serial port, fetching a tile from a navigation server. These can block for tens or hundreds of milliseconds.
- **Heavy computation** — image processing, FFTs on audio data, large file parsing.
- **High-rate continuous work** — sampling at 1 kHz, logging every CAN frame, decoding video.

For all three, the answer is the same: get the work off the GUI thread so the HMI keeps responding. To do that you need a **worker thread**. The next two sections explain what that actually means, and the section after that walks through a real automotive worker.

---

## 8. Threads and Multithreading — The Basics

Before the Qt-specific pattern, the underlying idea.

### What is a thread?

A thread is an independent lane of execution inside your program. When your app starts, the operating system gives it **one thread** — the "main thread" — and starts running your `main()` function on it. In a Qt GUI app that main thread is called the **GUI thread**: it handles all painting, mouse clicks, key presses, timers, and signal/slot calls by default.

Code on a thread runs **sequentially** — one line at a time, in order. If a line takes 200 ms to complete (a slow sensor read, a network request), the thread is stuck there for 200 ms. Every other piece of work waiting on that same thread waits too.

### The HMI problem in one picture

A 200 ms blocking sensor read on the GUI thread looks like this:

    Time →
    GUI thread:  [draw][click handler][SENSOR READ — 200ms — BLOCKED][draw]
                                                                     ^
                                                            screen frozen here

During those 200 ms the user can't tap a button, the speedometer needle doesn't move, the warning icon doesn't blink. The whole HMI feels broken even though nothing crashed.

### What multithreading does

Multithreading means **running more than one thread at the same time**. The OS interleaves them across CPU cores, so they run in parallel:

    GUI thread:     [draw][click][draw][click][draw][click][draw][click]
    Worker thread:  [READ sensor — 200ms ───────────────────][emit result]

The GUI thread stays free — drawing, animating, handling input — while the worker thread does the slow work. When the worker has data to share, it emits a signal that the GUI thread receives and acts on.

### A kitchen analogy

A restaurant with **one cook** takes an order, cooks the steak, plates it, and serves — all in order. Customer #2 waits while customer #1's steak cooks. That's a single-threaded HMI.

Add a **second cook**: one takes orders and serves, the other cooks. Both happen at the same time. Customer #2 gets their order taken while customer #1's steak is still on the grill. That's multithreading.

### Why this is exactly what real-time HMIs need

Automotive HMIs constantly handle data sources that arrive on their own clock — independent of what the user is doing on screen:

| Data source | Rate | If done on GUI thread |
| --- | --- | --- |
| CAN bus frames | 50–500 per second | Each parse adds jitter to the UI |
| GPS NMEA strings | 1–10 per second | Slow text parsing stalls painting |
| Serial sensor poll | 20–100 Hz | Blocks UI between reads |
| Phone over Bluetooth | bursty | Whole-second freezes when a call connects |
| Disk logging of trip data | bursty | Buffered writes stall every widget |

Move each onto its own worker thread and the GUI thread is freed to do nothing but draw — which is what gives you the **smooth 60 fps cluster experience** the driver expects, even while data is streaming in continuously.

### One non-negotiable rule

**Widgets can only be touched from the GUI thread.** That's a Qt rule, enforced by the framework. A worker thread reads sensors, processes data, computes results — and emits a signal. The matching slot on the GUI thread is the only place the widget is actually updated.

This is exactly why signals & slots (Module 03) and threading fit together so neatly: the signal carries the data across the thread boundary, the slot runs back on the GUI thread, the widget updates safely.

> 📘 **Reference:** [Thread Support in Qt (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/thread-basics.html) · [Threads and QObjects (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/threads-qobject.html)

---

## 9. QThread + `moveToThread` — A Minimal Example

In Qt 5/6 you do **not** subclass `QThread`. You write a worker class that inherits `QObject`, then **move** it onto a `QThread`. This is the modern, recommended pattern.

Three steps:

1. Define a `QObject` worker with a slot that does the work and a signal that reports results.
2. Create a `QThread`, create the worker, call `worker->moveToThread(thread)`.
3. Connect the worker's signal to a GUI-thread slot, then start the thread.

### Simplest possible worker — a counter

A worker that counts from 1 to 10, emitting each number with a one-second pause. Running this loop directly on the GUI thread would freeze the app for 10 seconds. On a worker thread, the UI stays responsive — buttons still click, timers still fire, the screen still repaints.

    class Counter : public QObject {
        Q_OBJECT
    public slots:
        void start() {
            for (int i = 1; i <= 10; ++i) {
                emit countChanged(i);
                QThread::sleep(1);     // pretend this is slow work
            }
            emit finished();
        }
    signals:
        void countChanged(int n);
        void finished();
    };

### Wiring it from the main window

    void MainWindow::startCounting() {
        QThread *thread  = new QThread(this);
        Counter *counter = new Counter;       // no parent yet
        counter->moveToThread(thread);        // now lives on worker thread

        // When the thread starts, call counter->start() on the worker thread
        connect(thread,  &QThread::started,    counter, &Counter::start);

        // Each emitted number arrives back on the GUI thread automatically
        connect(counter, &Counter::countChanged, this, &MainWindow::onCount);

        // Clean shutdown once the work is done
        connect(counter, &Counter::finished, thread,  &QThread::quit);
        connect(counter, &Counter::finished, counter, &QObject::deleteLater);
        connect(thread,  &QThread::finished, thread,  &QObject::deleteLater);

        thread->start();
    }

    void MainWindow::onCount(int n) {
        countLabel->setText(QString("Count: %1").arg(n));
    }

Run this and the label updates once per second while the rest of the UI stays fully responsive. That's the whole pattern in roughly 30 lines.

Three things worth pointing out:

- **The worker has no parent.** You can't `moveToThread` an object that has a parent — moving an object means changing its thread affinity, and Qt requires a child to live on the same thread as its parent. `deleteLater` handles cleanup once the worker signals it's finished.
- **The `started → start` connection is what kicks the worker into action *on its own thread*.** If you just called `counter->start()` directly from the main window, it would run on the GUI thread and the whole point would be lost.
- **All cross-thread connections use the default `Qt::AutoConnection`,** which Qt automatically promotes to `Qt::QueuedConnection` between threads. The slot runs on the receiver's thread, every time.

Once this minimal pattern clicks, the real-world example in §10 is just a longer version of the same skeleton.

> 📘 **Reference:** [QThread (Qt 6.1)](https://doc.qt.io/archives/qt-6.1/qthread.html) · [You're doing it wrong — Qt blog](https://www.qt.io/blog/2010/06/17/youre-doing-it-wrong)

---

## 10. Worked Example — Sensor Worker for Real-time Data

Same skeleton as §9, applied to a real automotive case. A worker reads a simulated speed sensor 20 times per second on its own thread and emits a signal carrying the latest value. The GUI thread receives each signal — automatically queued across the thread boundary — and updates the speedometer widget.

This is the production shape: instead of looping with `sleep` like the counter, the worker uses its **own `QTimer`** to drive periodic sampling. Every CAN-bus reader, GPS parser, or sensor sampler in a real Qt cluster follows this exact pattern.

### The worker

    class SensorWorker : public QObject {
        Q_OBJECT
    public:
        explicit SensorWorker(QObject *parent = nullptr) : QObject(parent) {}

    public slots:
        void start() {
            m_timer = new QTimer(this);         // QTimer parented on worker thread
            m_timer->setInterval(50);           // 20 Hz
            connect(m_timer, &QTimer::timeout,
                    this,   &SensorWorker::doSample);
            m_timer->start();
        }

        void stop() {
            if (m_timer) m_timer->stop();
        }

    signals:
        void speedSampled(int kmh);

    private slots:
        void doSample() {
            // Read from real CAN / serial / shared memory here.
            // For now, simulated:
            const int kmh = readSpeedFromSensor();
            emit speedSampled(kmh);
        }

    private:
        QTimer *m_timer = nullptr;
    };

### Wiring it up on the GUI thread

In your Dashboard's constructor, set up the worker and thread:

    auto *thread = new QThread(this);
    auto *worker = new SensorWorker;            // no parent — we move it
    worker->moveToThread(thread);

    // When the thread starts, ask the worker to begin sampling
    connect(thread, &QThread::started,
            worker, &SensorWorker::start);

    // Worker's signal fires on the worker thread, slot runs on GUI thread
    connect(worker, &SensorWorker::speedSampled,
            this,   &Dashboard::onSpeedSampled);

    // Clean shutdown when the dashboard is destroyed
    connect(this,   &Dashboard::destroyed, worker, &SensorWorker::stop);
    connect(this,   &Dashboard::destroyed, thread, &QThread::quit);
    connect(thread, &QThread::finished,    worker, &QObject::deleteLater);
    connect(thread, &QThread::finished,    thread, &QObject::deleteLater);

    thread->start();

### The GUI-thread slot

    void Dashboard::onSpeedSampled(int kmh) {
        speedometer->setSpeed(kmh);   // safe — we're on the GUI thread
    }

### Two rules to remember

1. **Never touch widgets from a worker thread.** Widget calls are GUI-thread only. The worker emits signals; the slot on the GUI thread is what touches the widget.
2. **`moveToThread` moves the object, not children created before the move.** The `QTimer` in `start()` above is created *inside a slot that runs on the worker thread*, so it lives there correctly. If you created it in the constructor (which runs on the GUI thread), it would live on the wrong thread — and `QTimer` would refuse to start.

A complete sensor-worker setup is typically 100–200 lines of code. Once you have one working, copying the same skeleton for a CAN reader, a GPS parser, or a disk logger is mechanical — change the data type in the signal, change what `doSample` does, done.

---

## 11. Official Documentation Map

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

## 12. Reference Videos

| Video | Length | Why watch |
| --- | --- | --- |
| [Qt QTimer Tutorial — Basics](https://www.youtube.com/watch?v=H-Cs2-jjyA8) | ~12 min | First timer, single-shot vs repeating, simple slot |
| [Animating Widgets with QTimer in Qt C++](https://www.youtube.com/watch?v=HVL4Yz0oqYM) | ~18 min | Driving a custom-painted widget — connects directly to Module 04 |
| [Qt Event Loop Explained](https://www.youtube.com/watch?v=DI5BLP4SH3o) | ~15 min | What's actually happening under `app.exec()` |
| [QThread the Right Way — KDAB](https://www.youtube.com/watch?v=fQyqIO9P1ek) | ~25 min | Official KDAB explanation of `moveToThread` vs subclassing |
| [Multithreading in Qt — Worker Pattern](https://www.youtube.com/watch?v=eRZgenIxX_g) | ~20 min | Full worker setup, signals across threads |

---

## 13. Common Errors & Fixes

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
