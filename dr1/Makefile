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

xtest: xtest.o xdrxml.o
	gcc -g -oxtest xtest.o xdrxml.o -lxml2

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

apothecary.o: dice.h /usr/include/string.h /usr/include/features.h
apothecary.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
apothecary.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
apothecary.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
apothecary.o: /usr/include/bits/types.h /usr/include/time.h
apothecary.o: /usr/include/endian.h /usr/include/bits/endian.h
apothecary.o: /usr/include/sys/select.h /usr/include/bits/select.h
apothecary.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
apothecary.o: /usr/include/rpc/types.h /usr/include/stdlib.h
apothecary.o: /usr/include/alloca.h /usr/include/sys/time.h
apothecary.o: /usr/include/bits/time.h /usr/include/sys/param.h
apothecary.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
apothecary.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
apothecary.o: /usr/include/linux/limits.h /usr/include/linux/param.h
apothecary.o: /usr/include/asm/param.h /usr/include/netinet/in.h
apothecary.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
apothecary.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
apothecary.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
apothecary.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
apothecary.o: /usr/include/stdio.h
apothecary.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
apothecary.o: /usr/include/libio.h /usr/include/_G_config.h
apothecary.o: /usr/include/bits/stdio_lim.h apothecary.h merchant.h item.h
apothecary.o: money.h barter.h itemset.h potions/heal.h player.h attr.h
apothecary.o: weapon.h globals.h armor.h location.h /usr/include/math.h
apothecary.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
apothecary.o: /usr/include/bits/mathcalls.h
apothecary.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
armor.o: registry.h armor.h xdrxml.h /usr/include/rpc/xdr.h
armor.o: /usr/include/features.h /usr/include/sys/cdefs.h
armor.o: /usr/include/gnu/stubs.h /usr/include/sys/types.h
armor.o: /usr/include/bits/types.h
armor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
armor.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
armor.o: /usr/include/sys/select.h /usr/include/bits/select.h
armor.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
armor.o: /usr/include/rpc/types.h /usr/include/stdlib.h /usr/include/alloca.h
armor.o: /usr/include/sys/time.h /usr/include/bits/time.h
armor.o: /usr/include/sys/param.h
armor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
armor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
armor.o: /usr/include/linux/limits.h /usr/include/linux/param.h
armor.o: /usr/include/asm/param.h /usr/include/netinet/in.h
armor.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
armor.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
armor.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
armor.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
armor.o: /usr/include/stdio.h
armor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
armor.o: /usr/include/libio.h /usr/include/_G_config.h
armor.o: /usr/include/bits/stdio_lim.h item.h money.h
attr.o: /usr/include/stdlib.h /usr/include/features.h
attr.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
attr.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
attr.o: /usr/include/sys/types.h /usr/include/bits/types.h
attr.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
attr.o: /usr/include/sys/select.h /usr/include/bits/select.h
attr.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
attr.o: /usr/include/alloca.h attr.h dice.h /usr/include/string.h
attr.o: /usr/include/rpc/xdr.h /usr/include/rpc/types.h
attr.o: /usr/include/sys/time.h /usr/include/bits/time.h
attr.o: /usr/include/sys/param.h
attr.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
attr.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
attr.o: /usr/include/linux/limits.h /usr/include/linux/param.h
attr.o: /usr/include/asm/param.h /usr/include/netinet/in.h
attr.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
attr.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
attr.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
attr.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
attr.o: /usr/include/stdio.h
attr.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
attr.o: /usr/include/libio.h /usr/include/_G_config.h
attr.o: /usr/include/bits/stdio_lim.h xdrxml.h
barter.o: /usr/include/string.h /usr/include/features.h
barter.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
barter.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
barter.o: barter.h item.h merchant.h money.h itemset.h dice.h
barter.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
barter.o: /usr/include/bits/types.h /usr/include/time.h /usr/include/endian.h
barter.o: /usr/include/bits/endian.h /usr/include/sys/select.h
barter.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
barter.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
barter.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
barter.o: /usr/include/bits/time.h /usr/include/sys/param.h
barter.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
barter.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
barter.o: /usr/include/linux/limits.h /usr/include/linux/param.h
barter.o: /usr/include/asm/param.h /usr/include/netinet/in.h
barter.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
barter.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
barter.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
barter.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
barter.o: /usr/include/stdio.h
barter.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
barter.o: /usr/include/libio.h /usr/include/_G_config.h
barter.o: /usr/include/bits/stdio_lim.h
class.o: class.h registry.h dice.h /usr/include/string.h
class.o: /usr/include/features.h /usr/include/sys/cdefs.h
class.o: /usr/include/gnu/stubs.h
class.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
class.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
class.o: /usr/include/bits/types.h /usr/include/time.h /usr/include/endian.h
class.o: /usr/include/bits/endian.h /usr/include/sys/select.h
class.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
class.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
class.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
class.o: /usr/include/bits/time.h /usr/include/sys/param.h
class.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
class.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
class.o: /usr/include/linux/limits.h /usr/include/linux/param.h
class.o: /usr/include/asm/param.h /usr/include/netinet/in.h
class.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
class.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
class.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
class.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
class.o: /usr/include/stdio.h
class.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
class.o: /usr/include/libio.h /usr/include/_G_config.h
class.o: /usr/include/bits/stdio_lim.h attr.h
combatv.o: /usr/include/string.h /usr/include/features.h
combatv.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
combatv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
combatv.o: /usr/include/assert.h combatv.h /usr/include/rpc/xdr.h
combatv.o: /usr/include/sys/types.h /usr/include/bits/types.h
combatv.o: /usr/include/time.h /usr/include/endian.h
combatv.o: /usr/include/bits/endian.h /usr/include/sys/select.h
combatv.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
combatv.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
combatv.o: /usr/include/stdlib.h /usr/include/alloca.h
combatv.o: /usr/include/sys/time.h /usr/include/bits/time.h
combatv.o: /usr/include/sys/param.h
combatv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
combatv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
combatv.o: /usr/include/linux/limits.h /usr/include/linux/param.h
combatv.o: /usr/include/asm/param.h /usr/include/netinet/in.h
combatv.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
combatv.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
combatv.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
combatv.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
combatv.o: /usr/include/stdio.h
combatv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
combatv.o: /usr/include/libio.h /usr/include/_G_config.h
combatv.o: /usr/include/bits/stdio_lim.h player.h attr.h weapon.h globals.h
combatv.o: armor.h location.h /usr/include/math.h
combatv.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
combatv.o: /usr/include/bits/mathcalls.h
combatv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
combatv.o: monster.h registry.h dice.h class.h ttype.h money.h
dice.o: /usr/include/stdlib.h /usr/include/features.h
dice.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dice.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
dice.o: /usr/include/sys/types.h /usr/include/bits/types.h
dice.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
dice.o: /usr/include/sys/select.h /usr/include/bits/select.h
dice.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
dice.o: /usr/include/alloca.h /usr/include/math.h
dice.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
dice.o: /usr/include/bits/mathcalls.h
dice.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
dice.o: /usr/include/stdio.h
dice.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
dice.o: /usr/include/libio.h /usr/include/_G_config.h
dice.o: /usr/include/bits/stdio_lim.h dice.h /usr/include/string.h
dice.o: /usr/include/rpc/xdr.h /usr/include/rpc/types.h
dice.o: /usr/include/sys/time.h /usr/include/bits/time.h
dice.o: /usr/include/sys/param.h
dice.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
dice.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
dice.o: /usr/include/linux/limits.h /usr/include/linux/param.h
dice.o: /usr/include/asm/param.h /usr/include/netinet/in.h
dice.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
dice.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
dice.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
dice.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
fooread.o: /usr/include/stdio.h /usr/include/features.h
fooread.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fooread.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
fooread.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
fooread.o: /usr/include/bits/types.h /usr/include/libio.h
fooread.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
fooread.o: /usr/include/string.h /usr/include/stdlib.h
fooread.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
fooread.o: /usr/include/bits/endian.h /usr/include/sys/select.h
fooread.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fooread.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
fooread.o: /usr/include/assert.h
item.o: /usr/include/assert.h /usr/include/features.h
item.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h money.h item.h
item.o: itemreg.h /usr/include/rpc/xdr.h /usr/include/sys/types.h
item.o: /usr/include/bits/types.h
item.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
item.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
item.o: /usr/include/sys/select.h /usr/include/bits/select.h
item.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
item.o: /usr/include/rpc/types.h /usr/include/stdlib.h /usr/include/alloca.h
item.o: /usr/include/sys/time.h /usr/include/bits/time.h
item.o: /usr/include/sys/param.h
item.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
item.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
item.o: /usr/include/linux/limits.h /usr/include/linux/param.h
item.o: /usr/include/asm/param.h /usr/include/netinet/in.h
item.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
item.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
item.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
item.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
item.o: /usr/include/stdio.h
item.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
item.o: /usr/include/libio.h /usr/include/_G_config.h
item.o: /usr/include/bits/stdio_lim.h registry.h xdrxml.h armor.h
itemreg.o: itemreg.h /usr/include/rpc/xdr.h /usr/include/features.h
itemreg.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
itemreg.o: /usr/include/sys/types.h /usr/include/bits/types.h
itemreg.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
itemreg.o: /usr/include/time.h /usr/include/endian.h
itemreg.o: /usr/include/bits/endian.h /usr/include/sys/select.h
itemreg.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
itemreg.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
itemreg.o: /usr/include/stdlib.h /usr/include/alloca.h
itemreg.o: /usr/include/sys/time.h /usr/include/bits/time.h
itemreg.o: /usr/include/sys/param.h
itemreg.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
itemreg.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
itemreg.o: /usr/include/linux/limits.h /usr/include/linux/param.h
itemreg.o: /usr/include/asm/param.h /usr/include/netinet/in.h
itemreg.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
itemreg.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
itemreg.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
itemreg.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
itemreg.o: /usr/include/stdio.h
itemreg.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
itemreg.o: /usr/include/libio.h /usr/include/_G_config.h
itemreg.o: /usr/include/bits/stdio_lim.h registry.h potions/heal.h item.h
itemreg.o: dice.h /usr/include/string.h player.h attr.h weapon.h globals.h
itemreg.o: armor.h location.h /usr/include/math.h
itemreg.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
itemreg.o: /usr/include/bits/mathcalls.h
itemreg.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
itemset.o: /usr/include/assert.h /usr/include/features.h
itemset.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
itemset.o: /usr/include/string.h
itemset.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
itemset.o: itemset.h item.h xdrxml.h /usr/include/rpc/xdr.h
itemset.o: /usr/include/sys/types.h /usr/include/bits/types.h
itemset.o: /usr/include/time.h /usr/include/endian.h
itemset.o: /usr/include/bits/endian.h /usr/include/sys/select.h
itemset.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
itemset.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
itemset.o: /usr/include/stdlib.h /usr/include/alloca.h
itemset.o: /usr/include/sys/time.h /usr/include/bits/time.h
itemset.o: /usr/include/sys/param.h
itemset.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
itemset.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
itemset.o: /usr/include/linux/limits.h /usr/include/linux/param.h
itemset.o: /usr/include/asm/param.h /usr/include/netinet/in.h
itemset.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
itemset.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
itemset.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
itemset.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
itemset.o: /usr/include/stdio.h
itemset.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
itemset.o: /usr/include/libio.h /usr/include/_G_config.h
itemset.o: /usr/include/bits/stdio_lim.h
location.o: location.h /usr/include/math.h /usr/include/features.h
location.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
location.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
location.o: /usr/include/bits/mathcalls.h
location.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
location.o: xdrxml.h /usr/include/rpc/xdr.h /usr/include/sys/types.h
location.o: /usr/include/bits/types.h
location.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
location.o: /usr/include/time.h /usr/include/endian.h
location.o: /usr/include/bits/endian.h /usr/include/sys/select.h
location.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
location.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
location.o: /usr/include/stdlib.h /usr/include/alloca.h
location.o: /usr/include/sys/time.h /usr/include/bits/time.h
location.o: /usr/include/sys/param.h
location.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
location.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
location.o: /usr/include/linux/limits.h /usr/include/linux/param.h
location.o: /usr/include/asm/param.h /usr/include/netinet/in.h
location.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
location.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
location.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
location.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
location.o: /usr/include/stdio.h
location.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
location.o: /usr/include/libio.h /usr/include/_G_config.h
location.o: /usr/include/bits/stdio_lim.h
main.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/gnu/stubs.h
main.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
main.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
main.o: /usr/include/bits/types.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
main.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
main.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
main.o: /usr/include/bits/confname.h /usr/include/getopt.h
main.o: /usr/include/string.h /usr/include/assert.h player.h attr.h weapon.h
main.o: globals.h armor.h location.h /usr/include/math.h
main.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
main.o: /usr/include/bits/mathcalls.h
main.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
main.o: playerv.h apothecary.h merchant.h item.h money.h barter.h itemset.h
main.o: smithy.h tanner.h wright.h dice.h /usr/include/rpc/xdr.h
main.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
main.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
main.o: /usr/include/bits/time.h /usr/include/sys/param.h
main.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
main.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
main.o: /usr/include/linux/limits.h /usr/include/linux/param.h
main.o: /usr/include/asm/param.h /usr/include/netinet/in.h
main.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
main.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
main.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
main.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h xdrxml.h
main.o: monster.h registry.h combatv.h
merchant.o: /usr/include/stdio.h /usr/include/features.h
merchant.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
merchant.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
merchant.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
merchant.o: /usr/include/bits/types.h /usr/include/libio.h
merchant.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
merchant.o: /usr/include/string.h merchant.h item.h money.h barter.h
merchant.o: itemset.h dice.h /usr/include/rpc/xdr.h /usr/include/sys/types.h
merchant.o: /usr/include/time.h /usr/include/endian.h
merchant.o: /usr/include/bits/endian.h /usr/include/sys/select.h
merchant.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
merchant.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
merchant.o: /usr/include/stdlib.h /usr/include/alloca.h
merchant.o: /usr/include/sys/time.h /usr/include/bits/time.h
merchant.o: /usr/include/sys/param.h
merchant.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
merchant.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
merchant.o: /usr/include/linux/limits.h /usr/include/linux/param.h
merchant.o: /usr/include/asm/param.h /usr/include/netinet/in.h
merchant.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
merchant.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
merchant.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
merchant.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
money.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
money.o: /usr/include/gnu/stubs.h /usr/include/bits/huge_val.h
money.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
money.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
money.o: /usr/include/string.h
money.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
money.o: /usr/include/ctype.h /usr/include/bits/types.h /usr/include/endian.h
money.o: /usr/include/bits/endian.h money.h xdrxml.h /usr/include/rpc/xdr.h
money.o: /usr/include/sys/types.h /usr/include/time.h
money.o: /usr/include/sys/select.h /usr/include/bits/select.h
money.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
money.o: /usr/include/rpc/types.h /usr/include/stdlib.h /usr/include/alloca.h
money.o: /usr/include/sys/time.h /usr/include/bits/time.h
money.o: /usr/include/sys/param.h
money.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
money.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
money.o: /usr/include/linux/limits.h /usr/include/linux/param.h
money.o: /usr/include/asm/param.h /usr/include/netinet/in.h
money.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
money.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
money.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
money.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
money.o: /usr/include/stdio.h
money.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
money.o: /usr/include/libio.h /usr/include/_G_config.h
money.o: /usr/include/bits/stdio_lim.h
monster.o: dice.h /usr/include/string.h /usr/include/features.h
monster.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
monster.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
monster.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
monster.o: /usr/include/bits/types.h /usr/include/time.h
monster.o: /usr/include/endian.h /usr/include/bits/endian.h
monster.o: /usr/include/sys/select.h /usr/include/bits/select.h
monster.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
monster.o: /usr/include/rpc/types.h /usr/include/stdlib.h
monster.o: /usr/include/alloca.h /usr/include/sys/time.h
monster.o: /usr/include/bits/time.h /usr/include/sys/param.h
monster.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
monster.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
monster.o: /usr/include/linux/limits.h /usr/include/linux/param.h
monster.o: /usr/include/asm/param.h /usr/include/netinet/in.h
monster.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
monster.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
monster.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
monster.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
monster.o: /usr/include/stdio.h
monster.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
monster.o: /usr/include/libio.h /usr/include/_G_config.h
monster.o: /usr/include/bits/stdio_lim.h monster.h registry.h location.h
monster.o: /usr/include/math.h /usr/include/bits/huge_val.h
monster.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
monster.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
player.o: /usr/include/stdio.h /usr/include/features.h
player.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
player.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
player.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
player.o: /usr/include/bits/types.h /usr/include/libio.h
player.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
player.o: /usr/include/string.h /usr/include/assert.h player.h attr.h
player.o: weapon.h globals.h armor.h location.h /usr/include/math.h
player.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
player.o: /usr/include/bits/mathcalls.h
player.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
player.o: xdrxml.h /usr/include/rpc/xdr.h /usr/include/sys/types.h
player.o: /usr/include/time.h /usr/include/endian.h
player.o: /usr/include/bits/endian.h /usr/include/sys/select.h
player.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
player.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
player.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
player.o: /usr/include/bits/time.h /usr/include/sys/param.h
player.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
player.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
player.o: /usr/include/linux/limits.h /usr/include/linux/param.h
player.o: /usr/include/asm/param.h /usr/include/netinet/in.h
player.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
player.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
player.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
player.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h class.h
player.o: registry.h dice.h
playerv.o: /usr/include/stdio.h /usr/include/features.h
playerv.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
playerv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
playerv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
playerv.o: /usr/include/bits/types.h /usr/include/libio.h
playerv.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
playerv.o: /usr/include/string.h /usr/include/assert.h playerv.h attr.h
playerv.o: race.h /usr/include/rpc/xdr.h /usr/include/sys/types.h
playerv.o: /usr/include/time.h /usr/include/endian.h
playerv.o: /usr/include/bits/endian.h /usr/include/sys/select.h
playerv.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
playerv.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
playerv.o: /usr/include/stdlib.h /usr/include/alloca.h
playerv.o: /usr/include/sys/time.h /usr/include/bits/time.h
playerv.o: /usr/include/sys/param.h
playerv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
playerv.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
playerv.o: /usr/include/linux/limits.h /usr/include/linux/param.h
playerv.o: /usr/include/asm/param.h /usr/include/netinet/in.h
playerv.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
playerv.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
playerv.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
playerv.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h registry.h
playerv.o: dice.h class.h
race.o: race.h /usr/include/rpc/xdr.h /usr/include/features.h
race.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
race.o: /usr/include/sys/types.h /usr/include/bits/types.h
race.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
race.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
race.o: /usr/include/sys/select.h /usr/include/bits/select.h
race.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
race.o: /usr/include/rpc/types.h /usr/include/stdlib.h /usr/include/alloca.h
race.o: /usr/include/sys/time.h /usr/include/bits/time.h
race.o: /usr/include/sys/param.h
race.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
race.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
race.o: /usr/include/linux/limits.h /usr/include/linux/param.h
race.o: /usr/include/asm/param.h /usr/include/netinet/in.h
race.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
race.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
race.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
race.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
race.o: /usr/include/stdio.h
race.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
race.o: /usr/include/libio.h /usr/include/_G_config.h
race.o: /usr/include/bits/stdio_lim.h attr.h registry.h
registry.o: registry.h
smithy.o: dice.h /usr/include/string.h /usr/include/features.h
smithy.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
smithy.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
smithy.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
smithy.o: /usr/include/bits/types.h /usr/include/time.h /usr/include/endian.h
smithy.o: /usr/include/bits/endian.h /usr/include/sys/select.h
smithy.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
smithy.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
smithy.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
smithy.o: /usr/include/bits/time.h /usr/include/sys/param.h
smithy.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
smithy.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
smithy.o: /usr/include/linux/limits.h /usr/include/linux/param.h
smithy.o: /usr/include/asm/param.h /usr/include/netinet/in.h
smithy.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
smithy.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
smithy.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
smithy.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
smithy.o: /usr/include/stdio.h
smithy.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
smithy.o: /usr/include/libio.h /usr/include/_G_config.h
smithy.o: /usr/include/bits/stdio_lim.h smithy.h merchant.h item.h money.h
smithy.o: barter.h itemset.h weapon.h globals.h armor.h
tanner.o: dice.h /usr/include/string.h /usr/include/features.h
tanner.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
tanner.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
tanner.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
tanner.o: /usr/include/bits/types.h /usr/include/time.h /usr/include/endian.h
tanner.o: /usr/include/bits/endian.h /usr/include/sys/select.h
tanner.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tanner.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
tanner.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
tanner.o: /usr/include/bits/time.h /usr/include/sys/param.h
tanner.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
tanner.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
tanner.o: /usr/include/linux/limits.h /usr/include/linux/param.h
tanner.o: /usr/include/asm/param.h /usr/include/netinet/in.h
tanner.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
tanner.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
tanner.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
tanner.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
tanner.o: /usr/include/stdio.h
tanner.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
tanner.o: /usr/include/libio.h /usr/include/_G_config.h
tanner.o: /usr/include/bits/stdio_lim.h tanner.h merchant.h item.h money.h
tanner.o: barter.h itemset.h armor.h
test.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
test.o: /usr/include/gnu/stubs.h
test.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
test.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
test.o: /usr/include/bits/types.h /usr/include/libio.h
test.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h money.h dice.h
test.o: /usr/include/string.h /usr/include/rpc/xdr.h /usr/include/sys/types.h
test.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
test.o: /usr/include/sys/select.h /usr/include/bits/select.h
test.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
test.o: /usr/include/rpc/types.h /usr/include/stdlib.h /usr/include/alloca.h
test.o: /usr/include/sys/time.h /usr/include/bits/time.h
test.o: /usr/include/sys/param.h
test.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
test.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
test.o: /usr/include/linux/limits.h /usr/include/linux/param.h
test.o: /usr/include/asm/param.h /usr/include/netinet/in.h
test.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
test.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
test.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
test.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
timeq.o: /usr/include/stdlib.h /usr/include/features.h
timeq.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
timeq.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
timeq.o: /usr/include/sys/types.h /usr/include/bits/types.h
timeq.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
timeq.o: /usr/include/sys/select.h /usr/include/bits/select.h
timeq.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
timeq.o: /usr/include/alloca.h player.h attr.h weapon.h globals.h armor.h
timeq.o: location.h /usr/include/math.h /usr/include/bits/huge_val.h
timeq.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
timeq.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/float.h
timeq.o: timeq.h
ttype.o: /usr/include/assert.h /usr/include/features.h
ttype.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ttype.o: /usr/include/string.h
ttype.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
ttype.o: ttype.h money.h
weapon.o: weapon.h globals.h money.h xdrxml.h /usr/include/rpc/xdr.h
weapon.o: /usr/include/features.h /usr/include/sys/cdefs.h
weapon.o: /usr/include/gnu/stubs.h /usr/include/sys/types.h
weapon.o: /usr/include/bits/types.h
weapon.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
weapon.o: /usr/include/time.h /usr/include/endian.h
weapon.o: /usr/include/bits/endian.h /usr/include/sys/select.h
weapon.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
weapon.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
weapon.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
weapon.o: /usr/include/bits/time.h /usr/include/sys/param.h
weapon.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
weapon.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
weapon.o: /usr/include/linux/limits.h /usr/include/linux/param.h
weapon.o: /usr/include/asm/param.h /usr/include/netinet/in.h
weapon.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
weapon.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
weapon.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
weapon.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
weapon.o: /usr/include/stdio.h
weapon.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
weapon.o: /usr/include/libio.h /usr/include/_G_config.h
weapon.o: /usr/include/bits/stdio_lim.h
wright.o: dice.h /usr/include/string.h /usr/include/features.h
wright.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
wright.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
wright.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h
wright.o: /usr/include/bits/types.h /usr/include/time.h /usr/include/endian.h
wright.o: /usr/include/bits/endian.h /usr/include/sys/select.h
wright.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
wright.o: /usr/include/sys/sysmacros.h /usr/include/rpc/types.h
wright.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/sys/time.h
wright.o: /usr/include/bits/time.h /usr/include/sys/param.h
wright.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
wright.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
wright.o: /usr/include/linux/limits.h /usr/include/linux/param.h
wright.o: /usr/include/asm/param.h /usr/include/netinet/in.h
wright.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
wright.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
wright.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
wright.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
wright.o: /usr/include/stdio.h
wright.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
wright.o: /usr/include/libio.h /usr/include/_G_config.h
wright.o: /usr/include/bits/stdio_lim.h wright.h merchant.h item.h money.h
wright.o: barter.h itemset.h weapon.h globals.h
xdrasc.o: /usr/include/assert.h /usr/include/features.h
xdrasc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
xdrasc.o: /usr/include/string.h
xdrasc.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
xdrasc.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
xdrasc.o: /usr/include/stdio.h /usr/include/bits/types.h /usr/include/libio.h
xdrasc.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h xdrasc.h
xdrasc.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h /usr/include/time.h
xdrasc.o: /usr/include/endian.h /usr/include/bits/endian.h
xdrasc.o: /usr/include/sys/select.h /usr/include/bits/select.h
xdrasc.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
xdrasc.o: /usr/include/rpc/types.h /usr/include/stdlib.h
xdrasc.o: /usr/include/alloca.h /usr/include/sys/time.h
xdrasc.o: /usr/include/bits/time.h /usr/include/sys/param.h
xdrasc.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
xdrasc.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
xdrasc.o: /usr/include/linux/limits.h /usr/include/linux/param.h
xdrasc.o: /usr/include/asm/param.h /usr/include/netinet/in.h
xdrasc.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
xdrasc.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
xdrasc.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
xdrasc.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
xdrxml.o: /usr/include/assert.h /usr/include/features.h
xdrxml.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
xdrxml.o: /usr/include/string.h
xdrxml.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stddef.h
xdrxml.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/stdarg.h
xdrxml.o: /usr/include/stdio.h /usr/include/bits/types.h /usr/include/libio.h
xdrxml.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h xdrxml.h
xdrxml.o: /usr/include/rpc/xdr.h /usr/include/sys/types.h /usr/include/time.h
xdrxml.o: /usr/include/endian.h /usr/include/bits/endian.h
xdrxml.o: /usr/include/sys/select.h /usr/include/bits/select.h
xdrxml.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
xdrxml.o: /usr/include/rpc/types.h /usr/include/stdlib.h
xdrxml.o: /usr/include/alloca.h /usr/include/sys/time.h
xdrxml.o: /usr/include/bits/time.h /usr/include/sys/param.h
xdrxml.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/limits.h
xdrxml.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include/syslimits.h
xdrxml.o: /usr/include/linux/limits.h /usr/include/linux/param.h
xdrxml.o: /usr/include/asm/param.h /usr/include/netinet/in.h
xdrxml.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
xdrxml.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
xdrxml.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
xdrxml.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
