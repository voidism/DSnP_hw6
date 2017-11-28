/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h
unsigned CirGate::_globalRef = 0;

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
  cout << "==================================================" << endl;
  stringstream ss;
  ss << "= " << type << "(" << gateID << ")" /*"symbol"*/ << ", line " << ((linenum==0)? 0: (linenum+1) );
  string s = ss.str();
   s.resize(49, ' ');
   s += "=\n";
   cout << s;
   cout << "==================================================" << endl;
}

void CirGate::DFSearchByLevel(const CirGate *it,int dig_level,int total_level,bool inv) const{
  //if((it->_fin.empty())){
  bool printed = (it->_ref==CirGate::_globalRef);
  string s = "";
  s.resize(2*(total_level - dig_level),' ');
  cout << s << (inv?"!":"") << it->type << " " << it->gateID;
  cout << (printed?" (*)":"")<<endl;//(symbol name)
  it->_ref=CirGate::_globalRef;
  
  //}
 // else{
   for (int jdx = 0; jdx < (int)it->_fin.size(); jdx++)
   {
    if(it->_fin.at(jdx)->_ref==CirGate::_globalRef) {
      string s = "";
      s.resize(2*(total_level - dig_level),' ');
      cout << s << (inv?"!":"") << it->type << " " << it->gateID;
      cout << " (*)"<<endl;//(symbol name)
      continue;
    }
    DFSearchByLevel(it->_fin.at(jdx),dig_level-1,total_level,(it->_idin.at(jdx) % 2));
   }
 // }
  
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   _globalRef++;
   DFSearchByLevel(this,level,level,false);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

PI::PI(int var,unsigned line) {
    //varID = (var);
    gateID = var;
    //litID = (lit);
    linenum = (line);
    type = "PI";
  }
  
PO::PO(int lit,unsigned line,unsigned gid) {
    //varID = (lit/2);
    /* litID = (lit); */
    linenum = (line);
    _idin.push_back(lit);
    gateID = gid;
    type = "PO";
  }
  
AIG::AIG(int var,unsigned in1,unsigned in2,unsigned line) {
    //varID = (var);
    /* litID = (lit); */
    gateID = var;
    linenum = (line);
    _idin.push_back(in1);
    _idin.push_back(in2);
    type = "AIG";
  }

AIG::AIG(int var,unsigned line) {
    //varID = (var);
    /* litID = (lit); */
    gateID = var;
    linenum = (line);
    type = "AIG";
}
  
Undef::Undef(int var) {
    //varID = (var);
    /* litID = (lit); */
    gateID = var;
    linenum = 0;
    type = "UNDEF";
  }
  
Const::Const(int var,/* int lit, */unsigned line) {
    //varID = (var);
    /* litID = (lit); */
    gateID = var;
    linenum = (line);
    //_idout.push_back(out);
    type = "CONST";
}
