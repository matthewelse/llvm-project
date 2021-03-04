#include "X86.h"
#include "X86InstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
//#include "llvm/Target/TargetRegisterInfo.h"

using namespace llvm;

#define X86_OCAML_EH_PASS_NAME "X86 OCaml exception handling pass"

namespace {


class X86OCamlEH : public MachineFunctionPass {
public:
  static char ID;

  X86OCamlEH() : MachineFunctionPass(ID) {
    initializeX86OCamlEHPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override { return X86_OCAML_EH_PASS_NAME; }
};

char X86OCamlEH::ID = 0;

bool X86OCamlEH::runOnMachineFunction(MachineFunction &MF) {
  for (auto &MBB : MF) {
    // machine basic block
    
    outs() << "welcome to the machine basic block\n";
    outs() << MBB << "\n";
  }

  return false;
}

} // end of anonymous namespace

INITIALIZE_PASS(X86OCamlEH, "x86-ocaml-eh", X86_OCAML_EH_PASS_NAME, true, // is cfg only
    true // is analysis
    )

namespace llvm {
  FunctionPass *createX86OCamlEHPass() { return new X86OCamlEH(); }
}
