#include <string.h>
#include "CUnit/Basic.h"
#include "cf_generator.h"

void test_surname2code(void) {
	char coded_surname[10] = "";

	surname2code("MALA", coded_surname);
	CU_ASSERT_TRUE(strcmp(coded_surname, "MLA") == 0);

	strcpy(coded_surname, "");
	surname2code("DE LUCA", coded_surname);
	CU_ASSERT_TRUE(strcmp(coded_surname, "DLC") == 0);

	strcpy(coded_surname, "");
	surname2code("DE' LUCA", coded_surname);
	CU_ASSERT_TRUE(strcmp(coded_surname, "DLC") == 0);

	strcpy(coded_surname, "");
	surname2code("FO", coded_surname);
	CU_ASSERT_TRUE(strcmp(coded_surname, "FOX") == 0);
}

void test_name2code(void) {
	char coded_name[4] = "";

	name2code("AL", coded_name);
	CU_ASSERT_TRUE(strcmp(coded_name, "LAX") == 0);

	strcpy(coded_name, "");
	name2code("CARLO", coded_name);
	CU_ASSERT_TRUE(strcmp(coded_name, "CRL") == 0);

	strcpy(coded_name, "");
	name2code("MICHELE", coded_name);
	CU_ASSERT_TRUE(strcmp(coded_name, "MHL") == 0);

	strcpy(coded_name, "");
	name2code("NICOLO'", coded_name);
	CU_ASSERT_TRUE(strcmp(coded_name, "NCL") == 0);

	strcpy(coded_name, "");
	name2code("LUIGI NICOLO'", coded_name);
	CU_ASSERT_TRUE(strcmp(coded_name, "LNC") == 0);
}

void test_date2code(void) {
	char coded_date[6] = "";

	date2code("12/01/1901", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "01A12") == 0);

	strcpy(coded_date, "");
	date2code("12/01/1901", 'F', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "01A52") == 0);

	strcpy(coded_date, "");
	date2code("28/02/2001", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "01B28") == 0);

	strcpy(coded_date, "");
	date2code("29/02/1976", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "76B29") == 0);

	strcpy(coded_date, "");
	date2code("30/04/1986", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "86D30") == 0);

	strcpy(coded_date, "");
	date2code("30/06/2015", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "15H30") == 0);

	strcpy(coded_date, "");
	date2code("30/09/1971", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "71P30") == 0);

	strcpy(coded_date, "");
	date2code("30/11/1991", 'M', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "91S30") == 0);

	strcpy(coded_date, "");
	date2code("31/12/2014", 'F', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "14T71") == 0);

	strcpy(coded_date, "");
	date2code("14/05/2000", 'F', coded_date);
	CU_ASSERT_TRUE(strcmp(coded_date, "00E54") == 0);
}

void test_cf_ctrl_code(void) {
	char ctrl_code;

	ctrl_code = cf_ctrl_code("CPN", "DNL", "40P17", "A662");
	CU_ASSERT_EQUAL(ctrl_code, 'A');

	ctrl_code = cf_ctrl_code("BRT", "LCU", "71S06", "G273");
	CU_ASSERT_EQUAL(ctrl_code, 'H');

	ctrl_code = cf_ctrl_code("CPT", "MLR", "80C65", "G337");
	CU_ASSERT_EQUAL(ctrl_code, 'N');
}


void test_cf_generator(void) {
	char cf[17] = "";

	strcpy(cf, "");
	cf_generator("DE' LUCA", "LUIGI NICOLO'", "01/01/1901", "A669", 'M', cf);
	CU_ASSERT_TRUE(strcmp(cf, "DLCLNC01A01A669X") == 0);

	strcpy(cf, "");
	cf_generator("BINETTI", "FRANCESCA", "29/02/1996", "D643", 'F', cf);
	CU_ASSERT_TRUE(strcmp(cf, "BNTFNC96B69D643H") == 0);

	strcpy(cf, "");
	cf_generator("RICCI", "CARLO", "28/02/2015", "L328", 'M', cf);
	CU_ASSERT_TRUE(strcmp(cf, "RCCCRL15B28L328X") == 0);

}

int init_suite_default(void) {
	return 0;
}

int clean_suite_default(void) {
	return 0;
}
