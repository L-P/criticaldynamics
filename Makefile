WASM_C=$(wildcard wasmsrc/*.c wasmsrc/lib/*.c)
WASM_O=$(patsubst %.c,%.o,$(WASM_C))
WASM_DEPS_H=$(wildcard wasmsrc/lib/*.h)
WASM_DEPS_O=$(patsubst %.c,%.o,$(wildcard wasmsrc/lib/*.c))
WASM_TARGET=$(subst wasmsrc/,wasm/,$(patsubst %.c,%.wasm,$(wildcard wasmsrc/*.c)))
WASM_LIBS=$(subst wasmsrc/lib/,,$(addprefix -l:,$(WASM_DEPS_O)))

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
		-std=c23 \
		--target=wasm32 \
		-nostdlib \
		-O3

all: $(WASM_TARGET)

.SECONDARY: $(WASM_O)
%.o: %.c $(WASM_DEPS_H)
	clang $(CFLAGS) -c "$<" -o "$@"

wasm/%.wasm: wasmsrc/%.o $(WASM_DEPS_O)
	wasm-ld-18 $(LDFLAGS) -o "$@" "$<"

.PHONY: clean
clean:
	rm -f wasm/*.wasm
	rm -f wasmsrc/*.o wasmsrc/lib/*.o
