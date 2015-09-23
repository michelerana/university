#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "cf_generator.h"
#include "cf_utilities.h"

void surname2code(const char org_surname[], char coded_surname[]) {
	assert(is_surname(org_surname));

	char vowels[4] = ""; // it stores first three vowels of org_surname
	int cod_idx = 0; /* index of coded_surname, when it is stored a character,
	 	 	 	 	  * cod_idx increases to one*/

	/* check if there are vowels into org_surname, if true
	 * it stores vowel of org_surname[i] into vowels[i], else if false it stored
	 * consonant of org_surname[i] into coded_surname[cod_idx].
	 * i is the index of org_surname, j is the index of vowels */
	for(int i = 0, j = 0; org_surname[i] != EOS && cod_idx < CON_MAX; i++) {
    	if(org_surname[i] != ' ' && org_surname[i] != '\'') {
			if(is_vocal(org_surname[i]) && j < VOW_MAX) {
    		    vowels[j] = org_surname[i];
    		    j++;
			} else {
			    coded_surname[cod_idx] = org_surname[i];
				cod_idx++;
			}
    	}
	}

	/* check if coded_surname contains less than 3 characters,
	 * if true, it stored the element of vowel[i] into
	 * coded_surname[cod_idx] */
	for(int i = 0; vowels[i] != EOS && cod_idx < VOW_MAX; i++) {
		coded_surname[cod_idx] = vowels[i];
		cod_idx++;
	}

	/* check if coded_surname contains less than 3 characters,
	 * if true, it stored X character into coded_surname[cod_idx] */
	while(cod_idx < COD_MAX) {
		coded_surname[cod_idx] = 'X';
		cod_idx++;
	}

	assert(is_coded_surname(coded_surname));
}

void name2code(const char name[], char coded_name[]) {
	assert(is_name(name));

	char vowels[4] = ""; // it stores first three vowels of name

	char consonants[5] = ""; // it stores first four consonants of name

	int cod_idx = 0; /* index of coded_surname, when it is stored a character,
	 	              * cod_idx increases to one*/

	/* check if there aren't vowels into name, if true
	 * it stores consonant of name[i] into consonants[j],
	 * else if false it stored vowel of name[i] into vowels[k].
	 * i is the index of org_surname, j is the index of consonants,
	 * k is the index of vowels */
	for(int i = 0, j = 0, k = 0; name[i] != EOS && j < CON_MAX + 1; i++) {
		 if(name[i] != ' ' && name[i] != '\'') {
			 if(!is_vocal(name[i])) {
			     consonants[j] = name[i];
				 j++;
			 } else if(k < VOW_MAX) {
			     vowels[k] = name[i];
			     k++;
			 }
		 }
	}

	/* if consonants has 4 characters, it stored coded_name[2]
	 * into coded_name[1] and change coded_name[2] into
	 * fourth character of consonants, else stored characters of
	 * consonants into coded_name */
	for(int i = 0; consonants[i] != EOS; i++) {
		if(i != 3) {
		    coded_name[cod_idx] = consonants[i];
		    cod_idx++;
		} else {
			coded_name[1] = coded_name[2];
			coded_name[2] = consonants[i];
		}
	}

	/* check if coded_name contains less than 3 characters,
	 * if true, it stored character of vowels[i] into
	 * coded_name[cod_idx] */
	for(int i = 0; vowels[i] != EOS && cod_idx < VOW_MAX; i++) {
		coded_name[cod_idx] = vowels[i];
		cod_idx++;
	}

	/* check if coded_name contains less than 3 characters,
	 * if true, it stored X character into coded_name[cod_idx] */
	while(cod_idx < COD_MAX) {
		coded_name[cod_idx] = 'X';
		cod_idx++;
	}

	assert(is_coded_name(coded_name));
}

void date2code(const char date[], const char sex, char coded_date[]) {
	assert(is_date(date) && is_sex(sex));

	// Calculate year
	coded_date[0] = date[8];
	coded_date[1] = date[9];

	// Calculate month
	int month = (date[3] - '0')*10 + (date[4] - '0');
	coded_date[2] = months_coded[month - 1];

	// Calculate date and sex
	coded_date[3] = date[0];
	coded_date[4] = date[1];
	if(sex == 'F') {
		coded_date[3] += 4;
	}

	assert(is_coded_date(coded_date));
}

char cf_ctrl_code(const char coded_surname[], const char coded_name[],
     const char coded_date[], const char coded_birth_place[]) {

	assert(is_coded_surname(coded_surname) &&
		   is_coded_name(coded_name) &&
		   is_coded_date(coded_date) &&
		   is_coded_place(coded_birth_place));


	char cf[16] = ""; // stored coded characters of surname, name, date, birth_place
	char cins[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // rest codes
	int odds, evens, sum;

	sprintf(cf, "%s%s%s%s", coded_surname, coded_name, coded_date,
			coded_birth_place);

	odds = evens = 0;

	/* calculate alphanumeric value of cf characters
	 * odd position is i with even value, even position
	 * is i with odd value */
	for(int i = 0; cf[i] != EOS; i++) {
		if((i + 1) % 2 != 0) {
			odds += generate_acode(cf[i], i + 1);
		} else { evens += generate_acode(cf[i], i + 1); }
	}
	sum = (odds + evens) % 26;

	assert(is_ctrl_code(cins[sum]));

	return cins[sum];
}

void cf_generator(const char surname[], const char name[], const char date[],
     const char birth_place_code[], const char sex, char cf[]) {

	assert(is_surname(surname) &&
		   is_name(name) &&
		   is_date(date) &&
		   is_coded_place(birth_place_code) &&
		   is_sex(sex));

	char coded_surname[4] = "";
	char coded_name[4] = "";
	char coded_date[6] = "";
	char ctrl_code;

	surname2code(surname, coded_surname);
	name2code(name, coded_name);
	date2code(date, sex, coded_date);
	ctrl_code = cf_ctrl_code(coded_surname, coded_name, coded_date,
							 birth_place_code);

	sprintf(cf, "%s%s%s%s%c", coded_surname, coded_name, coded_date,
			birth_place_code, ctrl_code);

	char birth_place_cf[5] = "";
	sprintf(birth_place_cf,"%c%c%c%c", cf[11], cf[12], cf[13], cf[14]);

	assert(is_coded_surname(coded_surname) &&
		   is_coded_name(coded_name) &&
		   is_coded_date(coded_date) &&
		   is_coded_place(birth_place_cf) &&
		   is_ctrl_code(ctrl_code));
}

bool get_coded_place(const char place[], char coded_place[]) {
	assert(is_place(place));

	city cities[N_CITIES];
	FILE *in;
	bool find = false;
	in = fopen(".\\..\\src\\coded_places.dat", "rb");
	fread(cities, sizeof(city), N_CITIES, in);
	fclose(in);

	/* check if place exits, if true, stored
	 * cities[i].code into coded_place and change
	 * find value into true */
	for(int i = 0; i < N_CITIES && !find; i++) {
		if(strcmp(place, cities[i].name) == 0) {
			find = true;
			strcpy(coded_place, cities[i].code);
		}
	}

	if(find) {
		assert(is_coded_place(coded_place));
	}
	return find;
}
