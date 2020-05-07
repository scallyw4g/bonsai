
function void
TestFunc()
{
  bool whatever = false;
  if (whatever)
  {
    /* currently this doesn't parse for some reason..
     * specifically the opening comment doesn't parse correctly.  I think. 
     */
#
  }

  if (whatever)
  {
    // Confusingly, this does parse

#if 0
#else
#endif
  }
}
