# CodeBrick

## 介绍
一种无OS的单片机通用解决方案，包括任务轮询框架，命令管理器、低功耗管理、环形缓冲区等实用模块。

### module模块
  时间轮询框架，使用例子：
 
1.使用此模块前需要系统提供滴答定时器，用于驱动任务轮询作业

```
//定时器中断(提供系统滴答)
void timer_interrupt(void)
{
    systick_increase(SYS_TICK_INTERVAL); //增加系统节拍
}
```
2.任务初始化及注册(以按键扫描为例)

```
static void key_init(void)
{
    /*do something*/
}

static void key_scan(void)
{
    /*do something*/
}

module_init("key", key_init);            //注册按键初始化接口
task_register("key", key_scan, 20);      //注册按键任务(20ms轮询1次)
```


  
### comdef模块
包含常用宏定义,段定义、匿名类型等。

### blink模块
具有闪烁特性(led, motor, buzzer)的设备(led, motor, buzzer)管理
使用之前有两个注意事项:
- 需要系统提供滴答时钟，blick.c中是通过get_tick()接口获取，依赖module模块
- 需要在任务中定时进行轮询

```
//主程序入口
void main(void)
{
    while (1)  {
        blink_dev_process();            //定时轮询
    }
    
}
```
或者通过"module"模块的任务注册来实现

```
task_register("blink", blink_dev_process, 50);  //50ms轮询1次
```
***使用例子,以LED为例***
```
blink_dev_t led;                             //定义led设备

/*
 *@brief     红色LED控制(GPIOA.8)
 *@param[in] on - 亮灭控制
 */
static void led_ctrl(int on)
{
    if (on)
        GPIOA->ODR |= (1 << 8);
    else 
        GPIOA->ODR &= ~(1 << 8);
}

/*
 *@brief     led初始化程序
 */
void led_init(void)
{
    led_io_init(void);                  //led io初始化
    blink_dev_create(&led, led_ctrl);   //创建led设备
    
    blink_dev_ctrl(&led, 50, 100, 0);   //快闪(50ms亮, 100ms灭)
}
```


### 按键管理模块
类似blink模块，使用之前有两个注意事项:
- 需要系统提供滴答时钟，key.c中是通过get_tick()接口获取，依赖module模块
- 需要在任务中定时进行轮询

```
key_t key;                             //定义按键管理器

/*
 *@brief     按键事件
 *@param[in] type     - 按键类型(KEY_PRESS, KEY_LONG_DOWN, KEY_LONG_UP)  
 *@param[in] duration - 长按持续时间
 */
void key_event(int type, unsigned int duration)
{
	if (type == KEY_PRESS) {                //短按
		 
	} else if (type == KEY_LONG_DOWN) {     //长按
		
	}
} 

//读取键值(假设按键输出口为STM32 MCU PA8)
int read_key(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == Bit_RESET;
}

/*
 *@brief     按键初始化
 */
void key_init(void)
{
    //打开GPIO 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//配置成输入模式
    gpio_conf(GPIOA, GPIO_Mode_IN, GPIO_PuPd_NOPULL, GPIO_Pin_8); 
    //创建1个按键
    key_create(&key, read_key, key_event);  
}

```
