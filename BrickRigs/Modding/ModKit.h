// Copyright Fluppisoft, 2023

#pragma once

// Compile time flag for the mod kit
#ifndef BR_BUILD_MODKIT
#define BR_BUILD_MODKIT 0
#endif

// Runtime flag for the mod kit
inline bool GIsModKit = BR_BUILD_MODKIT;
