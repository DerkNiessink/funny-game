/*
 * Naam : Derk Niessink
 * UvAnetID : 13438921
 * Studie : BSc Informatica
 *
 * opdracht_2.c:
 * − Dit is een programma voor een spel waarbij de speler zich met de
 * pijltjestoetsen door een doolhof moet navigeren om de schat te bereiken en
 * de bommen moet ontwijken. Het spel wordt gespeeld in de command line. Een
 * path naar een tekst file met een doolhof moet door de gebruiker worden
 * gespecificeerd bij het opstarten van het spel. Het programma accepteert
 * alleen doolhoven, waarin alle rijen en kolommen even lang zijn.
 *
 */


#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "rooster.h"
#include "burning.h"


#define MUUR     0
#define LEEG     1
#define FINISH   2
#define GRENS    -1
#define SPELER   -2
#define LAVA     -3
#define MUUR2    -4

#define MUUR_PAAR     1
#define FINISH_PAAR   2
#define LAVA_PAAR     3
#define SPELER_PAAR   4
#define MUUR2_PAAR    5
#define PAD_PAAR      6



/* Toont het gegeven rooster met ncurses.

   rp: een pointer naar het rooster.
*/
void toon_rooster(rooster *rp, int pad) {
    erase();

    int kleur;
    if (pad) {
        kleur = PAD_PAAR;
    } else {
        kleur = FINISH_PAAR;
    }

    int offset_y, offset_x;
    for (int y = 0; y < rooster_hoogte(rp); y++) {
        for (int x = 0; x < rooster_breedte(rp); x++) {
            offset_x = 60 + x;
            offset_y = y + 2;

            switch(rooster_kijk(rp, x, y)) {
                case SPELER:
                    attron(COLOR_PAIR(SPELER_PAAR));
                    mvaddch(offset_y, offset_x, 'o');
                    attroff(COLOR_PAIR(SPELER_PAAR)); break;
                case FINISH:
                    attron(COLOR_PAIR(kleur));
                    mvaddch(offset_y, offset_x, '=');
                    attroff(COLOR_PAIR(kleur)); break;
                case GRENS:
                    mvaddch(offset_y, offset_x, '\n'); break;
                case MUUR:
                    attron(COLOR_PAIR(MUUR_PAAR));
                    mvaddch(offset_y, offset_x, ' ');
                    attroff(COLOR_PAIR(MUUR_PAAR)); break;
                case MUUR2:
                    attron(COLOR_PAIR(MUUR2_PAAR));
                    mvaddch(offset_y, offset_x, ' ');
                    attroff(COLOR_PAIR(MUUR2_PAAR)); break;
                case LAVA:
                    attron(COLOR_PAIR(LAVA_PAAR));
                    mvaddch(offset_y, offset_x, ' ');
                    attroff(COLOR_PAIR(LAVA_PAAR)); break;
                default:
                    if (pad && (rooster_kijk(rp, x, y) > 3 || y == 1)) {
                        attron(COLOR_PAIR(PAD_PAAR));
                        mvaddch(offset_y, offset_x, ' ');
                        attroff(COLOR_PAIR(PAD_PAAR));
                    } else {
                    mvaddch(offset_y, offset_x, ' ');
                    }
            }
        }
    }
    refresh();
}


/* Voert de benodigde veranderingen in het rooster door als de speler in een
   bepaalde richting probeert te bewegen.

   rp   : een pointer naar het rooster
   dx,dy: de richting waarin de speler probeert te bewegen. De vier mogelijk-
          heden voor (dx,dy) zijn (-1,0), (1,0), (0,-1), (0,1) voor resp.
          links, rechts, omhoog en omlaag.

   Side effect: het rooster wordt aangepast op basis van de handeling van
                de speler.
*/
void beweeg(rooster *rp, int dx, int dy, int *x, int *y) {

    switch(rooster_kijk(rp, *x + dx, *y + dy)) {
        case FINISH: rooster_zet_toestand(rp, GEWONNEN); break;
        case LAVA: rooster_zet_toestand(rp, VERLOREN); break;
        case MUUR: break;
        case MUUR2: break;
        default:
            rooster_plaats(rp, *x, *y, LEEG);
            rooster_plaats(rp, *x + dx , *y + dy, SPELER); break;
    }

}


void stroom_lava(rooster *rp, int y) {
    for (int x = 2; x < rooster_breedte(rp)-1; x++) {
        rooster_plaats(rp, x, y, LAVA);
    }
}


void toon_pad(rooster *rp) {
    time_t wacht = time(NULL) + 3;
    while(time(NULL) < wacht) {
        toon_rooster(rp, 1);
    }
}


/* Speelt het spel met een gegeven rooster tot de toestand niet langer
   AAN_HET_SPELEN is.
 */
void speel(rooster *rp) {

    toon_pad(rp);
    int lava_y = rooster_hoogte(rp);
    int tijd = 0;
    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));

    rooster_zet_toestand(rp, AAN_HET_SPELEN);

    rooster_plaats(rp, rooster_breedte(rp)-2, rooster_hoogte(rp)-1, SPELER);
    rooster_plaats(rp, rooster_breedte(rp)-1, rooster_hoogte(rp)-1, MUUR2);
    rooster_plaats(rp, 1, rooster_hoogte(rp)-1, MUUR2);

    while(rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {

        rooster_zoek(rp, SPELER, x, y);

        if (*y == lava_y) {
            rooster_zet_toestand(rp, VERLOREN);
            break;
        }

        // Laat de lava om de twee beurten omhoog stromen
        if (tijd % 2 == 0 && *y < rooster_hoogte(rp)-2) {
            stroom_lava(rp, lava_y);
            lava_y--;
        }

        toon_rooster(rp, 0);
        int toets = getch();
        switch (toets) {
            case KEY_LEFT: beweeg(rp, -1, 0, x, y); break;
            case KEY_RIGHT: beweeg(rp, 1, 0, x, y); break;
            case KEY_UP: beweeg(rp, 0, -1, x, y); break;
            case KEY_DOWN: beweeg(rp, 0, 1, x, y); break;
        }
        tijd++;

    }
    free(x);
    free(y);
}


rooster *vind_random_rooster(void) {

    rooster *rooster1;
    while(1) {
        rooster1 = rooster_maak(50, 50, 0.5);
        if (start_algoritme(rooster1) == 1) {
            break;
        }
        rooster_klaar(rooster1);
    }
    return rooster1;
}


void init_ncurses(void) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    init_color(COLOR_GREEN, 0, 700, 0);
    curs_set(0);
    init_pair(MUUR_PAAR, COLOR_YELLOW, COLOR_CYAN);
    init_pair(FINISH_PAAR, COLOR_GREEN, COLOR_BLACK);
    init_pair(LAVA_PAAR, COLOR_RED, COLOR_RED);
    init_pair(SPELER_PAAR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(MUUR2_PAAR, COLOR_BLUE, COLOR_BLUE);
    init_pair(PAD_PAAR, COLOR_GREEN, COLOR_GREEN);
}


int main(int argc, char *argv[]) {

    srand(time(NULL));
    rooster *rooster1 = vind_random_rooster();
    init_ncurses();

    speel(rooster1);
    endwin();

    if (rooster_vraag_toestand(rooster1) == GEWONNEN) {
        printf("Je hebt gewonnen! :)\n");
    } else {
        printf("Je hebt verloren :(\n");
    }

    rooster_klaar(rooster1);

    return 0;
}
