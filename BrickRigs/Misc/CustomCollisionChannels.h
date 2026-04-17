#pragma once

#define ECC_Projectile ECC_GameTraceChannel1
#define ECC_Brick ECC_GameTraceChannel2
#define ECC_Fire ECC_GameTraceChannel3
#define ECC_CosmeticPhysicsBody ECC_GameTraceChannel4
#define ECC_Volume ECC_GameTraceChannel5
#define ECC_Item ECC_GameTraceChannel6
#define ECC_Explosion ECC_GameTraceChannel7
#define ECC_DetachedBrick ECC_GameTraceChannel8
#define ECC_Train ECC_GameTraceChannel9
#define ECC_Water ECC_GameTraceChannel10
#define ECC_HitableProjectile ECC_GameTraceChannel11

// Surface Types
#define SurfaceType_Metal SurfaceType1
#define SurfaceType_Plastic SurfaceType2
#define SurfaceType_Stone SurfaceType3
#define SurfaceType_Grass SurfaceType4
#define SurfaceType_Soil SurfaceType5
#define SurfaceType_Sand SurfaceType6
#define SurfaceType_Water SurfaceType7
#define SurfaceType_Snow SurfaceType8

// Collision dominance groups
// NOTE: Only 32 of these can exist
#define CDG_Default 0
#define CDG_Brick 1
#define CDG_Ragdoll 2
#define CDG_CosmeticPhysicsBody 3
#define CDG_Item 4
