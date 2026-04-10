#include <stdio.h>
#include "/usr/include/CUnit/Basic.h"
#include "/usr/include/CUnit/CUnit.h"
//#include <CUnit/Basic.h>
//#include <CUnit/CUnit.h>

#include "../lib/ennemi.h"
Mix_Chunk * bouleFeu = NULL;
Mix_Chunk * sonThwomp = NULL;

// Test 1 : Initialisation du Loupas
void test_init_loupas() {
    Ennemi loup;
    // On l'initialise à la position (500, 800)
    init_loupas(&loup, 500, 800);

    // Vérification des positions
    CU_ASSERT_EQUAL(loup.rect.x, 500);
    CU_ASSERT_EQUAL(loup.rect.y, 800);

    // Vérification de la hitbox (doit être 40x28)
    CU_ASSERT_EQUAL(loup.rect.w, 40);
    CU_ASSERT_EQUAL(loup.rect.h, 28);

    // Vérification de l'état initial
    CU_ASSERT_EQUAL(loup.vivant, 1);
    CU_ASSERT_EQUAL(loup.direction, -1); // Doit commencer vers la gauche
    CU_ASSERT_EQUAL(loup.state, STATE_RUN);
}

// Test 2 : Initialisation du Thwomp
void test_init_thwomp() {
    Thwomp t;
    // On l'initialise à la position (1200, 100)
    init_thwomp(&t, 1200, 100);

    // Vérification de la position et de la sauvegarde du point de départ
    CU_ASSERT_EQUAL(t.rect.x, 1200);
    CU_ASSERT_EQUAL(t.rect.y, 100);
    CU_ASSERT_EQUAL(t.startY, 100); // pour qu'il sache où remonter

    // Vérification de la hitbox (48x64)
    CU_ASSERT_EQUAL(t.rect.w, 48);
    CU_ASSERT_EQUAL(t.rect.h, 64);

    // Le Thwomp doit être immobile (IDLE) au début
    CU_ASSERT_EQUAL(t.state, THWOMP_IDLE);
    CU_ASSERT_EQUAL(t.vivant, 1);
}


int main(int argc, char* argv[]) {
    (void)argc; 
    (void)argv;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite pSuite = CU_add_suite("EnnemiInitTestSuite", 0, 0);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((CU_add_test(pSuite, "Init Loupas", test_init_loupas) == NULL) ||
        (CU_add_test(pSuite, "Init Thwomp", test_init_thwomp) == NULL)) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}