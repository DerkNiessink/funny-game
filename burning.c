#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "burning.h"
#include "rooster.h"


int eentje_bezet(rooster *rp, int* plekken) {
    for (int i = 1; i < rooster_breedte(rp); i++) {
        if (plekken[i] != 0) {
            return 1;
        }
    }
    return 0;
}


int is_bezet(rooster *rp, int x, int y) {
    if (rooster_bevat(rp, x, y) && rooster_kijk(rp, x, y) == 1) {
        return 1;
    }
    return 0;
}


int perculating_cluster(int *plekken, rooster *rp) {
    for (int i = 0; i < (int) (sizeof(plekken) / sizeof(int)); i+=2) {
        if (plekken[i+1] == rooster_hoogte(rp)-2) {
            return 1;
        }
    }
    return 0;
}

int burn_alg(rooster *rp) {

    int step = 2;
    int rooster_grootte = rooster_breedte(rp) * rooster_hoogte(rp);
    int buren[8] = {0, 1, 1, 0, 0, -1, -1, 0};

    // initaliseer met de eerste rij
    int bezette_plekken[rooster_grootte];
    for (int i = 0; i < rooster_breedte(rp)*2; i+=2) {
        bezette_plekken[i] = i/2;
        bezette_plekken[i+1] = 0;
    }

    while (1) {
        step++;

        // initialiseer met nullen
        int nieuw_bezette_plekken[rooster_grootte];
        for (int j = 0; j < rooster_grootte; j++) {
            nieuw_bezette_plekken[j] = 0;
        }

        int k = 0;
        for (int i = 0; i < rooster_grootte; i+=2) {
            for (int j = 0; j < 8; j+=2) {
                int buur_x = bezette_plekken[i] + buren[j];
                int buur_y = bezette_plekken[i+1] + buren[j+1];
                if (is_bezet(rp, buur_x, buur_y)) {
                    rooster_plaats(rp, buur_x, buur_y, step);
                    nieuw_bezette_plekken[k] = buur_x;
                    nieuw_bezette_plekken[k+1] = buur_y;
                    k+=2;
                }
            }
        }

        if (!eentje_bezet(rp, nieuw_bezette_plekken)) {
            break;
        }

        for (int i = 0; i < rooster_grootte; i++) {
            bezette_plekken[i] = nieuw_bezette_plekken[i];
        }

    }

    // zet de buitenmuren
    zet_kolom(rp, 1, -4);
    zet_kolom(rp, rooster_breedte(rp)-1, -4);

    if (perculating_cluster(bezette_plekken, rp)) {
        zet_rij(rp, rooster_hoogte(rp)-1, 1);
        return 1;
    } else {
        return 0;
    }

}



