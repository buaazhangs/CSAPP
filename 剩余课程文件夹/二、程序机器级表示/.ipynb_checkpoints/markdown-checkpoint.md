# 程序机器级表示
### lecture 1
导读部分：介绍c编译，汇编反汇编等  
gdb<br>
汇编基础：  
寄存器：  
%e:32bits, %r:64bits<br>
基础指令:  
mov:三种类型的move，立即数，寄存器与内存，但并不代表有9种可能性，见下图：  
<img src="markdown图片/屏幕截图 2023-11-06 201736.png" alt="图片alt" title="图片title"><br>
上文的地址：  
汇编中寄存器加括号类似于，"\*a",如下图：<br>
<img src="markdown图片/屏幕截图 2023-11-06 202745.png" alt="图片alt" title="图片title"><br>
这种小括号为精简表示，具体的见下图<br>
其中，直接括号代表直接取值(R),D(R)这种代表取R位置后移位D<br>
<img src="markdown图片/屏幕截图 2023-11-06 213943.png" alt="图片alt" title="图片title"><br>
另外，其他的表示形式如下图：<br>
<img src="markdown图片/屏幕截图 2023-11-06 214200.png" alt="图片alt" title="图片title"><br>
lea指令：load effective address(加载有效地址)<br>
leaq src dst<br>
int* p = &x[i],计算x[i]的地址并放入p中。  
当然其还可以被用作计算
指令总结：源在前，目的地在后
<img src="markdown图片/屏幕截图 2023-11-06 210533.png" alt="图片alt" title="图片title"><br>

## lecture2 控制
介绍条件，循环等：这节课把各个指令的方法截图下来即可

### 特殊寄存器：
%rsp:存放栈指针  
%rip:不能直接访问，存储程序执行到哪的指针<br>
condition code：
均为1位的标志量（flag），不能直接设置，根据其他操作设置值,其中4个的细节如下：<br>
CF:carry flag,unsigned溢出标志位。  
SF:sign flag,符号标志位  
ZF:零标志位  
OF:overflow flag,有符号数溢出标志位。<br>
#### 某些汇编指令：
add  
cmp  
test  
#### 读取条件码的值
set系列指令：  
<img src="markdown图片/屏幕截图 2023-11-07 164043.png" alt="图片alt" title="图片title"><br>

由上图，可以得到条件分支语句:<br>


### 以下实现都是用goto视角，goto意味着jump
### ifelse实现：
#### 条件jump：
<img src="markdown图片/屏幕截图 2023-11-07 173101.png" alt="图片alt" title="图片title"><br>


### loops：
基于do while，其他类型可以往这个方向参考。

### switch case：
与if的条件jump不完全相同，switch有一个jump table


## 重点开始，lecture3：程序，函数结构与调用

