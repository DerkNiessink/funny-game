/*
 * Naam : Derk Niessink
 * UvAnetID : 13438921
 * Studie : BSc Informatica
 *
 * rooster.h
 *
 * Deze module verzorgt het datatype "rooster". Een rooster representeert een
 * rechthoekig grid van integers.
 */


#ifndef _ROOSTER_H
#define _ROOSTER_H

#include <stdio.h>

struct rooster_data;
typedef struct rooster_data rooster;

typedef enum {
    BEGIN, AAN_HET_SPELEN, GEWONNEN, VERLOREN
} toestand;


/* Maakt een rooster waarbij posities met een bepaalde kans 1 zijn, en anders
   0. De eerste rij wordt op 2 gezet en de buitentste kolommen op 0, als
   voorbereiding op het Burning algoritme en het spel.

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


/* kiest een van twee random integers en plaatst deze in het rooster op de
   gegeven positie.

   rp: een pointer naar het rooster.
   x,y: de positie.
   kans: de kans dat waarde 1 wordt gekozen.
   waarde_1: de eerste integer die kan worden gekozen.
   waarde_2: de tweede integer die kan worden gekozen.
*/
void rooster_plaats_random(
   const rooster *rp,
   int x,
   int y,
   float kans,
   int waarde_1,
   int waarde_2
   );


/* kopieert een struct van het type rooster naar een nieuw struct van het type
   rooster.

   rp: een pointer naar het rooster.

   Uitvoer: een pointer naar een rooster (die op de heap is
   gealloceerd), dat overeenkomt met het invoer rooster.

   Als er niet voldoende geheugen kan worden gereserveerd, dan wordt
   NULL teruggegeven. (In dat geval blijft geen gereserveerd geheugen
   achter.)
  */
rooster *rooster_kopieer(const rooster *rp);


/* Checkt of een rooster gelijk is aan NULL. Sluit het programma af als dit zo 
   is.

   rp: een pointer naar het rooster.

   side-effect: Er wordt een foutmelding geprint.
*/
void rooster_check(rooster *rp);


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


/* Kijk welke integer er staat op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.

   Uitvoer: de integer op die positie, of 0 als de positie buiten het
            rooster valt.
*/
char rooster_kijk(const rooster *rp, int x, int y);


/* Schrijf een bepaalde integer op een bepaalde positie in het rooster.

   rp: een pointer naar het rooster
   x,y: de betreffende positie.
   d: de integer.

   Side effect: als (x,y) binnen het rooster ligt, wordt het object op
                de opgegeven positie geplaatst, anders verandert er niets.

   Uitvoer: 1 als het object is geplaatst, of 0 als het buiten de grenzen lag.
*/
int rooster_plaats(rooster *rp, int x, int y, int d);


/* Zoek een bepaalde integer in het rooster, en geef de coordinaten van de
   integer terug via de gegeven pointers. Let op: als er meerdere integers van
   het gezochte soort in het rooster voorkomen, is niet gedefinieerd van welke
   de positie wordt gevonden.

   rp: een pointer naar het rooster.
   d: de integer die gezocht moet worden.
   x,y: pointers naar de geheugenlocaties waar de gevonden positie moet worden
        geschreven.

   Uitvoer: via de pointers x en y. Als de integer niet wordt gevonden worden
            *x en *y op -1 gezet.
*/
void rooster_zoek(const rooster *rp, int d, int *x, int *y);


/* Zet alle waardes in een gegeven rij om naar een gegeven integer.

   rp: een pointer naar het rooster.
   y: de y coordinaat van de rij.
   d: de integer die in de rij moet worden geplaatst.
*/
void zet_rij(rooster *rp, int y, int d);


/* Zet alle waardes in een gegeven kolom om naar een gegeven integer.

   rp: een pointer naar het rooster.
   x: de x coordinaat van de kolom.
   d: de integer die in de kolom moet worden geplaatst.
*/
void zet_kolom(rooster *rp, int x, int d);


#endif
