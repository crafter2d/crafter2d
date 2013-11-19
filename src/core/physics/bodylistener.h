
#ifndef BODY_LISTENER_H
#define BODY_LISTENER_H

class Body;

class IBodyListener
{
public:
   virtual void onPositionChanged(Body& body) = 0;
};

#endif // BODY_LISTENER_H
