class A extends Object { A() { super(); } }

class B extends Object { B() { super(); } }

class Pair extends Object {
  Object fst;
  Object snd;

  // Constructor
  Pair(Object fst, Object snd) {
    super(); this.fst = fst; this.snd = snd; }

  // Method definition
  Pair setfst(Object newfst) {
    return new Pair(newfst, this.snd); } }
