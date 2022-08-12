#!/usr/bin/env python3
import subprocess
import argparse
from ast import literal_eval
from dataclasses import dataclass
from enum import Enum
from os import system, makedirs
from pathlib import Path
from sys import argv
from typing import Union, Optional, Tuple

class ExpectedOutputType(Enum):
    FAIL = 1
    EXIT_WITH_CODE = 2
    EXIT_WITH_OUTPUT = 3


@dataclass(frozen=True)
class Expected:
    type: ExpectedOutputType
    value: Union[int, str, None]


def get_expected(filename) -> Optional[Expected]:
    expected = None
    with open(filename) as file:
        for line in file:
            if not line.startswith("///"):
                break

            line = line[3:].strip()

            if line == "":
                continue

            if ":" not in line:
                print(f'[-] Invalid parameters in {filename}: "{line}"')
                break

            name, value = map(str.strip, line.split(":"))
            if name == "exit":
                expected = Expected(
                    type=ExpectedOutputType.EXIT_WITH_CODE,
                    value=int(value),
                )
                break
            elif name == "out":
                expected = Expected(
                    type=ExpectedOutputType.EXIT_WITH_OUTPUT,
                    value=value,
                )
                break
            elif name == "fail":
                expected = Expected(
                    type=ExpectedOutputType.FAIL,
                    value=value,
                )
            else:
                print(f'[-] Invalid parameter in {filename}: {line}')
                break

    return expected


def handle_test(compiler: str, path: Path, expected: Expected) -> Tuple[bool, str]:
    process = subprocess.run([compiler, str(path), '-o', 'build/test'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if expected.type == ExpectedOutputType.FAIL:
        if process.returncode == 0:
            return False, "Expected compilation failure, but succeeded"

        error = process.stdout.decode("utf-8").strip()
        expected_error = expected.value

        if expected_error in error:
            return True, "(Success)"
        else:
            return False, f"Did not find expected error message: {expected_error}"
    elif process.returncode != 0:
        return False, "Compilation failed"

    process = subprocess.run(['./build/test'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if expected.type == ExpectedOutputType.EXIT_WITH_CODE:
        if process.returncode != expected.value:
            return False, "Expected exit code {expected.value}, but got {process.returncode}"

    if expected.type == ExpectedOutputType.EXIT_WITH_OUTPUT:
        output = process.stdout.decode('utf-8').strip()
        expected_out = literal_eval(expected.value).strip()
        if output != expected_out:
            return False, f'Expected output {repr(expected_out)}, got {repr(output)}'

    return True, "(Success)"


def main():
    parser = argparse.ArgumentParser(description="Runs aecor test suite")
    parser.add_argument(
        "-c",
        "--compiler",
        required=True,
        help="Runs the self-hosted version"
    )
    parser.add_argument(
        "files",
        nargs="?",
        default=["tests"],
        help="Files / folders to run"
    )
    args = parser.parse_args()
    test_paths = [Path(pth) for pth in args.files]

    tests_to_run = []
    for path in test_paths:
        files = []

        if path.is_dir():
            for path_ in path.glob('**/*'):
                if path_.is_file():
                    files.append(path_)
        else:
            files.append(path)

        for file in files:
            if expected := get_expected(file):
                tests_to_run.append((file, expected))
            else:
                print(f"[-] Skipping test: {file}")

    num_passed = 0
    num_failed = 0
    num_total = len(tests_to_run)

    for i, (path, expected) in enumerate(tests_to_run):
        print(f" \33[2K[\033[92m{num_passed:3d}\033[0m", end="")
        print(f"/\033[91m{num_failed:3d}\033[0m]", end="")
        print(f" Running test {i+1}/{num_total}: {path}\r", end="", flush=True)
        passed, msg = handle_test(args.compiler, path, expected)
        if passed:
            num_passed += 1
        else:
            num_failed += 1
            print(f"\33[2K\033[91m[-] Failed {path}\033[0m")
            print(f"  - {msg}", flush=True)
    print("\33[2K")
    print(f"Tests passed: \033[92m{num_passed}\033[0m")
    print(f"Tests failed: \033[91m{num_failed}\033[0m")
    if num_failed > 0:
        exit(1)


if __name__ == "__main__":
    main()
