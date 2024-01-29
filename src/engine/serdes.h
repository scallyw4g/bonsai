#define LEVEL_FILE_DEBUG_OBJECT_DELIM (0xFCACFCACFCACFCAC)

#define LEVEL_FILE_DEBUG_MODE (1)

#if LEVEL_FILE_DEBUG_MODE
       
#define MAYBE_WRITE_DEBUG_OBJECT_DELIM() { u64 Tag = LEVEL_FILE_DEBUG_OBJECT_DELIM; Ensure( Serialize(Bytes, &Tag) ); }
#define MAYBE_READ_DEBUG_OBJECT_DELIM() { u64 Tag = Read_u64(Bytes); if ( Tag != LEVEL_FILE_DEBUG_OBJECT_DELIM ) { Result = False; Error("Reading Object Delim Failed in file (" __FILE__ ":" STRINGIZE(__LINE__) ")"); } }
#else
#define MAYBE_WRITE_DEBUG_OBJECT_DELIM(...)
#define MAYBE_READ_DEBUG_OBJECT_DELIM(...)
#endif

poof(
  func serdes_cursor(type)
  {
    link_internal b32
    Serialize(u8_cursor_block_array *Bytes, (type.name)_cursor* Cursor)
    {
      u64 ElementCount = AtElements(Cursor);
      b32 Result = Write(Bytes, ElementCount);

      RangeIterator_t(u64, EIndex, ElementCount)
      {
        Result &= Serialize(Bytes, Cursor->Start+EIndex);
      }

      MAYBE_WRITE_DEBUG_OBJECT_DELIM();
      return Result;
    }

    link_internal b32
    Deserialize(u8_cursor *Bytes, (type.name)_cursor* Cursor, memory_arena *Ignored)
    {
      u64 ElementCount = Read_u64(Bytes);

      type.name Element;

      b32 Result = True;
      RangeIterator_t(u64, EIndex, ElementCount)
      {
        Result &= Deserialize(Bytes, &Element, Ignored);
        Push(Element, Cursor);
      }

      MAYBE_READ_DEBUG_OBJECT_DELIM();
      return Result;
    }
  }
)

poof(
  func serialize_vector(type)
  {
    link_internal b32
    Serialize(u8_cursor_block_array *Bytes, (type.name) *Element)
    {
      b32 Result = Write(Bytes, Cast(u8*, Element), sizeof((type.name)));

      MAYBE_WRITE_DEBUG_OBJECT_DELIM();
      return Result;
    }
  }
)

poof(
  func deserialize_vector(type)
  {
    link_internal b32
    Deserialize(u8_cursor *Bytes, (type.name)* Element, memory_arena *Ignored)
    {
      *Element = *Cast((type.name)*, Bytes->At);
      Bytes->At += sizeof((type.name));
      Assert(Bytes->At <= Bytes->End);

      b32 Result = True;
      MAYBE_READ_DEBUG_OBJECT_DELIM();
      return Result;
    }
  }
)

struct member_info
{
  cs Type;
  cs Name;
  u64 Hash;

  /* u32 OffsetOfInBytes; */
};

poof(block_array(member_info, {8}))
#include <generated/block_array_member_info_688856411.h>

struct bonsai_type_info
{
  cs Name;
  u64 Version;

  u32 SizeOfInBytes;

  /* member_info_block_array Members; */
};

poof(are_equal(bonsai_type_info))
#include <generated/are_equal_bonsai_type_info.h>

link_internal umm
Hash(bonsai_type_info *Type)
{
  umm Result = Hash(Type->Name);
  return Result;
}

poof(buffer(bonsai_type_info))
#include <generated/buffer_bonsai_type_info.h>

poof(hashtable(bonsai_type_info))
#include <generated/hashtable_bonsai_type_info.h>

poof(hashtable_to_buffer(bonsai_type_info))
#include <generated/hashtable_to_buffer_bonsai_type_info.h>

poof(maybe(bonsai_type_info))
#include <generated/maybe_bonsai_type_info.h>

poof(hashtable_get(bonsai_type_info, {cs}, {Name}))
#include <generated/hashtable_get_bonsai_type_info_902_689333910.h>

global_variable bonsai_type_info_hashtable
Global_SerializeTypeTable;

global_variable memory_arena*
Global_SerializeTypeTableArena;


poof(
  func serialize_struct(type)
  {
    link_internal bonsai_type_info
    TypeInfo((type.name) *Ignored)
    {
      bonsai_type_info Result = {};

      Result.Name = CSz("type.name");
      Result.Version = type.has_tag(version)? { type.tag_value(version) } { 0 };

      /* type.map(member) */
      /* { */
      /*   { */
      /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
      /*     Push(&Result.Members, &Member); */
      /*   } */
      /* } */

      return Result;
    }

    link_internal b32
    Serialize(u8_cursor_block_array *Bytes, (type.name) *Element)
    {
      u64 PointerTrue = True; 
      u64 PointerFalse = False; 

      b32 Result = True;

      type.has_tag(version)?
      {
        Upsert(TypeInfo(Element), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
        u64 VersionNumber = type.tag_value(version);
        Serialize(Bytes, &VersionNumber);
      }

      type.map(member)
      {
        member.has_tag(no_serialize)?
        {
        }
        {
          member.has_tag(custom_serialize)?
          {
            member.tag_value(custom_serialize)
          }
          {
            member.is_pointer?
            {
              if (Element->(member.name)) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
              else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }
            }
            {
              member.is_enum?
              {
                Result &= Serialize(Bytes, (u32*)&Element->(member.name));
              }
              {
                member.is_array?
                {
                  Result &= SerializeArray(Bytes, Element->(member.name), member.array);
                }
                {
                  Result &= Serialize(Bytes, &Element->(member.name));
                }
              }
            }
          }
        }
      }

      type.map(member)
      {
        member.has_tag(no_serialize)?
        {
        }
        {
          member.is_pointer?
          {
            if (Element->(member.name)) { Result &= Serialize(Bytes, Element->(member.name)); }
          }
        }
      }

      MAYBE_WRITE_DEBUG_OBJECT_DELIM();
      return Result;
    }
  }
)

poof(
  func deserialize_versioned_struct(type, type_poof_index version_max)
  {
    link_internal b32
    DeserializeVersioned(u8_cursor *Bytes, type.name *Element, bonsai_type_info *TypeInfo, u64 Version, memory_arena *Memory)
    {
      Assert(Version <= version_max);

      b32 Result = True;

      version_max.map(this_version)
      {
        if (Version == this_version)
        {
          (type.name)_(this_version) T(this_version) = {};
          Result &= Deserialize(Bytes, &T(this_version), Memory);
          Marshal(&T(this_version), Element);
        }
      }

      return Result;
    }
  }

)

poof(
  func deserialize_struct(type)
  {
    link_internal b32
    Deserialize(u8_cursor *Bytes, (type.name) *Element, memory_arena *Memory);

    link_internal b32
    DeserializeUnversioned(u8_cursor *Bytes, (type.name) *Element, memory_arena *Memory)
    {
      b32 Result = True;
      type.map(member)
      {
        member.has_tag(no_serialize)?
        {
        }
        {
          member.has_tag(custom_deserialize)?
          {
            member.tag_value(custom_deserialize)
          }
          {
            member.is_pointer?
            {
              b64 Had(member.name)Pointer = Read_u64(Bytes);
              Assert(Had(member.name)Pointer < 2); // Should be 0 or 1
            }
            {
              member.is_enum?
              {
                Element->(member.name) = Cast((member.type), Read_u32(Bytes));
              }
              {
                member.is_array?
                {
                  RangeIterator(ElementIndex, member.array)
                  {
                    Result &= Deserialize(Bytes, &Element->(member.name)[ElementIndex]);
                  }
                }
                {
                  // NOTE(Jesse): Unfortunately we can't check for primitives because
                  // strings are considered primitive, but need memory to deserialize
                  Result &= Deserialize(Bytes, &Element->(member.name), Memory);
                }
              }
            }
          }
        }
      }

      type.map(member)
      {
        member.is_pointer?
        {
          member.has_tag(no_serialize)?
          {
          }
          {
            if (Had(member.name)Pointer)
            {
              if (Element->member.name == 0) { Element->member.name = Allocate((member.type), Memory, 1); }
              Result &= Deserialize(Bytes, Element->(member.name), Memory);
            }
          }
        }
      }
      return Result;
    }

    link_internal b32
    Deserialize(u8_cursor *Bytes, (type.name) *Element, memory_arena *Memory)
    {
      b32 Result = True;

      type.has_tag(version)?
      {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("type.name"));

        if (MaybeSerializedType.Tag)
        {

          u64 VersionNumber = 0;
          if (MaybeSerializedType.Value.Version > 0)
          {
            Deserialize(Bytes, &VersionNumber, Memory);
          }
          Result &= DeserializeVersioned(Bytes, Element, &MaybeSerializedType.Value, VersionNumber, Memory);
        }
        else
        {
          Result &= DeserializeUnversioned(Bytes, Element, Memory);
          MAYBE_READ_DEBUG_OBJECT_DELIM();
        }
      }
      {
        Result &= DeserializeUnversioned(Bytes, Element, Memory);
        MAYBE_READ_DEBUG_OBJECT_DELIM();
      }

      return Result;
    }
  }
)


poof(
  func serialize_array(type)
  {
    link_internal b32
    SerializeArray(u8_cursor_block_array *Bytes, (type.name) *Element, umm Count)
    {
      Assert(Count);
      {
        RangeIterator_t(umm, ElementIndex, Count)
        {
          Serialize(Bytes, Element+ElementIndex);
        }
      }
      return True;
    }
  }
)

poof(
  func deserialize_array(type)
  {
    link_internal b32
    DeserializeArray(u8_cursor *Bytes, (type.name) **Dest, umm Count, memory_arena *Memory)
    {
      Assert(Count);
      if (*Dest == 0) { *Dest = Allocate((type.name), Memory, Count); }
      RangeIterator_t(umm, ElementIndex, Count)
      {
        Deserialize(Bytes, (*Dest)+ElementIndex, Memory);
      }
      return True;
    }
  }
)

poof(
  func serdes_array(type)
  {
    serialize_array(type)
    deserialize_array(type)
  }
)

poof(
  func serdes_struct(type)
  {
    serialize_struct(type)
    deserialize_struct(type)
  }
)

poof(
  func serdes_vector(type)
  {
    serialize_vector(type)
    deserialize_vector(type)
  }
)


poof(
  func serdes_primitive(type_poof_symbol list)
  {
    list.map(type)
    {
      link_internal b32
      Serialize(u8_cursor_block_array *Bytes, (type.name) *Element, memory_arena *Ignored = 0)
      {
        b32 Result = Write(Bytes, Cast(u8*, Element), sizeof((type.name)));
        return Result;
      }

      link_internal b32
      Deserialize(u8_cursor *Bytes, (type.name) *Element, memory_arena *Ignored = 0)
      {
        *Element = *Cast((type.name)*, Bytes->At);
        Bytes->At += sizeof((type.name));
        Assert(Bytes->At <= Bytes->End);
        return True;
      }

      serdes_array(type)
    }
  }
)


link_internal b32
Serialize(u8_cursor_block_array *Bytes, cs *Element, memory_arena *Ignored = 0)
{
  u64 Count = u64(Element->Count);
  b32 Result  = Write(Bytes, Cast(u8*, &Count), sizeof(u64));
      Result &= Write(Bytes, Cast(u8*, Element->Start), Element->Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, cs *Element, memory_arena *Memory)
{
  Element->Count = *Cast(u64*, Bytes->At);
  Bytes->At += sizeof(u64);

  Element->Start = Allocate(const char, Memory, Element->Count);
  CopyMemory(Bytes->At, Cast(u8*, Element->Start), Element->Count);

  Bytes->At += Element->Count;

  Assert(Bytes->At <= Bytes->End);
  return True;
}




poof(serdes_primitive({u8 s8 u16 s16 u32 s32 u64 s64 b8 r32 r64}))
#include <generated/serdes_primitive_592771169.h>
