/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "cirGate.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

#define CLEAN_GLIST                                                               \
  for (vector<CirGate *>::iterator it = _Glist.begin(); it != _Glist.end(); it++) \
  {                                                                               \
    delete *it;                                                                   \
  }

// TODO: Define your own data members and member functions
class CirMgr
{
public:
  CirMgr() {}
  ~CirMgr()
  {
    CLEAN_GLIST;
  }

  // Access functions
  // return '0' if "gid" corresponds to an undefined gate.
  CirGate *getGate(unsigned gid) const
  {
    std::map<unsigned int, CirGate *>::const_iterator tmp = _idMap.find(gid);
    return ((tmp != _idMap.end()) ? tmp->second : 0);
  }

  // Member functions about circuit construction
  bool readCircuit(const string &);

  // Member functions about circuit reporting
  void printSummary() const;
  void printNetlist() const;
  void printPIs() const;
  void printPOs() const;
  void printFloatGates() const;
  void writeAag(ostream &) const;

  void DFSearch(CirGate *, unsigned &) const;
  void DFSearch_NoPrint(CirGate *, unsigned &, stringstream &ss) const;
  //void DFSearchByLevel_fanin(CirGate*, int, int) const;
  //void sortsingle(vector<CirGate*>);
  //void sortsingleint(vector<unsigned>);
  void insertSort(vector<pair<CirGate*,bool>>&);
  //bool cmp(const pair<CirGate*,bool> i1, const pair<CirGate*,bool> i2);
  
private:
  vector<CirGate *> _Glist;
  map<unsigned, CirGate *> _idMap;
  int m, i, l, o, a;
  //vector<string> c;
};

#endif // CIR_MGR_H
