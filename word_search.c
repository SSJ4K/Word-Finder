/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


# define max 370100
# define maxword 255



typedef struct word{
  char data[maxword];
  struct word * next;
}words;

words * hashtable[max]; // Using pointers because we don't need the space for the full table unless we actually fill up the table
                        // Another is because it's now easy to tell when the spot on a table is empty, because we can just set it to null    



unsigned int hash(char name[maxword]);
void init_hash_table();
void print_table();
bool hashtable_insert(words *p);
words * hashtable_lookup (char *data);
void searchGrid(char** grid, int row, int col, char** argv);
char** makeGrid(char** grid, int wor, int col, char** argv);
void destroyGrid(char** grid, int row);



int main(int argc, char** argv)
{
 	 	
 	
 	char** grid = NULL;
    // Opens the lexis and inserts the words into the hash table
    FILE* dictionary = fopen("lexis.txt", "r");
    if (dictionary == NULL)
    {
        printf("Unable to open file!\n");
    }
    

    char* dict_word = calloc(maxword+ 1,sizeof(char)); 

    while(fscanf(dictionary, "%s", dict_word) != EOF)
    {
        words* word1 = malloc(sizeof(words));
        strcpy(word1->data,dict_word);
        
		hashtable_insert(word1);
	}
    
    
    // Open puzzle file to get number of lines
     FILE *pF = fopen(argv[1], "r"); 

	   if(pF == NULL)
	   {
	      printf("Unable to open file!\n");
	   }
	   
	   int current = 1;
	   char a;
	   
	   do{
	   	a = fgetc(pF);
	   	if (a == '\n'){
	   		current++;
		   }
	   }while (a != EOF);
	   
	   
	fclose(pF);
	

  	
  	grid = makeGrid(grid, current, current, argv);
  	searchGrid(grid, current, current, argv);
  	destroyGrid(grid, current);
  	
  	
    return 0;
}




/*
	Title: Understanding and implementing a Hash Table (in C)
	Author: Jacob Sorber
	Date: January 20, 2020
	Availability: https://www.youtube.com/watch?v=2Ti5yvumFTU&t=746s
*/

unsigned int hash(char name[maxword]){ // hash functiion
    int len = strlen(name);
    int i;
    unsigned int ascii_sum = 0;
    
    for (i = 0; i < len; i++){
        ascii_sum += name[i];
        ascii_sum = (ascii_sum * name[i]) % max; // Toget a more random result
    }
    return ascii_sum;
}


/*
	Title: Understanding and implementing a Hash Table (in C)
	Author: Jacob Sorber
	Date: January 20, 2020
	Availability: https://www.youtube.com/watch?v=2Ti5yvumFTU&t=746s
*/

void init_hash_table(){  // Sets each of the pointers equal to null, initaily sets the table to empty
    int i;
    
    for (i = 0; i < max; i++){
        hashtable[i] = NULL;
    }
}


/*
	Title: Understanding and implementing a Hash Table (in C)
	Author: Jacob Sorber
	Date: January 20, 2020
	Availability: https://www.youtube.com/watch?v=2Ti5yvumFTU&t=746s
*/


bool hashtable_insert(words *p){ // inserts words into table
    if (p == NULL){
        return false;
    }
    int index = hash(p->data);
    
    p->next = hashtable[index];
    hashtable[index] = p;
    return true;
}


/*
	Title: Understanding and implementing a Hash Table (in C)
	Author: Jacob Sorber
	Date: January 20, 2020
	Availability: https://www.youtube.com/watch?v=2Ti5yvumFTU&t=746s
*/

words * hashtable_lookup (char *data){  // search, no loops are used giving us constant time, this is the beauty of hash tables
    int index = hash(data);
    words *temp = hashtable[index];
    while (temp != NULL && strcmp(temp->data, data)){
        temp = temp->next;
    }
    return temp;
    
}


/*
	Title: WordSearch.c
	Author: Isaias Perez Vega
	Date: December 7, 2018
	Availability: https://github.com/wolfofalgstreet/WordSearch/blob/master/WordSearch.c
*/

char** makeGrid(char** grid, int row, int col, char** argv) {	// Creates the word search grid
	 

	// Open again to store lines in a 2d array
	FILE *fP = fopen(argv[1], "r");
	
	int i=0, j=0;
    char puzzle[row][col+1];
	
	if(fP == NULL)
	   {
	      printf("Unable to open file!\n");
	   }
	   
	   for (i=0; i < row; i++){
	   	for (j = 0; j<col; j++){
	   		fscanf(fP, "%c", &puzzle[i][j]);
		   }
		   fscanf(fP, "\n");
	   }

   fclose(fP);
   
   
    grid = malloc(sizeof(char*) * row);
    if (grid) {
    	int x = 0;
        for (x = 0; x < row; x++) {
            // Memory for null terminator included
            grid[x] = malloc(sizeof(char) * col + 1);
        }
    
        for (x = 0; x < row; x++) {
            strcpy(grid[x], puzzle[x]);
        }
    }
    	
    return grid;
}




/*
	Title: WordSearch.c
	Author: Isaias Perez Vega
	Date: December 7, 2018
	Availability: https://github.com/wolfofalgstreet/WordSearch/blob/master/WordSearch.c
*/

void searchGrid(char** grid, int row, int col, char** argv) {  // Search through the grid in every direction and prints any word found
    const int DX_SIZE = 8;
    const int DX[] = {-1,-1,-1,0,0,1,1,1};
    const int DY[] = {-1,0,1,-1,1,-1,0,1};
    int newX, newY;
    int keyWordCounter = 0;

     
    char* keyWord = malloc(sizeof(char) * (col + 1));
    int z = strtol(argv[2], NULL, 10);

    printf("\nWords Found in Grid:\n");
    printf("________________________\n\n");

    // Traveling
    int x;
    int y;
    int direction;
    for (x = 0; x < row; x++) {
        for (y = 0; y < col; y++) {

            // Looking in all directions
            for (direction = 0; direction < DX_SIZE; direction++) {
                keyWordCounter = 0;
                newX = x; newY = y;

                // Run as long as we are in bounds
                while ((newX >= 0 && newX < row) && (newY >= 0 && newY < col)) {
                    keyWord[keyWordCounter + 1] = '\0';

                    keyWord[keyWordCounter] = grid[newX][newY];

                    // If keyword meets requirements (MIN_STRING_LENGTH < length < MAX_STRING_LENGTH),
                    // check if it exists in dictionary
                    if (strlen(keyWord) >= z) {
                    	words *temp = hashtable_lookup(keyWord);
					    if (temp != NULL){
                            printf("|%s\t\t|\n", keyWord);
					    }
                       
                    }

                    // Moving
                    newX += DX[direction]; newY += DY[direction];
                    keyWordCounter++;
                }
            }
        }
    }
    printf("________________________");
    free(keyWord);
}


/*
	Title: WordSearch.c
	Author: Isaias Perez Vega
	Date: December 7, 2018
	Availability: https://github.com/wolfofalgstreet/WordSearch/blob/master/WordSearch.c
*/

void destroyGrid(char** grid, int row) { // frees allocated memory
	int x;
    for (x = 0; x < row; x++) {
        free(grid[x]);
    }
    free(grid); grid = NULL;
}


