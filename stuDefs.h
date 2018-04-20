typedef struct {
  char first[32];
  char last[32];
  int  age;
} PersonType;

typedef struct {
  PersonType basicInfo;
  char       stuNumber[12];
  float      gpa;
} StudentType;

typedef struct{
  char name[32];
  int multiCell;
} SeaSponges;

typedef struct{
  SeaSponges basics;
  int eyeNum;
  int limbNum;
} Placoderms;

typedef struct{
  Placoderms basics;
  int liveOnLand;
  float lungCapacity;
} Amphibians;

typedef struct{
  Amphibians basics;
  int warmBlood;
} Mammals;

typedef struct{
  Mammals basics;
  int intelligence;
  char lastName[32];
} HomoSapians;

