#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "cf_utilities.h"

bool is_surname(const char surname[]) {
	bool result = true;
	int count_upper = 0;
	if(strlen(surname) >= SURNAME_MIN) {
		/* check if place is valid string (it doesn't contain other characters
		 * than capital letter, single quotes, space and dash, and check if contains
		 * fewer than 2 capital letter */
	    for(int i = 0; surname[i] != EOS && result; i++) {
	    	if(!isupper(surname[i]) && surname[i] != ' ' && surname[i] != '\'') {
	    		result = false;
	    	} else if(isupper(surname[i])) {
	    		count_upper++;
	    	}
	    }
	    if(count_upper < CAP_LET_MIN) {
	    	result = false;
	    }
	} else {
		result = false;
	}
	return result;
}

bool is_name(const char name[]) {
	return is_surname(name);
}

bool is_sex(const char sex) {
	return (sex == 'M' || sex == 'F');
}

bool is_date(const char date_org[]) {
	bool correct = true;

	int date_idx[10] = {0, 1, 3, 4, 6, 7, 8, 9, 2, 5};
	/* date format is "GG/MM/AAAA"
	 * 0,1 correspond GG index
	 * 3,4 correspond MM index
	 * 6,7,8,9 correspond AAAA index
	 * 2,5 correspond // index */

	// check if date_org is in format GG/MM/AAAA
	for(int i = 0; i < DATE_LEN && correct; i++) {
		if(i < 8) {
			if( !isdigit( date_org[ date_idx[i] ] ) ) {
				correct = false;
			}
		}
		else {
			if(date_org[ date_idx[i] ] != '/' ) {
				correct = false;
			}
		}
	}

	// check if date_org is a valid date
	if(correct == true) {
		// it convert date_org to int values with ascii calculation
		int date = (date_org[0] - '0')*10 + (date_org[1] - '0');
		int month = (date_org[3] - '0')*10 + (date_org[4] - '0');
		int year = (date_org[6] - '0')*1000 + (date_org[7] - '0')*100 +
				   (date_org[8] - '0')*10 + (date_org[9] - '0');

		if(date <= 0 || date > 31 || month <= 0 || month > 12) {
			correct = false;
		} else if(year <= 1900 || year > 2015) {
			correct = false;
		} else if(date > 28 && month == FEB) {
			if(date == 29 && year % 4 != 0) {
				correct = false;
			} else if(date > 29) {
				correct = false;
			}
		} else if(date > 30 && (month == APR || month == JUN ||
		    month == SEP || month == NOV)) {
			correct = false;
		}
	}
	return correct;
}

bool is_place(const char place[]) {
	bool result = true;
	int count_upper = 0;
	if(strlen(place) >= 2) {
		/* check if place is valid string (it doesn't contain other characters
		 * than capital letter, single quotes, space and dash, and check if contains
		 * fewer than 2 capital letter */
	    for(int i = 0; place[i] != EOS && result; i++) {
	    	if(!isupper(place[i]) && place[i] != ' ' && place[i] != '\''
	    		&& place[i]!='-') {
	    		result = false;
	    	} else if(isupper(place[i])) {
	    		count_upper++;
	    	}
	    }
	    if(count_upper < CAP_LET_MIN) {
	    	result = false;
	    }
	} else {
		result = false;
	}
	return result;
}

bool is_vocal(const char letter) {
	bool result = false;
	if(letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' ||
	   letter == 'U') { result = true; }
	return result;
}

int generate_acode(const char code, const int idx) {
	int avalue;
	/* odd values of alphanumeric characters
	 * first 9 values are get with calculation of index (code - '0')
	 * other values are get with calculation of index (code - 'A' + 10) */
	int odd_c[] = { 1,  0,  5,  7,  9, 13, 15, 17, 19,
			       21,  1,  0,  5,  7,  9, 13, 15, 17,
				   19, 21,  2,  4, 18, 20, 11,  3,  6,
				    8, 12, 14, 16, 10, 22, 25, 24, 23 };

	// calculation of even values of alphanumeric characters
	if(idx % 2 == 0) {
		if(code >= '0' && code <= '9') {
			avalue = code - '0';
		} else if(code >= 'A' && code <= 'Z') {
			avalue = code - 'A';
		}
	// calculation of odd values of alphanumeric characters
	} else {
		if(code >= '0' && code <= '9') {
			avalue = odd_c[code - '0'];
		} else if(code >= 'A' && code <= 'Z') {
			avalue = odd_c[code - 'A' + 10];
		}
	}
	return avalue;
}

bool is_coded_surname(const char coded_surname[]) {
	bool result;
	if(isupper(coded_surname[0]) && isupper(coded_surname[1])
	   && isupper(coded_surname[2])) {
		result = true;
	} else {
		result = false;
	}
	return result;
}

bool is_coded_name(const char coded_name[]) {
	return is_coded_surname(coded_name);
}

bool is_coded_date(const char coded_date[]) {
	bool result = true;
	if(strlen(coded_date) != 5) {
		result = false;
		// check if coded_date is to NNLNN format (N = DIGIT NUMBER, L = CAPITAL LETTER)
	} else if(!isdigit(coded_date[0]) || !isdigit(coded_date[1])
			  || !isupper(coded_date[2]) || !isdigit(coded_date[3])
			  || !isdigit(coded_date[4])) {
		result = false;
		// check if month coded_date is valid
	} else if(strchr(months_coded, coded_date[2]) == NULL) {
		result = false;
	} else {  // check if date is valid
		char date_std[11] = "";
		char date[2] = "";
		char year[5] = "";

		// convert date to GG format
		date[0] = coded_date[3];
		date[1] = coded_date[4];
		if(date[0] >= '4' && date[0] <= '7') {
			date[0] -= 4;
		}

		// convert month to MM format
		int month = strchr(months_coded, coded_date[2]) - months_coded + 1;

		// convert year to AAAA format
		if(coded_date[0] == '0' && coded_date[1] == '0') {
			sprintf(year, "20");
		} else {
			sprintf(year, "19");
		}
		sprintf(year, "%s%c%c", year, coded_date[0], coded_date[1]);

		// convert data, month, and year to GG/MM/AAAA format
		sprintf(date_std, "%s/%02d/%s", date, month, year);
		result = is_date(date_std);
	}
	return result;
}

bool is_coded_place(const char coded_place[]) {
	bool result;
	if (strlen(coded_place) != 4) {
		result = false;
	// check if place is LNNN (L = CAPITAL LETTER, N = DIGIT NUMBER)
	} else if(isupper(coded_place[0]) && isdigit(coded_place[1]) &&
	   isdigit(coded_place[2]) && isdigit(coded_place[3])) {
		result = true;
	} else {
		result = false;
	}
	return result;
}

bool is_ctrl_code(const char ctrl_code) {
	return isupper(ctrl_code);
}
