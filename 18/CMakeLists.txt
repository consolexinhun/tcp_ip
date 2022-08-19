cmake_minimum_required(VERSION 3.16)
project(18)

include_directories(
    # /usr/include
)

link_directories(

)

add_executable(thread1 thread1.c)
target_link_libraries(
    thread1
    pthread
)

add_executable(thread2 thread2.c)
target_link_libraries(
    thread2
    pthread
)

add_executable(thread3 thread3.c)
target_link_libraries(
    thread3
    pthread
)

add_executable(thread4 thread4.c)
target_link_libraries(
    thread4
    pthread
)

add_executable(mutex mutex.c)
target_link_libraries(
    mutex
    pthread
)

add_executable(semaphore semaphore.c)
target_link_libraries(
    semaphore
    pthread
)