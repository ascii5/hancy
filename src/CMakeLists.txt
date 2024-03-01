ADD_EXECUTABLE(server main.cpp)
INCLUDE_DIRECTORIES(/usr/include/mysql/mysql.h)
ADD_LIBRARY(serverlib "")
TARGET_LINK_LIBRARIES(serverlib
mysqlclient
)
TARGET_SOURCES(serverlib
PUBLIC
    ./config/config.h
    ./config/config.cpp
    ./lock/locker.h
    ./log/block_queue.h
    ./log/log.h
    ./log/log.cpp
    ./CGImysql/sql_connection_pool.h
    ./CGImysql/sql_connection_pool.cpp
    ./timer/heapTimer.h
    ./timer/heapTimer.cpp
    ./http/http_conn.h
    ./http/http_conn.cpp
    ./threadpool/threadpool.h
    ./webserver/webserver.h
    ./webserver/webserver.cpp
)
TARGET_LINK_LIBRARIES(server
PRIVATE
    serverlib
)