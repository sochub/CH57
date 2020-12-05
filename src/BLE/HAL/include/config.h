/********************************** (C) COPYRIGHT *******************************
 * File Name          : CONFIG.h
 * Author             : WCH
 * Version            : V1.10
 * Date               : 2019/11/05
 * Description        : ����˵����Ĭ��ֵ�������ڹ����������Ԥ�������޸ĵ�ǰֵ
 *******************************************************************************/

/******************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#define	ID_CH573							0x73

#define CHIP_ID								ID_CH573

#ifdef CH57xBLE_ROM
#include "CH57xBLE_ROM.H"
#else
#include "CH57xBLE_LIB.H"
#endif
/*********************************************************************
 ��MAC��
 BLE_MAC											- �Ƿ��Զ�������Mac��ַ ( Ĭ��:FALSE - ʹ��оƬMac��ַ )����Ҫ��main.c�޸�Mac��ַ����

 ��SLEEP��
 HAL_SLEEP   									- �Ƿ���˯�߹��� ( Ĭ��:FALSE )
 WAKE_UP_RTC_MAX_TIME					- ˯�߻�������ʱ�䣬���ݲ�ͬ˯������ȡֵ�ɷ�Ϊ�� ˯��ģʽ/�µ�ģʽ       -	65(Ĭ��)
                                                                                                                                                                               ��ͣģʽ					-	65
                                                                                                                                                                               ����ģʽ					-	5
 
 ��TEMPERATION��
 TEM_SAMPLE										- �Ƿ�򿪸����¶ȱ仯У׼�Ĺ��ܣ�����У׼��ʱС��10ms( Ĭ��:TRUE )
 
 ��CALIBRATION��
 BLE_CALIBRATION_ENABLE				- �Ƿ�򿪶�ʱУ׼�Ĺ��ܣ�����У׼��ʱС��10ms( Ĭ��:TRUE )
 BLE_CALIBRATION_PERIOD				- ��ʱУ׼�����ڣ���λms( Ĭ��:120000 ) 
 
 ��SNV��
 BLE_SNV    				          - �Ƿ���SNV���ܣ����ڴ������Ϣ( Ĭ��:TRUE )
 BLE_SNV_ADDR    					    - SNV��Ϣ�����ַ��ʹ��data flash���( Ĭ��:0x77C00 )

 ��RTC��
 CLK_OSC32K										- RTCʱ��ѡ�������������ɫ����ʹ���ⲿ32K( Ĭ��:0 �ⲿ(32768Hz)��1���ڲ�(32000Hz)��2���ڲ�(32768Hz) )

 ��MEMORY��
 BLE_MEMHEAP_SIZE  						- ����Э��ջʹ�õ�RAM��С����С��6K ( Ĭ��:(1024*5) )

 ��DATA��
 BLE_BUFF_MAX_LEN							- ����������������( Ĭ��:27 (ATT_MTU=23)��ȡֵ��Χ[27~251] )
 BLE_BUFF_NUM									- ����������İ�����( Ĭ��:5 )
 BLE_TX_NUM_EVENT							- ���������¼������Է����ٸ����ݰ�( Ĭ��:1 )
 BLE_TX_POWER									- ���书��( Ĭ��:LL_TX_POWEER_0_DBM (0dBm) )
 
 ��MULTICONN��
 PERIPHERAL_MAX_CONNECTION	  - ����ͬʱ�����ٴӻ���ɫ( Ĭ��:1 )
 CENTRAL_MAX_CONNECTION				- ����ͬʱ������������ɫ( Ĭ��:3 )
 **********************************************************************/

/*********************************************************************
 * Ĭ������ֵ
 */
#ifndef BLE_MAC
#define BLE_MAC											FALSE
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP										FALSE
#endif
#ifndef WAKE_UP_RTC_MAX_TIME
#define WAKE_UP_RTC_MAX_TIME				65
#endif
#ifndef HAL_KEY
#define HAL_KEY											FALSE
#endif
#ifndef HAL_LED
#define HAL_LED											FALSE	
#endif
#ifndef TEM_SAMPLE
#define TEM_SAMPLE									TRUE
#endif
#ifndef BLE_CALIBRATION_ENABLE
#define BLE_CALIBRATION_ENABLE			TRUE
#endif
#ifndef BLE_CALIBRATION_PERIOD
#define BLE_CALIBRATION_PERIOD			120000
#endif
#ifndef BLE_SNV
#define BLE_SNV											TRUE
#endif
#ifndef BLE_SNV_ADDR
#define BLE_SNV_ADDR								0x77000-FLASH_ROM_MAX_SIZE
#endif
#ifndef CLK_OSC32K
#define CLK_OSC32K									0							// ��������ڹ����������Ԥ�������޸ģ������������ɫ����ʹ���ⲿ32K
#endif
#ifndef BLE_MEMHEAP_SIZE
#define BLE_MEMHEAP_SIZE						(1024*5)
#endif
#ifndef BLE_BUFF_MAX_LEN
#define BLE_BUFF_MAX_LEN						27
#endif
#ifndef BLE_BUFF_NUM
#define BLE_BUFF_NUM								5
#endif
#ifndef BLE_TX_NUM_EVENT
#define BLE_TX_NUM_EVENT						1
#endif
#ifndef BLE_TX_POWER
#define BLE_TX_POWER								LL_TX_POWEER_0_DBM
#endif
#ifndef PERIPHERAL_MAX_CONNECTION
#define PERIPHERAL_MAX_CONNECTION		1
#endif
#ifndef CENTRAL_MAX_CONNECTION
#define CENTRAL_MAX_CONNECTION			3
#endif

extern u32 MEM_BUF[BLE_MEMHEAP_SIZE / 4];
extern u8C MacAddr[6];

#endif

/******************************* endfile @ config ******************************/