MAP_SRC=$(wildcard mapssrc/*.map)
MAP_TARGET=$(subst mapssrc/,maps/,$(patsubst %.map,%.bsp,$(MAP_SRC)))
NOD_TARGET=$(subst maps/,maps/graphs/,$(patsubst %.bsp,%.nod,$(MAP_TARGET)))

VMLIB=src/external/wasm-micro-runtime/build/libvmlib.a

all: $(VMLIB) tup userconfig.cfg

.PHONY: tup monitor
tup:
	tup

monitor:
	tup monitor --foreground --autoupdate

WASM_BUILD_DIR=src/external/wasm-micro-runtime/build
WASM_RELEASE?=Release
$(VMLIB):
	mkdir -p "$(WASM_BUILD_DIR)"
	cd "$(WASM_BUILD_DIR)" && cmake ../CMakeLists.txt \
		-DCMAKE_BUILD_TYPE=$(WASM_RELEASE) \
		-DWAMR_BUILD_PLATFORM=linux \
		-DWAMR_BUILD_TARGET=X86_32 \
		-DWAMR_BUILD_FAST_INTERP=0 \
		-DWAMR_BUILD_SIMD=0 \
		-DWAMR_BUILD_LIBC_BUILTIN=0 \
		-DWAMR_BUILD_LIBC_WASI=0 \
		-DWAMR_BUILD_AOT=0
	$(MAKE) -C $(WASM_BUILD_DIR)

userconfig.cfg:
	echo "exec dev.cfg" > userconfig.cfg
	echo "default_fov 90" >> userconfig.cfg

.PHONY: nodes
nodes: $(NOD_TARGET)
maps/graphs/%.nod: maps/%.bsp
	bin/run +map "$(subst .bsp,,$(notdir $<))"
	# Prevents running maps without nodes upon every make invocation.
	touch "$@"

maps/graphs/prefabs.nod:
	@:

.PHONY: clean mrproper
clean:
	rm -f maps/graphs/*.nrp

mrproper:
	git clean -fdX
