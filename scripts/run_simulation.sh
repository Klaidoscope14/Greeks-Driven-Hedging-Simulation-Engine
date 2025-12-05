set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
RESULTS_DIR="$ROOT_DIR/results"

echo "Root: $ROOT_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$RESULTS_DIR"

cd "$BUILD_DIR"
cmake ..
make -j

echo "Running simulation..."
./options-mm

echo "Simulation finished. Outputs (CSV) are in $RESULTS_DIR"