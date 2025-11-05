# Editor

To enter world edit mode, select "World Edit" from the menu in the top-left corner
of the screen.

## Brushes

Brushes are the backbone of the world editing system.  Brushes describe how a
world edit affects the selected area.

### Creation

To create a brush, select "New" from the "Brush Settings" window.

### Brush Settings Window

Here we'll detail each option in the "Brush Settings window, in order.

* Brush Name
  The name of the brush, on disk.

* Layer Count
  The number of layers the brush has

* Affect World
  Should the brush interact with voxels from world-gen, or strictly voxels from
  edits.

---

### ----- LAYERS ------

* Brush Type : Noise | Shape
  Changes the type of the brush

* Brush Type(Noise)
    * Noise Type : Perlin | Voronoi | White


* Value Modifier
    * ClampPos : max(0, sample), after applying ValueBias and Power
    * ClampNeg : min(0, sample), after applying ValueBias and Power
    * Threshold : Set sample to 0 if it does not pass threshold, before Power and ValueBias
    * Discard : Set sample to 0
  
* Blend Mode
    * Additive    : output = sample + previous
    * Subtractive : output = previous - sample
    * Multiply    : output = previous * sample
    * Threshold   : output = sample > threshold ? threshold : sample
    * Disabled    : output = output
