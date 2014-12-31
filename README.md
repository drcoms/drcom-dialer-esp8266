drcom-dialer-esp8266
====================

基于 ESP8266 的吉林大学 DrCOM 拨号器。

### 背景
目的：为了随时随地，在任何路由器上完成 DrCOM 认证。  
由于个人原因只是做了模型，敬请随意修改，但是需要开放代码。

### 使用方法
最简：买一块 esp8266 模块板，至少要带 GPIO14 的。  
`app/gen_misc.sh` 之后按照 8266 的刷机方法刷进去。  
可以看看图片说明 <http://blog.lyj.me/hei-ke-ji-esp8266-drcom-client/>

1. GPIO14接地则进入配置，这时候会有一个`DrCOMDialer`热点, 如果只收到一个`ESP_xxxx`热点，则重启一下。
2. 连接热点，打开`http://192.168.4.1`，按说明正确配置，提交以后会自动重启。
3. 成功的标志就是串口输出`login success`，或者能上网。

### 说明
DrCOM 登陆代码部分来源于 `https://github.com/drcoms/jlu-drcom-client/tree/master/C-version` 由 @feix 编写。
