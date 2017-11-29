/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
  friend class CirMgr;

public:
  CirGate(){}
  virtual ~CirGate() {}

  // Basic access methods
  string getTypeStr() const { return type; }
  unsigned getLineNo() const { return linenum; }

  // Printing functions
  virtual void printGate() const = 0;
  void reportGate() const;
  void reportFanin(int level) const;
  void reportFanout(int level) const;

  void DFSearchByLevel_fanin(const CirGate *it,int dig_level,int total_level,bool inv) const;
  void DFSearchByLevel_fanout(const CirGate *it,int dig_level,int total_level,bool inv) const;

private:
protected:
  static unsigned _globalRef;
  mutable unsigned _ref; //marked number used in DFS
  //int varID;
  //int litID;
  unsigned gateID;
  vector<CirGate *> _fout;
  vector<unsigned> _idin; //fanin literal id list
  vector<CirGate *> _fin;
  vector<unsigned> _idout;
  unsigned linenum;
  string type;
  string symb = "";
};

class PI : public CirGate
{
  friend class CirMgr;
public:
  PI(int, unsigned);
  ~ PI(){}
  void printGate() const {};
};

//const string PI::type = "PI";

class PO : public CirGate
{
  friend class CirMgr;
public:
  PO(int, unsigned, unsigned);
  ~ PO(){}
  void printGate() const {};
};
//const string PO::type = "PO";
class Const : public CirGate
{
  friend class CirMgr;
public:
  Const(int, unsigned);
  ~ Const(){}
  void printGate() const {};
};
//const string Const::type = "CONST";

class Undef : public CirGate
{
  friend class CirMgr;
public:
  Undef(int);
  ~ Undef(){}
  void printGate() const {};
};
//const string Undef::type = "UNDEF";
class AIG : public CirGate
{
  friend class CirMgr;
public:
  AIG(int, unsigned, unsigned, unsigned);
  AIG(int, unsigned);
  ~AIG() {}
  void printGate() const {};
};
//const string AIG::type = "AIG";
#endif // CIR_GATE_H
