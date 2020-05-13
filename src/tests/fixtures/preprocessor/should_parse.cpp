#include <bonsai_types.h>

void
TestFunc()
{
  bool whatever = true;

  if (whatever)
  {
    /* currently this doesn't parse for some reason..
     * specifically the opening comment doesn't parse correctly.  I think. 
     */
#define foo
  }

  if (whatever)
  {
    // Confusingly, this does parse
#define bar
  }

  int Thing = 3 + 3;
}
