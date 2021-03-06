#ifndef THREE_FOG_EXP2_H
#define THREE_FOG_EXP2_H

#include <three/common.h>

#include <three/math/color.h>

#include <three/utils/noncopyable.h>

namespace three {

class FogExp2 : public IFog, NonCopyable {

public:

  typedef std::shared_ptr<FogExp2> Ptr;

  static Ptr create( int hex, float density = 0.00025f ) {

    return three::make_shared<FogExp2>( hex, density );

  }

  virtual THREE::FogType type() const THREE_OVERRIDE { return THREE::FogExp2; }

  virtual IFog::Ptr clone() const THREE_OVERRIDE {

    return create( color.getHex(), density );

  }

  std::string name;

  Color color;
  float density;

protected:

   FogExp2( int hex, float density )
    : IFog(), color( hex ), density( density ) { }

};

} // namespace three

#endif // THREE_FOG_EXP2_H