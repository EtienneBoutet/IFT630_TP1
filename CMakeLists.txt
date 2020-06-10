# CMakeList.txt : CMake project for IFT630-TP1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("IFT630-TP1")

# Add source to this project's executable.
add_executable (devoir1 "devoir1.cpp"    "Philosopher.h" "PhilosopherSemaphore.h" "PhilosopherMonitor.h" "Random.h" "PhilosopherConstants.h")

add_executable (devoir2 "devoir2.cpp"   "CartMonitor.h" "CartSemaphore.h" "CartConstants.h")

# TODO: Add tests and install targets if needed.
