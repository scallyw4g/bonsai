// external/bonsai_stdlib/src/mesh.h:10:0

link_internal vertex_material
VertexMaterial( u16 ColorIndex , f32 Transparency , f32 Emission  )
{
  vertex_material Reuslt = {
    .ColorIndex = ColorIndex,
    .Transparency = Transparency,
    .Emission = Emission
  };
  return Reuslt;
}

