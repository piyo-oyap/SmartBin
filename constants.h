#define dhtPin 10
#define ir1 11
#define ir2 12
#define servoPin 7
#define MAX 200
#define BIOSAMPLES 3
#define plateFlat 92
#define paletBio 20
#define plateNonBio 92

int range_cm, range1_cm, bio = 0;
int temp, humid;
bool cap, ind;
char str[32];
