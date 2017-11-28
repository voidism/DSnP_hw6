/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <sstream>
#include <algorithm>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
// unsigned CirMgr::CirGate::_globalRef = 0;

bool
CirMgr::readCircuit(const string& fileName)
{
  ifstream file;
  file.open(fileName.c_str());
  if(!file.is_open()){ cerr << "Cannot open design \"" << fileName << "\"!!" << endl; return false;}
  string Head, M, I, L, O, A;
  //int m, i, l, o, a;
  if(!(file >> Head)) {
    cerr << "Error1" << endl;
    return false;
  }
  if(Head!="aag") {
    cerr << "[ERROR] Line 1: Illegal number of PIs()!!" << endl;
    return false;
  }
  if(!(file >> M)) { cerr << "Error3" << endl; return false;}
  if(!myStr2Int(M,m)) { cerr << "Error4" << endl; return false;}
  if(!(file >> I)) { cerr << "Error5" << endl; return false;}
  if(!myStr2Int(I,i)) { cerr << "Error6" << endl; return false;}
  if(!(file >> L)) { cerr << "Error7" << endl; return false;}
  if(!myStr2Int(L,l)) { cerr << "Error8" << endl; return false;}
  if(!(file >> O)) { cerr << "Error9" << endl; return false;}
  if(!myStr2Int(O,o)) { cerr << "Error10" << endl; return false;}
  if(!(file >> A)) { cerr << "Error11" << endl; return false;}
  if(!myStr2Int(A,a)) { cerr << "Error12" << endl; return false;}
  //Get Miloa !!!
  lineNo++;
  vector<string> content;
  string line;
  while(getline(file, line)){
    //stringstream ss_line(line);
    content.push_back(line);
    // string item;
    // vector<string> itm_vec;
    // while(getline(ss_line, item, ' ')){
    //   itm_vec.push_back(item.c_str());
    // }
    // content.push_back(itm_vec);
  }
  _Glist.push_back(new Const(0, 0));
  for (int it = 0; it < i; it++)
  {
    cout << "I:";
    cout << content.at(lineNo) << endl;
    stringstream ss_line(content.at(lineNo));
    unsigned lit;
    if(!(ss_line >> lit)) { cerr << "Error13" << endl; return false;}
    assert(lit % 2 == 0);
    _Glist.push_back(new PI(lit / 2, lineNo));
    //_idMap.insert(pair<unsigned, CirGate *>(lit / 2, _Glist.back()));
    _idMap[lit / 2] = _Glist.back();
    lineNo++;
  }

  for (int it = 0; it < l; it++)
  {
    lineNo++;
  }
  unsigned num = m;
  for (int it = 0; it < o; it++)
  {
    cout << "O:";
    cout << content.at(lineNo) << endl;
    stringstream ss_line(content.at(lineNo));
    unsigned lit;
    if(!(ss_line >> lit)) { cerr << "Error14" << endl; return false;}
    _Glist.push_back(new PO(/* lit / 2,  */lit, lineNo, ++num));
    //_idMap.insert(pair<unsigned, CirGate *>(num, _Glist.back()));
    _idMap[num] = _Glist.back();
    lineNo++;
  }

  for (int it = 0; it < a; it++)
  {
    cout << "A:";
    cout << content.at(lineNo) << endl;
    stringstream ss_line(content.at(lineNo));
    unsigned lit,in1,in2;
    if(!(ss_line >> lit)) { cerr << "Error15" << endl; return false;}
    if(!(ss_line >> in1)) { cerr << "Error16" << endl; return false;}
    if(!(ss_line >> in2)) { cerr << "Error17" << endl; return false;}
    _Glist.push_back(new AIG(lit / 2, /* lit, */in1,in2, lineNo));
    //_idMap.insert(pair<unsigned, CirGate *>(lit / 2, _Glist.back()));
    _idMap[lit / 2] = _Glist.back();
    lineNo++;
  }

  for (unsigned i = 0; i < _Glist.size();i++){
    if(_Glist.at(i)->_idin.empty()) cout<<_Glist.at(i)->type<<" "<< _Glist.at(i)->gateID<<endl;
    else{// if(_Glist.at(i)->_idin.size()=){
      for (unsigned j = 0; j < _Glist.at(i)->_idin.size();j++){
        std::map<unsigned int, CirGate*>::iterator tmp = _idMap.find(_Glist.at(i)->_idin.at(j)/2);
        CirGate *cpr = tmp->second;
        if (tmp == _idMap.end())
        {
          if(_Glist.at(i)->_idin.at(j)==0||_Glist.at(i)->_idin.at(j)==1){
            cpr = _Glist.at(0);
          }
          else{
          _Glist.push_back(new Undef((_Glist.at(i)->_idin.at(j) / 2)));
          cpr = _idMap[(_Glist.at(i)->_idin.at(j) / 2)] = _Glist.back();
          }
        }
        _Glist.at(i)->_fin.push_back(cpr);
        cpr->_fout.push_back(_Glist.at(i));
      }
      
      // cout << _Glist.at(i)->type <<" "<< _Glist.at(i)->gateID;
      // for (unsigned u = 0; u < _Glist.at(i)->_idin.size();u++){
      //   cout << " " << _Glist.at(i)->_idin.at(u) / 2;
      // }
      // cout << endl;
      cout << _Glist.at(i)->type << " " << _Glist.at(i)->gateID;
      for (unsigned u = 0; u < _Glist.at(i)->_fin.size();u++){
        cout << " " << ((_Glist.at(i)->_idin.at(u) % 2)? "!":"")<< _Glist.at(i)->_fin.at(u)->gateID;
      }
      cout << endl;
    }
  }
  lineNo = 0;

  // }

  return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
  cout << "Circuit Statistics" << endl;
  cout << "==================" << endl;
  cout << "  PI" << setw(12) << i << endl;
  cout << "  PO" << setw(12) << o<< endl;
  cout << "  AIG" << setw(11) << a << endl;
  cout << "------------------" << endl;
  cout << "  Total"<< setw(9) << i+o+a  << endl;
}

void
CirMgr::printNetlist() const
{
  unsigned prindex = 0;
  (CirGate::_globalRef)++;
  for (int idx = i + 1; idx < i + o + 1; idx++)
  {
    DFSearch(_Glist.at(idx), prindex);
  }
}

void CirMgr::DFSearch(CirGate *it,unsigned &prindex) const{
  if(it->_ref==CirGate::_globalRef) return;
  if(it->type == "UNDEF") {it->_ref=CirGate::_globalRef;return;}
  if(!(it->_fin.empty())){
   for (int jdx = 0; jdx < (int)it->_fin.size(); jdx++)
   {
    if(it->_fin.at(jdx)->_ref==CirGate::_globalRef) continue;
    DFSearch(it->_fin.at(jdx),prindex);
   }
  }
  cout << "[" << prindex << "] " << setw(4) << left << it->type << it->gateID;
  for (unsigned u = 0; u < it->_fin.size();u++){
        cout << " " << ((it->_fin.at(u)->type=="UNDEF")? "*":"") << ((it->_idin.at(u) % 2)? "!":"")<< it->_fin.at(u)->gateID;
      }
  cout << endl;//(symbol name)
  it->_ref=CirGate::_globalRef;
  (prindex)++;
}


void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for (int idx = 1; idx < i+1;idx++){
     cout << " " << _Glist.at(idx)->gateID;
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for (int idx = i+1; idx < i+o+1;idx++){
     cout << " " << _Glist.at(idx)->gateID;
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
  vector<unsigned> nu;
  vector<unsigned> wf;
  for (int idx = 1; idx < (int)_Glist.size(); idx++)
  {
    if((_Glist.at(idx)->type==("AIG")||_Glist.at(idx)->type==("PI"))&&_Glist.at(idx)->_fout.empty()){
      nu.push_back(_Glist.at(idx)->gateID);
    }
    if(_Glist.at(idx)->type==("AIG")||_Glist.at(idx)->type==("PO")){
      for (unsigned u = 0; u < _Glist.at(idx)->_fin.size();u++){
        if(_Glist.at(idx)->_fin.at(u)->type=="UNDEF"){
          wf.push_back(_Glist.at(idx)->gateID);
          break;
        }
      }
    }
  }
  sort(wf.begin(), wf.end());
  sort(nu.begin(), nu.end());
  
  if (wf.size() > 0)
  {
    cout << "Gates with floating fanin(s):";
    for (vector<unsigned>::iterator it = wf.begin(); it != wf.end(); it++)
    {
      cout << " " << *it;
    }
    cout << endl;
  }
  if(nu.size()>0){
  cout << "Gates defined but not used  :";
  for (vector<unsigned>::iterator it = nu.begin(); it != nu.end();it++){
    cout << " " << *it;
  }
  cout << endl;
  }

}

void
CirMgr::writeAag(ostream& outfile) const
{
}
