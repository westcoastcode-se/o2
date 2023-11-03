//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "primitive_value_compare.h"
using namespace o2;

static constexpr int32_t primitive_type_compare_combine(primitive_type t1, primitive_type t2)
{
	return ((int)t1 << (int)primitive_type::count) | (int)t2;
}

#define I8_OP_I8(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::int8): \
    lhs->bool_ = lhs->i8 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_U8(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::uint8): \
    lhs->bool_ = lhs->i8 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_I16(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::int16): \
    lhs->bool_ = lhs->i8 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_U16(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::uint16): \
    lhs->bool_ = lhs->i8 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_I32(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::int32): \
    lhs->bool_ = lhs->i8 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_U32(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::uint32): \
    lhs->bool_ = lhs->i8 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_I64(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::int64): \
    lhs->bool_ = lhs->i8 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_U64(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::uint64): \
    lhs->bool_ = lhs->i8 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_F32(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::float32): \
    lhs->bool_ = lhs->i8 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define I8_OP_F64(op) case primitive_type_compare_combine(primitive_type::int8, primitive_type::float64): \
    lhs->bool_ = lhs->i8 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define U8_OP_I8(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::int8): \
    lhs->bool_ = lhs->u8 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_U8(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::uint8): \
    lhs->bool_ = lhs->u8 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_I16(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::int16): \
    lhs->bool_ = lhs->u8 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_U16(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::uint16): \
    lhs->bool_ = lhs->u8 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_I32(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::int32): \
    lhs->bool_ = lhs->u8 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_U32(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::uint32): \
    lhs->bool_ = lhs->u8 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_I64(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::int64): \
    lhs->bool_ = lhs->u8 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_U64(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::uint64): \
    lhs->bool_ = lhs->u8 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_F32(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::float32): \
    lhs->bool_ = lhs->u8 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define U8_OP_F64(op) case primitive_type_compare_combine(primitive_type::uint8, primitive_type::float64): \
    lhs->bool_ = lhs->u8 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define I16_OP_I8(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::int8): \
    lhs->bool_ = lhs->i16 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_U8(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::uint8): \
    lhs->bool_ = lhs->i16 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_I16(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::int16): \
    lhs->bool_ = lhs->i16 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_U16(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::uint16): \
    lhs->bool_ = lhs->i16 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_I32(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::int32): \
    lhs->bool_ = lhs->i16 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_U32(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::uint32): \
    lhs->bool_ = lhs->i16 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_I64(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::int64): \
    lhs->bool_ = lhs->i16 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_U64(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::uint64): \
    lhs->bool_ = lhs->i16 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_F32(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::float32): \
    lhs->bool_ = lhs->i16 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define I16_OP_F64(op) case primitive_type_compare_combine(primitive_type::int16, primitive_type::float64): \
    lhs->bool_ = lhs->i16 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define U16_OP_I8(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::int8): \
    lhs->bool_ = lhs->u16 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_U8(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::uint8): \
    lhs->bool_ = lhs->u16 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_I16(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::int16): \
    lhs->bool_ = lhs->u16 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_U16(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::uint16): \
    lhs->bool_ = lhs->u16 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_I32(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::int32): \
    lhs->bool_ = lhs->u16 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_U32(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::uint32): \
    lhs->bool_ = lhs->u16 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_I64(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::int64): \
    lhs->bool_ = lhs->u16 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_U64(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::uint64): \
    lhs->bool_ = lhs->u16 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_F32(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::float32): \
    lhs->bool_ = lhs->u16 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define U16_OP_F64(op) case primitive_type_compare_combine(primitive_type::uint16, primitive_type::float64): \
    lhs->bool_ = lhs->u16 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define I32_OP_I8(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::int8): \
    lhs->bool_ = lhs->i32 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_U8(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::uint8): \
    lhs->bool_ = lhs->i32 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_I16(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::int16): \
    lhs->bool_ = lhs->i32 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_U16(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::uint16): \
    lhs->bool_ = lhs->i32 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_I32(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::int32): \
    lhs->bool_ = lhs->i32 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_U32(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::uint32): \
    lhs->bool_ = lhs->i32 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_I64(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::int64): \
    lhs->bool_ = lhs->i32 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_U64(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::uint64): \
    lhs->bool_ = lhs->i32 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_F32(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::float32): \
    lhs->bool_ = lhs->i32 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define I32_OP_F64(op) case primitive_type_compare_combine(primitive_type::int32, primitive_type::float64): \
    lhs->bool_ = lhs->i32 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define U32_OP_I8(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::int8): \
    lhs->bool_ = lhs->u32 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_U8(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::uint8): \
    lhs->bool_ = lhs->u32 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_I16(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::int16): \
    lhs->bool_ = lhs->u32 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_U16(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::uint16): \
    lhs->bool_ = lhs->u32 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_I32(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::int32): \
    lhs->bool_ = lhs->u32 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_U32(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::uint32): \
    lhs->bool_ = lhs->u32 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_I64(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::int64): \
    lhs->bool_ = lhs->u32 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_U64(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::uint64): \
    lhs->bool_ = lhs->u32 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_F32(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::float32): \
    lhs->bool_ = lhs->u32 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define U32_OP_F64(op) case primitive_type_compare_combine(primitive_type::uint32, primitive_type::float64): \
    lhs->bool_ = lhs->u32 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define I64_OP_I8(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::int8): \
    lhs->bool_ = lhs->i64 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_U8(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::uint8): \
    lhs->bool_ = lhs->i64 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_I16(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::int16): \
    lhs->bool_ = lhs->i64 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_U16(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::uint16): \
    lhs->bool_ = lhs->i64 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_I32(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::int32): \
    lhs->bool_ = lhs->i64 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_U32(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::uint32): \
    lhs->bool_ = lhs->i64 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_I64(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::int64): \
    lhs->bool_ = lhs->i64 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_U64(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::uint64): \
    lhs->bool_ = lhs->i64 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_F32(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::float32): \
    lhs->bool_ = lhs->i64 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define I64_OP_F64(op) case primitive_type_compare_combine(primitive_type::int64, primitive_type::float64): \
    lhs->bool_ = lhs->i64 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define U64_OP_I8(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::int8): \
    lhs->bool_ = lhs->u64 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_U8(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::uint8): \
    lhs->bool_ = lhs->u64 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_I16(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::int16): \
    lhs->bool_ = lhs->u64 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_U16(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::uint16): \
    lhs->bool_ = lhs->u64 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_I32(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::int32): \
    lhs->bool_ = lhs->u64 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_U32(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::uint32): \
    lhs->bool_ = lhs->u64 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_I64(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::int64): \
    lhs->bool_ = lhs->u64 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_U64(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::uint64): \
    lhs->bool_ = lhs->u64 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_F32(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::float32): \
    lhs->bool_ = lhs->u64 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define U64_OP_F64(op) case primitive_type_compare_combine(primitive_type::uint64, primitive_type::float64): \
    lhs->bool_ = lhs->u64 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define F32_OP_I8(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::int8): \
    lhs->bool_ = lhs->f32 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_U8(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::uint8): \
    lhs->bool_ = lhs->f32 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_I16(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::int16): \
    lhs->bool_ = lhs->f32 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_U16(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::uint16): \
    lhs->bool_ = lhs->f32 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_I32(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::int32): \
    lhs->bool_ = lhs->f32 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_U32(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::uint32): \
    lhs->bool_ = lhs->f32 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_I64(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::int64): \
    lhs->bool_ = lhs->f32 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_U64(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::uint64): \
    lhs->bool_ = lhs->f32 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_F32(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::float32): \
    lhs->bool_ = lhs->f32 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define F32_OP_F64(op) case primitive_type_compare_combine(primitive_type::float32, primitive_type::float64): \
    lhs->bool_ = lhs->f32 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define F64_OP_I8(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::int8): \
    lhs->bool_ = lhs->f64 op rhs->i8; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_U8(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::uint8): \
    lhs->bool_ = lhs->f64 op rhs->u8; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_I16(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::int16): \
    lhs->bool_ = lhs->f64 op rhs->i16; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_U16(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::uint16): \
    lhs->bool_ = lhs->f64 op rhs->u16; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_I32(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::int32): \
    lhs->bool_ = lhs->f64 op rhs->i32; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_U32(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::uint32): \
    lhs->bool_ = lhs->f64 op rhs->u32; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_I64(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::int64): \
    lhs->bool_ = lhs->f64 op rhs->i64; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_U64(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::uint64): \
    lhs->bool_ = lhs->f64 op rhs->u64; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_F32(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::float32): \
    lhs->bool_ = lhs->f64 op rhs->f32; \
    lhs->type = primitive_type::bool_; \
    break
#define F64_OP_F64(op) case primitive_type_compare_combine(primitive_type::float64, primitive_type::float64): \
    lhs->bool_ = lhs->f64 op rhs->f64; \
    lhs->type = primitive_type::bool_; \
    break

#define PTR_OP_PTR(op) case primitive_type_compare_combine(primitive_type::ptr, primitive_type::ptr): \
    lhs->bool_ = lhs->ptr op rhs->ptr; \
    lhs->type = primitive_type::bool_; \
    break


bool o2::primitive_value_less_then(o2::primitive_value* const lhs,
		const o2::primitive_value* const rhs)
{
	switch (primitive_type_compare_combine(lhs->type, rhs->type))
	{
	I8_OP_I8(<);
	I8_OP_U8(<);
	I8_OP_I16(<);
	I8_OP_U16(<);
	I8_OP_I32(<);
	I8_OP_U32(<);
	I8_OP_I64(<);
	I8_OP_U64(<);
	I8_OP_F32(<);
	I8_OP_F64(<);

	U8_OP_I8(<);
	U8_OP_U8(<);
	U8_OP_I16(<);
	U8_OP_U16(<);
	U8_OP_I32(<);
	U8_OP_U32(<);
	U8_OP_I64(<);
	U8_OP_U64(<);
	U8_OP_F32(<);
	U8_OP_F64(<);

	I16_OP_I8(<);
	I16_OP_U8(<);
	I16_OP_I16(<);
	I16_OP_U16(<);
	I16_OP_I32(<);
	I16_OP_U32(<);
	I16_OP_I64(<);
	I16_OP_U64(<);
	I16_OP_F32(<);
	I16_OP_F64(<);

	U16_OP_I8(<);
	U16_OP_U8(<);
	U16_OP_I16(<);
	U16_OP_U16(<);
	U16_OP_I32(<);
	U16_OP_U32(<);
	U16_OP_I64(<);
	U16_OP_U64(<);
	U16_OP_F32(<);
	U16_OP_F64(<);

	I32_OP_I8(<);
	I32_OP_U8(<);
	I32_OP_I16(<);
	I32_OP_U16(<);
	I32_OP_I32(<);
	I32_OP_U32(<);
	I32_OP_I64(<);
	I32_OP_U64(<);
	I32_OP_F32(<);
	I32_OP_F64(<);

	U32_OP_I8(<);
	U32_OP_U8(<);
	U32_OP_I16(<);
	U32_OP_U16(<);
	U32_OP_I32(<);
	U32_OP_U32(<);
	U32_OP_I64(<);
	U32_OP_U64(<);
	U32_OP_F32(<);
	U32_OP_F64(<);

	I64_OP_I8(<);
	I64_OP_U8(<);
	I64_OP_I16(<);
	I64_OP_U16(<);
	I64_OP_I32(<);
	I64_OP_U32(<);
	I64_OP_I64(<);
	I64_OP_U64(<);
	I64_OP_F32(<);
	I64_OP_F64(<);

	U64_OP_I8(<);
	U64_OP_U8(<);
	U64_OP_I16(<);
	U64_OP_U16(<);
	U64_OP_I32(<);
	U64_OP_U32(<);
	U64_OP_I64(<);
	U64_OP_U64(<);
	U64_OP_F32(<);
	U64_OP_F64(<);

	F32_OP_I8(<);
	F32_OP_U8(<);
	F32_OP_I16(<);
	F32_OP_U16(<);
	F32_OP_I32(<);
	F32_OP_U32(<);
	F32_OP_I64(<);
	F32_OP_U64(<);
	F32_OP_F32(<);
	F32_OP_F64(<);

	F64_OP_I8(<);
	F64_OP_U8(<);
	F64_OP_I16(<);
	F64_OP_U16(<);
	F64_OP_I32(<);
	F64_OP_U32(<);
	F64_OP_I64(<);
	F64_OP_U64(<);
	F64_OP_F32(<);
	F64_OP_F64(<);

	PTR_OP_PTR(<);

	default:
		return false;
	}
	return true;
}

bool o2::primitive_value_less_equals(primitive_value* lhs, const primitive_value* rhs)
{
	switch (primitive_type_compare_combine(lhs->type, rhs->type))
	{
	I8_OP_I8(<=);
	I8_OP_U8(<=);
	I8_OP_I16(<=);
	I8_OP_U16(<=);
	I8_OP_I32(<=);
	I8_OP_U32(<=);
	I8_OP_I64(<=);
	I8_OP_U64(<=);
	I8_OP_F32(<=);
	I8_OP_F64(<=);

	U8_OP_I8(<=);
	U8_OP_U8(<=);
	U8_OP_I16(<=);
	U8_OP_U16(<=);
	U8_OP_I32(<=);
	U8_OP_U32(<=);
	U8_OP_I64(<=);
	U8_OP_U64(<=);
	U8_OP_F32(<=);
	U8_OP_F64(<=);

	I16_OP_I8(<=);
	I16_OP_U8(<=);
	I16_OP_I16(<=);
	I16_OP_U16(<=);
	I16_OP_I32(<=);
	I16_OP_U32(<=);
	I16_OP_I64(<=);
	I16_OP_U64(<=);
	I16_OP_F32(<=);
	I16_OP_F64(<=);

	U16_OP_I8(<=);
	U16_OP_U8(<=);
	U16_OP_I16(<=);
	U16_OP_U16(<=);
	U16_OP_I32(<=);
	U16_OP_U32(<=);
	U16_OP_I64(<=);
	U16_OP_U64(<=);
	U16_OP_F32(<=);
	U16_OP_F64(<=);

	I32_OP_I8(<=);
	I32_OP_U8(<=);
	I32_OP_I16(<=);
	I32_OP_U16(<=);
	I32_OP_I32(<=);
	I32_OP_U32(<=);
	I32_OP_I64(<=);
	I32_OP_U64(<=);
	I32_OP_F32(<=);
	I32_OP_F64(<=);

	U32_OP_I8(<=);
	U32_OP_U8(<=);
	U32_OP_I16(<=);
	U32_OP_U16(<=);
	U32_OP_I32(<=);
	U32_OP_U32(<=);
	U32_OP_I64(<=);
	U32_OP_U64(<=);
	U32_OP_F32(<=);
	U32_OP_F64(<=);

	I64_OP_I8(<=);
	I64_OP_U8(<=);
	I64_OP_I16(<=);
	I64_OP_U16(<=);
	I64_OP_I32(<=);
	I64_OP_U32(<=);
	I64_OP_I64(<=);
	I64_OP_U64(<=);
	I64_OP_F32(<=);
	I64_OP_F64(<=);

	U64_OP_I8(<=);
	U64_OP_U8(<=);
	U64_OP_I16(<=);
	U64_OP_U16(<=);
	U64_OP_I32(<=);
	U64_OP_U32(<=);
	U64_OP_I64(<=);
	U64_OP_U64(<=);
	U64_OP_F32(<=);
	U64_OP_F64(<=);

	F32_OP_I8(<=);
	F32_OP_U8(<=);
	F32_OP_I16(<=);
	F32_OP_U16(<=);
	F32_OP_I32(<=);
	F32_OP_U32(<=);
	F32_OP_I64(<=);
	F32_OP_U64(<=);
	F32_OP_F32(<=);
	F32_OP_F64(<=);

	F64_OP_I8(<=);
	F64_OP_U8(<=);
	F64_OP_I16(<=);
	F64_OP_U16(<=);
	F64_OP_I32(<=);
	F64_OP_U32(<=);
	F64_OP_I64(<=);
	F64_OP_U64(<=);
	F64_OP_F32(<=);
	F64_OP_F64(<=);

	PTR_OP_PTR(<=);

	default:
		return false;
	}
	return true;
}
