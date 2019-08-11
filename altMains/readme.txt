Directory contains test harnesses for framework.

Many of the test harnesses are "main" programs.
Process:  rename one of the mainxxx.cpp to main.cpp and rename main() in former main.cpp to hide it, eg. mainxxx()
Inside the mainxxx.cpp, rename mainxxx() to main.

Exclude from build any mainxxx.cpp that redefines names.