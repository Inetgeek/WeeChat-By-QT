/**
 * @author: Colyn
 * @group: NJUST
 * @date: 2022-10-30
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

//文件初始总大小
#define INIT_TOTAL_BYTES 0
//接收文件初始总大小
#define INIT_RECEIVED_BYTES 0
//文件初始总大小
#define INIT_FILE_SIZE 0
//数据块初始大小
#define INIT_BLOCK_SIZE 0
//写入数据初始大小
#define INIT_WRITTEN_BYTES 0
//将要写入的数据大小
#define INIT_TO_WRITE_BYTES 0
//TCP客户端端口
#define TCP_CLIENT_PORT 8888
//TCP服务器端口
#define TCP_SERVER_PORT 8888
//UDP端口
#define UDP_SOCKET_PORT 8080
//设置客户端的宽度
#define SET_CLIENT_SIZE_X 350
//设置客户端的高度
#define SET_CLIENT_SIZE_Y 180
//设置服务器的宽度
#define SET_SERVER_SIZE_X 350
//设置服务器的高度
#define SET_SERVER_SIZE_Y 180
//有效载荷窗口大小
#define PAYLOAD_SIZE 64*1024

#endif // CONFIG_H
