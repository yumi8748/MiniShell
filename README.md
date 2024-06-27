
to do:
在 Bash 中，双引号(`"`)内的字符串会进行以下几种类型的扩展：

1. **参数和变量扩展**：形如`$var`或`${var}`的表达式会被替换为变量`var`的值。如果`var`是一个位置参数或特殊参数，如`$1`或`$@`，则会被替换为对应的参数值。

2. **命令替换**：形如`$(command)`或`` `command` ``的表达式会被替换为`command`命令的输出。

3. **算术扩展**：形如`$((expression))`的表达式会被替换为expression的计算结果。


- [ ] expand variable  添加 {}  ()  算术扩展？
- [ ] norme
- [ ] test.sh 里面的case
- [ ] tester
- [ ] evaluation sheet一一測試
