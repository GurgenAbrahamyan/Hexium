#pragma once
enum class TextureType {
    Albedo,        // sRGB
    Normal,        // linear
    Metallic,      // linear
    Roughness,     // linear
    AO,            // linear
    ORM,           // linear packed
    Emissive,      // usually linear
    Height,        // linear
    Mask,          // linear
    Unknown
};
