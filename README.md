# CapFiles

### Решение задачи CapFiles

Приложение предоставляет Command-Line Interface для взаимодействия с подпрограммами:
```
CapFiles> help
Commands available:
 - help
	This help message
 - exit
	Quit the session
 - get <string>
	Get file from server by filename
 - unpack <string>
	Unpack file in memory
```

## Сборка

Требования:
```
- boost
- pcapplusplus
- libpcap
- cli
```
Все они доступны в пакетном менеджере **vcpkg**:
```commandline
$ vcpkg install boost pcapplusplus libpcap cli
```

Далее обычная сборка с использованием CMake
```commandline
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake -B ./build
$ cmake --build ./build --config Debug 
```