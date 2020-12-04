
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

/* **********************************************************************************
			Nick Eddy  CIS-236  Pam Dunn  Joiet Junior College  Program 13
		
	Write a program that stores information about movies, using an
  array of structures and a text file. This program modifies
  Program 12. 

                                (Program 12)

	Write a program that stores information about movies, using an
  array of structures.
Prompt the user for the number of movies to process. Then prompt
the user for the  title and number of downloads for each movie.
When all the data has been entered, 
display the list of movies and the number of downloads.You must
use functions as described in the requirements below.
***********************************************************************************
*/

typedef struct {			//structure declaration
  char name[21];	
  int number;
} film_t;

// function declarations
void addMovie(FILE * ptr);
void deleteMovie(FILE * ptr);
unsigned int displayMenu();
void displayMovieInfo(FILE * ptr);
void getMovieInfo();
FILE * openFile();
void updateMovie(FILE * ptr);

char const * file_name = "./accounts.dat";
int main() {

  FILE * pointer;

  pointer = openFile(); //calls function and passes structure by address, loop count variable
  if (pointer == NULL) {
    fprintf(stderr, "unable to open file %s\n", file_name);
    exit(EXIT_FAILURE);
  }

	unsigned int ans = displayMenu();

  while (ans != 5) {
    switch (ans) {
      case 1:
        addMovie(pointer);
        break;

      case 2:
        deleteMovie(pointer);
        break;

      case 3:
        updateMovie(pointer);
        break;

      case 4:
        displayMovieInfo(pointer);
        break;

      default:
        printf("Invalid Selection\n");
        break;
    }

  	ans = displayMenu(); 
	 	//calls fucntion to print with array and loop counts
	}
	fclose(pointer);

  return 0;
}

FILE * openFile() {

  FILE * cfPtr;
  if ((cfPtr = fopen(file_name, "wb")) == NULL) {
    puts("File could not be opened.\n");
  }
  else {
    // create clientData with default information
    film_t blankClient = { "", 0, };

    // output 10 blank records to file
    for (unsigned int i = 1; i <= 10; ++i) {          
      fwrite(&blankClient, sizeof(film_t), 1, cfPtr);
    }

    fclose (cfPtr); // fclose closes the file
  } 

  cfPtr = fopen(file_name, "rb+");
  return cfPtr;
}

unsigned int displayMenu() {
  bool complete = false;
  unsigned int request; // request number
  do {
    printf("%s", "Enter request\n"
          " 1. Add a movie\n"
          " 2. Delete a movie\n"
          " 3. Update a movie\n"
          " 4. Display all movies\n"
          " 5. Exit\n");

    scanf("%i", &request);
    if (request >= 1 || request <= 5) {
      complete = true;
    }
  } while(!complete);

  return request;      
} 

void addMovie(FILE * fPtr) {
		// obtain number of account to create
	printf("%s", "Enter new account number (1 - 10): ");
	unsigned int accountNum; // account number
	scanf("%d", &accountNum);

	// move file pointer to correct record in file                 
	fseek(fPtr, (accountNum - 1) * sizeof(film_t), SEEK_SET);

	// create clientData with default information
	film_t client = { "", 0};

	// read record from file                               
	fread(&client, sizeof(film_t), 1, fPtr);

	// display error if account already exists
	if (client.number != 0) {
		printf("Account #%d already contains information.\n", client.number);
	}
	else { // create record
    // user enters last name, first name and balance
    printf("%s", "Enter Moive Title, number of downloads? \n");
    scanf("%14s%9d", client.name, &client.number);

    // move file pointer to correct record in file   
    fseek(fPtr, (accountNum - 1) * sizeof(film_t), SEEK_SET);      

    // insert record in file                       
    fwrite(&client, sizeof(film_t), 1, fPtr);     
	}
}

void deleteMovie(FILE * ptr) {
  printf("%s", "Enter account number to delete (1 - 100): ");
  unsigned int accountNum; // account number
  scanf("%d", &accountNum);

  // move file pointer to correct record in file
  fseek(ptr, (accountNum - 1) * sizeof(film_t), SEEK_SET);

  film_t client; // stores record read from file
	
	// read record from file
	fread(&client, sizeof(film_t), 1, ptr);

  // display error if record does not exist
  if (client.number == 0) {
    printf("Account %d does not exist.\n", accountNum);
  }
	else { // delete record
    // move file pointer to correct record in file                 
    fseek(ptr, (accountNum - 1) * sizeof(film_t), SEEK_SET);

    film_t blankClient = { "", 0 }; // blank client

    // replace existing record with blank record
    fwrite(&blankClient, sizeof(film_t), 1, ptr);
	}
}

void updateMovie(FILE * ptr) {
  printf("%s", "Enter account to update (1 - 10): ");
  unsigned int account; // account number
  scanf("%d", &account);

  // move file pointer to correct record in file              
  fseek(ptr, (account - 1) * sizeof(film_t), SEEK_SET);

	// create clientData with no information
	film_t client = { "", 0};

	// read record from file
	fread(&client, sizeof(film_t), 1, ptr);

  // display error if account does not exist
  if (client.number == 0) {
    printf("Account #%d has no information.\n", account);
  } 
  else { // update record
    printf("%-6s%-16d\n\n", client.name, client.number);

    // request transaction amount from user
    printf("%s", "Enter Title and number of downloads: ");
    scanf("%s %d", client.name, &client.number);

    printf("%-6s%-16d\n", client.name, client.number);

		// move file pointer to correct record in file
		fseek(ptr, (account - 1) * sizeof(film_t), SEEK_SET);

		// write updated record over old record in file
		fwrite(&client, sizeof(film_t), 1, ptr);
	} 
}

void displayMovieInfo(FILE * ptr) {
  printf("%-6s%-16s\n", "Title", "Downloads");

  // read all records from file (until eof)
  while (!feof(ptr)) {
    // create clientData with default information
    film_t client = { "", 0, };

    int result = fread(&client, sizeof(film_t), 1, ptr);

    // display record
    if (result != 0 && client.number != 0) {
      printf("%-6s%-16d\n", client.name, client.number );
    }
  }
}

void displayMovies(film_t array[], int ad) {
  printf("\n%-30s%30s", "Title", "Downloads");		//prints labels
  printf("\n%-30s%30s", "=======", "=======");		//prints table line

  //loop to print each index of structure array
  for (int i = 0; i < ad; i++) {
    printf("\n%-30s %30d", array[i].name, array[i].number);
  }
}
