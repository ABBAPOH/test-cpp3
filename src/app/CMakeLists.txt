set(SOURCES
    clientthreadpool.cpp
    clientthreadpool_p.h
    clientthreadpool.h
    main.cpp
    runnable.h
    threadpool.cpp
    threadpool.h
)

add_executable(test-cpp3 ${SOURCES})
