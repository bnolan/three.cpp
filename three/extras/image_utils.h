#ifndef THREE_IMAGE_UTILS_H
#define THREE_IMAGE_UTILS_H

#include <three/math/color.h>

#include <three/textures/texture.h>

namespace three {

class THREE_DECL ImageUtils {
public:

  static Texture::Ptr loadTextureFromMemory(
    unsigned char const *buffer, 
    int length
    //,THREE::Mapping mapping = THREE::UVMapping
  );

  static Texture::Ptr loadTexture(
    const std::string& url
    //,THREE::Mapping mapping = THREE::UVMapping
  );

  static Texture::Ptr loadCompressedTexture(
    const std::string& url,
    THREE::Mapping mapping = THREE::UVMapping
  );

  static Texture::Ptr generateDataTexture(
    int width,
    int height,
    Color color
  );

#ifdef TODO_LOAD_TEXTURE_CUBE

  static Texture::Ptr loadTextureCube(
    std::array<std: string, 6> array,
    THREE::Mapping mapping = THREE::UVMapping
  );

  static Texture::Ptr loadCompressedTextureCube(
    std::array<std: string, 6> array,
    THREE::Mapping mapping = THREE::UVMapping
  );

#endif // TODO_LOAD_TEXTURE_CUBE

}; // ImageUtils

} // namespace three

#endif // THREE_IMAGE_UTILS_H
