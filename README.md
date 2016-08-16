# ISL To Sage
Converting [ISL]() code-generated ASTs into [Rose's](http://rosecompiler.org/ROSE_HTML_Reference/index.html) SageIII ASTs

## Quickstart
For convienience, `make all` is suffecient to build the libarary (under lib/libisl_sage.a)

## Project Directory Structure
* src: Where the project's source .cpp code lives. Known as $(SRC).
* include: Where the project's .hpp headers live. Known as $(INCLUDE).
* lib: Where the final libisl_sage binary is output to. Known as $(LIB).
* bin: Where binaries created during the creation of libisl_sage live. Known as $(BIN).
* tests: Where tests (mainly test applications to easily test the library and api during development) live. Known as $(TEST).
  + src: Where tests source lives. Known as $(TEST_SRC).
  + bin: Where tests are compiled to. Known as $(TEST_BIN).
* third-party: Where third-party library's source and installation lives. Known as $(THIRD_PARTY).
  + source: Where third-party source lives. Known as $(TP_SRC).
  + build: (Created during `make initialize`) Where third-party libraries are extracted to and built. Known as $(TP_BUILD).
  + install: (Created during `make initialize`) Where third-party libraries are installed to after being built. Mimics the ususal install locations (like /usr/). Known as $(TP_INSTALL).

## Make Commands
* `all`: Produces the libisl_sage.a library, fulfiling all requirements from the ground up if necessary.
* `initialize`, `init`: Sets up third-party libraries and installs them locally. Necessary before building any parts of the project.
* `neat`: clean $(BIN) of .o files
* `clean-third-party`: restores $(THIRD_PARTY) to initial state.
* `clean`: clean $(BIN) and $(LIB) of all files
* `clean-all`: Same as `make clean-third-party clean`

## Library Requirements
* [Rose](http://rosecompiler.org/ROSE_HTML_Reference/index.html)
* [GMP](https://gmplib.org/)
* [LoopChainIR](https://github.com/CompOpt4Apps/LoopChainIR)* (Only if building a select few of the test applications.)

## Test Examples
Under $(TEST_SRC) there are a few runner apps that serve to test the ISL To Sage library.
Running them requires that you have the ISL library installed and acessible.
If you do not, a simple solution is to add $(TP_INSTALL)/lib to your $LD_LIBRARY_PATH environment variable, since ISL To Sage ships and builds with ISL.
In the root directory of ISL To Sage, run: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/third-party/install/lib`

In some cases, you may also need to have the [LoopChainIR](https://github.com/CompOpt4Apps/LoopChainIR) library installed somewhere as well.

## Third-Party materials
Included with this project are several third-party materials under the
$(THIRD_PARTY_SRC) directory.

1. ISL
  + [isl-0.15.tar.gz](http://isl.gforge.inria.fr/isl-0.15.tar.gz)
  + Under MIT License
  + Requires GMP (Not provided).

All third-party materials are provided as they were downloaded, and include their copyright and license information.
