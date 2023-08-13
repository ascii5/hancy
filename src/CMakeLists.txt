SET(SRC_LIST ./lock/locker.h ./log/blockQueue.h ./log/log.h ./log/log.cc main.cc)
ADD_EXECUTABLE(hello ${SRC_LIST})