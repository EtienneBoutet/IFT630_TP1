cmake_minimum_required (VERSION 3.8)

project ("IFT630-TP1")

add_executable (devoir1 "devoir1.cpp"   "Philosopher.h" "PhilosopherSemaphore.h" "PhilosopherMonitor.h" "Random.h" "PhilosopherConstants.h" "semaphore.h")

add_executable (devoir2 "devoir2.cpp"   "CartMonitor.h" "CartSemaphore.h" "CartConstants.h" "semaphore.h")

add_executable (devoir3 "devoir3.cpp" "semaphore.h"  )

add_executable (devoir4 "devoir4.cpp" "semaphore.h" "Canal.h" )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(devoir1 PRIVATE Threads::Threads)
target_link_libraries(devoir2 PRIVATE Threads::Threads)
target_link_libraries(devoir3 PRIVATE Threads::Threads)
target_link_libraries(devoir4 PRIVATE Threads::Threads)
