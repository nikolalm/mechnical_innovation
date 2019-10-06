#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include "geometry_msgs/Quaternion.h"
#include <turtlesim/Spawn.h>
#include <iostream>
#include <cmath>

 
#define SERVER_PORT 4500

#define BACKLOG     10
 
 using namespace std;
double x,y,z,Aww,Azz,Ahh,Aii;
double theta;

int limit_out(int temp)
{
	if(temp>170)
		temp = 170;
	else if(temp<-170)
		temp = -170;
	return temp;
}

int main(int argc, char **argv)
{
	int iSocketServer;
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	struct sockaddr_in tSocketClientAddr;
	int iRet;
	socklen_t iAddrLen;
 
	int iRecvLen;
	unsigned char ucRecvBuf[1000];
 
	int iClientNum = -1;

   	ros::init(argc, argv, "two_tf_transform");
 
 	ros::NodeHandle node;


    while (node.ok()){
	    signal(SIGCHLD,SIG_IGN);//处理僵尸进程的，可以看我那篇关于僵尸进程的博客
	
	    iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	    if (-1 == iSocketServer)
	    {
            printf("socket error!\n");
            return -1;
        }
    
        tSocketServerAddr.sin_family      = AF_INET;
        tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
        tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
        memset(tSocketServerAddr.sin_zero, 0, 8);
        
        iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
        if (-1 == iRet)
        {
            printf("bind error!\n");
            return -1;
        }
    

        iRet = listen(iSocketServer, BACKLOG);
        if (-1 == iRet)
        {
            printf("listen error!\n");
            return -1;
        }
        while(node.ok())
        {
           
		iAddrLen = sizeof(struct sockaddr);

		iSocketClient = accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		
		if (-1 != iSocketClient)
		{
			iClientNum++;
			printf("Get connect from client %d : %s\n",  iClientNum, inet_ntoa(tSocketClientAddr.sin_addr));
			while(node.ok())
			{
				/* 子进程的源码 */
				 tf::TransformListener listener;
            ros::Rate rate(5.0);
            tf::StampedTransform transform;
            try{
                        //得到坐标odom和坐标base_link之间的关系
            listener.waitForTransform("tag_0", "tag_1", ros::Time(0), ros::Duration(3.0));
            listener.lookupTransform("tag_0", "tag_1",
                                            ros::Time(0), transform);
            }
            catch (tf::TransformException &ex) {
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
            }
                
                    x=transform.getOrigin().x();
                    y=transform.getOrigin().y();
                    z=transform.getOrigin().z();
                    cout<<"x: "<<x<<endl;
                    cout<<"y: "<<y<<endl;
                    cout<<"z: "<<z<<endl;

                    geometry_msgs::Quaternion msg;
                    msg.x = transform.getRotation().getX();
                    msg.y = transform.getRotation().getY();
                    msg.z = transform.getRotation().getZ();
                    msg.w = transform.getRotation().getW();
                    tf::Quaternion quat;
                    tf::quaternionMsgToTF(msg, quat);
                    double roll, pitch, yaw,yaw2;
                    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

                    theta=2*acos(transform.getRotation().getW())/3.1415926*180;
                    yaw2 = yaw /3.1415926*180;

                    cout<<"yaw2:  "<<yaw2<<endl;


                    
        			printf("ser :# ");

 					fflush(stdout);
					char buf[15];
		    int speed_x,speed_y,speed_z;
		    if((15-x*5)<5&&(15-x*5)>-5)
		    {
			speed_x = 0;
		    }
		    else
		    	speed_x = limit_out((0.15 - x)*100);

		    if((y*100)<5&&(y*100)>-5)
		    {
			speed_y = 0;
		    }
		    else
		    	speed_y = limit_out(y*100);

		    if((yaw2)<20&&(yaw2)>-20)
		    {
			speed_z = 0;
		    }
		    else
			speed_z = limit_out(yaw2*5);
	                    cout<<"x:  "<<speed_x<<endl;
cout<<"y:  "<<speed_y<<endl;
cout<<"z:  "<<speed_z<<endl;
                   cout<<endl;

                    sprintf(buf, "@x%dy%dr%d", speed_x, speed_y, speed_z);
//       				buf[16] = '\0';
        			write(iSocketClient, buf, strlen(buf));
 				rate.sleep();
				
            }				
		}
       
			
	}	
		
	close(iSocketClient);
    }
	close(iSocketServer);
	return 0;
}


