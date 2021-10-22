#include "oled/oled.hpp"
#include "sw_i2c/sw_i2c.h"
#include <stdlib.h>
#include "oled/Inc/oledfont.h" 
#include "oled/Inc/ASCII_CODE_8X16_5X8_VERTICAL.h"
#include "oled/Inc/Chinese_And_Graphic.h"
#include <stdio.h>

//#define SSD1306_I2C_ADDR        (0x3C << 1)
#define SSD1306_I2C_ADDR        0x78

oled::oled()
{
    SW_I2C_initial();
}

oled::oled(I2C_HandleTypeDef *oled_IIC_Handle)
{
    // TODO Auto-generated constructor stub
    this->oled_IIC_Handle = oled_IIC_Handle;
    oled_hw_iic.init(oled_IIC_Handle, SSD1306_I2C_ADDR);
}

void oled::WriteCmd(uint8_t byte)
{
    //oled_hw_iic.writeByte(0x00,byte);
    //oled_iic.Write_Addr_Data(0x78, 0x00, 1, &byte);
    //oled_iic.WriteCmd(byte);
    SW_I2C_WriteControl_8Bit(SW_I2C1,SSD1306_I2C_ADDR,0x00,byte);
}

void oled::WriteDat(uint8_t byte)
{
    //oled_hw_iic.writeByte(0x40,byte);
    //oled_iic.Write_Addr_Data(0x78, 0x40, 1, &byte);
    //oled_iic.WriteDat(byte);
    SW_I2C_WriteControl_8Bit(SW_I2C1,SSD1306_I2C_ADDR,0x40,byte);
}

void oled::init(void)
{   
    oled_iic.update(OLED_S);
    oled_iic.begin();
    HAL_Delay(10);
    WriteCmd(0xae);
    WriteCmd(0x00);
    WriteCmd(0x10);
    WriteCmd(0x40);
    WriteCmd(0x81);
    WriteCmd(0xcf);
    WriteCmd(0xa1);
    WriteCmd(0xc8);
    WriteCmd(0xa6);
    WriteCmd(0xa8);
    WriteCmd(0x3f);
    WriteCmd(0xd3);
    WriteCmd(0x00);
    WriteCmd(0xd5);
    WriteCmd(0x80);
    WriteCmd(0xd9);
    WriteCmd(0xf1);
    WriteCmd(0xda);
    WriteCmd(0x12);
    WriteCmd(0xdb);
    WriteCmd(0x40);
    WriteCmd(0x20);
    WriteCmd(0x02);
    WriteCmd(0x8d);
    WriteCmd(0x14);
    WriteCmd(0xa4);
    WriteCmd(0xa6);
    WriteCmd(0xaf);
}
void oled::SetPos(unsigned char x,unsigned char y)
{
    WriteCmd(0xb0+y);
    WriteCmd(((x&0xf0)>>4)|0x10);
    WriteCmd((x&0x0f)|0x01);
}
//-----------------------------------------------------------------------
//OLED全屏填充
//----------------------------------------------------------------------
void oled::Fill(uint8_t fill_Data)
{
    uint8_t m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd(0xb0+m);
        WriteCmd(0x00);
        WriteCmd(0x10);
        for(n=0;n<128;n++)
        {
            WriteDat(fill_Data);
        }
    }
}
//--------------------------------------------------------------
void oled::CLS(void)
{
    Fill(0x00);
}

void oled::On(void)
{
     WriteCmd(0x8d);
     WriteCmd(0x10);
     WriteCmd(0xae);
}
//-------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 
//------------------------------------------------------------
void oled::Off(void)
{
    WriteCmd(0x8d);
    WriteCmd(0x10);
    WriteCmd(0xae);
}
//-------------------------------------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//------------------------------------------------------------------------------------------------
void oled::ShowStr(unsigned char x,unsigned char y,const char ch[], unsigned char TextSize) //字符串显示函数
{
    unsigned char c=0,i=0,j=0;
    switch(TextSize)
    {	
        case 1:
        {
            while(ch[j]!='\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                SetPos(x,y);
                for(i=0;i<6;i++)
                    WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        }
        break;

        case 2:
        {		
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                SetPos(x,y);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i]);
                SetPos(x,y+1);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i+8]);
                x += 8;
                j++;			
            }
        }
        break;
    }			
}

void oled::ShowNumber(unsigned char x,unsigned char y,int num,unsigned char TextSize)
{
    sprintf(speed_buf,"%d",num);
    ShowStr(x,y,speed_buf, TextSize);
}

/*****************************************************************************/
//全屏清屏
void oled::Clear_Screen(void)
{
    unsigned char i,j;
     for(j=0;j<8;j++)
    {
        Lcd_Address(1+j,1);
        for(i=0;i<128;i++)
        {
            WriteDat(0x00);
        }
    }
}

void oled::Lcd_Address(uint8_t page,uint8_t column)
{
    column=column-1;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
    page=page-1;
    WriteCmd(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
    WriteCmd(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
    WriteCmd(column&0x0f);				//设置列地址的低4位
}

void oled::Display_Frame(void)
{ 
	sprintf(speed_buf,"lens robot");
	ShowStr(26,0,speed_buf, 2);
	sprintf(speed_buf,"wheel_L:");
	ShowStr(0,2,speed_buf, 1);
	sprintf(speed_buf,"wheel_R:");
	ShowStr(0,4,speed_buf, 1);
	//sprintf(speed_buf,"Voltage: V");
	//ShowStr(0,6,speed_buf, 1);
}
