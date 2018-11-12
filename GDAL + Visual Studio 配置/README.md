首先需要引用头文件

```c
#include "./gdal/gdal_priv.h"
```

把相应的gdal文件夹拷贝到工程目录下

试着运行，发现会提示error LNK2001: 无法解析的外部符号 ”void __cdel“。 这是因数光有头文件，没有lib文件引起的。所以，我们要加入lib文件。这里具体方法是，把需要用到的lib文件拷到项目目录下。一般熟悉OPENCV的同学可能知道，这时的解决方案是，把opencv的lib文件写到vs的配置里。但这样如果换一台电脑运行，会导致程序找不到库文件，会报错。因此，我们使用另一种方法：

把需要用到的lib文件拷到项目目录下，然后在 #include "./gdal/gdal_priv.h" 这一行下面，添加如下代码：

```
#pragma comment(lib, "gdal_i.lib")
```

这样就能编译通过了。

运行时，还需要提供gdal的动态链接库。把gdal18.dll拷到项目的Release目录下，程序就可以运行了。

------

