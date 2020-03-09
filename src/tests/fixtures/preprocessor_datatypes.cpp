struct thing1
{
  counted_string StringThing;
  // this should work
};

struct thing2
{
  counted_string StringThing;
  /* this should
   *
   * also work */
};

struct thing3
{
  // this should work
  counted_string StringThing;
};

struct thing4
{
  /* this should
   *
   * also work */
  counted_string StringThing;
};

