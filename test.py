import subprocess
from ast import literal_eval
from dataclasses import dataclass
from enum import Enum
from os import system
from pathlib import Path
from sys import argv
from typing import Union, Optional

class ExpectedOutputType(Enum):
    """Possible test results"""

    FAIL = 1
    EXIT_WITH_CODE = 2
    EXIT_WITH_OUTPUT = 3


@dataclass(frozen=True)
class Expected:
    """A container for the expected output of a test"""

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

            if line == "fail":
                expected = Expected(
                    type=ExpectedOutputType.FAIL,
                    value=None,
                )
                break

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
            else:
                print(f'[-] Invalid parameter in {filename}: {line}')
                break

    return expected


def handle_test(path: Path, expected: Expected) -> bool:
    process = subprocess.run(['./aecor', str(path)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if process.returncode != 0:
        if expected.type != ExpectedOutputType.FAIL:
            print(f'[-] Compiling the test code failed')
        return expected.type == ExpectedOutputType.FAIL

    process = subprocess.run(['./out'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if expected.type == ExpectedOutputType.EXIT_WITH_CODE:
        if process.returncode != expected.value:
            print(f'[-] Expected exit code {expected.value}, got {process.returncode}')
            return False

    if expected.type == ExpectedOutputType.EXIT_WITH_OUTPUT:
        output = process.stdout.decode('utf-8').strip()
        expected_out = literal_eval(expected.value).strip()
        if output != expected_out:
            print(f'[-] Expected output {repr(expected_out)}, got {repr(output)}')
            return False

    return True


def main():
    test_paths = argv[1:]
    if len(test_paths) == 0:
        test_paths = [Path(__file__).parent / "tests"]

    test_paths = [Path(pth) for pth in test_paths]

    if system("make") != 0:
        return 1

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

    print(f'Running {len(tests_to_run)} tests:')
    for path, expected in tests_to_run:
        passed = handle_test(path, expected)
        if passed:
            print(f'  {path} - PASS')
        else:
            print(f'  {path} - FAIL')


if __name__ == "__main__":
    main()
