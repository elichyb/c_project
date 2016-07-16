# include <stdio.h>
# include <string.h>

# define MAX_NAMES 30
# define MAX_CHARS_IN_NAME 100

/* define the dunction getname*/
void getname();

static int pointer = 0;

/*array of names of name*/
char Name[MAX_NAMES][MAX_CHARS_IN_NAME] = {"Alex", "Tal", "Tom", "Avia", "Elichy", "Rotem", "Golan", "Yuval", "Lotem", "Ron", "Tomas", "Felura", "Yoni", "Messi", "Harry", "Tim", "Dan", "Ronaldo", "Bolt", "Roy", "Tami", "Opal", "Shone", "Liam", "Toress", "Dor", "Joe", "Gadi", "Loren", "Cavin"};