#!/usr/bin/env python3
"""
Small helper to read a CSV produced by the sim and plot the price path.
Requires pandas and matplotlib.
Usage: python3 scripts/generate_price_paths.py results/simulation_output.csv
"""

import sys
import pandas as pd
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("Usage: generate_price_paths.py <csv_file>")
    sys.exit(1)

csv = sys.argv[1]
df = pd.read_csv(csv)
plt.plot(df['time_step'], df['spot'], marker='o')
plt.xlabel('time_step')
plt.ylabel('spot')
plt.title('Underlying Price Path')
plt.grid(True)
plt.show()