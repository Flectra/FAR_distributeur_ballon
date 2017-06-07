struct data1{
	int ballonDispo;
};
typedef struct data1 data1;

struct reponse1{
	char cleballon[256];
};
typedef struct reponse1 reponse1;

struct data2{
	char cleballon[256];
};
typedef struct data1 data1;

struct reponse1{
	int ok;
};
typedef struct reponse1 reponse1;

program Validation{
	version VERSION_UN{		 
		reponse1 Validation(data1) =1;
	} = 1;
} = 0x20000001;


program distribution{
	version VERSION_UN{		 
		reponse2 distribution(data2) =1;
	} = 2;
} = 0x20000001;