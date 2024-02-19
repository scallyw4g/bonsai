ui_id::operator bool()
{
  b32 Reuslt = (this->WindowBits | this->InteractionBits | this->ElementBits) != 0;
  return Reuslt;
}

