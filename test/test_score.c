#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
//#include <CUnit/Basic.h>
//#include <CUnit/CUnit.h>
#include "../lib/interface.h"

void test_score_progression() {
    Score s;
    init_score(&s);
    
    // Position initiale : Score à 0
    update_score(&s, 0);
    CU_ASSERT_EQUAL(s.value, 0);

    // Le joueur avance à X=1000. 
    // Calcul : 1000 / 20 = 50 points.
    update_score(&s, 1000);
    CU_ASSERT_EQUAL(s.max_x, 1000);
    CU_ASSERT_EQUAL(s.value, 50);

    // Le joueur recule à X=500.
    // Le score doit rester au score maximum atteint (50 points) 
    update_score(&s, 500);
    CU_ASSERT_EQUAL(s.max_x, 1000);
    CU_ASSERT_EQUAL(s.value, 50);
}

void test_score_reset() {
    Score s;
    init_score(&s);
    update_score(&s, 2000);
    
    // On vérifie que le reset remet bien tout à 0
    reset_score(&s);
    CU_ASSERT_EQUAL(s.value, 0);
    CU_ASSERT_EQUAL(s.max_x, 0);
}


int main(int argc, char* argv[]) {
    (void)argc; 
    (void)argv;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite pSuite = CU_add_suite("ScoreLogicTestSuite", 0, 0);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((CU_add_test(pSuite, "Progression et Record", test_score_progression) == NULL) ||
        (CU_add_test(pSuite, "Reset du Score", test_score_reset) == NULL)) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}