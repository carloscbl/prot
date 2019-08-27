# Prot
Is a C++ core of task scheduler, with support for users and auto-task setter based on user questionaries for recurrent tasks.

Powered by a engine parser of json forms, that provides the set up, pipeline and question/answer front end to custom the auto-task collection called "taskstories".

## Quick - Setup

I provide a script for dependencess installation for debian like systems

```bash
./configure_and_dependencies.sh
```
## Installation

To compile the project you need to have:
- Installed boost 1.69 libs and includes in your machine as CMake will look for it in the local machine
- C++ 17 compiler
- cmake > 3.10

```bash
sudo apt install gcc g++ make cmake
```
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
