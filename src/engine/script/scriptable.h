
#ifndef SCRIPTABLE_H
#define SCRIPTABLE_H

class Script;

/// Interface for objects that should be usable by the script engine
class IScriptable
{
public:

   enum Use { eInstantiatable, eUsable };

   //virtual Use getUse() const = 0;

   virtual Script& getScript() = 0;
};

#endif // SCRIPTABLE_H
