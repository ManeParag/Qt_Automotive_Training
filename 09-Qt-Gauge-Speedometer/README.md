# Module 09 — Custom Gauge / Speedometer

> Build a fully custom automotive-style speedometer using Qt Widgets and QPainter.  
> Learn how real instrument clusters render gauge faces, rotating needles, animated arcs, and live sensor-driven updates.

| Phase | Level | Time | Qt modules |
| --- | --- | --- | --- |
| Phase 3 | Advanced | 3 hours | Qt Gauge / Speedometer |

---

## Table of Contents

1. [Why Custom Gauges Matter](#1-why-custom-gauges-matter)
2. [Anatomy of a Gauge](#2-anatomy-of-a-gauge)
3. [Gauge API Design](#3-gauge-api-design)
4. [Drawing the Gauge Face](#4-drawing-the-gauge-face)
5. [Tick Marks and Labels](#5-tick-marks-and-labels)
6. [Rotating Needle Design](#6-rotating-needle-design)
7. [Smooth Needle Animation](#7-smooth-needle-animation)
8. [Complete Speedometer Widget](#8-complete-speedometer-widget)
9. [Wiring Sensor Data to the Gauge](#9-wiring-sensor-data-to-the-gauge)
10. [Gauge Variations](#10-gauge-variations)
11. [Themes and Styling](#11-themes-and-styling)
12. [Official Documentation Map](#12-official-documentation-map)
13. [Reference Videos](#13-reference-videos)
14. [Common Errors & Fixes](#14-common-errors--fixes)

---

# 1. Why Custom Gauges Matter

Modern automotive HMIs rarely use stock widgets for dashboard elements.  
Speedometers, tachometers, battery meters, and fuel gauges are all custom-painted widgets built specifically for the vehicle's visual identity.

Custom gauges provide:

- Real-time telemetry visualization
- Smooth animated feedback
- Scalable vector rendering
- Brand-specific styling
- Embedded-system friendly UI

Qt Widgets + QPainter allow complete control over every pixel rendered on screen.

---

# 2. Anatomy of a Gauge

Every automotive speedometer is composed of the same six core parts.

| Component | Purpose |
| --- | --- |
| Face | Circular background layer |
| Track | Base arc showing full range |
| Fill Arc | Active progress/value arc |
| Tick Marks | Visual speed divisions |
| Needle | Rotating current-value indicator |
| Readout | Numeric digital speed display |

These parts combine to form nearly every analog automotive gauge.

---

# 3. Gauge API Design

Before drawing anything, design the widget from the outside first.

A clean API usually includes:

| Element | Purpose |
| --- | --- |
| `setSpeed(int)` | Public slot for updates |
| `speedChanged(int)` | Notification signal |
| `targetSpeed` | Incoming sensor value |
| `displayedSpeed` | Smoothed animated value |
| `animationTimer` | Controls needle movement |

The key principle:

> Separate the **real sensor value** from the **displayed animated value**.

This prevents harsh visual jumps in the dashboard.

### Widget API Snippet

```cpp
public slots:
    void setSpeed(int speed);

signals:
    void speedChanged(int speed);
```

---

# 4. Drawing the Gauge Face

The gauge face is the base layer of the speedometer.

Typical elements include:

- Circular outer ring
- Gradient background
- Inner shadow
- Border glow
- Speed arc track

Most drawing happens inside:

```cpp
QPainter painter(this);
```

For smooth rendering:

```cpp
painter.setRenderHint(QPainter::Antialiasing);
```

The gauge face defines the visual identity of the dashboard.

---

# 5. Tick Marks and Labels

Tick marks visually divide the speed range.

Typical automotive layout:

| Type | Example |
| --- | --- |
| Minor Tick | Every 10 km/h |
| Major Tick | Every 20 km/h |
| Labels | 0 → 240 |

Labels are positioned using rotation and angle calculations around the gauge center.

Challenges include:

- Angle mapping
- Text alignment
- Rotation compensation
- Dynamic scaling

---

# 6. Rotating Needle Design

The needle is the central animated component.

Instead of recalculating coordinates every frame:

1. Move the painter origin to the gauge center
2. Rotate the coordinate system
3. Draw the needle at angle 0

Qt handles the transformation.

### Rotation Snippet

```cpp
painter.rotate(angle);
```

Typical needle styles:

- Thin racing needle
- Polygon arrow
- Minimal EV pointer
- Glow-effect needle

---

# 7. Smooth Needle Animation

Directly jumping between values looks unrealistic.

Instead:

| Step | Action |
| --- | --- |
| Sensor updates | `targetSpeed` changes |
| Timer updates | `displayedSpeed` interpolates |
| Widget repaints | Needle sweeps smoothly |

Animation is commonly driven using:

```cpp
QTimer *timer = new QTimer(this);
```

This creates the smooth movement seen in real instrument clusters.

---

# 8. Complete Speedometer Widget

A finished speedometer combines:

- Gauge face
- Tick calculations
- Needle rotation
- Speed labels
- Animated updates
- Dynamic scaling

Once complete, the widget can be:

- Embedded in layouts
- Reused across HMIs
- Combined into instrument clusters

This usually becomes the learner’s reusable base gauge component.

---

# 9. Wiring Sensor Data to the Gauge

A clean signal/slot API makes integration simple.

Typical data sources include:

- CAN Bus
- Serial Devices
- SensorWorker threads
- Simulated telemetry
- MQTT streams

Integration example:

```cpp
connect(sensor, SIGNAL(speedUpdated(int)),
        gauge, SLOT(setSpeed(int)));
```

Good widget architecture dramatically simplifies dashboard integration.

---

# 10. Gauge Variations

The same architecture can power many dashboard components.

| Gauge Type | Example |
| --- | --- |
| Tachometer | RPM |
| Fuel Gauge | Fuel Level |
| Temperature Gauge | Engine Temp |
| Battery Gauge | EV Charge |
| Pressure Gauge | Industrial Systems |

After building multiple gauges, many projects extract a reusable:

```text
BaseGauge
```

class for shared rendering logic.

---

# 11. Themes and Styling

Automotive HMIs rely heavily on visual styling.

Popular dashboard styles:

- Dark Performance Theme
- EV Minimal Dashboard
- Neon Cyberpunk UI
- Sport Redline Mode
- Glassmorphism Interface

Customizable elements:

- Arc gradients
- Needle colors
- Glow effects
- Tick density
- Font styles
- Warning indicators

Qt gradients and alpha blending are heavily used for modern cluster visuals.

---

# 12. Official Documentation Map

## Core Painting

| Resource | Purpose |
| --- | --- |
| [QPainter Documentation](https://doc.qt.io/qt-6/qpainter.html?utm_source=chatgpt.com) | Main drawing API |
| [Qt Paint System](https://doc.qt.io/qt-6/paintsystem.html?utm_source=chatgpt.com) | How Qt rendering works |
| [Coordinate System Documentation](https://doc.qt.io/qt-6/coordsys.html?utm_source=chatgpt.com) | Rotation and transformations |

## Widgets and Rendering

| Resource | Purpose |
| --- | --- |
| [Qt Widgets Module](https://doc.qt.io/qt-6/qtwidgets-index.html?utm_source=chatgpt.com) | Widget framework |
| [QPen Documentation](https://doc.qt.io/qt-6/qpen.html?utm_source=chatgpt.com) | Outline styling |
| [QBrush Documentation](https://doc.qt.io/qt-6/qbrush.html?utm_source=chatgpt.com) | Fill styling |

## Animation & Timing

| Resource | Purpose |
| --- | --- |
| [QTimer Documentation](https://doc.qt.io/qt-6/qtimer.html?utm_source=chatgpt.com) | Real-time updates |
| [Qt Animation Framework](https://doc.qt.io/qt-6/animation-overview.html?utm_source=chatgpt.com) | Advanced animations |

---

# 13. Reference Videos

| Topic | Focus |
| --- | --- |
| Qt QPainter Basics | Drawing shapes |
| Custom Qt Widgets | Widget architecture |
| Analog Clock Tutorials | Rotation concepts |
| Speedometer UI Design | Automotive HMIs |
| Qt Animation Tutorials | Smooth motion |

Recommended searches:

- “Qt Speedometer Widget”
- “Qt QPainter Gauge”
- “Qt Automotive Dashboard”
- “Qt Instrument Cluster”

---

# 14. Common Errors & Fixes

| Problem | Cause |
| --- | --- |
| Needle points wrong direction | Incorrect angle mapping |
| Needle jumps instead of sweeping | No interpolation logic |
| Labels rotate incorrectly | Painter transform leaked |
| Arc looks jagged | Antialiasing disabled |
| UI flickers during updates | Excessive repainting |
| Gauge scales badly | Hardcoded dimensions |
| CPU usage very high | Infinite repaint loop |
| Animation never stops | Timer condition issue |
| Text overlaps tick marks | Poor spacing calculations |
| Widget remains blank | `paintEvent()` not triggered |

---

# What's Next

Once the speedometer is rendering smoothly, the next step is driving it with real-time data.

Future modules typically include:

- CAN Bus integration
- Multi-threaded telemetry
- Animated dashboard transitions
- Full digital instrument clusters
- Embedded Linux deployment

---

← Previous Module · Back to Syllabus · Next Module →