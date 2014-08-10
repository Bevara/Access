#ifndef _GF_BEVARA_COMP_H_
#define _GF_BEVARA_COMP_H_

#include <gpac/tools.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct __bv_Comp_s BV_Comp;

/*!
 *	\brief LLVM compiler 
 *
 * \param llvm_code the llvm code of the decoder
 * \return pointer to the code
 */
GF_Err compile(BV_Comp** comp, const char* llvm_code, unsigned int length);
GF_Err clean(BV_Comp* comp);

void* getMDFn(BV_Comp* comp, const char * name);
int getMDint(BV_Comp* comp, const char * name);
int getMD4CC(BV_Comp* comp, const char * name);
int getMDVal(BV_Comp* comp, const char * name);

#ifdef __cplusplus
}
#endif

#endif	/*_GF_BEVARA_COMP_H_*/