/**
 * @file test_cfutils.c
 * Funzioni che testano cfutils.
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "CUnit/Basic.h"
#include "cfgenerator.h"
#include "list/list.h"
#include "list/person.h"
#include "utils/cfutils.h"

#define COD_PLACE_LEN 5 /** Lunghezza massima del codice catastale compreso il carattere '\0' */

#define N_PEOPLE 20 /** Quantita' massima di persone */
#define N_PEOPLE_AL 10 /** Quantita' massima di persone comprese tra il cognome A-L */
#define N_PEOPLE_MZ 10 /** Quantita' massima di persone comprese tra il cognome M-Z */
#define N_PEOPLE_MALES_AL 7 /** Quantita' massima di uomini compresi tra il cognome A-L */
#define N_PEOPLE_MALES_MZ 5 /** Quantita' massima di uomini compresi tra il cognome M-Z */
#define N_PEOPLE_FEMALES_AL 3 /** Quantita' massima di donne comprese tra il cognome A-L */
#define N_PEOPLE_FEMALES_MZ 5 /** Quantita' massima di donne comprese tra il cognome M-Z */
#define N_FILEPATHS 3 /** Quantita' massima di percorsi assoluti di file */
#define N_PLACES 6 /** Quantita' massima di citta' */

#define PATH_PEOPLE "../input_data_tests/people.txt" /** Percorso del file contenente i dati delle persone senza codice fiscale */
#define PATH_PEOPLE_AL "../input_data_tests/people_al.txt" /** Percorso del file contenente i dati delle persone senza codice fiscale con cognome compreso tra la lettera A-L  */
#define PATH_PEOPLE_MZ "../input_data_tests/people_mz.txt" /** Percorso del file contenente i dati delle persone senza codice fiscale con cognome compreso tra la lettera M-Z  */
#define PATH_PEOPLE_MALES_AL "../input_data_tests/people_males_al.txt" /** Percorso del file contenente i dati di uomini senza codice fiscale con cognome compreso tra la lettera A-L  */
#define PATH_PEOPLE_MALES_MZ "../input_data_tests/people_males_mz.txt" /** Percorso del file contenente i dati di uomini senza codice fiscale con cognome compreso tra la lettera M-Z  */
#define PATH_PEOPLE_FEMALES_AL "../input_data_tests/people_females_al.txt" /** Percorso del file contenente i dati di donne senza codice fiscale con cognome compreso tra la lettera A-L  */
#define PATH_PEOPLE_FEMALES_MZ "../input_data_tests/people_females_mz.txt" /** Percorso del file contenente i dati di donne senza codice fiscale con cognome compreso tra la lettera M-Z  */

#define PATH_ITALIAN_TOWNS "../input_data_tests/italian_towns.txt" /** Percorso del file contenente i nomi e codici catastali delle citta' italiane */
#define PATH_PLACES_CODE "../input_data_tests/places_code.txt" /** Percorso del file contenente i nomi di alcune citta' italiane */
#define PATH_PLACES_NAME "../input_data_tests/places_name.txt" /** Percorso del file contenente i codici catastali di alcune citta' italiane */

#define PATH_CFS "../input_data_tests/cfs.txt" /** Percorso del file contenente solo codici fiscali */
#define PATH_CF_PEOPLE "../input_data_tests/cf_people.txt" /** Percorso del file contente i dati delle persone con codice fiscale */
#define PATH_CF_PEOPLE_MALES_AL "../input_data_tests/cf_people_males_al.txt" /** Percorso del file contenente i dati di uomini con codice fiscale con cognome compreso tra la lettera A-L  */
#define PATH_CF_PEOPLE_FEMALES_MZ "../input_data_tests/cf_people_females_mz.txt" /** Percorso del file contenente i dati di donne con codice fiscale con cognome compreso tra la lettera M-Z  */



// 			------------- UTILITY FUNCTIONS -------------

/**
 * Carica in una lista i dati contenuti in un array di tipo person.
 * @param people_list  lista che conterra' i dati dell'array people
 * @param people 	   array contenente i dati di persone
 * @param n_people 	   dimensione dell'array people
 * @pre people_list e people non devono essere NULL, n_people dev'essere maggiore di zero
 */
void load_list_from_people(list *people_list, person people[], int n_people) {
	assert(people_list != NULL);
	assert(people != NULL);
	assert(n_people > 0);

	node *people_node, *node_prev;
	node_prev = NULL;
	for(int i = 0; i < n_people; i++) {
		people_node = new_node(&people[i]);
		add_node(people_list, node_prev, people_node);
		node_prev = people_node;
	}
}

/**
 * Carica in un array di tipo person i dati contenuti in un file.
 * 		#test_is_surname_AL
 * 		#test_is_surname_MZ
 * 		#test_is_male_AL
 * 		#test_is_female_MZ
 * 		#test_filter_people
 * 		#test_generate_cfs
 * 		#test_read_person
 * 		#test_people_file_to_list
 * @param filepath file contenente dati di persone senza codice fiscale
 * @param people   array che conterra' i dati del file
 * @pre filepath e people non devono essere NULL
 */
void load_people_from_file(FILE* filepath, person people[]) {
	assert(filepath != NULL && people != NULL);
	for(int i = 0; !feof(filepath); i++) {
		fscanf(filepath, "%[^\n]%*c", people[i].surname);
		fscanf(filepath, "%[^\n]%*c", people[i].name);
		fscanf(filepath, "%[^\n]%*c", people[i].birth_date);
		fscanf(filepath, "%[^\n]%*c", people[i].birth_place);
		fscanf(filepath, "%[^\n]%*c", &people[i].sex);
		strcpy(people[i].cf, "");
	}
}

/**
 * Carica in un array di tipo person i dati contenuti in un file.
 * 		#test_list_to_file
 * 		#test_list_to_file_males_AL
 * 		#test_list_to_file_females_MZ
 * 		#test_people_to_file_cf
 * 		#test_people_to_file_cf_males_AL
 * 		#test_people_to_file_cf_females_MZ
 * @param filepath file contenente dati di persone con codice fiscale
 * @param people   array che conterra' i dati del file
 * @pre filepath e people non devono essere NULL
 */
void load_cf_people_from_file(FILE* filepath, person people[]) {
	assert(filepath != NULL);
	assert(people != NULL);

	for(int i = 0; !feof(filepath); i++) {
		fscanf(filepath, "%[^\n]%*c", people[i].surname);
		fscanf(filepath, "%[^\n]%*c", people[i].name);
		fscanf(filepath, "%[^\n]%*c", people[i].birth_date);
		fscanf(filepath, "%[^\n]%*c", people[i].birth_place);
		fscanf(filepath, "%[^\n]%*c", &people[i].sex);
		fscanf(filepath, "%[^\n]%*c", people[i].cf);
	}
}

/**
 * Verifica se ogni elemento della lista rispetta la condizione di filtraggio.
 * @param people_list lista in cui saranno fatti i controlli del
 * @param filter	  puntatore a funzione che determina la condizione di filtraggio
 * @pre people_list e filter non devono essere NULL
 * @return Valore di verita'
 */
bool is_filtered_list(list *people_list, filter_fn filter) {
	assert(people_list != NULL);
	assert(filter != NULL);

	node *people_node = head(people_list);
	bool answer = true;
	while(people_node != NULL && answer) {
		if(!filter(content(people_node))) {
			answer = false;
		}
	people_node = next(people_list, people_node);
	}
	return answer;
}



// 			------------- TEST FUNCTIONS -------------

/**
 * Verifica se il sesso di una persona e' M.
 */
void test_is_male(void) {
	person people = {"", "", "", "", '\0', ""};

	people.sex = 'M';
	CU_ASSERT_TRUE(is_male(&people))

	people.sex = 'F';
	CU_ASSERT_FALSE(is_male(&people));
}

/**
 * Verifica se il sesso di una persona e' F.
 */
void test_is_female(void) {
	person people = {"", "", "", "", '\0', ""};

	people.sex = 'F';
	CU_ASSERT_TRUE(is_female(&people))

	people.sex = 'M';
	CU_ASSERT_FALSE(is_female(&people));
}

/**
 * Verifica se il cognome di una persona incomincia tra la lettere A-L.
 */
void test_is_surname_AL(void) {
	person people_al[N_PEOPLE_AL] = {{"", "", "", "", '\0', ""}},
		   people_mz[N_PEOPLE_MZ] = {{"", "", "", "", '\0', ""}};
	FILE *people_file;

	// Loads data of people_al from file and checks if assertions are true
	people_file = fopen(PATH_PEOPLE_AL, "r");
	load_people_from_file(people_file, people_al);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_AL; i++) {
		CU_ASSERT_TRUE(is_surname_AL(&people_al[i]));
	}

	// Loads data of people_mz from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_MZ, "r");
	load_people_from_file(people_file, people_mz);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_MZ; i++) {
		CU_ASSERT_FALSE(is_surname_AL(&people_mz[i]));
	}
}

/**
 * Verifica se il cognome di una persona incomincia tra le lettere M-Z.
 */
void test_is_surname_MZ(void) {
	person people_al[N_PEOPLE_AL] = {{"", "", "", "", '\0', ""}},
		   people_mz[N_PEOPLE_MZ] = {{"", "", "", "", '\0', ""}};
	FILE *people_file;

	// Loads data of people_mz from file and checks if assertions are true
	people_file = fopen(PATH_PEOPLE_MZ, "r");
	load_people_from_file(people_file, people_mz);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_MZ; i++) {
		CU_ASSERT_TRUE(is_surname_MZ(&people_mz[i]));
	}

	// Loads data of people_al from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_AL, "r");
	load_people_from_file(people_file, people_al);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_AL; i++) {
		CU_ASSERT_FALSE(is_surname_MZ(&people_al[i]));
	}
}

/**
 * Verifica se la persona e' un uomo il cui cognome inizia tra le lettere A-L.
 */
void test_is_male_AL(void) {
	person people_males_al[N_PEOPLE_MALES_AL] = {{"", "", "", "", '\0', ""}},
		   people_males_mz[N_PEOPLE_MALES_MZ] = {{"", "", "", "", '\0', ""}},
		   people_females_al[N_PEOPLE_FEMALES_AL] = {{"", "", "", "", '\0', ""}},
		   people_females_mz[N_PEOPLE_FEMALES_MZ] = {{"", "", "", "", '\0', ""}};
	FILE *people_file;

	// Loads data of people_males_al from file and checks if assertions are true
	people_file = fopen(PATH_PEOPLE_MALES_AL, "r");
	load_people_from_file(people_file, people_males_al);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_MALES_AL; i++) {
		CU_ASSERT_TRUE(is_male_AL(&people_males_al[i]));
	}

	// Loads data of people_males_mz from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_MALES_MZ, "r");
	load_people_from_file(people_file, people_males_mz);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_MALES_MZ; i++) {
		CU_ASSERT_FALSE(is_male_AL(&people_males_mz[i]));
	}

	// Loads data of people_females_al from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_FEMALES_AL, "r");
	load_people_from_file(people_file, people_females_al);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_FEMALES_AL; i++) {
		CU_ASSERT_FALSE(is_male_AL(&people_females_al[i]));
	}

	// Loads data of people_females_mz from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_FEMALES_MZ, "r");
	load_people_from_file(people_file, people_females_mz);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_FEMALES_MZ; i++) {
		CU_ASSERT_FALSE(is_male_AL(&people_females_mz[i]));
	}
}

/**
 * Verifica se la persona e' una donna il cui cognome comincia tra le lettere M-Z.
 */
void test_is_female_MZ(void) {
	person people_females_mz[N_PEOPLE_FEMALES_MZ] = {{"", "", "", "", '\0', ""}},
	       people_females_al[N_PEOPLE_FEMALES_AL] = {{"", "", "", "", '\0', ""}},
		   people_males_mz[N_PEOPLE_MALES_MZ] = {{"", "", "", "", '\0', ""}},
		   people_males_al[N_PEOPLE_MALES_AL] = {{"", "", "", "", '\0', ""}};
	FILE *people_file;

	// Loads data of people_females_mz from file and checks if assertions are true
	people_file = fopen(PATH_PEOPLE_FEMALES_MZ, "r");
	load_people_from_file(people_file, people_females_mz);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_FEMALES_MZ; i++) {
		CU_ASSERT_TRUE(is_female_MZ(&people_females_mz[i]));
	}

	// Loads data of people_females_al from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_FEMALES_AL, "r");
	load_people_from_file(people_file, people_females_al);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_FEMALES_AL; i++) {
		CU_ASSERT_FALSE(is_female_MZ(&people_females_al[i]));
	}

	// Loads data of people_males_mz from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_MALES_MZ, "r");
	load_people_from_file(people_file, people_males_mz);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_MALES_MZ; i++) {
		CU_ASSERT_FALSE(is_female_MZ(&people_males_mz[i]));
	}

	// Loads data of people_males_al from file and checks if assertions are false
	people_file = fopen(PATH_PEOPLE_MALES_AL, "r");
	load_people_from_file(people_file, people_males_al);
	fclose(people_file);
	for(int i = 0; i < N_PEOPLE_MALES_AL; i++) {
		CU_ASSERT_FALSE(is_female_MZ(&people_males_al[i]));
	}
}

/**
 * Verifica se la lista viene filtrata correttamente.
 */
void test_filter_people(void) {
	FILE *people_file;
	list *people_list;
	person people[N_PEOPLE] = {{"", "", "", "", '\0', ""}};

	// Loads people data from file
	people_file = fopen(PATH_PEOPLE, "r");
	load_people_from_file(people_file, people);
	fclose(people_file);

	// TEST filter_people(people_list, is_male)
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	filter_people(people_list, is_male);
	CU_ASSERT_TRUE(is_filtered_list(people_list, is_male));
	delete_list(people_list);

	// TEST filter_people(people_list, is_female)
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	filter_people(people_list, is_female);
	CU_ASSERT_TRUE(is_filtered_list(people_list, is_female));
	delete_list(people_list);

	// TEST filter_people(people_list, is_surname_AL)
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	filter_people(people_list, is_surname_AL);
	CU_ASSERT_TRUE(is_filtered_list(people_list, is_surname_AL));
	delete_list(people_list);

	// TEST filter_people(people_list, is_surname_MZ)
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	filter_people(people_list, is_surname_MZ);
	CU_ASSERT_TRUE(is_filtered_list(people_list, is_surname_MZ));
	delete_list(people_list);

	// TEST filter_people(people_list, is_male_AL)
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	filter_people(people_list, is_male_AL);
	CU_ASSERT_TRUE(is_filtered_list(people_list, is_male_AL));
	delete_list(people_list);

	// TEST filter_people(people_list, is_female_MZ)
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	filter_people(people_list, is_female_MZ);
	CU_ASSERT_TRUE(is_filtered_list(people_list, is_female_MZ));
	delete_list(people_list);
}

/**
 * Verifica la corretta generazione del codice fiscale in una lista.
 */
void test_generate_cfs(void) {
	FILE *places_file, *people_file, *cfs_file;
	list *people_list; // It contains actual values
	person people[N_PEOPLE] = {{"", "", "", "", '\0', ""}}; // It contains expected values
	char cfs[N_PEOPLE][CF_LEN] = {""};

	people_file = fopen(PATH_PEOPLE, "r");
	places_file = fopen(PATH_ITALIAN_TOWNS, "r");
	cfs_file = fopen(PATH_CFS, "r");

	// Load people data from file
	load_people_from_file(people_file, people);
	for(int i = 0; i < N_PEOPLE; i++) {
		fscanf(cfs_file, "%[^\n]%*c", cfs[i]);
	}
	fclose(people_file);
	fclose(cfs_file);

	// Loads the data of people_list from people array and generates cfs saving into people_list
	people_list = new_list();
	load_list_from_people(people_list, people, N_PEOPLE);
	generate_cfs(people_list, places_file);
	fclose(places_file);

	// Checks assertions if actual_person->cf and cfs[i] are equals
	node *people_node = head(people_list);
	for(int i = 0; i < N_PEOPLE; i++) {
		person *actual_person = content(people_node);
		CU_ASSERT_TRUE(memcmp(actual_person->cf, cfs[i], sizeof(char)*CF_LEN) == 0);
		people_node = next(people_list, people_node);
	}
	delete_list(people_list);
}

/**
 * Verifica il corretto inserimento del codice catastale.
 */
void test_read_birth_place_code(void) {
	FILE *places_file, *places_name_src, *places_code_src;
	char places_name[N_PLACES][MAX_BIRTH_PLACE] = {""},
		 actual_places_code[N_PLACES][COD_PLACE_LEN] = {""},
		 expected_places_code[N_PLACES][COD_PLACE_LEN] = {""};

	// Loads the data of places name, expected values code place from file
	places_name_src = fopen(PATH_PLACES_NAME, "r");
	places_code_src = fopen(PATH_PLACES_CODE, "r");
	for(int i = 0; i < N_PLACES; i++) {
		fscanf(places_name_src, "%[^\n]%*c", places_name[i]);
		fscanf(places_code_src, "%[^\n]%*c", expected_places_code[i]);
	}
	fclose(places_name_src);
	fclose(places_code_src);

	// Loads the data of actual_places_code from read_birth_place_code function
	places_file = fopen(PATH_ITALIAN_TOWNS, "r");
	for(int i = 0; i < N_PLACES; i++) {
		char *place_code = read_birth_place_code(places_file, places_name[i]);
		strcpy(actual_places_code[i], place_code);
		free(place_code);
		rewind(places_file);
	}
	fclose(places_file);

	// Checks assertions if actual_place_code[i] and expected_places_code[i] are equal
	for(int i = 0; i < N_PLACES; i++) {
		CU_ASSERT_TRUE(memcmp(actual_places_code[i], expected_places_code[i],
					   sizeof(char)*COD_PLACE_LEN) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati anagrafici da file in una struttura person.
 */
void test_read_person(void) {
	FILE *people_file;
	person actual_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}},
		   expected_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}};

	people_file = fopen(PATH_PEOPLE, "r");

	// Loads the data of expected_people from file
	load_people_from_file(people_file, expected_people);

	// Loads the data of actual_people from read_person
	rewind(people_file);
	for(int i = 0; i < N_PEOPLE; i++) {
		person *p_temp = read_person(people_file);
		actual_people[i] = *p_temp;
		free(p_temp);
	}
	fclose(people_file);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati anagrafici da file in una lista.
 */
void test_people_file_to_list(void) {
	FILE *people_file;
	person expected_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}};
	list *actual_people_list;
	node *people_node;

	// Loads the data of expected_people from file
	people_file = fopen(PATH_PEOPLE, "r");
	load_people_from_file(people_file, expected_people);

	// Loads the data of actual_people_list from people_file_to_list
	rewind(people_file);
	actual_people_list = people_file_to_list(people_file);
	fclose(people_file);

	// Checks assertions if actual_person and expected_people[i] are equals
	people_node = head(actual_people_list);
	for(int i = 0; i < N_PEOPLE; i++) {
		person *actual_person = content(people_node);
		CU_ASSERT_TRUE(memcmp(actual_person, &expected_people[i], sizeof(person)) == 0);
		people_node = next(actual_people_list, people_node);
	}
	delete_list(actual_people_list);
}

/**
 * Verifica il corretto inserimento dei dati anagrafici della lista in un file.
 */
void test_list_to_file(void) {
	FILE *cf_people_src, *temp_people_src;
	list *people_list = new_list();
	person actual_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}},
		   expected_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}};

	// Loads the data of expected_people from file
	cf_people_src = fopen(PATH_CF_PEOPLE, "r");
	load_cf_people_from_file(cf_people_src, expected_people);
	fclose(cf_people_src);

	// Writes into temporary file the data of list_to_file
	temp_people_src = tmpfile();
	load_list_from_people(people_list, expected_people, N_PEOPLE);
	list_to_file(people_list, temp_people_src);
	delete_list(people_list);

	// Loads the data of actual_people from temporary file
	rewind(temp_people_src);
	load_cf_people_from_file(temp_people_src, actual_people);
	fclose(temp_people_src);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati anagrafici della lista in un file.
 * Questi dati anagrafici sono di uomini con cognome che inizia tra le lettere A-L.
 */
void test_list_to_file_males_AL(void) {
	FILE *cf_people_file, *cf_people_males_al_file, *temp_people_file;
	list *people_list = new_list();
	person	people[N_PEOPLE] = {{"", "", "", "", '\0', ""}},
			actual_people[N_PEOPLE_MALES_AL] = {{"", "", "", "", '\0', ""}},
			expected_people[N_PEOPLE_MALES_AL] = {{"", "", "", "", '\0', ""}};

	// Loads the data of expected_people from file
	cf_people_males_al_file = fopen(PATH_CF_PEOPLE_MALES_AL, "r");
	load_cf_people_from_file(cf_people_males_al_file, expected_people);
	fclose(cf_people_males_al_file);

	// Loads the people data from file
	cf_people_file = fopen(PATH_CF_PEOPLE, "r");
	load_cf_people_from_file(cf_people_file, people);
	fclose(cf_people_file);

	// Writes into temporary file the data of people_list
	temp_people_file = tmpfile();
	load_list_from_people(people_list, people, N_PEOPLE);
	list_to_file_males_AL(people_list, temp_people_file);
	delete_list(people_list);

	// Loads the data of actual_people from temporary file
	rewind(temp_people_file);
	load_cf_people_from_file(temp_people_file, actual_people);
	fclose(temp_people_file);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE_MALES_AL; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati anagrafici della lista in un file.
 * Questi dati anagrafici sono di donne con cognome che inizia tra le lettere M-Z.
 */
void test_list_to_file_females_MZ(void) {
	FILE *cf_people_file, *cf_people_female_mz_file, *temp_people_file;
	list *people_list = new_list();
	person	people[N_PEOPLE] = {{"", "", "", "", '\0', ""}},
			actual_people[N_PEOPLE_FEMALES_MZ] = {{"", "", "", "", '\0', ""}},
			expected_people[N_PEOPLE_FEMALES_MZ] = {{"", "", "", "", '\0', ""}};

	// Loads the data of expected_people from file
	cf_people_female_mz_file = fopen(PATH_CF_PEOPLE_FEMALES_MZ, "r");
	load_cf_people_from_file(cf_people_female_mz_file, expected_people);
	fclose(cf_people_female_mz_file);

	// Loads the people data from file
	cf_people_file = fopen(PATH_CF_PEOPLE, "r");
	load_cf_people_from_file(cf_people_file, people);
	fclose(cf_people_file);

	// Writes into temporary file the data of people_list
	temp_people_file = tmpfile();
	load_list_from_people(people_list, people, N_PEOPLE);
	list_to_file_females_MZ(people_list, temp_people_file);
	delete_list(people_list);

	// Loads the data of actual_people from temporary file
	rewind(temp_people_file);
	load_cf_people_from_file(temp_people_file, actual_people);
	fclose(temp_people_file);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE_FEMALES_MZ; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati in file, quest'ultimo viene popolato di dati
 * anagrafici di persone scrivendo nel file anche il loro codice fiscale.
 * I dati anagrafici sono presi da un altro file che non contiene il loro codice fiscale.
 */
void test_people_to_file_cf(void) {
	FILE *people_file, *places_file, *cf_people_file;
	person actual_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}},
		   expected_people[N_PEOPLE] = {{"", "", "", "", '\0', ""}};

	// Writes into temporary file the people data with cf
	people_file = fopen(PATH_PEOPLE, "r");
	places_file = fopen(PATH_ITALIAN_TOWNS, "r");
	cf_people_file = tmpfile();
	people_to_file_cf(people_file, cf_people_file, places_file);
	fclose(people_file);
	fclose(places_file);

	// Reads the data of actual_people from temporary file
	rewind(cf_people_file);
	load_cf_people_from_file(cf_people_file, actual_people);
	fclose(cf_people_file);

	// Reads the data of expected_people from temporary file
	cf_people_file = fopen(PATH_CF_PEOPLE, "r");
	load_cf_people_from_file(cf_people_file, expected_people);
	fclose(cf_people_file);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati in file, quest'ultimo viene popolato di dati
 * anagrafici di uomini con cognome che inizia tra le lettere A-L scrivendo nel file
 * anche il loro codice fiscale.
 * I dati anagrafici vengono presi da un file che non contiene il loro codice fiscale.
 */
void test_people_to_file_cf_males_AL(void) {
	FILE *people_file, *places_file, *cf_people_file;
	person actual_people[N_PEOPLE_MALES_AL] = {{"", "", "", "", '\0', ""}},
		   expected_people[N_PEOPLE_MALES_AL] = {{"", "", "", "", '\0', ""}};

	/* Writes into temporary files the data of male people with surname
	 * begins with letter A-L and it generates cf. The data are getted from file */
	people_file = fopen(PATH_PEOPLE, "r");
	places_file = fopen(PATH_ITALIAN_TOWNS, "r");
	cf_people_file = tmpfile();
	people_to_file_cf_males_AL(people_file, cf_people_file, places_file);
	fclose(people_file);
	fclose(places_file);

	// Loads the data of actual_people from temporary file
	rewind(cf_people_file);
	load_cf_people_from_file(cf_people_file, actual_people);
	fclose(cf_people_file);

	// Loads the data of actual_people from file
	cf_people_file = fopen(PATH_CF_PEOPLE_MALES_AL, "r");
	load_cf_people_from_file(cf_people_file, expected_people);
	fclose(cf_people_file);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE_MALES_AL; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica il corretto inserimento dei dati in file, quest'ultimo viene popolato di dati
 * anagrafici di donne con cognome che inizia tra le lettere M-Z scrivendo nel file
 * anche il loro codice fiscale.
 * I dati anagrafici vengono presi da un file che non contiene il loro codice fiscale.
 */
void test_people_to_file_cf_females_MZ(void) {
	FILE *people_file, *places_file, *cf_people_file;
	person actual_people[N_PEOPLE_FEMALES_MZ] = {{"", "", "", "", '\0', ""}},
		   expected_people[N_PEOPLE_FEMALES_MZ] = {{"", "", "", "", '\0', ""}};

	// Writes into temporary file the data written by people_to_file_cf_females_MZ function
	people_file = fopen(PATH_PEOPLE, "r");
	places_file = fopen(PATH_ITALIAN_TOWNS, "r");
	cf_people_file = tmpfile();
	people_to_file_cf_females_MZ(people_file, cf_people_file, places_file);
	fclose(people_file);
	fclose(places_file);

	// Loads the data of actual_people from temporary file
	rewind(cf_people_file);
	load_cf_people_from_file(cf_people_file, actual_people);
	fclose(cf_people_file);

	// Loads the data of expected_people from file
	cf_people_file = fopen(PATH_CF_PEOPLE_FEMALES_MZ, "r");
	load_cf_people_from_file(cf_people_file, expected_people);
	fclose(cf_people_file);

	// Checks assertions if actual_people[i] and expected_people[i] are equals
	for(int i = 0; i < N_PEOPLE_FEMALES_MZ; i++) {
		CU_ASSERT_TRUE(memcmp(&actual_people[i], &expected_people[i], sizeof(person)) == 0);
	}
}

/**
 * Verifica se il filepath inserito e' corretto e il separatore corrisponde a quello della
 * costante OS_SEP.
 */
void test_outfile_path(void) {
	char wd_path[] = "C: work ", filename[] = "tasks.txt",
		 *actual_filepath, expected_filepath[] = "C: work tasks.txt";

	wd_path[2] = OS_SEP;
	wd_path[7] = OS_SEP;
	expected_filepath[2] = OS_SEP;
	expected_filepath[7] = OS_SEP;

	actual_filepath = outfile_path(wd_path, filename);

	// Checks assertion if actual_path and expected_path are equals.
	CU_ASSERT_TRUE(strcmp(actual_filepath, expected_filepath) == 0);

	free(actual_filepath);
}

/**
 * Inizializza la suite.
 */
int init_suite_default(void) {
	return 0;
}

/**
 * Pulisce la suite.
 */
int clean_suite_default(void) {
	return 0;
}

int main() {
	CU_initialize_registry();

	CU_pSuite pSuite_sex_surname_filter, pSuite_person_path,
			  pSuite_list_to_file, pSuite_people_to_list_file_cf;

	pSuite_sex_surname_filter = CU_add_suite("sex_surname_filter",
			init_suite_default, clean_suite_default);
	CU_add_test(pSuite_sex_surname_filter, "is_male()", test_is_male);
	CU_add_test(pSuite_sex_surname_filter, "is_female()", test_is_female);
	CU_add_test(pSuite_sex_surname_filter, "is_surname_AL()", test_is_surname_AL);
	CU_add_test(pSuite_sex_surname_filter, "is_surname_MZ()", test_is_surname_MZ);
	CU_add_test(pSuite_sex_surname_filter, "is_male_AL()", test_is_male_AL);
	CU_add_test(pSuite_sex_surname_filter, "is_female_MZ()", test_is_female_MZ);
	CU_add_test(pSuite_sex_surname_filter, "filter_people()", test_filter_people);

	pSuite_person_path = CU_add_suite("person_path",
			init_suite_default, clean_suite_default);
	CU_add_test(pSuite_person_path, "test of generate_cfs()", test_generate_cfs);
	CU_add_test(pSuite_person_path, "test of read_birth_place_code()", test_read_birth_place_code);
	CU_add_test(pSuite_person_path, "test of read_person()", test_read_person);
	CU_add_test(pSuite_person_path, "test of outfile_path()", test_outfile_path);

	pSuite_list_to_file = CU_add_suite("list_to_file",
				init_suite_default, clean_suite_default);
	CU_add_test(pSuite_list_to_file, "test of list_to_file()", test_list_to_file);
	CU_add_test(pSuite_list_to_file, "test of list_to_file_males_AL()", test_list_to_file_males_AL);
	CU_add_test(pSuite_list_to_file, "test of list_to_file_females_MZ()", test_list_to_file_females_MZ);

	pSuite_people_to_list_file_cf = CU_add_suite("people_to_list_file_cf",
					init_suite_default, clean_suite_default);
	CU_add_test(pSuite_people_to_list_file_cf, "test of people_file_to_list()", test_people_file_to_list);
	CU_add_test(pSuite_people_to_list_file_cf, "test of people_to_file_cf()", test_people_to_file_cf);
	CU_add_test(pSuite_people_to_list_file_cf, "test of people_to_file_cf_males_AL()", test_people_to_file_cf_males_AL);
	CU_add_test(pSuite_people_to_list_file_cf, "test of people_to_file_cf_females_MZ()", test_people_to_file_cf_females_MZ);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
