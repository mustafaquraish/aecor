from sys import argv
from pathlib import Path
from os import walk, system
from enum import Enum
import subprocess

def get_expected(filename):
    expected = {}
    with open(filename) as f:
        for line in f:
            if not line.startswith("///"):
                break
            name, value = map(str.strip, line[3:].split(":"))
            if name == "exit":
                expected[name] = int(value)
            if name == "out":
                expected[name] = value
    if not expected:
        return None
    return expected

class TestState(Enum):
    PASS = 1
    FAIL = 2
    SKIP = 3

def handle_test(path, expected):
    if system(f'./compiler {str(path)}') != 0:
        return TestState.FAIL

    process = subprocess.run(['./test'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if "exit" in expected:
        if process.returncode != expected["exit"]:
            return TestState.FAIL

    if "out" in expected:
        if process.stdout.decode('utf-8') != expected["out"]:
            return TestState.FAIL

    return TestState.PASS



def main():
    if len(argv) == 1:
        test_locations = [Path(__file__).parent / "tests"]
    else:
        test_locations = [Path(arg) for arg in argv[1:]]

    system("make")

    tests_to_run = []
    for test_location in test_locations:
        for root, _, files in walk(test_location):
            for file in files:
                path = Path(root) / file
                if expected := get_expected(path):
                    tests_to_run.append((path, expected))

    print(f'Running {len(tests_to_run)} tests:')
    for path, expected in tests_to_run:
        state = handle_test(path, expected)
        if state == TestState.PASS:
            print(f'  {path} - PASS')
        elif state == TestState.FAIL:
            print(f'  {path} - FAIL')


if __name__ == "__main__":
    main()