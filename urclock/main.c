#include <lib.h>

#ifdef RUN_QEMU
#define BASE_X	500
#define BASE_Y	25
#else
#define BASE_X	853
#define BASE_Y	32
#endif
#define CHAR_MASK_SIZE	(sizeof(struct image) + ((FONT_WIDTH * 2) * FONT_HEIGHT * sizeof(struct pixelformat)))

unsigned char char_mask_data[CHAR_MASK_SIZE];
struct image *char_mask;

static void draw(struct datetime *dt);
static void update(struct datetime *dt);

int main(void)
{
	struct file *window = open("urclockbg.bgra");

	struct datetime dt;
	get_datetime(&dt);

	draw_fg(window);
	char_mask = (struct image *)char_mask_data;
	make_mask_region(BASE_X, BASE_Y, FONT_WIDTH * 2, FONT_HEIGHT, char_mask);
	draw(&dt);

	while (1) {
		sleep(1000000);
		update(&dt);
	}

	return 0;
}

static void draw(struct datetime *dt)
{
	move_cursor(BASE_X, BASE_Y);
	putd(dt->year, 4);
	putchar('-');
	putd(dt->mon, 2);
	putchar('-');
	putd(dt->day, 2);

	move_cursor(BASE_X + FONT_WIDTH, BASE_Y + FONT_HEIGHT);
	putd(dt->hour, 2);
	putchar(':');
	putd(dt->min, 2);
	putchar(':');
	putd(dt->sec, 2);
}

static void update(struct datetime *dt)
{
	unsigned int x = BASE_X + (FONT_WIDTH * 7);
	unsigned int y = BASE_Y + FONT_HEIGHT;
	draw_image(char_mask, x, y);
	move_cursor(x, y);
	dt->sec++;
	if (dt->sec >= 60) {
		dt->sec = 0;
		putd(dt->sec, 2);
		dt->min++;
	} else {
		putd(dt->sec, 2);
		return;
	}

	x = BASE_X + (FONT_WIDTH * 4);
	draw_image(char_mask, x, y);
	move_cursor(x, y);
	if (dt->min >= 60) {
		dt->min = 0;
		putd(dt->min, 2);
		dt->hour++;
	} else {
		putd(dt->min, 2);
		return;
	}

	x = BASE_X + FONT_WIDTH;
	draw_image(char_mask, x, y);
	move_cursor(x, y);
	if (dt->hour >= 24) {
		dt->hour = 0;
		putd(dt->hour, 2);
		dt->day++;
	} else {
		putd(dt->hour, 2);
		return;
	}

	/* FIXME: update year and mon */
}
