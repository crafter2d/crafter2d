
#ifndef AST_TYPE_LIST_H_
#define AST_TYPE_LIST_H_

#include <vector>

class ASTClass;
class ASTType;

class ASTTypeList
{
   typedef std::vector<ASTType*> TypeList;

public:
   ASTTypeList() {}
   ASTTypeList(const ASTTypeList& that);
   ~ASTTypeList() {
      clear();
   }

 // operators
   const ASTTypeList& operator=(const ASTTypeList& that);
   bool operator==(const ASTTypeList& that) const;

   const ASTType& operator[](int index) const { return *mTypeList[index]; }
         ASTType& operator[](int index) { return *mTypeList[index]; }

 // query
   int size() const { return mTypeList.size(); }
   bool empty() const;

   bool contains(const ASTClass& intrface) const;

 // operations
   void append(ASTType* ptype) { mTypeList.push_back(ptype); }
   void append(const ASTTypeList& that);

   void clear();

private:
   TypeList mTypeList;
};

#endif // AST_TYPE_LIST_H_
