#include<STC8.h>

//定义TLC5615接口
sbit TLC5615_DIN  = P2^2;
sbit TLC5615_SCLK = P2^1;
sbit TLC5615_CS   = P2^0;

void TLC5615_DAC(unsigned int Data)
{
  unsigned char i;
  Data <<= 6;   //在这里,Data是16位数据,即两个字节,而TLC5615是十位的D/A转换器,所以要把前面高6位移出不要,
     						//只保留十位有效数据.
  TLC5615_CS=0;
  TLC5615_SCLK=0; 
  for (i=0;i<12;i++)
  {
      TLC5615_DIN=(bit)(Data&0x8000);   //在这里有十位有效的Data与0x8000位与,然后达到一位一位的移出,移完10位有效数据后,再移两位0.
      TLC5615_SCLK=1;
      TLC5615_SCLK=0;                 //一个时钟传送一位
		  Data <<=1;    
	}
	TLC5615_CS=1;
  TLC5615_SCLK=0;
}
	

/**************************************
功能描述：ADC口初始化
入口参数：无
返回值：无
***************************************/
void	ADC_Init(void)
{	
	ADC_CONTR|=0x80;      	//开AD转换电源
	ADC_CONTR|=0xF7;	      //选择P1.7作为模拟功能AD使用
	ADC_CONTR&=0xF7;	      //选择P1.7作为模拟功能AD使用
	
	ADCCFG&=0xFC;           //AD转换速度为96个时钟周期转换一次
	ADCCFG|=0x0C;           //AD转换速度为96个时钟周期转换一次
	ADC_CONTR&=0xDF;        //清AD转换完成标志
		
	EADC=0;                 //禁止ADC转换中断
	ADCCFG|=0x20;           //ADC转换结果ADC_RES存高4位，ADC_RESL存低8位
	ADC_CONTR|=0x40;        //启动AD转换，ADC_START=1
}

/**************************************
功能描述：ADC口检测AD转换值函数
入口参数：无
返回值：ADC 12位数据
***************************************/
unsigned int	Get_ADC12bitResult(void)	
{
	unsigned int	ADC_Data=0;
  ADC_CONTR&=0xDF;  	       // 将ADC_FLAG清0
	ADC_CONTR&=0xBF;           //关闭AD转换，ADC_START=0
	                           //12位AD结果的高4位放ADC_RES的低4位，低8位在ADC_RESL
	ADC_Data = ADC_RES;		     //将ADC_RES低4位移到应在的第9位至第12位
	ADC_Data <<= 8;
	ADC_Data |= ADC_RESL;	     //将ADC_RESL的8位移到应在的低8位
	
  ADC_CONTR|=0x40;           //启动AD转换，ADC_START=1
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


