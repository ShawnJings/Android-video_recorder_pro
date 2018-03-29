## 说明
### 来源：
该项目主要是修改自：https://github.com/zhanxiaokai/Android-video_recorder_pro
非常感谢原作者的奉献！其主要功能是录制音视频。
### 目的：
原项目是Eclipse工程，现在改由AndroidStudio工程，使用CMake编译C代码
### 改动
1. 使用CMake编译C代码
2. compileSdkVersion修改为25，增加了Camera等权限请求
3. 增加C++方法的返回值，否则在真机运行时会发生崩溃问题
4. 只构建了“armeabi-v7a”的so库，“x86”prebuild资源不全，“armeabi”可自行添加
5. 其中定然有很多不足之处，请大家斧正！

### Version1:待修复问题
1. NEON优化开启不成功，一直报错，所以在代码中屏蔽了
2. 在低版本的手机上无法录制，测试信息如下：
	1. 手机型号：红米1，Android版本：4.2.1，错误信息：MediaMuxer找不到以及MediaCodec部分方法找不到
	2. 手机型号：华为P7-L07，Android版本：5.1.1，错误信息：MediaCodec.dequeueOutputBuffer()抛出参数异常