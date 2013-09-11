
#ifndef JOINT_DEFINITION_H
#define JOINT_DEFINITION_H

class JointDefinition
{
public:
   enum Kind
   {
      eRevolute,
   };

 // get/set
   Kind getKind() const;

protected:
   JointDefinition(Kind kind);

private:
   JointDefinition();

   Kind  mKind;
};

#endif // JOINT_DEFINITION_H
