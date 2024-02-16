// src/engine/editor.h:281:0

enum noise_params_type
{
  type_noise_params_noop,
  type_perlin_noise_params,
  type_voronoi_noise_params,
};

struct noise_params
{
  enum noise_params_type Type;

  union
  {
    struct perlin_noise_params perlin_noise_params;
    struct voronoi_noise_params voronoi_noise_params;
  };
};


