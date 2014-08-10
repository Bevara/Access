

#include <gpac/tools.h>


#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"


#include "Bevara_comp.h"

using namespace llvm;

struct __bv_Comp_s
{
	Module *Mod;
	ExecutionEngine *EE;
};

GF_Err compile(BV_Comp** comp, const char* llvm_code, unsigned int length){
	LLVMContext &Context = getGlobalContext();
	InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	InitializeNativeTargetAsmParser();

	// Parse code
	SMDiagnostic Err;
	MemoryBuffer *Buf = MemoryBuffer::getMemBuffer(StringRef(llvm_code, length));
	Module *Mod = ParseIR(Buf, Err, Context);
	//Module *Mod = ParseIRFile("D:\\projet\\Bevara\\Bevara_bin\\Win32\\Debug\\decoders\\FAAD2_0.18.ll", Err, Context);

	if (!Mod) {
		//Parsing error
		return GF_CORRUPTED_DATA;
	}


	// Build engine
	EngineBuilder builder(Mod);
	builder.setOptLevel(CodeGenOpt::Aggressive);
	ExecutionEngine* EE = builder.create();
	if (!EE) return GF_OUT_OF_MEM;

	// Store compiler
	BV_Comp* tmp_comp;
	GF_SAFEALLOC(tmp_comp, BV_Comp);
	tmp_comp->EE = EE;
	tmp_comp->Mod = Mod;
	*comp = tmp_comp;

	return GF_OK;
}

GF_Err clean(BV_Comp* comp){

	return GF_OK;
}

void* getMDFn(BV_Comp* comp, const char * name){
	if (!comp || !name) return NULL;
	Function *EntryFn = NULL;

	NamedMDNode* EntryMD = comp->Mod->getNamedMetadata(name);
	if (!EntryMD) return NULL;
	for (unsigned i = 0, e = EntryMD->getNumOperands(); i != e; ++i) {
		MDNode* node = EntryMD->getOperand(i);
		for (unsigned j = 0, f = node->getNumOperands(); (j != f); ++j) {
			if (isa<Function>(node->getOperand(j)))
				EntryFn =  cast<Function>(node->getOperand(j));
			break;
		}
		if (EntryFn) break;
	}
	
	if (!EntryFn) return NULL;
	return comp->EE->getPointerToFunction(EntryFn);
}

int getMDint(BV_Comp* comp, const char * name){
	if (!comp || !name) return NULL;
	ConstantInt* val;

	NamedMDNode* EntryMD = comp->Mod->getNamedMetadata(name);
	for (unsigned i = 0, e = EntryMD->getNumOperands(); i != e; ++i) {
		MDNode* node = EntryMD->getOperand(i);
		for (unsigned j = 0, f = node->getNumOperands(); (j != f); ++j) {
			if (isa<ConstantInt>(node->getOperand(j)))
				val =  cast<ConstantInt>(node->getOperand(j));
			break;
		}
		if (val) break;
	}
	
	if (!val) return 0;
	return val->getLimitedValue();
}

int getMD4CC(BV_Comp* comp, const char * name){
	if (!comp || !name) return NULL;
	MDString* Str4cc = NULL;

	NamedMDNode* MD4CC = comp->Mod->getNamedMetadata(name);
	if (!MD4CC) return 0;
	for (unsigned i = 0, e = MD4CC->getNumOperands(); i != e; ++i) {
		MDNode* node = MD4CC->getOperand(i);
		for (unsigned j = 0, f = node->getNumOperands(); (j != f); ++j) {
			if (isa<MDString>(node->getOperand(j)))
				Str4cc =  cast<MDString>(node->getOperand(j));
			break;
		}
		if (Str4cc) break;
	}
	
	if (!Str4cc) return 0;
	std::string s4cc = Str4cc->getString();

	return GF_4CC(s4cc[0],s4cc[1],s4cc[2],s4cc[3]);
}

int getMDVal(BV_Comp* comp, const char * name){
	if (!comp || !name) return NULL;
	ConstantInt* IntVal = NULL;

	NamedMDNode* MDName = comp->Mod->getNamedMetadata(name);
	if (!MDName) return 0;
	for (unsigned i = 0, e = MDName->getNumOperands(); i != e; ++i) {
		MDNode* node = MDName->getOperand(i);
		for (unsigned j = 0, f = node->getNumOperands(); (j != f); ++j) {
			if (isa<ConstantInt>(node->getOperand(j)))
				IntVal =  cast<ConstantInt>(node->getOperand(j));
			break;
		}
		if (IntVal) break;
	}
	
	if (!IntVal) return 0;


	return (int)IntVal->getLimitedValue();
}