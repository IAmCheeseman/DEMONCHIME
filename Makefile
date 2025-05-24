CC = gcc

Q = @

INCLUDE = \
	-Icore -Ilib/glad/include -Ilib/miniz/include \
	-Ilib/stb/include -Ilib/luajit -Ilib -Ilib/freetype
CFLAGS = $(cflags) -std=c99 -Wall -Wextra -Wno-unused-parameter $(INCLUDE) \
	-Dbse_allow_opengl -DGLFW_INCLUDE_NONE
LDFLAGS =

PROJECT_NAME = DEMONCHIME

EXE = $(PROJECT_NAME)
OBJ = \
	main.o core/include.o core/core_mem.o core/core_log.o core/core_engine.o \
	core/core_image.o core/core_prng.o core/core_timer.o core/core_vfs.o \
	core/core_window.o \
	\
	core/math_mat4.o core/math_transform.o \
	\
	core/wrap.o core/wrap_enums.o core/wrap_engine.o core/wrap_log.o \
	core/wrap_mat4.o core/wrap_mesh.o core/wrap_vertex_format.o \
	core/wrap_shader.o core/wrap_texture.o core/wrap_prng.o core/wrap_font.o \
	\
	core/gfx_framebuffer.o core/gfx.o core/gfx_types.o core/gfx_mesh.o \
	core/gfx_shader.o core/gfx_texture.o core/gfx_vertex_array.o \
	core/gfx_text.o core/gfx_buffer_object.o \
	\
	core/opengl/gl_buffer_object.o core/opengl/gl_framebuffer.o \
	core/opengl/gl_gfx.o core/opengl/gl_shader.o \
	core/opengl/gl_texture.o core/opengl/gl_type_conv.o \
	core/opengl/gl_text.o core/opengl/gl_vertex_array.o \
	\
	lib/glad/src/glad.o lib/stb/stb.o
DEP = $(OBJ:%.o=%.d)

# HAD files store game content
# this one contains every core resource and script
CORE_HAD = CORE.HAD
CORE_HAD_DIR = core/had
CORE_HAD_DEP = res
CORE_HAD_DEP := $(CORE_HAD_DIR) $(addprefix $(CORE_HAD_DIR)/,$(CORE_HAD_DEP))

GAME_HAD = $(PROJECT_NAME).HAD
GAME_HAD_DIR = game
GAME_HAD_DEP = systems res res/textures res/fgd res/fonts
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
	LDFLAGS += -lluajit -lglfw -lfreetype -lm -L.
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
