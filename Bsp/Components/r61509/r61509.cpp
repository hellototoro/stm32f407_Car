/*
 * r61509.cpp
 *
 *  Created on: Oct 22, 2021
 *      Author: huang
 */

#include <r61509/R61509.hpp>

R61509::R61509() {
    // TODO Auto-generated constructor stub
    dir = MyDrivers::displayDir::vertical;
    //mode = MyDrivers::scanMode::R2L_D2U;
}

R61509::~R61509() {
    // TODO Auto-generated destructor stub
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void R61509::displayOn(void)
{
    LCD_IO_WriteReg(0x0029);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void R61509::displayOff(void)
{
    LCD_IO_WriteReg(0x0011);
}

/**
  * @brief  Prepare to write GRAM   
  * @param  None
  * @retval None
  */
void R61509::prepareWrite(void)
{
    LCD_IO_WriteReg(R34);
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t R61509::getPixelWidth(void)
{
 return width;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t R61509::getPixelHeight(void)
{
 return height;
}

uint32_t R61509::getRamAddress(void)
{
    return readRamAddress();
}

/**
  * @brief  Get the s6d04d1 ID.
  * @param  None
  * @retval The s6d04d1 ID 
  */
void R61509::readID(void)
{
    id = LCD_IO_ReadData(0X0000);
}

uint16_t R61509::getID(void)
{
    return id;
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void R61509::setCursor(uint16_t Xpos,uint16_t Ypos)
{
    if (dir == MyDrivers::displayDir::horizontal)
        Xpos = width - 1 -Xpos;//横屏其实就是调转x,y坐标
    LCD_IO_WriteReg(x_cmd); 
    LCD_IO_WriteData(Xpos);
    LCD_IO_WriteReg(y_cmd); 
    LCD_IO_WriteData(Ypos);
}

/**
  * @brief  Writes to the selected LCD register.
  * @param  LCDReg: Address of the selected register.
  * @param  LCDRegValue: Value to write to the selected register.
  * @retval None
  */
void R61509::writeReg(uint8_t LCDReg, uint16_t LCDRegValue)
{
  LCD_IO_WriteReg(LCDReg);
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData(&LCDRegValue, 1);
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  LCDReg: address of the selected register.
  * @retval LCD Register Value.
  */
uint16_t R61509::readReg(uint8_t LCDReg)
{ 
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCDReg));
}

/**
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void R61509::setDisplayWindow(uint16_t Xpos,uint16_t Ypos,uint16_t width,uint16_t height)
{
    uint16_t hsaval,heaval,vsaval,veaval;
    width = Xpos + width - 1;
    height = Ypos + height - 1;
    if(dir == MyDrivers::displayDir::horizontal)//横屏
    {
        //窗口值
        hsaval = Ypos;
        heaval = height;
        vsaval = this->width - width - 1;
        veaval = this->width - Xpos - 1;
    }else
    {
        hsaval = Xpos;
        heaval = width;
        vsaval = Ypos;
        veaval = height;
    }
    //设置寄存器值
    LCD_IO_WriteReg(0x0210); 
    LCD_IO_WriteData(hsaval); 
    LCD_IO_WriteReg(0x0211); 
    LCD_IO_WriteData(heaval); 
    LCD_IO_WriteReg(0x0212); 
    LCD_IO_WriteData(vsaval); 
    LCD_IO_WriteReg(0x0213); 
    LCD_IO_WriteData(veaval); 
    //LCD_SetCursor(sx,sy);	//设置光标位置
}

/**
  * @brief  设置扫模模式
  * @param  scanMode:   扫描模式见此枚举变量.
  * @retval None
  */
void R61509::setScanMode(MyDrivers::displayDir _displayDir)
{
    switch (_displayDir)
    {
    case MyDrivers::displayDir::vertical://竖屏
    {
        LCD_IO_WriteReg(0x0001);LCD_IO_WriteData(0x0100);
        LCD_IO_WriteReg(0x0003);LCD_IO_WriteData(0x1030);
        break;
    }
    case MyDrivers::displayDir::horizontal://横屏
    {
        LCD_IO_WriteReg(0x0001);LCD_IO_WriteData(0x0100);
        LCD_IO_WriteReg(0x0003);LCD_IO_WriteData(0x1038);
        break;
    }
    default :
        break;
    }
}

/**
  * @brief  设置显示方向
  * @param  displayDir:   显示方向：横屏和竖屏
  * @retval None
  */
void R61509::setDisplayDir(MyDrivers::displayDir _displayDir)
{
    dir = _displayDir;
    switch (dir)
    {
    case MyDrivers::displayDir::vertical://竖屏
        width = 240;
        height = 400;
        x_cmd = R32;
        y_cmd = R33;
        break;
    
    case MyDrivers::displayDir::horizontal://横屏
        width = 400;
        height = 240;
        x_cmd = R33;
        y_cmd = R32;
        break;
    default:
        break;
    }
    setScanMode(dir);	//默认扫描方向
}

/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  * @retval None
  */
void R61509::writePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  setCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  prepareWrite();

  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData(&RGBCode, 1);
}

/**
  * @brief  Read pixel.
  * @param  None
  * @retval The RGB pixel color
  */
uint16_t R61509::readPixel(uint16_t Xpos, uint16_t Ypos)
{
    /* Set Cursor */
    setCursor(Xpos, Ypos);
    
    /* Read 16-bit Reg */
    LCD_IO_WriteReg(R34);
    LCD_IO_ReadRamData();//Dummy read 
    return (LCD_IO_ReadRamData());
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void R61509::drawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint16_t counter = 0;

    /* Set Cursor */
    setCursor(Xpos, Ypos); 

    /* Prepare to write GRAM */
    prepareWrite();

    /* Write 16-bit GRAM Reg */
    for(counter = 0; counter < Length; counter++)
    {
        LCD_IO_WriteData(RGBCode);
    }  
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode: Specifies the RGB color    
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void R61509::drawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;

  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  //ili9325_WriteReg(LCD_REG_3, 0x1010);
  
  /* Set Cursor */
  setCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  prepareWrite();

  /* Write 16-bit GRAM Reg */
  for(counter = 0; counter < Length; counter++)
  {
    LCD_IO_WriteData(RGBCode);
  }
  
  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  //ili9325_WriteReg(LCD_REG_3, 0x1018);  
}

//画线
// startXpos, startYpos:起点坐标
// endXpos, endYpos:终点坐标  
void R61509::drawLine(uint16_t RGBCode, uint16_t startXpos, uint16_t startYpos, uint16_t endXpos, uint16_t endYpos)
{
    int xerr = 0, yerr = 0, distance; 
    int incx, incy; 
    int delta_x = endXpos - startXpos; //计算坐标增量 
    int delta_y = endYpos - startYpos; 
    int uRow = startXpos; 
    int uCol = startYpos; 
    if( delta_x > 0 )
        incx = 1; //设置单步方向 
    else if( delta_x == 0 )
        incx = 0;//垂直线 
    else {
        incx = -1;
        delta_x = -delta_x;
    } 
    if( delta_y > 0 )
        incy = 1; 
    else if( delta_y == 0 )
        incy = 0;//水平线 
    else {
        incy = -1;
        delta_y = -delta_y;
    } 
    if( delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴 
    else 
        distance = delta_y; 
    for( uint16_t i = 0; i <= distance+1; i++ )//画线输出 
    {  
        writePixel(uRow,uCol,RGBCode);//画点 
        xerr += delta_x ; 
        yerr += delta_y ; 
        if( xerr > distance ) 
        { 
            xerr -= distance; 
            uRow += incx; 
        } 
        if( yerr > distance ) 
        { 
            yerr -= distance; 
            uCol += incy; 
        } 
    }  
}

/**
  * @brief  Displays a bitmap picture.
  * @param  BmpAddress: Bmp picture address.
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD    
  * @retval None
  */
void R61509::drawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
    uint32_t index = 0, size = 0;
    /* Read bitmap size */
    size = *(volatile uint16_t *) (pbmp + 2);
    size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
    /* Get bitmap data address offset */
    index = *(volatile uint16_t *) (pbmp + 10);
    index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
    size = (size - index)/2;
    pbmp += index;
    /* Set GRAM write direction and BGR = 1 */
    /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    //ili9325_WriteReg(LCD_REG_3, 0x1008);

    /* Set Cursor */
    setCursor(Xpos, Ypos);  
    
    /* Prepare to write GRAM */
    prepareWrite();
    
    LCD_IO_WriteMultipleData((uint16_t*)pbmp, size);
    
    /* Set GRAM write direction and BGR = 1 */
    /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
    /* AM = 1 (address is updated in vertical writing direction) */
    //ili9325_WriteReg(LCD_REG_3, 0x1018);
}

/**
  * @brief  Displays picture.
  * @param  pdata: picture address.
  * @param  Xpos: Image X position in the LCD
  * @param  Ypos: Image Y position in the LCD
  * @param  Xsize: Image X size in the LCD
  * @param  Ysize: Image Y size in the LCD
  * @retval None
  */
void R61509::drawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  uint32_t size = 0;

  size = (Xsize * Ysize);

  /* Set Cursor */
  setCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  prepareWrite();
 
  LCD_IO_WriteMultipleData((uint16_t*)pdata, size);
}


/*
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
                continue;
            }else enshow=1; 
              
        }
         LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
    }
} 

void LCD_Show_xNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
                else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
                 continue;
            }else enshow=1; 
              
        }
         LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
    }
}

void LCD_ShowChinese32X29(uint16_t x, uint16_t y, char *cn)
{
    uint8_t i, j, wordNum;
    uint16_t color;
    uint16_t x0=x;
    uint16_t y0=y;
    while (*cn != '\0')
    {
        for (wordNum=0; wordNum<sizeof(CnChar32x29)/118; wordNum++)
        {
            if ((CnChar32x29[wordNum].Index[0]==*cn)
                &&(CnChar32x29[wordNum].Index[1]==*(cn+1))
                &&(CnChar32x29[wordNum].Index[2]==*(cn+2)))
            {
                for(i=0; i<116; i++) 
                {
                    color=CnChar32x29[wordNum].Msk[i];
                    for(j=0;j<8;j++) 
                    {
                        if((color&0x80)==0x80)
                        {
                            LCD_DrawPoint(x,y,lcd_dev.color.front);
                        } 						
                        else
                        {
                            LCD_DrawPoint(x,y,lcd_dev.color.back);
                        } 
                        color<<=1;
                        x++;
                        if((x-x0)==32)
                        {
                            x=x0;
                            y++;
                            if((y-y0)==29)
                            {
                                y=y0;
                            }
                        }
                    }
                }	
            }
        }
        cn += 3;
        x += 32;
        x0=x;
    }
}	

void LCD_ShowChinese16X14(uint16_t x, uint16_t y, char *cn)
{
    uint8_t i, j, wordNum;
    uint16_t color;
    uint16_t x0=x;
    uint16_t y0=y;
    while (*cn != '\0')
    {
        for (wordNum=0; wordNum<sizeof(CnChar16x14)/30; wordNum++)
        {
            if ((CnChar16x14[wordNum].Index[0]==*cn)
                &&(CnChar16x14[wordNum].Index[1]==*(cn+1))
                &&(CnChar16x14[wordNum].Index[2]==*(cn+2)))
            {
                for(i=0; i<28; i++) 
                {
                    color=CnChar16x14[wordNum].Msk[i];
                    for(j=0;j<8;j++) 
                    {
                        if((color&0x80)==0x80)
                        {
                            LCD_DrawPoint(x,y,lcd_dev.color.front);
                        } 						
                        else
                        {
                            LCD_DrawPoint(x,y,lcd_dev.color.back);
                        } 
                        color<<=1;
                        x++;
                        if((x-x0)==16)
                        {
                            x=x0;
                            y++;
                            if((y-y0)==14)
                            {
                                y=y0;
                            }
                        }
                    }
                }	
            }
        }
        cn += 3;
        x += 16;
        x0=x;
    }
}	

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t wide, uint16_t high,uint8_t *pic)
{
    uint16_t temp = 0;
    long tmp=0,num=0;
    LCD_SetCursor(x,y);
    LCD_WriteRAM_Prepare();
    num = wide * high*2 ;
    do
    {  
        temp = pic[tmp + 1];
        temp = temp << 8;
        temp = temp | pic[tmp];
        LCD_IO_WriteData(temp);
        tmp += 2;
    }
    while(tmp < num);	
}

void LCD_ShowPicture2(uint16_t x, uint16_t y, uint16_t wide, uint16_t high,uint16_t *pic)
{
    long tmp=0,num=0;
    LCD_SetCursor(0,0);
    LCD_WriteRAM_Prepare();
    num = wide * high ;
    do
    {
        LCD_IO_WriteData(pic[tmp++]);
    }
    while(tmp < num);
}
*/

void R61509::init(void)
{
    LCD_Delay(100);
    readID();
    
    LCD_IO_WriteReg(0x0000);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0000);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0000);LCD_IO_WriteData(0x0000);

    LCD_IO_WriteReg(0x0400);LCD_IO_WriteData(0x6200);
    LCD_IO_WriteReg(0x0008);LCD_IO_WriteData(0x0808);

    LCD_IO_WriteReg(0x0300);LCD_IO_WriteData(0x0C00);
    LCD_IO_WriteReg(0x0301);LCD_IO_WriteData(0x5A0B);
    LCD_IO_WriteReg(0x0302);LCD_IO_WriteData(0x0906);
    LCD_IO_WriteReg(0x0303);LCD_IO_WriteData(0x1017);
    LCD_IO_WriteReg(0x0304);LCD_IO_WriteData(0x2300);
    LCD_IO_WriteReg(0x0305);LCD_IO_WriteData(0x1700);
    LCD_IO_WriteReg(0x0306);LCD_IO_WriteData(0x6309);
    LCD_IO_WriteReg(0x0307);LCD_IO_WriteData(0x0C09);
    LCD_IO_WriteReg(0x0308);LCD_IO_WriteData(0x010C);
    LCD_IO_WriteReg(0x0309);LCD_IO_WriteData(0x2232);

    LCD_IO_WriteReg(0x0010);LCD_IO_WriteData(0x0016);
    LCD_IO_WriteReg(0x0011);LCD_IO_WriteData(0x0101);
    LCD_IO_WriteReg(0x0012);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0013);LCD_IO_WriteData(0x0001);

    LCD_IO_WriteReg(0x0100);LCD_IO_WriteData(0x0330);
    LCD_IO_WriteReg(0x0101);LCD_IO_WriteData(0x0336);
    LCD_IO_WriteReg(0x0103);LCD_IO_WriteData(0x1000);

    LCD_IO_WriteReg(0x0280);LCD_IO_WriteData(0x6100);
    LCD_IO_WriteReg(0x0102);LCD_IO_WriteData(0xBBB4);

    LCD_IO_WriteReg(0x0001);LCD_IO_WriteData(0x0100);
    LCD_IO_WriteReg(0x0002);LCD_IO_WriteData(0x0100);
    LCD_IO_WriteReg(0x0003);LCD_IO_WriteData(0x1030);
    LCD_IO_WriteReg(0x0009);LCD_IO_WriteData(0x0001);
    LCD_IO_WriteReg(0x000C);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0090);LCD_IO_WriteData(0x0800);
    LCD_IO_WriteReg(0x000F);LCD_IO_WriteData(0x0000);

    LCD_IO_WriteReg(0x0210);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0211);LCD_IO_WriteData(0x00EF);
    LCD_IO_WriteReg(0x0212);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0213);LCD_IO_WriteData(0x018F);         //;400


    LCD_IO_WriteReg(0x0500);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0501);LCD_IO_WriteData(0x0000);
    LCD_IO_WriteReg(0x0502);LCD_IO_WriteData(0x005F);

    LCD_IO_WriteReg(0x0401);LCD_IO_WriteData(0x0001);
    LCD_IO_WriteReg(0x0404);LCD_IO_WriteData(0x0000);

    LCD_IO_WriteReg(0x0007);LCD_IO_WriteData(0x0100);

    setDisplayDir(MyDrivers::displayDir::horizontal);      //默认为竖屏
    setCursor(0, 0);
    prepareWrite();
}

