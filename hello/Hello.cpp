#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <map>

using namespace llvm;
using namespace std;

namespace {
    static int c = 0;
    static std::map<BasicBlock*, int> BBMap;

    class HelloPass : public FunctionPass {
        static char ID;
        HelloPass() : FunctionPass(ID) {}
        

        virtual bool runOnFunction(Function &F) {
            errs() << "digraph \"CFG for '" << F.getName().str() << "' function\" {" << '\n'; 
            for(auto& B : F){
                if(BBMap.count(&B) == 0){
                    BBMap[&B] = c;
                    c += 1;
                }

                errs() << '\t' << "BB" << BBMap[&B] << " [shape=record,\n\tlabel=\"{" << B.getName() << ":\\l\\l\n";
                for(auto& I : B){
                    errs() << "\t"<<I << "\\l\n";
                    if(I.getOpcode() == 1 || I.getOpcode() == 2){
                        errs() << "\t\\l}\"];\n";

                        for(int i = 0; i < I.getNumOperands(); ++i){
                            if (isa<BasicBlock>(I.getOperand(i))) {
                                BasicBlock* op = dyn_cast<BasicBlock>(I.getOperand(i));
                                if(BBMap.count(op) == 0){
                                    BBMap[op] = c;
                                    c += 1;
                                }
                                errs() << '\t' << "BB" << BBMap[&B] << " -> " << "BB" << BBMap[op] << '\n';
                            }
                        }

                    }
                }
            }
            errs() << "}" << "\n";
        }
    };
}

char HelloPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerHelloPass(const PassManagerBuilder &,
        legacy::PassManagerBase &PM) {
    PM.add(new HelloPass());
}
static RegisterStandardPasses
RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
        registerHelloPass);
