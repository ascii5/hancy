#include "heapTimer.h"
#include <iostream>

int main(){
    heapTimer timerManager;
    //50 is the seconds
    int id = 0;

    for(int i = 2;i <= 100;i += 2,id++){
        std::time_t expire = std::time(nullptr) + i;
    
        struct client_data* client = new  struct client_data;
        sockaddr_in timerSockAddr;
        memset(&timerSockAddr,0,sizeof(sockaddr));
        
        util_timer timer;
        timer.id = id;
        timer.cb_func = cb_func;
        timer.expire = expire;
        timer.user_data = client;


        client->address = timerSockAddr;
        client->sockfd = 0;
        client->timer = &timer;
        
        //timerManager.addTimer(timer);

    }
    

    
    sleep(13);

    timerManager.tick();

    std::cout<<"timer count:   "<<timerManager.getTimerCount()<<std::endl;
    timerManager.pop();
    std::cout<<"timer count:   "<<timerManager.getTimerCount()<<std::endl;


    int nextTickTime = timerManager.GetNextTick();
    std::cout<<nextTickTime<<std::endl;

    std::cout<<"timer count:   "<<timerManager.getTimerCount()<<std::endl;
    timerManager.doWork(timerManager.getFrontTimer().id);
    std::cout<<"timer count:   "<<timerManager.getTimerCount()<<std::endl;

    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;

    std::cout<<"======================test the heapTimer order or not======================"<<std::endl;
    // timerManager.adjust(timerManager.getFrontTimer().id,50);
    // timerManager.adjust(timerManager.getFrontTimer().id,52);
    // timerManager.adjust(timerManager.getFrontTimer().id,53);
    // timerManager.adjust(timerManager.getFrontTimer().id,55);
    timerManager.printTimerExpire();
    

    return 0;
}