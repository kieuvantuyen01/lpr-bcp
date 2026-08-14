#!/usr/bin/env python3

import csv
import pathlib
import subprocess
import sys


def read_graph(path: pathlib.Path) -> tuple[int, list[tuple[int, int, int]]]:
    n = 0
    edges: list[tuple[int, int, int]] = []
    with path.open(encoding="utf-8") as handle:
        for raw_line in handle:
            fields = raw_line.split()
            if not fields or fields[0] == "c":
                continue
            if fields[0] == "p":
                n = int(fields[2])
            elif fields[0] == "e":
                u, v, distance = map(int, fields[1:4])
                if u != v:
                    edges.append((u - 1, v - 1, distance))
    return n, edges


def main() -> int:
    if len(sys.argv) != 4:
        raise SystemExit("usage: smoke_test.py EXECUTABLE INSTANCE OUTPUT_DIR")

    executable = pathlib.Path(sys.argv[1]).resolve()
    instance = pathlib.Path(sys.argv[2]).resolve()
    output_dir = pathlib.Path(sys.argv[3]).resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    run_csv = output_dir / "triangle.runs.csv"

    completed = subprocess.run(
        [str(executable), str(instance), "5", "3", "123", str(run_csv)],
        check=True,
        capture_output=True,
        text=True,
    )
    if "successes=3" not in completed.stdout:
        raise AssertionError(completed.stdout)

    with run_csv.open(newline="", encoding="utf-8") as handle:
        rows = list(csv.DictReader(handle))
    if len(rows) != 3 or any(row["success"] != "1" for row in rows):
        raise AssertionError(rows)
    if [int(row["seed"]) for row in rows] != [123, 124, 125]:
        raise AssertionError(rows)

    n, edges = read_graph(instance)
    solution_path = pathlib.Path(str(run_csv) + ".solutions.txt")
    lines = solution_path.read_text(encoding="utf-8").splitlines()
    color_lines = [lines[index] for index in range(1, len(lines), 2)]
    if len(color_lines) != 3:
        raise AssertionError(lines)
    for line in color_lines:
        colors = [int(value) for value in line.split()]
        if len(colors) != n or not all(0 <= color < 5 for color in colors):
            raise AssertionError(colors)
        for u, v, distance in edges:
            if abs(colors[u] - colors[v]) < distance:
                raise AssertionError((colors, u, v, distance))

    zero_csv = output_dir / "triangle-infeasible.runs.csv"
    zero_result = subprocess.run(
        [str(executable), str(instance), "1", "1", "77", str(zero_csv)],
        check=True,
        capture_output=True,
        text=True,
    )
    if "successes=0" not in zero_result.stdout or "successful_cpu_average=NA" not in zero_result.stdout:
        raise AssertionError(zero_result.stdout)
    with zero_csv.open(newline="", encoding="utf-8") as handle:
        zero_rows = list(csv.DictReader(handle))
    if len(zero_rows) != 1 or zero_rows[0]["success"] != "0":
        raise AssertionError(zero_rows)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
