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
### lecture2 控制
介绍条件，循环等：
