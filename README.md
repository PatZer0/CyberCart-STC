# CyberCart-STC (STC32G12K128)

## 简介

基于**STC32G12K128**单片机的超市购物机器人项目源代码，参加浙江省第八届大学生机器人竞赛(2024)，此方案于2024年5月4日被废弃，后更换Rasberry Pi+笔记本方案。

本项目主要实现了舵机、步进电机、激光测距、电子罗盘、行走电机、SPI屏幕的控制
在 **SeekFree逐飞** 开源的 STC32 单片机库函数基础上进行修改，但其中串口通信（上位机）、激光测距、电子罗盘是手动基于官方文档实现的。

## 说明

**推荐使用VSCode配合Keil插件进行开发**，详细教程请看：['https://patzer0.com/archives/use-vscode-with-keil']

**SEEKFREE逐飞开源库**：[https://portrait.gitee.com/seekfree/STC32G12K128_Library]

**注意：本项目仅供学习交流使用，请勿用于商业用途！**