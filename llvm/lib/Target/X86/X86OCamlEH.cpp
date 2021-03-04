/* I made this as a machine function pass, but I wish it could be a regular IR pass :(
 */

#include "X86.h"
#include "X86InstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

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
  SmallString<128> Name;

  for (auto &MBB : MF) {
    // machine basic block
    
    outs() << "welcome to the machine basic block\n" << MBB << "\n";
    bool previous_was_setjmp = false;

    for (auto &MI : MBB) {

      if (previous_was_setjmp) {
        //previous_was_setjmp = false;

        outs() << "after setjmp: " << MI << "\n";
        continue;
      }

      if (MI.getOpcode() != X86::CALL64pcrel32) continue; 
      if (MI.getNumOperands() < 1) continue; 

      auto MO = MI.getOperand(0);

      if (!MO.isGlobal()) continue;

      auto GI = MO.getGlobal()->getGlobalIdentifier();


      if (GI == "ocaml.setjmp") {
        outs() << "victory\n";
        outs() << "op : " << MI << "\n";
        outs() << "operand(0) name = " << GI << "\n";
        previous_was_setjmp = true;

        for (auto DEF : MI.defs()) { 
          outs() << "def : " << DEF << "\n";
        }

        // For v = ocaml.setjmp(), we generate
        // 
        // thisMBB:
        //   push restoreMBB
        //   push (r14+16)
        //   mov %rsp,(r14+16)
        // restoreMBB:
        //   pop (r14+16)
        //   add $8,%rsp
        //   ; result is already in %rax
        // mainMBB:
        //   %rax <- 0

        const BasicBlock* BB = MBB.getBasicBlock();
        MachineBasicBlock *thisMBB = &MBB;

      }
    }
  }

  return false;
}

} // end of anonymous namespace

INITIALIZE_PASS(
  X86OCamlEH,
  "x86-ocaml-eh",
  X86_OCAML_EH_PASS_NAME,
  true, // is cfg only
  true // is analysis
)

namespace llvm {
  FunctionPass *createX86OCamlEHPass() { return new X86OCamlEH(); }
}
