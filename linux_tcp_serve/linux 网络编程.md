# linux 网络编程

## linux tcp服务器

- socket套接字

  与内核网络相关接口。

- tcp服务器

  ```mermaid
  graph TD
  a[创建socket]-->b[绑定bind]
  b-->c[监听listen]
  c-->d[与客户端通讯accept]
  d-->|a|e[write]
  d-->|b|f[read]
  e-->g[close]
  f-->g
  ```

- 参考博客

  https://www.cnblogs.com/wangcq/p/3520400.html