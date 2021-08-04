
PlusSamplesps.dll: dlldata.obj PlusSamples_p.obj PlusSamples_i.obj
	link /dll /out:PlusSamplesps.dll /def:PlusSamplesps.def /entry:DllMain dlldata.obj PlusSamples_p.obj PlusSamples_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PlusSamplesps.dll
	@del PlusSamplesps.lib
	@del PlusSamplesps.exp
	@del dlldata.obj
	@del PlusSamples_p.obj
	@del PlusSamples_i.obj
