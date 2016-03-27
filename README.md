# Project PIVO

[![Build Status](https://travis-ci.org/ProjectPIVO/pivo-core.svg?branch=master)](https://travis-ci.org/ProjectPIVO/pivo-core)

Project PIVO (Profiler-Independent Visual Output) - implementation part of bachelor thesis.

Developed by: Martin Úbl (A13B0453P)  
Supervised by: Ing. Jindřich Skupa  
University of West Bohemia  
The Faculty of Applied Sciences, Department of Computer Science and Engineering

## Dependencies

- Platform-dependent build system (gcc + make, Visual Studio, etc.)
- CMake
- more dependencies relies on your module selection

## Structure

- `src` - core sources
- `modules` - here we put modules we want to build and use

## Build

First you need to clone `pivo-core` repository, move to `modules`, clone modules you want to use, move back to core root,
perform CMake routine, and then use your favourite buildsystem to build resulting executable file and libraries.

The whole routine may look like this:
```bash
# clone core repository
git clone https://github.com/ProjectPIVO/pivo-core.git
# move to core directory
cd pivo-core
# move to modules directory
cd modules
# clone gprof input module and html output module
git clone https://github.com/ProjectPIVO/pivo-input-gprof.git
git clone https://github.com/ProjectPIVO/pivo-output-html.git  
# move back to core root  
cd ..
# perform CMake routine
cmake .
# now use your favourite build toolchain, mine is make
make
```

Now your local PIVO suite should be ready

## Usage

After you sucessfully built your PIVO suite, you may want to move to `bin` subdirectory, and invoke `pivo-core` command with
appropriate parameters for your case

### Example

```bash
./pivo-core -im gprof -om html -i /path/to/gmon.out -b ../binpath/a.out
```

This will invoke `pivo-core` with `gprof` input module and `html` output module. Next we specify input file (in this case gprof
output report file) and path to binary executable that has been analyzed using specified tool (gprof, ..) (paths may be both absolute
and relative)

### List of parameters

- `-im` - (mandatory) specifies input module (gprof, perf, dummy, ..)
- `-om` - (mandatory) specifies output module (html, ..)
- `-i` - (mandatory) specifies input file, in most cases, the profiler output (gmon.out, perf.data, ..)
- `-b` - specifies executable file that was analysed using profiling tool
- `-ll` - log level (0 = OFF, 1 = ERROR, 2 = WARNING, 3 = INFO, 4 = VERBOSE, 5 = DEBUG) (default 2 = WARNING)
- `-lf` - log file (default off)
- `-s` - silent mode (no output at all)

## Reporting issue

Feel free to use GitHub issue tracker to open an issue. Please, try to use appropriate repository to submit issue to - if
the problem lies within specific module, use tracker in its own repository

## Have an enhacement idea?

If you happen to have some useful idea, you may eiter use GitHub issue tracker as mentioned before, or fork appropriate
repository and submit pull request.

The same applies for creating new input or output module. You may want to fork `pivo-input-dummy` repository when starting a new 
input module project.
