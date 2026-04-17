#pragma once

#include "CoreMinimal.h"

using FBrickRigsSaveVersion = uint8;

// Current save version, the legacy UBrickStatics version ended at 6
const static FBrickRigsSaveVersion BR_SAVE_VERSION = 15;
// Version where the brick units and brick size types were removed
const static FBrickRigsSaveVersion BR_SAVE_BRICK_UNITS_REMOVED_VERSION = 15;
// Version where the driver seat stopped being serialized
const static FBrickRigsSaveVersion BR_SAVE_DRIVER_SEAT_BY_INDEX_VERSION = 15;
// Version where color was saved as RGB again instead of HSV
const static FBrickRigsSaveVersion BR_SAVE_COLOR_RGB_VERSION = 15;
// Version where brick units started to be saved as a float instead of uint16
const static FBrickRigsSaveVersion BR_SAVE_BRICK_UNITS_FLOAT_VERSION = 14;
// Version where wheel meshes were moved so their connectors would line up with the origin
const static FBrickRigsSaveVersion BR_SAVE_ALIGNED_WHEEL_ORIGIN_VERSION = 14;
// Prior to this version GetNameStringByIndex was used instead of GetNameStringByIndex in FEnumBrickPropertyBase::SerializeProperty
const static FBrickRigsSaveVersion BR_SAVE_FIXED_ENUM_SERIALIZATION_VERSION = 13;
// Version where input channel indices where replaced by brick references
const static FBrickRigsSaveVersion BR_SAVE_INPUT_CHANNEL_REFERENCE_VERSION = 13;
// Version where item mass and price started to be saved to the meta data
const static FBrickRigsSaveVersion BR_SAVE_MASS_AND_PRICE = 12;
// Version where min and max sensor input values were replaced by universal input channel parameters
const static FBrickRigsSaveVersion BR_SAVE_INPUT_CHANNEL_CURVE_VERSION = 11;
// Version where the min and max actuation limit properties have been inverted
const static FBrickRigsSaveVersion BR_SAVE_ACTUATOR_LIMITS_VERSION = 10;
// Version where suspension damping was made player controllable
const static FBrickRigsSaveVersion BR_SAVE_SUSPENSION_DAMPING_VERSION = 9;
// Version where the tags were added to the meta data
const static FBrickRigsSaveVersion BR_SAVE_TAGS_VERSION = 8;
// Version where we transition from the UBrickStatics save functions to the UBrickEditorSaveInterface
const static FBrickRigsSaveVersion BR_SAVE_INTERFACE_VERSION = 7;
// Version before the save interface was implemented
const static FBrickRigsSaveVersion BR_SAVE_LAST_LEGACY_VERSION = 6;
// Version where input channels were introduced and the input axes enum has been changed
const static FBrickRigsSaveVersion BR_SAVE_INPUT_CHANNEL_VERSION = 6;
// Legacy version where the brick location accuracy was increased
const static FBrickRigsSaveVersion BR_SAVE_SMALLER_STEPS_VERSION = 4;
// Version where the saved element size has been fixed
const static FBrickRigsSaveVersion BR_SAVE_FIXED_ELEMENT_SIZE_VERSION = 3;
