#include "bd_rnss.h"

uint8_t icjc[12]={0x24,0x49,0x43,0x4A,0x43,0x00,0x0C,0x00,0x00,0x00,0x00,0x2B};
uint8_t xtzj[13]={0x24,0x58,0x54,0x5A,0x4A,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x35};
uint8_t sjsc[13]={0x24,0x53,0x4A,0x53,0x43,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x20};
uint8_t gnps[13]={0x24,0x47,0x4E,0x50,0x53,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x23};
uint8_t bssz[15]={0x24,0x42,0x53,0x53,0x5A,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33};

ICXX_TypeDef icxx_type;
ZJXX_TypeDef zjxx_type;
SJXX_TypeDef sjxx_type;
GNPX_TypeDef gnpx_type;
FKXX_TypeDef fkxx_type;

RNSSStatus_TypeDef RNSS_Cmp(uint8_t *data){
	if(data[0]==0x24&data[1]==0x49&data[2]==0x43&data[3]==0x58&data[4]==0x58){
		return ICXX;
	}else if(data[0]==0x24&data[1]==0x5A&data[2]==0x4A&data[3]==0x58&data[4]==0x58){
		return ZJXX;
	}else if(data[0]==0x24&data[1]==0x53&data[2]==0x4A&data[3]==0x58&data[4]==0x58){
		return SJXX;
	}else if(data[0]==0x24&data[1]==0x47&data[2]==0x4E&data[3]==0x50&data[4]==0x58){
		return GNPX;
	}else if(data[0]==0x24&data[1]==0x46&data[2]==0x4B&data[3]==0x58&data[4]==0x58){
		return FKXX;
	}else if(data[0]==0x24&data[1]==0x54&data[2]==0x58&data[3]==0x58&data[4]==0x58){
		return TXXX;
	}else{
		return OTHER;
	}
}

uint8_t crc(uint8_t *data){
 return 0;
}

void RNSS_CMD(uint8_t *cmd,uint16_t size){
	Usart_SendStr(USART1,cmd,size);
}


void RNSS_ICJC_GET(uint8_t *buf){
	memcpy(icxx_type.user_address,&buf[7],3);
	icxx_type.frame=buf[10];
	memcpy(icxx_type.broadcast_id,&buf[11],3);
	icxx_type.user_feature=buf[14];
	memcpy(icxx_type.server_time,&buf[15],2);
	icxx_type.send_level=buf[17];
	icxx_type.enc_flag=buf[18];
	memcpy(icxx_type.under_sum,&buf[19],2);
	icxx_type.crc=buf[21];
}

void RNSS_XTZJ_GET(uint8_t *buf){
	uint8_t Max,i=0,result,result_count=0;
	zjxx_type.ic_status=buf[10];
	zjxx_type.broad_status=buf[11];
	zjxx_type.battery_status=buf[12];
	zjxx_type.join_status=buf[13];
	zjxx_type.one_level=buf[14];
	zjxx_type.two_level=buf[15];
	zjxx_type.three_level=buf[16];
	zjxx_type.four_level=buf[17];
	zjxx_type.five_level=buf[18];
	zjxx_type.six_level=buf[19];
	zjxx_type.crc=buf[20];
	for(i=0;i<6;i++){
		if(buf[14+i]==0x04){
			result_count++;
		}
		if(Max<buf[14+i]){
			Max=buf[14+i];
			result=i;
		}
	}
	if(Max!=0x00&&result_count>1){
		bssz[10]=result;
		bssz[14]=Get_CRC(bssz,14);
		Send_Flag=0x01;
	}else{
		bssz[10]=0x00;
		bssz[14]=Get_CRC(bssz,14);
		Send_Flag=0x00;
	}
	RNSS_CMD(bssz,sizeof(bssz));
}

void RNSS_SJSC_GET(uint8_t *buf){
	sjxx_type.year=((buf[10]<<8)|buf[11]);
	sjxx_type.month=buf[12];
	sjxx_type.day=buf[13];
	sjxx_type.hour=buf[14];
	sjxx_type.minute=buf[15];
	sjxx_type.second=buf[16];
	sjxx_type.crc=buf[17];
}

void RNSS_GNPS_GET(uint8_t *buf){
	double buf_lat,buf_lon;
	memcpy(gnpx_type.longitude,&buf[11],4);
	buf_lon=(gnpx_type.longitude[3]+gnpx_type.longitude[2]*100)/360000.0000+(gnpx_type.longitude[1]/60.0000)+gnpx_type.longitude[0];
	gnpx_type.lon=buf_lon;
	memcpy(gnpx_type.latitude,&buf[16],4);
	buf_lat=(gnpx_type.latitude[3]+gnpx_type.latitude[2]*100)/360000.0000+(gnpx_type.latitude[1]/60.0000)+gnpx_type.latitude[0];
	gnpx_type.lat=buf_lat;
	memcpy(gnpx_type.height,&buf[20],2);
	memcpy(gnpx_type.speed,&buf[22],2);
	memcpy(gnpx_type.direction,&buf[24],2);
	gnpx_type.bd_sum=buf[26];
	gnpx_type.status=buf[27];
	gnpx_type.precision=buf[28];
	memcpy(gnpx_type.deviation,&buf[29],2);
	gnpx_type.crc=buf[31];
}

void RNSS_GNTS(void){
	
}

RNSSStatus_TypeDef RNSS_FKXX_GET(uint8_t *buf){
	if(buf[11]==0x54&&buf[12]==0x58&&buf[13]==0x53&&buf[14]==0x51){
		if(buf[10]==0x00){
			return ICXX;
		}else{
			return ICXX_F;
		}
	}
	return FKXX;
}

void RNSS_GNVS(void){

}


uint8_t Get_CRC(uint8_t *buf, uint8_t len) 
{
    uint8_t i;
    uint8_t checksum = 0;
 
    for (i = 0; i < len; ++i)
    {
        checksum ^= *(buf++);
    }

    return checksum;
}

uint8_t CRC_Check(uint8_t *buf,uint8_t len,uint8_t crc){
	if(Get_CRC(buf,len)==crc){
		return 0x01;
	}else{
		return 0x00;
	}
}
