
#include "generator.h"

#include "core/smartptr/autoptr.h"
#include "core/streams/filereaderstream.h"
#include "core/string/string.h"
#include "core/string/stringinterface.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"

#include "mod_yas/common/type.h"

#include "mod_yas/cil/class.h"
#include "mod_yas/cil/codestreamer.h"
#include "mod_yas/cil/field.h"
#include "mod_yas/cil/function.h"
#include "mod_yas/cil/guard.h"
#include "mod_yas/cil/guards.h"
#include "mod_yas/cil/switchtabel.h"
#include "mod_yas/cil/switchtableentry.h"
#include "mod_yas/cil/switchtables.h"

#include "mod_yas/vm/virtualarray.h"
#include "mod_yas/vm/virtualclass.h"
#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualfield.h"
#include "mod_yas/vm/virtualfunction.h"
#include "mod_yas/vm/virtualfunctiontable.h"
#include "mod_yas/vm/virtualfunctiontableentry.h"
#include "mod_yas/vm/virtualguard.h"
#include "mod_yas/vm/virtuallookuptable.h"

#include "cpu.h"
#include "irgenerator.h"
#include "program.h"

namespace ByteCode
{
   Generator::Generator()
   {
   }

   Generator::~Generator()
   {
   }

   // - Operations

   VirtualClass& Generator::generate(VirtualContext& context, const String& classname)
   {
      VirtualClass* pclass = loadClass(context, classname);
      context.mClassTable.insert(pclass);

      return *pclass;
   }

   CIL::Class* loadCompiledClass(const String& classname)
   {
      String filename;
      int index = classname.indexOf(L'<');
      if ( index != -1 )
      {
         filename = classname.subStr(0, index);
      }
      else
      {
         filename = classname;
      }

      AutoPtr<File> file = FileSystem::getInstance().open(filename + UTEXT(".cas"), File::ERead | File::EBinary);
      if ( file.hasPointer() )
      {
         CIL::Class* pcilclass = new CIL::Class();

         FileReaderStream filestream(*file);
         CIL::CodeStreamer streamer(filestream);
         streamer >> *pcilclass;

         return pcilclass;
      }
      return NULL;
   }

   VirtualClass* Generator::loadClass(VirtualContext& context, const String& classname)
   {
      AutoPtr<CIL::Class> cilclass = loadCompiledClass(classname);
      if ( !cilclass.hasPointer() )
      {
         String msg = UTEXT("Could not load class ") + classname;
         throw new std::exception(msg.toUtf8().c_str());
      }

      VirtualClass* presult = new VirtualClass();
      presult->setName(classname);

      if ( !cilclass->getBaseName().isEmpty() )
      {
         VirtualClass& baseclass = context.resolveClass(cilclass->getBaseName());
         presult->setBaseClass(baseclass);
      }

      const CIL::Class::Interfaces& interfaces = cilclass->getInterfaces();
      for ( std::size_t index = 0; index < interfaces.size(); ++index )
      {
         VirtualClass& interfce = generate(context, interfaces[index]);
         presult->addInterface(interfce);
      }

      const CIL::Class::Fields& fields = cilclass->getFields();
      for ( std::size_t index = 0; index < fields.size(); ++index )
      {
         const CIL::Field* pfield = fields[index];
         VirtualField* pvfield = new VirtualField(pfield->getName(), pfield->getType());
         presult->addField(pvfield);
      }

      const CIL::Class::Fields& staticfields = cilclass->getStatics();
      for ( std::size_t index = 0; index < staticfields.size(); ++index )
      {
         const CIL::Field* pfield = staticfields[index];
         VirtualField* pvfield = new VirtualField(pfield->getName(), pfield->getType());
         presult->addStaticField(pvfield);
      }

      CIL::Class::Functions& functions = cilclass->getFunctions();
      for ( std::size_t index = 0; index < functions.size(); ++index )
      {
         const CIL::Function& function = *functions[index];
         
         VirtualFunction* pentry = new VirtualFunction();
         pentry->setName(function.getName());
         pentry->setModifiers(function.getModifiers());
         pentry->setArguments(function.getArguments());
         pentry->setLocals(function.getLocals());
         pentry->setReturnType(function.getReturnType());
         pentry->setInstructions(function.getInstructions());

         const StringList& annotations = function.getAnnotations();
         if ( !annotations.isEmpty() )
         {
            ListConstIterator<String> it = annotations.getFront();
            for ( ; it.isValid(); ++it )
            {
               const String& annotation = *it;
               pentry->addAnnotation(annotation);
            }
         }

         const CIL::Guards& guards = function.getGuards();
         for ( int index = 0; index < guards.size(); ++index )
         {
            const CIL::Guard& guard = guards[index];
            
            VirtualGuard* pguard = new VirtualGuard();
            pguard->finalize = guard.finalize;
            pguard->locations[VirtualGuard::sStart] = guard.labels[CIL::Guard::sStart];
            pguard->locations[VirtualGuard::sCatch] = guard.labels[CIL::Guard::sCatch];
            pguard->locations[VirtualGuard::sEnd] = guard.labels[CIL::Guard::sEnd];
            pguard->locations[VirtualGuard::sFinal] = guard.labels[CIL::Guard::sFinal];

            pentry->addGuard(pguard);
         }

         const CIL::SwitchTables& tables = function.getSwitchTables();
         for ( int index = 0; index < tables.size(); ++index )
         {
            const CIL::SwitchTable& table = tables[index];
            VirtualLookupTable* ptable = new VirtualLookupTable();

            ptable->setEnd(table.getEnd());
            if ( table.hasDefault() )
            {
               ptable->setDefault(table.getDefault());
            }

            VirtualValue value;
            for ( int e = 0; e < table.size(); ++e )
            {
               const CIL::SwitchTableEntry& entry = table[e];
               value.setTo(entry.value, context.mProgram.getStringCache());
               ptable->add(value, entry.label);
            }

            pentry->addLookupTable(ptable);
         }

         presult->addFunction(pentry);
      }

      presult->build();

      return presult;
   }
}
