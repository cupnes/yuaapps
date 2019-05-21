#include <lib.h>

#define SLIDE_FILE_NAME	"s.yuaupdate01"
#define SLIDE_WAIT_SEC	5
#define SLIDESHOW_MAX_IMAGES	100

static void slideshow_load_slidefile(struct file *slidefile);
static void init(void);
static void start(void);

struct image *slide_images[SLIDESHOW_MAX_IMAGES];
unsigned int num_slide_images;
unsigned int slide_idx;

int main(void)
{
	init();
	start();
	return 0;
}

static void slideshow_load_slidefile(struct file *slidefile)
{
	struct textfile text = {0, slidefile};
	char buf[FILE_NAME_LEN];
	num_slide_images = 0;
	while (file_read_line(buf, FILE_NAME_LEN, &text)) {
		struct file *f = open(buf);
		slide_images[num_slide_images++] = (struct image *)f->data;
	}
}

static void init(void)
{
	slideshow_load_slidefile(open(SLIDE_FILE_NAME));
	slide_idx = 0;
}

static void start(void)
{
	while (1) {
		image_viewer(slide_images[slide_idx++]);
		if (slide_idx >= num_slide_images)
			slide_idx = 0;
		sleep(SLIDE_WAIT_SEC * 1000000);
	}
}
