@REM このバッチファイルはIAR Embedded WorkbenchC-SPYデバッガによって
@REM  適切な設定を使用するcspybatコマンドライン
@REM ユーティリティを実行するためのコマンドラインの準備を支援するものとして生成されています。
@REM
@REM このファイルは新しいデバッグセッションが初期化されるたびに生成されるため、
@REM ファイルを名称変更または移動してから
@REM 変更を行うことを推奨します。
@REM
@REM cspybatは、このバッチファイル名に続いてデバッグファイル名(通常はELF/DWARFまたはUBROFファイル)を入力することにより、
@REM 起動できます。
@REM
@REM 使用可能なコマンドラインのパラメータについては、C-SPYデバッグガイドを参照してください。
@REM 特定の場合に役立つコマンドラインパラメータに関する他のヒント
@REM :
@REM   --download_only   後にデバッグセッションを起動せずにコードのイメージをダウンロードします
@REM                     。
@REM   --silent          サインオンのメッセージを省略します。
@REM   --timeout         実行時間の上限を設定します。
@REM 


@echo off 

if not "%~1" == "" goto debugFile 

@echo on 

"D:\IAR\EWARM8.42.2\common\bin\cspybat" -f "D:\temp\STLDR\W25Q128_STM32F412ZG\settings\W25Q128_STM32F412ZG.W25Q128_STM32F412ZG.general.xcl" --backend -f "D:\temp\STLDR\W25Q128_STM32F412ZG\settings\W25Q128_STM32F412ZG.W25Q128_STM32F412ZG.driver.xcl" 

@echo off 
goto end 

:debugFile 

@echo on 

"D:\IAR\EWARM8.42.2\common\bin\cspybat" -f "D:\temp\STLDR\W25Q128_STM32F412ZG\settings\W25Q128_STM32F412ZG.W25Q128_STM32F412ZG.general.xcl" "--debug_file=%~1" --backend -f "D:\temp\STLDR\W25Q128_STM32F412ZG\settings\W25Q128_STM32F412ZG.W25Q128_STM32F412ZG.driver.xcl" 

@echo off 
:end