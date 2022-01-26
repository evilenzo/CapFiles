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
 - download <string>
	Download file from server by filename
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

Далее обычная сборка с использованием CMake:
```commandline
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake -B ./build
$ cmake --build ./build --config Debug 
```

## Примеры

Программа устанавливает подключение к http://ftp.moex.com/pub/FAST/ASTS/dump/ автоматически.
Используя команду **download** можно скачать файл с данного эндпоинта:
```
CapFiles> download test_eq.cap.gz
Downloading...
```
Распаковка в память и парсинг представлены модулем Archiver и осуществляются по команде **unpack**:
```
CapFiles> unpack test_eq.cap.gz
...
...
...
[Пакет 2182369]
Время получения: 12:11:50 01-12-2021
Размер: 268 байта
Порт отправителя: 49109
Порт получателя: 16109
```