#include <stdio.h>
#include "/usr/include/CUnit/Basic.h"
#include "/usr/include/CUnit/CUnit.h"
//#include <CUnit/Basic.h>
//#include <CUnit/CUnit.h>
#include "../lib/collision.h" 



// Test avec une tuile qui est un sol sur la Map 1 (ex: tuile 6)
void test_is_full_map1_tuilePleine() {
    int tile_id = 6;
    int levelID = 1;

    int result = is_full(tile_id, levelID);
    
    // Le resultat doit être 1 (Vrai)
    CU_ASSERT_EQUAL(result, 1);
}

// Test avec une tuile qui n'est pas un sol sur la Map 1 (ex: ciel, tuile 0)
void test_is_full_map1_tuileVide() {
    int tile_id = 0;
    int levelID = 1;

    int result = is_full(tile_id, levelID);
    
    // Le resultat doit être 0 (Faux)
    CU_ASSERT_EQUAL(result, 0);
}

// Test avec une tuile qui est un sol sur la Map 2 (ex: tuile 1)
void test_is_full_map2_tuilePleine() {
    int tile_id = 1;
    int levelID = 2;

    int result = is_full(tile_id, levelID);
    
    // Le resultat doit être 1 (Vrai)
    CU_ASSERT_EQUAL(result, 1);
}

// Test avec une tuile qui n'est pas un sol sur la Map 2 (ex: tuile 12)
void test_is_full_map2_tuileVide() {
    int tile_id = 12;
    int levelID = 2;

    int result = is_full(tile_id, levelID);
    
    // Le resultat doit être 0 (Faux)
    CU_ASSERT_EQUAL(result, 0);
}

// Test avec une Map qui n'existe pas (ex: Map 3) pour vérifier la sécurité
void test_is_full_mapInvalide() {
    int tile_id = 6; 
    int levelID = 3;

    int result = is_full(tile_id, levelID);
    
    // Le resultat doit être 0 car il y a pas de map 3
    CU_ASSERT_EQUAL(result, 0);
}


int main(int argc, char* argv[]) {
    (void)argc; 
    (void)argv;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("CollisionTestSuite", 0, 0);
    
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((CU_add_test(pSuite, "Test Map 1 - Tuile Pleine", test_is_full_map1_tuilePleine) == NULL) ||
        (CU_add_test(pSuite, "Test Map 1 - Tuile Vide", test_is_full_map1_tuileVide) == NULL) ||
        (CU_add_test(pSuite, "Test Map 2 - Tuile Pleine", test_is_full_map2_tuilePleine) == NULL) ||
        (CU_add_test(pSuite, "Test Map 2 - Tuile Vide", test_is_full_map2_tuileVide) == NULL) ||
        (CU_add_test(pSuite, "Test Securite - Map Invalide", test_is_full_mapInvalide) == NULL))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}