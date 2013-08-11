
inline
Size::Size():
   width(0.0f),
   height(0.0f)
{
}

inline
Size::Size(const Size& that):
   width(that.width),
   height(that.height)
{
}

inline
Size::Size(float w, float h):
   width(w),
   height(h)
{
}

inline
Size& Size::operator=(const Size& that)
{
   width = that.width;
   height = that.height;
   return *this;
}

inline Size Size::operator/(float value)
{
   return Size(width / value, height / value);
}
