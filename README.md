# aecor

Statically typed, compiled programming language, transpiled to C. The compiler is self-hosted,
and written in `aecor` itself. You will need `gcc` installed and available in your PATH to be
able to compile `aecor` programs.

## Usage

### Bootstrapping

This stage takes the pre-compiled `bootstrap/stage0.c` file and compiles it to make the initial
compiler. It then uses this to re-generate the compiler and verify that it is correct. If successful,
the output is the `./bootstrap/aecor` executable which can be used to compile other programs.

```bash
$ ./meta/bootstrap.sh   # Generates ./bootstrap/aecor if successful
$ ./bootstrap/aecor -h
```

### Setting up environment [Optional, recommended]

In order to use `aecor` from outside the repository root, it is recommended to add the following to your `.bashrc`
(or other way of setting up envionment in your shell)

```bash
# To find libraries
export AECOR_ROOT="/path/to/aecor/repo/"
# To add compiler to path
export PATH="$PATH:$AECOR_ROOT/bootstrap/"
```

### Compiling other programs

For now, the compiler _needs_ to be run from the root of the repository, since it uses relative paths.
By default, the compiler will generate C code and also compile it, but this can be customized:

```bash
# generates ./build/out and ./build/out.c
$ ./bootstrap/aecor /path/to/file.ae

# generates ./build/out and ./build/temp.c
$ ./bootstrap/aecor /path/to/file.ae -o ./build/out -c /build/temp.c

# generates only ./temp.c
$ ./bootstrap/aecor /path/to/file.ae -n -c ./temp.c
```

### Running tests

The `meta/test.py` script can be used to run the tests. It takes in the path to the compiler executable 
to be tested, and paths to the files/directories containing the tests.

_Note: Specify `./` before the executable name for relative paths._

```bash
# Test the bootstrapped compiler on a particular test
$ python3 test.py -c ./bootstrap/aecor /path/to/file.ae

# Test the bootstrapped compiler on all tests in given directories
$ python3 test.py -c ./bootstrap/aecor /folder/with/tests /another/one

# Run all the tests (no arguments)
$ python3 test.py -c ./build/aecor
```

### Development

If you wish to develop on the compiler, here is my workflow, which may be helpful:

```bash
$ ./meta/bootstrap.sh
# > Add feature to compiler source code
$ ./bootstrap/aecor compiler/main.ae -o build/aecor
# > Add a test for the new feature
$ python3 meta/test.py -c ./build/aecor # make sure it all passes!
```

Essentially, keep `bootstrap/aecor` as the canonical compiler which will not break, and `build/aecor` 
as the temporary build for testing.

Once significant language features are implemented, we should update the bootstrapped files before
we actually use any of these features in the compiler itself, to avoid any circular issues. For instance: if add
feature XYZ to the language and immediately use feature XYZ in the compiler (without updating the bootstrap) it will not work.

To update the bootstrap, run `./meta/gen_bootstrap.sh`, which performs some sanity checks and then generates the bootstrap.
The script requires all the tests to pass.
