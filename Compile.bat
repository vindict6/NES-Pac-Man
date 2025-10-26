@rem compile C source file with given name into NES file
@rem useful to compile few projects at once without repeating the build script

@set CC65_HOME=.\lib\

@if "%PATH%"=="%PATH:cc65=%" @PATH=%PATH%;%CC65_HOME%bin\

@.\compiler\ca65 .\lib\crt0.s || goto fail

@.\compiler\cc65 -Oi pacman.c --add-source || goto fail

@.\compiler\ca65 pacman.s || goto fail

@.\compiler\ld65 -C .\compiler\nrom_256_horz.cfg -o pacman.nes .\lib\crt0.o pacman.o .\lib\runtime.lib || goto fail

@goto exit

:fail

pause

:exit

@del pacman.s

del .\lib\*.o
del *.o