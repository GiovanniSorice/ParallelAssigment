//
// Created by gs1010 on 24/05/20.
//
/*
 * Simple file compressor using miniz and the FastFlow pipeline.
 *
 * miniz source code: https://github.com/richgel999/miniz
 * https://code.google.com/archive/p/miniz/
 *
 */
/* Part of the code from: Massimo Torquati <massimo.torquati@unipi.it>
 * This code is a mix of POSIX C code and some C++ library call
 * (mainly for strings manipulation).
 */

#include <miniz.h>

#include <map>
#include <string>
#include <iostream>

#include <ff/ff.hpp>
#include <ff/farm.hpp>
#include <utility.hpp>
#include <filesystem>
using namespace ff;

struct Task {
  Task(unsigned char *ptr, size_t size, const std::string &name, size_t idChunk, unsigned char *beginPtr,
       const std::string docFilename, size_t nChunk) :
      ptr(ptr),
      size(size),
      cmp_size(0),
      filename(name),
      idChunk(idChunk),
      beginPtr(),
      docFilename(docFilename),
      nChunk(
          nChunk) {}
  // ptr to the specific chunk
  unsigned char *ptr;
  // beginPtr pointer to begin of file
  unsigned char *beginPtr;
  size_t size;
  size_t cmp_size;
  size_t idChunk;
  //Name of the chunk
  const std::string filename;
  //Name of the document
  const std::string docFilename;
  // Number of chunk
  const size_t nChunk;

};

// 1st stage
struct Read : ff_node_t<Task> {
  Read(const char **argv, int argc, const size_t threshold) : argv(argv), argc(argc), threshold(threshold) {}

  // ------------------- utility functions
  // It memory maps the input file and then assigns a task to
  // one Worker
  bool doWork(const std::string &fname, size_t size) {
    unsigned char *ptr = nullptr;
    //! Split large file in more tasks
    size_t nChunk = size / threshold;
    size_t remaining = size % threshold;
    bool lastChunk = size % threshold; // Need and extra chunk for the remaining byte?
    for (int i = 0; i < nChunk; i++) {
      if (!mapFile(fname.c_str(), size, ptr)) return false;
      Task *t = new Task(ptr + i * threshold, threshold,
                         std::filesystem::path(fname).filename().u8string() + std::to_string(i),
                         i,
                         ptr,
                         std::filesystem::path(fname).filename().u8string(),
                         nChunk + lastChunk);
      ff_send_out(t);
    }

    //! Check if some byte are not in a chunk
    if (remaining) {
      if (!mapFile(fname.c_str(), size, ptr)) return false;
      Task *t =
          new Task(ptr + nChunk * threshold,
                   remaining,
                   std::filesystem::path(fname).filename().u8string() + std::to_string(nChunk),
                   nChunk,
                   ptr,
                   std::filesystem::path(fname).filename().u8string(),
                   nChunk + lastChunk);
      ff_send_out(t);
    }
    // sending to the next stage
    return true;
  }
  // walks through the directory tree rooted in dname
  bool walkDir(const std::string &dname, size_t size) {
    DIR *dir;
    if ((dir = opendir(dname.c_str())) == NULL) {
      perror("opendir");
      fprintf(stderr, "Error: opendir %s\n", dname.c_str());
      return false;
    }
    struct dirent *file;
    bool error = false;
    while ((errno = 0, file = readdir(dir)) != NULL) {
      struct stat statbuf;
      std::string filename = dname + "/" + file->d_name;
      if (stat(filename.c_str(), &statbuf) == -1) {
        perror("stat");
        fprintf(stderr, "Error: stat %s\n", filename.c_str());
        return false;
      }
      if (S_ISDIR(statbuf.st_mode)) {
        if (!isdot(filename.c_str())) {
          if (!walkDir(filename, statbuf.st_size)) error = true;
        }
      } else {
        if (!doWork(filename, statbuf.st_size)) error = true;
      }
    }
    if (errno != 0) {
      perror("readdir");
      error = true;
    }
    closedir(dir);
    return !error;
  }
  // -------------------

  Task *svc(Task *) {
    for (long i = 0; i < argc; ++i) {
      struct stat statbuf;
      if (stat(argv[i], &statbuf) == -1) {
        perror("stat");
        fprintf(stderr, "Error: stat %s\n", argv[i]);
        continue;
      }
      bool dir = false;
      if (S_ISDIR(statbuf.st_mode)) {
        success &= walkDir(argv[i], statbuf.st_size);
      } else {
        success &= doWork(argv[i], statbuf.st_size);
      }
    }
    return EOS;
  }

  void svc_end() {
    if (!success) {
      printf("Read stage: Exiting with (some) Error(s)\n");
      return;
    }
  }

  const size_t threshold;
  const char **argv;
  const int argc;
  bool success = true;
};
// 2nd stage
struct Compressor : ff_node_t<Task> {
  Task *svc(Task *task) {
    unsigned char *inPtr = task->ptr;
    size_t inSize = task->size;
    // get an estimation of the maximum compression size
    unsigned long cmp_len = compressBound(inSize);
    // allocate memory to store compressed data in memory
    unsigned char *ptrOut = new unsigned char[cmp_len];

    if (compress(ptrOut, &cmp_len, (const unsigned char *) inPtr, inSize) != Z_OK) {
      printf("Failed to compress file in memory\n");
      success = false;
      delete[] ptrOut;
      return GO_ON;
    }

    task->ptr = ptrOut;
    task->cmp_size = cmp_len;
    ff_send_out(task);
    unmapFile(task->beginPtr, inSize);

    return GO_ON;
  }
  void svc_end() {
    if (!success) {
      printf("Comprssor stage: Exiting with (some) Error(s)\n");
      return;
    }
  }
  bool success = true;
};
// 3rd stage
struct Write : ff_node_t<Task> {
  Task *svc(Task *task) {
    const std::string outfile = "tmp/" + task->docFilename + "/" + task->filename + ".zip";

    std::pair<std::map<std::string, size_t>::iterator, bool>
        ret = docs.insert(std::pair<std::string, size_t>(task->docFilename, task->nChunk - 1));
    //! Check if already inset
    if (ret.second == false) {
      docs[task->docFilename] -= 1;
    } else {
      std::string
          command_string = "mkdir tmp/" + task->docFilename;
      int r = system(command_string.c_str());
      if (r != 0) {
        success = false;
      }
    }

    // write the compressed data into disk
    success &= writeFile(outfile, task->ptr, task->cmp_size);

    if (success && REMOVE_ORIGIN) {
      unlink(task->filename.c_str());
    }

    if (!docs[task->docFilename]) {
      //! Compress all small file in a big one
      std::string command_string = "tar  -czf tmp/" + task->docFilename + ".tar.gz tmp/" + task->docFilename;

      /*
      std::string command_string = "tar  -czf tmp/" + task->docFilename + ".tar.gz";
      for (int i = 0; i < task->nChunk; i++) {
        command_string += " tmp/" + task->docFilename + "/" + task->docFilename + std::to_string(i) + ".zip";
      }
      */
      int r = system(command_string.c_str());

      if (r != 0) {
        success = false;
      }
      // Remove all the created tmp file
      command_string = "rm -rf tmp/" + task->docFilename;
      r = system(command_string.c_str());
      if (r != 0) {
        success = false;
      }
    }

    delete[] task->ptr;

    delete task;
    return GO_ON;
  }
  void svc_end() {
    if (!success) {
      printf("Write stage: Exiting with (some) Error(s)\n");
      return;
    }
  }

  std::map<std::string, size_t> docs;
  bool success = true;
};

static inline void usage(const char *argv0) {
  printf("--------------------\n");
  printf("Usage: %s nworkers file-or-directory [file-or-directory]\n", argv0);
  printf("\nModes: COMPRESS ONLY\n");
  printf("--------------------\n");
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    usage(argv[0]);
    return -1;
  }
  argc--;

  const size_t nworkers = std::stol(argv[1]);
  argc--;

  const size_t threshold = std::stol(argv[2]);
  argc--;

  Read reader(const_cast<const char **>(&argv[3]), argc, threshold);
  Write writer;
  //! Farm creation
  ff_farm farm;
  std::vector < ff_node * > W;
  for (size_t i = 0; i < nworkers; ++i)
    W.push_back(new Compressor);
  farm.add_workers(W);
  farm.add_collector(nullptr);
  farm.cleanup_workers();

  ff_pipeline pipe;
  pipe.add_stage(reader);
  pipe.add_stage(farm);
  pipe.add_stage(writer);

  ff::ffTime(ff::START_TIME);

  if (pipe.run_and_wait_end() < 0) {
    error("running pipeline\n");
    return -1;
  }
  ff::ffTime(ff::STOP_TIME);
  std::cout << " time: " << ff::ffTime(ff::GET_TIME) << "\n";

  bool success = true;
  success &= reader.success;
  //success &= farm.success;
  success &= writer.success;
  if (success) printf("Done.\n");

  return 0;
}

