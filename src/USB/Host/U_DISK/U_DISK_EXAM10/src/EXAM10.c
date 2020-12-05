/********************************** (C) COPYRIGHT *******************************
 * File Name          : EXAM10.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/11
 * Description        :
 C���Ե�U���ļ��������޸��ļ����ԣ�ɾ���ļ��Ȳ���
 ֧��: FAT12/FAT16/FAT32
 ע����� CHRV3UFI.LIB/USBHOST.C/DEBUG.C
 *******************************************************************************/

/** ��ʹ��U���ļ�ϵͳ�⣬��Ҫ�ڹ�������Ԥ�������޸� DISK_LIB_ENABLE=0        */
/** U�̹���USBhub���棬��Ҫ�ڹ�������Ԥ�������޸� DISK_WITHOUT_USB_HUB=0  */

#include "CH57x_common.h"
#include "CHRV3UFI.H"

__attribute__((aligned(4)))  UINT8 RxBuffer[MAX_PACKET_SIZE];    // IN, must even address
__attribute__((aligned(4)))  UINT8 TxBuffer[MAX_PACKET_SIZE];    // OUT, must even address

UINT8 buf[100];                                  //���ȿ��Ը���Ӧ���Լ�ָ��

/* ������״̬,�����������ʾ������벢ͣ�� */
void mStopIfError( UINT8 iError )
{
  if ( iError == ERR_SUCCESS )
  {
    return; /* �����ɹ� */
  }
  printf( "Error: %02X\n", ( UINT16 ) iError ); /* ��ʾ���� */
  /* ���������,Ӧ�÷����������Լ�CHRV3DiskStatus״̬,�������CHRV3DiskReady��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
   ���������Ĵ�������:
   1������һ��CHRV3DiskReady,�ɹ����������,����Open,Read/Write��
   2�����CHRV3DiskReady���ɹ�,��ôǿ�н���ͷ��ʼ����(�ȴ�U�����ӣ�CH554DiskReady��) */
  while( 1 )
  {
  }
}

int main()
{
  UINT8 s, c, i;

  SetSysClock( CLK_SOURCE_PLL_32MHz );

  GPIOA_SetBits( GPIO_Pin_9 );
  GPIOA_ModeCfg( GPIO_Pin_8, GPIO_ModeIN_PU );
  GPIOA_ModeCfg( GPIO_Pin_9, GPIO_ModeOut_PP_5mA );
  UART1_DefInit();
  PRINT( "Start @ChipID=%02X \n", R8_CHIP_ID );

  pHOST_RX_RAM_Addr = RxBuffer;
  pHOST_TX_RAM_Addr = TxBuffer;
  USB_HostInit();
  CHRV3LibInit();                           //��ʼ��U�̳������֧��U���ļ�

  FoundNewDev = 0;
  while( 1 )
  {
    s = ERR_SUCCESS;
    if ( R8_USB_INT_FG & RB_UIF_DETECT )    // �����USB��������ж�����
    {
      R8_USB_INT_FG = RB_UIF_DETECT;                // �������жϱ�־
      s = AnalyzeRootHub();                                          // ����ROOT-HUB״̬
      if ( s == ERR_USB_CONNECT )
        FoundNewDev = 1;
    }
    
    if ( FoundNewDev || s == ERR_USB_CONNECT )                         // ���µ�USB�豸����
    {
      FoundNewDev = 0;
      mDelaymS( 200 );                                               // ����USB�豸�ղ�����δ�ȶ�,�ʵȴ�USB�豸���ٺ���,������ζ���
      s = InitRootDevice();    // ��ʼ��USB�豸
      if ( s == ERR_SUCCESS )
      {
        // U�̲������̣�USB���߸�λ��U�����ӡ���ȡ�豸������������USB��ַ����ѡ�Ļ�ȡ������������֮�󵽴�˴�����CHRV3�ӳ���������ɺ�������
        CHRV3DiskStatus = DISK_USB_ADDR;
        for ( i = 0; i != 10; i++ )
        {
          printf( "Wait DiskReady\n" );
          s = CHRV3DiskReady();                                 //�ȴ�U��׼����
          if ( s == ERR_SUCCESS )
          {
            break;
          }
          else
          {
            printf( "%02x\n", ( UINT16 ) s );
          }
          mDelaymS( 50 );
        }
        
        if ( CHRV3DiskStatus >= DISK_MOUNTED )
        {
          //�����ļ���ʾ
          printf( "Create\n" );
          strcpy( ( PCHAR ) mCmdParam.Create.mPathName, "/NEWFILE.TXT" ); /* ���ļ���,�ڸ�Ŀ¼��,�����ļ��� */
          s = CHRV3FileCreate(); /* �½��ļ�����,����ļ��Ѿ���������ɾ�������½� */
          mStopIfError( s );
          printf( "ByteWrite\n" );
          //ʵ��Ӧ���ж�д���ݳ��ȺͶ��建���������Ƿ������������ڻ�������������Ҫ���д��
          i = sprintf( ( PCHAR ) buf, "Note: \xd\xa������������ֽ�Ϊ��λ����U���ļ���д,RV3����ʾ���ܡ�\xd\xa" ); /*��ʾ */
          for ( c = 0; c < 10; c++ )
          {
            mCmdParam.ByteWrite.mByteCount = i; /* ָ������д����ֽ��� */
            mCmdParam.ByteWrite.mByteBuffer = buf; /* ָ�򻺳��� */
            s = CHRV3ByteWrite(); /* ���ֽ�Ϊ��λ���ļ�д������ */
            mStopIfError( s );
            printf( "�ɹ�д�� %02X��\n", ( UINT16 ) c );
          }
          //��ʾ�޸��ļ�����
          printf( "Modify\n" );
          mCmdParam.Modify.mFileAttr = 0xff;    //�������: �µ��ļ�����,Ϊ0FFH���޸�
          mCmdParam.Modify.mFileTime = 0xffff;    //�������: �µ��ļ�ʱ��,Ϊ0FFFFH���޸�,ʹ���½��ļ�������Ĭ��ʱ��
          mCmdParam.Modify.mFileDate = MAKE_FILE_DATE( 2015, 5, 18 );    //�������: �µ��ļ�����: 2015.05.18
          mCmdParam.Modify.mFileSize = 0xffffffff;    // �������: �µ��ļ�����,���ֽ�Ϊ��λд�ļ�Ӧ���ɳ����ر��ļ�ʱ�Զ����³���,���Դ˴����޸�
          i = CHRV3FileModify();    //�޸ĵ�ǰ�ļ�����Ϣ,�޸�����
          mStopIfError( i );
          printf( "Close\n" );
          mCmdParam.Close.mUpdateLen = 1; /* �Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ����� */
          i = CHRV3FileClose();
          mStopIfError( i );

//          strcpy( ( PCHAR ) mCmdParam.Create.mPathName, "/NEWFILE.TXT" ); /* ���ļ���,�ڸ�Ŀ¼��,�����ļ��� */
//          s = CHRV3FileOpen(); /* �½��ļ�����,����ļ��Ѿ���������ɾ�������½� */
//          mStopIfError( s );

          /* ɾ��ĳ�ļ� */
          printf( "Erase\n" );
          strcpy( ( PCHAR ) mCmdParam.Create.mPathName, "/OLD" );    //����ɾ�����ļ���,�ڸ�Ŀ¼��
          i = CHRV3FileErase();    //ɾ���ļ����ر�
          if ( i != ERR_SUCCESS )
            printf( "Error: %02X\n", ( UINT16 ) i );    //��ʾ����
        }
      }
    }
    mDelaymS( 100 );    // ģ�ⵥƬ����������
    SetUsbSpeed( 1 );    // Ĭ��Ϊȫ��
  }
}
