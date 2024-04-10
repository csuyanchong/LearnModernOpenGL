@echo off
::项目生成后事件

::1.拷贝data文件夹下的内容到bin文件夹下，
::如果bin文件夹下不存在data文件夹，/i指令会自动创建。
::/d检查是否有更新的数据，如果有则覆盖。
Xcopy data bin\data /d /y /e /s /i

::2.拷贝dll文件夹下的内容到bin文件夹下，如果有更新的dll，则覆盖。
Xcopy dll bin /d /y