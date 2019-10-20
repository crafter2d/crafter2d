
#include "generator.h"

#include <libcil/common/type.h>
#include <libcil/cil/class.h>
#include <libcil/cil/codestreamer.h>
#include <libcil/cil/field.h>
#include <libcil/cil/function.h>
#include <libcil/cil/guard.h>
#include <libcil/cil/guards.h>
#include <libcil/cil/switchtabel.h>
#include <libcil/cil/switchtableentry.h>
#include <libcil/cil/switchtables.h>

#include "core/streams/filereaderstream.h"
#include "core/string/string.h"
#include "core/string/stringinterface.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"

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
      std::size_t index = classname.indexOf(L'<');
      if ( index != String::npos )
      {
         filename = classname.subStr(0, index);
      }
      else
      {
         filename = classname;
      }

      std::unique_ptr<File> file(FileSystem::getInstance().open(filename + UTEXT(".cas"), File::ERead | File::EBinary));
      if ( file )
      {
         CIL::Class* pcilclass = new CIL::Class();

         FileReaderStream filestream(*file);
         CIL::CodeStreamer streamer(filestream);
         streamer >> *pcilclass;

         return pcilclass;
      }
      return nullptr;
   }

   VirtualClass* Generator::loadClass(VirtualContext& context, const String& classname)
   {
      std::unique_ptr<CIL::Class> cilclass(loadCompiledClass(classname));
      if ( !cilclass )
      {
         String msg = UTEXT("Could not load class ") + classname;
         throw new std::runtime_error(msg.toUtf8().c_str());
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
            for ( auto& annotation : annotations )
            {
               pentry->addAnnotation(annotation);
            }
         }

         const CIL::Guards& guards = function.getGuards();
         for ( int index = 0; index < guards.size(); ++index )
         {
            const CIL::Guard& guard = guards[index];
            
            VirtualGuard vguard;
            vguard.finalize = guard.finalize;
            vguard.locations[VirtualGuard::sStart] = guard.labels[CIL::Guard::sStart];
            vguard.locations[VirtualGuard::sCatch] = guard.labels[CIL::Guard::sCatch];
            vguard.locations[VirtualGuard::sEnd] = guard.labels[CIL::Guard::sEnd];
            vguard.locations[VirtualGuard::sFinal] = guard.labels[CIL::Guard::sFinal];

            pentry->addGuard(vguard);
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

      presult->build(context);

      return presult;
   }
}
