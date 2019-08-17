Prot is a C++ core of task scheduler, with support for users and auto-task setter based on user questionaries for recurrent tasks.

Powered by a engine parser of json forms, that provides the set up, pipeline and question/answer front end to custom the auto-task collection called "taskstories".

To compile the project you need to have:
- Installed boost 1.69 libs and includes in your machine as CMake will look for it in the local machine
- C++ 17 compiler
- cmake > 3.10

sudo apt install gcc g++ make cmake

To build the application, from prot root directory execute the build.sh script:

./build.sh

It will compile using 7 cores by default but you can pass the number of cores to use as parameter:

./build.sh 4


It is unlicensed, you can only use it to learn from the code. Reach me for commercial traits or secondary uses.
Working to provide a commercial solution in the future .
