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
	main.o core/include.o core/c_mem.o core/c_log.o core/c_engine.o \
	core/c_img.o core/c_prng.o core/c_timer.o core/c_vfs.o \
	core/c_win.o \
	\
	core/m_mat4.o core/m_trans.o \
	\
	core/w_wrap.o core/w_enums.o core/w_engine.o core/w_log.o \
	core/w_mat4.o core/w_mesh.o core/w_tex.o core/w_prng.o core/w_font.o \
	\
	core/g_framebuf.o core/g_renderer.o core/g_types.o core/g_mesh.o \
	core/g_shader.o core/g_tex.o core/g_vert_arr.o \
	core/g_font.o core/g_buf_obj.o \
	\
	core/opengl/gl_buf_obj.o core/opengl/gl_framebuf.o \
	core/opengl/gl_gfx.o core/opengl/gl_shader.o \
	core/opengl/gl_tex.o core/opengl/gl_type_conv.o \
	core/opengl/gl_font.o core/opengl/gl_vert_arr.o \
	\
	lib/glad/src/glad.o lib/stb/stb.o lib/schrift/schrift.o
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
	ifeq ($(config),releasesym)
		CFLAGS += -O2 -g -Dbse_release
	else
		config = debug
		CFLAGS += -O2 -g -Dbse_debug
		ifeq ($(HOST_SYS),Linux)
			CFLAGS += -fsanitize=address
		endif
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
