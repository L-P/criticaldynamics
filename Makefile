MAP_SRC=$(wildcard mapssrc/*.map)
MAP_TARGET=$(subst mapssrc/,maps/,$(patsubst %.map,%.bsp,$(MAP_SRC)))
NOD_TARGET=$(subst maps/,maps/graphs/,$(patsubst %.bsp,%.nod,$(MAP_TARGET)))

VMLIB=src/external/wasm-micro-runtime/build/libvmlib.a

all: tup userconfig.cfg background

.PHONY: tup monitor background
tup:
	tup

monitor:
	tup monitor --foreground --autoupdate

# Because names are generated and I don't want to hardcode them in a Tupfile,
# do it via make.
background:
	bin/buildbackground

userconfig.cfg: Makefile
	echo "exec dev.cfg" > userconfig.cfg
	echo "default_fov 90" >> userconfig.cfg

maps/graphs/prefabs.nod:
	@:

.PHONY: clean mrproper
clean:
	rm -f maps/graphs/*.nrp

mrproper:
	git clean -fdX
