# Hedging Experiments & Examples

This document provides detailed hedging experiments you can run with the starter simulation. Each experiment includes: goal, configuration parameters to vary, expected behaviour, measurement recipes, and interpretation guidance.

---

## Experiment 1 — Baseline: frequent hedging vs. infrequent hedging

**Goal:** Observe how hedging frequency affects hedging error and trading costs.

**Setup:**
- Single European call option, `S0 = 100`, `K = 100`, `T = 0.5`, `sigma = 0.2`, `r = 0.01`.
- Simulate 1 price path with `steps = 200`.
- Two hedging strategies:
  - **Frequent**: rehedge every tick (periodic with step `1`).
  - **Infrequent**: rehedge every `10` ticks.
- Transaction cost: proportional `tx_pct = 0.0005` and `tx_fixed = 0`.

**Metrics:**
- Hedging RMSE of delta error.
- Total transaction costs.
- Final P&L distribution (if multiple seeds).
- Number of trades executed.

**Expected behaviour:**
- Frequent hedging reduces hedging error (RMSE) but increases transaction costs.
- With non-zero transaction costs, final P&L may be worse with too-frequent hedging.
- The optimal trade-off depends on volatility and cost; plot RMSE vs total cost.

**How to measure:**
- Run multiple seeds and compute average RMSE and average cost per strategy.
- Plot RMSE (y-axis) vs total transaction costs (x-axis) per strategy.

---

## Experiment 2 — Threshold hedging sweep

**Goal:** Find a threshold that optimally balances hedging error vs transaction cost.

**Setup:**
- Use `ThresholdHedging` strategy.
- Sweep `threshold` in `[0.1, 0.25, 0.5, 1.0, 2.0]` (expressed in underlying shares).
- Use `steps = 200` and run `Nsim = 50` Monte Carlo price paths.

**Metrics:**
- Average hedging RMSE across seeds.
- Mean total transaction cost.
- Average final P&L.

**Expected behaviour:**
- Small thresholds mimic frequent hedging -> small RMSE, high trading costs.
- Large thresholds -> large RMSE, low trading costs.
- Expect U-shaped curves for final P&L vs threshold if transaction costs are moderate.

**Interpretation:**
- The "knee" of the RMSE vs cost curve is a good candidate for a practical threshold.
- Use normalized metrics (RMSE per unit cost) when comparing across market regimes.

---

## Experiment 3 — Model misspecification: realized volatility vs model volatility

**Goal:** Examine hedging P&L when model volatility (used for pricing/delta) differs from realized volatility of the underlying.

**Setup:**
- Realized underlying generated with `sigma_real = 0.30`.
- Model uses `sigma_model = 0.20` (volatility underestimation case) and vice versa (overestimation).
- Use threshold hedging with moderate threshold (e.g., `0.5`).
- Run multiple seeds.

**Metrics:**
- Bias in P&L (systematic losses or gains).
- Hedging RMSE and final P&L distribution.

**Expected behaviour & explanation:**
- If model `sigma_model` is lower than `sigma_real`, the model underestimates option convexity; delta hedges will systematically under-hedge and the hedger can incur losses during volatile moves (gamma exposure).
- Conversely, overestimating volatility may cause over-hedging and higher transaction costs but possibly smaller catastrophic losses.

**Interpretation:**
- Use P&L attribution to separate gamma-related losses from execution costs.
- This experiment highlights model risk — a key quant interview talking point.

---

## Experiment 4 — Hedging under latency & slippage

**Goal:** Measure the impact of execution latency (delayed fills) and slippage on hedging error.

**Setup:**
- Introduce a simple latency parameter: when the hedger emits an order at time `t`, it is executed at `t + L` (if within path bounds) at the then-prevailing spot price.
- Introduce slippage as an additional proportional cost: `slippage_pct`.
- Use moderate volatility and threshold hedging.

**Metrics:**
- Hedging RMSE degraded by latency.
- Transaction cost + realized slippage.
- Final P&L degradation versus zero-latency baseline.

**Expected behaviour:**
- Latency increases realized hedging error, especially in high-volatility regimes.
- Slippage amplifies the effect of frequent hedging.

**Interpretation:**
- Discuss the trade-offs of aggressive hedging when latency is material.
- In practice, latency-aware policies use larger thresholds or predictive hedging (e.g., forecast short-term drift) to compensate.

---

## Experiment 5 — Portfolio netting (two-option example)

**Goal:** Demonstrate netting benefits across instruments.

**Setup:**
- Two options with opposite deltas or correlated deltas (e.g., same underlying, different strikes).
- Construct a portfolio where net delta is smaller than individual deltas.
- Hedge net delta only.

**Metrics:**
- Compare transaction costs and RMSE when hedging each option individually vs hedging net delta.

**Expected behaviour:**
- Netting reduces required underlying trades, lowering transaction costs and possibly improving realized P&L.
- Show sample time-series where two option deltas partially cancel.

**Interpretation:**
- Real market makers always net across book — this experiment is to show the advantage.

---

## Implementation tips & logging

- **Record trades explicitly**: for final P&L attribution you must log `trade_qty`, `trade_price`, and `tx_cost` for every fill.
- **Record option inventory** and the notional per option contract.
- **Run Monte Carlo**: run many independent seeds and aggregate statistics (mean, median, quantiles).
- **Plot distributions**: final P&L histograms and boxplots are more informative than single-run traces.

---

## Example analysis recipes

- To find an optimal threshold: for each threshold value perform `M` simulations and compute `(mean_final_pnl - transaction_cost)/std_final_pnl` or simply mean final P&L. Choose threshold with highest mean final P&L adjusting for risk.
- To evaluate robustness: sweep `sigma_real` while keeping `sigma_model` fixed, and plot final P&L vs realized sigma.

---

## Interview talking points

- Explain the theory: continuous-time delta hedging reduces replication error as rebalancing frequency increases, but discrete rebalancing and transaction costs cause trade-offs.
- Quantify trade-offs with plots: hedging RMSE vs total transaction cost.
- Discuss model risk with the volatility mis-specification experiment.