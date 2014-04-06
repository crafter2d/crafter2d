
#include "codestreamer.h"

#include "core/streams/datastream.h"

#include "../common/type.h"

#include "class.h"
#include "field.h"
#include "function.h"
#include "guard.h"
#include "switchtabel.h"
#include "switchtableentry.h"

namespace CIL
{
   CodeStreamer::CodeStreamer(DataStream& stream) :
      mStream(stream)
   {
      
   }

   // - Operations

   CodeStreamer& CodeStreamer::operator<<(const Class& klass)
   {
      mStream << klass.getName() << klass.getBaseName();

      // write the interfaces
      const Class::Interfaces& interfaces = klass.getInterfaces();
      mStream.writeUint32(interfaces.size());
      for ( std::size_t index = 0; index < interfaces.size(); ++index )
      {
         mStream << interfaces[index];
      }

      // write the static members
      const Class::Fields& statics = klass.getStatics();
      mStream.writeUint32(statics.size());
      for ( std::size_t index = 0; index < statics.size(); ++index )
      {
         const Field& field = *statics[index];
         writeField(field);
      }

      // write the members
      const Class::Fields& fields = klass.getFields();
      mStream.writeUint32(fields.size());
      for ( std::size_t index = 0; index < fields.size(); ++index )
      {
         const Field& field = *fields[index];
         writeField(field);
      }

      // write the functions
      const Class::Functions& functions = klass.getFunctions();
      mStream.writeUint32(functions.size());
      for ( std::size_t index = 0; index < functions.size(); ++index )
      {
         const Function& function = *functions[index];
         writeFunction(function);
      }
      
      return *this;
   }

   void CodeStreamer::writeField(const Field& field)
   {
      mStream << field.getName();
      writeType(field.getType());
   }

   void CodeStreamer::writeFunction(const Function& function)
   {
      mStream << function.getName() << function.getModifiers().toInt();

      // annotations
      // arguments
      // local variables
      // return type
      // guards
      // switch tables
      // instructions

      const StringList& annotations = function.getAnnotations();
      mStream << annotations.size();
      if ( annotations.size() > 0 )
      {
         ListConstIterator<String> it = annotations.getFront();
         for ( ; it.isValid(); ++it )
         {
            const String& value = *it;
            mStream << value;
         }
      }

      const yasc::Types& args = function.getArguments();
      mStream << args.size();
      for ( int index = 0; index < args.size(); ++index )
      {
         const yasc::Type& type = args[index];
         writeType(type);
      }

      const yasc::Types& locals = function.getLocals();
      mStream << locals.size();
      for ( int index = 0; index < locals.size(); ++index )
      {
         const yasc::Type& type = locals[index];
         writeType(type);
      }

      writeType(function.getReturnType());

      const Guards& guards = function.getGuards();
      mStream << guards.size();
      for ( int index = 0; index < guards.size(); ++index )
      {
         const Guard& guard = guards[index];
         writeGuard(guard);
      }

      const SwitchTables& tables = function.getSwitchTables();
      mStream << tables.size();
      for ( int index = 0; index < tables.size(); ++index )
      {
         const SwitchTable& table = tables[index];
         writeTable(table);
      }

      const Instructions& instructions = function.getInstructions();
      mStream.writeUint32(instructions.size());
      for ( std::size_t index = 0; index < instructions.size(); ++index )
      {
         const Instruction& inst = instructions[index];
         writeInstruction(inst);
      }
   }

   void CodeStreamer::writeType(const yasc::Type& type)
   {
      mStream << type.toString();
   }

   void CodeStreamer::writeGuard(const Guard& guard)
   {
      mStream << guard.finalize 
              << guard.labels[0]
              << guard.labels[1]
              << guard.labels[2]
              << guard.labels[3];
   }

   void CodeStreamer::writeTable(const SwitchTable& table)
   {
      mStream << table.getDefault() << table.getEnd() << table.size();

      for ( int index = 0; index < table.size(); ++index )
      {
         const SwitchTableEntry& entry = table[index];
         mStream << entry.label << entry.value;
      }
   }

   void CodeStreamer::writeInstruction(const Instruction& inst)
   {
      mStream << inst.opcode;

      int opc_type = opcodeinfo[inst.opcode].type;
      switch ( opc_type )
      {
         case T_INT:
            mStream << inst.mInt;
            break;
         case T_REAL:
            mStream << inst.mReal;
            break;
         case T_STR:
            mStream << *inst.mString;
            break;
         case T_NONE:
         default:
            break;
      }
   }
   
   // - Reading

   CodeStreamer& CodeStreamer::operator>>(Class& klass)
   {
      String name, basename;
      uint32_t count;

      mStream >> name >> basename;

      klass.setName(name);
      klass.setBaseName(basename);

      // read implemented interfaces
      mStream.readUint32(count);
      for ( uint32_t index = 0; index < count; ++index )
      {
         String intrface;
         mStream.readString(intrface);
         klass.addInterface(intrface);
      }

      // read static members
      mStream.readUint32(count);
      for ( uint32_t index = 0; index < count; ++index )
      {
         Field* pfield = readField();
         klass.addStatic(pfield);
      }

      // read members
      mStream.readUint32(count);
      for ( uint32_t index = 0; index < count; ++index )
      {
         Field* pfield = readField();
         klass.addField(pfield);
      }

      // read the functions
      mStream.readUint32(count);
      for ( uint32_t index = 0; index < count; ++index )
      {
         Function* pfunction = readFunction(name);
         klass.add(pfunction);
      }

      return *this;
   }

   Field* CodeStreamer::readField()
   {
      String name;
      mStream >> name;

      Field* pfield = new Field();
      pfield->setName(name);
      pfield->setType(readType());

      return pfield;
   }

   Function* CodeStreamer::readFunction(const String& klass)
   {
      Function* pfunction = new Function();

      String name;
      int modifiers;
      mStream >> name >> modifiers;
      pfunction->setName(name);
      pfunction->setModifiers(Modifiers(modifiers));

      if ( !pfunction->getModifiers().isStatic() )
      {
         // add this argument as first function argument
         yasc::Type* ptype = yasc::Type::fromString(klass);
         pfunction->addArgument(ptype);
      }

      // annotations
      // arguments
      // local variables
      // return type
      // guards
      // switch tables
      // instructions

      int argc;
      mStream >> argc;
      for ( int index = 0; index < argc; ++index )
      {
         String value;
         mStream >> value;
         pfunction->addAnnotation(value);
      }

      mStream >> argc;
      for ( int index = 0; index < argc; ++index )
      {
         yasc::Type* ptype = readType();
         pfunction->addArgument(ptype);
      }

      mStream >> argc;
      for ( int index = 0; index < argc; ++index )
      {
         yasc::Type* ptype = readType();
         pfunction->addLocal(ptype);
      }

      yasc::Type* prettype = readType();
      pfunction->setReturnType(prettype);

      int guardc;
      mStream >> guardc;
      for ( int index = 0; index < guardc; ++index )
      {
         Guard* pguard = readGuard();
         pfunction->addGuard(pguard);
      }

      int tablec;
      mStream >> tablec;
      for ( int index = 0; index < tablec; ++index )
      {
         SwitchTable* ptable = readTable();
         pfunction->addSwitchTable(ptable);
      }

      uint32_t instc;
      mStream.readUint32(instc);
      Instructions insts;
      for ( std::size_t index = 0; index < instc; ++index )
      {
         Instruction inst = readInstruction();
         insts.add(inst);
      }
      pfunction->setInstructions(insts);

      return pfunction;
   }

   yasc::Type* CodeStreamer::readType()
   {
      String type;
      mStream >> type;
      return yasc::Type::fromString(type);
   }

   Guard* CodeStreamer::readGuard()
   {
      Guard* pguard = new Guard();

      mStream >> pguard->finalize 
              >> pguard->labels[0]
              >> pguard->labels[1]
              >> pguard->labels[2]
              >> pguard->labels[3];

      return pguard;
   }

   SwitchTable* CodeStreamer::readTable()
   {
      SwitchTable* ptable = new SwitchTable();

      int defaultlabel, endlabel, size;
      mStream >> defaultlabel >> endlabel >> size;
      ptable->addDefault(defaultlabel);
      ptable->addEnd(endlabel);

      SwitchTableEntry entry;
      for ( int index = 0; index < size; ++index )
      {
         mStream >> entry.label >> entry.value;
         ptable->add(entry);
      }

      return ptable;
   }

   Instruction CodeStreamer::readInstruction()
   {
      Instruction inst;

      mStream >> (int&)inst.opcode;

      int opc_type = opcodeinfo[inst.opcode].type;
      switch ( opc_type )
      {
         case T_INT:
            mStream >> inst.mInt;
            break;
         case T_REAL:
            mStream >> inst.mReal;
            break;
         case T_STR:
            {
               String value;
               mStream >> value;
               inst.mString = new String(value);
            }
            break;
         case T_NONE:
         default:
            break;
      }
      
      return inst;
   }
}
