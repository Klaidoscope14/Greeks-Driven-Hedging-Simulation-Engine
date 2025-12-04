# Pricing Model Notes — Binomial Tree & Black–Scholes

This document explains the pricing methodology used in the project, numerical choices, and validation recipes. It is designed to communicate both the mathematics and practical implementation decisions.

---

## Notation

- \(S\) : current spot price
- \(K\) : strike price
- \(r\) : risk-free interest rate (continuously compounded)
- \(\sigma\) : volatility (annualized)
- \(T\) : time to maturity (in years)
- \(N\) : number of steps in the binomial tree
- \(dt = T/N\)
- \(u = e^{\sigma \sqrt{dt}}\), \(d = 1/u\)  (Cox-Ross-Rubinstein parameterization)
- Risk-neutral probability \(p = \dfrac{e^{r dt} - d}{u - d}\)

---

## Binomial Tree (CRR) — recombining tree

### Tree construction (recombining)
- The stock can move up by factor \(u\) or down by factor \(d\) each step.
- At time \(N\), there are \(N+1\) unique terminal nodes with underlying prices:
  \[
  S_{N,i} = S \cdot u^{N-i} d^{i} \quad (i = 0 \ldots N)
  \]

### Pricing by backward induction
1. Compute payoffs at maturity for each node:
   - Call payoff: \(\max(0, S_{N,i} - K)\)
   - Put payoff: \(\max(0, K - S_{N,i})\)
2. Move backward: at each node
   \[
   V_{n,i} = e^{-r dt}\left( p V_{n+1,i} + (1-p) V_{n+1,i+1} \right)
   \]
   - For American options: allow early exercise:
     \[
     V_{n,i} = \max( V_{n,i}, \text{exercise payoff at } S_{n,i} )
     \]

### Complexity
- Time complexity is \(O(N^2)\) for naive full-tree computation. Memory can be reduced to \(O(N)\) using in-place backward induction (store a single row of node values and overwrite).

### Common numerical choices
- Use `u = exp(sigma * sqrt(dt))` and `d = 1/u` for stability.
- Use `p = (exp(r*dt) - d) / (u - d)`. If volatility is very small (u ≈ 1), handle edge cases with care.
- For deep trees (large `N`) take care with underflow/overflow when computing `pow(u, N)` — work in log-space if necessary.

---

## Black–Scholes closed form (for European options)

### Formula (call)
\[
C = S \Phi(d_1) - K e^{-rT} \Phi(d_2)
\]
where
\[
d_1 = \frac{\ln(S/K) + (r + \frac{1}{2}\sigma^2)T}{\sigma \sqrt{T}}, \quad
d_2 = d_1 - \sigma \sqrt{T}
\]
and \(\Phi\) is the standard normal CDF.

### Delta (call)
\[
\Delta_{call} = \Phi(d_1)
\]
For puts:
\[
\Delta_{put} = \Phi(d_1) - 1
\]

---

## Greeks — numerical approaches & practical tips

### Delta and Gamma (tree-based)
- Delta (at t=0) from the two immediate values:
  \[
  \Delta \approx \frac{V_{up} - V_{down}}{S u - S d}
  \]
- Gamma via finite-difference of delta or using 3-point second derivative approximation:
  \[
  \Gamma \approx \frac{\Delta(S+\varepsilon) - \Delta(S-\varepsilon)}{2 \varepsilon}
  \]
  where \( \varepsilon \) is a small bump in the spot price.

### Vega and Rho
- Use central finite differences:
  \[
  \text{Vega} \approx \frac{V(\sigma + \delta\sigma) - V(\sigma - \delta\sigma)}{2\delta\sigma}
  \]
  \[
  \rho \approx \frac{V(r + \delta r) - V(r - \delta r)}{2\delta r}
  \]

### Theta
- Reduce time-to-maturity by one (e.g., one day or `dt`) and compute difference:
  \[
  \Theta \approx \frac{V(T) - V(T - \delta t)}{\delta t}
  \]
  Note: Theta is usually reported per calendar day or per year depending on convention.

### Bump sizes
- Delta bumps: `dS = max(1e-4, 1e-3 * S)` is a reasonable start.
- Vol bump for vega: `dsig = 1e-4` or `1bp` (0.0001) — but scale to the sensitivity of the pricer.
- Interest rate bump: `dr = 1e-4` (0.01%).
- Avoid absolute bumps that are too small (numerical noise) or too large (nonlinearities).

---

## Validation & convergence

### Convergence test (binomial → Black–Scholes)
- For a European option, increase `N` and compare binomial price to Black–Scholes price.
- Convergence is monotonic in many CRR parameterizations but check signs and tolerances.

### Unit tests to include
- `|price_binomial(N=200) - price_bs| < tolerance` (tolerance e.g., `1e-3`).
- Greeks sanity checks: delta between -1 and 1, gamma >= 0 for vanilla options.
- For American vs European pricing: call early exercise shouldn't be profitable for non-dividend paying underlying (so American call ~ European call), but puts may show early exercise.

---

## Implementation notes & pitfalls

- **Edge case T→0**: If maturity is zero or near zero, use payoff directly.
- **Small sigma**: handle division by zero in `d1` calculation; treat as limit-case.
- **Numerical precision**: use `double`. If you need extremely deep trees or extremely long maturities, consider log-space computations.
- **Reuse**: if you price many options across the same set of `S u^i d^j` nodes, you can reuse partial computations.

---

## Practical parameter choices (starter)
- `N` for binomial: 100–400 for demonstrations. Larger N gives better accuracy at the expense of time.
- `dt` for GBM path: `T / steps`. Keep steps reasonably large (≥100) for smoother hedging studies.
- Hedging threshold: start around `0.5` shares for options with notional 100; tune in experiments.

---

## Final thoughts

The binomial tree is easy to implement and explain, which makes it excellent for interviews. Pair the binomial implementation with Black–Scholes for validation and add careful numeric tests (finite difference bump choices, edge-case handling). When you expand to more realistic settings (stochastic volatility or path-dependent payoffs), consider Monte Carlo pricers and variance reduction techniques.