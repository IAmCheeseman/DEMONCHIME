#include "include.h"

#include "core_engine.h"

engine_t engine;

static void cleanup()
{
  engine_destroy(&engine);
}

int main(int argc, const char* args[])
{
  log_info("os: %s", bse_os_str);

  engine_conf_t conf = (engine_conf_t){
    .mount_path = "DEMONCHIME.HAD",
    .window_title = "DEMONCHIME",
#ifdef bse_release
    .fullscreen = FULLSCREEN_FULL,
#else
    .fullscreen = fullscreen_none,
#endif
    .window_size = (vec2i_t){320 * 3, 180 * 3},
    .screen_size = (vec2i_t){320, 180},
    .vsync = 0,
  };

  engine_init(&engine, conf);
  atexit(cleanup);

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      vfs_err_t err = vfs_mount(&engine.vfs, args[i]);
      if (err != vfs_ok) {
        log_fatal(1, "could not load '%s'", args[i]);
      }
    }
  }

  engine_init_lua(&engine);

  while (!is_engine_closed(&engine))
  {
    engine_update(&engine);
    engine_draw(&engine);
  }
}
