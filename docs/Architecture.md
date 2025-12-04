# Architecture — Options Market Maker Simulation

## Overview

This document describes the architecture of the Options Market Maker simulation repository. The design intentionally separates concerns into logical modules: **market**, **pricing**, **greeks**, **hedging**, **execution**, **market-making (mm)**, **simulation orchestration (sim)**, and **utilities**. The separation yields a testable, extensible codebase suitable for interview demos and incremental development.

High-level goals:
- Clear separation of *modeling* (pricing, Greeks) from *execution* (order handling, transaction costs).
- Deterministic, reproducible simulations (seedable RNG).
- Straightforward hooking of alternate models (LOB, stochastic volatility) with minimal changes.
- Easy-to-interpret output for analysis (CSV and notebooks).

---

## Component Map

### 1. `market/`
**Responsibilities**
- Price path generation for the underlying (synthetic market).
- Simple feed abstraction for driving the simulator tick-by-tick.
- Price path containers.

**Key classes**
- `UnderlyingSimulator` — generates GBM paths from `(S0, mu, sigma)`.
- `PricePath` — container for the simulated series.
- `MarketDataFeed` — an iterator-style wrapper that emits ticks and optionally invokes callbacks.

**Notes**
- The GBM generator uses log-Euler (exact discrete step for GBM in log space) for numerical stability.
- The feed abstraction lets you later replace the synthetic generator with real market data or a UDP message feed without changing the simulator.

---

### 2. `pricing/`
**Responsibilities**
- Core valuation engines for derivative instruments.
- Provide both discrete (binomial) and closed-form (Black–Scholes) pricers for validation and flexibility.

**Key classes**
- `BinomialTree` — recombining binomial tree, supports European & American via backward induction. Configurable step `N`.
- `BlackScholes` — analytic price and Delta (for European options).

**Notes**
- The binomial pricer is primary for interview purposes because it is easy to reason about and supports American exercise.
- Black–Scholes is used for convergence checks; for production or extension you may add finite-difference PDE or Monte Carlo pricers.

---

### 3. `greeks/`
**Responsibilities**
- Calculate option sensitivities (Δ, Γ, Θ, Vega, Rho).
- Provide numerical wrappers and helpers for pricers.

**Key classes**
- `GreeksCalculator` — central-difference wrappers around any pricer function. Returns a `Greeks` struct.

**Notes**
- Use central differences for better numerical stability.
- Choose bump sizes relative to the parameter (e.g., `dS = max(1e-4, 1e-2 * S)`).
- Theta and Rho require pricers that accept time-to-maturity and interest rate as arguments — pass those explicitly in lambdas.

---

### 4. `hedging/`
**Responsibilities**
- Decide when and how to hedge option risk (primarily delta hedging).
- Provide multiple strategy implementations (threshold, periodic, optimal scheduling).

**Key classes**
- `HedgingStrategy` — abstract base class to unify strategy interfaces.
- `ThresholdHedging` — concrete strategy that rebalances when deviation exceeds a threshold.
- `DeltaHedger` — convenience wrapper to compute target quantities and prepare orders.

**Notes**
- Strategies should be stateless where possible. The simulator maintains position state.
- Consider building an `OptimalHedging` strategy later that minimizes expected hedging cost given transaction cost model.

---

### 5. `execution/`
**Responsibilities**
- Simulate the execution of trades in the underlying.
- Model transaction costs and simple slippage.

**Key classes**
- `Order` — simple POD for side, qty, and price.
- `ExecutionEngine` — fills orders at a model price and charges costs (fixed + proportional).
- `TransactionCostModel` — encapsulates cost logic.

**Notes**
- Current engine is a market-taker model (instant fills at mid). This can be replaced with a limit-order-book (LOB) simulator to model queueing and partial fills.

---

### 6. `mm/` (Market Making)
**Responsibilities**
- Generate quotes based on model price and risk posture.
- Track market maker inventory and basic risk limits.

**Key classes**
- `MarketMaker` — maintains option inventory and builds quotes.
- `QuoteGenerator` — produces bid/ask from mid price and spread parameter.
- `InventoryManager` — thread-safe storage for positions (optional mutex).

**Notes**
- Quote strategy can be extended: include inventory skew, volatility-aware spreads, and adverse selection adjustments.

---

### 7. `sim/` (Simulation Orchestrator)
**Responsibilities**
- Drive the simulation loop: receive new ticks, price options, compute Greeks, ask hedgers for orders, execute trades, and record outputs.
- Produce CSVs and time-series for later analysis.

**Key classes**
- `Simulator` — central controller, configured with a `SimConfig` object that defines all global parameters.

**Notes**
- Simulation is deterministic given a seed. The simulator currently assumes a single-option scenario (one instrument). Extend to handle multi-option portfolios by tracking multiple instruments and netting Greeks.

---

### 8. `util/` and `core/`
**Responsibilities**
- Small helper utilities: logging, CSV writing, RNG wrapper, math helpers.

**Key classes**
- `CSVWriter`, `Logger`, `Random`, `MathUtils`.

**Notes**
- Keep these lightweight; for larger projects swap `Logger` for `spdlog`, and use `Eigen` for heavy math.

---

## Data flow (tick lifecycle)

1. `MarketDataFeed` emits next price tick.
2. `Simulator` receives tick and:
   - constructs a `BinomialTree` (or uses cached pricer) to compute option price,
   - computes Greeks (delta) via `BlackScholes` or `GreeksCalculator`,
   - passes delta to hedging strategy to decide on an order,
   - sends `Order` to `ExecutionEngine`,
   - updates `InventoryManager` and cashbook,
   - logs state (spot, option mark, delta, pos, cash, pnl).
3. Repeat until path end. On completion, the simulator writes CSV.

---

## Extensibility points

- **LOB Execution**: Add a multi-level LOB module, tie `Order` execution to queue position, and model fill probability & partial fills.
- **Stochastic Volatility**: Replace GBM with Heston or local vol and add Monte Carlo pricer for path-dependent instruments.
- **Multi-instrument book**: Support many option instruments, net Greeks, and run portfolio-level hedging.
- **Calibration**: Add module to create and fit an implied vol surface from market quotes.
- **Risk Limits**: Add dynamic risk controls that widen spreads or stop quoting when limits are breached.

---

## Recommended development workflow

1. Implement & validate core pricing (binomial vs Black–Scholes).
2. Add deterministic GBM path & single-option hedging loop.
3. Add transaction costs and threshold/periodic hedging strategies.
4. Add CSV logging + notebook analysis.
5. Iteratively add complexity (LOB, multi-instrument) and measure performance.

---

## Final remarks

This architecture balances clarity (easy to discuss during interviews) with realistic features (execution model, hedging, and P&L tracking). It intentionally keeps the core deterministic and unit-testable — a crucial property for quant code.