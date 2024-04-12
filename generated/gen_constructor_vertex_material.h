// external/bonsai_stdlib/src/mesh.h:35:0

link_internal vertex_material
VertexMaterial( u16  ColorIndex , u8  Transparency , u8  Emission  )
{
  vertex_material Reuslt = {
    .ColorIndex = ColorIndex,
    .Transparency = Transparency,
    .Emission = Emission
  };
  return Reuslt;
}

