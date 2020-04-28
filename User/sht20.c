#include "sht20.h"

const int16_t POLYNOMIAL = 0x131;

SHT20_INFO sht20_info;

void SHT20_reset(void)
{
	
    I2C_WriteByte(SHT20_ADDRESS, SHT20_SOFT_RESET, (void *)0);
	
}

unsigned char  SHT20_read_user_reg(void)
{
	
    unsigned char val = 0;
	
    I2C_ReadByte(SHT20_ADDRESS, SHT20_READ_REG, &val);
	
    return val;
	
}

//CRC��飬ȷ�����ݵ���ȷ��
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum)
{
    char crc = 0;
    char bit = 0;
    char byteCtr = 0;
	
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
	
    if(crc != checksum)
		return 1;
    else
		return 0;
}

float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
	
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [�C] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
	
    return temperatureC;
	
}

float SHT2x_CalcRH(unsigned short u16sRH)
{
	
    float humidityRH = 0;              // variable for result
	
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
	
    return humidityRH;
	
}

float SHT2x_MeasureHM(unsigned char cmd, unsigned short *pMeasurand)
{
	
    char  checksum = 0;  //checksum
    char  data[2];    //data array for checksum verification
	unsigned char addr = 0;
    unsigned short tmp = 0;
    float t = 0;
	
    addr = SHT20_ADDRESS << 1;
	
	IIC_Start();
	
	IIC_SendByte(addr);
	if(IIC_WaitAck(50000)) //�ȴ�Ӧ��
		return 0.0;
	
	IIC_SendByte(cmd);
	if(IIC_WaitAck(50000)) //�ȴ�Ӧ��
		return 0.0;
	
	IIC_Start();
	
	IIC_SendByte(addr + 1);
	while(IIC_WaitAck(50000)) //�ȴ�Ӧ��
	{
		IIC_Start();
		IIC_SendByte(addr + 1);
	}
	
	delay_ms(70);
	
	data[0] = IIC_RecvByte();
	IIC_Ack();
	data[1] = IIC_RecvByte();
	IIC_Ack();
	
	checksum = IIC_RecvByte();
	IIC_NAck();
	
	IIC_Stop();
	
	SHT2x_CheckCrc(data, 2, checksum);
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
    }
	
    if(pMeasurand)
    {
        *pMeasurand = (unsigned short)t;
    }
	
    return t;
	
}

void Sht20_GetValue(void)
{
	
	unsigned char val = 0;
	
	IIC_SpeedCtl(5);
	
	SHT20_read_user_reg();
	delay_ms(1);
	
 sht20_info.tempreture = SHT2x_MeasureHM(SHT20_Measurement_T_HM, (void *)0);
	if(sht20_info.tempreture>=100){
		sht20_info.tempreture=30;
	}
	delay_ms(70);
	
	sht20_info.humidity = SHT2x_MeasureHM(SHT20_Measurement_RH_HM, (void *)0);
	if(sht20_info.humidity>=100)
	{
		sht20_info.humidity=30;
	}
	delay_ms(25);
	
	SHT20_read_user_reg();
	delay_ms(25);
	
	I2C_WriteByte(SHT20_ADDRESS, SHT20_WRITE_REG, &val);
	delay_ms(100);
	
	SHT20_read_user_reg();
	delay_ms(100);
	
	SHT20_reset();
	delay_ms(100);

}
