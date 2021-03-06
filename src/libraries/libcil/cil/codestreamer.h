
#ifndef CIL_CODE_STREAMER_H
#define CIL_CODE_STREAMER_H

class DataStream;
class String;

namespace CIL
{
   class Class;
   class Field;
   class Function;
   class Guard;
   class Instruction;
   class SwitchTable;
   class Type;

   class CodeStreamer
   {
   public:
      CodeStreamer(DataStream& stream);

    // operations
      CodeStreamer& operator<<(const Class& klass);
      CodeStreamer& operator>>(Class& klass);

   private:

    // writing
      void writeField(const Field& field);
      void writeFunction(const Function& function);
      void writeGuard(const Guard& guard);
      void writeTable(const SwitchTable& table);
      void writeType(const Type& type);
      void writeInstruction(const Instruction& inst);

    // reading
      Field*       readField();
      Function*    readFunction(const String& klass);
      Type         readType();
      Guard*       readGuard();
      SwitchTable* readTable();
      Instruction  readInstruction();

    // data
      DataStream& mStream;
   };
}

#endif // CIL_CODE_STREAMER_H
