#include <PointsCounter.h>

int PointsCounter::count(std::vector<int> wyniki) {
    int sum=0;
    int ct[]={0,0,0,0,0,0,0};
    for (int i:wyniki) {
        ct[i]+=1;
    }
    if (ct[3]>2) {
        //TODO GAME OVER
        return 0;
    }
    if (ct[3]>0) {
        return 0;
    }
    sum=50*ct[0]+20*ct[2]+200*ct[4]+120*ct[5];
    return sum;
};
