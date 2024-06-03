
# 全热风咖啡烘豆机主控板，适合自己diy用。

## 性能特点
- 2路温度采集：bt和inlet
- 温度采集使用max6675 k型热电偶
- 无线连接artisan
- 0-10v连续模拟输出，可以控制工业用可控硅
- 风机使用12/24v无刷控制。
- 留有I2C接口可以接扩展传感器、oled等。
  
## 目前开源板程序功能如下：
- 1：为无线接artisan记录温度数据
- 2：手动控制风力和火力。

## 相关资源
- 硬件推荐：

        1.风扇控制:模型电调(3s-4s,40A 以上的就OK)，pwm频率为50Hz，分辨率可以选择参数10（0-1024档）或者9（512档）。
        2.可控硅: MGR 2440 (0-10V，40A输出)


- 硬件开源地址：https://oshwhub.com/sakunamary/re-feng-ji-kong-zhi-ban-kai-yuan
- 固件开源地址：
  
        1.https://github.com/sakunamary/Roaster_controller_opensource

        2.https://gitee.com/sakunamary/Roaster_controller_opensource

- 操作视频：
  
        1.https://www.bilibili.com/video/BV1eg411e7H4/（怎么链接模块的Wi-Fi）
  
        2.https://www.bilibili.com/video/BV1et4y1w7i5/ （artisan怎么链接）

        3.https://www.bilibili.com/video/BV1AV4y1L7yL/ （artisan的基本操作）
  
            
## 特别提醒
本项目是开源项目，欢迎集成新功能进来、修改bug。请注意本项目的软件和硬件使用的是非商业版权授权协议的。如果需要做商业用途，请自行做闭源软硬件设计。控制原理就是那点东西了，请不要做拿来党。
