# GEC6818模拟电梯系统

## 上位机

**实时监控子系统**

![image](https://user-images.githubusercontent.com/48900845/128978104-a957bfd0-2034-45de-8e64-e1ca1ecdb716.png)


**梯外按键模拟器**

![image](https://user-images.githubusercontent.com/48900845/128977601-9588465f-9ec6-4af2-b561-459cac796922.png)


**报警监控子系统**

**功能：** 当按下梯内报警按钮后，上位机播放报警铃声

## GEC6818开发板

**电梯运行系统**


![image](https://user-images.githubusercontent.com/48900845/128977727-b440264d-dd1e-4c84-92b5-2417b755eca9.png)



**多媒体广告子系统**

左上方为图片广告

左下方为视频广告

![image](https://user-images.githubusercontent.com/48900845/128977734-b1bf1a8b-6601-4a95-bd63-8008679da257.png)


**监控子系统**

**功能：** 负责抓取摄像头画面，发送至监控端。


# 项目结构

## win(上位机项目)

**bj(报警监控子系统)**

**jk(实时监控子系统)**

**elevator(梯外按键模拟器)**


## gec6818(开发板项目)

**adv(多媒体广告子系统)**

**elevator(电梯运行系统)**

**jk(监控子系统)**

## 图片资源

**广告系统图片资源(gec6818/elevator/adv)**

**广告系统视频资源(gec6818/adv/1.avi)**

**梯内图片资源(gec6818/btn)**

**梯内楼层图片资源(gec6818/f)**
