#include <gpac/modules/service.h>
#include <gpac/modules/codec.h>
#include <gpac/avparse.h>
#include <gpac/constants.h>

#include "Bevara_mod.h"

//#define _WRAP

#ifdef _WRAP
/*extern int shutdown(void);
extern int init(void* data, unsigned int dataLength);*/
extern int entry( 
		char *inBuffer, unsigned int inBufferLength,
		char *outBuffer, unsigned int *outBufferLength,
		unsigned char PaddingBits, unsigned int mmlevel);
extern int GetCapabilities(GF_CodecCapability *capability);
#endif

static GF_Err BV_AttachStream(GF_BaseDecoder *ifcg, GF_ESD *esd)
{
	GF_Err err = GF_OK;
	char *llvm_code = NULL; 
	u32 llvm_length;
	char* data = NULL;
	u32 dataLength = 0;
	
	BV_Mod *ctx = (BV_Mod*) ifcg->privateStack;

	/*not supported for a first version (and may be never be)*/
	if (esd->dependsOnESID) return GF_NOT_SUPPORTED;
	llvm_length = esd->decoderConfig->rvc_config->dataLength;
	if (!llvm_length) return GF_NOT_SUPPORTED;

	ctx->ES_ID = esd->ESID;
	llvm_code = esd->decoderConfig->rvc_config->data;
	llvm_code[llvm_length] = '\0';

	// Init LLVM compilation
	err = compile(&ctx->comp, llvm_code, llvm_length);
	if (err) return err;
	
#ifdef _WRAP
	//ctx->init = init;
	ctx->caps = GetCapabilities;
	ctx->entry = entry;
	//ctx->shutdown = shutdown;
#else
	ctx->entry = getMDFn(ctx->comp, "ENTRY");
	if (!ctx->entry) return GF_CORRUPTED_DATA;
	ctx->init = getMDFn(ctx->comp, "INIT");
	ctx->shutdown = getMDFn(ctx->comp, "CLOSE");
	ctx->caps = getMDFn(ctx->comp, "CAPABILITIES");
	ctx->set_caps = getMDFn(ctx->comp, "SET_CAPABILITIES");
#endif
	if (esd->decoderConfig->decoderSpecificInfo){
		data = esd->decoderConfig->decoderSpecificInfo->data;
		dataLength = esd->decoderConfig->decoderSpecificInfo->dataLength;
	}

	/* Initialize dec */
	if (ctx->init){
		err = ctx->init(data, dataLength);

		if (err) return err;
	}

	//gf_free(llvm_code);
	esd->decoderConfig->rvc_config->data = NULL;
	esd->decoderConfig->rvc_config->dataLength = 0;

	return err;
}
static GF_Err BV_DetachStream(GF_BaseDecoder *ifcg, u16 ES_ID)
{
	BV_Mod *ctx = (BV_Mod*) ifcg->privateStack;
	if (ctx->shutdown)  ctx->shutdown();
	return GF_OK;
}

static GF_Err BV_GetCapabilities(GF_BaseDecoder *ifcg, GF_CodecCapability *capability)
{
	BV_Mod *ctx = (BV_Mod*) ifcg->privateStack;
	if (!ctx->caps) return GF_NOT_SUPPORTED;
	return ctx->caps(capability);

}

static GF_Err BV_SetCapabilities(GF_BaseDecoder *ifcg, GF_CodecCapability capability)
{
	BV_Mod *ctx = (BV_Mod*) ifcg->privateStack;
	if (!ctx->set_caps) return GF_NOT_SUPPORTED;
	return ctx->set_caps(capability);
}


int bookmark = 0;

static GF_Err BV_ProcessData(GF_MediaDecoder *ifcg, 
		char *inBuffer, u32 inBufferLength,
		u16 ES_ID,
		char *outBuffer, u32 *outBufferLength,
		u8 PaddingBits, u32 mmlevel)
{
	BV_Mod *ctx = (BV_Mod*) ifcg->privateStack;
	return ctx->entry(inBuffer, inBufferLength, outBuffer, outBufferLength,0,0);
}

static u32 BV_CanHandleStream(GF_BaseDecoder *dec, u32 StreamType, GF_ESD *esd, u8 PL)
{
	if (esd && esd->decoderConfig && esd->decoderConfig->rvc_config) return GF_CODEC_SUPPORTED;
	return GF_CODEC_NOT_SUPPORTED;
}

static const char *BV_GetCodecName(GF_BaseDecoder *dec)
{
	return "Bevara Decoder";
}


void Delete_BV_Mod(GF_BaseDecoder *ifcg)
{
	BV_Mod *ctx = (BV_Mod*) ifcg->privateStack;
	gf_free(ctx);
	gf_free(ifcg);
}

GF_BaseDecoder *New_BV_Decoder()
{
	GF_MediaDecoder *ifcd;
	BV_Mod *dec;
	
	GF_SAFEALLOC(ifcd, GF_MediaDecoder);
	GF_SAFEALLOC(dec, BV_Mod);
	GF_REGISTER_MODULE_INTERFACE(ifcd, GF_MEDIA_DECODER_INTERFACE, "Bevara Decoder", "Bevara Technologies")

	ifcd->privateStack = dec;

	/*setup our own interface*/	
	ifcd->AttachStream = BV_AttachStream;
	ifcd->DetachStream = BV_DetachStream;
	ifcd->GetCapabilities = BV_GetCapabilities;
	ifcd->SetCapabilities = BV_SetCapabilities;
	ifcd->GetName = BV_GetCodecName;
	ifcd->CanHandleStream = BV_CanHandleStream;
	ifcd->ProcessData = BV_ProcessData;
	return (GF_BaseDecoder *) ifcd;
}



GPAC_MODULE_EXPORT
const u32 *QueryInterfaces() 
{
	static u32 si [] = {
#ifndef GPAC_DISABLE_AV_PARSERS
		GF_MEDIA_DECODER_INTERFACE,
#endif
		0
	};
	return si; 
}

GPAC_MODULE_EXPORT
GF_BaseInterface *LoadInterface(u32 InterfaceType) 
{
	//if (InterfaceType == GF_NET_CLIENT_INTERFACE) return (GF_BaseInterface *)New_BV_Service();
	if (InterfaceType == GF_MEDIA_DECODER_INTERFACE) return (GF_BaseInterface *)New_BV_Decoder();
	return NULL;
}

GPAC_MODULE_EXPORT
void ShutdownInterface(GF_BaseInterface *ifce)
{
	switch (ifce->InterfaceType) {
#ifndef GPAC_DISABLE_AV_PARSERS
	case GF_MEDIA_DECODER_INTERFACE: 
		Delete_BV_Mod((GF_BaseDecoder*)ifce);
		break;
#endif
	}
}


GPAC_MODULE_STATIC_DELARATION( BV_Mod )
