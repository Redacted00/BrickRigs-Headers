// Copyright Fluppisoft, 2023

#pragma once

// Compile time flag for the mod kit
#ifndef BR_BUILD_MODKIT
#define BR_BUILD_MODKIT 0
#endif

// Runtime flag for the mod kit
inline bool GIsModKit = BR_BUILD_MODKIT;

// Compile time flag for building for vanilla UE4
#ifndef BR_BUILD_VANILLA
#define BR_BUILD_VANILLA 0
#endif

// The BR_BUILD_VANILLA_OPTIONAL is used to enclose code elements that should be omitted in vanilla builds
#if BR_BUILD_VANILLA
#define BR_BUILD_VANILLA_OPTIONAL(Param)
#else
#define BR_BUILD_VANILLA_OPTIONAL(Param) Param
#endif
