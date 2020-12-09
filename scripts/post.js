const fs = require("fs");

const prelude = '/* Post-processed with scripts/post.js */\n#include "glue.h"\n\n';
const cabify = [
  "BinaryenConst",
  "BinaryenLiteralInt64",
  "BinaryenLiteralInt32",
  "BinaryenLiteralFloat32",
  "BinaryenLiteralFloat64",
  "BinaryenLiteralVec128",
  "BinaryenModuleAllocateAndWrite"
];

const src = fs.readFileSync(__dirname + "/../src/assemblyscript.c", { encoding: "utf8" });
const dst = prelude + src.replace(/\(\*Z_binaryenZ__(\w+)Z_\w+\)/g,
  ($0, $1) =>
    cabify.includes($1)
      ? $1 + "_cabi"
      : $1
);

fs.writeFileSync(__dirname + "/../src/assemblyscript.post.c", dst);
