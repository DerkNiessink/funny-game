/* rooster.c

   Deze module is de implementatie van rooster.h.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rooster.h"


typedef struct rooster_data {
   toestand  toestand;
   int       breedte;
   int       hoogte;
   int      *rooster;
} rooster;


rooster *rooster_maak(int breedte, int hoogte, float prob) {
   int buren[8] = {0, 1, 1, 0, 0, -1, -1, 0};
   int grootte = breedte * hoogte;
   rooster *rooster1 = malloc(sizeof(rooster));
   int *rooster = malloc(grootte * sizeof(int));

   rooster1->toestand = BEGIN;
   rooster1->breedte = breedte;
   rooster1->hoogte = hoogte;
   rooster1->rooster = rooster;


   for (int y = 0; y < hoogte; y++) {
      for (int x = 0; x < breedte; x++) {
         int i = y * breedte + x;

         if  (y == hoogte-1 || y == 0) {
            rooster1->rooster[i] = 0;
            continue;
         }

         rooster_plaats_random(rooster1, x, y, prob, 1, 0);
      }
      int i = y * breedte;
      rooster1->rooster[i] = -1;
   }

   zet_rij(rooster1, 0, 2);
   zet_kolom(rooster1, 1, 0);
   zet_kolom(rooster1, rooster_breedte(rooster1)-1, 0);

   return rooster1;
}


void rooster_plaats_random(const rooster *rp, int x, int y, float prob, int waarde_1, int waarde_2) {
   float rand_num = (double) rand() / (double) RAND_MAX;
   int i = y * rp->breedte + x;

   if (rand_num < prob) {
      rp->rooster[i] = waarde_1;
   } else {
      rp->rooster[i] = waarde_2;
      }
}


rooster *rooster_kopieer(const rooster *rp) {

   int grootte = rooster_breedte(rp) * rooster_hoogte(rp);
   rooster *rooster1 = malloc(sizeof(rooster));

   int *rooster = malloc(grootte * sizeof(int));
   memcpy(rooster, rp->rooster, grootte * sizeof(int));

   rooster1->toestand = rooster_vraag_toestand(rp);
   rooster1->breedte = rooster_breedte(rp);
   rooster1->hoogte = rooster_hoogte(rp);
   rooster1->rooster = rooster;

   return rooster1;
}


toestand rooster_vraag_toestand(const rooster *rp) {
   return rp->toestand;
}


void rooster_zet_toestand(rooster *rp, toestand t) {
   rp->toestand = t;
}


void rooster_klaar(rooster *rp) {
   free(rp->rooster);
   free(rp);
}


int rooster_breedte(const rooster *rp) {
   return rp->breedte;
}


int rooster_hoogte(const rooster *rp) {
   return rp->hoogte;
}


int rooster_bevat(const rooster *rp, int x, int y) {
   if (x >= 0 && x <= rp->breedte && y >= 0 && y < rp->hoogte) {
      return 1;
   }
   return 0;
}


char rooster_kijk(const rooster *rp, int x, int y) {
   if (rooster_bevat(rp, x, y)) {
      return rp->rooster[y * rp->breedte + x];
   }
   return 0;
}


int rooster_plaats(rooster *rp, int x, int y, int i) {
   if (rooster_bevat(rp, x, y)) {
      rp->rooster[y * rp->breedte + x] = i;
      return 1;
   }
   return 0;
}


void rooster_zoek(const rooster *rp, int d, int *x, int *y) {
   *x = *y = -1;
   for (int i = 0; i < rp->breedte*rp->hoogte; i++) {

      if (rp->rooster[i] == d) {
         *x = i % rp->breedte;
         *y = (i - *x) / rp->breedte;
      }
   }
}


void zet_rij(rooster *rp, int y, int d) {
    for (int x = 1; x < rooster_breedte(rp); x++) {
        rooster_plaats(rp, x, y, d);
    }
}


void zet_kolom(rooster *rp, int x, int d) {
    for (int y = 0; y < rooster_hoogte(rp); y++) {
        rooster_plaats(rp, x, y, d);
    }
}


void print_rooster(rooster *rp) {
    for (int y = 0; y < rooster_hoogte(rp); y++) {
        for (int x = 0; x < rooster_breedte(rp); x++) {

            if (rooster_kijk(rp, x, y) == -1) {
                printf("\n");
            } else {
                printf("%d", rooster_kijk(rp, x, y));
            }
        }
    }
    printf("\n");
}


