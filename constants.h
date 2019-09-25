#ifndef BIN_CONSTS
#define BIN_CONSTS

#define pinDht 10
#define pinServo 7
#define ir1 11
#define ir2 12
#define MAX 200
#define BIOSAMPLES 3
#define plateFlat 92
#define plateBio 20
#define plateNonBio 160

int range_cm, range1_cm, bio = 0;
int temp, humid;
bool cap, ind;
char str[32];
#endif