/*
 * cc1101.c
 * cc1101����ģ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.19
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.03.29
 */

#include "system_define.h"
#include "hardware.h"		//io�˿ں궨��

#include "stm32_delay.h"

#include "stm32_spi.h"
#include "error.h"

#include "cc1101.h"

//************************���Ŷ���***********************//
//CS����ѡ����������λ��spi.c�ļ�
#define CC1101_CS   { PCout(4) = 0; }
#define CC1101_CS_DIS { PCout(4) = 1; }

//GDO����ѡ����������λ�� cc1101_init_gdo()����
#define CC1101_GDO0 PAin(0)
//#define CC1101_GDO2 PAin(2)  //��ʹ��

//SPI1 SPI2ѡ��
#define CC1101_SPI 1  // 1 -> SPI1, 2-> SPI2

#define TIMEOUT_CC1101 5000

#if (CC1101_SPI == 1)

#define SPI_RW spi1_rw
#define CC1101_SCK  PAout(5)
#define CC1101_MOSI PAout(7)
#define CC1101_MISO PAin(6)

#elif (CC1101_SPI == 2)

#define SPI_RW spi2_rw
#define CC1101_SCK  PBout(13)
#define CC1101_MOSI PBout(15)
#define CC1101_MISO PBin(14)

#endif


#define CC1101_BURST_BIT 0x40
#define CC1101_READ_BIT  0x80
#define CC1101_WRITE_BIT 0x00

//************************��������***********************//


//***************���๦�ʲ������ÿ���ϸ�ο�DATACC1100Ӣ���ĵ��е�48-49ҳ�Ĳ�����******************
//const uint8_t PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04};  //-30dBm   ������С
const uint8_t PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm
//const uint8_t PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};	//10dBm     �������

//************************�Ĵ�����ȡ***********************//
void cc1101_wait_miso(void)
{
	int32_t timeout;
	timeout = TIMEOUT_CC1101;
	while(CC1101_MISO)
	{
		timeout--;
		if(timeout <= 0)
		{
			error_alarm(ERROR_CODE_EX_CC1101 | ERROR_CODE_TIME_OUT);
			break;
		}
		delayus(1);
	}
}

uint8_t cc1101_read_reg(uint8_t regAddr)
{
	uint8_t ret;
	CC1101_CS
	
	cc1101_wait_miso();
	SPI_RW(CC1101_READ_BIT | regAddr);
	ret = SPI_RW(0);

	CC1101_CS_DIS
	
	return ret;
}

void cc1101_read_reg_burst(uint8_t regAddr, uint8_t *buf, uint8_t len)
{
	uint8_t i;
	CC1101_CS
	cc1101_wait_miso();
	SPI_RW(CC1101_READ_BIT | CC1101_BURST_BIT | regAddr);
	for(i = 0; i < len; i++)
	{
		buf[i] = SPI_RW(0);
	}
	CC1101_CS_DIS
}

void cc1101_write_reg(uint8_t regAddr, uint8_t dat)
{
	CC1101_CS
	cc1101_wait_miso();
	SPI_RW(CC1101_WRITE_BIT | regAddr);
	SPI_RW(dat);
	CC1101_CS_DIS
}

void cc1101_write_reg_burst(uint8_t regAddr, const uint8_t *dat, uint8_t len)
{
	uint8_t i;
	
	CC1101_CS
	cc1101_wait_miso();
	SPI_RW(CC1101_WRITE_BIT | CC1101_BURST_BIT | regAddr);
	for(i = 0; i < len; i++)
	{
		SPI_RW(dat[i]);
	}
	CC1101_CS_DIS
}

void cc1101_write_cmd(uint8_t dat)
{
	CC1101_CS
	cc1101_wait_miso();
	SPI_RW(dat);
	CC1101_CS_DIS
}

uint8_t cc1101_read_cmd(uint8_t regAddr)
{
	uint8_t ret;
	CC1101_CS
	//while (CC1101_GDO0);
	cc1101_wait_miso();
	SPI_RW(CC1101_READ_BIT | CC1101_BURST_BIT | regAddr);
	ret = SPI_RW(0);
	
	CC1101_CS_DIS
	return ret;
}

//************************���ſ���***********************//
void cc1101_pin_mode_output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#if (CC1101_SPI == 1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;  //SPI1 SCK MOSI
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif (CC1101_SPI == 2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;  //SPI2 SCK MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}

void cc1101_pin_mode_spi(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
#if (CC1101_SPI == 1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;  //SPI1 SCK MOSI
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif (CC1101_SPI == 2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;  //SPI2 SCK MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}



//************************��ʼ��***********************//
void cc1101_init_gdo(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void cc1101_reset_when_power_up(void) 
{
	cc1101_pin_mode_output();
	CC1101_CS_DIS
	delayus(5);
	CC1101_CS
	delayus(5);
	CC1101_CS_DIS
	delayus(80);
	CC1101_CS
	while(CC1101_MISO);
	CC1101_CS_DIS
	cc1101_pin_mode_spi();
	delayus(500);
	
	cc1101_reset();
	delayus(1000);
}

void cc1101_write_rf_settings(void) 
{
	//
	// Rf settings for CC1101
	//
	cc1101_write_reg(CC1101_IOCFG0,0x06);  //GDO0 
	cc1101_write_reg(CC1101_PKTCTRL0,0x05);//Packet Automation Control
	cc1101_write_reg(CC1101_FSCTRL1,0x0C); //Frequency Synthesizer Control
	cc1101_write_reg(CC1101_FREQ2,0x10);   //Frequency Control Word, High Byte
	cc1101_write_reg(CC1101_FREQ1,0xB1);   //Frequency Control Word, Middle Byte
	cc1101_write_reg(CC1101_FREQ0,0x3B);   //Frequency Control Word, Low Byte
	cc1101_write_reg(CC1101_MDMCFG4,0x2D); //Modem Configuration
	cc1101_write_reg(CC1101_MDMCFG3,0x3B); //Modem Configuration
	cc1101_write_reg(CC1101_MDMCFG2,0x13); //Modem Configuration
	cc1101_write_reg(CC1101_DEVIATN,0x62); //Modem Deviation Setting
	cc1101_write_reg(CC1101_MCSM0,0x18);   //Main Radio Control State Machine Configuration
	cc1101_write_reg(CC1101_FOCCFG,0x1D);  //Frequency Offset Compensation Configuration
	cc1101_write_reg(CC1101_BSCFG,0x1C);   //Bit Synchronization Configuration
	cc1101_write_reg(CC1101_AGCCTRL2,0xC7);//AGC Control
	cc1101_write_reg(CC1101_AGCCTRL1,0x00);//AGC Control
	cc1101_write_reg(CC1101_AGCCTRL0,0xB0);//AGC Control
	cc1101_write_reg(CC1101_WORCTRL,0xFB); //Wake On Radio Control
	cc1101_write_reg(CC1101_FREND1,0xB6);  //Front End RX Configuration
	cc1101_write_reg(CC1101_FSCAL3,0xEA);  //Frequency Synthesizer Calibration
	cc1101_write_reg(CC1101_FSCAL2,0x2A);  //Frequency Synthesizer Calibration
	cc1101_write_reg(CC1101_FSCAL1,0x00);  //Frequency Synthesizer Calibration
	cc1101_write_reg(CC1101_FSCAL0,0x1F);  //Frequency Synthesizer Calibration
	cc1101_write_reg(CC1101_TEST0,0x09);   //Various Test Settings
}

void cc1101_write_pa_tabel(void)
{
	cc1101_write_reg_burst(CC1101_PATABLE, PaTabel, 8);
}

void cc1101_init(void) 
{
	cc1101_init_gdo();
	cc1101_reset_when_power_up();
	cc1101_write_rf_settings();
	cc1101_write_pa_tabel();
}

// return 1: Ӳ��æ
// return 0: ����
uint8_t cc1101_busy(void)
{
	if(CC1101_GDO0) return 1;
	return 0;
}

#if (CC1101_INTER_MODE == 1)  //**�ж�ģʽ**//

void cc1101_inter_send(const uint8_t *txBuffer, uint8_t size)
{
	cc1101_write_reg(CC1101_TXFIFO, size);
	cc1101_write_reg_burst(CC1101_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����
	cc1101_write_cmd(CC1101_STX);		//���뷢��ģʽ��������
}

#else                         //**���ж�ģʽ**//

void cc1101_wait_gdo0(void)
{
	int32_t timeout;
	timeout = TIMEOUT_CC1101;
	while(CC1101_GDO0)
	{
		timeout--;
		if(timeout <= 0)
		{
			error_alarm(ERROR_CODE_EX_CC1101 | ERROR_CODE_TIME_OUT);
		}
		delayus(1);
	}
}

void cc1101_wait_gdo0_n(void)
{
	int32_t timeout;
	timeout = TIMEOUT_CC1101;
	while(!CC1101_GDO0)
	{
		timeout--;
		if(timeout <= 0)
		{
			error_alarm(ERROR_CODE_EX_CC1101 | ERROR_CODE_TIME_OUT);
		}
		delayus(1);
	}
}

void cc1101_single_recv_start(void)
{
	cc1101_write_cmd(CC1101_SRX);		//�������״̬
	// Wait for GDO0 to be set -> sync received
	cc1101_wait_gdo0_n();
}

uint8_t cc1101_single_recv_is_ok(void)
{
	if(CC1101_GDO0) return 0;  //δ�������
	else return 1;             //�������
}

void cc1101_single_send(const uint8_t *txBuffer, uint8_t size) 
{
	cc1101_write_reg(CC1101_TXFIFO, size);
	cc1101_write_reg_burst(CC1101_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����
	cc1101_write_cmd(CC1101_STX);		//���뷢��ģʽ��������
	// Wait for GDO0 to be set -> sync transmitted
	cc1101_wait_gdo0_n();

	// Wait for GDO0 to be cleared -> end of packet
	cc1101_wait_gdo0();
	cc1101_write_cmd(CC1101_SFTX);
}

#endif

//************************��ȡBuffer***********************//
uint8_t cc1101_recv_get_data(uint8_t *rxBuffer, uint8_t size_max, uint32_t *err)
{
	uint8_t status[2];
	uint8_t retValue;
	uint8_t packetLength;  //64�ֽ����
	retValue = 0;
	if (0 == (cc1101_read_cmd(CC1101_RXBYTES) & CC1101_BYTES_IN_RXFIFO)) //������յ��ֽ���Ϊ0
	{
		*err = 1;
	}else{
		packetLength = cc1101_read_reg(CC1101_RXFIFO);  //������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
		if (packetLength > size_max)  //size_maxС��packetLength
		{
			*err = 3;
		}else{
			cc1101_read_reg_burst(CC1101_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
			cc1101_read_reg_burst(CC1101_RXFIFO, status, 2); 	//����CRCУ��λ
			if(0 == (status[1] & CC1101_CRC_OK))  //У��ʧ��
			{
				*err = 2;
			}else{
				*err = 0;
				retValue = packetLength;
			}
		}
	}
	return retValue;
}
