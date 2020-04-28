#ifndef __BD_RNSS_H_
#define __BD_RNSS_H_

#include "main.h"

extern uint8_t icjc[12];
extern uint8_t xtzj[13];
extern uint8_t sjsc[13];
extern uint8_t gnps[13];



typedef enum{
	OTHER = 0x00,
	ICXX = 0x01,
	ZJXX = 0x02,
	SJXX = 0x03,
	GNPX = 0x04,
	FKXX = 0x05,
	ICXX_F=0x06,
	TXXX = 0x07
}RNSSStatus_TypeDef;

typedef struct {
	uint8_t user_address[3];
	uint8_t frame;
	uint8_t broadcast_id[3];
	uint8_t user_feature;
	uint8_t server_time[2];
	uint8_t send_level;
	uint8_t enc_flag;
	uint8_t under_sum[2];
	uint8_t crc;
}ICXX_TypeDef;

typedef struct {
	uint8_t ic_status;
	uint8_t broad_status;
	uint8_t battery_status;
	uint8_t join_status;
	uint8_t one_level;
	uint8_t two_level;
	uint8_t three_level;
	uint8_t four_level;
	uint8_t five_level;
	uint8_t six_level;
	uint8_t crc;
}ZJXX_TypeDef;

typedef struct{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t crc;
}SJXX_TypeDef;

typedef struct{
	uint8_t longitude[4];
	uint8_t latitude[4];
	double lat;
	double lon;
	uint8_t height[2];
	uint8_t speed[2];
	uint8_t direction[2];
	uint8_t bd_sum;
	uint8_t status;
	uint8_t precision;
	uint8_t deviation[2];
	uint8_t crc;
}GNPX_TypeDef;

typedef struct{
	uint8_t flag;
	uint8_t cmd[4];
	uint8_t crc;
}FKXX_TypeDef;


extern ICXX_TypeDef icxx_type;
extern ZJXX_TypeDef zjxx_type;
extern SJXX_TypeDef sjxx_type;
extern GNPX_TypeDef gnpx_type;
extern FKXX_TypeDef fkxx_type;

extern uint8_t GNPX_BUF[32];

void RNSS_CMD(uint8_t *cmd,uint16_t size);
void RNSS_ICJC_GET(uint8_t *buf);

void RNSS_XTZJ_GET(uint8_t *buf);

void RNSS_SJSC_GET(uint8_t *buf);

void RNSS_GNPS_GET(uint8_t *buf);

RNSSStatus_TypeDef RNSS_FKXX_GET(uint8_t *buf);

RNSSStatus_TypeDef RNSS_Cmp(uint8_t *data);
uint8_t Get_CRC(uint8_t *buf, uint8_t len);
uint8_t CRC_Check(uint8_t *buf,uint8_t len,uint8_t crc);



#endif
