#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "binaryen-c.h"

typedef struct BinaryenLiteral BinaryenLiteral;
typedef struct BinaryenModuleAllocateAndWriteResult BinaryenModuleAllocateAndWriteResult;

// Polyfill what's only present in an Emscripten build of Binaryen:

size_t BinaryenSizeofLiteral(void) { return sizeof(BinaryenLiteral); }

size_t BinaryenSizeofAllocateAndWriteResult(void) {
  return sizeof(BinaryenModuleAllocateAndWriteResult);
}

void _i32_store8(int8_t* ptr, int8_t value) { *ptr = value; }

void _i32_store16(int16_t* ptr, int16_t value) { *ptr = value; }

void _i32_store(int32_t* ptr, int32_t value) { *ptr = value; }

void _f32_store(float* ptr, float value) { *ptr = value; }

void _f64_store(double* ptr, double value) { *ptr = value; }

int8_t _i32_load8_s(int8_t* ptr) { return *ptr; }

uint8_t _i32_load8_u(uint8_t* ptr) { return *ptr; }

int16_t _i32_load16_s(int16_t* ptr) { return *ptr; }

uint16_t _i32_load16_u(uint16_t* ptr) { return *ptr; }

int32_t _i32_load(int32_t* ptr) { return *ptr; }

float _f32_load(float* ptr) { return *ptr; }

double _f64_load(double* ptr) { return *ptr; }

// Polyfill C ABI in Wasm

BinaryenExpressionRef BinaryenConst_cabi(BinaryenModuleRef module, BinaryenLiteral* value) {
  return BinaryenConst(module, *value);
}

void BinaryenLiteralInt32_cabi(BinaryenLiteral* out, int32_t x) {
  BinaryenLiteral lit = BinaryenLiteralInt32(x);
  memcpy(out, &lit, sizeof(BinaryenLiteral));
}

void BinaryenLiteralInt64_cabi(BinaryenLiteral* out, uint32_t lo, uint32_t hi) {
  BinaryenLiteral lit = BinaryenLiteralInt64(((uint64_t)lo) | ((uint64_t)hi) << 32);
  memcpy(out, &lit, sizeof(BinaryenLiteral));
}

void BinaryenLiteralFloat32_cabi(BinaryenLiteral* out, float x) {
  BinaryenLiteral lit = BinaryenLiteralFloat32(x);
  memcpy(out, &lit, sizeof(BinaryenLiteral));
}

void BinaryenLiteralFloat64_cabi(BinaryenLiteral* out, double x) {
  BinaryenLiteral lit = BinaryenLiteralFloat64(x);
  memcpy(out, &lit, sizeof(BinaryenLiteral));
}

void BinaryenLiteralVec128_cabi(BinaryenLiteral* out, const uint8_t x[16]) {
  BinaryenLiteral lit = BinaryenLiteralVec128(x);
  memcpy(out, &lit, sizeof(BinaryenLiteral));
}

void BinaryenModuleAllocateAndWrite_cabi(BinaryenModuleAllocateAndWriteResult* out, BinaryenModuleRef module, const char* sourceMapUrl) {
  BinaryenModuleAllocateAndWriteResult res = BinaryenModuleAllocateAndWrite(module, sourceMapUrl);
  memcpy(out, &res, sizeof(BinaryenModuleAllocateAndWriteResult));
}
