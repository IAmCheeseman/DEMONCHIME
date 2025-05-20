#include "include.h"

#include "engine.h"

int main(int argc, const char* args[])
{
  LogInfo("os: %s", bse_os_str);

  struct EngineConfig conf = (struct EngineConfig){
    .mount_path = "DEMONCHIME.HAD",
    .window_title = "DEMONCHIME",
    .window_size = (vec2i_t){320 * 3, 180 * 3},
    .screen_size = (vec2i_t){320, 180},
    .vsync = 0,
  };

  struct Engine engine;
  EngineInit(&engine, conf);

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      enum VfsError err = VfsMount(&engine.vfs, args[i]);
      if (err != VFS_OK) {
        LogFatal(1, "could not load '%s'", args[i]);
      }
    }
  }

  EngineInitLua(&engine);

  while (!EngineIsClosed(&engine))
  {
    EngineUpdate(&engine);
    EngineDraw(&engine);
  }

  EngineDestroy(&engine);
}
