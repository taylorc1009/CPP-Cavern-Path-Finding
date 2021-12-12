compile:
	cl data.cpp navigate.cpp /link /out:navigate.exe

clean:
	del *.exe
	del *.obj
	del *.o
	del *.csn

navigate:
	caveroute.bat $(cav)