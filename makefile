edit: lock.exe unlock.exe genkey.exe
lock.exe: lock.c
	gcc -Wall -std=c11 lock.c -o lock.exe
unlock.exe: unlock.c
	gcc -Wall -std=c11 unlock.c -o unlock.exe
genkey.exe: genkey.c
	gcc -Wall -std=c11 genkey.c -o genkey.exe
