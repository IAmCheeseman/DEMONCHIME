CC = gcc

Q = @

INCLUDE = \
	-Icore -Ilib/glad/include -Ilib/miniz/include \
	-Ilib/stb/include -Ilib/luajit -Ilib 
CFLAGS = $(cflags) -std=c99 -Wall -Wextra -Wno-unused-parameter $(INCLUDE) \
	-Dbse_allow_opengl
LDFLAGS =

PROJECT_NAME = DEMONCHIME

EXE = $(PROJECT_NAME)
OBJ = \
	main.o core/include.o core/mem.o core/log.o core/engine.o core/image.o \
	core/prng.o core/timer.o core/vfs.o core/math/mat4.o core/math/transform.o \
	core/wrap/wrap.o core/wrap/wrap_enums.o core/wrap/wrap_engine.o \
	core/wrap/wrap_log.o core/wrap/wrap_mat4.o core/wrap/wrap_mesh.o \
	core/wrap/wrap_vertex_format.o core/wrap/wrap_shader.o \
	core/wrap/wrap_texture.o core/wrap/wrap_prng.o core/gfx/buffer_object.o \
	core/gfx/framebuffer.o core/gfx/gfx.o core/gfx/gfx_types.o core/gfx/mesh.o \
	core/gfx/shader.o core/gfx/texture.o core/gfx/vertex_array.o \
	core/gfx/opengl/gl_buffer_object.o core/gfx/opengl/gl_framebuffer.o \
	core/gfx/opengl/gl_gfx.o core/gfx/opengl/gl_shader.o \
	core/gfx/opengl/gl_texture.o core/gfx/opengl/gl_type_conv.o \
	core/gfx/opengl/gl_vertex_array.o lib/glad/src/glad.o lib/stb/stb.o
DEP = $(OBJ:%.o=%.d)

# HAD files store game content
# this one contains every core resource and script
CORE_HAD = CORE.HAD
CORE_HAD_DIR = core/had
CORE_HAD_DEP = res
CORE_HAD_DEP := $(CORE_HAD_DIR) $(addprefix $(CORE_HAD_DIR)/,$(CORE_HAD_DEP))

GAME_HAD = $(PROJECT_NAME).HAD
GAME_HAD_DIR = game
GAME_HAD_DEP = systems res res/textures res/fgd
GAME_HAD_DEP := $(GAME_HAD_DIR) $(addprefix $(GAME_HAD_DIR)/,$(GAME_HAD_DEP))

CLEAN_FILES = $(OBJ) $(DEP) $(EXE) $(CORE_HAD) $(GAME_HAD)

CD = cd
ECHO = echo
RM = rm -f
SILENCE = &> /dev/null

ifeq (,$(findstring Windows,$(OS)))
	HOST_SYS = $(shell uname -s)
else
	HOST_SYS = Windows
	EXE := $(EXE).EXE
	CLEAN_FILES := $(subst /,\,$(CLEAN_FILES))
	RM = del
	SILENCE = > nul
endif

ifeq ($(config),release)
	CFLAGS += -O2 -Dbse_release
else
	config = debug
	CFLAGS += -O2 -g -Dbse_debug
	ifeq ($(HOST_SYS),Linux)
		CFLAGS += -fsanitize=address
	endif
endif

ifeq (Linux,$(HOST_SYS))
	CFLAGS += -Dbse_linux
	LDFLAGS += -lluajit -lglfw -lm -L.
endif
ifeq (Windows,$(HOST_SYS))
	CFLAGS += -Dbse_windows
	LDFLAGS += -llua51 -lglfw3 -L.
endif

.PHONY: all clean compile_flags

all: $(EXE) $(CORE_HAD) $(GAME_HAD)

$(EXE): $(OBJ)
	@$(ECHO) "cc $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.c
	@$(ECHO) "cc $< -> $@"
	$(Q)$(CC) $(CFLAGS) -MMD -c -o $@ $<

$(CORE_HAD): $(CORE_HAD_DEP)
	@$(ECHO) "had $@"
	$(Q)$(CD) $(CORE_HAD_DIR) && 7z a -tzip ../../$@ ./* $(SILENCE)

$(GAME_HAD): $(GAME_HAD_DEP)
	@$(ECHO) "had $@"
	$(Q)$(CD) $(GAME_HAD_DIR) && 7z a -tzip ../$@ ./* $(SILENCE)

clean:
	$(RM) $(CLEAN_FILES)

compile_flags:
	@$(ECHO) "" > compile_flags.txt
	$(Q)$(foreach flag,$(CFLAGS),echo $(flag) >> compile_flags.txt;)

-include $(DEP)
