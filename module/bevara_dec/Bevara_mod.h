#ifndef _GF_BEVARA_MOD_H_
#define _GF_BEVARA_MOD_H_

#include "Bevara_comp.h"

typedef struct s_bv_mod
{
	u16 ES_ID;

	BV_Comp* comp;

	GF_Err (*init)(char* data, unsigned int dataLength);
	GF_Err (*entry)(char *inBuffer, u32 inBufferLength, char *outBuffer, u32 *outBufferLength, u8 PaddingBits, u32 mmlevel);
	GF_Err (*shutdown)();
	GF_Err (*set_caps)(GF_CodecCapability capability);
	GF_Err (*caps)(GF_CodecCapability *capability);

} BV_Mod;

#endif	/*_GF_BEVARA_COMP_H_*/