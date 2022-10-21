/*
 * Naam : Derk Niessink
 * UvAnetID : 13438921
 * Studie : BSc Informatica
 *
 * spel.c:
 *
 * − Dit is een programma voor het spel LAVAPHOBIA waarin de speler met de
 * pijltjestoetsen een toren moet beklimmen. De speler moet zowel snel als
 * voorzichtig zijn omdat de toren zich vult met lava en sommige delen van de
 * muren al in de fik staan. De levels zijn random gegeneert met een bepaalde
 * kans op normale muren en muren die in de fik staan. Met het burning
 * algoritme wordt er gecheckt dat het level altijd een pad van onder naar
 * boven bevat.
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


/* DEZE WAARDES BEPALEN DE MOEILIJKHEID VAN HET SPEL */
#define BREEDTE    50
#define HOOGTE     40
#define PAD_KANS   0.48
#define LAVA_KANS  0.07
#define LAVA_TIJD  30

/* Spel objecten */
#define MUUR     0
#define LEEG     1
#define FINISH   2
#define GRENS    -1
#define SPELER   -2
#define LAVA     -3
#define MUUR2    -4

/* kleur paren */
#define MUUR_PAAR     1
#define FINISH_PAAR   2
#define LAVA_PAAR     3
#define SPELER_PAAR   4
#define MUUR2_PAAR    5
#define PAD_PAAR      6
#define MENU_PAAR     7

#define OFFSET_X   7
#define OFFSET_Y   3
#define OPNIEUW      1
#define NIEUW_LEVEL  2

const int buren[4][2] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
const int right[2] = {1, 0};
const int left[2] = {-1, 0};
const int up[2] = {0, -1};
const int down[2] = {0, 1};


/* Toont het gegeven rooster met ncurses. Als de pad waarde 1 is, toon
   dan de paden van onder naar boven.

   rp: een pointer naar het rooster.
   pad: boolean integer waarde van 0 of 1.
*/
void toon_rooster(rooster *rp, int pad) {
    erase();

    int kleur;
    if (pad) {
        kleur = PAD_PAAR;
    } else {
        kleur = FINISH_PAAR;
    }

    int x, y;
    for (int pos_y = 0; pos_y < rooster_hoogte(rp); pos_y++) {
        for (int pos_x = 0; pos_x < rooster_breedte(rp); pos_x++) {
            x = pos_x + OFFSET_X;
            y = pos_y + OFFSET_Y;

            switch(rooster_kijk(rp, x - OFFSET_X, y - OFFSET_Y)) {
                case SPELER:
                    attron(COLOR_PAIR(SPELER_PAAR));
                    mvaddch(y, x, 'o');
                    attroff(COLOR_PAIR(SPELER_PAAR));
                    break;
                case FINISH:
                    attron(COLOR_PAIR(kleur));
                    mvaddch(y, x, '=');
                    attroff(COLOR_PAIR(kleur));
                    break;
                case GRENS:
                    mvaddch(y, x, '\n');
                    break;
                case MUUR:
                    attron(COLOR_PAIR(MUUR_PAAR));
                    mvaddch(y, x, ' ');
                    attroff(COLOR_PAIR(MUUR_PAAR));
                    break;
                case MUUR2:
                    attron(COLOR_PAIR(MUUR2_PAAR));
                    mvaddch(y, x, ' ');
                    attroff(COLOR_PAIR(MUUR2_PAAR));
                    break;
                case LAVA:
                    attron(COLOR_PAIR(LAVA_PAAR));
                    mvaddch(y, x, ' ');
                    attroff(COLOR_PAIR(LAVA_PAAR));
                    break;

                default:
                    /* Alle integer waarden van het rooster groter dan 3 geven
                    het pad aan */
                    if (
                        pad &&
                        (rooster_kijk(rp, x - OFFSET_X, y - OFFSET_Y) > 3 ||
                        (y - OFFSET_Y) == 1)
                    ) {
                        attron(COLOR_PAIR(PAD_PAAR));
                        mvaddch(y, x, ' ');
                        attroff(COLOR_PAIR(PAD_PAAR));
                    } else {
                    mvaddch(y, x, ' ');
                    }
            }
        }
    }
    refresh();
}


/* Voert de benodigde veranderingen in het rooster door als de speler in een
   bepaalde richting probeert te bewegen.

   rp: een pointer naar het rooster
   richting: pointer naar de richting waarin de speler probeert te bewegen. De
             vier mogelijkheden voor de richting zijn (-1,0), (1,0), (0,-1),
             (0,1) voor resp. links, rechts, omhoog en omlaag.

   Side effect: het rooster wordt aangepast op basis van de handeling van
                de speler.
*/
void beweeg(rooster *rp, const int *richting) {
    int dx = richting[0];
    int dy = richting[1];
    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));
    rooster_zoek(rp, SPELER, x, y);

    switch(rooster_kijk(rp, *x + dx, *y + dy)) {
        case FINISH: rooster_zet_toestand(rp, GEWONNEN); break;
        case LAVA: rooster_zet_toestand(rp, VERLOREN); break;
        case MUUR: break;
        case MUUR2: break;
        default:
            rooster_plaats(rp, *x, *y, LEEG);
            rooster_plaats(rp, *x + dx , *y + dy, SPELER); break;
    }
    free(x);
    free(y);

}


/* Laat de lava de kamer binnen stromen tot een bepaalde hoogte. Let hierbij op
   dat in het spel y = 0 de hoogste waarde is en de positieve waarde van de
   rooster hoogte de laagste.

    rp: een pointer naar het rooster.
    y : pointers naar de huidige positie van de speler.
*/
void stroom_lava(rooster *rp, int y) {
    for (int x = 2; x < rooster_breedte(rp)-1; x++) {
        rooster_plaats(rp, x, y, LAVA);
    }
}


/* Toont het rooster met de paden van onder naar boven voor een tijdsduur van 3
   sec.

   rp: een pointer naar het rooster.
*/
void toon_pad(rooster *rp) {
    time_t wacht = time(NULL) + 3;
    while(time(NULL) < wacht) {
        toon_rooster(rp, 1);
    }
}


/* Verandert random muren in lava met een kans van LAVA_KANS. Alleen muren die
   grenzen aan een LEEG kunnen in lava te veranderen.

   rp: een pointer naar het rooster.
*/
void plaats_random_lava(rooster *rp) {
    int buur_x;
    int buur_y;

    for (int y = 1; y < rooster_hoogte(rp)-1; y++) {
        for (int x = 1; x < rooster_breedte(rp)-1; x++) {

            for (int i = 0; i < 4; i++) {
                buur_x = x + buren[i][0];
                buur_y = y + buren[i][1];
                int buur_kar = rooster_kijk(rp, buur_x, buur_y);
                int kar = rooster_kijk(rp, x, y);

                if (kar == MUUR && buur_kar >= 1) {
                    rooster_plaats_random(rp, x, y, LAVA_KANS, LAVA, MUUR);
                }
            }
        }
    }
}


/* Speelt het spel met een gegeven rooster tot de toestand niet langer
   AAN_HET_SPELEN is. Verandert eerst een random muren in lava en past
   vervolgens het rooster aan op basis van de handelingen van de speler. Laat
   elke LAVA_TIJD de lava steeds verder de kamer vullen.

   rp: een pointer naar het rooster.
*/
void speel(rooster *rp) {

    int lava_y = rooster_hoogte(rp);
    int tijd = 0;
    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));

    toon_pad(rp);
    plaats_random_lava(rp);
    rooster_zet_toestand(rp, AAN_HET_SPELEN);


    while(rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {

        rooster_zoek(rp, SPELER, x, y);

        if (*y == lava_y) {
            rooster_zet_toestand(rp, VERLOREN);
            break;
        }

        if (tijd % LAVA_TIJD == 0 && *y < rooster_hoogte(rp)-2) {
            stroom_lava(rp, lava_y);
            lava_y--;
        }

        toon_rooster(rp, 0);
        int toets = getch();
        switch (toets) {
            case KEY_LEFT: beweeg(rp, left); break;
            case KEY_RIGHT: beweeg(rp, right); break;
            case KEY_UP: beweeg(rp, up); break;
            case KEY_DOWN: beweeg(rp, down); break;
        }

        tijd++;
        napms(16);
    }
    free(x);
    free(y);
}


/* Maakt een random rooster en check met het Burning algoritme of deze een pad
   heeft van onder naar boven. Als dit het geval is, plaats de buitenmuren en
   de speler.

   Side-effect: Sluit het spel af en geef een foutmelding als er iets fout is
   gegaan bij het maken van een rooster of als het te lang duurt.

   Uitvoer: een pointer naar het rooster.
*/
rooster *vind_random_rooster(void) {

    rooster *rp;
    int tijd = time(0);
    while(1) {

        if ((time(0) - tijd) > 20) {
            printf("Er kon geen level gegegeneerd worden binnen de maximale tijd. ");
            printf("Probeer eventueel de PAD_KANS bovenaan het programma te verhogen.\n");
            exit(1);
        }

        rp = rooster_maak(BREEDTE, HOOGTE, PAD_KANS);
        rooster_check(rp);

        if (burn_alg(rp) == 1) {
            break;
        }
        rooster_klaar(rp);
    }

    rooster_plaats(rp, rooster_breedte(rp)-2, rooster_hoogte(rp)-1, SPELER);
    rooster_plaats(rp, rooster_breedte(rp)-1, rooster_hoogte(rp)-1, MUUR2);
    rooster_plaats(rp, 1, rooster_hoogte(rp)-1, MUUR2);

    return rp;
}


/* Tekent een window border.

   window: een pointer naar het window.
*/
void teken_grens(WINDOW *window) {

    for (int y = 0; y < getmaxy(window); y++) {
        mvwprintw(window, y, 0, "|");
        mvwprintw(window, y, getmaxx(window)-1, "|");
    }
    for (int x = 0; x < getmaxx(window); x++) {
        mvwprintw(window, 0, x, "~");
        mvwprintw(window, getmaxy(window)-1, x, "~");
    }
}


/* Maakt een window bestemt voor een menu

   Uitvoer: een pointer naar het window.
*/
WINDOW *maak_menu(void) {
    int menu_breedte = BREEDTE + 4;
    int menu_hoogte = 9;

    WINDOW *window = newwin(
        menu_hoogte,
        menu_breedte,
        HOOGTE / 2 - OFFSET_Y,
        BREEDTE / 2 + OFFSET_X - menu_breedte/2
    );

    wbkgd(window, COLOR_PAIR(MENU_PAAR));
    return window;
}


/* Toont het menu waarin de speler kan kiezen tussen het spel verlaten, het
   level opnieuw spelen of een nieuw level genereren. Speelt het spel opnieuw
   en roept zichzelf aan als de speler het spel niet wilt verlaten. Het
   onaangepaste rooster is een argument, omdat deze wordt hergebruikt als de
   speler kiest om opnieuw te spelen.

   rp: een pointer naar het gespeelde rooster.
   rp_onaangepast: een pointer naar het ongespeelde rooster.

   Uitvoer: 0 als de speler het spel heeft verlaten.
*/
int toon_menu(rooster *rp, rooster *rp_onaangepast) {

    int toets;
    int keuze;
    int verlaat = 0;

    WINDOW *menu = maak_menu();
    while(!verlaat) {

        teken_grens(menu);
        if (rooster_vraag_toestand(rp) == GEWONNEN) {
            mvwprintw(menu, 2, 4, "Je hebt gewonnen! :)");
        } else {
            mvwprintw(menu, 2, 4, "Je hebt verloren :(");
        }
        mvwprintw(menu, 4, 2, "> \"R\": speel opnieuw");
        mvwprintw(menu, 5, 2, "> \"G\": genereer nieuw level");
        mvwprintw(menu, 6, 2, "> \"Q\": verlaat spel");
        wrefresh(menu);

        int toets = getch();
        switch(toets) {
            case 'q': verlaat = 1; break;
            case 'r': verlaat = 1; keuze = OPNIEUW; break;
            case 'g': verlaat = 1; keuze = NIEUW_LEVEL; break;
        }
    }
    rooster_klaar(rp);

    switch(keuze) {
        case OPNIEUW:;
            rooster *rp_onaangepast_kopie = rooster_kopieer(rp_onaangepast);
            rooster_check(rp_onaangepast_kopie);
            speel(rp_onaangepast);
            return toon_menu(rp_onaangepast, rp_onaangepast_kopie);

        case NIEUW_LEVEL:
            erase();
            refresh();
            rooster_klaar(rp_onaangepast);
            rooster *nieuw_rooster = vind_random_rooster();
            rooster *nieuw_rooster_kopie = rooster_kopieer(nieuw_rooster);
            rooster_check(nieuw_rooster);
            rooster_check(nieuw_rooster_kopie);
            speel(nieuw_rooster);
            return toon_menu(nieuw_rooster, nieuw_rooster_kopie);
    }

    rooster_klaar(rp_onaangepast);
    return 0;
}


/* Initializeer ncurses functies en kleuren. */
void init_ncurses(void) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(0);
    start_color();
    init_color(COLOR_GREEN, 0, 700, 0);
    init_pair(MUUR_PAAR, COLOR_YELLOW, COLOR_CYAN);
    init_pair(FINISH_PAAR, COLOR_GREEN, COLOR_BLACK);
    init_pair(LAVA_PAAR, COLOR_RED, COLOR_RED);
    init_pair(SPELER_PAAR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(MUUR2_PAAR, COLOR_BLUE, COLOR_BLUE);
    init_pair(PAD_PAAR, COLOR_GREEN, COLOR_GREEN);
    init_pair(MENU_PAAR, COLOR_CYAN, COLOR_BLACK);
}


int main(int argc, char *argv[]) {

    srand(time(NULL));

    rooster *rooster1 = vind_random_rooster();
    rooster *rooster1_kopie = rooster_kopieer(rooster1);
    rooster_check(rooster1);
    rooster_check(rooster1_kopie);

    init_ncurses();

    speel(rooster1);
    toon_menu(rooster1, rooster1_kopie);
    endwin();

    return 0;
}
