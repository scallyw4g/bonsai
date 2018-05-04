#ifndef PERLIN
#define PERLIN

#include <vector>

class perlin_noise {
  // The permutation vector
  std::vector<int> p;

 public:
  perlin_noise() = delete;
  perlin_noise(unsigned int seed = DEBUG_NOISE_SEED);

  // Get a noise value, for 2D images z can have any value
  double noise(double x, double y, double z);
 private:
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};

#endif
