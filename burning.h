/*
 * Naam : Derk Niessink
 * UvAnetID : 13438921
 * Studie : BSc Informatica
 *
 * burning.h
 *
 * Deze module voert het Burning algoritme uit op een rooster van het datatype
 * "rooster" en bepaalt of er een pad is van onder naar boven.
 *
 */


#include <stdio.h>
#include "rooster.h"


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
int burn_alg(rooster *rp);
