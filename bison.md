# 语法分析 #

## 目的 ##
  * 将词法分析器识别的token用[语法规则](BNF.md)规约
  * 识别并报告语法错误 （尚未完成）
  * 建立[抽象语法树AST](AST.md)

## 实现 ##
采用bison自动生成语法分析器，利用flex与bison的接口：
  * `yylex()`：由bison生成的语法分析器调用，负责扫描token，返回终结符类型
  * `yylval`：由bison生成的语法分析器提供，这是一个描述终结符文法属性的变量，类型由'YYSTYPE'指定，本项目中是自定的一个`union`（参见minic.y和attr.h）

## 相关代码 ##

| minic.y | bison源文件 |
|:--------|:---------------|
| AST.h   | 抽象语法树相关结构的定义 |
| AST\_operation.c | [抽象语法树](AST.md)上的相关操作 |
| attr.h  | 文法元素的属性定义 |