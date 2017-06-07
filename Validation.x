struct data {
 char cleBallon[256];
 char RFID[32];
};
typedef struct data data;
struct reponse {
  int ok;
};
typedef struct reponse reponse;
program ValidationBut{
  version VERSION_UN{
    reponse Validation(data) = 1;
  } = 1;
} = 0x20000001;