# 利用可见光传输信息的软件
该课程项目分为发送方和接收方 2 个部分。 发送方将信息编码为一组信号，并通过可见光发送。接收方通过手机录制视频的 形式接收，并解码得到原始消息。 由于大多数同学普遍尚无法完成手机编程，编码部分可简化为：
1、接受用户输入消息。 
2、利用 FFMPEG 或 OPENCV 编码生成一系列图像。
3、利用 FFMPEG 命令行将图像编码为视频。 
4、利用系统播放器播放在普通计算机液晶显示器上。 
解码部分可简化为： 1、手机录制显示器播放的视频，通过 QQ 或 USB 线复制到计算机上。 
2、利用 FFMPEG 命令行将视频解码为图像。 
3、利用 FFMPEG 或 OPENCV 接受上述输入的一组图像。
4、解码得到消息，输出在界面上。
