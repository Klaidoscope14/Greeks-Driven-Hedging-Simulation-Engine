# Performance Measurement & Benchmarking

This document explains how to measure the performance of the Options Market Maker simulation, what to benchmark, recommended tools, and how to interpret results. Performance is important for quant roles — both algorithmic correctness and runtime characteristics matter.

---

## Benchmarks to collect

1. **Single-option pricing time**
   - Measure `time_per_price` for `BinomialTree::price` for a given `N`.
   - Run `K` iterations and take mean & std.
   - Example: `N = 200, iterations = 1000`.

2. **Greeks computation time**
   - Compute time to obtain all Greeks (delta, gamma, vega) via numeric wrappers.
   - Compare tree-based Greek computation vs analytic (Black–Scholes) where applicable.

3. **Per-tick simulation throughput**
   - Time to process one tick in the main loop (price -> hedge -> execute -> log).
   - Useful metric for how many ticks/second the engine can simulate.

4. **Full scenario runtime**
   - Time for a full simulation of `steps` ticks for `M` Monte Carlo paths.
   - Example: `steps = 1000`, `M = 100` (parallelizable across seeds).

5. **Memory usage**
   - Peak memory footprint of a simulation run. Track allocations if you use large N or many concurrent paths.

6. **Scaling experiments**
   - Runtime vs `N` (binomial steps).
   - Runtime vs `steps` (simulation length).
   - Runtime vs `M` (Monte Carlo paths).

---

## Measurement recipes

### Simple microbenchmark in C++
Use `std::chrono::steady_clock` to measure time. Example:

```cpp
auto start = std::chrono::steady_clock::now();
// call function
auto end = std::chrono::steady_clock::now();
auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();