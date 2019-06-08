#include <lib.h>

#define IMAGE_LIST_FILE	"s.darius2"
#define FPUS	41666

int main(void)
{
	set_bg(0, 0, 0);
	clear_screen();

	struct file *img_list = open(IMAGE_LIST_FILE);
	while (1)
		anime_viewer(img_list, 0, 115, FPUS);

	return 0;
}
