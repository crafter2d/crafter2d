
#ifndef ANTLR_TEXT_POSITION_H
#define ANTLR_TEXT_POSITION_H

class AntlrTextPosition
{
public:
   AntlrTextPosition(): mLine(-1), mPos(-1) {}
   AntlrTextPosition(int line, int pos): mLine(line), mPos(pos) {}
   AntlrTextPosition(const AntlrTextPosition& that): mLine(that.mLine), mPos(that.mPos) {}

   const AntlrTextPosition& operator=(const AntlrTextPosition& that) {
      mLine = that.mLine;
      mPos = that.mPos;
      return *this;
   }

 // get/set
   int getLine() const { return mLine; }
   int getPos() const { return mPos; }

private:

 // data
   int mLine;
   int mPos;
};

#endif // ANTLR_TEXT_POSITION_H
