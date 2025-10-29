#ifndef _CALLIB_H_
#define _CALLIB_H_

typedef struct _CALORIE {
    char name[40];
    float value;
} CALORIE;

int calregist(CALORIE *, int);
float calcalc(CALORIE *, int);

#endif