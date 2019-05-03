#define PATH "dataDogs.dat"
#define T_SIZE 1009

typedef struct dogType{

    char name[33];
    char type[33];
    int age;
    char breed[17];
    int size;
    float weight;
    char sex;
    int doc_id;
    int prev;
    int next;
} dogType;
