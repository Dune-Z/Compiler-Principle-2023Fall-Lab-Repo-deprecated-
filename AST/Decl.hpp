#ifndef TINYCCOMPILER_DECL_HPP
#define TINYCCOMPILER_DECL_HPP
#include "AST.hpp"
#include "Lex/Tokens.hpp"
#include <utility>
#include <vector>
#include <string>
namespace tinyc {
enum DeclNodeType {
  UNDEFINED_DECL,
  VAR_DECL,
  ARRAY_DECL,
  FUNC_DECL,
  PARAMS_DECL,
  STRUCT_DECL
};
class Decl: public ASTNode {
protected:
  TokenInfo type;
  TokenInfo name;
public:
  Decl(TokenInfo type, TokenInfo name);
  void accept(ASTVisitor &visitor) override;
  [[nodiscard]] int getLocation() const {return name.getLocation();}
  [[nodiscard]] std::string getName() const {return name.getLexeme();}
  [[nodiscard]] std::string getTypeName() const {return type.getLexeme();}
  virtual DeclNodeType getNodeType() {return UNDEFINED_DECL;}
};

class VarDecl final: public Decl {
private:
  ExprPtr expr;
public:
  VarDecl(const TokenInfo &type, const TokenInfo &name, ExprPtr expr);
  DeclNodeType getNodeType() override {return VAR_DECL;}
  [[nodiscard]] ExprPtr getExpr() const {return expr;}
};

class ArrayDecl final: public Decl {
private:
  ListExpr* list;
  ExprPtr size;
public:
  ArrayDecl(const TokenInfo &type, const TokenInfo &name, ExprPtr size, ListExpr* list);
  DeclNodeType getNodeType() override {return ARRAY_DECL;}
  [[nodiscard]] ExprPtr getSize() const {return size;}
  [[nodiscard]] ListExpr* getExpr() const {return list;}
};

class ParaDecl final : public Decl {
private:
  bool isRef;
public:
  ParaDecl(const TokenInfo &type, const TokenInfo &name, bool isRef);
  DeclNodeType getNodeType() override {return PARAMS_DECL;}
  [[nodiscard]] bool getIsRef() const {return isRef;}
};

class FuncDecl final : public Decl {
private:
  std::vector<ParaDecl*> params;
  CompoundStmt* body;
public:
  FuncDecl(const TokenInfo &type, const TokenInfo &name,
           const std::vector<ParaDecl*> &params, CompoundStmt *body);
  DeclNodeType getNodeType() override {return FUNC_DECL;}
  [[nodiscard]] std::vector<ParaDecl*> getParams() const {return params;}
  [[nodiscard]] CompoundStmt* getBody() const {return body;}
};

class StructDecl final : public Decl {
private:
  std::vector<ParaDecl*> members;
public:
  StructDecl(const TokenInfo &type, const TokenInfo &name, const std::vector<ParaDecl*> &members);
  DeclNodeType getNodeType() override {return STRUCT_DECL;}
  [[nodiscard]] std::vector<ParaDecl*> getMembers() const {return members;}
};
}

#endif //TINYCCOMPILER_DECL_HPP
