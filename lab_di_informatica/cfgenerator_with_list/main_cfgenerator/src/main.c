/**
 * @file main.c
 * Codice del progetto main_cfgenerator.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "utils/cfutils.h"

#define PEOPLE_FILE "people.txt" /** Nome del file di testo contenente i dati anagrafici delle persone senza codice fiscale */
#define PLACES_FILE "italian_towns.txt" /** Nome del file di testo contenente i nomi delle citta' italiane e i loro codici catastali */

#define PEOPLE_CF_FILE "people_cf.txt" /** Nome del file di testo che conterra' i dati anagrafici delle persone */
#define PEOPLE_CF_MALES_AL_FILE "people_cf_males_al.txt" /** Nome del file di testo che conterra' i dati anagrafici di uomini con cognome che inizia tra le lettere A-L */
#define PEOPLE_CF_FEMALES_MZ_FILE "people_cf_females_mz.txt" /** Nome del file di testo che conterra' i dati anagrafici di donne con cognome che inizia tra le lettere M-Z */

/**
 * Funzione main del programma per la scrittura dei codici fiscali.
 *
 * Questo programma acquisisce da riga di comando 3 argomenti:
 *  - il path assoluto del file contenente i dati anagrafici di un numero di persone
 *  - il path assoluto del file contenente i comuni italiani e i realtivi codici catastali
 *  - il path assoluto di una cartella di lavoro, che ospitera' i file di output
 *
 * Popola un file di output chiamato "people_cf.txt", creato nella cartella di lavoro, con dati anagrafici letti dal file fornito in lettura
 * aggiungendovi il codice fiscale generato.
 * Crea altri due file di output: il primo, "people_cf_males_AL.txt", nel quale sono presenti i dati inerenti le sole persone
 * di sesso maschile con cognome compreso fra A-L; ed il secondo, "people_cf_females_MZ", con i dati delle persone di sesso
 * femminile e cognome compreso fra M-Z.
 *
 * Per tale scopo utilizza tutte le funzionalità della libreria cfgenerator (importando cfutils.h)
 *
 * @param[in] argc Numero di argomenti automaticamente rilevati all'esecuzione del programma
 * @param[in] argv Array di stringhe contenente il nome del programma e gli argomenti passati
 * @return EXIT_SUCCES oppure EXIT_FAILURE
 *
 * @pre Gli argomenti passati al programma devono essere 3
 *
 */
int main(int argc, char* argv[]) {
	bool exit = EXIT_SUCCESS;

	if(argc != 4) {
		printf("ERROR, you must type 3 paths <people_without_cf> <italian_towns> <output_files>\n");
		assert(argc == 4);
		exit = EXIT_FAILURE;
	}

	else {
		FILE *people_cf, *people, *italian_towns;

		char *path_people_cf, *path_people, *path_italian_towns;

		// INPUT FILES
		path_people = outfile_path(argv[1], PEOPLE_FILE);
		people = fopen(path_people, "r");

		path_italian_towns = outfile_path(argv[2], PLACES_FILE);
		italian_towns = fopen(path_italian_towns, "r");

		// OUTPUT FILES

		// PEOPLE CF
		path_people_cf = outfile_path(argv[3], PEOPLE_CF_FILE);
		people_cf = fopen(path_people_cf, "w");
		people_to_file_cf(people, people_cf, italian_towns);
		fclose(people_cf);
		free(path_people_cf);

		// PEOPLE CF MALES AL
		path_people_cf = outfile_path(argv[3], PEOPLE_CF_MALES_AL_FILE);
		people_cf = fopen(path_people_cf, "w");
		people_to_file_cf_males_AL(people, people_cf, italian_towns);
		fclose(people_cf);
		free(path_people_cf);

		// PEOPLE CF_FEMALES_MZ
		path_people_cf = outfile_path(argv[3], PEOPLE_CF_FEMALES_MZ_FILE);
		people_cf = fopen(path_people_cf, "w");
		people_to_file_cf_females_MZ(people, people_cf, italian_towns);
		fclose(people_cf);
		free(path_people_cf);

		// END
		fclose(people);
		fclose(italian_towns);

		free(path_people);
		free(path_italian_towns);
	}
	return exit;
}
