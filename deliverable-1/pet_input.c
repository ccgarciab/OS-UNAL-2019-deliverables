#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"

/*Read a line up to [bound] chars.
    If the line is too long, truncates it and returns 0*/
int get_bounded_str(char *buffer, unsigned int bound){

    int i = 0;
    char c;
    for(c = (char)fgetc(stdin); c != '\n' && i <= bound; i++){

        buffer[i] = c;
        c = (char)fgetc(stdin);
    }

    buffer[i] = '\0';

    if(bound < i){

        if(c != '\n') while(fgetc(stdin) != '\n');
        return 0;
    }

    return 1;
}


/*Verifies that a string is comprised of just digits*/
int is_int_str(char* str){

    do{

        if(*str < '0' || '9' < *str) return 0;

    }while(*(++str));

    return 1;
}

/*Verifies that a string represents a floating point number*/
int is_float_str(char* s) {

	if(s[0] != '-' && !isdigit(s[0]))return 0;
	if(s[0] == '-'&& !isdigit(s[1]))return 0;

	int ind;

	for(ind = 1; s[ind] != '\0'; ind++){

		if(s[ind] == '.') break ;
		if(!isdigit(s[ind])) return 0;
	}

	if(s[ind] == '\0') return 1;
	if(s[ind] == '.' && s[ind + 1] == '\0') return 0;

	for(ind++ ; s[ind] != '\0'; ind++){

		if(!isdigit(s[ind])) return 0;
	}

	return 1;
}

/*Uses stdin to get all info for a pet record.
    Handles invalid input by 'asking' again*/
void fill_pet_info(dogType *pet){

    char prompts[4][6] = {"name", "type", "breed", "sex"};
    char sex[2];
    char *ptrs[4] = {pet->name, pet->type, pet->breed, sex};
    int sizes[4] = {32, 32, 16, 1};

    for(int i = 0; i < 4; i++){

        while(1){
            printf("%s: ", prompts[i]);
            if(!get_bounded_str(ptrs[i], sizes[i])) continue;
            if(ptrs[i][0] == '\0') continue;
            if(i == 3 && sex[0] != 'H' && sex[0] != 'M') continue;
            break;
        }
    }

    pet->sex = sex[0];

    char prompts_n[2][7] = {"age", "size"};
    int *ptrs_n[2] = {&(pet->age), &(pet->size)};
    char numbuf[20];
    long n;

    for(int i = 0; i < 2; i++){

        while(1){

            printf("%s: ", prompts_n[i]);
            if(!get_bounded_str(numbuf, 10)) continue;
            if(!is_int_str(numbuf)) continue;
            n = atol(numbuf);
            if(2147483647 < n) continue;
            *ptrs_n[i] = (int)n;
            break;
        }
    }

    while(1){

        printf("weight: ");
        get_bounded_str(numbuf, 19);
        if(!is_float_str(numbuf)) continue;
        pet->weight = atof(numbuf);
        break;
    }
}

/*Get a non-negative int from stdin*/
int get_int(char *msg){

    long n;
    char numbuf[20];
    while(!get_bounded_str(numbuf, 10) ||
          !is_int_str(numbuf) ||
          (n = atol(numbuf)) > 2147483647){

        printf("%s", msg);
    }

    return (int)n;
}
