
#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H

#include <vector>

#include <libcil/cil/cil.h>
#include <libcil/cil/guards.h>
#include <libcil/cil/instructions.h>
#include <libcil/cil/switchtables.h>

class ASTFunction;
class String;

class FunctionBuilder
{
public:
   FunctionBuilder();

 // get/set
   const CIL::Instructions& getInstructions() const;
   const CIL::Guards&       getGuards() const;
   const CIL::SwitchTables& getSwitchTables() const;
   
 // label operations
   int  allocateLabel();
   void addLabel(int id);
   void addGuard(CIL::Guard* pguard);
   int  addTable(CIL::SwitchTable* ptable);

 // emitting
   void emit(CIL::Opcode opcode);
   void emit(CIL::Opcode opcode, int arg);
   void emit(CIL::Opcode opcode, float arg);
   void emit(CIL::Opcode opcode, const String& arg);

 // building
   void start();
   void end();

private:

   typedef std::vector<int> Labels;

 // operations
   void createJumps();
   void createGuards();
   void createSwitches();

   int  findLabel(int label) const;
   int  getLocation(int label);

   CIL::Instructions mInstructions;
   CIL::Guards       mGuards;
   CIL::SwitchTables mSwitches;
   Labels            mLabels;
   int               mLabel;
};

#endif // FUNCTION_BUILDER_H
