#include <miniz.h>

//#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include "ffc_pipe.cpp"
int main(int argc, char *argv[]) {
  if (argc < 2) {
    usage(argv[0]);
    return -1;
  }
  argc--;

  Read reader(const_cast<const char **>(&argv[1]), argc);
  Compressor compress;
  Write writer;
  ff_Pipe pipe(reader, compress, writer);
  if (pipe.run_and_wait_end() < 0) {
    error("running pipeline\n");
    return -1;
  }
  bool success = true;
  success &= reader.success;
  success &= compress.success;
  success &= writer.success;
  if (success) printf("Done.\n");

  return 0;
}

