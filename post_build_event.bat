@echo off
::项目生成后事件

::删除bin路径下的data文件夹
if EXIST bin\data (rd bin\data /s /q)

::拷贝data文件夹下的内容到bin文件夹下，如果bin文件夹下不存在data文件夹，/i指令会自动创建
Xcopy data bin\data /e /s /i