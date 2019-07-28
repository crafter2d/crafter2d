
#ifndef XML_PARTS_H
#define XML_PARTS_H

#include <cassert>
#include <string>
#include <vector>

#include "xml/xml_base.h"

namespace c2d::xml
{
   class Attribute;
   class Child;

   class XML_API Element
   {
   public:
      using Attributes = std::vector<Attribute*>;
      using Children = std::vector<Child*>;

      explicit Element(const std::string& name) :
         mName(name)
      {
      }

      const std::string& getName() const {
         return mName;
      }

      Attributes& getAttributes() { return mAttributes; }
      Children& getChildren() { return mChildren; }

      void registerAttribute(Attribute* pattr) {
         mAttributes.push_back(pattr);
      }

      void registerChild(Child* pchild) {
         mChildren.push_back(pchild);
      }

   private:

      Attributes mAttributes;
      Children mChildren;
      std::string mName;
   };

   class XML_API Attribute
   {
   public:
      enum Type { eString, eInt, eFloat, eBool };
      enum Requirement { eMandatory, eOptional };

      Attribute(const std::string& name, void* pdata, Type type, Requirement requirement) :
         mName(name),
         mpData(pdata),
         mType(type),
         mRequirement(requirement)
      {
      }

      const std::string& getName() const {
         return mName;
      }

      std::string& asString() {
         assert(mType == eString);
         return *reinterpret_cast<std::string*>(mpData);
      }

      int& asInt() {
         assert(mType == eInt);
         return *reinterpret_cast<int*>(mpData);
      }

      float& asFloat() {
         assert(mType == eFloat);
         return *reinterpret_cast<float*>(mpData);
      }

      bool& asBool() {
         assert(mType == eBool);
         return *reinterpret_cast<bool*>(mpData);
      }

      std::string mName;
      void* mpData;
      Type mType;
      Requirement mRequirement;
   };

   class XML_API Child
   {
   public:
      enum Relation { eZeroOrOne, eZeroOrAny, eOne, eOneOrAny };

      Child(const std::string& name, Relation relation, bool allowempty) :
         mName(name),
         mRelation(relation),
         mAllowEmpty(allowempty)
      {
      }

      const std::string& getName() const { return mName; }

      virtual Element& createInstance() = 0;
      virtual bool     hasInstances() const = 0;

      std::string mName;
      Relation mRelation;
      bool mAllowEmpty;
   };

   template <class E>
   class ChildImp : public Child
   {
   public:
      using Instances = std::vector<E>;

      ChildImp(const std::string& name, Instances& instances, Relation relation, bool allowempty) :
         Child(name, relation, allowempty),
         m_instances(instances)
      {
      }

      virtual Element& createInstance() override {
         m_instances.emplace_back();
         return m_instances.back();
      }

      virtual bool hasInstances() const {
         return !m_instances.empty();
      }

      Instances& m_instances;
   };
}

#define ELEMENT_DEF(name) class name : public c2d::xml::Element { public:
#define ELEMENT_IMP(name) name::name(): c2d::xml::Element(#name) {
//#define ELEMENT_BASE_DEF(name) class name : public c2d::Element { public: name(const std::string&);
//#define ELEMENT_BASE_IMP(name) }; name::name(const std::string& name): c2d::Element(name) {
//#define ELEMENT_DERIVED_DEF(name, base) class name : public base { public: name();
//#define ELEMENT_DERIVED_IMP(name, base) }; name::name(): base(#name) {
#define ELEMENT_END } };

#define ATTRIBUTE(type, name) type name;
#define ATTRIBUTE_INIT(name, requirement, type, default) name = default; registerAttribute(new Attribute(#name, &name, type, requirement));
#define ATTRIBUTE_INIT_NAMED(name, realname, requirement, type, default) name = default; registerAttribute(new Attribute(#realname, &name, type, requirement));

#define CHILD(type) std::vector<type> type##s;
#define CHILD_NAMED(type, xmlname) std::vector<type> xmlname##s;
#define CHILD_INIT(type, rel, empty) registerChild(new ChildImp<type>(#type, type##s, rel, empty));
#define CHILD_INIT_NAMED(type, xmlname, rel, empty) registerChild(new ChildImp<type>(#xmlname, xmlname##s, rel, empty));

#endif // XML_PARTS_H

