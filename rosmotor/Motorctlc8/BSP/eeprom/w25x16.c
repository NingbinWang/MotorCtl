#include "w25x16.h"

#if (W25X16_ENABLE)
#include <stdio.h>

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
	uint8_t tmp = 0;
	HAL_StatusTypeDef ret;
	SPI_FLASH_CS_0;//使能器件
	ret = HAL_SPI_Transmit(W25X16hspi,&TxData,1,0);    //发送读取状态寄存器命令
	if(ret != HAL_OK )
	{
			printf("SPI_ReadWriteByte error \r\n");
	}
	SPI_FLASH_CS_1; //取消片选
	return tmp; //返回通过SPIx最近接收的数据

}

//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t SPI_Flash_ReadSR(void)
{
	uint8_t send_cmd=0;
	uint8_t recv_buf=0;
	HAL_StatusTypeDef ret;
	SPI_FLASH_CS_0;//使能器件
	send_cmd = W25X_ReadStatusReg;
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //发送写取状态寄存器命令
	if(ret != HAL_OK )
	{
			printf("SPI_Flash_ReadSR send cmd error \r\n");
	}
	ret = HAL_SPI_Transmit(W25X16hspi, &recv_buf, 1, 0);  //写入一个字节
	if(ret != HAL_OK )
	{
				printf("SPI_Flash_ReadSR send sr error \r\n");
	}
	SPI_FLASH_CS_1; //取消片选
	return recv_buf;
}
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPI_FLASH_Write_SR(uint8_t sr)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
	SPI_FLASH_CS_0; //使能器件
	send_cmd = W25X_WriteStatusReg;
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //发送写取状态寄存器命令
	if(ret != HAL_OK )
	{
		printf("SPI_FLASH_Write_SR send cmd error \r\n");
	}
	ret = HAL_SPI_Transmit(W25X16hspi, &sr, 1, 0);  //写入一个字节
	if(ret != HAL_OK )
	{
			printf("SPI_FLASH_Write_SR send sr error \r\n");
	}
	SPI_FLASH_CS_1;
}
//SPI_FLASH写使能
//将WEL置位
void SPI_FLASH_Write_Enable(void)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	send_cmd = W25X_WriteEnable;
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //发送写取状态寄存器命令
	if(ret != HAL_OK )
	{
		printf("SPI_FLASH_Write_Enable error \r\n");
	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
}
//SPI_FLASH写禁止
//将WEL清零
void SPI_FLASH_Write_Disable(void)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	send_cmd = W25X_WriteDisable;
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
	if(ret != HAL_OK )
	{
			printf("SPI_FLASH_Write_Disable error \r\n");
	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
}
//读取芯片ID W25X16的ID:0XEF14
uint16_t SPI_Flash_ReadID(void)
{
	uint16_t device_id = 0;
	uint8_t recv_buf[2]={0};
	HAL_StatusTypeDef ret;
	uint8_t send_cmd[4]={W25X_ManufactDeviceID,0x00,0x00,0x00};
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;
	ret = HAL_SPI_Transmit(W25X16hspi, send_cmd,4, 0);   //发送读取ID命令
	if(ret != HAL_OK )
	{
			printf("SPI_Flash_ReadID send id cmd error \r\n");
	}else{

	  ret = HAL_SPI_Receive(W25X16hspi,recv_buf,2,1000);
	  if(ret != HAL_OK )
	  {
			printf("SPI_Flash_ReadID get  id error \r\n");
	   }
	   device_id = (recv_buf[0] << 8) | recv_buf[1];
	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;
	return device_id;
}
//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	  uint8_t send_cmd=0;
	  HAL_StatusTypeDef ret;
	  uint8_t cmd[4];
	  SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	  send_cmd = W25X_ReadData;         //发送读取命令
	  ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
	  if(ret != HAL_OK )
	  {
	  	printf("SPI_Flash_Read error \r\n");
	  }
	  cmd[0] = (uint8_t)(ReadAddr>>16);
	  cmd[1] = (uint8_t)(ReadAddr>>8);
	  cmd[2] = (uint8_t)ReadAddr;
	  ret = HAL_SPI_Transmit(W25X16hspi,cmd,3, 0);
	  if(ret != HAL_OK )
	  {
				printf("SPI_Flash_Read Transmit error \r\n");
	  }
	  ret = HAL_SPI_Receive(W25X16hspi,pBuffer,NumByteToRead,1000);
	  if(ret != HAL_OK )
	  {
			printf("SPI_Flash_Read HAL_SPI_Receive error \r\n");
	   }
	  SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void SPI_Flash_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint8_t send_cmd=0;
	uint8_t cmd[3];
	 HAL_StatusTypeDef ret;
    SPI_FLASH_Write_Enable();                  //SET WEL
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	send_cmd = W25X_PageProgram;      //发送写页命令
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
	if(ret != HAL_OK )
	{
		printf("SPI_Flash_Write_Page error \r\n");
	}
	cmd[0] = (uint8_t)(WriteAddr>>16);
	cmd[1] = (uint8_t)(WriteAddr>>8);
	cmd[2] = (uint8_t)WriteAddr;
	ret = HAL_SPI_Transmit(W25X16hspi, cmd,3, 0);   //发送读取ID命令
	if(ret != HAL_OK )
	{
		printf("SPI_Flash_Write_Page Transmit error \r\n");
	}
	ret = HAL_SPI_Transmit(W25X16hspi, pBuffer,NumByteToWrite, 0);   //发送读取ID命令
	if(ret != HAL_OK )
	{
		printf("SPI_Flash_Write_Page Transmit data error \r\n");
	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
	SPI_Flash_Wait_Busy();					   //等待写入结束
}
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t pageremain;
	pageremain=256-WriteAddr%256; //单页剩余的字节数
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};
}
//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
uint8_t SPI_FLASH_BUF[4096];
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;

	secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16

	secoff=WriteAddr%4096;//在扇区内的偏移

	secremain=4096-secoff;//扇区剩余空间大小

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1)
	{
			SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容

			for(i=0;i<secremain;i++)//校验数据
			{
				if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除
			}
			if(i<secremain)//需要擦除
			{
				SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
				for(i=0;i<secremain;i++)	   //复制
				{
					SPI_FLASH_BUF[i+secoff]=pBuffer[i];
				}
				SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区

			}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
			if(NumByteToWrite==secremain){break;}//写入结束了
			else//写入未结束
			{
				secpos++;//扇区地址增1
				secoff=0;//偏移位置为0

			   	pBuffer+=secremain;  //指针偏移
				WriteAddr+=secremain;//写地址偏移
			   	NumByteToWrite-=secremain;				//字节数递减
				if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
				else secremain=NumByteToWrite;			//下一个扇区可以写完了
			}

		};
}
//擦除整个芯片
//整片擦除时间:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//等待时间超长...
void SPI_Flash_Erase_Chip(void)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_Flash_Wait_Busy();
  	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
  	send_cmd = W25X_ChipErase;        //发送片擦除命令
  	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
  	if(ret != HAL_OK )
  	{
  			printf("SPI_Flash_Erase_Chip error \r\n");
  	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
	SPI_Flash_Wait_Busy();
}
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
	uint8_t cmd[4];
	Dst_Addr*=4096;
	SPI_FLASH_Write_Enable();                  //SET WEL
	 SPI_Flash_Wait_Busy();
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	send_cmd = W25X_SectorErase;      //发送扇区擦除指令
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
	if(ret != HAL_OK )
	{
			printf("SPI_Flash_Erase_Sector cmd error \r\n");
	}
	cmd[0] = (uint8_t)(Dst_Addr>>16);
	cmd[1] = (uint8_t)(Dst_Addr>>8);
	cmd[2] = (uint8_t)Dst_Addr;
	ret = HAL_SPI_Transmit(W25X16hspi, cmd,3, 0);   //发送读取ID命令
	if(ret != HAL_OK )
	{
			printf("SPI_Flash_Erase_Sector Transmit error \r\n");
	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
	SPI_Flash_Wait_Busy();
}

//等待空闲
void SPI_Flash_Wait_Busy(void)
{
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}

//进入掉电模式
void SPI_Flash_PowerDown(void)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
  	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
  	send_cmd = W25X_PowerDown;        //发送掉电命令
  	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
  	if(ret != HAL_OK )
  	{
  		printf("SPI_Flash_PowerDown error \r\n");
  	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
	HAL_Delay(3);                               //等待TPD
}
//唤醒
void SPI_Flash_WAKEUP(void)
{
	uint8_t send_cmd=0;
	HAL_StatusTypeDef ret;
	SPI_FLASH_CS_0; //SPI_FLASH_CS=0;                            //使能器件
	send_cmd = W25X_ReleasePowerDown;   //  send W25X_PowerDown command 0xAB
	ret = HAL_SPI_Transmit(W25X16hspi, &send_cmd, 1, 0);   //
	if(ret != HAL_OK )
	{
	   printf("SPI_Flash_WAKEUP error \r\n");
	}
	SPI_FLASH_CS_1; //SPI_FLASH_CS=1;                            //取消片选
	HAL_Delay(3);                               //等待TRES1
}


#endif
