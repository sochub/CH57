/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description 		    : adc����ʾ���������¶ȼ�⡢��ͨ����⡢���ͨ����⡢TouchKey��⡢�жϷ�ʽ������
 *******************************************************************************/

#include "CH57x_common.h"

UINT16 abcBuff[40];
volatile UINT8 adclen;

void DebugInit( void )
{
  GPIOA_SetBits( GPIO_Pin_9 );
  GPIOA_ModeCfg( GPIO_Pin_8, GPIO_ModeIN_PU );
  GPIOA_ModeCfg( GPIO_Pin_9, GPIO_ModeOut_PP_5mA );
  UART1_DefInit();
}

int main()
{
  UINT8 i;
  signed short RoughCalib_Value = 0;    // ADC�ֵ�ƫ��ֵ

  SetSysClock( CLK_SOURCE_PLL_32MHz );

  /* ���ô��ڵ��� */
  DebugInit();
  PRINT( "Start @ChipID=%02X\n", R8_CHIP_ID );

  /* �¶Ȳ���������� ��������У׼ */
  PRINT( "\n1.Temperature sampling...\n" );
  ADC_InterTSSampInit();
  RoughCalib_Value = ADC_DataCalib_Rough(); // ���ڼ���ADC�ڲ�ƫ���¼������ RoughCalib_Value�У�ע�����������Ҫ����Ϊ�з��ű���
  for( i = 0; i < 20; i++ )
  {
    abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // ��������20��
  }
  for( i = 0; i < 20; i++ )
  {
    PRINT( "%d \n", abcBuff[i] );
  }

  /* ��ͨ��������ѡ��adcͨ��0����������Ӧ PA4���ţ� ������У׼���� */
  PRINT( "\n2.Single channel sampling...\n" );
  GPIOA_ModeCfg( GPIO_Pin_4, GPIO_ModeIN_Floating );
  ADC_ExtSingleChSampInit( SampleFreq_3_2, ADC_PGA_0 );

  /*
   ע�⣺����У׼��
   ADC_DataCalib_Rough() ����ǰ�豣֤PA5�������ø�������ģʽ���ⲿû�е�ѹ�źţ���ADC��ʼ��֮����ã�һ�����һ�Σ����Ǹ�����ADCģʽ������ı䣩
   */
  GPIOA_ModeCfg( GPIO_Pin_5, GPIO_ModeIN_Floating );
  RoughCalib_Value = ADC_DataCalib_Rough(); // ���ڼ���ADC�ڲ�ƫ���¼��ȫ�ֱ��� RoughCalib_Value��
  PRINT( "RoughCalib_Value =%d \n", RoughCalib_Value );

  ADC_ChannelCfg( 0 );
  for( i = 0; i < 20; i++ )
  {
    abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // ��������20��
  }
  for( i = 0; i < 20; i++ )
  {
    PRINT( "%d \n", abcBuff[i] ); // ע�⣺����ADC�ڲ�ƫ��Ĵ��ڣ���������ѹ����ѡ���淶Χ���޸�����ʱ�򣬿��ܻ�����������������
  }

  /* ���ͨ��������ѡ��adcͨ��0����������Ӧ PA4(AIN0)��PA12(AIN2) */
  PRINT( "\n3.Diff channel sampling...\n" );
  GPIOA_ModeCfg( GPIO_Pin_4 | GPIO_Pin_12, GPIO_ModeIN_Floating );
  ADC_ExtDiffChSampInit( SampleFreq_3_2, ADC_PGA_0 );
  ADC_ChannelCfg( 0 );
  for( i = 0; i < 20; i++ )
  {
    abcBuff[i] = ADC_ExcutSingleConver();      // ��������20��
  }
  for( i = 0; i < 20; i++ )
  {
    PRINT( "%d \n", abcBuff[i] );
  }

  /* TouchKey������ѡ��adcͨ�� 2 ����������Ӧ PA12 */
  PRINT( "\n4.TouchKey sampling...\n" );
  GPIOA_ModeCfg( GPIO_Pin_12, GPIO_ModeIN_Floating );
  TouchKey_ChSampInit();
  ADC_ChannelCfg( 2 );

  for( i = 0; i < 20; i++ )
  {
    abcBuff[i] = TouchKey_ExcutSingleConver( 0x10, 0 );      // ��������20��
  }
  for( i = 0; i < 20; i++ )
  {
    PRINT( "%d \n", abcBuff[i] );
  }

  /* ��ͨ���������жϷ�ʽ,ѡ��adcͨ��1����������Ӧ PA5���ţ� ��������У׼���� */
  PRINT( "\n5.Single channel sampling in interrupt mode...\n" );
  GPIOA_ModeCfg( GPIO_Pin_5, GPIO_ModeIN_Floating );
  ADC_ExtSingleChSampInit( SampleFreq_3_2, ADC_PGA_0 );
  ADC_ChannelCfg( 1 );
  adclen = 0;
  ADC_ClearITFlag();
  PFIC_EnableIRQ( ADC_IRQn );

  ADC_StartUp();
  while( adclen < 20 )
    ;
  PFIC_DisableIRQ( ADC_IRQn );
  for( i = 0; i < 20; i++ )
  {
    PRINT( "%d \n", abcBuff[i] );
  }

  while( 1 )
    ;
}

__attribute__((interrupt("WCH-Interrupt-fast")))
void ADC_IRQHandler( void )			//adc�жϷ������
{
  if( ADC_GetITStatus() )
  {
    ADC_ClearITFlag();
    if( adclen < 20 )
    {
      abcBuff[adclen] = ADC_ReadConverValue();
      ADC_StartUp();    // ��������жϱ�־��������һ�ֲ���
    }
    adclen++;
  }
}
