# Contributing

Welcome! This repo is structured so trainees can submit work as Pull Requests and get reviewed feedback. Whether you're a solo learner or part of a cohort, the workflow below keeps things tidy.

---

## Workflow for Trainees

### 1. Fork the repo

Click **Fork** in the top-right of the GitHub page. Now you have your own copy at `https://github.com/<your-username>/Qt-Automotive-HMI-Training`.

### 2. Clone your fork

```bash
git clone https://github.com/<your-username>/Qt-Automotive-HMI-Training.git
cd Qt-Automotive-HMI-Training
git remote add upstream https://github.com/ManeParag/Qt-Automotive-HMI-Training.git
```

### 3. Create a working branch per module

Branch naming: `module-NN/<your-name>` (e.g., `module-04/parag`).

```bash
git checkout -b module-04/parag
```

### 4. Do the exercise

Each module has an `exercises/` folder. Work in there — **don't modify the starter project** (it's reference material). Copy it into `exercises/<your-solution>/` and edit the copy.

### 5. Commit with clear messages

```bash
git add .
git commit -m "module 04: implement dial face with tick marks"
git push origin module-04/parag
```

Commit message format: `module NN: <short description>`

### 6. Open a Pull Request

On GitHub, click **Compare & pull request**. Use this template:

```markdown
## Module
04 — QPainter Custom Drawing

## What I built
Brief description of what you did.

## Screenshot
(drag a screenshot of the running app here)

## Self-checklist
- [ ] Builds clean with no warnings
- [ ] App runs without crashing
- [ ] Followed naming conventions
- [ ] Submission criteria from module README all met
```

---

## Code Style

This is C++17 with Qt 6. We follow a simplified Qt style:

- **Indentation:** 4 spaces, no tabs
- **Braces:** opening brace on same line for functions and control flow
- **Class members:** prefix with `m_` (e.g., `m_speedLabel`)
- **Signals:** named in past tense (`speedChanged`, `engineStarted`)
- **Slots:** named in present tense (`updateSpeed`, `onConnectClicked`)
- **Headers:** include guards via `#pragma once`
- **`Q_OBJECT`** is required in any class declaring signals or slots

Example:

```cpp
#pragma once
#include <QWidget>

class Speedometer : public QWidget {
    Q_OBJECT
public:
    explicit Speedometer(QWidget *parent = nullptr);

public slots:
    void setSpeed(int kmh);

signals:
    void redlineEntered();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_speed = 0;
};
```

---

## Review SLA

In an active cohort, expect reviews within **2 working days** of opening a PR. For self-learners, you can use the [Issues tab](../../issues) to ask questions on any module.

---

## For Maintainers / Trainers

- The Excel progress tracker (separate file/repo) is the source of truth for who's where in the program.
- Update the trainee's row when their PR is merged.
- The `final-project/` rubric drives the final grade — keep it in sync if you adapt the program.

---

Questions? Open an issue with the `question` label. Bug in the starter code? Open one with `bug`. Suggestion? `enhancement`. We read every one.
