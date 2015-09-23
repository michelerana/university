#include "test_cfgenerator.h"
#include "CUnit/Basic.h"

int main(void) {
	CU_initialize_registry();

	CU_pSuite pSuite_cf_surname, pSuite_cf_name, pSuite_cf_date,
	          pSuite_cf_ctrl_code, pSuite_cf_generator;

	pSuite_cf_surname = CU_add_suite("cf_surname", init_suite_default,
			clean_suite_default);

	pSuite_cf_name = CU_add_suite("cf_name",init_suite_default,
				clean_suite_default);
	pSuite_cf_date = CU_add_suite("cf_date",init_suite_default,
				clean_suite_default);
	pSuite_cf_ctrl_code = CU_add_suite("cf_ctrl_code",init_suite_default,
				clean_suite_default);
	pSuite_cf_generator = CU_add_suite("cf_generator",init_suite_default,
				clean_suite_default);

	CU_add_test(pSuite_cf_surname, "surname2code()",
			test_surname2code);
	CU_add_test(pSuite_cf_name, "name2code()", test_name2code);
	CU_add_test(pSuite_cf_date, "date2code()", test_date2code);
	CU_add_test(pSuite_cf_ctrl_code, "cf_ctrl_code()",
			test_cf_ctrl_code);
	CU_add_test(pSuite_cf_generator, "cf_generator()",
			test_cf_generator);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}
