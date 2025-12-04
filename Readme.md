# Options Market Maker Simulation 

A comprehensive, professional-grade simulation framework for option pricing, market making, delta hedging, and execution modelling. This project is designed to demonstrate strong quantitative, mathematical, and software engineering skills, suitable for quant interviews and real‑world research experimentation.

---

## 📌 Overview

This project implements a full pipeline of an options market‑maker:

* Pricing options using **Binomial Trees** and **Black‑Scholes**.
* Computing **Greeks** analytically and numerically.
* Automating **delta hedging** using configurable hedging strategies.
* Simulating **underlying price paths** using Geometric Brownian Motion.
* Modelling **transaction costs**, order execution, and inventory changes.
* Running full simulations and exporting **CSV results** for analysis.

It is structured using modern, modular **C++20**, with a clean architecture, strong encapsulation, and full unit test coverage via GoogleTest.

---

## 🧱 Architecture

```
Options-MarketSim/
├── src/
│   ├── core/                # Time utils, types, math helpers
│   ├── pricing/             # Option pricing models
│   ├── greeks/              # Numerical + analytical Greeks
│   ├── market/              # Underlying price simulation
│   ├── hedging/             # Delta hedging framework
│   ├── execution/           # Order + transaction cost model
│   ├── sim/                 # Simulation engine + config
│   ├── mm/                  # Market maker components
│   └── util/                # CSV writer, logger, RNG
│
├── include/                 # Public headers
├── tests/                   # GTest test suite
├── scripts/                 # Notebooks for analysis
└── docs/                    # Architecture + research notes
```

---

## 🧮 Option Pricing Models

### **Binomial Tree Pricing**

Implements a step‑based tree (CRR model):

* Up/down factors from volatility
* Risk‑neutral probability
* Backward induction for European options

Useful for validating Greeks, exploring convergence, and demonstrating numerical understanding.

### **Black‑Scholes Analytical Model**

Closed‑form pricing for European calls and puts:

* Fast
* Stable
* Provides analytical Greeks for comparison and model validation

---

## 📈 Greeks Calculation

The framework computes Greeks in two ways:

### **1. Analytical Greeks (Black‑Scholes)**

* Delta
* Gamma
* Vega
* Theta
* Rho

### **2. Numerical Greeks**

Using finite‑difference approximations:

* Delta ≈ (P(S+h) – P(S−h)) / (2h)
* Gamma ≈ (P(S+h) − 2P(S) + P(S−h)) / h²
* Vega via perturbation of volatility

These are essential for hedging validation and model sanity‑checks.

---

## 🔄 Hedging Engine

The market maker uses:

### **Delta Hedger**

Computes hedge quantity:

```
target_position = - delta × notional
```

Keeps the portfolio approximately delta‑neutral.

### **Hedging Strategies**

Two included examples:

* **Threshold Hedging** → trade only when mismatch exceeds a threshold
* **Perfect Delta Match** → immediate hedge to target

This structure is extensible for gamma‑hedging, vega‑hedging, etc.

---

## 💹 Execution Engine

Simulates realistic order execution:

* Buy/Sell orders
* Transaction cost model:

```
cost = fixed_fee + pct_fee × notional
```

* Updates cash, inventory, and PnL

This helps quantify hedging performance under market frictions.

---

## 📊 Market Simulation

Underlying asset follows **Geometric Brownian Motion (GBM)**:

```
dS = μSdt + σS dW
```

Generates a price path used to:

* Recompute Greeks each timestep
* Trigger hedging actions
* Compute PnL (realized + mark‑to‑market)

---

## 🧪 Unit Testing

GoogleTest covers:

* Binomial Tree convergence
* Greeks correctness
* Execution engine logic
* Hedging strategy behavior
* Simulation output sanity checks

Ensures correctness and reliability.

---

## 📂 Simulation Output

Simulation generates a CSV in `results/` containing:

```
step, spot, option_price, delta, position, cash, pnl, hedge_order_qty, hedge_order_side
```

This file can be explored using the included Jupyter notebook.

---

## 📊 Jupyter Notebook Analysis

`analyze_results.ipynb` generates:

* PnL curves
* Hedge mismatch over time
* Delta trajectories
* Price vs hedge actions
* Greeks evolution

Excellent for showcasing quant intuition during interviews.

---

## 🛠️ Build Instructions

### **Build**

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### **Run Simulation**

```bash
./options-mm
```

### **Run Tests**

```bash
./unit_tests
```

---

## 🚀 Future Extensions

You may extend this project using:

* Heston or SABR volatility models
* Local volatility surfaces
* Order book simulation
* Gamma & Vega hedging
* Reinforcement‑learning‑based hedging (advanced option)

---

## 🎯 Purpose of This Project

This project is intentionally structured like something you would develop as:

* A **Quantitative Research Intern**
* A **Derivatives Trader Intern**
* A **Quant Developer**
* A **Risk Analyst**

It demonstrates mastery of:

* Numerical methods
* Stochastic calculus
* Option pricing theory
* Hedging techniques
* C++ architecture + industry practices
* Testing & simulation frameworks

---

## 📜 License

MIT License (Optional — Add if publishing publicly)

---

## 🙌 Acknowledgments

This simulator is inspired by classical quantitative finance literature including:

* Hull’s *Options, Futures, and Other Derivatives*
* Wilmott's *Paul Wilmott Introduces Quantitative Finance*
* Various industry market‑making research notes