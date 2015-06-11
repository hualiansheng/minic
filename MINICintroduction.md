# 简介 #

MiniC是一个C语言子集的编译器，目标代码面向北京大学设计的UNICORE架构。

# 编译器结构 #

MiniC编译器将按顺序对输入文件做如下处理：
  * [词法分析](lex.md)（由flex辅助完成）
  * 语法分析 - 生成抽象语法树AST（由bison辅助完成）
  * 符号表生成
  * 类型检查
  * 运行时内存分配
  * 中间代码生成
  * 中间代码优化
  * 目标代码生成

本Wiki将随着工程的开发进度，提供每一个处理阶段的设计和部分实现细节。

# 使用方法 #
**注意：以下说明仅适合r84之后的版本，且仅在Linux下编译通过。**
```
minic [option] source_file
options:
	-d Bison parser debug output
	-t Print AST
	-s Print symbol tables
	-a All of above = -tds
```