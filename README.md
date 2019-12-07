[![Codacy Badge](https://api.codacy.com/project/badge/Grade/2d4567f7bacd461aaec359bcb6e2c54c)](https://www.codacy.com/manual/carloscbl/prot?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=carloscbl/prot&amp;utm_campaign=Badge_Grade)

# Prot
Is a C++ core of task scheduler, with support for users and auto-task setter based on user questionaries for recurrent tasks.

Powered by a engine parser of json forms, that provides the set up, pipeline and question/answer front end to custom the auto-task collection called "taskstories".

## Quick - Setup

To install dependencies for debian like systems

```bash
./configure_and_dependencies.sh
```
## Manual Installation

To compile the project you need to have:
- Installed boost 1.69 libs and includes in your machine as CMake will look for it in the local machine
- C++ 17 compiler
- cmake > 3.10

```bash
sudo apt install gcc g++ make cmake
```
Then you need to install boost as like [Here](https://onethinglab.com/2019/01/30/how-to-install-latest-boost-library-on-ubuntu/).
Or follow dependencies.sh and custom your params

## Build

To build the application, from prot root directory execute the build.sh script:
```bash
./build.sh
```
It will compile using 7 cores by default but you can pass the number of cores to use as parameter:
```bash
./build.sh 4
```

## Docker

In development

## License
It is unlicensed, you can only use it to learn from the code. Reach me for commercial traits or secondary uses.
Working to provide a commercial solution in the future .
