# CapFiles

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

## Building

Requirements:
```
- boost
- pcapplusplus
- libpcap
- cli
```
All of them are available in **vcpkg**:
```commandline
$ vcpkg install boost pcapplusplus libpcap cli
```

Simple building using cmake
```commandline
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/toolchain -B ./build
$ cmake --build ./build --config Debug 
```

## Examples

App connects to http://ftp.moex.com/pub/FAST/ASTS/dump/ automatically.
You can download any file from this server using **download** command:
```
CapFiles> download test_eq.cap.gz
Downloading...
```
Also you can unpack to memory and parse file using **unpack** command:
```
CapFiles> unpack test_eq.cap.gz
...
...
...
[Packet 2182369]
Time of arrival: 12:11:50 01-12-2021
Syze: 268 bytes
Sender port: 49109
Receiver port: 16109
```
