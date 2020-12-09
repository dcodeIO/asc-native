#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemblyscript.h"
#include "binaryen-c.h"
#include "wtf.h"

// We are limited to 32-bit currently due to integrating with Binaryen, which
// is assumed to be 32-bit in Wasm like any other Emscripten artifact for now.
typedef s32 isize;
typedef u32 usize;

// WebAssembly imports
static void (*Z_envZ_abortZ_viiii)(u32, u32, u32, u32);
static void (*Z_envZ_markZ_vi)(u32);
static void (*Z_envZ_traceZ_viiddddd)(u32, u32, f64, f64, f64, f64, f64);
void env_abort(usize message, usize filename, usize lineNo, usize columnNo);
void env_mark(u32 cookie);
void env_trace(usize message, u32 nArgs, f64 arg0, f64 arg1, f64 arg2, f64 arg3, f64 arg4);

// WebAssembly exports
#define _start Z__startZ_vv
#define memory Z_memory
#define __new Z___newZ_iii
#define newOptions Z_newOptionsZ_iv
#define setTarget Z_setTargetZ_vii
#define setNoAssert Z_setNoAssertZ_vii
#define initializeProgram Z_initializeProgramZ_vi
#define newProgram Z_newProgramZ_ii
#define compile Z_compileZ_ii

int main(int argc, char** argv) {
  // The loader would typically implement these
  Z_envZ_abortZ_viiii = &env_abort;
  Z_envZ_markZ_vi = &env_mark;
  Z_envZ_traceZ_viiddddd = &env_trace;

  init();
  _start();

  usize options = newOptions();
  setTarget(options, 0);
  setNoAssert(options, 0);
  usize program = newProgram(options);
  // TODO: parse stdlib and entry
  initializeProgram(program);
  usize module = compile(program);
  return 0;
}

// Utility for string reading and allocation, i.e. what the loader would do

#define STRING_ID 1
#define ID_OFFSET -8
#define SIZE_OFFSET -4

u32 load32(u64 addr) {
  u32 result;
  memcpy(&result, &memory->data[addr], sizeof(u32));
  return result;
}

static char* lastGetString = NULL; // keep just the last one alive

const char* __getString(usize ptr) {
  if (!ptr) return "null";
  u32 rtId = load32(ptr + ID_OFFSET);
  if (rtId != STRING_ID) return "<not-a-string>";
  u32 rtSize = load32(ptr + SIZE_OFFSET);
  usize size = utf16_to_utf8((const utf16_t*)&memory->data[ptr], ((usize)rtSize) >> 1, NULL, 0);
  if (lastGetString) free(lastGetString);
  lastGetString = malloc(size + 1);
  utf16_to_utf8((const utf16_t*)&memory->data[ptr], ((usize)rtSize) >> 1, (utf8_t*)lastGetString, size);
  lastGetString[size] = 0;
  return lastGetString;
}

usize __newString(const char* str) {
  if (!str) return 0;
  size_t str_len = strlen(str);
  usize len = utf8_to_utf16((const utf8_t*)str, str_len, NULL, 0);
  usize ptr = __new(len << 1, STRING_ID);
  utf8_to_utf16((const utf8_t*)str, str_len, (utf16_t*)&memory->data[ptr], len);
  return ptr;
}

// WebAssembly import implementations

void env_abort(usize messagePtr, usize filenamePtr, u32 lineNo, u32 columnNo) {
  const char* message = __getString(messagePtr);
  printf("abort: %s in ", message);
  const char* filename = __getString(filenamePtr);
  printf("%s at line %d:%d\n", filename, lineNo, columnNo);
}

void env_mark(u32 cookie) {
}

void env_trace(usize messagePtr, u32 nArgs, f64 arg0, f64 arg1, f64 arg2, f64 arg3, f64 arg4) {
  const char* message = __getString(messagePtr);
  switch (nArgs) {
    case 0: { printf("trace: %s\n", message); break; }
    case 1: { printf("trace: %s %f\n", message, arg0); break; }
    case 2: { printf("trace: %s %f %f\n", message, arg0, arg1); break; }
    case 3: { printf("trace: %s %f %f %f\n", message, arg0, arg1, arg2); break; }
    case 4: { printf("trace: %s %f %f %f %f\n", message, arg0, arg1, arg2, arg3); break; }
    default:
    case 5: { printf("trace: %s %f %f %f %f %f\n", message, arg0, arg1, arg2, arg3, arg4); break; }
  }
}
