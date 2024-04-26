#include<STC8.h>

//����TLC5615�ӿ�
sbit TLC5615_DIN  = P2^2;
sbit TLC5615_SCLK = P2^1;
sbit TLC5615_CS   = P2^0;

void TLC5615_DAC(unsigned int Data)
{
  unsigned char i;
  Data <<= 6;   //������,Data��16λ����,�������ֽ�,��TLC5615��ʮλ��D/Aת����,����Ҫ��ǰ���6λ�Ƴ���Ҫ,
     						//ֻ����ʮλ��Ч����.
  TLC5615_CS=0;
  TLC5615_SCLK=0; 
  for (i=0;i<12;i++)
  {
      TLC5615_DIN=(bit)(Data&0x8000);   //��������ʮλ��Ч��Data��0x8000λ��,Ȼ��ﵽһλһλ���Ƴ�,����10λ��Ч���ݺ�,������λ0.
      TLC5615_SCLK=1;
      TLC5615_SCLK=0;                 //һ��ʱ�Ӵ���һλ
		  Data <<=1;    
	}
	TLC5615_CS=1;
  TLC5615_SCLK=0;
}
	

/**************************************
����������ADC�ڳ�ʼ��
��ڲ�������
����ֵ����
***************************************/
void	ADC_Init(void)
{	
	ADC_CONTR|=0x80;      	//��ADת����Դ
	ADC_CONTR|=0xF7;	      //ѡ��P1.7��Ϊģ�⹦��ADʹ��
	ADC_CONTR&=0xF7;	      //ѡ��P1.7��Ϊģ�⹦��ADʹ��
	
	ADCCFG&=0xFC;           //ADת���ٶ�Ϊ96��ʱ������ת��һ��
	ADCCFG|=0x0C;           //ADת���ٶ�Ϊ96��ʱ������ת��һ��
	ADC_CONTR&=0xDF;        //��ADת����ɱ�־
		
	EADC=0;                 //��ֹADCת���ж�
	ADCCFG|=0x20;           //ADCת�����ADC_RES���4λ��ADC_RESL���8λ
	ADC_CONTR|=0x40;        //����ADת����ADC_START=1
}

/**************************************
����������ADC�ڼ��ADת��ֵ����
��ڲ�������
����ֵ��ADC 12λ����
***************************************/
unsigned int	Get_ADC12bitResult(void)	
{
	unsigned int	ADC_Data=0;
  ADC_CONTR&=0xDF;  	       // ��ADC_FLAG��0
	ADC_CONTR&=0xBF;           //�ر�ADת����ADC_START=0
	                           //12λAD����ĸ�4λ��ADC_RES�ĵ�4λ����8λ��ADC_RESL
	ADC_Data = ADC_RES;		     //��ADC_RES��4λ�Ƶ�Ӧ�ڵĵ�9λ����12λ
	ADC_Data <<= 8;
	ADC_Data |= ADC_RESL;	     //��ADC_RESL��8λ�Ƶ�Ӧ�ڵĵ�8λ
	
  ADC_CONTR|=0x40;           //����ADת����ADC_START=1
	return	ADC_Data;	
}

void main()
{
  unsigned int ADC_Data;
 	ADC_Init();
	P2M1 = 0x00;
	P2M0 = 0x07; 
	while(1)
	{
	     ADC_Data = Get_ADC12bitResult()>>4;	     
		   TLC5615_DAC(ADC_Data);  
	}
}


