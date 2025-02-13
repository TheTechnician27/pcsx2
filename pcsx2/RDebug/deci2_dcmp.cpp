// SPDX-FileCopyrightText: 2002-2025 PCSX2 Dev Team
// SPDX-License-Identifier: GPL-3.0+

#include "Common.h"
#include "deci2.h"

struct DECI2_DCMP_HEADER{
	DECI2_HEADER	h;		//+00
	u8				type,	//+08
					code;	//+09
	u16				_pad;	//+0A
};		//=0C

struct DECI2_DCMP_CONNECT{
	u8				result,	//+00
					_pad[3];//+01
	u64				EEboot,	//+04
					IOPboot;//+0C
};		//=14

struct DECI2_DCMP_ECHO{
	u16				identifier,	//+00
					sequence;	//+02
	u8				data[32];	//+04
};				//=24

void D2_DCMP(const u8 *inbuffer, u8 *outbuffer, char *message){
	DECI2_DCMP_HEADER	*in=(DECI2_DCMP_HEADER*)inbuffer,
						*out=(DECI2_DCMP_HEADER*)outbuffer;
	u8	*data=(u8*)in+sizeof(DECI2_DCMP_HEADER);

	//DECI2_DCMP_CONNECT	*connect=(DECI2_DCMP_CONNECT*)data;
	//DECI2_DCMP_ECHO		*echo	=(DECI2_DCMP_ECHO*)data;

	memcpy(outbuffer, inbuffer, 128*1024);//BUFFERSIZE
	out->h.length=sizeof(DECI2_DCMP_HEADER);
/*	switch(in->type){
		case 0:
			sprintf(message, "  [DCMP] type=CONNECT code=%s EEboot=0x%I64X IOP=0x%I64X",
				in->code==0?"CONNECT":"DISCONNECT", connect->EEboot, connect->IOPboot);
			data=(u8*)out+sizeof(DECI2_DCMP_HEADER);
			connect=(DECI2_DCMP_CONNECT*)data;
			connect->result=0;
			break;
		case 1:
			sprintf(message, "  [DCMP] type=ECHO id=%X seq=%X", echo->identifier, echo->sequence);
			exchangeSD(&out->h);
			break;
//		not implemented, not needed?
		default:
			sprintf(message, "  [DCMP] type=%d[unknown]", in->type);
	}*/
	out->code++;
}

void sendDCMP(u16 protocol, u8 source, u8 destination, u8 type, u8 code, char *data, int size){
	static u8 tmp[100];
	((DECI2_DCMP_HEADER*)tmp)->h.length		=sizeof(DECI2_DCMP_HEADER)+size;
	((DECI2_DCMP_HEADER*)tmp)->h._pad		=0;
	((DECI2_DCMP_HEADER*)tmp)->h.protocol	=protocol;
	((DECI2_DCMP_HEADER*)tmp)->h.source		=source;
	((DECI2_DCMP_HEADER*)tmp)->h.destination=destination;
	((DECI2_DCMP_HEADER*)tmp)->type			=type;
	((DECI2_DCMP_HEADER*)tmp)->code			=code;
	((DECI2_DCMP_HEADER*)tmp)->_pad			=0;
	memcpy(&tmp[sizeof(DECI2_DCMP_HEADER)], data, size);
	writeData(tmp);
}
