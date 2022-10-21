/*
 * Naam : Derk Niessink
 * UvAnetID : 13438921
 * Studie : BSc Informatica
 *
 * burning.c
 *
 * Deze module is de implementatie van "rooster.h" en voert het Burning
 * algoritme uit op een rooster van het datatype "rooster". Hiermee wordt
 * bepaald of het rooster een pad heeft van onder naar boven.
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "burning.h"
#include "rooster.h"


const int buur[4][2] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };


/* Controleert of er minimaal een plek groter dan 0 is in een array met plekken

   rp: een pointer naar het rooster.
   plekken: een pointer naar een int array met plekken.

   Uitvoer: 1 als er minimaal 1 plek geen 0 is, anders is de uitvoer 0.
*/
int eentje_bezet(rooster *rp, int* plekken) {
    for (int i = 1; i < rooster_breedte(rp); i++) {
        if (plekken[i] != 0) {
            return 1;
        }
    }
    return 0;
}


/* Controleert of een gegeven positie in het rooster gelijk is aan 1 (i.e.
   bezet is).

   rp: een pointer naar het rooster.
   x,y: de positie van de plek die moet worden gecontroleerd.

   Uitvoer: 1 als de plek bezet is, anders 0.
*/
int is_bezet(rooster *rp, int x, int y) {
    if (rooster_bevat(rp, x, y) && rooster_kijk(rp, x, y) == 1) {
        return 1;
    }
    return 0;
}


/* Controleert of het gegeven rooster een "perculating cluster" heeft, oftwel
   een pad van onder naar boven. Dit is het geval als de y waarde van een
   positie in de laatste array met bezette plekken van het algoritme gelijk is
   aan de hoogte van het rooster - 2.

   rp: een pointer naar het rooster.
   plekken: een pointer naar de laatste bezette plekken van het algoritme.

   Uitvoer: 1 als er een "perculating cluster" is, anders 0.
*/
int perculating_cluster(int *plekken, rooster *rp) {
    for (int i = 0; i < (int) (sizeof(plekken) / sizeof(int)); i+=2) {
        if (plekken[i+1] == rooster_hoogte(rp)-2) {
            return 1;
        }
    }
    return 0;
}


/* Voert het Burning algoritme uit, waarmee wordt bepaald of een rooster een
   "perculating cluster" heeft (i.e. een pad van onder naar boven). Het
   algoritme werkt als volgt:

   Een rooster van het datatype "rooster" is geinitializeerd met op de eerste
   rij allemaal tweeen. Onthoud deze bezette plekken. Check vervolgens bij elke
   bezette plek of er een buurman gelijk is aan 1, oftwel ook bezet is. Zet de
   waarde van deze buurmannen op de waarde van de huidige stap en update de
   bezette plekken. Ga zo door tot er geen bezette plekken meer zijn of tot het
   eind wordt bereikt.

   rp: een pointer naar het rooster.

   Uitvoer: 1 als er een "perculating cluster" is, anders 0.

   side-effect: de eenen in het rooster worden aangepast naar de stap, als deze
   op het pad liggen. De buitenste kolommen worden aangepast naar -1, zodat
   deze als buitenmuren kunnen worden gebruikt in het spel.
*/
int burn_alg(rooster *rp) {

    int step = 2;
    int rooster_grootte = rooster_breedte(rp) * rooster_hoogte(rp);

    /* initaliseer met de eerste rij */
    int bezette_plekken[rooster_grootte];
    for (int i = 0; i < rooster_breedte(rp)*2; i+=2) {
        bezette_plekken[i] = i/2;
        bezette_plekken[i+1] = 0;
    }

    while (1) {
        step++;

        /* initialiseer met nullen */
        int nieuw_bezette_plekken[rooster_grootte];
        for (int j = 0; j < rooster_grootte; j++) {
            nieuw_bezette_plekken[j] = 0;
        }

        int k = 0;
        for (int i = 0; i < rooster_grootte; i+=2) {
            for (int j = 0; j < 4; j++) {
                int buur_x = bezette_plekken[i] + buur[j][0];
                int buur_y = bezette_plekken[i+1] + buur[j][1];

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



