.c.o:
	gcc -c -g -Wall -Wno-comment -I. $< -o $@

#	gcc -c -g -Wall -Wno-comment -Wno-multichar -I. $< -o $@

dr1: main.o money.o merchant.o apothecary.o dice.o player.o \
	    barter.o itemset.o item.o attr.o registry.o \
	    itemreg.o xdrxml.o playerv.o race.o class.o \
	    smithy.o weapon.o monster.o combatv.o armor.o \
	    ttype.o tanner.o wright.o location.o \
	    potions/heal.o 
	gcc -g -Wl,-warn-common -odr1 main.o money.o merchant.o apothecary.o dice.o \
	player.o barter.o itemset.o item.o attr.o registry.o \
	itemreg.o xdrxml.o playerv.o race.o class.o \
	smithy.o weapon.o monster.o combatv.o armor.o \
	ttype.o tanner.o wright.o location.o \
	potions/heal.o -lxml2 -lm

#xtest: xtest.o xdrxml.o
#	gcc -g -oxtest xtest.o xdrxml.o -lxml2

xtest2: xtest.o xdrxml.o
	gcc -g -oxtest xtest.o xdrxml.o dice.o money.o registry.o item.o itemreg.o armor.o weapon.o potions/heal.o -lxml2

xtest3: xtest.o xdrxml.o
	gcc -g -oxtest xtest.o xdrxml.o dice.o money.o registry.o item.o itemreg.o armor.o weapon.o potions/heal.o class.o race.o location.o itemset.o -lxml2
depend:
	makedepend -I. \
		-I/usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include *.c

test: money.o test.o dice.o
	gcc -g test.o money.o dice.o -o test -lm

tags:
	ctags *.h *.c

clean:
	rm *.o */*.o

test.o: test.c
dice.o: dice.c
player.o: player.c
money.o: money.c
potion/regen.o: potion/regen.c
# DO NOT DELETE


apothecary.o: apothecary.h armor.h attr.h barter.h dice.h globals.h item.h
apothecary.o: itemset.h location.h merchant.h money.h player.h potions/heal.h
apothecary.o: /usr/include/alloca.h /usr/include/asm/param.h
apothecary.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
apothecary.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
apothecary.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
apothecary.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
apothecary.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
apothecary.o: /usr/include/bits/sched.h /usr/include/bits/select.h
apothecary.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
apothecary.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
apothecary.o: /usr/include/bits/time.h /usr/include/bits/types.h
apothecary.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
apothecary.o: /usr/include/bits/wordsize.h /usr/include/endian.h
apothecary.o: /usr/include/features.h /usr/include/_G_config.h
apothecary.o: /usr/include/gconv.h /usr/include/gnu/stubs.h
apothecary.o: /usr/include/libio.h /usr/include/linux/limits.h
apothecary.o: /usr/include/linux/param.h /usr/include/math.h
apothecary.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
apothecary.o: /usr/include/rpc/xdr.h /usr/include/stdint.h
apothecary.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
apothecary.o: /usr/include/sys/cdefs.h /usr/include/sys/param.h
apothecary.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
apothecary.o: /usr/include/sys/time.h /usr/include/sys/types.h
apothecary.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
apothecary.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
apothecary.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
apothecary.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
apothecary.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
apothecary.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
apothecary.o: weapon.h
armor.o: armor.h dice.h globals.h item.h money.h registry.h
armor.o: /usr/include/alloca.h /usr/include/asm/param.h
armor.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
armor.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
armor.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
armor.o: /usr/include/bits/sched.h /usr/include/bits/select.h
armor.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
armor.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
armor.o: /usr/include/bits/time.h /usr/include/bits/types.h
armor.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
armor.o: /usr/include/bits/wordsize.h /usr/include/endian.h
armor.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
armor.o: /usr/include/gnu/stubs.h /usr/include/libio.h
armor.o: /usr/include/linux/limits.h /usr/include/linux/param.h
armor.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
armor.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
armor.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
armor.o: /usr/include/sys/param.h /usr/include/sys/select.h
armor.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
armor.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
armor.o: /usr/include/xlocale.h
armor.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
armor.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
armor.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
armor.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h xdrxml.h
attr.o: attr.h dice.h /usr/include/alloca.h /usr/include/asm/param.h
attr.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
attr.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
attr.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
attr.o: /usr/include/bits/sched.h /usr/include/bits/select.h
attr.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
attr.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
attr.o: /usr/include/bits/time.h /usr/include/bits/types.h
attr.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
attr.o: /usr/include/bits/wordsize.h /usr/include/endian.h
attr.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
attr.o: /usr/include/gnu/stubs.h /usr/include/libio.h
attr.o: /usr/include/linux/limits.h /usr/include/linux/param.h
attr.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
attr.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
attr.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
attr.o: /usr/include/sys/param.h /usr/include/sys/select.h
attr.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
attr.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
attr.o: /usr/include/xlocale.h
attr.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
attr.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
attr.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
attr.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h xdrxml.h
barter.o: barter.h dice.h item.h itemset.h merchant.h money.h
barter.o: /usr/include/alloca.h /usr/include/asm/param.h
barter.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
barter.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
barter.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
barter.o: /usr/include/bits/sched.h /usr/include/bits/select.h
barter.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
barter.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
barter.o: /usr/include/bits/time.h /usr/include/bits/types.h
barter.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
barter.o: /usr/include/bits/wordsize.h /usr/include/endian.h
barter.o: /usr/include/features.h /usr/include/_G_config.h
barter.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
barter.o: /usr/include/linux/limits.h /usr/include/linux/param.h
barter.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
barter.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
barter.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
barter.o: /usr/include/sys/param.h /usr/include/sys/select.h
barter.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
barter.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
barter.o: /usr/include/xlocale.h
barter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
barter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
barter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
barter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
class.o: attr.h class.h dice.h registry.h /usr/include/alloca.h
class.o: /usr/include/asm/param.h /usr/include/asm/socket.h
class.o: /usr/include/asm/sockios.h /usr/include/bits/byteswap.h
class.o: /usr/include/bits/endian.h /usr/include/bits/in.h
class.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
class.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
class.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
class.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
class.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
class.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
class.o: /usr/include/endian.h /usr/include/features.h
class.o: /usr/include/_G_config.h /usr/include/gconv.h
class.o: /usr/include/gnu/stubs.h /usr/include/libio.h
class.o: /usr/include/linux/limits.h /usr/include/linux/param.h
class.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
class.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
class.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
class.o: /usr/include/sys/param.h /usr/include/sys/select.h
class.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
class.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
class.o: /usr/include/xlocale.h
class.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
class.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
class.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
class.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
combatv.o: armor.h attr.h class.h combatv.h dice.h globals.h item.h itemset.h
combatv.o: location.h money.h monster.h player.h registry.h ttype.h
combatv.o: /usr/include/alloca.h /usr/include/asm/param.h
combatv.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
combatv.o: /usr/include/assert.h /usr/include/bits/byteswap.h
combatv.o: /usr/include/bits/endian.h /usr/include/bits/huge_val.h
combatv.o: /usr/include/bits/in.h /usr/include/bits/mathcalls.h
combatv.o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
combatv.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
combatv.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
combatv.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
combatv.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
combatv.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
combatv.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
combatv.o: /usr/include/endian.h /usr/include/features.h
combatv.o: /usr/include/_G_config.h /usr/include/gconv.h
combatv.o: /usr/include/gnu/stubs.h /usr/include/libio.h
combatv.o: /usr/include/linux/limits.h /usr/include/linux/param.h
combatv.o: /usr/include/math.h /usr/include/netinet/in.h
combatv.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
combatv.o: /usr/include/stdint.h /usr/include/stdio.h /usr/include/stdlib.h
combatv.o: /usr/include/string.h /usr/include/sys/cdefs.h
combatv.o: /usr/include/sys/param.h /usr/include/sys/select.h
combatv.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
combatv.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
combatv.o: /usr/include/xlocale.h
combatv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
combatv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
combatv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
combatv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
combatv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
combatv.o: weapon.h
dice.o: dice.h /usr/include/alloca.h /usr/include/asm/param.h
dice.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
dice.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
dice.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
dice.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
dice.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
dice.o: /usr/include/bits/sched.h /usr/include/bits/select.h
dice.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
dice.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
dice.o: /usr/include/bits/time.h /usr/include/bits/types.h
dice.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
dice.o: /usr/include/bits/wordsize.h /usr/include/endian.h
dice.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
dice.o: /usr/include/gnu/stubs.h /usr/include/libio.h
dice.o: /usr/include/linux/limits.h /usr/include/linux/param.h
dice.o: /usr/include/math.h /usr/include/netinet/in.h /usr/include/rpc/types.h
dice.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
dice.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
dice.o: /usr/include/sys/param.h /usr/include/sys/select.h
dice.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
dice.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
dice.o: /usr/include/xlocale.h
dice.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
dice.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
dice.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
dice.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
dice.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
fooread.o: /usr/include/alloca.h /usr/include/assert.h
fooread.o: /usr/include/bits/endian.h /usr/include/bits/pthreadtypes.h
fooread.o: /usr/include/bits/sched.h /usr/include/bits/select.h
fooread.o: /usr/include/bits/sigset.h /usr/include/bits/stdio_lim.h
fooread.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
fooread.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
fooread.o: /usr/include/features.h /usr/include/_G_config.h
fooread.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/iconv.h
fooread.o: /usr/include/libio.h /usr/include/stdio.h /usr/include/stdlib.h
fooread.o: /usr/include/string.h /usr/include/sys/cdefs.h
fooread.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
fooread.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
fooread.o: /usr/include/xlocale.h
fooread.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
fooread.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
fooread.o: /usr/local/include/libxml/encoding.h
fooread.o: /usr/local/include/libxml/entities.h
fooread.o: /usr/local/include/libxml/hash.h /usr/local/include/libxml/list.h
fooread.o: /usr/local/include/libxml/parser.h /usr/local/include/libxml/tree.h
fooread.o: /usr/local/include/libxml/valid.h /usr/local/include/libxml/xmlIO.h
fooread.o: /usr/local/include/libxml/xmlmemory.h
fooread.o: /usr/local/include/libxml/xmlversion.h
fooread.o: /usr/local/include/libxml/xpath.h
fooread.o: /usr/local/include/libxml/xpointer.h
item.o: armor.h dice.h globals.h item.h itemreg.h money.h registry.h
item.o: /usr/include/alloca.h /usr/include/asm/param.h
item.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
item.o: /usr/include/assert.h /usr/include/bits/byteswap.h
item.o: /usr/include/bits/endian.h /usr/include/bits/in.h
item.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
item.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
item.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
item.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
item.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
item.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
item.o: /usr/include/endian.h /usr/include/features.h /usr/include/_G_config.h
item.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
item.o: /usr/include/linux/limits.h /usr/include/linux/param.h
item.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
item.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
item.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
item.o: /usr/include/sys/param.h /usr/include/sys/select.h
item.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
item.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
item.o: /usr/include/xlocale.h
item.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
item.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
item.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
item.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h xdrxml.h
itemreg.o: armor.h attr.h dice.h globals.h item.h itemreg.h itemset.h
itemreg.o: location.h money.h player.h potions/heal.h registry.h
itemreg.o: /usr/include/alloca.h /usr/include/asm/param.h
itemreg.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
itemreg.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
itemreg.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
itemreg.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
itemreg.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
itemreg.o: /usr/include/bits/sched.h /usr/include/bits/select.h
itemreg.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
itemreg.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
itemreg.o: /usr/include/bits/time.h /usr/include/bits/types.h
itemreg.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
itemreg.o: /usr/include/bits/wordsize.h /usr/include/endian.h
itemreg.o: /usr/include/features.h /usr/include/_G_config.h
itemreg.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
itemreg.o: /usr/include/linux/limits.h /usr/include/linux/param.h
itemreg.o: /usr/include/math.h /usr/include/netinet/in.h
itemreg.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
itemreg.o: /usr/include/stdint.h /usr/include/stdio.h /usr/include/stdlib.h
itemreg.o: /usr/include/string.h /usr/include/sys/cdefs.h
itemreg.o: /usr/include/sys/param.h /usr/include/sys/select.h
itemreg.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
itemreg.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
itemreg.o: /usr/include/xlocale.h
itemreg.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
itemreg.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
itemreg.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
itemreg.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
itemreg.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
itemreg.o: weapon.h
itemset.o: item.h itemset.h /usr/include/alloca.h /usr/include/asm/param.h
itemset.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
itemset.o: /usr/include/assert.h /usr/include/bits/byteswap.h
itemset.o: /usr/include/bits/endian.h /usr/include/bits/in.h
itemset.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
itemset.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
itemset.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
itemset.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
itemset.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
itemset.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
itemset.o: /usr/include/endian.h /usr/include/features.h
itemset.o: /usr/include/_G_config.h /usr/include/gconv.h
itemset.o: /usr/include/gnu/stubs.h /usr/include/libio.h
itemset.o: /usr/include/linux/limits.h /usr/include/linux/param.h
itemset.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
itemset.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
itemset.o: /usr/include/stdlib.h /usr/include/string.h
itemset.o: /usr/include/sys/cdefs.h /usr/include/sys/param.h
itemset.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
itemset.o: /usr/include/sys/time.h /usr/include/sys/types.h
itemset.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
itemset.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
itemset.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
itemset.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
itemset.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
itemset.o: xdrxml.h
location.o: location.h /usr/include/alloca.h /usr/include/asm/param.h
location.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
location.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
location.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
location.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
location.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
location.o: /usr/include/bits/sched.h /usr/include/bits/select.h
location.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
location.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
location.o: /usr/include/bits/time.h /usr/include/bits/types.h
location.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
location.o: /usr/include/bits/wordsize.h /usr/include/endian.h
location.o: /usr/include/features.h /usr/include/_G_config.h
location.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
location.o: /usr/include/linux/limits.h /usr/include/linux/param.h
location.o: /usr/include/math.h /usr/include/netinet/in.h
location.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
location.o: /usr/include/stdint.h /usr/include/stdio.h /usr/include/stdlib.h
location.o: /usr/include/sys/cdefs.h /usr/include/sys/param.h
location.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
location.o: /usr/include/sys/time.h /usr/include/sys/types.h
location.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
location.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
location.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
location.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
location.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
location.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
location.o: xdrxml.h
main.o: apothecary.h armor.h attr.h barter.h combatv.h dice.h globals.h item.h
main.o: itemset.h location.h merchant.h money.h monster.h player.h playerv.h
main.o: registry.h smithy.h tanner.h /usr/include/alloca.h
main.o: /usr/include/asm/param.h /usr/include/asm/socket.h
main.o: /usr/include/asm/sockios.h /usr/include/assert.h
main.o: /usr/include/bits/byteswap.h /usr/include/bits/confname.h
main.o: /usr/include/bits/endian.h /usr/include/bits/environments.h
main.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
main.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
main.o: /usr/include/bits/nan.h /usr/include/bits/posix_opt.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
main.o: /usr/include/bits/stat.h /usr/include/bits/stdio_lim.h
main.o: /usr/include/bits/time.h /usr/include/bits/types.h
main.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
main.o: /usr/include/bits/wordsize.h /usr/include/endian.h
main.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
main.o: /usr/include/getopt.h /usr/include/gnu/stubs.h /usr/include/iconv.h
main.o: /usr/include/libio.h /usr/include/linux/limits.h
main.o: /usr/include/linux/param.h /usr/include/math.h
main.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
main.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
main.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
main.o: /usr/include/sys/param.h /usr/include/sys/select.h
main.o: /usr/include/sys/stat.h /usr/include/sys/sysmacros.h
main.o: /usr/include/sys/time.h /usr/include/sys/types.h /usr/include/time.h
main.o: /usr/include/unistd.h /usr/include/wchar.h /usr/include/xlocale.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
main.o: /usr/local/include/libxml/encoding.h
main.o: /usr/local/include/libxml/entities.h /usr/local/include/libxml/hash.h
main.o: /usr/local/include/libxml/list.h /usr/local/include/libxml/parser.h
main.o: /usr/local/include/libxml/tree.h /usr/local/include/libxml/valid.h
main.o: /usr/local/include/libxml/xmlIO.h
main.o: /usr/local/include/libxml/xmlmemory.h
main.o: /usr/local/include/libxml/xmlversion.h
main.o: /usr/local/include/libxml/xpath.h /usr/local/include/libxml/xpointer.h
main.o: weapon.h wright.h xdrxml.h
merchant.o: barter.h dice.h item.h itemset.h merchant.h money.h
merchant.o: /usr/include/alloca.h /usr/include/asm/param.h
merchant.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
merchant.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
merchant.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
merchant.o: /usr/include/bits/sched.h /usr/include/bits/select.h
merchant.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
merchant.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
merchant.o: /usr/include/bits/time.h /usr/include/bits/types.h
merchant.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
merchant.o: /usr/include/bits/wordsize.h /usr/include/endian.h
merchant.o: /usr/include/features.h /usr/include/_G_config.h
merchant.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
merchant.o: /usr/include/linux/limits.h /usr/include/linux/param.h
merchant.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
merchant.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
merchant.o: /usr/include/stdlib.h /usr/include/string.h
merchant.o: /usr/include/sys/cdefs.h /usr/include/sys/param.h
merchant.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
merchant.o: /usr/include/sys/time.h /usr/include/sys/types.h
merchant.o: /usr/include/time.h /usr/include/wchar.h /usr/include/xlocale.h
merchant.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
merchant.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
merchant.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
merchant.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
money.o: money.h /usr/include/alloca.h /usr/include/asm/param.h
money.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
money.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
money.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
money.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
money.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
money.o: /usr/include/bits/sched.h /usr/include/bits/select.h
money.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
money.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
money.o: /usr/include/bits/time.h /usr/include/bits/types.h
money.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
money.o: /usr/include/bits/wordsize.h /usr/include/ctype.h
money.o: /usr/include/endian.h /usr/include/features.h
money.o: /usr/include/_G_config.h /usr/include/gconv.h
money.o: /usr/include/gnu/stubs.h /usr/include/libio.h
money.o: /usr/include/linux/limits.h /usr/include/linux/param.h
money.o: /usr/include/math.h /usr/include/netinet/in.h
money.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h /usr/include/stdint.h
money.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
money.o: /usr/include/sys/cdefs.h /usr/include/sys/param.h
money.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
money.o: /usr/include/sys/time.h /usr/include/sys/types.h /usr/include/time.h
money.o: /usr/include/wchar.h /usr/include/xlocale.h
money.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
money.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
money.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
money.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
money.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h xdrxml.h
monster.o: armor.h attr.h dice.h globals.h item.h itemset.h location.h money.h
monster.o: monster.h player.h registry.h /usr/include/alloca.h
monster.o: /usr/include/asm/param.h /usr/include/asm/socket.h
monster.o: /usr/include/asm/sockios.h /usr/include/bits/byteswap.h
monster.o: /usr/include/bits/endian.h /usr/include/bits/huge_val.h
monster.o: /usr/include/bits/in.h /usr/include/bits/mathcalls.h
monster.o: /usr/include/bits/mathdef.h /usr/include/bits/nan.h
monster.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
monster.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
monster.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
monster.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
monster.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
monster.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
monster.o: /usr/include/endian.h /usr/include/features.h
monster.o: /usr/include/_G_config.h /usr/include/gconv.h
monster.o: /usr/include/gnu/stubs.h /usr/include/libio.h
monster.o: /usr/include/linux/limits.h /usr/include/linux/param.h
monster.o: /usr/include/math.h /usr/include/netinet/in.h
monster.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
monster.o: /usr/include/stdint.h /usr/include/stdio.h /usr/include/stdlib.h
monster.o: /usr/include/string.h /usr/include/sys/cdefs.h
monster.o: /usr/include/sys/param.h /usr/include/sys/select.h
monster.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
monster.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
monster.o: /usr/include/xlocale.h
monster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
monster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
monster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
monster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
monster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
monster.o: weapon.h
player.o: armor.h attr.h class.h dice.h globals.h item.h itemset.h location.h
player.o: money.h player.h registry.h /usr/include/alloca.h
player.o: /usr/include/asm/param.h /usr/include/asm/socket.h
player.o: /usr/include/asm/sockios.h /usr/include/assert.h
player.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
player.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
player.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
player.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
player.o: /usr/include/bits/sched.h /usr/include/bits/select.h
player.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
player.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
player.o: /usr/include/bits/time.h /usr/include/bits/types.h
player.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
player.o: /usr/include/bits/wordsize.h /usr/include/endian.h
player.o: /usr/include/features.h /usr/include/_G_config.h
player.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
player.o: /usr/include/linux/limits.h /usr/include/linux/param.h
player.o: /usr/include/math.h /usr/include/netinet/in.h
player.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
player.o: /usr/include/stdint.h /usr/include/stdio.h /usr/include/stdlib.h
player.o: /usr/include/string.h /usr/include/sys/cdefs.h
player.o: /usr/include/sys/param.h /usr/include/sys/select.h
player.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
player.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
player.o: /usr/include/xlocale.h
player.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
player.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
player.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
player.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
player.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h weapon.h
player.o: xdrxml.h
playerv.o: armor.h attr.h class.h dice.h globals.h item.h itemset.h location.h
playerv.o: money.h player.h playerv.h race.h registry.h /usr/include/alloca.h
playerv.o: /usr/include/asm/param.h /usr/include/asm/socket.h
playerv.o: /usr/include/asm/sockios.h /usr/include/assert.h
playerv.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
playerv.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
playerv.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
playerv.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
playerv.o: /usr/include/bits/sched.h /usr/include/bits/select.h
playerv.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
playerv.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
playerv.o: /usr/include/bits/time.h /usr/include/bits/types.h
playerv.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
playerv.o: /usr/include/bits/wordsize.h /usr/include/endian.h
playerv.o: /usr/include/features.h /usr/include/_G_config.h
playerv.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
playerv.o: /usr/include/linux/limits.h /usr/include/linux/param.h
playerv.o: /usr/include/math.h /usr/include/netinet/in.h
playerv.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h
playerv.o: /usr/include/stdint.h /usr/include/stdio.h /usr/include/stdlib.h
playerv.o: /usr/include/string.h /usr/include/sys/cdefs.h
playerv.o: /usr/include/sys/param.h /usr/include/sys/select.h
playerv.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
playerv.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
playerv.o: /usr/include/xlocale.h
playerv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
playerv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
playerv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
playerv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
playerv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
playerv.o: weapon.h
race.o: attr.h race.h registry.h /usr/include/alloca.h
race.o: /usr/include/asm/param.h /usr/include/asm/socket.h
race.o: /usr/include/asm/sockios.h /usr/include/bits/byteswap.h
race.o: /usr/include/bits/endian.h /usr/include/bits/in.h
race.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
race.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
race.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
race.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
race.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
race.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
race.o: /usr/include/endian.h /usr/include/features.h /usr/include/_G_config.h
race.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
race.o: /usr/include/linux/limits.h /usr/include/linux/param.h
race.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
race.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
race.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
race.o: /usr/include/sys/param.h /usr/include/sys/select.h
race.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
race.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
race.o: /usr/include/xlocale.h
race.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
race.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
race.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
race.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
registry.o: registry.h
smithy.o: armor.h barter.h dice.h globals.h item.h itemset.h merchant.h
smithy.o: money.h smithy.h /usr/include/alloca.h /usr/include/asm/param.h
smithy.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
smithy.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
smithy.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
smithy.o: /usr/include/bits/sched.h /usr/include/bits/select.h
smithy.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
smithy.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
smithy.o: /usr/include/bits/time.h /usr/include/bits/types.h
smithy.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
smithy.o: /usr/include/bits/wordsize.h /usr/include/endian.h
smithy.o: /usr/include/features.h /usr/include/_G_config.h
smithy.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
smithy.o: /usr/include/linux/limits.h /usr/include/linux/param.h
smithy.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
smithy.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
smithy.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
smithy.o: /usr/include/sys/param.h /usr/include/sys/select.h
smithy.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
smithy.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
smithy.o: /usr/include/xlocale.h
smithy.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
smithy.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
smithy.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
smithy.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h weapon.h
tanner.o: armor.h barter.h dice.h globals.h item.h itemset.h merchant.h
tanner.o: money.h tanner.h /usr/include/alloca.h /usr/include/asm/param.h
tanner.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
tanner.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
tanner.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
tanner.o: /usr/include/bits/sched.h /usr/include/bits/select.h
tanner.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
tanner.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
tanner.o: /usr/include/bits/time.h /usr/include/bits/types.h
tanner.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
tanner.o: /usr/include/bits/wordsize.h /usr/include/endian.h
tanner.o: /usr/include/features.h /usr/include/_G_config.h
tanner.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
tanner.o: /usr/include/linux/limits.h /usr/include/linux/param.h
tanner.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
tanner.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
tanner.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
tanner.o: /usr/include/sys/param.h /usr/include/sys/select.h
tanner.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
tanner.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
tanner.o: /usr/include/xlocale.h
tanner.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
tanner.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
tanner.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
tanner.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
test.o: dice.h money.h /usr/include/alloca.h /usr/include/asm/param.h
test.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
test.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
test.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
test.o: /usr/include/bits/sched.h /usr/include/bits/select.h
test.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
test.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
test.o: /usr/include/bits/time.h /usr/include/bits/types.h
test.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
test.o: /usr/include/bits/wordsize.h /usr/include/endian.h
test.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
test.o: /usr/include/gnu/stubs.h /usr/include/libio.h
test.o: /usr/include/linux/limits.h /usr/include/linux/param.h
test.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
test.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
test.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
test.o: /usr/include/sys/param.h /usr/include/sys/select.h
test.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
test.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
test.o: /usr/include/xlocale.h
test.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
test.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
test.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
test.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
timeq.o: armor.h attr.h dice.h globals.h item.h itemset.h location.h money.h
timeq.o: player.h timeq.h /usr/include/alloca.h /usr/include/asm/param.h
timeq.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
timeq.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
timeq.o: /usr/include/bits/huge_val.h /usr/include/bits/in.h
timeq.o: /usr/include/bits/mathcalls.h /usr/include/bits/mathdef.h
timeq.o: /usr/include/bits/nan.h /usr/include/bits/pthreadtypes.h
timeq.o: /usr/include/bits/sched.h /usr/include/bits/select.h
timeq.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
timeq.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
timeq.o: /usr/include/bits/time.h /usr/include/bits/types.h
timeq.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
timeq.o: /usr/include/bits/wordsize.h /usr/include/endian.h
timeq.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
timeq.o: /usr/include/gnu/stubs.h /usr/include/libio.h
timeq.o: /usr/include/linux/limits.h /usr/include/linux/param.h
timeq.o: /usr/include/math.h /usr/include/netinet/in.h
timeq.o: /usr/include/rpc/types.h /usr/include/rpc/xdr.h /usr/include/stdint.h
timeq.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
timeq.o: /usr/include/sys/cdefs.h /usr/include/sys/param.h
timeq.o: /usr/include/sys/select.h /usr/include/sys/sysmacros.h
timeq.o: /usr/include/sys/time.h /usr/include/sys/types.h /usr/include/time.h
timeq.o: /usr/include/wchar.h /usr/include/xlocale.h
timeq.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
timeq.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
timeq.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
timeq.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
timeq.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h weapon.h
ttype.o: dice.h money.h ttype.h /usr/include/alloca.h /usr/include/asm/param.h
ttype.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
ttype.o: /usr/include/assert.h /usr/include/bits/byteswap.h
ttype.o: /usr/include/bits/endian.h /usr/include/bits/in.h
ttype.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
ttype.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
ttype.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
ttype.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
ttype.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
ttype.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
ttype.o: /usr/include/endian.h /usr/include/features.h
ttype.o: /usr/include/_G_config.h /usr/include/gconv.h
ttype.o: /usr/include/gnu/stubs.h /usr/include/libio.h
ttype.o: /usr/include/linux/limits.h /usr/include/linux/param.h
ttype.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
ttype.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
ttype.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
ttype.o: /usr/include/sys/param.h /usr/include/sys/select.h
ttype.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
ttype.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
ttype.o: /usr/include/xlocale.h
ttype.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ttype.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
ttype.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ttype.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
weapon.o: dice.h globals.h item.h money.h /usr/include/alloca.h
weapon.o: /usr/include/asm/param.h /usr/include/asm/socket.h
weapon.o: /usr/include/asm/sockios.h /usr/include/bits/byteswap.h
weapon.o: /usr/include/bits/endian.h /usr/include/bits/in.h
weapon.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
weapon.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
weapon.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
weapon.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
weapon.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
weapon.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
weapon.o: /usr/include/endian.h /usr/include/features.h
weapon.o: /usr/include/_G_config.h /usr/include/gconv.h
weapon.o: /usr/include/gnu/stubs.h /usr/include/libio.h
weapon.o: /usr/include/linux/limits.h /usr/include/linux/param.h
weapon.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
weapon.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
weapon.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
weapon.o: /usr/include/sys/param.h /usr/include/sys/select.h
weapon.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
weapon.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
weapon.o: /usr/include/xlocale.h
weapon.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
weapon.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
weapon.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
weapon.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h weapon.h
weapon.o: xdrxml.h
wright.o: barter.h dice.h globals.h item.h itemset.h merchant.h money.h
wright.o: /usr/include/alloca.h /usr/include/asm/param.h
wright.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
wright.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
wright.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
wright.o: /usr/include/bits/sched.h /usr/include/bits/select.h
wright.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
wright.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
wright.o: /usr/include/bits/time.h /usr/include/bits/types.h
wright.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
wright.o: /usr/include/bits/wordsize.h /usr/include/endian.h
wright.o: /usr/include/features.h /usr/include/_G_config.h
wright.o: /usr/include/gconv.h /usr/include/gnu/stubs.h /usr/include/libio.h
wright.o: /usr/include/linux/limits.h /usr/include/linux/param.h
wright.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
wright.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
wright.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
wright.o: /usr/include/sys/param.h /usr/include/sys/select.h
wright.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
wright.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
wright.o: /usr/include/xlocale.h
wright.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
wright.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
wright.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
wright.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h weapon.h
wright.o: wright.h
xdrxml.o: /usr/include/alloca.h /usr/include/asm/param.h
xdrxml.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
xdrxml.o: /usr/include/assert.h /usr/include/bits/byteswap.h
xdrxml.o: /usr/include/bits/endian.h /usr/include/bits/in.h
xdrxml.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
xdrxml.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
xdrxml.o: /usr/include/bits/sockaddr.h /usr/include/bits/socket.h
xdrxml.o: /usr/include/bits/stdio_lim.h /usr/include/bits/time.h
xdrxml.o: /usr/include/bits/types.h /usr/include/bits/waitflags.h
xdrxml.o: /usr/include/bits/waitstatus.h /usr/include/bits/wordsize.h
xdrxml.o: /usr/include/endian.h /usr/include/features.h
xdrxml.o: /usr/include/_G_config.h /usr/include/gconv.h
xdrxml.o: /usr/include/gnu/stubs.h /usr/include/iconv.h /usr/include/libio.h
xdrxml.o: /usr/include/linux/limits.h /usr/include/linux/param.h
xdrxml.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
xdrxml.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
xdrxml.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/cdefs.h
xdrxml.o: /usr/include/sys/param.h /usr/include/sys/select.h
xdrxml.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
xdrxml.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
xdrxml.o: /usr/include/xlocale.h
xdrxml.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
xdrxml.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
xdrxml.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
xdrxml.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
xdrxml.o: /usr/local/include/libxml/encoding.h
xdrxml.o: /usr/local/include/libxml/entities.h
xdrxml.o: /usr/local/include/libxml/hash.h /usr/local/include/libxml/list.h
xdrxml.o: /usr/local/include/libxml/parser.h /usr/local/include/libxml/tree.h
xdrxml.o: /usr/local/include/libxml/valid.h /usr/local/include/libxml/xmlIO.h
xdrxml.o: /usr/local/include/libxml/xmlmemory.h
xdrxml.o: /usr/local/include/libxml/xmlversion.h
xdrxml.o: /usr/local/include/libxml/xpath.h
xdrxml.o: /usr/local/include/libxml/xpointer.h xdrxml.h
xtest.o: /usr/include/alloca.h /usr/include/asm/param.h
xtest.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
xtest.o: /usr/include/bits/byteswap.h /usr/include/bits/endian.h
xtest.o: /usr/include/bits/in.h /usr/include/bits/pthreadtypes.h
xtest.o: /usr/include/bits/sched.h /usr/include/bits/select.h
xtest.o: /usr/include/bits/sigset.h /usr/include/bits/sockaddr.h
xtest.o: /usr/include/bits/socket.h /usr/include/bits/stdio_lim.h
xtest.o: /usr/include/bits/time.h /usr/include/bits/types.h
xtest.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
xtest.o: /usr/include/bits/wordsize.h /usr/include/endian.h
xtest.o: /usr/include/features.h /usr/include/_G_config.h /usr/include/gconv.h
xtest.o: /usr/include/gnu/stubs.h /usr/include/iconv.h /usr/include/libio.h
xtest.o: /usr/include/linux/limits.h /usr/include/linux/param.h
xtest.o: /usr/include/netinet/in.h /usr/include/rpc/types.h
xtest.o: /usr/include/rpc/xdr.h /usr/include/stdint.h /usr/include/stdio.h
xtest.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
xtest.o: /usr/include/sys/param.h /usr/include/sys/select.h
xtest.o: /usr/include/sys/sysmacros.h /usr/include/sys/time.h
xtest.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/wchar.h
xtest.o: /usr/include/xlocale.h
xtest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
xtest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
xtest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
xtest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h
xtest.o: /usr/local/include/libxml/encoding.h
xtest.o: /usr/local/include/libxml/entities.h /usr/local/include/libxml/hash.h
xtest.o: /usr/local/include/libxml/list.h /usr/local/include/libxml/parser.h
xtest.o: /usr/local/include/libxml/tree.h /usr/local/include/libxml/valid.h
xtest.o: /usr/local/include/libxml/xmlIO.h
xtest.o: /usr/local/include/libxml/xmlmemory.h
xtest.o: /usr/local/include/libxml/xmlversion.h
xtest.o: /usr/local/include/libxml/xpath.h
xtest.o: /usr/local/include/libxml/xpointer.h xdrxml.h
