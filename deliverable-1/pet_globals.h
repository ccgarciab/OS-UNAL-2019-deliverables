//relative path of the file used as database
#define PATH "dataDogs.dat"

//size of the hashtable used in memory
#define T_SIZE 100151

typedef struct dogType{

    char name[33];
    char type[33];
    int age;
    char breed[17];
    int size;
    float weight;
    char sex;
    int doc_id; // Unique number for naming medical records
    int prev; // Line indicator of the next struct that has the same Hash Value
    int next; // Line indicator of the last struct that has the same Hash Value
} dogType;

typedef struct node{

    int line;

    //identifier to avoid collisions
    long signature;

} node;
