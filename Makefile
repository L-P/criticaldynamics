WASM_C=$(wildcard wasmsrc/*.c wasmsrc/lib/*.c)
WASM_O=$(patsubst %.c,%.o,$(WASM_C))
WASM_DEPS_H=$(wildcard wasmsrc/lib/*.h)
WASM_DEPS_O=$(patsubst %.c,%.o,$(wildcard wasmsrc/lib/*.c))
WASM_TARGET=$(subst wasmsrc/,wasm/,$(patsubst %.c,%.wasm,$(wildcard wasmsrc/*.c)))
WASM_LIBS=$(subst wasmsrc/lib/,,$(addprefix -l:,$(WASM_DEPS_O)))

MAP_SRC=$(wildcard mapssrc/*.map)
MAP_TARGET=$(subst mapssrc/,maps/,$(patsubst %.map,%.bsp,$(MAP_SRC)))
NOD_TARGET=$(subst maps/,maps/graphs/,$(patsubst %.bsp,%.nod,$(MAP_TARGET)))

LDFLAGS=\
		--no-entry \
		--strip-all \
		--export-dynamic \
		--import-memory \
		--allow-undefined-file=wasmsrc/lib/native_symbols.txt \
		--lto-O3 \
		-L "wasmsrc/lib" $(WASM_LIBS)

CFLAGS=\
		-Wall -Wextra \
		-Wno-unused-parameter \
		-mbulk-memory \
		-std=c23 \
		--target=wasm32 \
		-nostdlib \
		-O3

all: $(WASM_TARGET) $(MAP_TARGET) userconfig.cfg

userconfig.cfg:
	echo "exec dev.cfg" > userconfig.cfg
	echo "default_fov 90" >> userconfig.cfg

.PHONY: nodes
nodes: $(NOD_TARGET)
maps/graphs/%.nod: maps/%.bsp
	bin/run +map "$(subst .bsp,,$(notdir $<))"
	# Prevents running maps without nodes upon every make invocation.
	touch "$@"

maps/%.bsp: mapssrc/%.map
	bin/buildmap "$(notdir $<)"

maps/graphs/prefabs.nod:
	@:
maps/prefabs.bsp:
	@:

.SECONDARY: $(WASM_O)
%.o: %.c $(WASM_DEPS_H)
	clang $(CFLAGS) -c "$<" -o "$@"

wasm/%.wasm: wasmsrc/%.o $(WASM_DEPS_O)
	wasm-ld-18 $(LDFLAGS) -o "$@" "$<"

.PHONY: clean mrproper
clean:
	rm -f wasm/*.wasm
	rm -f wasmsrc/*.o wasmsrc/lib/*.o
	rm -f maps/graphs/*.nrp

mrproper:
	git clean -fdX
