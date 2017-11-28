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

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

PI::PI(int var,int lit,unsigned line) {
    varID = (var);
    gateID = var;
    litID = (lit);
    linenum = (line);
    type = "PI";
  }
  
PO::PO(int var,int lit,unsigned line,unsigned gid) {
    varID = (var);
    litID = (lit);
    linenum = (line);
    _idin.push_back(lit);
    gateID = gid;
    type = "PO";
  }
  
AIG::AIG(int var,int lit,unsigned in1,unsigned in2,unsigned line) {
    varID = (var);
    litID = (lit);
    gateID = var;
    linenum = (line);
    _idin.push_back(in1);
    _idin.push_back(in2);
    type = "AIG";
  }

AIG::AIG(int var,int lit,unsigned line) {
    varID = (var);
    litID = (lit);
    gateID = var;
    linenum = (line);
    type = "AIG";
}
  
Undef::Undef(int var,int lit,unsigned line) {
    varID = (var);
    litID = (lit);
    gateID = var;
    linenum = (line);
    type = "UNDEF";
  }
  
Const::Const(int var,int lit,unsigned out,unsigned line) {
    varID = (var);
    litID = (lit);
    gateID = var;
    linenum = (line);
    _idout.push_back(out);
    type = "CONST";
}
