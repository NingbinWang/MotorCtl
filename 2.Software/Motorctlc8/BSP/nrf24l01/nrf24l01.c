#include "nrf24l01conf.h"

#if (NRF24L01_ENBALE)



u8 const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
u8 const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址




//初始化NRF IO配置
void NRF_IO_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	  SPI_InitTypeDef  SPI_InitStructure;

	  RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOD, ENABLE );//PORT时钟使能

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;//PD5 NEF CS  PD6 NRF CE
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO


	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PD7 	   NEF IRQ
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO


		SPI_Cmd(SPI1, DISABLE); // SPI外设不使能			  NRF的SPI要特殊配置一下

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主机
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//发送接收8位帧结构

		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//时钟悬空低
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第1个时钟沿

		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由软件控制
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为16
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据传输从MSB位开始
		SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
		SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

		SPI_Cmd(SPI1, ENABLE); //使能SPI外设

		CEN=0; 			//使能24L01
		CSN=1;			//SPI片选取消
}





//从NRF读取一个字节数据
//reg 寄存器地址
u8 SPI_Read(u8 reg)
{
	u8 reg_val;

	CSN = 0;                //片选使能
    SPI1_ReadWriteByte(reg);
	reg_val = SPI1_ReadWriteByte(0xff);    // 读取数据到reg_val
	CSN = 1;                // 取消片选

	return(reg_val);        // 返回读取的数据
}



//向NRF写入一个字节数据
//reg 寄存器地址  value 要写入的数据
u8 SPI_RW_Reg(u8 reg, u8 value)
{
	u8 status;

	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI1_ReadWriteByte(reg);
    SPI1_ReadWriteByte(value);
	CSN = 1;                   // CSN high again

	return(status);            // return nRF24L01 status uchar
}



//从NRF读取多个字节数据
//reg 寄存器地址  *pBuf 读取数据存储指针  uchars 读取的字节个数
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 uchars)
{
	u8 status,uchar_ctr;

	CSN = 0;                    		// Set CSN low, init SPI tranaction

	status = SPI1_ReadWriteByte(reg);
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)		  //循环 uchars次
    pBuf[uchar_ctr] = SPI1_ReadWriteByte(0xff); 				  //分别将	 SPI_RW(0)读出的数据地址 放入数组中

	CSN = 1;

	return status;                    // return nRF24L01 status uchar
}


//向NRF写入多个字节数据
//reg 寄存器地址  *pBuf 要写入的数据  uchars 写入的字节个数
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 uchars)
{
	u8 status,uchar_ctr;

	CSN = 0;            //SPI使能

	  status = SPI1_ReadWriteByte(reg);
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //	 根据数据个数循环
		SPI1_ReadWriteByte(*pBuf++);						//将数组的数据 依次写入

	CSN = 1;           //关闭SPI
	return(status);    //
}


//检测24L01是否存在
//返回值:0，成功;1，失败
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
    SPI1_SetSpeed(SPI_BaudRatePrescaler_8);       //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
	SPI_Write_Buf(WRITE_REG_NRF+TX_ADDR,buf,5);   //写入5个字节的地址.
	SPI_Read_Buf(TX_ADDR,buf,5); //读出写入的地址
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
	if(i!=5)return 1;//检测24L01错误
	return 0;		 //检测到24L01
}




//NRF接收数据函数
//rx_buf  数据缓存区
//该函数检测NRF状态寄存器状态 当有中断立即接收数据到rx_buf缓存区
u8 nRF24L01_RxPacket(u8 *rx_buf)
{
    u8 sta;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）

	sta=SPI_Read(STATUS);	    // 读取状态寄存器来判断数据接收状况
	SPI_RW_Reg(WRITE_REG_NRF+STATUS,sta);   //清中断 （接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志）
	if(sta&RX_OK)				// 判断是否接收到数据
	{

		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		SPI_RW_Reg(FLUSH_RX,0xff);

		return 0;
	}

	return 1;
}


//将缓存区tx_buf中的数据发送出去
//tx_buf  要发送的数据缓存区
u8 nRF24L01_TxPacket(u8 *tx_buf)
{
    u8 st;
	u16 cnt;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
	CEN=0;			//StandBy I模式

	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据

	CEN=1;		 //置高CE，激发数据发送
	while(IRQ!=0)		   //等待发送完成
	{
	  if(++cnt >= 60000)break;
	}
	st=	SPI_Read(STATUS);			  //读NRF寄存器状态
	SPI_RW_Reg(WRITE_REG_NRF+STATUS,st);  //清中断

	if(st&MAX_TX)//达到最大重发次数
	{
		SPI_RW_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器
		return MAX_TX;
	}

	if(st&TX_OK)				 //发送成功
	{
	return TX_OK;
	}
	return 0xff;

}


//NRF24L01初始化
//m 1 发送模式   0 接收模式
void RX_TX_Mode(u8 m)	        //接收 or 发射 模式 初始化
{
 	CEN=0;    // chip enable
 	CSN=1;   // Spi disable
	SPI_Write_Buf(WRITE_REG_NRF + TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址
	SPI_Write_Buf(WRITE_REG_NRF + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG_NRF + EN_AA, 0x01);      //  频道0自动	ACK应答允许
	SPI_RW_Reg(WRITE_REG_NRF + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21
	SPI_RW_Reg(WRITE_REG_NRF + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG_NRF + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG_NRF + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB

	if(m==1) SPI_RW_Reg(WRITE_REG_NRF + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	else if(m==0) SPI_RW_Reg(WRITE_REG_NRF + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收

	CEN=1;
}
#endif

