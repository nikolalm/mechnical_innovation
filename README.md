# 机械创新设计大赛控制部分
## apriltags2_ros使用

- apriltags2_ros使用

  <https://github.com/nikolalm/apriltags2_ros/blob/master/README.md>

```c
//启动apriltags检测
roslaunch apriltags2_ros continuous_detection.launch 
//打开摄像头
roslaunch usb_cam usb_cam-test.launch 
//观察tf坐标
rviz
```

- tf坐标变换订阅与发布

  ```c
  rosrun apriltags2_ros apriltags2_ros_detector_subscriber_node
  ```

  订阅发布tag_0和tag_1的相对位姿

- ros节点中建立linux tcp服务器，发送底盘速度数据

  ```c
  rosrun tf_trans tf_trans
  ```

  - tcp服务器端口设置

    tf_trans功能包src下cpp文件

    ```c++
    #define SERVER_PORT 4500
    ```

  - 服务器ip查看

    ifconfig

  - 程序流程

    - 创建ros节点
    - 创建tcp服务器
    - 监听客户端
    - 若有客户端连接
      - tag_0和tag_1的相对位姿，转换成小车移动速度发送

  - 通讯协议

    ```c++
    sprintf(buf, "@x%dy%dr%d", speed_x, speed_y, speed_z);
    /*
    @帧头
    speed_x 左右速度
    speed_y 前后速度
    speed_z 旋转速度
    */
    ```

    

  