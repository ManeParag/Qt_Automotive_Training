# Setup Guide — Qt 6 for Automotive HMI Training

This page walks you through installing everything you need to build and run every module in this repo. Aim for **Qt 6.5 LTS or newer**.

---

## 1. Install Qt 6

The easiest path on every OS is the **online installer**.

1. Create a free Qt account → <https://www.qt.io/download-qt-installer>
2. Download the online installer for your OS
3. Run it. When prompted to choose components, tick:

   - ✅ **Qt 6.5 (or later) → MSVC 2019 64-bit** (Windows) **or GCC 64-bit** (Linux) **or macOS** (Mac)
   - ✅ **Qt Charts** (required for module 08)
   - ✅ **Qt Creator** (the IDE)
   - ✅ **CMake** and **Ninja** (Qt Creator bundles them, leave defaults on)
   - ✅ **MinGW 11.x** *(optional, Windows only — alternative compiler if you don't have MSVC)*

Total install size: ~10–12 GB. Coffee break time.

---

## 2. Verify the install

Open **Qt Creator**. From the menu:

- `Help → About Qt Creator…` should show **Qt Creator 12.x** or newer
- `Edit → Preferences → Kits` should show at least one kit with a green ✅
- `Edit → Preferences → Kits → Qt Versions` should list your installed Qt 6 build

If any kit shows a red ❌, click it and read the warning — usually a missing compiler.

---

## 3. Build & run the smoke-test project

```bash
git clone https://github.com/ManeParag/Qt-Automotive-HMI-Training.git
cd Qt-Automotive-HMI-Training/01-qt-creator-ide/HelloAutoHMI
```

Open `HelloAutoHMI.pro` in Qt Creator → select your kit → click the green ▶ button (or press `Ctrl+R`).

If you see a window titled **"Hello AutoHMI"** with a button on it, your toolchain is working. ✅

---

## 4. OS-specific notes

### Windows

- Use **MSVC 2019** or **2022** for best compatibility. Install the *Build Tools for Visual Studio* (free) if you don't have Visual Studio itself.
- If you see *"cannot find compiler"* errors, run Qt Creator from a **Developer Command Prompt** the first time.
- Antivirus on `C:\Qt\` slows builds dramatically — add an exclusion.

### Linux (Ubuntu / Debian)

```bash
# Required system packages before running the Qt installer
sudo apt update
sudo apt install -y build-essential libgl1-mesa-dev libxkbcommon-x11-0 \
                    libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-randr0 \
                    libxcb-render-util0 libxcb-shape0 libxcb-sync1 libxcb-xfixes0 \
                    libxcb-xinerama0 libxcb-xkb1
```

### macOS

- Install **Xcode Command Line Tools** first: `xcode-select --install`
- Apple Silicon (M1/M2/M3) is fully supported from Qt 6.2 onwards
- macOS 11 Big Sur is the minimum supported version

---

## 5. Troubleshooting

| Symptom                                         | Fix                                                                  |
| :---------------------------------------------- | :------------------------------------------------------------------- |
| `qmake: command not found`                      | Add `~/Qt/6.x/<compiler>/bin` to your `PATH`                         |
| `Cannot find -lQt6Charts` (linker error)        | Re-run installer, tick **Qt Charts** under your Qt version           |
| App builds but shows blank window on Linux      | Install `libxcb-*` packages (see Linux section above)                |
| Slow build on Windows                           | Add `C:\Qt\` to your antivirus exclusion list                        |
| `error: 'Q_OBJECT' macro requires moc`          | Re-run **qmake** (right-click project → *Run qmake*) then rebuild    |

Stuck? Open a GitHub Issue with: your OS, Qt version (`Help → About Qt Creator`), and the full error text.

---

You're ready. Head back to [the main README](./README.md) and start with [Module 01](./01-qt-creator-ide/).
