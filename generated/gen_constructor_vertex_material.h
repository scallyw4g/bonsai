// external/bonsai_stdlib/src/mesh.h:10:0

link_internal vertex_material
VertexMaterial( v3 Color , f32 Transparency , f32 Emission  )
{
  vertex_material Reuslt = {
    .Color = Color,
    .Transparency = Transparency,
    .Emission = Emission
  };
  return Reuslt;
}

