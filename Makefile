RAYLIB_DIR = raylib
RAYLIB_LIB = $(RAYLIB_DIR)/src/libraylib.a

CC = emcc
CFLAGS = -Wall -Iinclude -I$(RAYLIB_DIR)/src -D__EMSCRIPTEN__ -Os
LDFLAGS = -L$(RAYLIB_DIR)/src -l:libraylib.a

# Emscripten specific flags for WebGL and browser environments
# We preload the assets folder so the browser has access to models and audio
# MIN_CHROME_VERSION=74 is the lowest Emscripten supports now.
# For Tizen 5.5 (Chromium 69), this is safe because the flag only affects
# JS glue code, and the 69→74 JS delta is negligible.
# The real compatibility concern is WASM opcodes — we explicitly disable
# post-MVP features to keep the binary safe for older engines.
LDFLAGS += -s USE_GLFW=3 \
           -s MIN_CHROME_VERSION=74 \
           -s ASYNCIFY \
           -s ALLOW_MEMORY_GROWTH=1 \
           -s EXPORTED_RUNTIME_METHODS=ccall,cwrap \
           -mno-sign-ext \
           -mno-bulk-memory \
           -mno-nontrapping-fptoint \
           -mno-simd128 \
           --preload-file assets

SRC_DIR = src
OBJ_DIR = build
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = web/toon_dash.js

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(RAYLIB_LIB) $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(RAYLIB_LIB):
	git clone --depth 1 https://github.com/raysan5/raylib.git $(RAYLIB_DIR)
	cd $(RAYLIB_DIR)/src && make PLATFORM=PLATFORM_WEB -B
	cp $(RAYLIB_DIR)/src/libraylib.web.a $(RAYLIB_LIB)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) web/toon_dash.wasm web/toon_dash.data
