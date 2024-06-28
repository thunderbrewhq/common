const std = @import("std");
const system = @import("system");

pub fn build(b: *std.Build) void {
  const target = b.standardTargetOptions(.{});
  const optimize = b.standardOptimizeOption(.{});

  // Common library
  const common = b.addStaticLibrary(.{
    .name = "common",
    .target = target,
    .optimize = optimize
  });
  // Link C++ standard library
  common.linkLibCpp();
  // Add system detection defines
  system.add_defines(common);

  // Get dependencies
  const squall = b.dependency("squall", .{});
  const typhoon = b.dependency("typhoon", .{});
  const libexpat = b.dependency("libexpat", .{});
  // Link squall
  common.addIncludePath(squall.path("."));
  common.linkLibrary(squall.artifact("storm"));
  // Link typhoon
  common.addIncludePath(typhoon.path("."));
  common.linkLibrary(typhoon.artifact("tempest"));
  // Link libexpat
  common.addIncludePath(libexpat.path("expat/lib"));
  common.linkLibrary(libexpat.artifact("expat"));

  // Include common project directory
  common.addIncludePath(b.path("."));

  const common_compiler_flags = [_][]const u8 {
    "-std=c++11"
  };

  const common_sources = [_][]const u8 {
    "common/datamgr/CDataMgr.cpp",

    "common/datastore/CDataStore.cpp",

    "common/mempool/MemChunk.cpp",
    "common/mempool/MemPool.cpp",

    "common/objectalloc/CObjectHeap.cpp",
    "common/objectalloc/CObjectHeapList.cpp",
    "common/objectalloc/ObjAllocGlobals.cpp",

    "common/string/CStringManager.cpp",
    "common/string/CStringMemory.cpp",
    "common/string/RCString.cpp",

    "common/thread/OsTls.cpp",

    "common/xml/XMLNode.cpp",
    "common/xml/XMLTree.cpp",

    "common/BigInteger.cpp",
    "common/Call.cpp",
    "common/DataMgr.cpp",
    "common/Handle.cpp",
    "common/MD5.cpp",
    "common/ObjectAlloc.cpp",
    "common/Prop.cpp",
    "common/SHA1.cpp",
    "common/Time.cpp"
  };

  common.addCSourceFiles(.{
    .files = &common_sources,
    .flags = &common_compiler_flags
  });

  // CommonTest executable
  const common_test_exe = b.addExecutable(.{
    .name = "CommonTest",
    .target = target,
    .optimize = optimize
  });
  // Link C++ standard library
  common_test_exe.linkLibCpp();
  // Add system detection defines
  system.add_defines(common_test_exe);

  // Link common
  common_test_exe.addIncludePath(b.path("."));
  common_test_exe.linkLibrary(common);

  // Link squall
  common_test_exe.addIncludePath(squall.path("."));

  // Link typhoon
  common_test_exe.addIncludePath(typhoon.path("."));

  common_test_exe.addCSourceFiles(.{
    .files = &.{
      "test/Array.cpp",
      "test/BigInteger.cpp",
      "test/DataStore.cpp",
      "test/Handle.cpp",
      "test/MemPool.cpp",
      "test/ObjectAlloc.cpp",
      "test/Prop.cpp",
      "test/SHA1.cpp",
      "test/String.cpp",
      "test/Test.cpp",
      "test/Thread.cpp",
      "test/XML.cpp"
    },

    .flags = &common_compiler_flags
  });

  b.installArtifact(common_test_exe);
  b.installArtifact(common);
}
