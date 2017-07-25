# openssl_dec_enc_tool
客户需求解密板子里面的加密文件，做的openssl的加解密的tool


详情：http://blog.csdn.net/linbounconstraint/article/details/74910323


客户需求解密板子里面的加密文件，做的openssl的加解密的tool

tool去除了加密的，只需要解密，想加密只需反向操作下。

源码： 
https://github.com/leolin0518/openssl_dec_enc_tool

文件解密

算psw的值

记录下： 
这个工具是给客户测试使用的 
1.主要是公司的RE板子上会把一些config和信息加密备份，客户测试那边想通过一个tool可以解密，查看一些数据。 
2.是不同的板子的root级别密码是根据mac地址和sn号的一部分组成一个字符串，然后对字符串进行加密，加完的密钥的某X位作为密码，本来是用openssl的-aes-256-cbc 加密密码，但是-aes-256-cbc 加密的话，每次密钥都会变化，so，用的md5算得。 
3.板子加密的是在板子的Linux系统下，加密的，解密的tools是需要在windows下使用。

思路： 
我用qt写的，如果使用openssl源码，感觉蛮费事，刚开始使用源码函数写的，发现在windows下，测试，自己加密解密都是ok的。但是把板子里面的备份文件，死活就是解密不了。赶时间，后来就想因为只是简单的使用加密相关的，就直接用qt做个ui， QProcess调用openssl的进程，通过命令的形式获取相关的数据就行了。 
但是，然而md5的密码没问题。 
加密文件用-aes-256-cbc加解密，在板子是下命令可以解密，然后我把加密文件拷贝到另一台linux系统的电脑上，同样的命令并不是每次的可以解密的，醉了。很大概率不行。 
后来发现，加密的key不要用一些特殊字符，就可以了，原因可能是板子的Linux是裁剪修改过的，mtk的sdk。在板子里面的key和windows下输入的key，可能是编码格式不一样，去除特殊字符就ok了。醉了。

tips： 
1.最好是Linux下的openssl 和windows下的openssl版本一致。 
2.加密的key不要用一些特殊字符 
3.windows下没有echo这些用，每次加密都是通过-in -out 参数“` 
openssl enc -aes-256-cbc -salt -a -in /tmp/.config.tgz -out /tmp/.config.enc -k xxxxxx 
4.注意加解密行的空格或者回车 
5.同样的加密，板子上是先tar打包，加密，然后算一个md5值插入到加密文件的某一行还有EOF行。so，解密的时候先删除加密文件的某一行（md5行）再进行解密，删除EOF行。刚开始做的时候，忘了这个。

md5是，md5为了保持和板子的一致性，用的openssl里面的OpenSSL> dgst -md5 in.log

其实也可以使用qt里面的加密类QCryptographicHash。有哈希，md4,5什么的。 
Qt5自带加密方法

QProcess如何调用其它进程

QProcess用法demo
