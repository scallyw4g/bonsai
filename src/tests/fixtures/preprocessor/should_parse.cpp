#include <bonsai_types.h>

#define Value 1

function void
TestFunc()
{
  bool whatever = Value;

  if (whatever)
  {
    /* currently this doesn't parse for some reason..
     * specifically the opening comment doesn't parse correctly.  I think. 
     */
#define foo
  }

  if (Value)
  {
    // Confusingly, this does parse
#define bar
  }

  u32 Thing = Value + 3;
}
