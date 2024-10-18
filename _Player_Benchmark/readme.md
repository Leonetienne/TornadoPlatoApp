# Benchmarking suite

## Create performance metrics
1. Compile as usual (SDL2 required): `cmake -B build && cd build && make`.
2. Run benchmark: `./Benchmark.out`

If you want to skip a benchmark, press SPACE to advance to the next benchmarking scene.

## Inspect benchmark results
Compile them with python...
1. `cd dataplotter`
2. Install all requirements in `Reqiurements.txt` with pip.
3. Run `./plot-metrics.py ./performance-plot/2024-12-31/19-31-59`
4. `open performance-plots` to view created png graphs

## Compare benchmark results
Compile them with python...
1. `cd dataplotter`
2. Install all requirements in `Reqiurements.txt` with pip.
3. Run `./plot-metrics.py ./performance-plot/2024-12-31/19-31-59 --compare-to ./performance-plot/2024-12-31/19-39-21`
4. `open performance-plots` to view created png graphs. The `--compare-to` results set will be overlaid with reduced transparency.

