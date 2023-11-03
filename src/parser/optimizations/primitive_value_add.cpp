//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "primitive_value_add.h"

using namespace o2;

static constexpr int32_t primitive_type_combine(primitive_type t1, primitive_type t2)
{
	return ((int)t1 << (int)primitive_type::count) | (int)t2;
}

#define I8_OP_I8(op) case primitive_type_combine(primitive_type::int8, primitive_type::int8): \
    lhs->i32 = lhs->i8 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define I8_OP_U8(op) case primitive_type_combine(primitive_type::int8, primitive_type::uint8): \
    lhs->i32 = lhs->i8 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define I8_OP_I16(op) case primitive_type_combine(primitive_type::int8, primitive_type::int16): \
    lhs->i32 = lhs->i8 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define I8_OP_U16(op) case primitive_type_combine(primitive_type::int8, primitive_type::uint16): \
    lhs->i32 = lhs->i8 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define I8_OP_I32(op) case primitive_type_combine(primitive_type::int8, primitive_type::int32): \
    lhs->i32 = lhs->i8 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define I8_OP_U32(op) case primitive_type_combine(primitive_type::int8, primitive_type::uint32): \
    lhs->u32 = lhs->i8 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define I8_OP_I64(op) case primitive_type_combine(primitive_type::int8, primitive_type::int64): \
    lhs->i64 = lhs->i8 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define I8_OP_U64(op) case primitive_type_combine(primitive_type::int8, primitive_type::uint64): \
    lhs->u64 = lhs->i8 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define I8_OP_F32(op) case primitive_type_combine(primitive_type::int8, primitive_type::float32): \
    lhs->f32 = lhs->i8 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define I8_OP_F64(op) case primitive_type_combine(primitive_type::int8, primitive_type::float64): \
    lhs->f64 = lhs->i8 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define I8_OP_PTR(op) case primitive_type_combine(primitive_type::int8, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->i8 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define U8_OP_I8(op) case primitive_type_combine(primitive_type::uint8, primitive_type::int8): \
    lhs->i32 = lhs->u8 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define U8_OP_U8(op) case primitive_type_combine(primitive_type::uint8, primitive_type::uint8): \
    lhs->i32 = lhs->u8 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define U8_OP_I16(op) case primitive_type_combine(primitive_type::uint8, primitive_type::int16): \
    lhs->i32 = lhs->u8 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define U8_OP_U16(op) case primitive_type_combine(primitive_type::uint8, primitive_type::uint16): \
    lhs->i32 = lhs->u8 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define U8_OP_I32(op) case primitive_type_combine(primitive_type::uint8, primitive_type::int32): \
    lhs->i32 = lhs->u8 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define U8_OP_U32(op) case primitive_type_combine(primitive_type::uint8, primitive_type::uint32): \
    lhs->u32 = lhs->u8 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define U8_OP_I64(op) case primitive_type_combine(primitive_type::uint8, primitive_type::int64): \
    lhs->i64 = lhs->u8 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define U8_OP_U64(op) case primitive_type_combine(primitive_type::uint8, primitive_type::uint64): \
    lhs->u64 = lhs->u8 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define U8_OP_F32(op) case primitive_type_combine(primitive_type::uint8, primitive_type::float32): \
    lhs->f32 = lhs->u8 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define U8_OP_F64(op) case primitive_type_combine(primitive_type::uint8, primitive_type::float64): \
    lhs->f64 = lhs->u8 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define U8_OP_PTR(op) case primitive_type_combine(primitive_type::uint8, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->u8 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define I16_OP_I8(op) case primitive_type_combine(primitive_type::int16, primitive_type::int8): \
    lhs->i32 = lhs->i16 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define I16_OP_U8(op) case primitive_type_combine(primitive_type::int16, primitive_type::uint8): \
    lhs->i32 = lhs->i16 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define I16_OP_I16(op) case primitive_type_combine(primitive_type::int16, primitive_type::int16): \
    lhs->i32 = lhs->i16 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define I16_OP_U16(op) case primitive_type_combine(primitive_type::int16, primitive_type::uint16): \
    lhs->i32 = lhs->i16 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define I16_OP_I32(op) case primitive_type_combine(primitive_type::int16, primitive_type::int32): \
    lhs->i32 = lhs->i16 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define I16_OP_U32(op) case primitive_type_combine(primitive_type::int16, primitive_type::uint32): \
    lhs->u32 = lhs->i16 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define I16_OP_I64(op) case primitive_type_combine(primitive_type::int16, primitive_type::int64): \
    lhs->i64 = lhs->i16 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define I16_OP_U64(op) case primitive_type_combine(primitive_type::int16, primitive_type::uint64): \
    lhs->u64 = lhs->i16 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define I16_OP_F32(op) case primitive_type_combine(primitive_type::int16, primitive_type::float32): \
    lhs->f32 = lhs->i16 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define I16_OP_F64(op) case primitive_type_combine(primitive_type::int16, primitive_type::float64): \
    lhs->f64 = lhs->i16 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define I16_OP_PTR(op) case primitive_type_combine(primitive_type::int16, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->i16 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define U16_OP_I8(op) case primitive_type_combine(primitive_type::uint16, primitive_type::int8): \
    lhs->i32 = lhs->u16 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define U16_OP_U8(op) case primitive_type_combine(primitive_type::uint16, primitive_type::uint8): \
    lhs->i32 = lhs->u16 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define U16_OP_I16(op) case primitive_type_combine(primitive_type::uint16, primitive_type::int16): \
    lhs->i32 = lhs->u16 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define U16_OP_U16(op) case primitive_type_combine(primitive_type::uint16, primitive_type::uint16): \
    lhs->i32 = lhs->u16 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define U16_OP_I32(op) case primitive_type_combine(primitive_type::uint16, primitive_type::int32): \
    lhs->i32 = lhs->u16 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define U16_OP_U32(op) case primitive_type_combine(primitive_type::uint16, primitive_type::uint32): \
    lhs->u32 = lhs->u16 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define U16_OP_I64(op) case primitive_type_combine(primitive_type::uint16, primitive_type::int64): \
    lhs->i64 = lhs->u16 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define U16_OP_U64(op) case primitive_type_combine(primitive_type::uint16, primitive_type::uint64): \
    lhs->u64 = lhs->u16 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define U16_OP_F32(op) case primitive_type_combine(primitive_type::uint16, primitive_type::float32): \
    lhs->f32 = lhs->u16 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define U16_OP_F64(op) case primitive_type_combine(primitive_type::uint16, primitive_type::float64): \
    lhs->f64 = lhs->u16 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define U16_OP_PTR(op) case primitive_type_combine(primitive_type::uint16, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->u16 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define I32_OP_I8(op) case primitive_type_combine(primitive_type::int32, primitive_type::int8): \
    lhs->i32 = lhs->i32 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define I32_OP_U8(op) case primitive_type_combine(primitive_type::int32, primitive_type::uint8): \
    lhs->i32 = lhs->i32 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define I32_OP_I16(op) case primitive_type_combine(primitive_type::int32, primitive_type::int16): \
    lhs->i32 = lhs->i32 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define I32_OP_U16(op) case primitive_type_combine(primitive_type::int32, primitive_type::uint16): \
    lhs->i32 = lhs->i32 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define I32_OP_I32(op) case primitive_type_combine(primitive_type::int32, primitive_type::int32): \
    lhs->i32 = lhs->i32 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define I32_OP_U32(op) case primitive_type_combine(primitive_type::int32, primitive_type::uint32): \
    lhs->u32 = lhs->i32 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define I32_OP_I64(op) case primitive_type_combine(primitive_type::int32, primitive_type::int64): \
    lhs->i64 = lhs->i32 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define I32_OP_U64(op) case primitive_type_combine(primitive_type::int32, primitive_type::uint64): \
    lhs->u64 = lhs->i32 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define I32_OP_F32(op) case primitive_type_combine(primitive_type::int32, primitive_type::float32): \
    lhs->f32 = lhs->i32 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define I32_OP_F64(op) case primitive_type_combine(primitive_type::int32, primitive_type::float64): \
    lhs->f64 = lhs->i32 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define I32_OP_PTR(op) case primitive_type_combine(primitive_type::int32, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->i32 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define U32_OP_I8(op) case primitive_type_combine(primitive_type::uint32, primitive_type::int8): \
    lhs->u32 = lhs->u32 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define U32_OP_U8(op) case primitive_type_combine(primitive_type::uint32, primitive_type::uint8): \
    lhs->u32 = lhs->u32 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define U32_OP_I16(op) case primitive_type_combine(primitive_type::uint32, primitive_type::int16): \
    lhs->u32 = lhs->u32 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define U32_OP_U16(op) case primitive_type_combine(primitive_type::uint32, primitive_type::uint16): \
    lhs->u32 = lhs->u32 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define U32_OP_I32(op) case primitive_type_combine(primitive_type::uint32, primitive_type::int32): \
    lhs->u32 = lhs->u32 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define U32_OP_U32(op) case primitive_type_combine(primitive_type::uint32, primitive_type::uint32): \
    lhs->u32 = lhs->u32 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define U32_OP_I64(op) case primitive_type_combine(primitive_type::uint32, primitive_type::int64): \
    lhs->i64 = lhs->u32 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define U32_OP_U64(op) case primitive_type_combine(primitive_type::uint32, primitive_type::uint64): \
    lhs->u64 = lhs->u32 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define U32_OP_F32(op) case primitive_type_combine(primitive_type::uint32, primitive_type::float32): \
    lhs->f32 = lhs->u32 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define U32_OP_F64(op) case primitive_type_combine(primitive_type::uint32, primitive_type::float64): \
    lhs->f64 = lhs->u32 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define U32_OP_PTR(op) case primitive_type_combine(primitive_type::uint32, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->u32 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define I64_OP_I8(op) case primitive_type_combine(primitive_type::int64, primitive_type::int8): \
    lhs->i64 = lhs->i64 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define I64_OP_U8(op) case primitive_type_combine(primitive_type::int64, primitive_type::uint8): \
    lhs->i64 = lhs->i64 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define I64_OP_I16(op) case primitive_type_combine(primitive_type::int64, primitive_type::int16): \
    lhs->i64 = lhs->i64 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define I64_OP_U16(op) case primitive_type_combine(primitive_type::int64, primitive_type::uint16): \
    lhs->i64 = lhs->i64 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define I64_OP_I32(op) case primitive_type_combine(primitive_type::int64, primitive_type::int32): \
    lhs->i64 = lhs->i64 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define I64_OP_U32(op) case primitive_type_combine(primitive_type::int64, primitive_type::uint32): \
    lhs->i64 = lhs->i64 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define I64_OP_I64(op) case primitive_type_combine(primitive_type::int64, primitive_type::int64): \
    lhs->i64 = lhs->i64 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define I64_OP_U64(op) case primitive_type_combine(primitive_type::int64, primitive_type::uint64): \
    lhs->u64 = lhs->i64 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define I64_OP_F32(op) case primitive_type_combine(primitive_type::int64, primitive_type::float32): \
    lhs->f32 = lhs->i64 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define I64_OP_F64(op) case primitive_type_combine(primitive_type::int64, primitive_type::float64): \
    lhs->f64 = lhs->i64 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define I64_OP_PTR(op) case primitive_type_combine(primitive_type::int64, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->i64 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define U64_OP_I8(op) case primitive_type_combine(primitive_type::uint64, primitive_type::int8): \
    lhs->u64 = lhs->u64 op rhs->i8; \
    lhs->type = primitive_type::int32; \
    break
#define U64_OP_U8(op) case primitive_type_combine(primitive_type::uint64, primitive_type::uint8): \
    lhs->u64 = lhs->u64 op rhs->u8; \
    lhs->type = primitive_type::int32; \
    break
#define U64_OP_I16(op) case primitive_type_combine(primitive_type::uint64, primitive_type::int16): \
    lhs->u64 = lhs->u64 op rhs->i16; \
    lhs->type = primitive_type::int32; \
    break
#define U64_OP_U16(op) case primitive_type_combine(primitive_type::uint64, primitive_type::uint16): \
    lhs->u64 = lhs->u64 op rhs->u16; \
    lhs->type = primitive_type::int32; \
    break
#define U64_OP_I32(op) case primitive_type_combine(primitive_type::uint64, primitive_type::int32): \
    lhs->u64 = lhs->u64 op rhs->i32; \
    lhs->type = primitive_type::int32; \
    break
#define U64_OP_U32(op) case primitive_type_combine(primitive_type::uint64, primitive_type::uint32): \
    lhs->u64 = lhs->u64 op rhs->u32; \
    lhs->type = primitive_type::uint32; \
    break
#define U64_OP_I64(op) case primitive_type_combine(primitive_type::uint64, primitive_type::int64): \
    lhs->u64 = lhs->u64 op rhs->i64; \
    lhs->type = primitive_type::int64; \
    break
#define U64_OP_U64(op) case primitive_type_combine(primitive_type::uint64, primitive_type::uint64): \
    lhs->u64 = lhs->u64 op rhs->u64; \
    lhs->type = primitive_type::uint64; \
    break
#define U64_OP_F32(op) case primitive_type_combine(primitive_type::uint64, primitive_type::float32): \
    lhs->f32 = lhs->u64 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define U64_OP_F64(op) case primitive_type_combine(primitive_type::uint64, primitive_type::float64): \
    lhs->f64 = lhs->u64 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break
#define U64_OP_PTR(op) case primitive_type_combine(primitive_type::uint64, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->u64 op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

#define F32_OP_I8(op) case primitive_type_combine(primitive_type::float32, primitive_type::int8): \
    lhs->f32 = lhs->f32 op rhs->i8; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_U8(op) case primitive_type_combine(primitive_type::float32, primitive_type::uint8): \
    lhs->f32 = lhs->f32 op rhs->u8; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_I16(op) case primitive_type_combine(primitive_type::float32, primitive_type::int16): \
    lhs->f32 = lhs->f32 op rhs->i16; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_U16(op) case primitive_type_combine(primitive_type::float32, primitive_type::uint16): \
    lhs->f32 = lhs->f32 op rhs->u16; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_I32(op) case primitive_type_combine(primitive_type::float32, primitive_type::int32): \
    lhs->f32 = lhs->f32 op rhs->i32; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_U32(op) case primitive_type_combine(primitive_type::float32, primitive_type::uint32): \
    lhs->f32 = lhs->f32 op rhs->u32; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_I64(op) case primitive_type_combine(primitive_type::float32, primitive_type::int64): \
    lhs->f32 = lhs->f32 op rhs->i64; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_U64(op) case primitive_type_combine(primitive_type::float32, primitive_type::uint64): \
    lhs->f32 = lhs->f32 op rhs->u64; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_F32(op) case primitive_type_combine(primitive_type::float32, primitive_type::float32): \
    lhs->f32 = lhs->f32 op rhs->f32; \
    lhs->type = primitive_type::float32; \
    break
#define F32_OP_F64(op) case primitive_type_combine(primitive_type::float32, primitive_type::float64): \
    lhs->f64 = lhs->f32 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break

#define F64_OP_I8(op) case primitive_type_combine(primitive_type::float64, primitive_type::int8): \
    lhs->f64 = lhs->f64 op rhs->i8; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_U8(op) case primitive_type_combine(primitive_type::float64, primitive_type::uint8): \
    lhs->f64 = lhs->f64 op rhs->u8; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_I16(op) case primitive_type_combine(primitive_type::float64, primitive_type::int16): \
    lhs->f64 = lhs->f64 op rhs->i16; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_U16(op) case primitive_type_combine(primitive_type::float64, primitive_type::uint16): \
    lhs->f64 = lhs->f64 op rhs->u16; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_I32(op) case primitive_type_combine(primitive_type::float64, primitive_type::int32): \
    lhs->f64 = lhs->f64 op rhs->i32; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_U32(op) case primitive_type_combine(primitive_type::float64, primitive_type::uint32): \
    lhs->f64 = lhs->f64 op rhs->u32; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_I64(op) case primitive_type_combine(primitive_type::float64, primitive_type::int64): \
    lhs->f64 = lhs->f64 op rhs->i64; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_U64(op) case primitive_type_combine(primitive_type::float64, primitive_type::uint64): \
    lhs->f64 = lhs->f64 op rhs->u64; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_F32(op) case primitive_type_combine(primitive_type::float64, primitive_type::float32): \
    lhs->f64 = lhs->f64 op rhs->f32; \
    lhs->type = primitive_type::float64; \
    break
#define F64_OP_F64(op) case primitive_type_combine(primitive_type::float64, primitive_type::float64): \
    lhs->f64 = lhs->f64 op rhs->f64; \
    lhs->type = primitive_type::float64; \
    break

#define PTR_OP_PTR(op) case primitive_type_combine(primitive_type::ptr, primitive_type::ptr): \
    lhs->ptr = ((char*)0) + ((size_t)lhs->ptr op (size_t)rhs->ptr); \
    lhs->type = primitive_type::ptr; \
    break

bool o2::primitive_value_add(primitive_value* const lhs, const primitive_value* const rhs)
{
	switch (primitive_type_combine(lhs->type, rhs->type))
	{
	I8_OP_I8(+);
	I8_OP_U8(+);
	I8_OP_I16(+);
	I8_OP_U16(+);
	I8_OP_I32(+);
	I8_OP_U32(+);
	I8_OP_I64(+);
	I8_OP_U64(+);
	I8_OP_F32(+);
	I8_OP_F64(+);
	I8_OP_PTR(+);

	U8_OP_I8(+);
	U8_OP_U8(+);
	U8_OP_I16(+);
	U8_OP_U16(+);
	U8_OP_I32(+);
	U8_OP_U32(+);
	U8_OP_I64(+);
	U8_OP_U64(+);
	U8_OP_F32(+);
	U8_OP_F64(+);
	U8_OP_PTR(+);

	I16_OP_I8(+);
	I16_OP_U8(+);
	I16_OP_I16(+);
	I16_OP_U16(+);
	I16_OP_I32(+);
	I16_OP_U32(+);
	I16_OP_I64(+);
	I16_OP_U64(+);
	I16_OP_F32(+);
	I16_OP_F64(+);
	I16_OP_PTR(+);

	U16_OP_I8(+);
	U16_OP_U8(+);
	U16_OP_I16(+);
	U16_OP_U16(+);
	U16_OP_I32(+);
	U16_OP_U32(+);
	U16_OP_I64(+);
	U16_OP_U64(+);
	U16_OP_F32(+);
	U16_OP_F64(+);
	U16_OP_PTR(+);

	I32_OP_I8(+);
	I32_OP_U8(+);
	I32_OP_I16(+);
	I32_OP_U16(+);
	I32_OP_I32(+);
	I32_OP_U32(+);
	I32_OP_I64(+);
	I32_OP_U64(+);
	I32_OP_F32(+);
	I32_OP_F64(+);
	I32_OP_PTR(+);

	U32_OP_I8(+);
	U32_OP_U8(+);
	U32_OP_I16(+);
	U32_OP_U16(+);
	U32_OP_I32(+);
	U32_OP_U32(+);
	U32_OP_I64(+);
	U32_OP_U64(+);
	U32_OP_F32(+);
	U32_OP_F64(+);
	U32_OP_PTR(+);

	I64_OP_I8(+);
	I64_OP_U8(+);
	I64_OP_I16(+);
	I64_OP_U16(+);
	I64_OP_I32(+);
	I64_OP_U32(+);
	I64_OP_I64(+);
	I64_OP_U64(+);
	I64_OP_F32(+);
	I64_OP_F64(+);
	I64_OP_PTR(+);

	U64_OP_I8(+);
	U64_OP_U8(+);
	U64_OP_I16(+);
	U64_OP_U16(+);
	U64_OP_I32(+);
	U64_OP_U32(+);
	U64_OP_I64(+);
	U64_OP_U64(+);
	U64_OP_F32(+);
	U64_OP_F64(+);
	U64_OP_PTR(+);

	F32_OP_I8(+);
	F32_OP_U8(+);
	F32_OP_I16(+);
	F32_OP_U16(+);
	F32_OP_I32(+);
	F32_OP_U32(+);
	F32_OP_I64(+);
	F32_OP_U64(+);
	F32_OP_F32(+);
	F32_OP_F64(+);

	F64_OP_I8(+);
	F64_OP_U8(+);
	F64_OP_I16(+);
	F64_OP_U16(+);
	F64_OP_I32(+);
	F64_OP_U32(+);
	F64_OP_I64(+);
	F64_OP_U64(+);
	F64_OP_F32(+);
	F64_OP_F64(+);

	PTR_OP_PTR(+);

	default:
		return false;
	}
	return true;
}

bool o2::primitive_value_sub(primitive_value* lhs, const primitive_value* rhs)
{
	switch (primitive_type_combine(lhs->type, rhs->type))
	{
	I8_OP_I8(-);
	I8_OP_U8(-);
	I8_OP_I16(-);
	I8_OP_U16(-);
	I8_OP_I32(-);
	I8_OP_U32(-);
	I8_OP_I64(-);
	I8_OP_U64(-);
	I8_OP_F32(-);
	I8_OP_F64(-);
	I8_OP_PTR(-);

	U8_OP_I8(-);
	U8_OP_U8(-);
	U8_OP_I16(-);
	U8_OP_U16(-);
	U8_OP_I32(-);
	U8_OP_U32(-);
	U8_OP_I64(-);
	U8_OP_U64(-);
	U8_OP_F32(-);
	U8_OP_F64(-);
	U8_OP_PTR(-);

	I16_OP_I8(-);
	I16_OP_U8(-);
	I16_OP_I16(-);
	I16_OP_U16(-);
	I16_OP_I32(-);
	I16_OP_U32(-);
	I16_OP_I64(-);
	I16_OP_U64(-);
	I16_OP_F32(-);
	I16_OP_F64(-);
	I16_OP_PTR(-);

	U16_OP_I8(-);
	U16_OP_U8(-);
	U16_OP_I16(-);
	U16_OP_U16(-);
	U16_OP_I32(-);
	U16_OP_U32(-);
	U16_OP_I64(-);
	U16_OP_U64(-);
	U16_OP_F32(-);
	U16_OP_F64(-);
	U16_OP_PTR(-);

	I32_OP_I8(-);
	I32_OP_U8(-);
	I32_OP_I16(-);
	I32_OP_U16(-);
	I32_OP_I32(-);
	I32_OP_U32(-);
	I32_OP_I64(-);
	I32_OP_U64(-);
	I32_OP_F32(-);
	I32_OP_F64(-);
	I32_OP_PTR(-);

	U32_OP_I8(-);
	U32_OP_U8(-);
	U32_OP_I16(-);
	U32_OP_U16(-);
	U32_OP_I32(-);
	U32_OP_U32(-);
	U32_OP_I64(-);
	U32_OP_U64(-);
	U32_OP_F32(-);
	U32_OP_F64(-);
	U32_OP_PTR(-);

	I64_OP_I8(-);
	I64_OP_U8(-);
	I64_OP_I16(-);
	I64_OP_U16(-);
	I64_OP_I32(-);
	I64_OP_U32(-);
	I64_OP_I64(-);
	I64_OP_U64(-);
	I64_OP_F32(-);
	I64_OP_F64(-);
	I64_OP_PTR(-);

	U64_OP_I8(-);
	U64_OP_U8(-);
	U64_OP_I16(-);
	U64_OP_U16(-);
	U64_OP_I32(-);
	U64_OP_U32(-);
	U64_OP_I64(-);
	U64_OP_U64(-);
	U64_OP_F32(-);
	U64_OP_F64(-);
	U64_OP_PTR(-);

	F32_OP_I8(-);
	F32_OP_U8(-);
	F32_OP_I16(-);
	F32_OP_U16(-);
	F32_OP_I32(-);
	F32_OP_U32(-);
	F32_OP_I64(-);
	F32_OP_U64(-);
	F32_OP_F32(-);
	F32_OP_F64(-);

	F64_OP_I8(-);
	F64_OP_U8(-);
	F64_OP_I16(-);
	F64_OP_U16(-);
	F64_OP_I32(-);
	F64_OP_U32(-);
	F64_OP_I64(-);
	F64_OP_U64(-);
	F64_OP_F32(-);
	F64_OP_F64(-);

	PTR_OP_PTR(-);

	default:
		return false;
	}
	return true;
}

bool o2::primitive_value_mult(primitive_value* lhs, const primitive_value* rhs)
{
	switch (primitive_type_combine(lhs->type, rhs->type))
	{
	I8_OP_I8(*);
	I8_OP_U8(*);
	I8_OP_I16(*);
	I8_OP_U16(*);
	I8_OP_I32(*);
	I8_OP_U32(*);
	I8_OP_I64(*);
	I8_OP_U64(*);
	I8_OP_F32(*);
	I8_OP_F64(*);

	U8_OP_I8(*);
	U8_OP_U8(*);
	U8_OP_I16(*);
	U8_OP_U16(*);
	U8_OP_I32(*);
	U8_OP_U32(*);
	U8_OP_I64(*);
	U8_OP_U64(*);
	U8_OP_F32(*);
	U8_OP_F64(*);

	I16_OP_I8(*);
	I16_OP_U8(*);
	I16_OP_I16(*);
	I16_OP_U16(*);
	I16_OP_I32(*);
	I16_OP_U32(*);
	I16_OP_I64(*);
	I16_OP_U64(*);
	I16_OP_F32(*);
	I16_OP_F64(*);

	U16_OP_I8(*);
	U16_OP_U8(*);
	U16_OP_I16(*);
	U16_OP_U16(*);
	U16_OP_I32(*);
	U16_OP_U32(*);
	U16_OP_I64(*);
	U16_OP_U64(*);
	U16_OP_F32(*);
	U16_OP_F64(*);

	I32_OP_I8(*);
	I32_OP_U8(*);
	I32_OP_I16(*);
	I32_OP_U16(*);
	I32_OP_I32(*);
	I32_OP_U32(*);
	I32_OP_I64(*);
	I32_OP_U64(*);
	I32_OP_F32(*);
	I32_OP_F64(*);

	U32_OP_I8(*);
	U32_OP_U8(*);
	U32_OP_I16(*);
	U32_OP_U16(*);
	U32_OP_I32(*);
	U32_OP_U32(*);
	U32_OP_I64(*);
	U32_OP_U64(*);
	U32_OP_F32(*);
	U32_OP_F64(*);

	I64_OP_I8(*);
	I64_OP_U8(*);
	I64_OP_I16(*);
	I64_OP_U16(*);
	I64_OP_I32(*);
	I64_OP_U32(*);
	I64_OP_I64(*);
	I64_OP_U64(*);
	I64_OP_F32(*);
	I64_OP_F64(*);

	U64_OP_I8(*);
	U64_OP_U8(*);
	U64_OP_I16(*);
	U64_OP_U16(*);
	U64_OP_I32(*);
	U64_OP_U32(*);
	U64_OP_I64(*);
	U64_OP_U64(*);
	U64_OP_F32(*);
	U64_OP_F64(*);

	F32_OP_I8(*);
	F32_OP_U8(*);
	F32_OP_I16(*);
	F32_OP_U16(*);
	F32_OP_I32(*);
	F32_OP_U32(*);
	F32_OP_I64(*);
	F32_OP_U64(*);
	F32_OP_F32(*);
	F32_OP_F64(*);

	F64_OP_I8(*);
	F64_OP_U8(*);
	F64_OP_I16(*);
	F64_OP_U16(*);
	F64_OP_I32(*);
	F64_OP_U32(*);
	F64_OP_I64(*);
	F64_OP_U64(*);
	F64_OP_F32(*);
	F64_OP_F64(*);

	default:
		return false;
	}
	return true;
}

bool o2::primitive_value_div(primitive_value* lhs, const primitive_value* rhs)
{
	switch (primitive_type_combine(lhs->type, rhs->type))
	{
	I8_OP_I8(/);
	I8_OP_U8(/);
	I8_OP_I16(/);
	I8_OP_U16(/);
	I8_OP_I32(/);
	I8_OP_U32(/);
	I8_OP_I64(/);
	I8_OP_U64(/);
	I8_OP_F32(/);
	I8_OP_F64(/);

	U8_OP_I8(/);
	U8_OP_U8(/);
	U8_OP_I16(/);
	U8_OP_U16(/);
	U8_OP_I32(/);
	U8_OP_U32(/);
	U8_OP_I64(/);
	U8_OP_U64(/);
	U8_OP_F32(/);
	U8_OP_F64(/);

	I16_OP_I8(/);
	I16_OP_U8(/);
	I16_OP_I16(/);
	I16_OP_U16(/);
	I16_OP_I32(/);
	I16_OP_U32(/);
	I16_OP_I64(/);
	I16_OP_U64(/);
	I16_OP_F32(/);
	I16_OP_F64(/);

	U16_OP_I8(/);
	U16_OP_U8(/);
	U16_OP_I16(/);
	U16_OP_U16(/);
	U16_OP_I32(/);
	U16_OP_U32(/);
	U16_OP_I64(/);
	U16_OP_U64(/);
	U16_OP_F32(/);
	U16_OP_F64(/);

	I32_OP_I8(/);
	I32_OP_U8(/);
	I32_OP_I16(/);
	I32_OP_U16(/);
	I32_OP_I32(/);
	I32_OP_U32(/);
	I32_OP_I64(/);
	I32_OP_U64(/);
	I32_OP_F32(/);
	I32_OP_F64(/);

	U32_OP_I8(/);
	U32_OP_U8(/);
	U32_OP_I16(/);
	U32_OP_U16(/);
	U32_OP_I32(/);
	U32_OP_U32(/);
	U32_OP_I64(/);
	U32_OP_U64(/);
	U32_OP_F32(/);
	U32_OP_F64(/);

	I64_OP_I8(/);
	I64_OP_U8(/);
	I64_OP_I16(/);
	I64_OP_U16(/);
	I64_OP_I32(/);
	I64_OP_U32(/);
	I64_OP_I64(/);
	I64_OP_U64(/);
	I64_OP_F32(/);
	I64_OP_F64(/);

	U64_OP_I8(/);
	U64_OP_U8(/);
	U64_OP_I16(/);
	U64_OP_U16(/);
	U64_OP_I32(/);
	U64_OP_U32(/);
	U64_OP_I64(/);
	U64_OP_U64(/);
	U64_OP_F32(/);
	U64_OP_F64(/);

	F32_OP_I8(/);
	F32_OP_U8(/);
	F32_OP_I16(/);
	F32_OP_U16(/);
	F32_OP_I32(/);
	F32_OP_U32(/);
	F32_OP_I64(/);
	F32_OP_U64(/);
	F32_OP_F32(/);
	F32_OP_F64(/);

	F64_OP_I8(/);
	F64_OP_U8(/);
	F64_OP_I16(/);
	F64_OP_U16(/);
	F64_OP_I32(/);
	F64_OP_U32(/);
	F64_OP_I64(/);
	F64_OP_U64(/);
	F64_OP_F32(/);
	F64_OP_F64(/);

	default:
		return false;
	}
	return true;
}
