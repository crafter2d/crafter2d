
#include "blendstatedesc.h"

#include "core/defines.h"
#include "core/string/string.h"

namespace Graphics
{

static const String sOne("One");
static const String sZero("Zero");
static const String sSrcColor("SrcColor");
static const String sSrcInvColor("SrcInvColor");
static const String sSrcAlpha("SrcAlpha");
static const String sSrcInvAlpha("SrcInvAlpha");
static const String sDstColor("DstColor");
static const String sDstInvColor("DstInvColor");
static const String sDstAlpha("DstAlpha");
static const String sDstInvAlpha("DstInvAlpha");

// - Statics

BlendStateDesc::BlendFactor BlendStateDesc::fromString(const String& factor)
{
   if ( factor.compare(sOne) )
   {
      return BlendStateDesc::BS_ONE;
   }
   else if ( factor.compare(sZero) )
   {
      return BlendStateDesc::BS_ZERO;
   }
   else if ( factor.compare(sSrcColor) )
   {
      return BlendStateDesc::BS_SRC_COLOR;
   }
   else if ( factor.compare(sSrcInvColor) )
   {
      return BlendStateDesc::BS_SRC_INV_COLOR;
   }
   else if ( factor.compare(sSrcAlpha) )
   {
      return BlendStateDesc::BS_SRC_ALPHA;
   }
   else if ( factor.compare(sSrcInvAlpha) )
   {
      return BlendStateDesc::BS_SRC_INV_ALPHA;
   }
   else if ( factor.compare(sDstColor) )
   {
      return BlendStateDesc::BS_DST_COLOR;
   }
   else if ( factor.compare(sDstInvColor) )
   {
      return BlendStateDesc::BS_DST_INV_COLOR;
   }
   else if ( factor.compare(sDstAlpha) )
   {
      return BlendStateDesc::BS_DST_ALPHA;
   }
   else if ( factor.compare(sDstInvAlpha) )
   {
      return BlendStateDesc::BS_DST_INV_ALPHA;
   }
   
   return BlendStateDesc::BS_INVALID;
}

// - BlendStateDesc

BlendStateDesc::BlendStateDesc(BlendStateDesc::BlendFactor source, BlendStateDesc::BlendFactor dest, bool enabled):
   mSourceFactor(source),
   mDestFactor(dest),
   mEnabled(enabled)
{
}

// - Get/set

BlendStateDesc::BlendFactor BlendStateDesc::getSourceFactor() const
{
   return mSourceFactor;
}

BlendStateDesc::BlendFactor BlendStateDesc::getDestFactor() const
{
   return mDestFactor;
}

bool BlendStateDesc::isEnabled() const
{
   return mEnabled;
}

};
