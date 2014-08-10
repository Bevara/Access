#include "Bevara_serv.h"


#include <gpac/modules/codec.h>
#include <gpac/avparse.h>
#include <gpac/constants.h>
#include <gpac/download.h>

#include <time.h>


static u32 AAC_RegisterMimeTypes(const GF_InputService *plug){
    return 1;
}


static Bool AAC_CanHandleURL(GF_InputService *plug, const char *url)
{
	return GF_TRUE;
}


static GF_ESD *AAC_GetESD(void *read)
{
	return 0;
}

static void * AAC_Reader_new(){
	return NULL;
}



static GF_Err AAC_ConnectService(GF_InputService *plug, GF_ClientService *serv, const char *url)
{
	return GF_OK;
}

static GF_Err AAC_CloseService(GF_InputService *plug)
{
	return GF_OK;
}

static GF_Descriptor *AAC_GetServiceDesc(GF_InputService *plug, u32 expect_type, const char *sub_url)
{
	return NULL;
}

static GF_Err AAC_ConnectChannel(GF_InputService *plug, LPNETCHANNEL channel, const char *url, Bool upstream)
{
	
	return GF_OK;
}

static GF_Err AAC_DisconnectChannel(GF_InputService *plug, LPNETCHANNEL channel)
{
	return GF_OK;
}

static GF_Err AAC_ServiceCommand(GF_InputService *plug, GF_NetworkCommand *com)
{
	return GF_OK;
}


static GF_Err AAC_ChannelGetSLP(GF_InputService *plug, LPNETCHANNEL channel, char **out_data_ptr, u32 *out_data_size, GF_SLHeader *out_sl_hdr, Bool *sl_compressed, GF_Err *out_reception_status, Bool *is_new_data)
{
	return GF_OK;
}

static GF_Err AAC_ChannelReleaseSLP(GF_InputService *plug, LPNETCHANNEL channel)
{
	return GF_OK;
}

GF_InputService *AAC_Load()
{

	GF_InputService *plug = gf_malloc(sizeof(GF_InputService));
	memset(plug, 0, sizeof(GF_InputService));
	GF_REGISTER_MODULE_INTERFACE(plug, GF_NET_CLIENT_INTERFACE, "GPAC AAC Reader", "gpac distribution")

	plug->CanHandleURL = AAC_CanHandleURL;
	plug->ConnectService = AAC_ConnectService;
	plug->CloseService = AAC_CloseService;
	plug->GetServiceDescriptor = AAC_GetServiceDesc;
	plug->ConnectChannel = AAC_ConnectChannel;
	plug->DisconnectChannel = AAC_DisconnectChannel;
	plug->ServiceCommand = AAC_ServiceCommand;
	/*we do support pull mode*/
	plug->ChannelGetSLP = AAC_ChannelGetSLP;
	plug->ChannelReleaseSLP = AAC_ChannelReleaseSLP;
	plug->RegisterMimeTypes = AAC_RegisterMimeTypes;
	plug->priv = AAC_Reader_new();
	return plug;
}

void AAC_Delete(void *ifce)
{

}



GF_InputService *New_BV_Service()
{

	GF_InputService *plug = gf_malloc(sizeof(GF_InputService));
	memset(plug, 0, sizeof(GF_InputService));
	GF_REGISTER_MODULE_INTERFACE(plug, GF_NET_CLIENT_INTERFACE, "GPAC AAC Reader", "gpac distribution")

	plug->CanHandleURL = AAC_CanHandleURL;
	plug->ConnectService = AAC_ConnectService;
	plug->CloseService = AAC_CloseService;
	plug->GetServiceDescriptor = AAC_GetServiceDesc;
	plug->ConnectChannel = AAC_ConnectChannel;
	plug->DisconnectChannel = AAC_DisconnectChannel;
	plug->ServiceCommand = AAC_ServiceCommand;
	/*we do support pull mode*/
	plug->ChannelGetSLP = AAC_ChannelGetSLP;
	plug->ChannelReleaseSLP = AAC_ChannelReleaseSLP;
	plug->RegisterMimeTypes = AAC_RegisterMimeTypes;
	plug->priv = AAC_Reader_new();
	return plug;
}