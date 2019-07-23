# 关于工具链的说明

## 字节顺序标记（byte order mark，BOM）
* UTF-8 编码的源文件，当出现中文字符时，必须增加 BOM 才能使用 Visual Studio 成功编译。否则，编译时会出现莫名的问题，最常见的错误就是“找不到函数定义代码”之类。所以，源文件必须使用带 BOM 的 UTF-8 编码。

* GCC 虽然支持UTF-8 编码的源文件，但是，GCC 4.4 以下版本却不支持编译带 BOM 的 UTF-8 编码的源文件。GCC 4.4 以上版本才修复了这个 BUG。

## 编译器版本的选择
编译器需要支持 C++11 ，所以，选择如下：
* Windows: Visual Studio 2015
* Linux: GCC 4.8.2

## 工具链
最终，编译环境以及工具链如下：
* Windows 7+, Visual Studio 2015+, 带 BOM 的 UTF-8 编码源文件
* GCC 4.8.2+, 带 BOM 的 UTF-8 编码源文件

## 参考
[Byte order mark(BOM) ](https://en.wikipedia.org/wiki/Byte_order_mark)
