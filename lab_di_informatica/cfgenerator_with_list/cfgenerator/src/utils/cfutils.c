#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "utils/cfutils.h"
#include "list/list.h"
#include "cfgenerator.h"

bool is_male(const person* p) {
	//Precondition
	assert(p != NULL);

	bool answer = false;
	if(p -> sex == 'M') {
		answer = true;
	}
	return answer;
}


bool is_female(const person* p) {
	// Precondition
	assert(p != NULL);

	bool answer = false;
	if(p -> sex == 'F') {
			answer = true;
	}
	return answer;
}


bool is_surname_AL(const person* p) {
	// Precondition
	assert(p != NULL);

	bool answer = false;
	if(p -> surname[0] >= 'A' && p -> surname[0] <= 'L') {
		answer = true;
	}
	return answer;
}


bool is_surname_MZ(const person* p) {
	// Precondition
	assert(p != NULL);

	bool answer = false;
	if(p -> surname[0] >= 'M' && p -> surname[0] <= 'Z') {
		answer = true;
	}
	return answer;
}


bool is_male_AL(const person* p) {
	// precondition
	assert(p != NULL);

	return is_male(p) && is_surname_AL(p);
}


bool is_female_MZ(const person* p) {
	// precondition
	assert(p != NULL);

	return is_female(p) && is_surname_MZ(p);
}


void filter_people(list* people_list, filter_fn filter) {
	// Preconditions
	assert(people_list != NULL);
	assert(filter != NULL);

	node *person_node;

	/* Reads people list and check condition of filter,
	 * if condition is true, remove node from list
	 * else continue the reading of people list */
	person_node = head(people_list);
	while(person_node != NULL) {
		if(!filter(content(person_node))) {
			node *person_next = next(people_list, person_node);
			remove_node(people_list, person_node);
			delete_node(person_node);
 			person_node = person_next;
		} else {
			person_node = next(people_list, person_node);
		}
	}
}


void generate_cfs(list* people_list, FILE* places_file) {
	// Preconditions
	assert(people_list != NULL && places_file != NULL);

	node *person_node;

	// Read people list without cf, generate cf and save it
	person_node = head(people_list);
	while(person_node != NULL) {
		rewind(places_file);
		person *p_temp = content(person_node);
		cf_generator(p_temp -> surname, p_temp -> name, p_temp -> birth_date,
					 read_birth_place_code(places_file, p_temp -> birth_place),
					 p_temp -> sex, p_temp -> cf);
		person_node = next(people_list, person_node);
	}
}

char* read_birth_place_code(FILE* places_file,const char* birth_place) {
	// Preconditions
	assert(places_file != NULL);
	assert(birth_place != NULL);
	assert(strlen(birth_place) > 0);

	char place[MAX_BIRTH_PLACE] = "";
	bool found;
	char *coded_place;

	// Reads birth place names from file and searches their code from file
	found = false;
	coded_place = (char *)malloc(CODED_BIRTH_PLACE_LENGTH + 1);
	memset(coded_place, '\0', CODED_BIRTH_PLACE_LENGTH + 1);
	while(!feof(places_file) && !found) {
		strcpy(place, "");
		fscanf(places_file, "%[^,]%*c", place);
		if(strcmp(place, birth_place) == 0) {
			found = true;
			fscanf(places_file, "%[^\n]%*c", coded_place);
		} else {
			fscanf(places_file, "%[^\n]%*c", place);
		}
	}

	// Postconditions
	assert(coded_place != NULL && strlen(coded_place) == CODED_BIRTH_PLACE_LENGTH);

	return coded_place;
}

person* read_person(FILE* people_file) {
	// Precondition
	assert(people_file != NULL);

	person *p = (person *)malloc(sizeof(person));
	memset(p, '\0', sizeof(person));
	if(p != NULL) {
		fscanf(people_file, "%[^\n]%*c", p -> surname);
		fscanf(people_file, "%[^\n]%*c", p -> name);
		fscanf(people_file, "%[^\n]%*c", p -> birth_date);
		fscanf(people_file, "%[^\n]%*c", p -> birth_place);
		fscanf(people_file, "%[^\n]%*c", &p -> sex);
		strcpy(p -> cf, "");
	}

	// Postcondition
	assert(p != NULL);

	return p;
}


list* people_file_to_list(FILE* people_file) {
	// Precondition
	assert(people_file != NULL);
	list *people_list;
	node *person_prev;

	people_list = new_list();
	person_prev = NULL;

	rewind(people_file);
	while(!feof(people_file)) {
		person *p_temp = read_person(people_file);
		node *person_node = new_node(p_temp);
		add_node(people_list, person_prev, person_node);
		person_prev = person_node;
	}

	// Postcondition
	assert(people_list != NULL);

	return people_list;
}


void list_to_file(list* people_list, FILE* output_file) {
	// Preconditions
	assert(people_list != NULL);
	assert(output_file != NULL);

	node *person_node;
	
	person_node = head(people_list);
	while(person_node != NULL) {
		person *p_temp = content(person_node);
		fprintf(output_file, "%s\n", p_temp -> surname);
		fprintf(output_file, "%s\n", p_temp -> name);
		fprintf(output_file, "%s\n", p_temp -> birth_date);
		fprintf(output_file, "%s\n", p_temp -> birth_place);
		fprintf(output_file, "%c\n", p_temp -> sex);
		fprintf(output_file, "%s\n", p_temp -> cf);
		person_node = next(people_list, person_node);
	}
}

void list_to_file_males_AL(list* people_list, FILE* output_file) {
	// Preconditions
	assert(people_list != NULL);
	assert(output_file != NULL);

	filter_people(people_list, is_male_AL);
	list_to_file(people_list, output_file);
}

void list_to_file_females_MZ(list* people_list, FILE* output_file) {
	// Preconditions
	assert(people_list != NULL);
	assert(output_file != NULL);

	filter_people(people_list, is_female_MZ);
	list_to_file(people_list, output_file);
}

void people_to_file_cf(FILE* people_file, FILE* output_file, FILE* places_file) {
	// Preconditions
	assert(people_file != NULL);
	assert(output_file != NULL);
	assert(places_file != NULL);

	list *people_list = people_file_to_list(people_file);
	generate_cfs(people_list, places_file);
	list_to_file(people_list, output_file);
	free(people_list);
}

void people_to_file_cf_males_AL(FILE* people_file, FILE* output_file, FILE* places_file) {
	// Preconditions
	assert(people_file != NULL);
	assert(output_file != NULL);
	assert(places_file != NULL);

	list *people_list = people_file_to_list(people_file);
	generate_cfs(people_list, places_file);
	list_to_file_males_AL(people_list, output_file);
	free(people_list);
}

void people_to_file_cf_females_MZ(FILE* people_file, FILE* output_file, FILE* places_file) {
	// Preconditions
	assert(people_file != NULL);
	assert(output_file != NULL);
	assert(places_file != NULL);

	list *people_list = people_file_to_list(people_file);
	generate_cfs(people_list, places_file);
	list_to_file_females_MZ(people_list, output_file);
	free(people_list);
}

char* outfile_path(char* wd_path, char* filename) {
	// Precondition
	assert(wd_path[strlen(wd_path) - 1] == OS_SEP);

	size_t filepath_length = strlen(wd_path) + strlen(filename);
	char *filepath = (char *)malloc(filepath_length);
	memset(filepath, '\0', filepath_length);
	strcpy(filepath, wd_path);
	strcat(filepath, filename);

	// Postcondition
	assert(strlen(filepath) == filepath_length);

	return filepath;
}
