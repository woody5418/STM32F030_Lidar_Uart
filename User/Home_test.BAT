SET PATH=D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin;C:\Program Files (x86)\AMD APP\bin\x86_64;C:\Program Files (x86)\AMD APP\bin\x86;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Python27\;C:\Python27\Scripts;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Users\houti\.dnx\bin;C:\Program Files\Microsoft DNX\Dnvm\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;D:\EmbeddedTools\QT5.9.1\5.9.1\mingw53_32\bin;C:\Program Files\dotnet\;D:\Program Files\Java\jdk-10.0.1\bin;D:\Program Files\Java\jdk-10.0.1\jre\bin;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;D:\EmbeddedTools\MS_MCU\HI-TECH Software\PICC\9.83\bin;;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Users\houti\AppData\Local\Microsoft\WindowsApps;;C:\Program Files (x86)\SSH Communications Security\SSH Secure Shell
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Home_test
SET CPU_CLOCK=0x00B71B00
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\main.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f10x_it.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\system_stm32f10x.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer2.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer3.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer4.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\usart1.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\usart2.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\led.__i"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\ArmLink" --Via "..\Obj\STM32F103_Home.lnp"
"D:\EmbeddedTools\MDK51_RAM\ARM\ARMCC\Bin\fromelf.exe" "..\Obj\STM32F103_Home.axf" --i32combined --output "..\Obj\STM32F103_Home.hex"
