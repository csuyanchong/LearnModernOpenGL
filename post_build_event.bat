@echo off
::��Ŀ���ɺ��¼�

::ɾ��bin·���µ�data�ļ���
if EXIST bin\data (rd bin\data /s /q)

::����data�ļ����µ����ݵ�bin�ļ����£����bin�ļ����²�����data�ļ��У�/iָ����Զ�����
Xcopy data bin\data /e /s /i