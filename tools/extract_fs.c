#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_ARGS	3

#define FS_FILE_NAME_LEN	28
#define FILE_PATH_LEN	128

static void usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s [-h] FS_IMG EXT_DIR\n", prog_name);
}

int main(int argc, char *argv[])
{
	if (argc != NUM_ARGS) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *fs_img = fopen(argv[1], "rb");
	if (fs_img == NULL) {
		perror("fopen(fs_img)");
		exit(EXIT_FAILURE);
	}

	char *ext_dir_path = argv[2];
	mkdir(ext_dir_path, S_IRWXU | S_IRWXG | S_IROTH);

	while (1) {
		char fs_file_name[FS_FILE_NAME_LEN];
		size_t res_read =
			fread(fs_file_name, 1, FS_FILE_NAME_LEN, fs_img);
		if (feof(fs_img)) {
			if (fs_file_name[0] == 0) {
				break;
			} else {
				fprintf(stderr,
					"Error: Invalid FS Termination.\n");
				exit(EXIT_FAILURE);
			}
		}
		if (res_read != FS_FILE_NAME_LEN) {
			perror("fgets(fs_file_name)");
			exit(EXIT_FAILURE);
		}
		printf("## FileName: %s\n", fs_file_name);

		unsigned int fs_file_size;
		res_read =
			fread(&fs_file_size, 1, sizeof(unsigned int), fs_img);
		if (res_read != sizeof(unsigned int)) {
			perror("fscanf(fs_file_size)");
			exit(EXIT_FAILURE);
		}
		printf("## FileSize: %d\n", fs_file_size);

		unsigned char *buf = malloc(fs_file_size);
		if (buf == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		res_read = fread(buf, 1, fs_file_size, fs_img);
		if (res_read != fs_file_size) {
			perror("fread(file)");
			exit(EXIT_FAILURE);
		}

		char file_path[FILE_PATH_LEN];
		int res_snprintf = snprintf(file_path, FILE_PATH_LEN, "%s/%s",
					    ext_dir_path, fs_file_name);
		if ((res_snprintf > FILE_PATH_LEN) || (res_snprintf < 0)) {
			perror("snprintf");
			exit(EXIT_FAILURE);
		}

		FILE *f = fopen(file_path, "wb");
		if (f == NULL) {
			perror("fopen(create file)");
			exit(EXIT_FAILURE);
		}

		size_t res_write = fwrite(buf, 1, fs_file_size, f);
		if (res_write != fs_file_size) {
			perror("fwrite(file)");
			exit(EXIT_FAILURE);
		}

		fclose(f);
		free(buf);
	}

	fclose(fs_img);
	return EXIT_SUCCESS;
}
