#include <sw_i2c/Inc/stm32_iic.hpp>

GPIO_TypeDef* IIC_SCL_PORT[] = {SOFT_I2C_SCL_GPIO_Port, SOFT_I2C_SCL_GPIO_Port, SOFT_I2C_SCL_GPIO_Port};
GPIO_TypeDef* IIC_SDA_PORT[] = {SOFT_I2C_SDA_GPIO_Port, SOFT_I2C_SDA_GPIO_Port, SOFT_I2C_SDA_GPIO_Port};

const uint16_t  IIC_SCL_PIN[] = {SOFT_I2C_SCL_Pin, SOFT_I2C_SCL_Pin, SOFT_I2C_SCL_Pin};
const uint16_t  IIC_SDA_PIN[] = {SOFT_I2C_SDA_Pin, SOFT_I2C_SDA_Pin, SOFT_I2C_SDA_Pin};

Stm32IIC::Stm32IIC()
{

}

void Stm32IIC::delay_us(__IO uint32_t nCount)
{
    for (; nCount != 0;nCount--);
}

void Stm32IIC::update(IIC_TypeDef iic_)
{
     iic = iic_;
}
void Stm32IIC::begin(void)
{

}

void Stm32IIC::sda_out(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.Pin = IIC_SDA_PIN[this->iic];
    GPIO_InitStructer.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructer.Pull = GPIO_NOPULL;
    GPIO_InitStructer.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(IIC_SDA_PORT[this->iic], &GPIO_InitStructer);
}

void  Stm32IIC::sda_in(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.Pin = IIC_SDA_PIN[this->iic];
    GPIO_InitStructer.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructer.Pull = GPIO_NOPULL;
    GPIO_InitStructer.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(IIC_SDA_PORT[this->iic], &GPIO_InitStructer);

}

int Stm32IIC::sda_read()
{
    return static_cast<int>(HAL_GPIO_ReadPin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic]));
}

void  Stm32IIC::Start(void)
{
    sda_out();
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);
//	delay_us(IIC_DELAY);
}

void Stm32IIC::Stop(void)
{   
    sda_out();
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);			//SDA_L
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET); 				//SCL_H
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);	 			//SDA_H
    delay_us(IIC_DELAY);
}

//????????????:		uint8_t Stm32IIC::Read_Ack(void)
// ??? ???:	    ??????ACK
uint8_t Stm32IIC::Read_Ack(void)
{
    uint8_t ReAck;
    sda_in();
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET); 				//SCL_H	
    delay_us(IIC_DELAY);
    ReAck=(uint8_t)(IIC_SDA_PORT[this->iic]->IDR&IIC_SDA_PIN[this->iic]) >>0;   //Read SDA
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);			//SCL_L
    delay_us(IIC_DELAY);
    return ReAck;
}

//????????????:		void Stm32IIC::Write_OneByte(uint8_t Dat)
//????????????:	    ????????????????????????????????????
//??????	Dat     ???????????????
void Stm32IIC::Write_OneByte(uint8_t Dat)
{
    uint8_t i;  
    sda_out();
    for(i=0;i<8;i++)
    {  
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);			//SCL_L
        delay_us(IIC_DELAY);
        if(Dat&0x80)
        {
            HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);	 		//SDA_H
        }
        else
        {
            HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);		//SDA_L
        }
        Dat<<=1;
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET); 				//SCL_H
        delay_us(IIC_DELAY);
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);			//SCL_L
        delay_us(IIC_DELAY);
    }
    Read_Ack();
}

//????????????:		uint8_t Stm32IIC::Read_OneByte(void)
//????????????:	    ????????????????????????????????????
uint8_t Stm32IIC::Read_OneByte(void)
{  
    uint8_t data,i;
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);	 			   	//SDA_H
    delay_us(IIC_DELAY);
    for(i=0;i<8;i++)
    {
        data<<=1;
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);			//SCL_L
        delay_us(IIC_DELAY);
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET); 				//SCL_H
        delay_us(IIC_DELAY);
        if(HAL_GPIO_ReadPin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic]))  //Read SDA
            data=data | 0x01;
        else 
            data=data & 0xFE;
    }
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);				//SCL_L
    delay_us(IIC_DELAY);
    return data;
}

//????????????:		bool Stm32IIC::get_ack() 
//????????????:	    ??????ACK
bool Stm32IIC::get_ack() 
{   
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);		//SCL_L
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);			//SDA_H
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);			//SCL_H
    bool ret = !sda_read();
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);		//SCL_L
    return ret;
}

//????????????:		void Stm32IIC::send_ack() 
//????????????:	    ??????ACK
void Stm32IIC::send_ack() 
{   
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);		//SDA_L
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);			//SCL_H
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);		//SCL_L
}

//????????????:		void Stm32IIC::send_nack() 
//????????????:	    ??????NACK
void Stm32IIC::send_nack() 
{  
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);			//SDA_H
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);			//SCL_H
    delay_us(IIC_DELAY);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);		//SCL_L
}

//????????????:		uint8_t Stm32IIC::shift_in() 
//????????????:	    ?????????????????????
uint8_t Stm32IIC::shift_in() 
{
    uint8_t data = 0;
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);			//SDA_H
    int i;
    for (i = 0; i < 8; i++) 
    {   
        delay_us(IIC_DELAY);
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);		//SCL_H
        data |= sda_read() << (7-i);
        delay_us(IIC_DELAY);
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);	//SCL_L
    }
    return data;
}

//????????????:		uint8_t Stm32IIC::shift_in() 
//????????????:	    ?????????????????????
void Stm32IIC::shift_out(uint8_t val) 
{
    int i;
    for (i = 0; i < 8; i++) 
    {
        if(!!(val& (1<<(7-i))) == 0)
        {
            HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);		//SDA_L
        }
        if(!!(val& (1<<(7-i))) == 1)
        {
            HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);		//SDA_H
        }
        delay_us(IIC_DELAY);		
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);			//SCL_H
        delay_us(IIC_DELAY);
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 				//SCL_L
    }
}

//????????????:		void Stm32IIC::WriteCmd(uint8_t Cmd)
//????????????:	    ?????????????????????
//??????	Cmd     ????????????????????????
void Stm32IIC::WriteCmd(uint8_t Cmd)
{
    Start();
    Write_OneByte(0x78);
    //Read_Ask();
    Write_OneByte(0x00);
    //Read_Ask();
    Write_OneByte(Cmd);
    //Read_Ask();
    Stop();
}

//????????????:		void Stm32IIC::WriteDat(uint8_t Data)
//????????????:	    ?????????????????????
//??????	Cmd     ????????????????????????
void Stm32IIC::WriteDat(uint8_t Data)
{
    Start();
    Write_OneByte(0x78);
    //Read_Ask();
    Write_OneByte(0x40);
    //Read_Ask();
    Write_OneByte(Data);
    //Read_Ask();
    Stop();
}

//????????????:		int Stm32IIC::Write_Addr_Data(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
//????????????:	    ????????????????????????????????????????????????????????????????????????
int Stm32IIC::Write_Addr_Data(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    int i;
    Start();
    Write_OneByte(addr );
    /*if (!Wait_Ack_Reply()) 
    {
        Stop();
        return 1;
    }*/
    Write_OneByte(reg);
    //Wait_Ack_Reply();
    for (i = 0; i < len; i++) 
    {
        Write_OneByte(data[i]);
        /*if (!Wait_Ack_Reply()) 
        {
            Stop();
            return 0;
        }*/
    }
    Stop();
    return 0;

}

//????????????:		int Stm32IIC::Read_Addr_Data(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
//????????????:	    ????????????????????????????????????????????????????????????
int Stm32IIC::Read_Addr_Data(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    Write_OneByte(addr << 1);
    if (!Wait_Ack_Reply()) 
    {
        Stop();
        return 1;
    }
    Write_OneByte(reg);
    Wait_Ack_Reply();
    Start();
    Write_OneByte((addr << 1)+1);
    Wait_Ack_Reply();
    while (len) {
        if (len == 1)
            *buf = Read_Byte_Ack(0);
        else
            *buf = Read_Byte_Ack(1);
        buf++;
        len--;
    }
    Stop();
    return 0;
}

//????????????:		uint8_t Stm32IIC::Read_DeviceAddr_OneByte(uint8_t Addr,uint8_t addr)
//????????????:	    ?????????????????? ???????????????????????????
//??????	Addr    ??????????????????
//		addr	???????????????
//??????   ???????????????
uint8_t Stm32IIC::Read_DeviceAddr_OneByte(uint8_t Addr,uint8_t addr)
{
    uint8_t res=0;
    Start();	
    Write_OneByte(Addr);	   //???????????????
    res++;
    Wait_Ack_Reply();
    Write_OneByte(addr); res++;  //????????????
    Wait_Ack_Reply();	  
    //IIC_Stop();//????????????????????????	
    Start();
    Write_OneByte(Addr+1); res++;          //??????????????????			   
    Wait_Ack_Reply();
    res=Read_Byte_Ack(0);	   
    Stop();//????????????????????????
    return res;
}

//????????????:		int Stm32IIC::Wait_Ack_Reply(void)
// ??? ???:	    ??????ACK??????
//??????  int:  	0 ???ACK??????  1???ACK??????
int Stm32IIC::Wait_Ack_Reply(void)
{
    uint8_t ucErrTime=0;
    sda_in();      																//SDA Set Input
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);	 			//SDA_H
    delay_us(1);	   
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET); 				//SCL_H
    delay_us(1);	 
//	while((uint8_t)(IIC_SDA_PORT[this->iic]->IDR&IIC_SDA_PIN[this->iic]) >>0)  	//Read SDA
    while(sda_read())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            Stop();
            return 1;
        }
//	  delay_us(1);
    }
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);			//SCL_L  
    return 0;  
} 

//????????????:		void Stm32IIC::Generate_Ack_Reply(void)
// ??? ???:	    ??????ACK??????
void Stm32IIC::Generate_Ack_Reply(void)
{   
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L
    sda_out();                                                                  //SDA_OUT
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);			//SDA_L
    delay_us(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);	 			//SCL_H
    delay_us(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L	
}

//????????????:		void Stm32IIC::NoGenerate_Ack_Reply(void)
// ??? ???:	    ?????????ACK??????
void  Stm32IIC::NoGenerate_Ack_Reply(void)
{   
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L
    sda_out();                                                                  //SDA_OUT
    HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);				//SDA_H
    delay_us(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);	 			//SCL_H
    delay_us(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L	
}

//IIC??????????????????
//????????????????????????
//1????????????
//0????????????			  
void Stm32IIC::Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    sda_out();//SDA???????????????	    
    HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET);//SCL_L ??????????????????????????????
    for(t=0;t<8;t++)
    {              
        if(((txd&0x80)>>7) == 0)
        {
            HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_RESET);		//SDA_L
        }
        if(((txd&0x80)>>7) == 1)
        {
            HAL_GPIO_WritePin(IIC_SDA_PORT[this->iic], IIC_SDA_PIN[this->iic], GPIO_PIN_SET);			//SDA_H
        }
        txd<<=1; 	  
        delay_us(2);   //???TEA5767??????????????????????????????
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);				//SCL_H IIC_SCL=1;
        delay_us(2); 
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L IIC_SCL=0;	
        delay_us(2);
    }	 
} 	 

//????????????:		uint8_t Stm32IIC::Read_Byte_Ack(uint8_t ack)
//????????????:	    ????????????????????????????????? ACK????????????
uint8_t Stm32IIC::Read_Byte_Ack(uint8_t ack)
{
    uint8_t i,receive=0;
    sda_in();//SDA???????????????
    for(i=0;i<8;i++ )
    {
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_RESET); 			//SCL_L
        delay_us(2);
        HAL_GPIO_WritePin(IIC_SCL_PORT[this->iic], IIC_SCL_PIN[this->iic], GPIO_PIN_SET);			//SCL_H
        receive<<=1;
        if(sda_read())
            receive++;
        delay_us(1);
    }					 
    if (!ack)
        NoGenerate_Ack_Reply();														//??????nACK  
    else
        Generate_Ack_Reply(); 														//??????ACK 
    return receive;
}

Stm32IIC::~Stm32IIC()
{

}




