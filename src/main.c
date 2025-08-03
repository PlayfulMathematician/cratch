#include "miniz/miniz.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
  #include <direct.h>
  #define MKDIR(path) _mkdir(path)
  #define PATH_SEP '\\'
#else
#endif

unsigned long djb2_hash_data(const unsigned char *data, size_t len) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

int mkdir_if_not_exists(const char *dir_path) {
    MKDIR(dir_path);
    return 0;
}

int get_cratch_appdata_path(char *out, size_t out_sz) {
#ifdef _WIN32
    const char *appdata = getenv("APPDATA");
    snprintf(out, out_sz, "%s\\Cratch", appdata);
#else
#endif
    return 0;
}

void build_output_path(const char *base, const char *filename, char *out, size_t out_sz) {
    size_t blen = strlen(base);
    strcpy(out, base);
    if (blen > 0 && base[blen - 1] != PATH_SEP) {
        out[blen] = PATH_SEP;
        out[blen + 1] = '\0';
    }
    strcat(out, filename);
}

void unzip_sb3_to_cratch(const char *zip_path) {
    FILE *f = fopen(zip_path, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *file_data = malloc(file_size);
    if (!file_data) {
        fclose(f);
        return;
    }
    fread(file_data, 1, file_size, f);
    fclose(f);

    unsigned long hash = djb2_hash_data(file_data, file_size);
    free(file_data);

    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    if (!mz_zip_reader_init_file(&zip_archive, zip_path, 0)) {
        return;
    }

    char base_dir[1024];
    get_cratch_appdata_path(base_dir, sizeof(base_dir));

    char hash_dir[64];
    snprintf(hash_dir, sizeof(hash_dir), "%08lx", hash);

    char dest_root[1100];
    snprintf(dest_root, sizeof(dest_root), "%s%c%s", base_dir, PATH_SEP, hash_dir);

    mkdir_if_not_exists(base_dir);
    mkdir_if_not_exists(dest_root);

    int num_files = (int)mz_zip_reader_get_num_files(&zip_archive);

    for (int i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) continue;

        if (file_stat.m_is_directory) continue;

        char out_path[2048];
        build_output_path(dest_root, file_stat.m_filename, out_path, sizeof(out_path));
        mz_zip_reader_extract_to_file(&zip_archive, i, out_path, 0);
    }

    mz_zip_reader_end(&zip_archive);
}

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    unzip_sb3_to_cratch(argv[1]);
    return 0;
}


