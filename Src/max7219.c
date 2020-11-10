#include "max7219.h"
#include "light_up.h"
#include "spi.h"



const uint8_t Arrow[5][8]={
{0xE7,0xCE,0x9C,0x39,0x39,0x9C,0xCE,0xE7},
{0xCE,0x9C,0x39,0x73,0x73,0x39,0x9C,0xCE},
{0x9C,0x39,0x73,0xE7,0xE7,0x73,0x39,0x9C},
{0x39,0x73,0xE7,0xCE,0xCE,0xE7,0x73,0x39},
{0x73,0xE7,0xCE,0x9C,0x9C,0xCE,0xE7,0x73},
};
uint8_t Pic_cnt[LED_Number]={0,3,1,4};



uint16_t Combine_uint8_t(uint8_t Data1, uint8_t Data2)
{
	return (Data1<<8)+Data2;
}
void Write_MAX7219(SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *pData, uint16_t Size)
{
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, pData, Size, 10000);
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin, GPIO_PIN_SET);
}
void Write_MAX7219_Multi_Copy(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t Num, uint8_t Cmd, uint8_t Data)	//对Num个LED写入相同数据
{
	uint16_t Send_Data[LED_Number]={0};
	uint16_t Temp=Combine_uint8_t(Cmd,Data);
	for(uint8_t i=0; i<Num; ++i)
		Send_Data[i]=Temp;
	Write_MAX7219(LED_Matrix_SPI,GPIOx,GPIO_Pin,(uint8_t*)&Send_Data, LED_Number);
}
void Write_MAX7219_Single(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ID, uint8_t Cmd, uint8_t Data)		//对第ID个LED写数据
{
	uint16_t Send_Data[LED_Number]={0};
	Send_Data[ID-1]=Combine_uint8_t(Cmd,Data);
	Write_MAX7219(LED_Matrix_SPI,GPIOx,GPIO_Pin,(uint8_t*)&Send_Data, LED_Number);
}
void LED_Matrix_Init(void)
{
	for(uint8_t i=0; i<Wing_Num; ++i)
	{
		Write_MAX7219_Multi_Copy(Wing[i].Matrix_CS_GPIOx ,Wing[i].Matrix_CS_GPIO_Pin,LED_Number, Decode_Mode, 0x00);
		Write_MAX7219_Multi_Copy(Wing[i].Matrix_CS_GPIOx ,Wing[i].Matrix_CS_GPIO_Pin,LED_Number, Intensity, 0x00);
		Write_MAX7219_Multi_Copy(Wing[i].Matrix_CS_GPIOx ,Wing[i].Matrix_CS_GPIO_Pin,LED_Number, Scan_Limit, 0x07);
		Write_MAX7219_Multi_Copy(Wing[i].Matrix_CS_GPIOx ,Wing[i].Matrix_CS_GPIO_Pin,LED_Number, ShutDown, 0x01);
		Write_MAX7219_Multi_Copy(Wing[i].Matrix_CS_GPIOx ,Wing[i].Matrix_CS_GPIO_Pin,LED_Number, Display_Test, 0x00);
	}
}
void Pic_8_Display(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint16_t Send_Data[8][LED_Number]={0};
	for(uint8_t i=0; i<LED_Number; ++i)
		for(uint8_t j=0; j<8; ++j)
			Send_Data[j][i]=Combine_uint8_t( j+1, Arrow[ Pic_cnt[i] ][j] );
	for(uint8_t i=0; i<8; ++i)
		Write_MAX7219(LED_Matrix_SPI,GPIOx,GPIO_Pin,(uint8_t*)&Send_Data[i][0], LED_Number);
	for(uint8_t i=0; i<LED_Number; ++i)
	{
		++Pic_cnt[i];
		if(Pic_cnt[i]>4)
			Pic_cnt[i]=0;
	}
}
void Light_Up_All(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint16_t Send_Data[8][LED_Number]={0};
	for(uint8_t i=0; i<LED_Number; ++i)
		for(uint8_t j=0; j<8; ++j)
			Send_Data[j][i]=Combine_uint8_t( j+1, 0xFF );
	for(uint8_t i=0; i<8; ++i)
		Write_MAX7219(LED_Matrix_SPI,GPIOx,GPIO_Pin,(uint8_t*)&Send_Data[i][0], LED_Number);
}
void OFF_All(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint16_t Send_Data[8][LED_Number]={0};
	for(uint8_t i=0; i<LED_Number; ++i)
		for(uint8_t j=0; j<8; ++j)
			Send_Data[j][i]=Combine_uint8_t( j+1, 0 );
	for(uint8_t i=0; i<8; ++i)
		Write_MAX7219(LED_Matrix_SPI,GPIOx,GPIO_Pin,(uint8_t*)&Send_Data[i][0], LED_Number);
}

