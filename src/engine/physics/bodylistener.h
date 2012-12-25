
#ifndef BODY_LISTENER_H
#define BODY_LISTENER_H

class IBodyListener
{
public:
   virtual void onPositionChanged(Body& body);
};

#endif // BODY_LISTENER_H
