from sys import argv
from pathlib import Path
from os import system
import subprocess


def get_expected(filename):
    expected = {}
    with open(filename) as f:
        for line in f:
            if not line.startswith("///"):
                break
            line = line[3:].strip()
            if line == "":
                continue

            if line == "fail":
                expected[line] = True
                break

            if ":" not in line:
                print(f'[-] Invalid parameters in {filename}: "{line}"')
                return None

            name, value = map(str.strip, line.split(":"))
            if name == "exit":
                expected[name] = int(value)
            elif name == "out":
                expected[name] = value
            else:
                print(f'[-] Invalid parameter in {filename}: {line}')
                return None

    if not expected:
        return None

    return expected


def handle_test(path, expected):
    if system(f'./compiler {str(path)}') != 0:
        print(f'  {path} - FAIL (compilation failed)')
        return expected.get("fail")

    process = subprocess.run(['./test'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if "exit" in expected:
        if process.returncode != expected["exit"]:
            print(f'Expected exit code {expected["exit"]}, got {process.returncode}')
            return False

    if "out" in expected:
        output = process.stdout.decode('utf-8')
        if output != expected["out"]:
            print(f'Expected output {repr(expected["out"])}, got {repr(output)}')
            return False

    return True


def main(test_paths: list[str]):
    if len(test_paths) == 0:
        test_paths = [Path(__file__).parent / "tests"]

    test_paths = [Path(pth) for pth in test_paths]

    system("make")

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
    main(argv[1:])
