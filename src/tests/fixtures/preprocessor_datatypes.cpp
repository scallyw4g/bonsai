#define foo 0

struct thing1
{
  counted_string StringThing;
  // this should work
};

struct thing3
{
  // this should work
  counted_string StringThing;
};

// A random comment

struct thing2
{
  counted_string StringThing;
  /* this should
   *
   * also work */
};

/* Another
 *
 * random comment */

// With yet another comment

struct thing4
{
  /* this should
   *
   * also work */
  counted_string StringThing;
};

struct thing5
{
    int zero = 0;
};

