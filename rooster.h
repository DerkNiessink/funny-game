/* rooster.h

   Deze module verzorgt het datatype "rooster". Een rooster representeert een
   rechthoekig grid van objecten. Elk object is in dit rooster een char.

   Deze header file beschrijft het interface voor "rooster".
   De implementatie, in "rooster.c", moet je grotendeels zelf schrijven.
*/

#ifndef _ROOSTER_H
#define _ROOSTER_H

#include <stdio.h>

struct rooster_data;
typedef struct rooster_data rooster;

typedef enum {
    BEGIN, AAN_HET_SPELEN, GEWONNEN, VERLOREN
} toestand;


/* Maakt een rooster waarbij posities met een bepaalde kans 1 zijn, en anders 0.

   breedte: de breedte van het rooster.
   hoogte: de hoogte van het rooster.
   prob: de kans voor elke positie dat het een 1 is.

   Uitvoer: als alles goed gaat, een pointer naar een rooster (die op de heap is
      gealloceerd), dat overeenkomt met de gegeven beschrijving.
      De begintoestand is BEGIN.

      Als er niet voldoende geheugen kan worden gereserveerd, dan wordt
      NULL teruggegeven. (In dat geval blijft geen gereserveerd geheugen
      achter.)
*/
rooster *rooster_maak(int breedte, int hoogte, float prob);


void rooster_plaats_random(const rooster *rp, int x, int y, float prob, int waarde_1, int waarde_2);


rooster *rooster_kopieer(const rooster *rp);


/* Vraag de huidige toestand van het spel op.

   rp: een pointer naar het rooster.

   Uitvoer: de toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
*/
toestand rooster_vraag_toestand(const rooster *rp);


/* Verander de huidige toestand van het spel.

   rp: een pointer naar het rooster.
   t:  de nieuwe toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
*/
void rooster_zet_toestand(rooster *rp, toestand t);


/* Geef alle resources vrij die zijn gealloceerd voor een rooster.

   rp: een pointer naar het rooster.
*/
void rooster_klaar(rooster *rp);


/* Geef de breedte van het rooster, dat wil zeggen, het aantal kolommen.

   rp: een pointer naar het rooster.

   Uitvoer: de breedte.
*/
int rooster_breedte(const rooster *rp);


/* Geef de hoogte van het rooster, dat wil zeggen, het aantal rijen.

   rp: een pointer naar het rooster.

   Uitvoer: de hoogte.
*/
int rooster_hoogte(const rooster *rp);


/* Kijk of de gegeven positie binnen het rooster valt.

   rp:  een pointer naar het rooster.
   x,y: de positie.

   Uitvoer: 1 als de positie binnen het rooster valt, anders 0.
*/
int rooster_bevat(const rooster *rp, int x, int y);


/* Kijk welk object er staat op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.

   Uitvoer: het object op die positie, of '\0' als de positie buiten het
            rooster valt.
*/
char rooster_kijk(const rooster *rp, int x, int y);


/* Schrijf een bepaald object op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.
   c  : het object.

   Side effect: als (x,y) binnen het rooster ligt, wordt het object op
                de opgegeven positie geplaatst, anders verandert er niets.

   Uitvoer: 1 als het object is geplaatst, of 0 als het buiten de grenzen lag.
*/
int rooster_plaats(rooster *rp, int x, int y, int i);


/* Zoek een bepaald object in het rooster, en geef de coordinaten van het
   object terug via de gegeven pointers. Let op: als er meerdere objecten van
   het gezochte soort in het rooster voorkomen, is niet gedefinieerd van welke
   de positie wordt gevonden.

   rp : een pointer naar het rooster
   c  : het object dat moet worden gezocht
   x,y: pointers naar de geheugenlocaties waar de gevonden positie moet worden
        geschreven.

   Uitvoer: via de pointers x en y. Als het object niet wordt gevonden worden
            *x en *y op -1 gezet.
*/
void rooster_zoek(const rooster *rp, int d, int *x, int *y);


void zet_rij(rooster *rp, int y, int d);


void zet_kolom(rooster *rp, int x, int d);


void print_rooster(rooster *rp);

#endif
